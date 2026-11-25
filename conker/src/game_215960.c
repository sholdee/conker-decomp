#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0_5
#include "functions.h"
#undef func_1502B7F0
/* variables.h types D_800CC2D0 as struct127[], whose 0x128 is inside `u32 pad128`;
   func_151E9D18 needs a byte there, so shadow the declaration file-locally. */
#define D_800CC2D0 D_800CC2D0_struct127_decl_in_variables_h
#include "variables.h"
#undef D_800CC2D0

extern s32 D_800E0A74;
extern u8 D_800E0B96;
extern u8 D_800E0B97;
extern Gfx D_80090028;
extern u8 D_800917F8;
extern u8 D_80091804;
extern u8 D_80091810;
extern Gfx *(*D_8008FFF4[])(Gfx *);
extern s32 D_80000300;
extern u8 D_800BE616;
extern u8 D_800BE740;
extern s8 D_8008FD90;
extern s8 D_800E0BD3;
extern char **D_800E0BD8;
extern u8 D_7FB;
extern u8 D_887;
extern u8 D_800E0A94;
extern u8 D_800E0C85;
extern s32 D_8009013C;
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern s32 func_1502B7F0(s32 *, s32, s32);
extern void func_10004074(void *);
Gfx *func_151ED1E0(Gfx *);
void func_15042D94();
Gfx *func_151ED430(Gfx *, void *, s32, s32, s32, s32, f32, s32);
Gfx *func_15096934(Gfx *);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);

/* Draws the HUD overlay: shared HUD, the per-mode overlay, and a blinking warning message. */
Gfx *func_151E84B0(Gfx *gfx) {
    s32 message;

    D_8003C8E0 = 0x9000001;
    message = 0;
    gfx = func_151ED1E0(gfx);
    if (D_8008FFF4[D_800E0B94] != NULL) {
        gfx = D_8008FFF4[D_800E0B94](gfx);
    }

    if (D_80000300 != 0) {
        if (D_800BE616 != 0 && D_8008FD90 >= 2) {
            if ((D_800BE740 & 0xF) == 0) {
                if (D_800E0BD3 == 1) {
                    message = 0x33;
                } else if (D_800E0BD3 == 2) {
                    message = 0x16;
                }
            }
        } else {
            if ((D_800BE740 & 1) == 0) {
                if (D_800E0BD3 == 1) {
                    message = 0x32;
                } else if (D_800E0BD3 == 2) {
                    message = 0x15;
                }
            }
        }
    }

    if (message != 0) {
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(0x94, 0xC8, 0x81, D_800E0BD8[message]);
    }

    D_8003C8E0 = 0;
    return gfx;
}

extern u8 D_800E0B94;
extern Gfx *(*D_8008FFC0[])(Gfx *);
extern s32 D_8003C8E0;
extern s32 D_80090058;
extern s16 D_800E0C78;
extern u8 D_800BE9C0;
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;

/* Draws the per-mode overlay; if it overran the display list buffer, discards what it wrote. */
Gfx *func_151E8620(Gfx *gfx) {
    s32 overflow;
    Gfx *start;
    s32 mode;

    mode = D_800E0B94;
    start = gfx;
    D_8003C8E0 = 0x9000000;
    if (D_8008FFC0[mode] != NULL) {
        gfx = D_8008FFC0[mode](gfx);
    }
    if (D_800E0B94 != 0) {
        D_80090058 = 0;
        D_800E0C78 = 0;
    }
    D_8003C8E0 = 0;
    if (gfx - D_800BE9C8[D_800BE9C0] > D_800BEBA4) {
        overflow = TRUE;
    } else {
        overflow = FALSE;
    }
    if (overflow) {
        return start;
    }
    return gfx;
}

extern f32 D_8008FE1C;
extern f32 D_8008FE20;
/* Emits a scissored texture rectangle, scaled by the current screen scale factors. */
Gfx *func_151E86E4(Gfx *gfx, s32 xl, s32 yl, s32 xh, s32 yh, s32 tile, s32 s, s32 t, s32 dsdx, s32 dtdy) {
    f32 yScale;

    if (D_8008FE1C != 1.0f) {
        xl = xl * D_8008FE1C;
        yScale = D_8008FE20;
        xh = xh * D_8008FE1C;
        yl = yl * yScale;
        yh = yh * yScale;
        dsdx = dsdx / D_8008FE1C;
        dtdy = dtdy / D_8008FE20;
    }
    gSPScisTextureRectangle(gfx++, xl, yl, xh, yh, tile, s, t, dsdx, dtdy);
    return gfx;
}


typedef struct {
    /* 0x000 */ char pad0[0x75];
    /* 0x075 */ u8 unk75;
    /* 0x076 */ char pad76[0xB2];
    /* 0x128 */ u8 unk128;
} ScoreObjSub;

typedef struct {
    /* 0x000 */ char pad0[0x14];
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ char pad20[0x108];
    /* 0x128 */ u8 unk128;
    /* 0x129 */ char pad129[0x13];
    /* 0x13C */ u8 unk13C;
    /* 0x13D */ char pad13D[0x1A7];
    /* 0x2E4 */ s32 unk2E4;
    /* 0x2E8 */ char pad2E8[0x34];
    /* 0x31C */ ScoreObjSub *unk31C;
    /* 0x320 */ char pad320[0xC];
} ScoreObj; /* size 0x32C */

extern ScoreObj D_800CC2D0[];
extern u8 D_800ABA90[][4];
extern s8 D_800E0BB0;
extern u8 D_D10;
extern u8 D_D14;
extern u8 D_D16;
extern u16 D_8008FDC0;
extern u16 D_800E0BCC;
extern s32 D_800BE9AC;
extern u8 D_8009006C[];
extern u8 D_80090070[];
extern s32 func_15086D48(s32, s32);
extern void func_15086CBC(s32, f32 *, f32 *, f32 *);
Gfx *func_151E966C(Gfx *gfx, s32 arg1, s32 arg2, s8 arg3, u8 arg4);
Gfx *func_151E9D18(Gfx *, s32, s32);

/* Draws the multiplayer score HUD: team scores, target and team icons, and arrows. */
Gfx *func_151E89A0(Gfx *gfx, s32 unused, s32 alpha) {
    s32 zero; // matching: indexing D_800E0BCC through a zero variable keeps &D_800E0BCC in a register
    s32 x;
    s32 y;
    s32 i;
    s32 image;
    s32 score;
    s32 place;
    s32 spacing;
    s32 digit;
    s32 digitX;
    s32 team;
    s32 width;
    s32 kind;
    ScoreObjSub *sub;
    ScoreObj *obj;
    u8 nearTarget;
    u8 flags;
    s8 started;
    s8 done;
    s32 rowOffset;
    s32 t;
    f32 px;
    f32 py;
    f32 pz;
    s32 s;

    flags = 0;
    D_8008FDBC &= ~0x10;
    gSPDisplayList(gfx++, &D_80090028);
    gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
    if (D_80082FA0 == 0) {
        y = 0x320;
    } else {
        y = 0x198;
    }
    spacing = 0x124;
    if (D_800E0BB0 > 0) { spacing = 0x124 / D_800E0BB0; } // matching: one line (-g3 line numbers steer scheduling)
    zero = 0;
    D_800E0BCC = 0;
    nearTarget = 0;
    team = -1;
    for (i = 0; i < D_8008FD8C; i++) {
        obj = &D_800CC2D0[i];
        sub = obj->unk31C;
        kind = sub->unk75 & 0x7F;
        if ((obj->unk13C != 0) || (sub->unk128 & 0x20)) {
            team = obj->unk128;
            (&D_800E0BCC)[zero] = D_800E0BCC | (1 << i);
        }
        if ((team != -1) && (D_8008FDC0 & 0x8000)) {
            if (team == 0) {
                flags |= 2;
            } else {
                flags |= 1;
            }
            team = -1;
        }
        if ((obj->unk128 == 1) && (obj->unk2E4 != 0) && (D_8008FDC0 & 0x4000)) {
            flags |= 4;
        }
        if (kind == 0x25) {
            func_15086CBC(func_15086D48(0x49, i), &px, &py, &pz);
            px -= obj->unk14;
            pz -= obj->unk1C;
            pz = px * px + pz * pz;
            if (pz < 6250000.0f) {
                D_8008FDBC |= 0x10;
                nearTarget = 1;
            }
            {
                s32 bit = 1 << i;
                (&D_800E0BCC)[zero] = D_800E0BCC | bit;
            }
        }
    }
    if ((D_800BE9AC & 0x1F) < 0xB) {
        D_800E0BCC = 0;
        nearTarget = 0;
        team = -1;
        flags = 0;
    }
    image = 0;
    if (D_8008FDC0 & 9) {
        image = func_1510D0EC(&D_D14, 0, 3, 0);
        if (image != (s32)0x80000000) {
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        } else {
            image = 0;
        }
    }
    if ((D_8008FDC0 & 8) && (image != 0)) {
        x = spacing >> 1;
        for (i = 0; i < D_800E0BB0; i++) {
            score = func_150859AC(i, 6);
            started = 0;
            done = 0;
            gDPPipeSync(gfx++);
            {
                u8 *col = (u8 *)D_800ABA90 + (i << 2);
                gDPSetEnvColor(gfx++, col[0], col[1], col[2], alpha);
            }
            gfx = func_151E86E4(gfx, (x + 3) << 2, y, (x + 10) << 2, y + 0x1C, 0, 0x1C0, 0x200, 0x400, 0x400);
            digitX = (x + 10) << 2;
            place = 10000;
            do {
                digit = score / place;
                if ((digit != 0) || started || (place == 1)) {
                    started = 1;
                    s = D_8009006C[digit & 3] << 5;
                    t = D_80090070[digit >> 2] << 5;
                    gfx = func_151E86E4(gfx, digitX, y, digitX + 0x14, y + 0x1C, 0, s, t, 0x400, 0x400);
                    digitX += 0x18;
                }
                score -= digit * place;
                if (place == 1) {
                    done = 1;
                }
                place /= 10;
            } while (!done);
            x += spacing;
        }
    }
    if (D_8008FDC0 & 1) {
        gfx = func_151E966C(gfx, y, team, 0, 1);
    }
    if (nearTarget || (team != -1)) {
        if (D_8008FDC0 & 0x4000) {
            i = (s32)(&D_D16 + 1);
        } else {
            i = (s32)&D_D16;
        }
        image = func_1510D0EC(i, 0, 3, 0);
        if (image != (s32)0x80000000) {
            gDPSetTextureImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image + 0x800);
            gDPLoadSync(gfx++);
            gDPLoadTLUTCmd(gfx++, 6, 15);
            gDPSetOtherMode(gfx++, 0x00AC3F, 0x504244);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            if (nearTarget) {
                gfx = func_151E86E4(gfx, 0x238, 0xC, 0x268, 0x48, 0, 0x640, 0x660, 0x400, 0x400);
            }
            if (team != -1) {
                if (D_8008FDC0 & 0x4000) {
                    gfx = func_151E86E4(gfx, 0x220, 0xC, 0x250, 0x48, 0, 0x600, 0x540, 0x400, 0x400);
                } else {
                    rowOffset = 0;
                    if (D_8008FDC0 & 1) {
                        t = 0x1A;
                    } else {
                        t = 0x26;
                        rowOffset = 0xB;
                    }
                    gfx = func_151E86E4(gfx, 0x238, (rowOffset + 3) << 2, 0x268, (rowOffset + 0xE) << 2, 0, 0x640, t << 5, 0x400, 0x400);
                }
            }
        }
    }
    width = 0x20;
    if (flags) {
        if (flags & 4) {
            i = (s32)(&D_D10 + 1);
            width = 0x10;
        } else {
            i = (s32)&D_D10;
        }
        image = func_1510D0EC(i, 0, 3, 0);
        if (image != (s32)0x80000000) {
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, (width * 32) - 1, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
                       G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, (width - 1) << 2, 0x7C);
            gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
            if (flags & 1) {
                x = y - 0x10U; // matching: unsigned subtract steers IDO register allocation
                gfx = func_151E86E4(gfx, 0xA0, x, 0xE0, x + 0x40, 0, 0, 0x200, 0x400, 0x400);
            }
            if (flags & 2) {
                s32 yy = y - 0x10U;
                gfx = func_151E86E4(gfx, 0x3B0, yy, 0x3F0, yy + 0x40, 0, 0x200, 0x200, 0x400, 0x400);
            }
            if (flags & 4) {
                gfx = func_151E86E4(gfx, 0x250, 0xC, 0x280, 0x48, 0, 0, 0x200, 0x400, 0x400);
            }
        }
    }
    if (D_8008FDC0 & 0x6340) {
        gfx = func_151E9D18(gfx, y, 1);
    }
    return gfx;
}

extern u8 D_800ABA90[][4];
extern s8 D_800E0BB0;
extern s8 D_80087268;
extern u8 D_D14;
extern u8 D_D16;
extern s16 D_800E0AA0[];
extern s8 D_800E0C00[];


Gfx *func_151E966C(Gfx *gfx, s32 arg1, s32 arg2, s8 arg3, u8 arg4) {
    s8 mask;
    s32 image;
    s32 i;
    s32 j;
    s32 v;
    s32 x0;
    s32 x;
    s32 step;

    mask = 0;
    if (arg3 != 0) {
        image = func_1510D0EC(&D_D14, 0, 3, 0);
        if (image == (s32)0x80000000) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
    }
    step = 0x124;
    if (D_800E0BB0 > 0) {
        step = 0x124 / D_800E0BB0;
    }
    if (arg4) {
        mask = 0xF;
        for (i = 0; i < D_8008FD8C; i++) {
            if (func_150859AC(i, 3)) {
                mask &= ~(1 << D_800E0C00[i]);
            }
        }
    }
    x = step >> 1;
    for (i = 0; i < D_800E0BB0; i++) {
        if (arg4) {
            v = func_150859AC(i, 6);
            D_800E0AA0[i] = v;
        } else {
            v = D_800E0AA0[i];
        }
        if (!(mask & (1 << i))) {
            if (i == arg2) {
                v++;
            }
            gDPPipeSync(gfx++);
            {
                u8 *col = (u8 *)D_800ABA90 + (i << 2);
                gDPSetEnvColor(gfx++, col[0], col[1], col[2], 0xFF);
            }
            x0 = x - (D_80087268 * 4);
            for (j = 0; j < D_80087268; j++) {
                if (j == v) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, 0x40, 0x40, 0x40, 0x40);
                }
                gfx = func_151E86E4(gfx, x0 << 2, arg1, (x0 + 7) << 2, arg1 + 0x1C, 0, 0x1C0, 0x200, 0x400, 0x400);
                x0 += 8;
            }
        }
        x += step;
    }
    if (mask) {
        image = func_1510D0EC((s32)&D_D16 + 1, 0, 3, 0);
        if (image == (s32)0x80000000) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetTextureImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image + 0x800);
        gDPLoadSync(gfx++);
        gDPLoadTLUTCmd(gfx++, 6, 15);
        gDPSetOtherMode(gfx++, 0x00AC3F, 0x504244);
        x = step >> 1;
        for (i = 0; i < D_800E0BB0; i++) {
            if (mask & (1 << i)) {
                gDPPipeSync(gfx++);
                {
                    u8 *col = (u8 *)D_800ABA90 + (i << 2);
                    gDPSetEnvColor(gfx++, col[0], col[1], col[2], 0xFF);
                }
                gfx = func_151E86E4(gfx, (x - 8) << 2, arg1 - 0x18, (x + 8) << 2, arg1 + 0x28, 0, 0x600, 0x280, 0x400, 0x400);
            }
            x += step;
        }
    }
    gDPPipeSync(gfx++);
    return gfx;
}


typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ char pad4[4];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ char pad10[0x10];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ char pad22[0xA];
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ char pad2D[0x11];
    /* 0x3E */ s8 unk3E;
    /* 0x3F */ char pad3F[1];
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ s8 unk42;
    /* 0x43 */ char pad43[3];
    /* 0x46 */ s16 unk46[4];
    /* 0x4E */ char pad4E[0x18];
    /* 0x66 */ struct {
        /* 0x00 */ s16 unk0;
        /* 0x02 */ s16 unk2;
        /* 0x04 */ s16 unk4;
        /* 0x06 */ s16 unk6;
        /* 0x08 */ s16 unk8;
        /* 0x0A */ s16 unkA;
    } rec[4];
} ScoreState;

typedef struct {
    /* 0x000 */ char pad0[0x1AA];
    /* 0x1AA */ s16 unk1AA;
} ScoreActor;

extern ScoreState *D_8008FDD4;
extern ScoreActor *D_800CC5EC[];
extern u16 D_8008FDC0;
extern s8 D_8008FDC8;
extern s8 D_8008FD7C;
extern s8 D_8008FE44[];
extern s8 D_800E0C00[];
extern s8 D_800E0AC0[];
extern s16 D_800E0AA0[];
extern s16 D_800E0AD0[];
extern u8 D_800ABA90[][4];
extern u8 D_800ABAA8;
extern u8 D_800ABAAC;
extern u8 D_800ABAB0;
extern u8 D_800ABAB8;
extern u8 D_800ABABC;
extern u8 D_800ABAC0;
extern u8 D_800ABAC4;
extern u8 D_843;
extern u8 D_888;
Gfx *func_151E9D18(Gfx *, s32, s32);


extern u8 D_D10;
extern u8 D_800ABAA0;
extern u8 D_800ABAA4;

Gfx *func_151E9D18(Gfx *gfx, s32 arg1, s32 arg2) {
    u8 *tex;
    s32 tOffset;
    s32 image;
    s32 width;
    s32 amount;
    s32 total0;
    s32 total1;
    s32 y;
    s32 sOffset0;
    s32 sOffset1;
    s32 i;

    y = arg1 - 0x10;
    if ((D_8008FDC0 & 0x4000) || (D_8008FDD4->unk42 == 8)) {
        tex = (D_8008FDD4->unk42 == 8) ? &D_D10 + 2 : &D_D10 + 1;
        width = 0x10;
        sOffset0 = 0;
        sOffset1 = 0x200;
    } else {
        tex = &D_D10;
        width = 0x20;
        sOffset0 = 0x200;
        sOffset1 = 0;
    }

    image = func_1510D0EC(tex, 0, 3, 0);
    if (image == (s32)0x80000000) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, (width * 32) - 1, 0);
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
               G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, (width - 1) << 2, 0x7C);
    gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);

    tOffset = 0;
    total0 = 0;
    total1 = 0;
    if (arg2 != 0) {
        if (D_8008FDC0 & 0x6040) {
            total0 = func_150859AC(0, 6);
            total1 = func_150859AC(1, 6);
        } else if (D_8008FDC0 & 0x100) {
            for (i = 0; i < D_8008FD8C; i++) {
                amount = func_150859AC(i, 3);
                if (amount < 0) {
                    amount = 0;
                }
                if (D_800CC2D0[i].unk128 == 0) {
                    total0 += amount;
                } else {
                    total1 += amount;
                }
            }
        } else {
            tOffset = 0x200;
            for (i = 0; i < D_8008FD8C; i++) {
                amount = D_8008FDD4->unk46[i];
                if (amount < 0) {
                    amount = 0;
                }
                if (D_800E0C00[i] == 0) {
                    total0 += amount;
                } else {
                    total1 += amount;
                }
            }
        }
        D_800E0AA0[0] = total0;
        D_800E0AA0[1] = total1;
    } else {
        total0 = D_800E0AA0[0];
        total1 = D_800E0AA0[1];
    }

    gfx = func_151E86E4(gfx, 0x108, y, 0x148, y + 0x40, 0, sOffset0, tOffset, 0x400, 0x400);
    gfx = func_151E86E4(gfx, 0x318, y, 0x358, y + 0x40, 0, 0, tOffset + sOffset1, 0x400, 0x400);
    func_1504332C(0xC0, 0xC0, 0xC0, 0xFF);
    func_15042D94(0x4F, (y >> 2) + 1, 0x80, &D_800ABAA0, total1);
    func_15042D94(0xD3, (y >> 2) + 1, 0x80, &D_800ABAA4, total0);
    return gfx;
}

Gfx *func_151EA15C(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    s32 image;
    s32 y;
    s32 i;
    s32 c;
    s32 v;
    s32 xbase;
    s32 x;
    s32 yq;
    s32 strBase;
    s32 yq2;
    s32 pct;
    s32 total;
    s32 sIdx;
    s8 flagA;
    s8 flagB;
    s8 flagC;

    if ((D_8008FDD4->unk42 == 0xB) || (D_8008FDD4->unk42 == 0xC)) {
        if (arg3 != 0) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, arg2);
        gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
        image = func_1510D0EC(&D_843, 0, 3, 0);
        if (image == (s32)0x80000000) {
            return gfx;
        }
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        y = 0x48;
        for (i = 0; i < 4; i++) {
            if (D_8008FE44[i] >= 0) {
                v = D_8008FE44[i] & 3;
                gfx = func_151E86E4(gfx, 0x120, (y - 2) << 2, 0x160, (y + 0xE) << 2, 0,
                                    (v & 1) << 9, (v >> 1) << 9, 0x400, 0x400);
                y += 0x12;
            }
        }
        return gfx;
    }

    y = arg1;
    if (D_8008FDC0 & 1) {
        xbase = 0x1A;
    } else {
        xbase = 0x34;
    }
    if (D_800E0B94 != 0) {
        if (D_8008FDC8 != 0) {
            y = arg1 - 0x28;
        }
    }
    func_1504332C(0xFF, 0xFF, 0xFF, arg2);
    if (D_800E0BD3 == 2) {
        strBase = 9;
        flagC = 1;
    } else {
        strBase = 0x5E;
        flagC = 0;
    }
    if (D_8008FDD4->unk42 == 8) {
        flagA = 0;
    } else {
        flagA = 1;
    }
    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, arg2);
    gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
    flagB = 0;
    for (i = 0; i < 4; i++) {
        if (D_8008FE44[i] < 0) {
            continue;
        }
        if (D_80087270[i] == 0xA) {
            continue;
        }
        c = D_800E0C00[i];
        if ((c != 0) || ((c == 0) && (flagA != 0))) {
            if (flagB == 0) {
                image = func_1510D0EC(&D_887, 0, 3, 0);
                if (image == (s32)0x80000000) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
                x = (xbase - 0x28) << 2;
                if (D_8008FDC0 & 1) {
                    yq = (y - 0x11) << 2;
                    gfx = func_151E86E4(gfx, x + 0x1D4, yq, x + 0x214, yq + 0x40, 0, 0x200, 0, 0x400, 0x400);
                    x += 0x78;
                }
                yq = (y - 0x11) << 2;
                x += 0x1D4;
                if (D_8008FDD4->unk42 == 1) {
                    x -= 0x78;
                }
                gfx = func_151E86E4(gfx, x + 0x164, yq, x + 0x1A4, yq + 0x3C, 0, 0, 0x200, 0x400, 0x400);
                if (D_8008FDD4->unk42 == 1) {
                    x += 0x78;
                }
                gDPPipeSync(gfx++);
                image = func_1510D0EC(&D_888, 0, 3, 0);
                if (image == (s32)0x80000000) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                yq2 = yq + 0x40;
                gfx = func_151E86E4(gfx, x, yq, x + 0x40, yq2, 0, 0x200, 0, 0x400, 0x400);
                x += 0x74;
                gfx = func_151E86E4(gfx, x, yq, x + 0x40, yq2, 0, 0, 0, 0x400, 0x400);
                if (D_8008FDD4->unk42 != 1) {
                    x += 0x4C;
                    gfx = func_151E86E4(gfx, x, yq, x + 0x80, yq2, 0, 0, 0x200, 0x400, 0x400);
                }
                gDPPipeSync(gfx++);
                image = func_1510D0EC(&D_843, 0, 3, 0);
                if (image == (s32)0x80000000) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
                flagB = 1;
            }
            func_1504332C(D_800ABA90[c][0], D_800ABA90[c][1], D_800ABA90[c][2], arg2);
            pct = 0;
            v = D_8008FE44[i] & 3;
            gfx = func_151E86E4(gfx, (xbase + 8) << 2, (y - 2) << 2, (xbase + 0x18) << 2, (y + 0xE) << 2, 0,
                                (v & 1) << 9, (v >> 1) << 9, 0x400, 0x400);
            if (arg3 != 0) {
                total = ((ScoreActor *)*(s32 *)((u8 *)D_800CC5EC + (i * 0x32C)))->unk1AA;
            } else {
                total = D_8008FDD4->rec[i].unk6;
            }
            if (total > 0) {
                pct = (D_8008FDD4->rec[i].unk0 * 100) / total;
            }
            if (pct >= 10000) {
                pct = 9999;
            }
            if (D_8008FDC8 != 0) {
                c = i;
            }
            x = xbase;
            sIdx = (s8)(D_800E0AC0[c] - 1);
            if (sIdx < 0) {
                sIdx = 0;
            }
            func_15042D94(x + 0x3C, y, 0x81, &D_800ABAA8, D_800E0BD8[sIdx + strBase]);
            if (D_8008FDC0 & 1) {
                func_15042D94(xbase + 0x54, y, 0x81, &D_800ABAAC, D_800E0AA0[c]);
                x = xbase + 0x1E;
            }
            x += 0x58;
            func_15042D94(x, y, 0x81, &D_800ABAB0, pct);
            x += 0x1A;
            func_15042D94(x, y, 0x81, &D_800ABAB8, D_8008FDD4->unk46[i]);
            x += 0x1E;
            if (D_8008FDD4->unk42 != 1) {
                if ((D_80087270[i] == 9) || ((i == 0) && (D_8008FD7C == 9))) {
                    func_15042D94(x, y, 0x81, &D_800ABABC);
                } else {
                    func_15042D94(x, y, 0x81, &D_800ABAC0, D_8008FDD4->rec[i].unk4);
                }
                x += 0x1E;
            }
            func_15042D94(x, y, 0x81, &D_800ABAC4, D_800E0AD0[i]);
            y += 0xF;
        }
    }
    if (D_8008FDC0 & 0x6340) {
        gSPDisplayList(gfx++, &D_80090028);
        gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
        gfx = func_151E9D18(gfx, 0x320, flagC);
    }
    if (arg3 != 0) {
        if (D_8008FDC0 & 1) {
            gfx = func_151E966C(gfx, 0x320, -1, 1, flagC);
        }
    }
    return gfx;
}

typedef struct {
    /* 0x00 */ s32 v[7];
} PlaceValueTable;

extern void *D_80090074[];
extern PlaceValueTable D_8009009C;

Gfx *func_151EADFC(Gfx *gfx, s32 x, s32 y, s32 value) {
    s32 i;
    s32 digit;
    s32 started;
    s32 image;
    PlaceValueTable places;

    places = D_8009009C;

    x <<= 2; y <<= 2;
    if (value >= 10000000) {
        value = 9999999;
    }
    started = 0;
    if (value < 0) {
        value = 0;
    }

    for (i = 6; i >= 0; i--) {
        digit = value / places.v[i];
        value = value % places.v[i];
        if ((digit > 0) || started || (i == 0)) {
            image = func_1510D0EC(D_80090074[digit], 0, 3, 0);
            started = 1;
            if (image != (s32)0x80000000) {
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gfx = func_151E86E4(gfx, x, y, x + 0x80, y + 0x80, 0, 0, 0, 0x400, 0x400);
            }
            x += 0x60;
        }
    }

    return gfx;
}
typedef struct {
    /* 0x00 */ Gfx **unk0;
    /* 0x04 */ Gfx *unk4[4];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16[6];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ f32 unk28[2][4][4];
} GameStruct151ED90C;
typedef struct {
    /* 0x00 */ void *img;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 width;
    /* 0x08 */ u16 height;
    /* 0x0A */ u8 fmt;
    /* 0x0B */ u8 siz;
} Tex151EBB50;
extern Tex151EBB50 D_80090060;
extern u8 D_1647;
extern u8 D_1648;
extern u8 D_1653;
extern u8 D_1654;
extern u8 D_7D3;
extern u8 D_7D4;
extern u8 D_7F1;
extern u8 D_7F2;
extern u8 D_7F6;
extern u8 D_7FC;
extern u8 D_80E;
extern u8 D_82F;
extern u8 D_830;
extern u8 D_891;
extern s32 *D_800891BC[];
extern s8 D_8008FEF0;
extern GameStruct151ED90C *D_8009005C;
extern u8 D_800BEAC1;
extern s16 D_800C3C9E;
typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ s32 unk64;
} Struct1509B570;
s32 func_151E564C(void);
Gfx *func_151E89A0(Gfx *, s32, s32);
void func_1510D694(s32);
s32 func_1509BE40(s32, s32, s32);
Struct1509B570 *func_1509B570(s32);
Gfx *func_150900F0(Gfx *, s32);

extern s16 D_8008FDCC;
extern s16 D_800E0C30;
extern Mtx D_800E0C38;
void func_151EDB58();
GameStruct151ED90C *func_151ED90C(s32, s32, s32, f32);
Gfx *func_151EDBDC(Gfx *, GameStruct151ED90C *, f32, f32, f32);
Gfx *func_151EA15C(Gfx *, s32, s32, s32);
Gfx *func_151EADFC(Gfx *, s32, s32, s32);

#define OBJ58 (*(GameStruct151ED90C **)&D_80090058)
Gfx *func_151EB06C(Gfx *gfx) {
    s32 show;
    s32 value;
    s32 x;
    s32 hasValue;
    s32 i;
    s32 start;
    extern s32 func_1510D0EC();
    static s8 D_800900B8 = 0;

    show = (D_800D2E4C->unk19 & 4) || (D_8008FDA8 < 0);
    if (D_800BE616 && !D_800BEAC0 && !D_800BEAC1) {
        if (!func_151E564C() || D_8008FDD4->unk42 == 1) {
            gfx = func_151E89A0(gfx, 0, 0xFF);
        }
    } else {
        D_800E0C78 = 0;
    }
    if (!D_800BEAC0 || D_8008FEF0) {
        if (OBJ58 != 0) {
            func_151EDB58(OBJ58);
            func_151EDB58(D_8009005C);
            OBJ58 = 0;
        }
        D_8008FDCC = 0;
        D_800900B8 = 0;
        return gfx;
    }
    if (D_800B0DF0->unk8 == 4 && D_800900B8 == 0) {
        if (D_800BE9F0 == 0x13) {
            s32 id;
            id = D_800891BC[D_800B0DF0->unk9][2];
            for (i = 0; i < 0x48; i++, id++) {
                if (id != (s32)&D_1647 && id != (s32)&D_1648 && id != (s32)(&D_1648 + 1) &&
                    id != (s32)&D_1653 && id != (s32)&D_1654 && id != (s32)(&D_1654 + 1)) {
                    func_1510D694(id);
                }
            }
            start = 0x4E;
        } else {
            start = 0;
        }
        for (i = start; i < 0x168; i++) {
            func_1510D694(D_800891BC[D_800B0DF0->unk9][2] + i);
        }
    }
    if (D_800900B8 < 5) {
        D_800900B8++;
        return gfx;
    }
    D_8008FDCC += 0x20;
    if (D_8008FDCC >= 0x100) {
        D_8008FDCC = 0xFF;
    }
    gSPDisplayList(gfx++, &D_80090028);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
    D_80090060.img = &D_891;
    {
    s32 remaining;
    s32 pad;
    func_1510D0EC(&D_7D3, &remaining, 3, 0);
    func_1510D0EC(&D_7D4, &remaining, 3, 0);
    func_1510D0EC(&D_7FC, &remaining, 3, 0);
    func_1510D0EC((&D_7FC + 1), &remaining, 3, 0);
    func_1510D0EC(&D_7F1, &remaining, 3, 0);
    func_1510D0EC(&D_7F2, &remaining, 3, 0);
    func_1510D0EC(&D_82F, &remaining, 3, 0);
    func_1510D0EC(&D_830, &remaining, 3, 0);
    if (D_800BE616) {
        func_1510D0EC(&D_80E, &remaining, 3, 0);
        func_1510D0EC((&D_80E + 1), &remaining, 3, 0);
    }
    if (!D_800BE616) {
        f32 hw;
        value = func_150859AC(0, 6);
        if (value == 0x7D00) {
            value = 1000000;
        }
        if (func_1509BE40(0, 0x5082, 0x1A) && !func_1509BE40(0, 0x5083, 0x1A)) {
            Struct1509B570 *p = func_1509B570(0x83);
            if (p != NULL) {
                if (p->unk64 == 0) {
                    value = 0;
                } else if (p->unk64 < 3) {
                    value = (s32)(value * (u32)p->unk64) / 3;
                }
            } else {
                value = 0;
            }
        }
        if (D_800C35EA == 1 && D_800C3C9E != -1) {
            if (D_800C3C9E == 0) {
                value = 0;
            } else {
                value = (D_800C3C9E == 0x7D00) ? 1000000 : value - D_800C3C9E;
            }
        }
        if (value > 0) {
            hasValue = 1;
        } else {
            hasValue = 0;
        }
        gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x3C, 3, 1, 1.0f, 0);
        if (OBJ58 == 0) {
            OBJ58 = func_151ED90C(0xA4, 0x17, 0, 1.0f);
            D_8009005C = func_151ED90C(0xA2, 8, 0, 1.0f);
        }
        gSPSetGeometryMode(gfx++, 0x220404);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
        gfx = func_150900F0(gfx, 1);
        hw = ((f32 *)D_800BE628)[1] * 0.5f;
        func_151EF954((f32 (*)[4])&D_800E0C38, -hw, hw, -(((f32 *)D_800BE628)[2] * 0.5f), ((f32 *)D_800BE628)[2] * 0.5f, 1.0f, 20000.0f, 1.0f);
        D_800E0C30 = D_8008FDCC;
        if (OBJ58 != 0) {
            i = 10;
            if (hasValue) {
                x = -0x10;
                while (i < value) {
                    x -= 0x10;
                    i *= 10;
                }
                gfx = func_151EDBDC(gfx, OBJ58, x + -10.0f, -83.0f, 0.2699999809f);
            }
        }
        if (D_8009005C != 0 && show) {
            gfx = func_151EDBDC(gfx, D_8009005C, 70.0f, 80.0f, 0.3000000119f);
        }
        gSPDisplayList(gfx++, &D_80090028);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        if (show) {
            s32 n;
            n = func_150859AC(0, 3);
            if (n != 0) {
                n--;
            }
            if (n >= 100) {
                n = 99;
            }
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
            gfx = func_151EADFC(gfx, 0xEC, 0x1E, n);
        }
        if (hasValue) {
            s32 pad4;
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
            D_80090060.img = &D_7F6;
            gfx = func_151EADFC(func_151ED430(gfx, &D_80090060, x + 0xA5, 0xAA, 1, 1, 1.0f, 0), x + 0xAD, 0x9B, value);
        }
    } else {
        gfx = func_151EA15C(func_151ED430(gfx, &D_80090060, 0x94, 0x12, 3, 1, 1.0f, 0), 0x85, D_8008FDCC, 1);
    }
    }
    return gfx;
}

extern s16 D_8008FDCC;
Gfx *func_151EA15C(Gfx *, s32, s32, s32);

s32 func_151EB930(s32 arg0) {
    if (D_8008FDCC != 0) {
        arg0 = (s32)func_151EA15C((Gfx *)arg0, 0x6A, D_8008FDCC, 0);
    }
    return arg0;
}

extern s16 D_800E0A80;
extern s32 D_800E0A90;

s32 func_151EB96C(s32 arg0) {
    s32 pad;
    s16 alpha;
    s16 i;
    s32 count;
    u8 a;
    s32 spacing;
    s32 x;
    s32 x0;
    u8 *str;
    if (D_800E0A80 < 0) {
        return arg0;
    }
    {
        if (D_800E0A90 < 0x168) {
            alpha = D_800E0A90 << 2;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        } else {
            alpha = 0x1A9 - D_800E0A90;
            alpha <<= 2;
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
        i = D_800E0A80;
        count = 0;
        a = alpha;
        while (((u8 **)D_800E0BD8)[i][0] != 0x2A) {
            i++;
            count++;
        }
        if (count >= 0xE) {
            spacing = 0xB;
        } else {
            spacing = 0x11;
        }
        func_1504332C(0xFF, 0, 0, a);
        x = (0xD0 - count * spacing) >> 1;
        i = D_800E0A80;
        str = ((u8 **)D_800E0BD8)[i];
        x0 = x;
        while (*str != 0x2A) {
            func_15042D94(0x94, x, 1, str);
            func_1504332C(0xFF, 0xFF, 0xFF, a);
            if (x == x0) {
                x += 0xC;
            }
            i++;
            str = ((u8 **)D_800E0BD8)[i];
            x += spacing;
        }
    }
    return arg0;
}

extern Tex151EBB50 D_80090060;
extern u8 D_800917EC;
extern u8 D_85D;
extern u8 D_89C;
extern f32 D_800ABAE8;
extern s32 D_800E0A90;
extern s32 D_800BE620;
extern s32 D_800BE624;

#define MIN(a, b) ((b) < (a) ? (b) : (a))
Gfx *func_151EBB50(Gfx *gfx) {
    s32 alpha;
    f32 scale;

    alpha = 0xFF - ((D_800E0A90 - 0xF0) << 3);
    if (alpha < 0) {
        alpha = 0;
    }
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x082C3F, 0x504340);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFEFB7D);
    gDPSetEnvColor(gfx++, 0, 0, 0, 0xFF);
    gfx = func_1501A6CC(gfx, 0, 0, D_800BE620, D_800BE624);
    if (D_80000300 == 0) {
        return gfx;
    }
    gSPDisplayList(gfx++, &D_80090028);
    if (D_800E0A90 < 0xF0) {
        alpha = 0xFF - ((D_800E0A90 - 0xB4) << 3);
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        scale = D_800E0A90 / 180.0f + 0.75f;
        if (scale > 1.5f) {
            scale = 1.5f;
        }
        func_150432BC(scale);
        func_1504332C(0xFF, 0, 0, alpha);
        func_15042D94(0x94, 0x69, 1, D_800E0BD8[0x238 / 4]);
        func_150432BC(1.0f);
    } else if (D_800E0A90 < 0x21C) {
        if (D_800E0A90 < 0x12C) {
            alpha = D_800E0A90 - 0xF0;
            alpha = (alpha << 3);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        } else {
            alpha = 0xFF - ((D_800E0A90 - 0x1E0) << 3);
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha >> 2);
        gfx = func_151ED430(gfx, &D_800917F8, 0x92, 0x67, 5, 6, 1.0f, 0);
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gfx = func_151ED430(gfx, &D_800917EC, 0x92, 0x1C, 2, 1, 1.0f, 0);
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(0x94, 0x9D, 1, D_800E0BD8[0x204 / 4]);
        func_15042D94(0x94, 0xAA, 1, D_800E0BD8[0x208 / 4]);
        func_15042D94(0x94, 0xBC, 1, D_800E0BD8[0x20C / 4]);
        func_150432BC(D_800ABAE8);
        func_15042D94(0x94, 0x2D, 1, D_800E0BD8[0x210 / 4]);
        func_15042D94(0x94, 0x37, 1, D_800E0BD8[0x214 / 4]);
        func_15042D94(0x94, 0x41, 1, D_800E0BD8[0x218 / 4]);
        func_15042D94(0x94, 0x4B, 1, D_800E0BD8[0x21C / 4]);
        func_15042D94(0x94, 0x55, 1, D_800E0BD8[0x220 / 4]);
        func_15042D94(0x94, 0x5F, 1, D_800E0BD8[0x224 / 4]);
        func_15042D94(0x94, 0x69, 1, D_800E0BD8[0x228 / 4]);
        func_15042D94(0x94, 0x73, 1, D_800E0BD8[0x22C / 4]);
        func_15042D94(0x94, 0x7D, 1, D_800E0BD8[0x230 / 4]);
        func_15042D94(0x94, 0x87, 1, D_800E0BD8[0x234 / 4]);
        func_150432BC(1.0f);
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        D_80090060.img = &D_89C;
        D_80090060.width = 0x10;
        D_80090060.height = 0x10;
        gfx = func_151ED430(gfx, &D_80090060, 0xC7, 0xB0, 1, 1, 1.0f, 0);
        D_80090060.width = 0x20;
        D_80090060.height = 0x20;
    } else {
        if (D_800E0A90 < 0x258) {
            alpha = D_800E0A90 - 0x21C;
            alpha = (alpha << 3);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        } else {
            alpha = 0xFF - ((D_800E0A90 - 0x30C) << 3);
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
        gDPSetEnvColor(gfx++, 0xFF, 0, 0, alpha);
        D_80090060.fmt = 4;
        D_80090060.siz = 1;
        D_80090060.img = &D_85D;
        D_80090060.width = 0x40;
        D_80090060.height = 0x40;
        gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x69, 3, 1, 1.0f, 0);
        D_80090060.width = 0x20;
        D_80090060.height = 0x20;
        D_80090060.fmt = 0;
        D_80090060.siz = 3;
    }
    return gfx;
}

extern s32 D_800E0A90;
extern char **D_800E0BD8;
void func_15042D94();

s32 func_151EC178(s32 arg0) {
    s32 temp;
    if (D_800E0A90 >= 0x5DD) {
        temp = (D_800E0A90 - 0x5DC) << 3;
        if (temp >= 0x100) {
            temp = 0xFF;
        }
        func_1504332C(0xFF, 0xFF, 0xFF, temp);
        func_15042D94(0xDC, 0x130, 1, D_800E0BD8[0x1D0 / 4]);
    }
    return arg0;
}

Gfx *func_151EC1F0(Gfx *gfx) {
    s32 alpha;
    s32 temp;

    if (D_800BE9F0 == 0x21) {
        return gfx;
    }

    gSPDisplayList(gfx++, &D_80090028);
    temp = D_800E0A90;
    alpha = temp;
    if (alpha >= 0x12D) {
        temp = 0x1AC;
        alpha = temp - alpha;
        alpha <<= 1;
        if (alpha < 0) {
            alpha = 0;
        }
    } else {
        alpha = temp << 3;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }

    if (alpha != 0) {
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gfx = func_151ED430(gfx, &D_800917F8, 0x92, 0x63, 5, 6, 1.0f, 0);
    }

    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
    gfx = func_151ED430(gfx, &D_80091804, 0x92, 0xCB, 5, 2, 1.0f, 0);
    gDPSetCombine(gfx++, 0xFFD3FF, 0xFFA6FF7F);
    gDPSetEnvColor(gfx++, 0x20, 0xFF, 0x20, D_800E0B97);
    gfx = func_151ED430(gfx, &D_80091810, 0x92, 0xCB, 5, 2, 1.0f, 0);
    gfx = func_15096934(gfx);

    alpha = 0x1EA - D_800E0A74;
    if (alpha < 0) {
        alpha = 0;
    } else {
        alpha <<= 4;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }
    alpha = 0xFF - alpha;
    D_800E0B96 = alpha;
    return gfx;
}

typedef struct {
    /* 0x00 */ char pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ char padD[3];
} SaveSlot215960; /* size 0x10 */

extern u8 D_800E0A95;
extern s8 D_8008FEF8;
extern s32 D_800900BC[];
Gfx *func_151EC648(Gfx *);
Gfx *func_151EE184(Gfx *);
Gfx *func_151EEBE8(Gfx *, s32);

Gfx *func_151EC3E8(Gfx *gfx) {
    static u8 D_800900D4;
    s32 alpha;

    if (D_800C35EA == 1) {
        D_800900D4 = 0;
        return gfx;
    }
    alpha = D_800900D4 + (D_800BE9E4 * 4);
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    D_800900D4 = alpha;
    gSPDisplayList(gfx++, &D_80090028);
    if (D_8008FDD4->unk3E == 1) {
        return func_151EC648(gfx);
    }
    if (D_8008FDD4->unk3E == 0) {
        if (D_8008FDD4->unk2C == 1 && D_8008FEF8 != 0) {
            gfx = func_151EE184(gfx);
        } else {
            alpha = (D_8008FDD4->unk0 - 0.5f) * 512.0f;
            if (alpha < 0) {
                alpha = -alpha;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            if (D_800900D4 < alpha) {
                alpha = D_800900D4;
            }
            if (D_8008FDD4->unk2C == 1) {
                alpha = (D_800E0A95 * alpha) >> 8;
                if (alpha >= 0xFE) {
                    alpha = 0xFF;
                }
            }
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
            *(s32 *)&D_80090060 = D_800900BC[D_8008FDD4->unk2C];
            gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x1E, 3, 1, 1.0f, 0);
        }
        if (D_8008FDD4->unk8 >= 0.5f) {
            if (D_8008FDD4->unk2C >= 3) {
                if (((SaveSlot215960 *)&D_800BE3F8)[D_8008FDD4->unk2C - 3].unk8 != -1) {
                    gfx = func_151EEBE8(gfx, ((SaveSlot215960 *)&D_800BE3F8)[D_8008FDD4->unk2C - 3].unkC);
                }
            }
        }
    }
    return gfx;
}

extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 D_8008FDE8;
extern s8 D_8008FE2C;
extern s8 D_8008FE40;
extern u8 *D_800900D8[];
extern u8 *D_800900FC[];
extern s8 D_800E0B90[];
extern struct127 *D_800E0BA0[];
extern u8 D_800E0C88[];
extern u8 D_87B;
extern u8 D_88C;
s32 func_151E24F0(s32 *);
void func_151E7F60(s32, s32);

Gfx *func_151EC648(Gfx *gfx) {
    static s8 D_800900F8 = 0;
    s32 a;
    s32 alpha;
    s32 pad;
    s32 y;
    ScoreState *s;
    f32 scale;
    f32 f;
    u8 **pp;
    s8 n;
    s8 m;
    s32 i;
    s32 j;
    s32 prev;
    s32 cur;
    s32 mid;
    s32 v;
    s32 env;
    f32 angle;
    s32 diff;
    f32 sc;

    a = (D_8008FDD4->unk0 - 0.5f) * 512.0f;
    if (a < 0) {
        a = -a;
    }
    if (a >= 0x100) {
        a = 0xFF;
    }
    gDPPipeSync(gfx++);
    s = D_8008FDD4;
    f = s->unk8;
    if (0.0f == s->unk8 && 0.0f == s->unkC) {
        m = s->unk2C;
        D_800900F8 = m;
    } else {
        m = s->unk2C;
        if (m == 0 && s->unkC < 0.0f) {
            D_800900F8 = 4;
        }
    }
    if (m == 0) {
        y = 0x6A;
        alpha = 0xFF - (s32)(f * 300.0f);
    } else {
        alpha = a;
        if (D_800900F8 == 4) {
            sc = 68.0f;
        } else {
            sc = 76.0f;
        }
        y = 0x6A - (s32)(f * sc);
    }
    if (s->unk40 != 3) {
        if (alpha > 0 && *(pp = &D_800900D8[D_800900F8]) != NULL) {
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            diff = 2;
            if (D_800900F8 == 4) {
                scale = 0.8000000119f;
                v = 2;
            } else {
                scale = 1.0f;
                v = 1;
                if (D_800900F8 != 0) {
                    diff = 3;
                }
            }
            D_80090060.img = *pp;
            gfx = func_151ED430(gfx, &D_80090060, 0x94, y, diff, v, scale, 0);
            s = D_8008FDD4;
            f = s->unk8;
        }
        alpha = 0xFF - (s32)(f * 300.0f);
        f = 2.0f;
        if (alpha > 0) {
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            D_80090060.img = &D_88C;
            gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x1E, 3, 1, 1.0f, 0);
            s = D_8008FDD4;
        }
    }
    switch (s->unk40) {
    case 0:
        D_800E0C88[0] = 0;
        D_800E0C88[1] = 0xFF;
        D_800E0C88[2] = 0xFF;
        D_800E0C88[3] = 0xFF;
        if (s->unk8 >= 0.5f && s->unk2C == 4) {
            env = (D_800E0A94 * D_800E0A95) >> 8;
            if (env >= 0xFE) {
                env = 0xFF;
            }
            angle = 0.6283185482f;
            angle += 0.6283185482f + 0.6283185482f * D_8008FDE8;
            prev = 0x7E - (s32)(cosf(angle) * 52.0f);
            j = D_8008FDD4->unk41 - 4;
            if (j < 0) {
                j += 5;
            }
            for (i = 1; i != 8; i++) {
                angle -= 0.6283185482f;
                cur = 0x7E - (s32)(cosf(angle) * 52.0f);
                if (prev < cur) {
                    diff = cur - prev;
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, env);
                    mid = (diff >> 1) + prev;
                    sc = diff / 32.0f;
                    D_80090060.img = D_800900FC[j];
                    gfx = func_151ED430(gfx, &D_80090060, 0x53, mid, 2, 1, sc, -1);
                    if (D_8008FDE8 == 0.0f && i == 5 && D_8008FE2C == 0) {
                        v = (D_800BE9AC * 2) & 0x7F;
                        if (v >= 0x40) {
                            v = 0x7F - v;
                        }
                        v = (s32)((u32)env * (v + v + v + 0x3F)) >> 8;
                        if (v >= 0x100) {
                            v = 0xFF;
                        }
                        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, v);
                        D_80090060.img = D_800900FC[j] + 2;
                        gfx = func_151ED430(gfx, &D_80090060, 0x53, mid, 2, 1, sc, -1);
                    }
                }
                j++;
                if (j >= 5) {
                    j -= 5;
                }
                prev = cur;
            }
        }
        break;
    case 4: {
        s32 x;
        s32 dir;
        s32 count;
        s32 pulse;
        s32 i4;
        s32 idx;
        s32 vals[3];
        struct127 **p;

        sc = (D_800BE9AC & 0x3F) * 0.0981746912f;
        sc = sinf(sc + 0.7853975296f);
        pulse = (D_800E0A95 * (s32)((sc + 1.0f) * 127.0f)) >> 8;
        if (D_8008FDD4->unk2C == 7) {
            x = 0x70;
            n = 2;
        } else {
            x = 0x28;
            n = 4;
        }
        count = func_151E24F0(vals);
        if (count != 0) {
            idx = 0;
        }
        for (i4 = 0; i4 < n; i4++) {
            mid = -1;
            dir = 1;
            for (v = 0; v < D_8008FE40; v++) {
                if (i4 == D_8008FE44[v]) {
                    mid = v;
                    v = D_8008FE40;
                    dir = -1;
                }
            }
            if (i4 != 0 && D_800E0BA0[0] != 0) {
                p = &D_800E0BA0[i4];
                if (D_800E0C88[i4] != 0xFF) {
                    if (*p == 0 && mid != -1) {
                        func_151E7F60(i4, D_800E0B90[mid]);
                    }
                } else if (*p != 0) {
                    func_15060F28(*p, 1);
                    *p = 0;
                }
            }
            alpha = D_800E0C88[i4];
            alpha += dir * (D_800BE9E4 << 3);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            } else if (alpha < 0) {
                alpha = 0;
            }
            D_800E0C88[i4] = alpha;
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A95);
            D_80090060.img = &D_89C + 2;
            y = 0x8C - (s32)(alpha * 0.1428571492f);
            gfx = func_151ED430(gfx, &D_80090060, x, y, 1, 1, 1.0f, 0);
            if (alpha > 0) {
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, pulse);
                gfx = func_151ED430(gfx, &D_80090060, x, y, 1, 1, 1.0f, 1);
            }
            if (alpha > 0) {
                gDPPipeSync(gfx++);
                alpha = (D_800E0A95 * alpha) >> 8;
                if (alpha >= 0xFE) {
                    alpha = 0xFF;
                }
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
                D_80090060.img = &D_87B;
                gfx = func_151ED430(gfx, &D_80090060, x, 0x82, 1, 1, 1.0f, 0);
            }
            func_1504332C(0xFF, 0xFF, 0xFF, D_800E0A95);
            func_15042D94(x, 0x41, 0x80, "#", i4 + 1);
            if (count != 0 && mid >= 0 && D_8008FE44[mid] >= 0) {
                count--;
                func_15042D94(x, 0xA8, 0, "#", 0x38);
                func_15042D94(x, 0xA2, 0x81, "%d", vals[idx] + 1);
                idx++;
            }
            x += 0x48;
        }
    }
        break;
    case 3:
        gfx = func_151EA15C(gfx, 0x6A, 0xFF, 0);
        break;
    }
    return gfx;
}

extern u8 D_800E0B97;
extern Gfx D_80090028;
extern u8 D_8009181C;
extern u8 D_80091828;
Gfx *func_151ED430(Gfx *, void *, s32, s32, s32, s32, f32, s32);
Gfx *func_15096934(Gfx *);

Gfx *func_151ED09C(Gfx *gfx) {
    s32 alpha;

    gSPDisplayList(gfx++, &D_80090028);
    alpha = D_800E0A90 << 2;
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
    gfx = func_151ED430(gfx, &D_8009181C, 0x92, 0x6C, 8, 3, 1.0f, 0);
    gDPPipeSync(gfx++);
    gDPSetCombine(gfx++, 0xFFD3FF, 0xFFA6FF7F);
    gDPSetEnvColor(gfx++, 0xFF, 0x80, 0x20, (D_800E0B97 * (alpha + 1)) >> 8);
    gfx = func_151ED430(gfx, &D_80091828, 0x92, 0x6C, 8, 3, 1.0f, 0);
    return func_15096934(gfx);
}

Gfx *func_151ED1E0(Gfx *gfx) {
    if (D_800E0B96 == 0) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x082C3F, 0x504340);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFEFB7D);
    gDPSetEnvColor(gfx++, 0, 0, 0, D_800E0B96);
    return func_1501A6CC(gfx, 0, 0, D_800BE620, D_800BE624);
}

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 width;
    /* 0x08 */ u16 height;
    /* 0x0A */ u8 fmt;
    /* 0x0B */ u8 siz;
} GameTex151ED29C;

extern u8 D_8009DEB0[];
extern u8 D_8009DEB4[];
extern u8 D_8009DEB8[];
extern u8 D_8009DEBC[];

Gfx *func_151ED29C(Gfx *gfx, GameTex151ED29C *tex, s32 *sizeOut) {
    s32 masks;
    s32 maskt;
    s32 i;
    s32 fmt;
    s32 size;

    size = D_8009DEB0[tex->siz] + tex->width * tex->height;
    size >>= D_8009DEB4[tex->siz];
    *sizeOut = size;
    masks = 1;
    for (i = 2; i < tex->width; i <<= 1) {
        masks++;
    }
    maskt = 1;
    for (i = 2; i < tex->height; i <<= 1) {
        maskt++;
    }
    fmt = tex->fmt;
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, fmt, D_8009DEBC[tex->siz], 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTile(gfx++, fmt, tex->siz, (D_8009DEB8[tex->siz] * tex->width + 7) >> 3, 0, 0, 0, G_TX_CLAMP, maskt, 0, G_TX_CLAMP, masks, 0);
    gDPSetTileSize(gfx++, 0, 0, 0, (tex->width - 1) << 2, (tex->height - 1) << 2);
    return gfx;
}

typedef struct Game215960Sprite {
    s32 resource;
    u8 pad4[2];
    u16 width;
    u16 height;
    u8 format;
    u8 size;
} Game215960Sprite;

#define MIN_ED430(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_ED430(a, b) (((a) > (b)) ? (a) : (b))
#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

Gfx *func_151ED430(Gfx *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, s32 arg7) {
    s32 column;
    s32 row;
    s32 width;
    s32 height;
    s32 top;
    s32 blockSize;
    s32 resource;
    s32 dsdx;
    s32 dtdy;
    s32 address;
    s32 continued;
    s32 remaining;
    f32 scale;

    continued = 0;
    arg0 = func_151ED29C(arg0, arg1, &blockSize);
    if (arg7 < 0) {
        scale = 1.0f;
        arg7 = 0;
    } else {
        scale = arg6;
    }
    resource = ((Game215960Sprite *)arg1)->resource + arg7;
    width = (u32)((Game215960Sprite *)arg1)->width * scale * D_8008FE1C;
    height = (u32)((Game215960Sprite *)arg1)->height * arg6 * D_8008FE20;
    dtdy = 1024.0f / (arg6 * D_8008FE20);
    dsdx = 1024.0f / (scale * D_8008FE1C);
    arg2 = arg2 * D_8008FE1C;
    arg3 = arg3 * D_8008FE20;
    arg2 -= (width * arg4) >> 1;
    top = arg3 - ((height * arg5) >> 1);
    for (column = 0; column < arg4; column++) {
        arg3 = top;
        for (row = 0; row < arg5; row++) {
            if (!continued) {
                address = func_1510D0EC(resource, (s32)&remaining, 3, 0);
            }
            if (address != (s32)0x80000000) {
                gDPPipeSync(arg0++);
                gDPSetTextureImage(arg0++, ((Game215960Sprite *)arg1)->format, D_8009DEBC[((Game215960Sprite *)arg1)->size], 1, address);
                gDPLoadBlock(arg0++, G_TX_LOADTILE, 0, 0, blockSize - 1, 0);
                { Gfx *command = arg0++; command->words.w0 = (_SHIFTL(0xE4, 24, 8) | _SHIFTL(MAX_ED430((s16)(((arg2 + width) << 2)), 0), 12, 12) | _SHIFTL(MAX_ED430((s16)(((arg3 + height) << 2)), 0), 0, 12)); command->words.w1 = (_SHIFTL((0), 24, 3) | _SHIFTL(MAX_ED430((s16)((arg2 << 2)), 0), 12, 12) | _SHIFTL(MAX_ED430((s16)((arg3 << 2)), 0), 0, 12)); { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xE1, 24, 8); command->words.w1 = (u32)((_SHIFTL(((0) - (((s16)((arg2 << 2)) < 0) ? (((s16)(dsdx) < 0) ? (MAX_ED430((((s16)((arg2 << 2)) * (s16)(dsdx)) >> 7), 0)) : (MIN_ED430((((s16)((arg2 << 2)) * (s16)(dsdx)) >> 7), 0))) : 0)), 16, 16) | _SHIFTL(((0) - ((((arg3 << 2)) < 0) ? (((s16)(dtdy) < 0) ? (MAX_ED430((((s16)((arg3 << 2)) * (s16)(dtdy)) >> 7), 0)) : (MIN_ED430((((s16)((arg3 << 2)) * (s16)(dtdy)) >> 7), 0))) : 0)), 0, 16))); } { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xF1, 24, 8); command->words.w1 = (u32)((_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); } }
            }
            arg3 += height;
            if (remaining > 0x1000) {
                continued = 1;
                if (address != 0) {
                    address += (blockSize << ((Game215960Sprite *)arg1)->size) >> 1;
                }
                remaining -= (blockSize << ((Game215960Sprite *)arg1)->size) >> 1;
            } else {
                continued = 0;
                resource++;
            }
        }
        arg2 += width;
    }
    gDPPipeSync(arg0++);
    return arg0;
}
#undef MIN
#define MIN(a, b) ((b) < (a) ? (b) : (a))


extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1503F62C(s32, s32, void *, void *, void *, void *, void *);
extern void func_1503F5B8(s32, s32, s32, f32, f32, s32);

GameStruct151ED90C *func_151ED90C(s32 arg0, s32 arg1, s32 arg2, f32 arg3) {
    GameStruct151ED90C *obj;
    f32 (*mtx1)[4];
    f32 (*mtx2)[4];
    Gfx *dst;
    Gfx *src;
    s32 cmd;
    s32 size;
    s32 i;

    obj = allocate_memory(sizeof(GameStruct151ED90C), 1, 0, 1);
    if (obj == NULL) {
        return NULL;
    }
    obj->unk15 = 1;
    if (func_1503F62C(arg0, arg1, obj, &obj->unk14, &obj->unk1C, &obj->unk20, &obj->unk24) != 0) {
        func_10004074(obj);
        return NULL;
    }

    mtx1 = obj->unk28[0];
    guMtxIdentF(mtx1);
    mtx2 = obj->unk28[1];
    guMtxIdentF(mtx2);
    *(f32 (**)[4])(obj->unk24 + 0x3E0) = mtx1;
    *(f32 (**)[4])(obj->unk24 + 0x3E4) = mtx2;
    func_1503F5B8(obj->unk24, 1, arg2, arg3, 0.0f, 0);

    for (i = 0; i < obj->unk14; i++) {
        src = obj->unk0[i];
        size = 0;
        do {
            cmd = (src->words.w0 >> 24) & 0xFF;
            src++;
            size += 8;
        } while (cmd != 0xDF);

        dst = allocate_memory(size, 1, 1, 1);
        obj->unk4[i] = dst;
        if (dst == NULL) {
            for (size = 0; size < i; size++) {
                func_10004074(obj->unk4[size]);
            }
            func_10004074(obj);
            return NULL;
        }
    }

    for (i = 0; i < obj->unk14; i++) {
        dst = obj->unk4[i];
        src = obj->unk0[i];
        do {
            dst->words.w0 = src->words.w0;
            dst->words.w1 = src->words.w1;
            src++;
            cmd = (dst->words.w0 >> 24) & 0xFF;
            if (cmd == 0xEF) {
                dst->words.w0 &= 0xFFFEFFFF;
                dst->words.w1 = 0x5041C8;
            }
            if (cmd == 0xFC) {
                dst->words.w0 = 0;
            }
            dst++;
        } while (cmd != 0xDF);
    }

    return obj;
}

typedef struct {
    s32 unk0;
    s32 *unk4[4];
    u8 unk14;
    u8 pad15[0xF];
    s32 unk24;
} GameStruct151EDB58;

extern void func_1503F7B8(s32);

void func_151EDB58(GameStruct151EDB58 *arg0) {
    s32 i;

    if (arg0 != 0) {
        func_1503F7B8(arg0->unk24);
        func_100043B4((s32 *)arg0, 4);
        for (i = 0; i < arg0->unk14; i++) {
            func_100043B4(arg0->unk4[i], 4);
        }
    }
}

extern Mtx D_80089470;
extern Mtx D_800E0C38;
extern Gfx D_80090110;
extern Gfx D_80090128;
extern s16 D_800E0C30;
extern u8 D_507;
extern u8 D_508;
extern void func_1503F4B0(s32);

Gfx *func_151EDBDC(Gfx *gfx, GameStruct151ED90C *obj, f32 x, f32 y, f32 scale) {
    u8 *tex;
    Gfx *start;
    s32 image;
    s32 i;

    start = gfx;
    if (D_8008FE1C != 1.0f) {
        x *= D_8008FE1C;
        y *= D_8008FE20;
        scale *= D_8008FE1C;
    }

    guMtxIdentF(obj->unk28[D_800BE9C0]);
    obj->unk28[D_800BE9C0][3][0] = x;
    obj->unk28[D_800BE9C0][3][1] = y;
    obj->unk28[D_800BE9C0][3][2] = -100.0f;
    obj->unk28[D_800BE9C0][0][0] = scale;
    obj->unk28[D_800BE9C0][1][1] = scale;
    obj->unk28[D_800BE9C0][2][2] = scale;
    func_1503F4B0(obj->unk24);

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_800E0C38, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, &D_80090110);
    gDPSetCombine(gfx++, 0x12FE25, 0xFFFFFBFD);
    gSPLoadGeometryMode(gfx++, 0);
    gSPSetGeometryMode(gfx++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gDPSetPrimColor(gfx++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0C30);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, *(s32 *)(obj->unk24 + (D_800BE9C0 * 4) + 0x3E8));
    gSPSegment(gfx++, 1, obj->unk1C);

    if ((s32)obj == D_80090058) {
        obj->unk15--;
        if (obj->unk15 == 0) {
            obj->unk15 = (func_150ADA20() & 0x7F) + 0xF;
        }
        if (obj->unk15 < 7) {
            tex = &D_507;
        } else {
            tex = &D_508;
        }
        image = func_1510D0EC(tex, 0, 3, 0);
        if (image == (s32)0x80000000) {
            return start;
        }
        gSPSegment(gfx++, 6, image);
        gSPSegment(gfx++, 7, image);
    }

    for (i = 0; i < obj->unk14; i++) {
        gSPDisplayList(gfx++, obj->unk4[i]);
    }
    gSPDisplayList(gfx++, &D_80090128);
    return gfx;
}

typedef struct {
    Gfx *dl;
    s32 *data;
    s32 unk8;
} Struct151EDF4C;

extern Struct151EDF4C *D_80090138;
extern u8 *D_800E0C7C;
extern s8 D_800E0C80;
extern s8 D_800E0C81;
extern s8 D_800E0C82;
extern s8 D_800E0C83;
extern s8 D_800E0C84;
s32 *func_1502B6BC(s32, s32, s32, s32, s32, s32);
s32 func_1510CE60(Gfx *, s32, u32, s32, s32 *);

void func_151EDF4C(void) {
    s32 i;
    s32 j;
    s32 pad1;
    s32 pad2;
    s32 *data;
    Gfx *dl;
    s32 cmd;
    u8 *vtx;

    D_80090138 = NULL;
    D_800E0C7C = allocate_memory(0x1A8, 1, 0, 0);
    if (D_800E0C7C != NULL) {
        D_80090138 = (Struct151EDF4C *)(D_800E0C7C + 0x40);
        for (i = 0x1C5, j = 0; i < 0x1E3; i++, j++) {
            data = func_1502B6BC(0, 1, 0, 2, 9, i);
            D_80090138[j].data = data;
            D_80090138[j].dl = (Gfx *)*data;
            func_1510CE60(D_80090138[j].dl, 0, 1, 0x3E, &D_80090138[j].unk8);
            dl = D_80090138[j].dl;
            do {
                cmd = (dl->words.w0 >> 24) & 0xFF;
                if (cmd == 1) {
                    dl->words.w1 += (s32)data;
                    vtx = (u8 *)(((volatile Gfx *)dl)->words.w1 | 0x80000000);
                    vtx[0xC] = 0;
                    vtx[0xD] = 0;
                    vtx[0xE] = 0;
                    vtx[0xF] = 0xFF;
                    vtx += 0x10;
                    vtx[0xC] = 0;
                    vtx[0xD] = 0;
                    vtx[0xE] = 0;
                    vtx[0xF] = 0xFF;
                }
                if (cmd == 0xFC) {
                    dl->words.w0 = 0xFC127E05;
                    dl->words.w1 = 0xFFFFF3F8;
                }
                if (cmd == 0xEF) {
                    dl->words.w0 |= 0x100000;
                    dl->words.w1 = 0x0F0A4000;
                }
                dl++;
            } while (cmd != 0xDF);
        }
        D_800E0C80 = 0;
        D_800E0C81 = 0;
        D_800E0C82 = 0;
        D_800E0C83 = 0;
        D_800E0C7C[0] = 0;
        D_800E0C7C[0x20] = 0;
        D_800E0C84 = 0;
    }
}

extern u8 D_800ABAD4;
extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
extern void func_150A7D00(void *, f32, f32, f32);
extern struct216 *func_1507BB28(s32, s32);

Gfx *func_151EE184(Gfx *gfx) {
    s32 sfx;
    s32 changed;
    s32 ncols;
    s32 i;
    s32 key;
    struct127 *obj;
    Mtx *mtx;
    s32 j;
    u32 mask;
    u16 perspNorm;
    s32 temp;
    s32 cheatType;
    u8 c;
    s32 width;
    s32 height;
    s32 unused;
    u8 *p;
    u8 *q;

    sfx = 0;
    if (D_80090138 == NULL) {
        return gfx;
    }
    changed = 0;
    if (D_8008FDD4->unk20 & 1) {
        changed = 1;
        D_800E0C81--;
    }
    if (D_8008FDD4->unk20 & 2) {
        D_800E0C81++;
        if (D_800E0C81 == 4 && D_800E0C80 >= 5) {
            D_800E0C80 = 4;
        }
        changed = 1;
    }
    if (D_800E0C81 < 0) {
        D_800E0C81 = 5;
    }
    if (D_800E0C81 >= 6) {
        D_800E0C81 = 0;
    }
    if (D_8008FDD4->unk20 & 8) {
        D_800E0C80++;
        changed = 1;
    }
    if (D_8008FDD4->unk20 & 4) {
        D_800E0C80--;
        changed = 1;
    }
    if (D_800E0C81 == 4) {
        ncols = 5;
    } else if (D_800E0C81 == 5) {
        ncols = 1;
    } else {
        ncols = 6;
    }
    if (D_800E0C80 < 0) {
        D_800E0C80 = ncols - 1;
    }
    if (D_800E0C80 >= ncols) {
        D_800E0C80 = 0;
    }
    if (changed) {
        func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
    }
    key = D_800E0C81 * 6 + D_800E0C80;
    if (D_800E0C81 == 5) {
        key--;
    }
    mtx = allocate_memory(0x80, 4, 2, 0);
    if (D_800BE748[0] & 0x8000) {
        D_800E0C82 += 8;
    } else {
        D_800E0C82 -= 8;
    }
    if (D_800E0C82 < 0) {
        D_800E0C82 = 0;
    }
    if (D_800E0C82 >= 13) {
        D_800E0C82 = 12;
    }
    if (D_8008FDD4->unk20 & 0x10) {
        i = key + 'A';

        func_10010F30(0x500, 0x7D00, 0x40, 0, 0);
        if (D_800E0C81 == 5) {
            i = ' ';
        } else if (D_800E0C81 == 4) {
            if (D_800E0C80 == 0) {
                if (D_800E0C83 > 0) {
                    D_800E0C83--;
                }
                i = 0;
                D_800E0C7C[D_800E0C83] = 0;
            } else if (D_800E0C80 == 1) {
                i = '.';
            } else if (D_800E0C80 == 4) {
                mask = 0xFFC3FFFE;
                for (i = 0x90; i < 0xA3; i++) {
                    p = ((u8 **)D_800E0BD8)[i];
                    temp = i - 0x90;
                    if (temp < D_800E0C83) {
                        c = D_800E0C7C[temp];
                    } else {
                        c = ' ';
                    }
                    for (j = 0; j < 32; j++) {
                        if (c != p[j]) {
                            mask &= ~(1 << j);
                        }
                    }
                }
                j = -1;
                for (i = 0; i < 33; i++) {
                    if (mask & 1) {
                        j = i;
                    }
                    mask >>= 1;
                }
                if (j >= 0) {
                    temp = 1 << j;
                    D_800E9D00 |= temp;
                    if (temp == 0x400) {
                        D_800E9D00 &= ~0x800;
                    } else if (temp == 0x800) {
                        D_800E9D00 &= ~0x400;
                    }
                    cheatType = 0x10;
                    sfx = (func_150ADA20() & 1) + 0xC3;
                lbl1:;
                } else {
                    cheatType = 0x11;
                    for (i = 0xA3; i < 0xC8 && sfx == 0; i++) {
                        p = D_800E0C7C;
                        while (p != NULL && (q = ((u8 **)D_800E0BD8)[i]) != NULL) {
                            j = 0;
                            while (q[j] == *p && *p != 0 && q[j] != 0) {
                                p++;
                                j++;
                            }
                            if (q[j] == 0 && (*p == ' ' || *p == 0)) {
                                p = NULL;
                                sfx = (func_150ADA20() & 1) + 0xC1;
                            } else if (*p == ' ') {
                                p++;
                            } else {
                                while (*p && *p != ' ') {
                                    p++;
                                }
                                if (*p == 0) {
                                    p = NULL;
                                }
                            }
                        }
                    }
                    if (sfx == 0) {
                        sfx = (func_150ADA20() & 1) + 0xC5;
                        i = 0;
                        while (D_800E0C7C[i + 0x20] && D_800E0C7C[i] && D_800E0C7C[i + 0x20] == D_800E0C7C[i]) {
                            i++;
                        }
                        if (D_800E0C7C[i + 0x20] == 0 && D_800E0C7C[i] == 0) {
                            D_800E0C84++;
                            if (D_800E0C84 == 1) {
                                sfx = 0xC7;
                            }
                        } else {
                            D_800E0C84 = 0;
                        }
                    } else {
                        D_800E0C84 = 0;
                    lbl2:;
                    }
                }
                for (i = 0; D_800E0C7C[i] != 0; i++) {
                    D_800E0C7C[i + 0x20] = D_800E0C7C[i];
                }
                D_800E0C7C[i + 0x20] = 0;
                i = 0;
                if (sfx != 0) {
                    func_1001263C(sfx, 0x7D00, 0x40);
                }
                obj = func_15083E90(9);
                if (obj != NULL) {
                    obj->unk232 = cheatType;
                    D_800D154C = obj;
                    D_800C3E78 = D_800D154C - D_800CC2D0;
                    obj->unk218 = func_1507BB28(0, obj->unk232);
                    obj->unk21C = 0;
                }
                D_800E0C83 = 0;
                D_800E0C7C[D_800E0C83] = 0;
            } else {
                i -= 2;
            }
        }
        if (i > 0) {
            D_800E0C7C[D_800E0C83] = i;
            if (D_800E0C83 < 19) {
                D_800E0C83++;
            }
            D_800E0C7C[D_800E0C83] = 0;
        }
    }
    func_15043E68(mtx, -30.0f, -10.0f, 0.0f, -115.0f, 10.0f, -530.0f);
    func_150A7D00(mtx + 1, 0.0f, 0.0f, -(f32)D_800E0C82);
    guPerspective(&D_800E0C38, &perspNorm, 50.0f, 60.6f, 53.0f, 20000.0f, 1.0f);
    gSPPerspNormalize(gfx++, perspNorm);
    gSPMatrix(gfx++, &D_800E0C38, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPLoadGeometryMode(gfx++, 0);
    gSPSetGeometryMode(gfx++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    for (i = 0; i < 30; i++) {
        gDPPipeSync(gfx++);
        if (i == key) {
            gDPSetEnvColor(gfx++, 0xFF, 0x00, 0xFF, D_800E0A95);
            gSPMatrix(gfx++, mtx + 1, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        } else {
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A95);
        }
        gSPDisplayList(gfx++, D_80090138[i].dl);
        if (i == key) {
            gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        }
    }
    func_1504332C(0xFF, 0xB1, 0x1E, D_800E0A95);
    func_150428D4(D_800E0C7C, &width, &height, &unused);
    i = 0x4B - (width >> 1);
    if (i < 10) {
        i = 10;
    }
    func_15042D94(i, 0xB4, 0x80, D_800E0C7C);
    if ((D_800BE9AC & 0x1F) >= 10) {
        width = i + width + 2;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(width, 0xB4, 0x80, &D_800ABAD4);
    }
    return gfx;
}

Gfx *func_151EEBE8(Gfx *gfx, s32 arg1) {
    s32 image;

    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A94);
    image = func_1510D0EC(&D_7FB, 0, 3, 0);
    if (image != (s32)0x80000000) {
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_MIRROR, 5, 0, G_TX_CLAMP, 6, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x5FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 12, 0, G_TX_RENDERTILE, 0, G_TX_MIRROR, 5, 0, G_TX_CLAMP, 6, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xBC, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        gfx = func_151E86E4(gfx, 0x190, 0x100, 0x250, 0x200, 0, 0, 0, 0x400, 0x400);
        gfx = func_151E86E4(gfx, 0x250, 0x100, 0x310, 0x200, 0, 0x5E0, 0, -0x400, 0x400);
    }

    gDPPipeSync(gfx++);
    image = func_1510D0EC(&D_887, 0, 3, 0);
    if (image != (s32)0x80000000) {
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gfx = func_151E86E4(gfx, 0x250, 0xBC, 0x290, 0xFC, 0, 0, 0, 0x400, 0x400);
        gfx = func_151E86E4(gfx, 0x180, 0xBC, 0x1D0, 0xFC, 0, 0x200, 0x200, 0x400, 0x400);
    }

    {
        s32 sp48;

        {
            u32 count;

            if (D_800E0C85 != (arg1 + 1)) {
                sp48 = 0;
                count = func_1502B7F0(&sp48, 1, 0x1D);
                if ((u32)arg1 < (u32)(count >> 1)) {
                    D_8009013C = ((u16 *)sp48)[arg1];
                } else {
                    D_8009013C = 0;
                }
                func_10004074((void *)sp48);
                D_800E0C85 = arg1 + 1;
            }
        }
    }

    if (D_8009013C != 0) {
        gDPPipeSync(gfx++);
        gfx = func_151ED430(gfx, &D_8009013C, 0x94, 0x60, 1, 2, 1.0f, 0);
    }
    return gfx;
}

void func_151EEFF0(void) {
    D_800E9D00 = 0;
}

/* 0x40 of inter-TU padding at 0x151EF000..0x151EF040.  Not code: it is the
   tail of this TU's conker.us.yaml range (0x215960..0x21C4F0) and must be
   stated explicitly, or everything downstream slides. */
GLOBAL_ASM(
glabel pad_151EF000
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
)

