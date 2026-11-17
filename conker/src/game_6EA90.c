#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_80084930;
extern u8 D_800848D0[];
extern u8 D_80085930;
extern u8 D_80085931;
extern u8 D_80085932;
extern u8 D_80085933[];

#define WGFX(pkt, a, b)             \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}
Gfx *func_150412C0(Gfx *gfx);

s32 func_15041480(u8 arg0);
Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);
Gfx *func_150413FC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
s32 func_15041480(u8 arg0);
Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);


void func_150415E0(s32 arg0, s32 *arg1, s32 *arg2, f32 *arg3, s32 *arg4, f32 *arg5) {
    if ((arg0 >= 0xA8) && (arg0 < 0x100)) {
        *arg5 = 1.0f;
        switch (arg0) {
            case 0xA8:
            case 0xA9:
                *arg1 = 0x20;
                *arg2 = 0x12;
                *arg3 = 3.1f;
                *arg4 = 1;
                break;
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
                *arg1 = 0x20;
                *arg2 = 0x1B;
                *arg3 = 2.0f;
                *arg4 = 1;
                *arg5 = 0.603f;
                break;
            case 0xAE:
                *arg1 = 0x10;
                *arg2 = 0x11;
                *arg3 = -1.6f;
                *arg4 = 1;
                break;
            case 0xAF:
            case 0xB0:
                *arg1 = 0x2E;
                *arg2 = 0x10;
                *arg3 = 2.0f;
                *arg4 = 1;
                break;
            case 0xB6:
            case 0xB7:
            case 0xB8:
            case 0xB9:
                *arg1 = 0x10;
                *arg2 = 0xC;
                *arg3 = 2.0f;
                *arg4 = 0;
                break;
            case 0xBA:
                *arg4 = 0;
                *arg3 = 0.2f;
                *arg1 = 0x37;
                *arg2 = 0xC;
                break;
            case 0xB1:
                *arg1 = 0x26;
                *arg2 = 0x18;
                *arg3 = 2.0f;
                *arg4 = 1;
                *arg5 = 0.726f;
                break;
            default:
                *arg1 = 0xC;
                *arg2 = 0x10;
                *arg3 = 2.0f;
                *arg4 = 0;
                break;
        }
    } else {
        *arg3 = 0.0f;
        *arg1 = 0;
        *arg2 = 0;
    }
}

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x8];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x14C];
} Cam417AC;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 w;
    s16 h;
    u8 alpha;
    u8 unkB;
    u8 unkC;
    u8 flip;
    u8 padE[2];
} Sprite417AC;

typedef struct {
    u8 w;
    u8 h;
    u8 x;
    u8 y;
} Glyph417AC;

extern u16 D_800CBD4E;
extern u8 D_800CBD50;
extern u8 D_800CBD57;
extern u8 D_800CBD58;
static u8 D_800CBD59;
static u8 D_800CBD5A;
static u8 D_800CBD5B;
extern u8 D_800CBD5C;
extern u8 D_800CBD5D;
extern u8 D_800CBD5E;
extern u8 *D_80085990[];
extern Glyph417AC *D_80085994[];
extern Gfx D_800859A0[];
extern Gfx D_800873D0[];
extern Gfx D_80087408[];
extern u8 D_800915DC[];
extern u8 D_800915EC[];
extern u8 D_800915FC[];
extern u8 D_8009160C[];
extern u8 D_8009161C[];
extern u8 D_8009162C[];
extern u8 D_8009163C[];
extern u8 D_8009164C[];
extern u8 D_8009165C[];
extern u8 D_80091690[];
extern u8 D_800916AC[];
extern u8 D_800916C8[];
extern u8 D_800916E4[];
extern u8 D_80091700[];
extern u8 D_80091710[];
extern u8 D_80091720[];
extern u8 D_80091730[];
extern u8 D_80091740[];
extern u8 D_80091750[];
extern u8 D_80091764[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2CA0;
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
f32 cosf(f32);
Gfx *func_15094F70(Gfx *, u8 *, s32, Sprite417AC *, s32, s32, s32, s32, s32);
Gfx *func_15095A48(Gfx *, Sprite417AC *, f32, f32);
s32 func_15042C40(u8 arg0);

#define CAM417AC ((Cam417AC *)D_800BE628 + D_80082FA4)

Gfx *func_150417AC(Gfx *gfx, f32 x, f32 y, u8 *str, u8 r, u8 g, u8 b, u8 a, f32 scaleX, f32 scaleY, s32 len) {
    u8 c;
    u8 *tex;
    s32 pad1AC;
    Sprite417AC sprite;
    s32 shift;
    f32 sx;
    f32 sy;
    f32 gy;
    s32 w;
    s32 h;
    s32 prevW;
    s32 prevH;
    f32 ty;
    s32 ix;
    s32 iy;
    s32 dsdx;
    s32 i;
    s32 dtdy;
    s32 yi;
    s32 pad15C;
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
    scaleX *= CAM417AC->unk14;
    scaleY *= CAM417AC->unk18;
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
            a = (D_800CBD5A - 0x7F) * flash + 127.0f;
            if (D_800CBD5A < a) {
                a = D_800CBD5A;
            }
            gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
        }
        if (D_800CBD5D != 0) {
            ang = D_800CBD5E + phase;
            wave = (cosf((ang & 0xFF) * 0.02454369329f) + 1.0f) * 1.5f;
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
                flash = (cosf((f32)D_800CBD5C * 0.02454369329f) + 1.0f) * 0.5f;
                break;
            case 0xA7:
                D_800CBD5D = 1;
                break;
            case 0xA8:
                tex = D_800915DC;
                shift = 0;
                break;
            case 0xA9:
                tex = D_800915EC;
                shift = 0;
                break;
            case 0xAE:
                tex = D_800915FC;
                shift = 0;
                break;
            case 0xAF:
                tex = D_8009160C;
                shift = 0;
                break;
            case 0xB0:
                tex = D_8009161C;
                shift = 0;
                break;
            case 0xAA:
                tex = D_8009162C;
                shift = 0;
                break;
            case 0xAB:
                tex = D_8009163C;
                shift = 0;
                break;
            case 0xAC:
                tex = D_8009164C;
                shift = 0;
                break;
            case 0xAD:
                tex = D_8009165C;
                shift = 0;
                break;
            case 0xB2:
                tex = D_800916AC;
                shift = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB4:
                tex = D_800916C8;
                shift = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB3:
                tex = D_800916E4;
                shift = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB5:
                tex = D_80091700;
                shift = (D_800CBD4E >> 8) & 3;
                break;
            case 0xB6:
                tex = D_80091710;
                shift = 0;
                break;
            case 0xB7:
                tex = D_80091720;
                shift = 0;
                break;
            case 0xB8:
                tex = D_80091730;
                shift = 0;
                break;
            case 0xB9:
                tex = D_80091740;
                shift = 0;
                break;
            case 0xBA:
                tex = D_80091750;
                shift = 0;
                break;
            case 0xBB:
                tex = D_80091764;
                shift = (D_800CBD4E >> 10) & 1;
                break;
            case 0xB1:
                shift = (D_800CBD4E >> 8) % 10;
                tex = D_80091690;
                break;
        }
        gDPPipeSync(gfx++);
        if (font != 1) {
            draw = 0;
            if (c == 0x60) {
                x += 4.0f * D_800380A0;
            } else if (c >= 0xA1 && c < 0xA8) {
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
                dx = CAM417AC->unk14 * ((w >> 1) * scale) * D_800380A0;
                x += dx;
                dy = CAM417AC->unk18 * ((h >> 1) * scale) * D_800380A4;
                sprite.flip = 0;
                sprite.alpha = a;
                y += dy;
                sprite.w = scaleX * scale;
                sprite.h = scaleY * scale;
                gSPDisplayList(gfx++, D_800873D0);
                D_800D2DAB = 1;
                gDPSetOtherMode(gfx++, 0x002C3F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
                if (noCombine == 0) {
                    WGFX(gfx++, 0xFCFFB3FF, 0xFF65FEFF);
                }
                gfx = func_15094F70(gfx, tex, shift <<= 8, &sprite, 0, 0, 0, 2, 3);
                if (D_800D2CA0 != 0x80000000) {
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
                {
                    extern f32 D_800380A4;

                gy = 0.0f;
                w = D_80085994[font][c].w;
                h = D_80085994[font][c].h;
                sx = scaleX * 0.000244140625f;
                sy = scaleY * 0.000244140625f;
                if (D_800CBD5D != 0) {
                    gy += wave;
                }
                x += (f32)D_80085994[font][c].x * D_800380A0 * sx;
                y += ((f32)D_80085994[font][c].y - gy) * D_800380A4 * sy;
                ix = scaleX;
                iy = scaleY;
                if (CAM417AC->unk2C <= x + w * D_800380A0 && x < CAM417AC->unk30 &&
                    CAM417AC->unk24 <= y + h * D_800380A4 && y < CAM417AC->unk28) {
                    dsdx = 0x400000 / ix;
                    dtdy = 0x400000 / iy;
                    if (w != prevW || h != prevH) {
                        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, ((w + 7) >> 3), 0, 3, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
                        gDPSetTileSize(gfx++, 3, 0, 0, (((w + 7) & 0xFFF8) - 1) << 2, h << 2);
                    }
                    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, D_80085990[font] + c * 0xE0);
                    gDPLoadBlock(gfx++, 1, 0, 0, ((((w + 7) & 0xFFF8) * (h + 1) + 1) >> 1) - 1, 0);
                    yi = y;
                    gSPScisTextureRectangle(gfx++,
                        (s32)(x * 4), yi * 4,
                        (s32)(x * 4 + (f32)((w * ix) / 4096) * 4),
                        ((s32)(f32)((h * iy) / 4096) + yi) * 4,
                        3, 0, ((-(s32)((y - (f32)yi) * 32.0f) * 4096) + 0x20000) / iy, dsdx, dtdy);
                }
                y -= ((f32)D_80085994[font][c].y - gy) * D_800380A4 * sx;
                x += (f32)(D_80085994[font][c].w - 1) * D_800380A0 * sy;
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_6EA90/func_150428D4.s")

s32 func_15042C40(u8 arg0) {
    s32 i;
    u8 c;
    u8 *ptr;
    s32 original;

    original = arg0;
    if ((original >= 0x61) && (original < 0x7B)) {
        c = original - 0x20;
    } else {
        c = original;
    }

    if (original == 0x20) {
        return 0x60;
    }

    i = 3;
    if (D_80085930 == c) {
        return 0;
    }
    if (D_80085931 == c) {
        return 1;
    }
    if (D_80085932 == c) {
        return 2;
    }

    ptr = D_80085933;
    for (i = 3; ; ) {
        if (c == ptr[0]) {
            return i;
        }
        if (c == ptr[1]) {
            return i + 1;
        }
        if (c == ptr[2]) {
            return i + 2;
        }
        if (c == ptr[3]) {
            return i + 3;
        }
        i += 4;
        ptr += 4;
        if (i != 0x5F) {
            continue;
        }
        break;
    }

    return original;
}
