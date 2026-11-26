#include <ultra64.h>
#include "structs.h"

/*
 * This file's .bss, in the original order. IDO lays out .bss in an order that depends on the declarations
 * before it, so it is defined before functions.h and variables.h.
 */
u8 gFont0Bitmaps[95][0xE0]; // font 0: glyph bitmaps (I4, unpacked by text_unpackGlyph), one per charset entry
FontGlyph gFont0Glyphs[95]; // font 0: glyph metrics
u8 D_800CBD4C;            // cleared by func_15015910
u16 gTextAnimClock;           // animation clock (high bits pick icon frames)

/* Text state. */
static u8 gTextControlCode; // control/icon code of the current character, 0xFF if none
static u8 D_800CBD51, D_800CBD52, D_800CBD53, D_800CBD54, D_800CBD55, D_800CBD56; // not referenced by any code
static u8 gTextRed; // current primitive colour r, g, b, a (gTextRed-5A)
static u8 gTextGreen;
static u8 gTextBlue;
static u8 gTextAlpha;
static u8 gTextFlashOn; // flashing text on
static u8 gTextFlashPhase; // flash phase
static u8 gTextWaveOn; // waving text on
static u8 gTextWavePhase; // wave phase

#include "functions.h"
#include "variables.h"

/* The font charset: glyph i shows character gFontCharset[i] (uppercase only; text_findGlyph folds case). */
static u8 gFontCharset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ&\\^:,$\"=!.>#{([<-%+?})];/~'\304\344\326\366\334\374\337\300\340\302\342\307\347\311\351\310\353\312\352\313\352\316\356\317\357\324\364\333\364\331\372@";
u8 (*gFontBitmaps[])[0xE0] = { gFont0Bitmaps }; // per font: glyph bitmaps
FontGlyph *gFontGlyphs[] = { gFont0Glyphs };   // per font: glyph metrics

/*
 * Size and layout of a special (icon) glyph, codes 0xA8-0xFF: its width and height in texels, how far
 * it is raised above the baseline, whether it keeps the current combiner (noCombine) and its scale.
 * Other codes get a zero size.
 */
void text_getIconSize(s32 glyph, s32 *width, s32 *height, f32 *yOffset, s32 *noCombine, f32 *scale) {
    if (glyph >= 0xA8 && glyph < 0x100) {
        *scale = 1.0f;
        switch (glyph) {
            case 0xA8:
            case 0xA9:
                *width = 0x20;
                *height = 0x12;
                *yOffset = 3.1f;
                *noCombine = 1;
                break;
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
                *width = 0x20;
                *height = 0x1B;
                *yOffset = 2.0f;
                *noCombine = 1;
                *scale = 0.603f;
                break;
            case 0xAE:
                *width = 0x10;
                *height = 0x11;
                *yOffset = -1.6f;
                *noCombine = 1;
                break;
            case 0xAF:
            case 0xB0:
                *width = 0x2E;
                *height = 0x10;
                *yOffset = 2.0f;
                *noCombine = 1;
                break;
            case 0xB6:
            case 0xB7:
            case 0xB8:
            case 0xB9:
                *width = 0x10;
                *height = 0xC;
                *yOffset = 2.0f;
                *noCombine = 0;
                break;
            case 0xBA:
                *noCombine = 0;
                *yOffset = 0.2f;
                *width = 0x37;
                *height = 0xC;
                break;
            case 0xB1:
                *width = 0x26;
                *height = 0x18;
                *yOffset = 2.0f;
                *noCombine = 1;
                *scale = 0.726f;
                break;
            default:
                *width = 0xC;
                *height = 0x10;
                *yOffset = 2.0f;
                *noCombine = 0;
                break;
        }
    } else {
        *yOffset = 0.0f;
        *width = 0;
        *height = 0;
    }
}

/*
 * Draws len characters of str at (x, y) in the current view, scaled by scaleX/scaleY (4096 = 1.0).
 * Codes 0xA1-0xA7 change colour or enable flashing (0xA6) and waving (0xA7) text; codes 0xA8 and up
 * draw icon sprites. If the display list buffer fills up, returns the list as it was before the
 * current character.
 */
Gfx *text_drawString(Gfx *gfx, f32 x, f32 y, u8 *str, u8 r, u8 g, u8 b, u8 a, f32 scaleX, f32 scaleY, s32 len) {
    u8 c;
    TextureDesc *tex;
    SpriteParams sprite;
    s32 frame;
    f32 sx;
    f32 sy;
    f32 gy;
    s32 w;
    s32 h;
    s32 prevW;
    s32 prevH;
    f32 ty; // matching: unused, holds a stack slot
    s32 ix;
    s32 iy;
    s32 dsdx;
    s32 i;
    s32 dtdy;
    s32 yi;
    s32 pad15C; // matching: stack-slot pads (frame layout of the IDO original)
    s32 pad158;
    s32 pad154;
    s32 pad150;
    s32 origR;
    s32 origG;
    s32 origB;
    u8 curR;
    u8 curG;
    u8 curB;
    u8 curA;
    u8 phase;
    f32 wave;
    f32 flash;
    s32 font;
    s32 draw;
    s32 ang;
    s32 pad124;
    Gfx *start;

    if (str == NULL) {
        return gfx;
    }
    scaleX *= gViews[gCurrentView].scaleX;
    scaleY *= gViews[gCurrentView].scaleY;
    scaleX *= gScreenScaleX;
    scaleY *= gScreenScaleY;
    prevW = prevH = -1;
    w = 0;
    i = 0;
    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
    curR = r;
    curG = g;
    curB = b;
    curA = a;
    origR = r;
    origG = g;
    origB = b;
    phase = 0;
    gTextRed = r;
    gTextGreen = g;
    gTextBlue = b;
    gTextAlpha = a;
    gTextFlashOn = 0;
    gTextWaveOn = 0;
    gTextWavePhase += D_800BE9E4 * 6;
    c = 0;
    flash = 0.0f;
    while (i < len) {
        start = gfx;
        if (curR != gTextRed || curG != gTextGreen || curB != gTextBlue || curA != gTextAlpha) {
            gTextRed = curR;
            gTextGreen = curG;
            gTextBlue = curB;
            gTextAlpha = curA;
            a = gTextAlpha;
            gDPSetPrimColor(gfx++, 1, 0, gTextRed, gTextGreen, gTextBlue, gTextAlpha);
        }
        if (gTextFlashOn != 0) {
            r = gTextRed;
            g = gTextGreen;
            b = gTextBlue;
            a = (gTextAlpha - 127) * flash + 127.0f;
            if (gTextAlpha < a) {
                a = gTextAlpha;
            }
            gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
        }
        if (gTextWaveOn != 0) {
            ang = gTextWavePhase + phase;
            wave = (cosf((ang & 0xFF) * 0.02454369329f) + 1.0f) * 1.5f; // 2*pi/256 per step
            phase += 0x4E;
        }
        if (str[i] == 0xBC) {
            gTextControlCode = 0xFF;
        } else {
            font = 0;
            c = text_findGlyph(str[i]);
            if (c > 0xA0) {
                gTextControlCode = c;
            } else {
                gTextControlCode = 0xFF;
            }
        }
        switch (gTextControlCode) {
            case 0xA1:
                gTextFlashOn = 0;
                curR = origR;
                curG = origG;
                curB = origB;
                gTextWaveOn = 0;
                break;
            case 0xA2:
                curR = 0xFF;
                curG = 0;
                curB = 0;
                break;
            case 0xA3:
                curR = 0;
                curG = 0xFF;
                curB = 0;
                break;
            case 0xA4:
                curR = 0;
                curG = 0;
                curB = 0xFF;
                break;
            case 0xA5:
                curR = 0xFF;
                curG = 0xFF;
                curB = 0;
                break;
            case 0xA6:
                gTextFlashOn = 1;
                gTextFlashPhase += D_800BE9E4 * 8;
                flash = (cosf(gTextFlashPhase * 0.02454369329f) + 1.0f) * 0.5f;
                break;
            case 0xA7:
                gTextWaveOn = 1;
                break;
            case 0xA8:
                tex = &D_800915DC;
                frame = 0;
                break;
            case 0xA9:
                tex = &D_800915EC;
                frame = 0;
                break;
            case 0xAE:
                tex = &D_800915FC;
                frame = 0;
                break;
            case 0xAF:
                tex = &D_8009160C;
                frame = 0;
                break;
            case 0xB0:
                tex = &D_8009161C;
                frame = 0;
                break;
            case 0xAA:
                tex = &D_8009162C;
                frame = 0;
                break;
            case 0xAB:
                tex = &D_8009163C;
                frame = 0;
                break;
            case 0xAC:
                tex = &D_8009164C;
                frame = 0;
                break;
            case 0xAD:
                tex = &D_8009165C;
                frame = 0;
                break;
            case 0xB2:
                tex = &D_800916AC;
                frame = (gTextAnimClock >> 8) & 3;
                break;
            case 0xB4:
                tex = &D_800916C8;
                frame = (gTextAnimClock >> 8) & 3;
                break;
            case 0xB3:
                tex = &D_800916E4;
                frame = (gTextAnimClock >> 8) & 3;
                break;
            case 0xB5:
                tex = &D_80091700;
                frame = (gTextAnimClock >> 8) & 3;
                break;
            case 0xB6:
                tex = &D_80091710;
                frame = 0;
                break;
            case 0xB7:
                tex = &D_80091720;
                frame = 0;
                break;
            case 0xB8:
                tex = &D_80091730;
                frame = 0;
                break;
            case 0xB9:
                tex = &D_80091740;
                frame = 0;
                break;
            case 0xBA:
                tex = &D_80091750;
                frame = 0;
                break;
            case 0xBB:
                tex = &D_80091764;
                frame = (gTextAnimClock >> 10) & 1;
                break;
            case 0xB1:
                frame = (gTextAnimClock >> 8) % 10;
                tex = &D_80091690;
                break;
        }
        gDPPipeSync(gfx++);
        if (font != 1) {
            draw = 0;
            if (c == 0x60) { // space
                x += 4.0f * gScreenScaleX;
            } else if (c >= 0xA1 && c < 0xA8) {
                // control code: nothing to draw
            } else if (c >= 0xA8 && c < 0x100) {
                f32 yOff;
                s32 noCombine;
                f32 scale;
                f32 dx;
                f32 dy;

                text_getIconSize(c, &w, &h, &yOff, &noCombine, &scale);
                if (gTextWaveOn != 0) {
                    yOff += wave;
                }
                dx = gViews[gCurrentView].scaleX * ((w >> 1) * scale) * gScreenScaleX;
                x += dx;
                dy = gViews[gCurrentView].scaleY * ((h >> 1) * scale) * gScreenScaleY;
                sprite.flip = 0;
                sprite.alpha = a;
                y += dy;
                sprite.scaleX = scaleX * scale;
                sprite.scaleY = scaleY * scale;
                gSPDisplayList(gfx++, gSpriteRenderDl);
                D_800D2DAB = 1;
                gDPSetOtherMode(gfx++, 0x002C3F, gEffectRenderModes[25].cycle1 | gEffectRenderModes[25].cycle2 | 4);
                if (noCombine == 0) {
                    gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0,
                                      0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0);
                }
                // matching: IDO allocates differently without the dead store in "frame <<= 8"
                gfx = sprite_loadDescFrame(gfx, tex, frame <<= 8, &sprite, 0, 0, 0, 2, 3);
                if (gSpriteLastTexFrame != (u8 *)0x80000000) {
                    gfx = sprite_drawScreen(gfx, &sprite, x, y - yOff);
                }
                gSPDisplayList(gfx++, gSpriteEndDl);
                gSPDisplayList(gfx++, gText2DSetupDL);
                D_800D2DAB = 0;
                y -= dy;
                x += dx * 0.5f;
            } else {
                draw = 1;
            }
        } else {
            draw = 1;
        }
        {
            s32 full;

            if (draw) {
                {
                    /* matching: -g3 block marker keeps as1 from hoisting the 1/4096 mtc1 above it */
                    extern f32 gScreenScaleY;

                gy = 0.0f;
                w = gFontGlyphs[font][c].w;
                h = gFontGlyphs[font][c].h;
                sx = scaleX * 0.000244140625f;
                sy = scaleY * 0.000244140625f;
                if (gTextWaveOn != 0) {
                    gy += wave;
                }
                x += gFontGlyphs[font][c].x * gScreenScaleX * sx;
                y += (gFontGlyphs[font][c].y - gy) * gScreenScaleY * sy;
                ix = scaleX;
                iy = scaleY;
                if (gViews[gCurrentView].left <= x + w * gScreenScaleX && x < gViews[gCurrentView].right &&
                    gViews[gCurrentView].top <= y + h * gScreenScaleY && y < gViews[gCurrentView].bottom) {
                    dsdx = 0x400000 / ix;
                    dtdy = 0x400000 / iy;
                    if (w != prevW || h != prevH) {
                        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, ((w + 7) >> 3), 0, 3, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
                        gDPSetTileSize(gfx++, 3, 0, 0, (((w + 7) & 0xFFF8) - 1) << 2, h << 2);
                    }
                    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, gFontBitmaps[font][0] + c * 0xE0); // matching: a byte offset; gFontBitmaps[font][c] changes the frame
                    gDPLoadBlock(gfx++, 1, 0, 0, ((((w + 7) & 0xFFF8) * (h + 1) + 1) >> 1) - 1, 0);
                    yi = y;
                    gSPScisTextureRectangle(gfx++,
                        (x * 4), yi * 4,
                        (x * 4 + (f32)((w * ix) / 4096) * 4),
                        ((s32)(f32)((h * iy) / 4096) + yi) * 4,
                        3, 0, ((-(s32)((y - yi) * 32.0f) * 4096) + 0x20000) / iy, dsdx, dtdy);
                }
                y -= (gFontGlyphs[font][c].y - gy) * gScreenScaleY * sx;
                x += (gFontGlyphs[font][c].w - 1) * gScreenScaleX * sy;
                }
            }
            i++;
            prevW = w;
            prevH = h;
            if (gfx != NULL) {
                if ((gfx - gDisplayListBuffers[gDrawBufferIndex]) > gDisplayListLimit) {
                    full = 1;
                } else {
                    full = 0;
                }
                if (full) {
                    gDPPipeSync(gfx);
                    return start;
                }
            }
        }
    }
    return gfx;
}

/*
 * Measures str as drawn by text_drawString at scale 1.0: the widest line, the total height (lines of at
 * least 12) and how far icons are raised above the baseline (depth), all in screen units.
 */
void text_measureString(u8 *str, s32 *width, s32 *height, s32 *depth) {
    s32 maxWidth;
    s32 lineHeight;
    s32 i;
    u8 c;
    u8 font;
    s32 glyphHeight;
    s32 isGlyph;

    if (str != NULL) {
        *width = 0;
        *height = 0;
        *depth = 0;
        maxWidth = 0;
        lineHeight = 0;
        i = 0;
        font = 0;
        if (*str == 0) {
            *height += 12;
        }
        while (str[i] != 0) {
            if (str[i] == 0xBC) { // escape code: skip it and its argument byte
                i += 2;
                continue;
            }
            c = text_findGlyph(str[i]);
            if (font != 1) {
                isGlyph = 0;
                if (c == 0x60) {
                    *width += 4;
                    if (lineHeight < 12) {
                        lineHeight = 12;
                    }
                } else if (c > 0xA0 && c < 0xA8) {
                    // control code: no size
                } else if (c >= 0xA8 && c < 0x100) {
                    s32 w;
                    s32 h;
                    f32 yOffset;
                    s32 noCombine;
                    f32 scale;
                    s32 iconHeight;
                    text_getIconSize(c, &w, &h, &yOffset, &noCombine, &scale);
                    *width += w * scale;
                    iconHeight = (s32)((s32)(h * scale) - yOffset) + 1;
                    if (lineHeight < iconHeight) {
                        lineHeight = iconHeight;
                    }
                    if (*depth < yOffset) {
                        *depth = yOffset;
                    }
                } else if (c < 0xA1) { // matching: always true here, but the test is in the original
                    if (str[i] == '\n') {
                        if (maxWidth < *width) {
                            maxWidth = *width;
                        }
                        *width = 0;
                        if (lineHeight < 12) {
                            lineHeight = 12;
                        }
                        *height += lineHeight;
                        lineHeight = 0;
                    } else {
                        isGlyph = 1;
                    }
                }
            } else {
                isGlyph = 1;
            }
            if (isGlyph) {
                *width += gFontGlyphs[font][c].w + gFontGlyphs[font][c].x - 1;
                glyphHeight = gFontGlyphs[font][c].h + gFontGlyphs[font][c].y;
                if (lineHeight <= glyphHeight) {
                    lineHeight = glyphHeight;
                }
            }
            i++;
        }
        if (*width < maxWidth) {
            *width = maxWidth;
        }
        *height += lineHeight;
        *width *= gScreenScaleX;
        *height *= gScreenScaleY;
        *depth *= gScreenScaleX;
    }
}

/* Maps a character to its glyph index in the font charset; lowercase folds to uppercase, space is 0x60. */
s32 text_findGlyph(u8 ch) {
    s32 i;
    u8 c;
    s32 original;

    original = ch;
    if (original >= 'a' && original <= 'z') {
        c = original - ('a' - 'A');
    } else {
        c = original;
    }

    if (original == ' ') {
        return 0x60;
    }

    for (i = 0; i < 95; i++) {
        if (c == gFontCharset[i]) {
            return i;
        }
    }

    return original;
}
