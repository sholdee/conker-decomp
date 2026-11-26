#include <ultra64.h>
#include "structs.h"

/* Per-view screen effects: a colour fade, a colour flash, a global tint, an iris wipe and the
 * cutscene letterbox. Per-view arrays are indexed by view (0..gScreenLayout). */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} FadeColor;

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
u8 gIrisWobbleFrames[4];         // iris: wobble frames left
u8 gIrisWobbleAngles[4];         // iris: wobble angle
u8 gIrisOverlayFrames[4];         // iris: overlay animation frame
u16 gIrisHoldFrames[4];        // iris: hold frames left
u8 gTintPulseSpeed;            // tint: pulse speed (angle step per frame)
u8 gTintPulseAngle;            // tint: pulse angle
u8 gTintColorIndex;            // tint: colour pair index into gTintColors
u8 gTintAlpha;            // tint: alpha
u8 gTintPulses;            // tint: nonzero: alpha pulses
FadeColor gFlashColors[4];  // flash: colour
u8 gFlashAlphas[4];         // flash: alpha
FadeColor gFadeColors[4];  // fade: colour
s8 gFadeDirections[4];         // fade: direction (nonzero: fading to the colour)
s32 gFadeElapsed[4];        // fade: elapsed frames
u8 gFadeFixedAlphas[4];         // fade: fixed alpha that overrides the timer when nonzero
f32 gIrisClosures[4];        // iris: closure (0 open .. 1 closed)
f32 gIrisSpeeds[4];        // iris: closure change per frame
f32 gIrisCentres[4][2];     // iris: hole centre, relative to the view centre
u16 gTintFrames;           // tint: frames left
u16 gFlashFrames[4];        // flash: frames left
f32 *gLetterbox;          // letterbox amount in percent, or NULL when off
u8 gIrisStyles[4];         // iris: style
u8 gIrisScripted[4];         // iris: scripted
s32 gFadeDurations[4];        // fade: duration
u8 D_800DDE38;
u8 gIrisTracking[4];         // iris: tracking the player
u8 gCutsceneIrisOn;            // nonzero: iris driven by the cutscene tracks
f32 *gCutsceneIrisClosure;          // cutscene iris: closure, percent
f32 *gCutsceneIrisX;          // cutscene iris: hole x, percent of the view width
f32 *gCutsceneIrisY;          // cutscene iris: hole y, percent of the view height

#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define gRenderViews ((RenderView *)gViews) // matching: the no-op cast changes the code; plain gViews mismatches

/* The colour pairs of the tint: red to yellow. */
static u8 gTintColors[1][6] = { { 255, 0, 0, 255, 255, 0 } };

/* Render setup of the iris wipe (screen_drawIris). */
static Gfx gIrisSetupDl[] = {
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5),
    gsSPEndDisplayList(),
};

/* Scripted iris closures, by gScreenLayout: the hold closure, then the final closure. */
static const f32 D_800A7260[4] = { 0.8f, 0.9f, 0.9f, 0.9f };
static const f32 D_800A7270[4] = { 0.9649999738f, 0.9649999738f, 0.9649999738f, 0.9649999738f };

/* Starts a fade of `view` to (toColor) or from the colour r,g,b over `duration` frames,
 * unless that same fade has already run to completion. */
void screen_startViewFade(s32 r, s32 g, s32 b, s32 duration, s8 toColor, s32 view) {
    FadeColor *color;

    if (toColor == gFadeDirections[view] && gFadeElapsed[view] >= gFadeDurations[view] && gFadeFixedAlphas[view] == 0) {
        return;
    }

    color = &gFadeColors[view];
    color->g = g;
    color->b = b;
    color->r = r;
    gFadeDurations[view] = duration;
    gFadeElapsed[view] = 0;
    gFadeDirections[view] = toColor;
    gFadeFixedAlphas[view] = 0;
}

/* Returns the current fade alpha (0..255) of `view`. */
s32 screen_getViewFadeAlpha(s32 view) {
    s32 alpha;

    if (gFadeFixedAlphas[view] != 0) {
        alpha = gFadeFixedAlphas[view];
    } else {
        if (gFadeDurations[view] != 0) {
            alpha = (gFadeElapsed[view] * 255) / gFadeDurations[view];
            alpha = (alpha < 256) ? alpha : 255;
        } else {
            alpha = 255;
        }
        if (gFadeDirections[view] == 0) {
            alpha = 255 - alpha;
        }
    }
    return alpha;
}

/* Returns whether `view` is fully faded to its colour. */
s32 screen_isViewFadedOut(s32 view) {
    if (screen_getViewFadeAlpha(view) == 255) {
        return 1;
    }
    return 0;
}

/* Returns whether every view is at least partly covered by a fade or an iris. */
s32 screen_areViewsCovered(void) {
    s32 covered = -1;
    s32 view;

    for (view = 0; view <= gScreenLayout; view++) {
        if (screen_getViewFadeAlpha(view) > 0 || gIrisClosures[view] > 0.0f) {
            covered++;
        }
    }
    if (covered == gScreenLayout) {
        return 1;
    }
    return 0;
}

/* Fills the whole of `view` with colour r,g,b at `alpha`. */
Gfx *screen_fillView(Gfx *gfx, s32 alpha, s32 r, s32 g, s32 b, s32 view) {
    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, alpha);
    gDPSetOtherMode(gfx++, 0x002CFF, 0x504344);
    gDPFillRectangle(gfx++, gRenderViews[view].left, gRenderViews[view].top, gRenderViews[view].right,
                     gRenderViews[view].bottom);
    return gfx;
}

/* Draws the colour fade of `view`, if it is visible. */
Gfx *screen_drawViewFade(Gfx *gfx, s32 view) {
    s32 alpha = screen_getViewFadeAlpha(view);
    FadeColor *color;

    if (alpha == 0) {
        return gfx;
    }
    color = &gFadeColors[view];
    return screen_fillView(gfx, alpha, color->r, color->g, color->b, view);
}

/* Returns whether the colour fade of `view` has run its full duration. */
s32 screen_isViewFadeDone(s32 view) {
    if (gFadeElapsed[view] >= gFadeDurations[view]) {
        return 1;
    }
    return 0;
}

/* Advances the colour fade timer of `view`. */
void screen_advanceViewFade(s32 view) {
    if (gFadeElapsed[view] != gFadeDurations[view]) {
        gFadeElapsed[view] += D_800BE9E4;
    }
}

/* Flashes `view` with colour r,g,b at `alpha` for `duration` frames. */
void screen_startViewFlash(s32 r, s32 g, s32 b, s32 alpha, s32 duration, s32 view) {
    FadeColor *color = &gFlashColors[view];

    color->r = r;
    color->g = g;
    color->b = b;
    gFlashAlphas[view] = alpha;
    gFlashFrames[view] = duration;
}

/* Draws the colour flash of `view`, if one is active. */
Gfx *screen_drawViewFlash(Gfx *gfx, s32 view) {
    FadeColor *color;

    if (gFlashFrames[view] == 0) {
        return gfx;
    }
    if (gFlashAlphas[view] == 0) {
        return gfx;
    }
    color = &gFlashColors[view];
    return screen_fillView(gfx, gFlashAlphas[view], color->r, color->g, color->b, view);
}

/* Draws the full-screen tint over view 0, pulsing its alpha when enabled. */
Gfx *screen_drawTint(Gfx *gfx) {
    f32 stack[4]; // matching: the original reads an uninitialized stack slot as the fraction
    f32 fraction;
    f32 pulse;
    u8 *colors;
    s32 baseAlpha;
    s32 alpha;
    s32 r;
    s32 g;
    s32 b;

    if (gTintFrames == 0) {
        return gfx;
    }
    baseAlpha = gTintAlpha;
    if (baseAlpha == 0) {
        return gfx;
    }
    if (gTintPulses != 0) {
        pulse = (math_sinBinAngle(gTintPulseAngle) + 1.0f) * 0.5f;
        alpha = gTintAlpha * pulse;
        fraction = 0.0f;
    } else {
        alpha = baseAlpha;
        fraction = stack[0];
    }

    colors = gTintColors[gTintColorIndex];
    r = colors[0] + (colors[3] - colors[0]) * fraction;
    g = colors[1] + (colors[4] - colors[1]) * fraction;
    b = colors[2] + (colors[5] - colors[2]) * fraction;
    return screen_fillView(gfx, alpha, r, g, b, 0);
}

/* Starts the full-screen tint: colour pair `colorIndex` at `alpha` for `duration` frames,
 * pulsing at `speed` when `pulse` is set. */
void screen_startTint(s32 colorIndex, s32 duration, s32 speed, s32 alpha, s32 pulse) {
    gTintFrames = duration;
    gTintPulseSpeed = speed;
    gTintPulseAngle = 0;
    gTintColorIndex = colorIndex;
    gTintAlpha = alpha;
    gTintPulses = pulse;
}

/* Counts down the colour flash timers of every view. */
void screen_updateViewFlashes(void) {
    s32 view;

    for (view = 0; view <= gScreenLayout; view++) {
        if (D_800BE9E4 < gFlashFrames[view]) {
            gFlashFrames[view] -= D_800BE9E4;
        } else {
            gFlashFrames[view] = 0;
        }
    }
}

/* Counts down the tint timer and advances its pulse. */
void screen_updateTint(void) {
    if (gTintFrames) {
        if (D_800BE9E4 < gTintFrames) {
            gTintFrames -= D_800BE9E4;
        } else {
            gTintFrames = 0;
        }
        gTintPulseAngle += gTintPulseSpeed * D_800BE9E4;
    }
}

/* Moves the iris hole of `view` to the screen position x,y (`onScreen`) or to the projection
 * of the world point x,y,z, clamped to +-10000. */
void screen_moveIris(s32 onScreen, f32 x, f32 y, f32 z, s32 view) {
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 screenW;
    RenderView *rv;

    if (onScreen == 0) {
        math_transformPointW(gViewProjMatrices[view], x, y, z, &screenX, &screenY, &screenZ, &screenW);
        if (screenW == 0.0f) {
            screenW = 1.0f;
        }
        rv = &gRenderViews[view];
        screenX = (rv->halfWidth * screenX) / screenW;
        screenY = (rv->halfHeight * screenY) / screenW;
        screenX += rv->halfWidth;
        screenY = rv->halfHeight - screenY;
        screenX += rv->left;
        screenY += rv->top;
    } else {
        screenX = x;
        screenY = y;
        rv = &gRenderViews[view];
    }
    if (screenX > 10000.0f) {
        screenX = 10000.0f;
    } else if (screenX < -10000.0f) {
        screenX = -10000.0f;
    }
    if (screenY > 10000.0f) {
        screenY = 10000.0f;
    } else if (screenY < -10000.0f) {
        screenY = -10000.0f;
    }
    gIrisCentres[view][0] = screenX - rv->halfWidth;
    gIrisCentres[view][1] = screenY - rv->halfHeight;
}

/* Fills the rectangle (ulx,uly)-(lrx,lry), in either corner order, clipped to `view`. */
Gfx *screen_fillViewRect(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 view) {
    RenderView *rv;
    s32 left;
    s32 clipped;
    s32 top;
    s32 bottom;
    s32 right;

    if (lrx < ulx) {
        left = ulx;
        ulx = lrx;
        lrx = left;
    }
    if (lry < uly) {
        left = uly;
        uly = lry;
        lry = left;
    }
    rv = &gRenderViews[view];
    left = rv->left;
    top = rv->top;
    right = rv->right;
    bottom = rv->bottom;
    if (uly >= bottom || top >= lry || ulx >= right || left >= lrx) {
        return gfx;
    }

    clipped = (ulx < left) ? left : ((right < ulx) ? right : ulx);
    ulx = clipped;
    clipped = (uly < top) ? top : ((bottom < uly) ? bottom : uly);
    uly = clipped;
    clipped = (lrx < left) ? left : ((right < lrx) ? right : lrx);
    lrx = clipped;
    clipped = (lry < top) ? top : ((bottom < lry) ? bottom : lry);
    lry = clipped;
    gDPFillRectangle(gfx++, ulx, uly, lrx, lry);
    return gfx;
}

/* Draws a `size`-pixel texture four times around the iris hole of `view`, mirrored into each
 * quadrant and pushed out by dx,dy; `texSize` is the texture's width in texels. */
Gfx *screen_drawIrisQuadrants(Gfx *gfx, s32 size, s32 texSize, f32 dx, f32 dy, s32 view) {
    s32 tmpA;
    s32 tmpB;
    f32 centerX;
    f32 centerY;
    f32 lowerY;
    f32 leftX;
    f32 upperY;
    f32 fsize;
    s32 frac[2]; // texel fractions

    // matching: the dead stores to tmpA/tmpB and the reuse of `view` as a temporary steer
    // IDO's spill-slot assignment; the cleaner forms tried all move the stack slots
    centerX = gIrisCentres[view][0] + gRenderViews[view].halfWidth;
    centerY = gIrisCentres[view][1] + gRenderViews[view].halfHeight;
    lowerY = centerY + dy;
    fsize = size;
    tmpA = (s32)(centerX + dx) << 2;
    tmpB = (s32)((centerX + dx) * 32.0f) & 0x1F;
    gSPScisTextureRectangle(gfx++,
        tmpA, (s32)lowerY << 2,
        (s32)((centerX + dx) + fsize) << 2, (s32)(lowerY + fsize) << 2,
        G_TX_RENDERTILE,
        ((s32)((centerX + dx) * 32.0f) & 0x1F) + (texSize << 5),
        ((s32)(lowerY * 32.0f) & 0x1F) + (texSize << 5),
        -(u16)(texSize / fsize * 1024.0f), -(u16)(texSize / fsize * 1024.0f));
    upperY = centerY - dy;
    leftX = centerX - dx;
    tmpA = ((centerY - dy) - size);
    tmpB = ((centerX - dx) - size);
    frac[1] = (s32)((leftX - fsize) * 32.0f) & 0x1F;
    frac[0] = (s32)((upperY - fsize) * 32.0f) & 0x1F;
    gSPScisTextureRectangle(gfx++,
        (s32)(leftX - fsize) << 2, (s32)(upperY - fsize) << 2,
        (s32)(centerX - dx) << 2, (s32)upperY << 2,
        G_TX_RENDERTILE,
        0x1F - frac[1],
        0x1F - frac[0],
        (u16)(texSize / fsize * 1024.0f), (u16)(texSize / fsize * 1024.0f));
    upperY = (centerY - dy) - size;
    frac[1] = (s32)((centerX + dx) * 32.0f) & 0x1F;
    tmpA = (s32)(centerX + dx) << 2;
    tmpB = (s32)((centerY - dy) - fsize) << 2;
    view = (s32)(centerY - dy) << 2;
    gSPScisTextureRectangle(gfx++,
        tmpA, tmpB,
        (s32)((centerX + dx) + fsize) << 2, view,
        G_TX_RENDERTILE,
        frac[1] + (texSize << 5),
        0x1F - frac[0],
        -(u16)(texSize / fsize * 1024.0f), (u16)(texSize / fsize * 1024.0f));
    view = 0x1F - ((s32)(((centerX - dx) - fsize) * 32.0f) & 0x1F);
    tmpB = (s32)(centerX - dx) << 2;
    tmpA = (s32)((centerX - dx) - size) << 2;
    gSPScisTextureRectangle(gfx++,
        tmpA, (s32)lowerY << 2,
        tmpB, (s32)(lowerY + fsize) << 2,
        G_TX_RENDERTILE,
        view,
        ((s32)(lowerY * 32.0f) & 0x1F) + (texSize << 5),
        (u16)(texSize / fsize * 1024.0f), -(u16)(texSize / fsize * 1024.0f));
    return gfx;
}

/* Draws the iris wipe of `view`: black around a round hole, with optional sight marks and overlay. */
Gfx *screen_drawIris(Gfx *gfx, s32 view) {
    f32 closure;

    if (gCutsceneIrisOn != 0) {
        if (gCutsceneStates[0] == 1) {
            f32 scale;

            closure = *gCutsceneIrisClosure * 0.01f;
            if (closure > 1.0f) {
                closure = 1.0f;
            } else if (closure < 0.0f) {
                closure = 0.0f;
            }
            scale = *gCutsceneIrisX * 0.01f;
            gIrisCentres[view][0] = gRenderViews->width * scale;
            scale = -(*gCutsceneIrisY * 0.01f);
            gIrisCentres[view][1] = gRenderViews->height * scale;
        } else {
            closure = 1.0f;
        }
    } else {
        closure = gIrisClosures[view];
    }
    if (closure == 0.0f) {
        return gfx;
    }
    gSPDisplayList(gfx++, gIrisSetupDl);
    gDPPipeSync(gfx++);
    gDPSetFillColor(gfx++, 0x10001);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
    gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetOtherMode(gfx++, 0x000CFF, 0x0F0A4004);
    gSPClearGeometryMode(gfx++, G_ZBUFFER | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    if (closure < 1.0f) {
        s32 i;
        s16 ulx;
        s16 uly;
        s16 lrx;
        s16 lry;
        s16 size; // hole radius
        s16 top;
        s16 left;
        s16 bottom;
        s16 right;

        top = gRenderViews[view].top;
        left = gRenderViews[view].left;
        bottom = gRenderViews[view].bottom;
        right = gRenderViews[view].right;
        size = (-399.0f * closure + 400.0f);
        // black bands above, below, left and right of the hole
        uly = top;
        ulx = left;
        lry = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) - size);
        lrx = right;
        gfx = screen_fillViewRect(gfx, ulx, uly, lrx, lry, view);
        uly = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) + size);
        ulx = left;
        lry = bottom;
        lrx = right;
        gfx = screen_fillViewRect(gfx, ulx, uly, lrx, lry, view);
        uly = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) - size);
        ulx = left;
        lry = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) + size);
        lrx = ((gIrisCentres[view][0] + gRenderViews[view].halfWidth) - size);
        gfx = screen_fillViewRect(gfx, ulx, uly, lrx, lry, view);
        uly = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) - size);
        ulx = ((gIrisCentres[view][0] + gRenderViews[view].halfWidth) + size);
        lry = ((gIrisCentres[view][1] + gRenderViews[view].halfHeight) + size);
        lrx = right;
        gfx = screen_fillViewRect(gfx, ulx, uly, lrx, lry, view);
        if (gIrisStyles[view] != 0) {
            gDPPipeSync(gfx++);
            gDPSetOtherMode(gfx++, 0x002C0F, 0x0F0A4004);
            gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
            // crosshair (and corner marks), first as a 1-pixel-wider outline, then on top
            for (i = 0; i < 2; i++) {
                uly = (gRenderViews[view].halfHeight - 1);
                lry = (gRenderViews[view].halfHeight + 1);
                gfx = screen_fillViewRect(gfx, left - i, uly - i, right + i, lry + i, view);
                ulx = (gRenderViews[view].halfWidth - 1);
                lrx = (gRenderViews[view].halfWidth + 1);
                gfx = screen_fillViewRect(gfx, ulx - i, top - i, lrx + i, bottom + i, view);
                if (gIrisStyles[view] == 1) {
                    s32 center[2];

                    center[0] = gIrisCentres[view][0] + gRenderViews[view].halfWidth;
                    center[1] = gIrisCentres[view][1] + gRenderViews[view].halfHeight;
                    uly = center[1] - 60;
                    ulx = center[0] - 60;
                    lry = center[1] - 58;
                    lrx = center[0] - 30;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 60;
                    ulx = center[0] - 60;
                    lry = center[1] - 30;
                    lrx = center[0] - 58;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 30;
                    ulx = center[0] + 58;
                    lry = center[1] - 60;
                    lrx = center[0] + 60;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 58;
                    ulx = center[0] + 30;
                    lry = center[1] - 60;
                    lrx = center[0] + 60;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] + 60;
                    lry = center[1] + 30;
                    lrx = center[0] + 58;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] + 60;
                    lry = center[1] + 58;
                    lrx = center[0] + 30;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] - 60;
                    lry = center[1] + 30;
                    lrx = center[0] - 58;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] - 60;
                    lry = center[1] + 58;
                    lrx = center[0] - 30;
                    gfx = screen_fillViewRect(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                }
                if (i == 0) {
                    gDPPipeSync(gfx++);
                    gDPSetOtherMode(gfx++, 0x002C0F, 0x00504244);
                    gDPSetFillColor(gfx++, 0);
                    gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, 0);
                }
            }
        }
        gDPPipeSync(gfx++);
        gDPSetCombineLERP(gfx++, 0, 0, 0, 0, 0, 0, 0, TEXEL0, 0, 0, 0, 0, 0, 0, 0, TEXEL0);
        gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, texture_getAddress(D_80090298[1], NULL, 3, 0));
        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 6,
                   G_TX_NOLOD);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0xFE, 0xFE);
        gDPSetOtherMode(gfx++, 0x002CFF, 0x005011C4);
        // matching: integer 0 offsets; 0.0f literals change the float register allocation
        gfx = screen_drawIrisQuadrants(gfx, size, 64, 0, 0, view);
        if (gIrisStyles[view] != 0) {
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, texture_getAddress(D_80090298[2], NULL, 3, 0));
            gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
            gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP,
                       6, G_TX_NOLOD);
            gfx = screen_drawIrisQuadrants(gfx, size / 2, 64, 0, 0, view);
            if (gIrisStyles[view] == 2) {
                gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                                   texture_getAddress(D_80090298[gIrisOverlayFrames[view] + 3], NULL, 3, 0));
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD,
                           G_TX_CLAMP, 5, G_TX_NOLOD);
                gfx = screen_drawIrisQuadrants(gfx, size / 4, 32, 32.0f, 28.0f, view);
                gfx = screen_drawIrisQuadrants(gfx, size / 4, 32, 28.0f, 32.0f, view);
                if (gIrisOverlayFrames[view] < 2) {
                    gIrisOverlayFrames[view]++;
                } else {
                    gIrisOverlayFrames[view] = 0;
                }
            }
        }
    } else {
        gDPFillRectangle(gfx++, gRenderViews[view].left, gRenderViews[view].top, gRenderViews[view].right,
                         gRenderViews[view].bottom);
    }
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}


/* Updates the iris of every view: scripted irises step through the player's mode states,
 * tracking irises follow the player, and free irises run at their speed until open or closed. */
void screen_updateIrises(void) {
    s32 i;
    Actor *player;

    for (i = 0; i <= gScreenLayout; i++) {
        if (gIrisScripted[i] != 0) {

            player = &movement_obj[i];
            gIrisClosures[i] += gIrisSpeeds[i] * D_800BE9E4;
            if (player->mv == NULL) {
                gIrisClosures[i] = 1.0f;
            } else if (player->mv->death_State == 3) {
                if (D_800A7260[gScreenLayout] <= gIrisClosures[i]) {
                    gIrisClosures[i] = D_800A7260[gScreenLayout];
                    player->mv->death_State = 4;
                    gIrisWobbleFrames[i] = 30;
                    gIrisWobbleAngles[i] = 0;
                }
            } else if (player->mv->death_State == 4) {
                if (gIrisWobbleFrames[i]) {
                    f32 scale;

                    if (gIrisWobbleFrames[i] >= D_800BE9E4) {
                        gIrisWobbleFrames[i] -= D_800BE9E4;
                    } else {
                        gIrisWobbleFrames[i] = 0;
                    }
                    gIrisWobbleAngles[i] += D_800BE9E4 * 20;
                    if (gIrisWobbleFrames[i] < 30) {
                        scale = (gIrisWobbleFrames[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    gIrisClosures[i] = D_800A7260[gScreenLayout] + scale * math_sinBinAngle(gIrisWobbleAngles[i]);
                } else {
                    gIrisClosures[i] = D_800A7260[gScreenLayout];
                    player->mv->death_State = 5;
                }
            } else if (player->mv->death_State == 5) {
                gIrisClosures[i] = D_800A7260[gScreenLayout];
            } else if (player->mv->death_State == 6) {
                gIrisSpeeds[i] = 0.009f;
                if (D_800A7270[gScreenLayout] <= gIrisClosures[i]) {
                    gIrisClosures[i] = D_800A7270[gScreenLayout];
                    player->mv->death_State = 7;
                    player->mv->deathTimer = 30;
                    gIrisSpeeds[i] = 0.0f;
                }
            } else if (player->mv->death_State == 8) {
                gIrisSpeeds[i] = 0.004f;
                if (gIrisClosures[i] >= 1.0f) {
                    gIrisClosures[i] = 1.0f;
                    player->mv->death_State = 9;
                }
            }
        } else if (gIrisTracking[i] != 0) {
            Actor *actor;
            f32 x;
            f32 y;
            f32 z;
            f32 prevX;
            f32 prevY;
            f32 rate;
            f32 diff;
            f32 step;
            s32 sign;

            actor = &movement_obj[i];
            gIrisClosures[i] += gIrisSpeeds[i] * D_800BE9E4;
            if (actor->ob_matrix_ptr != NULL) {
                MtxF *model;
                f32 height;

                model = actor->ob_matrix_ptr;
                if (actor->type == 1) {
                    model += 12; // bone 12
                }
                height = actor->scale_y * 40.0f;
                math_transformPoint(*model, 0.0f, height, 0.0f, &x, &y, &z);
            } else {
                x = actor->xpos;
                y = actor->ypos;
                z = actor->zpos;
            }
            prevX = gIrisCentres[i][0];
            prevY = gIrisCentres[i][1];
            screen_moveIris(0, x, y, z, i);
            x = gIrisCentres[i][0];
            y = gIrisCentres[i][1];
            if (D_800BE9B4 != 0) {
                rate = 1.0f;
            } else {
                rate = 0.05f;
            }
            diff = x - prevX;
            step = diff;
            if (fabsf(diff) > 15.0f) {
                sign = (diff < 0) ? -1 : 1;
                step = sign * 15.0f;
                prevX = x - step;
            }
            step *= rate;
            prevX += step;
            diff = y - prevY;
            step = diff;
            if (fabsf(diff) > 11.25f) {
                sign = (diff < 0) ? -1 : 1;
                step = sign * 11.25f;
                prevY = y - step;
            }
            step *= rate;
            prevY += step;
            gIrisCentres[i][0] = prevX;
            gIrisCentres[i][1] = prevY;
            if (gIrisTracking[i] == 1) {
                if (gIrisClosures[i] <= 0.9f) {
                    gIrisWobbleFrames[i] = 30;
                    gIrisClosures[i] = 0.9f;
                    gIrisSpeeds[i] = 0.0f;
                    gIrisTracking[i] = 2;
                }
            } else if (gIrisTracking[i] == 2) {
                if (gIrisWobbleFrames[i]) {
                    f32 scale;

                    if (gIrisWobbleFrames[i] >= D_800BE9E4) {
                        gIrisWobbleFrames[i] -= D_800BE9E4;
                    } else {
                        gIrisWobbleFrames[i] = 0;
                    }
                    gIrisWobbleAngles[i] -= D_800BE9E4 * 20;
                    if (gIrisWobbleFrames[i] < 30) {
                        scale = (gIrisWobbleFrames[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    gIrisClosures[i] = math_sinBinAngle(gIrisWobbleAngles[i]) * scale + 0.9f;
                } else {
                    gIrisClosures[i] = 0.9f;
                    gIrisTracking[i] = 3;
                    gIrisHoldFrames[i] = 60;
                }
            } else if (gIrisTracking[i] == 3) {
                if (D_800BE9E4 < gIrisHoldFrames[i]) {
                    gIrisHoldFrames[i] -= D_800BE9E4;
                } else {
                    gIrisTracking[i] = 4;
                    gIrisSpeeds[i] = -0.009f;
                }
            } else if (gIrisClosures[i] <= 0.0f) {
                gIrisTracking[i] = 0;
                gIrisClosures[i] = 0.0f;
            }
        } else if (gIrisSpeeds[i] != 0.0f) {
            f32 speed;

            speed = gIrisSpeeds[i];
            if (speed > 0.0f) {
                if (gIrisClosures[i] >= 1.0f) {
                    gIrisSpeeds[i] = 0.0f;
                    gIrisClosures[i] = 1.0f;
                }
            } else if (gIrisClosures[i] <= 0.0f) {
                gIrisSpeeds[i] = 0.0f;
                gIrisClosures[i] = 0.0f;
            }
            gIrisClosures[i] += gIrisSpeeds[i] * D_800BE9E4;
        }
    }
}


/* Returns whether the iris of `view` is fully open. */
s32 screen_isIrisOpen(s32 view) {
    if (gIrisClosures[view] == 0.0f) {
        return 1;
    }
    return 0;
}

/* Sets the iris style of `view`; any style but 0 also closes it to 0.73 around the view centre. */
void screen_setIrisStyle(s32 view, s32 style) {
    if (style == 0) {
        gIrisClosures[view] = 0.0f;
    } else {
        gIrisSpeeds[view] = 0.0f;
        gIrisClosures[view] = 0.7300000191f;
        gIrisCentres[view][0] = 0.0f;
        gIrisCentres[view][1] = 0.0f;
    }
    gIrisStyles[view] = style;
}

/* Starts closing the scripted iris of `view` from fully open. */
void screen_closeIris(s32 view) {
    gIrisSpeeds[view] = 0.009999999776f;
    gIrisClosures[view] = 0.0f;
    gIrisCentres[view][0] = 0.0f;
    gIrisCentres[view][1] = 0.0f;
    gIrisScripted[view] = 1;
}

/* Opens and stops the scripted iris of `view`. */
void screen_openIris(s32 view) {
    gIrisSpeeds[view] = 0; // matching: an integer 0 gets its own mtc1

    gIrisClosures[view] = 0.0f;
    gIrisCentres[view][0] = 0.0f;
    gIrisCentres[view][1] = 0.0f;
    gIrisScripted[view] = 0;
}

/* Starts the opening effect of `view`: a fade from black when gIsMultiplayer is set, otherwise an
 * iris that tracks the player while it opens. */
void screen_startOpening(s32 view) {
    FadeColor *color;

    if (gIsMultiplayer != 0) {
        color = &gFadeColors[view];
        color->r = 0;
        color->g = 0;
        color->b = 0;
        gFadeDurations[view] = 25;
        gFadeElapsed[view] = 0;
        gFadeDirections[view] = 0;
        gFadeFixedAlphas[view] = 0;
    } else {
        gIrisTracking[view] = 1;
        gIrisClosures[view] = 1.0f;
        gIrisCentres[view][0] = 0.0f;
        gIrisCentres[view][1] = 0.0f;
        gIrisSpeeds[view] = -0.00400000019f;
    }
}

// matching: 0.7f read from rodata; a literal would let IDO fold 1.0f - 0.7f
extern f32 D_800A72C0;

/* Draws the cutscene letterbox over view 0: black bars that slide in from the top and bottom by
 * *gLetterbox percent, with soft textured inner edges. */
Gfx *screen_drawLetterbox(Gfx *gfx) {
    s16 yl;
    s16 yh;
    s32 pad; // matching: unused stack slot
    s16 top;
    s16 left;
    s16 bottom;
    s16 right;
    s32 topBarEnd;
    s32 bottomBarStart;
    s32 xl;
    s32 xh;
    s32 dsdx;
    s32 dtdy;
    f32 travel;
    f32 amount;

    if (gLetterbox == NULL) {
        return gfx;
    }
    amount = *gLetterbox;
    amount *= 0.009999999776f;
    // matching: an integer 1 (not 1.0f) in the clamp keeps 1.0f out of a register
    amount = (amount < 0.0f) ? 0.0f : ((amount > 1.0f) ? 1 : amount);
    if (amount == 0.0f) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetFillColor(gfx++, 0x10001);
    gSPClearGeometryMode(gfx++, G_ZBUFFER | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gDPSetOtherMode(gfx++, 0x302C0F, 4);
    top = gRenderViews->top;
    left = gRenderViews->left;
    bottom = gRenderViews->bottom;
    right = gRenderViews->right;
    travel = gRenderViews->height * 0.6999999881f;
    travel += 30.0f;
    travel *= amount;
    topBarEnd = (gRenderViews->top - 30.0f) + travel;
    gfx = screen_fillViewRect(gfx, left, top, right, (s16)topBarEnd, 0);
    travel = gRenderViews->height * (1.0f - D_800A72C0);
    travel += 30.0f;
    travel *= amount;
    bottomBarStart = (gRenderViews->bottom + 30.0f) - travel;
    gfx = screen_fillViewRect(gfx, left, (s16)bottomBarStart, right, bottom, 0);

    gDPPipeSync(gfx++);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetOtherMode(gfx++, 0x002CFF, 0x5011C4);
    gDPSetCombineLERP(gfx++, 0, 0, 0, 0, 0, 0, 0, TEXEL0, 0, 0, 0, 0, 0, 0, 0, TEXEL0);
    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, texture_getAddress(gBarTopEdgeTexId, NULL, 3, 0));
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 7, G_TX_NOLOD, G_TX_CLAMP, 7,
               G_TX_NOLOD);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0x1FE, 0xFE);
    dtdy = 0x222;
    dsdx = (128.0f / gRenderViews->width) * 1024.0f;
    yl = topBarEnd;
    yh = topBarEnd + 30;
    xl = left;
    xh = right;
    gSPScisTextureRectangle(gfx++, xl << 2, yl << 2, xh << 2, yh << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);

    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, texture_getAddress(gBarBottomEdgeTexId, NULL, 3, 0));
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 7, G_TX_NOLOD, G_TX_CLAMP, 7,
               G_TX_NOLOD);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0x1FE, 0xFE);
    yl = bottomBarStart - 30;
    yh = bottomBarStart;
    gSPScisTextureRectangle(gfx++, xl << 2, yl << 2, xh << 2, yh << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}

void *memcpy(void *, const void *, u32);

/* Spawns an effect that flashes `view` with colour r,g,b, fading out from `alpha` over
 * `duration` frames. */
void screen_spawnFlash(r, g, b, alpha, duration, view, owner, kind)
    u8 r;
    u8 g;
    u8 b;
    u8 alpha;
    volatile s16 duration; // matching: the original reloads duration from its stack slot
    u8 view;
    u8 owner;
    s32 kind;
{
    s32 pad; // matching: unused stack slot
    FlashData data;
    FlashEffect *effect;

    if (duration > 0) {
        data.r = r;
        data.g = g;
        data.b = b;
        data.alpha = alpha;
        data.view = view;
        data.alphaStep = alpha / duration;

        effect = effect_newTimedEffect(duration, -1, 0x39, 3, 1, 0, sizeof(FlashData), owner, kind);
        if (effect != NULL) {
            memcpy(&effect->data, &data, sizeof(FlashData));
        }
    }
}

/* Effect update: the flash alpha falls with the remaining time. */
void screen_updateFlash(FlashEffect *effect) {
    effect->data.alpha = effect->data.alphaStep * effect->base.header.state;
}

/* Effect draw: fills the flash's view. */
Gfx *screen_drawFlash(Gfx *gfx, FlashEffect *effect, s16 view) {
    FlashData *data = &effect->data;

    if (view == data->view) {
        gfx = screen_fillView(gfx, data->alpha, data->r, data->g, data->b, data->view);
    }
    return gfx;
}
