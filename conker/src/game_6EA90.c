#include <ultra64.h>

/*
 * Text state: this file's .bss (0x800CBD50-0x800CBD5F), in the original order. IDO lays out .bss in an order
 * that depends on the declarations before it, so it is defined before functions.h and variables.h.
 */
static u8 D_800CBD50; // control/icon code of the current character, 0xFF if none
static u8 D_800CBD51, D_800CBD52, D_800CBD53, D_800CBD54, D_800CBD55, D_800CBD56; // not referenced by any code
static u8 D_800CBD57; // current primitive colour r, g, b, a (D_800CBD57-5A)
static u8 D_800CBD58;
static u8 D_800CBD59;
static u8 D_800CBD5A;
static u8 D_800CBD5B; // flashing text on
static u8 D_800CBD5C; // flash phase
static u8 D_800CBD5D; // waving text on
static u8 D_800CBD5E; // wave phase

#include "functions.h"
#include "variables.h"

extern u8 D_80085930[]; // font charset, 95 entries

/*
 * Size and layout of a special (icon) glyph, codes 0xA8-0xFF: its width and height in texels, how far
 * it is raised above the baseline, whether it keeps the current combiner (noCombine) and its scale.
 * Other codes get a zero size.
 */
void func_150415E0(s32 glyph, s32 *width, s32 *height, f32 *yOffset, s32 *noCombine, f32 *scale) {
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

extern TextureDesc D_800915DC;
extern TextureDesc D_800915EC;
extern TextureDesc D_800915FC;
extern TextureDesc D_8009160C;
extern TextureDesc D_8009161C;
extern TextureDesc D_8009162C;
extern TextureDesc D_8009163C;
extern TextureDesc D_8009164C;
extern TextureDesc D_8009165C;
extern TextureDesc D_80091690;
extern TextureDesc D_800916AC;
extern TextureDesc D_800916C8;
extern TextureDesc D_800916E4;
extern TextureDesc D_80091700;
extern TextureDesc D_80091710;
extern TextureDesc D_80091720;
extern TextureDesc D_80091730;
extern TextureDesc D_80091740;
extern TextureDesc D_80091750;
extern TextureDesc D_80091764;
extern s32 D_800A4AC8[][2];

/*
 * Draws len characters of str at (x, y) in the current view, scaled by scaleX/scaleY (4096 = 1.0).
 * Codes 0xA1-0xA7 change colour or enable flashing (0xA6) and waving (0xA7) text; codes 0xA8 and up
 * draw icon sprites. If the display list buffer fills up, returns the list as it was before the
 * current character.
 */
Gfx *func_150417AC(Gfx *gfx, f32 x, f32 y, u8 *str, u8 r, u8 g, u8 b, u8 a, f32 scaleX, f32 scaleY, s32 len) {
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
    scaleX *= D_800BE628[D_80082FA4].scaleX;
    scaleY *= D_800BE628[D_80082FA4].scaleY;
    scaleX *= D_800380A0;
    scaleY *= D_800380A4;
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
    D_800CBD57 = r;
    D_800CBD58 = g;
    D_800CBD59 = b;
    D_800CBD5A = a;
    D_800CBD5B = 0;
    D_800CBD5D = 0;
    D_800CBD5E += D_800BE9E4 * 6;
    c = 0;
    flash = 0.0f;
    while (i < len) {
        start = gfx;
        if (curR != D_800CBD57 || curG != D_800CBD58 || curB != D_800CBD59 || curA != D_800CBD5A) {
            D_800CBD57 = curR;
            D_800CBD58 = curG;
            D_800CBD59 = curB;
            D_800CBD5A = curA;
            a = D_800CBD5A;
            gDPSetPrimColor(gfx++, 1, 0, D_800CBD57, D_800CBD58, D_800CBD59, D_800CBD5A);
        }
        if (D_800CBD5B != 0) {
            r = D_800CBD57;
            g = D_800CBD58;
            b = D_800CBD59;
            a = (D_800CBD5A - 127) * flash + 127.0f;
            if (D_800CBD5A < a) {
                a = D_800CBD5A;
            }
            gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
        }
        if (D_800CBD5D != 0) {
            ang = D_800CBD5E + phase;
            wave = (cosf((ang & 0xFF) * 0.02454369329f) + 1.0f) * 1.5f; // 2*pi/256 per step
            phase += 0x4E;
        }
        if (str[i] == 0xBC) {
            D_800CBD50 = 0xFF;
        } else {
            font = 0;
            c = func_15042C40(str[i]);
            if (c > 0xA0) {
                D_800CBD50 = c;
            } else {
                D_800CBD50 = 0xFF;
            }
        }
        switch (D_800CBD50) {
            case 0xA1:
                D_800CBD5B = 0;
                curR = origR;
                curG = origG;
                curB = origB;
                D_800CBD5D = 0;
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
                D_800CBD5B = 1;
                D_800CBD5C += D_800BE9E4 * 8;
                flash = (cosf(D_800CBD5C * 0.02454369329f) + 1.0f) * 0.5f;
                break;
            case 0xA7:
                D_800CBD5D = 1;
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
                frame = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB4:
                tex = &D_800916C8;
                frame = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB3:
                tex = &D_800916E4;
                frame = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB5:
                tex = &D_80091700;
                frame = (D_800CBD4E >> 8) & 3;
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
                frame = (D_800CBD4E >> 10) & 1;
                break;
            case 0xB1:
                frame = (D_800CBD4E >> 8) % 10;
                tex = &D_80091690;
                break;
        }
        gDPPipeSync(gfx++);
        if (font != 1) {
            draw = 0;
            if (c == 0x60) { // space
                x += 4.0f * D_800380A0;
            } else if (c >= 0xA1 && c < 0xA8) {
                // control code: nothing to draw
            } else if (c >= 0xA8 && c < 0x100) {
                f32 yOff;
                s32 noCombine;
                f32 scale;
                f32 dx;
                f32 dy;

                func_150415E0(c, &w, &h, &yOff, &noCombine, &scale);
                if (D_800CBD5D != 0) {
                    yOff += wave;
                }
                dx = D_800BE628[D_80082FA4].scaleX * ((w >> 1) * scale) * D_800380A0;
                x += dx;
                dy = D_800BE628[D_80082FA4].scaleY * ((h >> 1) * scale) * D_800380A4;
                sprite.flip = 0;
                sprite.alpha = a;
                y += dy;
                sprite.scaleX = scaleX * scale;
                sprite.scaleY = scaleY * scale;
                gSPDisplayList(gfx++, D_800873D0);
                D_800D2DAB = 1;
                gDPSetOtherMode(gfx++, 0x002C3F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
                if (noCombine == 0) {
                    gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0,
                                      0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0);
                }
                // matching: IDO allocates differently without the dead store in "frame <<= 8"
                gfx = func_15094F70(gfx, tex, frame <<= 8, &sprite, 0, 0, 0, 2, 3);
                if (D_800D2CA0 != (u8 *)0x80000000) {
                    gfx = func_15095A48(gfx, &sprite, x, y - yOff);
                }
                gSPDisplayList(gfx++, D_80087408);
                gSPDisplayList(gfx++, D_800859A0);
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
                { // matching: -g3 block marker keeps as1 from hoisting the 1/4096 mtc1 above it
                    extern f32 D_800380A4;

                gy = 0.0f;
                w = D_80085994[font][c].w;
                h = D_80085994[font][c].h;
                sx = scaleX * 0.000244140625f;
                sy = scaleY * 0.000244140625f;
                if (D_800CBD5D != 0) {
                    gy += wave;
                }
                x += D_80085994[font][c].x * D_800380A0 * sx;
                y += (D_80085994[font][c].y - gy) * D_800380A4 * sy;
                ix = scaleX;
                iy = scaleY;
                if (D_800BE628[D_80082FA4].left <= x + w * D_800380A0 && x < D_800BE628[D_80082FA4].right &&
                    D_800BE628[D_80082FA4].top <= y + h * D_800380A4 && y < D_800BE628[D_80082FA4].bottom) {
                    dsdx = 0x400000 / ix;
                    dtdy = 0x400000 / iy;
                    if (w != prevW || h != prevH) {
                        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, ((w + 7) >> 3), 0, 3, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
                        gDPSetTileSize(gfx++, 3, 0, 0, (((w + 7) & 0xFFF8) - 1) << 2, h << 2);
                    }
                    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, D_80085990[font][0] + c * 0xE0); // matching: a byte offset; D_80085990[font][c] changes the frame
                    gDPLoadBlock(gfx++, 1, 0, 0, ((((w + 7) & 0xFFF8) * (h + 1) + 1) >> 1) - 1, 0);
                    yi = y;
                    gSPScisTextureRectangle(gfx++,
                        (x * 4), yi * 4,
                        (x * 4 + (f32)((w * ix) / 4096) * 4),
                        ((s32)(f32)((h * iy) / 4096) + yi) * 4,
                        3, 0, ((-(s32)((y - yi) * 32.0f) * 4096) + 0x20000) / iy, dsdx, dtdy);
                }
                y -= (D_80085994[font][c].y - gy) * D_800380A4 * sx;
                x += (D_80085994[font][c].w - 1) * D_800380A0 * sy;
                }
            }
            i++;
            prevW = w;
            prevH = h;
            if (gfx != NULL) {
                if ((gfx - D_800BE9C8[D_800BE9C0]) > D_800BEBA4) {
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
 * Measures str as drawn by func_150417AC at scale 1.0: the widest line, the total height (lines of at
 * least 12) and how far icons are raised above the baseline (depth), all in screen units.
 */
void func_150428D4(u8 *str, s32 *width, s32 *height, s32 *depth) {
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
            c = func_15042C40(str[i]);
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
                    func_150415E0(c, &w, &h, &yOffset, &noCombine, &scale);
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
                *width += D_80085994[font][c].w + D_80085994[font][c].x - 1;
                glyphHeight = D_80085994[font][c].h + D_80085994[font][c].y;
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
        *width *= D_800380A0;
        *height *= D_800380A4;
        *depth *= D_800380A0;
    }
}

/* Maps a character to its glyph index in the font charset; lowercase folds to uppercase, space is 0x60. */
s32 func_15042C40(u8 ch) {
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
        if (c == D_80085930[i]) {
            return i;
        }
    }

    return original;
}
