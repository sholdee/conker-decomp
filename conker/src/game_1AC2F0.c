#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define gRenderViews ((RenderView *)D_800BE628) // matching: the no-op cast changes the code; plain D_800BE628 mismatches

/* Per-view screen effects: a colour fade, a colour flash, a global tint, an iris wipe and the
 * cutscene letterbox. Per-view arrays are indexed by view (0..D_80082FA0). */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} FadeColor;

/* Colour fade (func_1517EE40) */
extern FadeColor D_800DDDA0[]; // colour
extern u8 D_800DDDC0[];        // fixed alpha that overrides the timer when nonzero

/* Colour flash (func_1517F488) */
extern FadeColor D_800DDD90[];
extern u8 D_800DDD9C[];  // alpha

/* Pulsing full-screen tint (func_1517F720), view 0 only */
extern u8 D_800DDD88;  // pulse speed (angle step per frame)
extern u8 D_800DDD89;  // pulse angle
extern u8 D_800DDD8A;  // colour pair index into D_8008D010
extern u8 D_800DDD8B;  // alpha
extern u8 D_800DDD8C;  // nonzero: alpha pulses

/* Iris wipe */
extern f32 D_800DDDE8[][2];  // hole centre, relative to the view centre
extern u8 D_800DDD70[];      // wobble frames left
extern u8 D_800DDD74[];      // wobble angle
extern u8 D_800DDD78[];      // overlay animation frame
extern u16 D_800DDD80[];     // hold frames left

/* The colour pairs of the tint: red to yellow. */
static u8 D_8008D010[1][6] = { { 255, 0, 0, 255, 255, 0 } };

/* Render setup of the iris wipe (func_15180580). */
static Gfx D_8008D018[] = {
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    // gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5) as Rare's gbi.h encoded it: it
    // shifted k2's high bits left (_SHIFTL(k2, 5, 4)) where 2.0L shifts them right
    {{ 0xEC15FDF0, 0x3B78E42A }},
    gsSPEndDisplayList(),
};

/* Scripted iris closures, by D_80082FA0: the hold closure, then the final closure. */
static const f32 D_800A7260[4] = { 0.8f, 0.9f, 0.9f, 0.9f };
static const f32 D_800A7270[4] = { 0.9649999738f, 0.9649999738f, 0.9649999738f, 0.9649999738f };

/* Starts a fade of `view` to (toColor) or from the colour r,g,b over `duration` frames,
 * unless that same fade has already run to completion. */
void func_1517EE40(s32 r, s32 g, s32 b, s32 duration, s8 toColor, s32 view) {
    FadeColor *color;

    if (toColor == D_800DDDAC[view] && D_800DDDB0[view] >= D_800DDE28[view] && D_800DDDC0[view] == 0) {
        return;
    }

    color = &D_800DDDA0[view];
    color->g = g;
    color->b = b;
    color->r = r;
    D_800DDE28[view] = duration;
    D_800DDDB0[view] = 0;
    D_800DDDAC[view] = toColor;
    D_800DDDC0[view] = 0;
}

/* Returns the current fade alpha (0..255) of `view`. */
s32 func_1517EF00(s32 view) {
    s32 alpha;

    if (D_800DDDC0[view] != 0) {
        alpha = D_800DDDC0[view];
    } else {
        if (D_800DDE28[view] != 0) {
            alpha = (D_800DDDB0[view] * 255) / D_800DDE28[view];
            alpha = (alpha < 256) ? alpha : 255;
        } else {
            alpha = 255;
        }
        if (D_800DDDAC[view] == 0) {
            alpha = 255 - alpha;
        }
    }
    return alpha;
}

/* Returns whether `view` is fully faded to its colour. */
s32 func_1517EFAC(s32 view) {
    if (func_1517EF00(view) == 255) {
        return 1;
    }
    return 0;
}

/* Returns whether every view is at least partly covered by a fade or an iris. */
s32 func_1517EFDC(void) {
    s32 covered = -1;
    s32 view;

    for (view = 0; view <= D_80082FA0; view++) {
        if (func_1517EF00(view) > 0 || D_800DDDC8[view] > 0.0f) {
            covered++;
        }
    }
    if (covered == D_80082FA0) {
        return 1;
    }
    return 0;
}

/* Fills the whole of `view` with colour r,g,b at `alpha`. */
Gfx *func_1517F08C(Gfx *gfx, s32 alpha, s32 r, s32 g, s32 b, s32 view) {
    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(gfx++, 0, 0, r, g, b, alpha);
    gDPSetOtherMode(gfx++, 0x002CFF, 0x504344);
    gDPFillRectangle(gfx++, gRenderViews[view].left, gRenderViews[view].top, gRenderViews[view].right,
                     gRenderViews[view].bottom);
    return gfx;
}

/* Draws the colour fade of `view`, if it is visible. */
Gfx *func_1517F3A0(Gfx *gfx, s32 view) {
    s32 alpha = func_1517EF00(view);
    FadeColor *color;

    if (alpha == 0) {
        return gfx;
    }
    color = &D_800DDDA0[view];
    return func_1517F08C(gfx, alpha, color->r, color->g, color->b, view);
}

/* Returns whether the colour fade of `view` has run its full duration. */
s32 func_1517F40C(s32 view) {
    if (D_800DDDB0[view] >= D_800DDE28[view]) {
        return 1;
    }
    return 0;
}

/* Advances the colour fade timer of `view`. */
void func_1517F448(s32 view) {
    if (D_800DDDB0[view] != D_800DDE28[view]) {
        D_800DDDB0[view] += D_800BE9E4;
    }
}

/* Flashes `view` with colour r,g,b at `alpha` for `duration` frames. */
void func_1517F488(s32 r, s32 g, s32 b, s32 alpha, s32 duration, s32 view) {
    FadeColor *color = &D_800DDD90[view];

    color->r = r;
    color->g = g;
    color->b = b;
    D_800DDD9C[view] = alpha;
    D_800DDE10[view] = duration;
}

/* Draws the colour flash of `view`, if one is active. */
Gfx *func_1517F4D8(Gfx *gfx, s32 view) {
    FadeColor *color;

    if (D_800DDE10[view] == 0) {
        return gfx;
    }
    if (D_800DDD9C[view] == 0) {
        return gfx;
    }
    color = &D_800DDD90[view];
    return func_1517F08C(gfx, D_800DDD9C[view], color->r, color->g, color->b, view);
}

/* Draws the full-screen tint over view 0, pulsing its alpha when enabled. */
Gfx *func_1517F564(Gfx *gfx) {
    f32 stack[4]; // matching: the original reads an uninitialized stack slot as the fraction
    f32 fraction;
    f32 pulse;
    u8 *colors;
    s32 baseAlpha;
    s32 alpha;
    s32 r;
    s32 g;
    s32 b;

    if (D_800DDE08 == 0) {
        return gfx;
    }
    baseAlpha = D_800DDD8B;
    if (baseAlpha == 0) {
        return gfx;
    }
    if (D_800DDD8C != 0) {
        pulse = (func_15048A40(D_800DDD89) + 1.0f) * 0.5f;
        alpha = D_800DDD8B * pulse;
        fraction = 0.0f;
    } else {
        alpha = baseAlpha;
        fraction = stack[0];
    }

    colors = D_8008D010[D_800DDD8A];
    r = colors[0] + (colors[3] - colors[0]) * fraction;
    g = colors[1] + (colors[4] - colors[1]) * fraction;
    b = colors[2] + (colors[5] - colors[2]) * fraction;
    return func_1517F08C(gfx, alpha, r, g, b, 0);
}

/* Starts the full-screen tint: colour pair `colorIndex` at `alpha` for `duration` frames,
 * pulsing at `speed` when `pulse` is set. */
void func_1517F720(s32 colorIndex, s32 duration, s32 speed, s32 alpha, s32 pulse) {
    D_800DDE08 = duration;
    D_800DDD88 = speed;
    D_800DDD89 = 0;
    D_800DDD8A = colorIndex;
    D_800DDD8B = alpha;
    D_800DDD8C = pulse;
}

/* Counts down the colour flash timers of every view. */
void func_1517F75C(void) {
    s32 view;

    for (view = 0; view <= D_80082FA0; view++) {
        if (D_800BE9E4 < D_800DDE10[view]) {
            D_800DDE10[view] -= D_800BE9E4;
        } else {
            D_800DDE10[view] = 0;
        }
    }
}

/* Counts down the tint timer and advances its pulse. */
void func_1517F7B4(void) {
    if (D_800DDE08) {
        if (D_800BE9E4 < D_800DDE08) {
            D_800DDE08 -= D_800BE9E4;
        } else {
            D_800DDE08 = 0;
        }
        D_800DDD89 += D_800DDD88 * D_800BE9E4;
    }
}

/* Moves the iris hole of `view` to the screen position x,y (`onScreen`) or to the projection
 * of the world point x,y,z, clamped to +-10000. */
void func_1517F814(s32 onScreen, f32 x, f32 y, f32 z, s32 view) {
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 screenW;
    RenderView *rv;

    if (onScreen == 0) {
        func_150A7A00(D_800D9D10[view], x, y, z, &screenX, &screenY, &screenZ, &screenW);
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
    D_800DDDE8[view][0] = screenX - rv->halfWidth;
    D_800DDDE8[view][1] = screenY - rv->halfHeight;
}

/* Fills the rectangle (ulx,uly)-(lrx,lry), in either corner order, clipped to `view`. */
Gfx *func_1517F9F4(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 view) {
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
Gfx *func_1517FB9C(Gfx *gfx, s32 size, s32 texSize, f32 dx, f32 dy, s32 view) {
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
    centerX = D_800DDDE8[view][0] + gRenderViews[view].halfWidth;
    centerY = D_800DDDE8[view][1] + gRenderViews[view].halfHeight;
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
Gfx *func_15180580(Gfx *gfx, s32 view) {
    f32 closure;

    if (D_800DDE40 != 0) {
        if (D_800C35EA[0] == 1) {
            f32 scale;

            closure = *D_800DDE44 * 0.01f;
            if (closure > 1.0f) {
                closure = 1.0f;
            } else if (closure < 0.0f) {
                closure = 0.0f;
            }
            scale = *D_800DDE48 * 0.01f;
            D_800DDDE8[view][0] = gRenderViews->width * scale;
            scale = -(*D_800DDE4C * 0.01f);
            D_800DDDE8[view][1] = gRenderViews->height * scale;
        } else {
            closure = 1.0f;
        }
    } else {
        closure = D_800DDDC8[view];
    }
    if (closure == 0.0f) {
        return gfx;
    }
    gSPDisplayList(gfx++, D_8008D018);
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
        lry = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) - size);
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, view);
        uly = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) + size);
        ulx = left;
        lry = bottom;
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, view);
        uly = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) - size);
        ulx = left;
        lry = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) + size);
        lrx = ((D_800DDDE8[view][0] + gRenderViews[view].halfWidth) - size);
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, view);
        uly = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) - size);
        ulx = ((D_800DDDE8[view][0] + gRenderViews[view].halfWidth) + size);
        lry = ((D_800DDDE8[view][1] + gRenderViews[view].halfHeight) + size);
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, view);
        if (D_800DDE1C[view] != 0) {
            gDPPipeSync(gfx++);
            gDPSetOtherMode(gfx++, 0x002C0F, 0x0F0A4004);
            gDPSetCombineMode(gfx++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 255);
            // crosshair (and corner marks), first as a 1-pixel-wider outline, then on top
            for (i = 0; i < 2; i++) {
                uly = (gRenderViews[view].halfHeight - 1);
                lry = (gRenderViews[view].halfHeight + 1);
                gfx = func_1517F9F4(gfx, left - i, uly - i, right + i, lry + i, view);
                ulx = (gRenderViews[view].halfWidth - 1);
                lrx = (gRenderViews[view].halfWidth + 1);
                gfx = func_1517F9F4(gfx, ulx - i, top - i, lrx + i, bottom + i, view);
                if (D_800DDE1C[view] == 1) {
                    s32 center[2];

                    center[0] = D_800DDDE8[view][0] + gRenderViews[view].halfWidth;
                    center[1] = D_800DDDE8[view][1] + gRenderViews[view].halfHeight;
                    uly = center[1] - 60;
                    ulx = center[0] - 60;
                    lry = center[1] - 58;
                    lrx = center[0] - 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 60;
                    ulx = center[0] - 60;
                    lry = center[1] - 30;
                    lrx = center[0] - 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 30;
                    ulx = center[0] + 58;
                    lry = center[1] - 60;
                    lrx = center[0] + 60;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] - 58;
                    ulx = center[0] + 30;
                    lry = center[1] - 60;
                    lrx = center[0] + 60;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] + 60;
                    lry = center[1] + 30;
                    lrx = center[0] + 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] + 60;
                    lry = center[1] + 58;
                    lrx = center[0] + 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] - 60;
                    lry = center[1] + 30;
                    lrx = center[0] - 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
                    uly = center[1] + 60;
                    ulx = center[0] - 60;
                    lry = center[1] + 58;
                    lrx = center[0] - 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, view);
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
        gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, func_1510D0EC(D_80090298[1], NULL, 3, 0));
        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP, 6,
                   G_TX_NOLOD);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0xFE, 0xFE);
        gDPSetOtherMode(gfx++, 0x002CFF, 0x005011C4);
        // matching: integer 0 offsets; 0.0f literals change the float register allocation
        gfx = func_1517FB9C(gfx, size, 64, 0, 0, view);
        if (D_800DDE1C[view] != 0) {
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, func_1510D0EC(D_80090298[2], NULL, 3, 0));
            gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
            gDPSetTile(gfx++, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_CLAMP,
                       6, G_TX_NOLOD);
            gfx = func_1517FB9C(gfx, size / 2, 64, 0, 0, view);
            if (D_800DDE1C[view] == 2) {
                gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                                   func_1510D0EC(D_80090298[D_800DDD78[view] + 3], NULL, 3, 0));
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD,
                           G_TX_CLAMP, 5, G_TX_NOLOD);
                gfx = func_1517FB9C(gfx, size / 4, 32, 32.0f, 28.0f, view);
                gfx = func_1517FB9C(gfx, size / 4, 32, 28.0f, 32.0f, view);
                if (D_800DDD78[view] < 2) {
                    D_800DDD78[view]++;
                } else {
                    D_800DDD78[view] = 0;
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
void func_151814FC(void) {
    s32 i;
    Actor *player;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800DDE20[i] != 0) {

            player = &movement_obj[i];
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
            if (player->mv == NULL) {
                D_800DDDC8[i] = 1.0f;
            } else if (player->mv->death_State == 3) {
                if (D_800A7260[D_80082FA0] <= D_800DDDC8[i]) {
                    D_800DDDC8[i] = D_800A7260[D_80082FA0];
                    player->mv->death_State = 4;
                    D_800DDD70[i] = 30;
                    D_800DDD74[i] = 0;
                }
            } else if (player->mv->death_State == 4) {
                if (D_800DDD70[i]) {
                    f32 scale;

                    if (D_800DDD70[i] >= D_800BE9E4) {
                        D_800DDD70[i] -= D_800BE9E4;
                    } else {
                        D_800DDD70[i] = 0;
                    }
                    D_800DDD74[i] += D_800BE9E4 * 20;
                    if (D_800DDD70[i] < 30) {
                        scale = (D_800DDD70[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    D_800DDDC8[i] = D_800A7260[D_80082FA0] + scale * func_15048A40(D_800DDD74[i]);
                } else {
                    D_800DDDC8[i] = D_800A7260[D_80082FA0];
                    player->mv->death_State = 5;
                }
            } else if (player->mv->death_State == 5) {
                D_800DDDC8[i] = D_800A7260[D_80082FA0];
            } else if (player->mv->death_State == 6) {
                D_800DDDD8[i] = 0.009f;
                if (D_800A7270[D_80082FA0] <= D_800DDDC8[i]) {
                    D_800DDDC8[i] = D_800A7270[D_80082FA0];
                    player->mv->death_State = 7;
                    player->mv->deathTimer = 30;
                    D_800DDDD8[i] = 0.0f;
                }
            } else if (player->mv->death_State == 8) {
                D_800DDDD8[i] = 0.004f;
                if (D_800DDDC8[i] >= 1.0f) {
                    D_800DDDC8[i] = 1.0f;
                    player->mv->death_State = 9;
                }
            }
        } else if (D_800DDE3C[i] != 0) {
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
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
            if (actor->ob_matrix_ptr != NULL) {
                MtxF *model;
                f32 height;

                model = actor->ob_matrix_ptr;
                if (actor->type == 1) {
                    model += 12; // bone 12
                }
                height = actor->scale_y * 40.0f;
                func_150A7960(*model, 0.0f, height, 0.0f, &x, &y, &z);
            } else {
                x = actor->xpos;
                y = actor->ypos;
                z = actor->zpos;
            }
            prevX = D_800DDDE8[i][0];
            prevY = D_800DDDE8[i][1];
            func_1517F814(0, x, y, z, i);
            x = D_800DDDE8[i][0];
            y = D_800DDDE8[i][1];
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
            D_800DDDE8[i][0] = prevX;
            D_800DDDE8[i][1] = prevY;
            if (D_800DDE3C[i] == 1) {
                if (D_800DDDC8[i] <= 0.9f) {
                    D_800DDD70[i] = 30;
                    D_800DDDC8[i] = 0.9f;
                    D_800DDDD8[i] = 0.0f;
                    D_800DDE3C[i] = 2;
                }
            } else if (D_800DDE3C[i] == 2) {
                if (D_800DDD70[i]) {
                    f32 scale;

                    if (D_800DDD70[i] >= D_800BE9E4) {
                        D_800DDD70[i] -= D_800BE9E4;
                    } else {
                        D_800DDD70[i] = 0;
                    }
                    D_800DDD74[i] -= D_800BE9E4 * 20;
                    if (D_800DDD70[i] < 30) {
                        scale = (D_800DDD70[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    D_800DDDC8[i] = func_15048A40(D_800DDD74[i]) * scale + 0.9f;
                } else {
                    D_800DDDC8[i] = 0.9f;
                    D_800DDE3C[i] = 3;
                    D_800DDD80[i] = 60;
                }
            } else if (D_800DDE3C[i] == 3) {
                if (D_800BE9E4 < D_800DDD80[i]) {
                    D_800DDD80[i] -= D_800BE9E4;
                } else {
                    D_800DDE3C[i] = 4;
                    D_800DDDD8[i] = -0.009f;
                }
            } else if (D_800DDDC8[i] <= 0.0f) {
                D_800DDE3C[i] = 0;
                D_800DDDC8[i] = 0.0f;
            }
        } else if (D_800DDDD8[i] != 0.0f) {
            f32 speed;

            speed = D_800DDDD8[i];
            if (speed > 0.0f) {
                if (D_800DDDC8[i] >= 1.0f) {
                    D_800DDDD8[i] = 0.0f;
                    D_800DDDC8[i] = 1.0f;
                }
            } else if (D_800DDDC8[i] <= 0.0f) {
                D_800DDDD8[i] = 0.0f;
                D_800DDDC8[i] = 0.0f;
            }
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
        }
    }
}


/* Returns whether the iris of `view` is fully open. */
s32 func_15181CC8(s32 view) {
    if (D_800DDDC8[view] == 0.0f) {
        return 1;
    }
    return 0;
}

/* Sets the iris style of `view`; any style but 0 also closes it to 0.73 around the view centre. */
void func_15181D00(s32 view, s32 style) {
    if (style == 0) {
        D_800DDDC8[view] = 0.0f;
    } else {
        D_800DDDD8[view] = 0.0f;
        D_800DDDC8[view] = 0.7300000191f;
        D_800DDDE8[view][0] = 0.0f;
        D_800DDDE8[view][1] = 0.0f;
    }
    D_800DDE1C[view] = style;
}

/* Starts closing the scripted iris of `view` from fully open. */
void func_15181D70(s32 view) {
    D_800DDDD8[view] = 0.009999999776f;
    D_800DDDC8[view] = 0.0f;
    D_800DDDE8[view][0] = 0.0f;
    D_800DDDE8[view][1] = 0.0f;
    D_800DDE20[view] = 1;
}

/* Opens and stops the scripted iris of `view`. */
void func_15181DC8(s32 view) {
    D_800DDDD8[view] = 0; // matching: an integer 0 gets its own mtc1

    D_800DDDC8[view] = 0.0f;
    D_800DDDE8[view][0] = 0.0f;
    D_800DDDE8[view][1] = 0.0f;
    D_800DDE20[view] = 0;
}

/* Starts the opening effect of `view`: a fade from black when D_800BE616 is set, otherwise an
 * iris that tracks the player while it opens. */
void func_15181E18(s32 view) {
    FadeColor *color;

    if (D_800BE616 != 0) {
        color = &D_800DDDA0[view];
        color->r = 0;
        color->g = 0;
        color->b = 0;
        D_800DDE28[view] = 25;
        D_800DDDB0[view] = 0;
        D_800DDDAC[view] = 0;
        D_800DDDC0[view] = 0;
    } else {
        D_800DDE3C[view] = 1;
        D_800DDDC8[view] = 1.0f;
        D_800DDDE8[view][0] = 0.0f;
        D_800DDDE8[view][1] = 0.0f;
        D_800DDDD8[view] = -0.00400000019f;
    }
}

extern s32 D_800902C8; // texture id: top bar edge
extern s32 D_800902CC; // texture id: bottom bar edge
// matching: 0.7f read from rodata; a literal would let IDO fold 1.0f - 0.7f
extern f32 D_800A72C0;

/* Draws the cutscene letterbox over view 0: black bars that slide in from the top and bottom by
 * *D_800DDE18 percent, with soft textured inner edges. */
Gfx *func_15181EE0(Gfx *gfx) {
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

    if (D_800DDE18 == NULL) {
        return gfx;
    }
    amount = *D_800DDE18;
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
    gfx = func_1517F9F4(gfx, left, top, right, (s16)topBarEnd, 0);
    travel = gRenderViews->height * (1.0f - D_800A72C0);
    travel += 30.0f;
    travel *= amount;
    bottomBarStart = (gRenderViews->bottom + 30.0f) - travel;
    gfx = func_1517F9F4(gfx, left, (s16)bottomBarStart, right, bottom, 0);

    gDPPipeSync(gfx++);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetOtherMode(gfx++, 0x002CFF, 0x5011C4);
    gDPSetCombineLERP(gfx++, 0, 0, 0, 0, 0, 0, 0, TEXEL0, 0, 0, 0, 0, 0, 0, 0, TEXEL0);
    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, func_1510D0EC(D_800902C8, NULL, 3, 0));
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

    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, func_1510D0EC(D_800902CC, NULL, 3, 0));
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
void func_15182670(r, g, b, alpha, duration, view, owner, kind)
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

        effect = func_15149130(duration, -1, 0x39, 3, 1, 0, sizeof(FlashData), owner, kind);
        if (effect != NULL) {
            memcpy(&effect->data, &data, sizeof(FlashData));
        }
    }
}

/* Effect update: the flash alpha falls with the remaining time. */
void func_15182748(FlashEffect *effect) {
    effect->data.alpha = effect->data.alphaStep * effect->base.header.state;
}

/* Effect draw: fills the flash's view. */
Gfx *func_15182768(Gfx *gfx, FlashEffect *effect, s16 view) {
    FlashData *data = &effect->data;

    if (view == data->view) {
        gfx = func_1517F08C(gfx, data->alpha, data->r, data->g, data->b, data->view);
    }
    return gfx;
}
