#include <ultra64.h>
/* functions.h declares func_1502B7F0 with five parameters; this file calls it with three. */
#define func_1502B7F0 func_1502B7F0_5
#include "functions.h"
#undef func_1502B7F0
/* variables.h declares D_800BE3F8 as a pointer; this file uses it as an array of SaveSlot215960. */
#define D_800BE3F8 D_800BE3F8_ptr_decl_in_variables_h
#include "variables.h"
#undef D_800BE3F8

/* Animation state of a HUD model, created by func_1503F62C; only the fields used here. */
typedef struct {
    /* 0x000 */ u8 pad0[0x3E0];
    /* 0x3E0 */ f32 (*matrices[2])[4]; /* one per frame buffer */
    /* 0x3E8 */ s32 segment3[2];        /* segment 3 base, one per frame buffer */
} HudModelAnim;

/* A 3D model drawn on the HUD: its display lists, patched copies, and a transform per frame buffer. */
typedef struct {
    /* 0x00 */ Gfx **srcDisplayLists;
    /* 0x04 */ Gfx *displayLists[4];
    /* 0x14 */ u8 displayListCount;
    /* 0x15 */ u8 blinkTimer;
    /* 0x16 */ u8 unk16[6];
    /* 0x1C */ s32 unk1C; /* segment 1 base */
    /* 0x20 */ s32 unk20;
    /* 0x24 */ HudModelAnim *anim;
    /* 0x28 */ MtxF matrices[2];
} HudModel;

/* A HUD image drawn by func_151ED430: a texture id (split into 4KB chunks of consecutive ids) and its format. */
typedef struct {
    /* 0x00 */ void *texId;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 width;
    /* 0x08 */ u16 height;
    /* 0x0A */ u8 fmt;
    /* 0x0B */ u8 siz;
} HudSprite; /* size 0xC */

extern s32 D_800E0A74;
extern u8 D_800E0B96;
extern u8 D_800E0B97;
extern Gfx D_80090028;
extern HudSprite D_800917F8;
extern HudSprite D_80091804;
extern HudSprite D_80091810;
extern Gfx *(*D_8008FFF4[])(Gfx *);
extern s32 D_80000300;
extern u8 D_800BE616;
extern u8 D_800BE740;
extern s8 D_8008FD90;
extern s8 D_800E0BD3;
extern u8 **D_800E0BD8;
extern u8 D_7FB;
extern u8 D_887;
extern u8 D_800E0A94;
extern u8 D_800E0C85;
extern HudSprite D_8009013C;
/* func_1510D0EC returns this instead of a texture address when it fails. */
#define TEXTURE_NOT_LOADED ((s32)0x80000000)

extern s32 func_1510D0EC(s32, s32, s32, s32); // matching: s32 params; converting &D_xxx ids to int steers scheduling
extern s32 func_1502B7F0(s32 *, s32, s32);
extern void func_10004074(void *);
Gfx *func_151ED1E0(Gfx *);
void func_15042D94(s32 x, s32 y, u8 flags, void *fmt, ...);
Gfx *func_151ED430(Gfx *gfx, HudSprite *sprite, s32 x, s32 y, s32 columns, s32 rows, f32 scale, s32 frame);
Gfx *func_15096934(Gfx *);
Gfx *func_1501A6CC(Gfx *gfx, s32, s32, s32, s32); /* fills a screen rectangle */

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
extern HudModel *D_80090058;
extern s16 D_800E0C78;
extern u8 D_800BE9C0;
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
        D_80090058 = NULL;
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
Gfx *func_151E966C(Gfx *gfx, s32 y, s32 extraPipTeam, s8 loadTexture, u8 refresh);
Gfx *func_151E9D18(Gfx *gfx, s32 rowY, s32 refresh);

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
    ActorCtl *sub;
    Actor *obj;
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
            team = obj->team;
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
        if ((obj->team == 1) && (obj->unk2E4 != 0) && (D_8008FDC0 & 0x4000)) {
            flags |= 4;
        }
        if (kind == 0x25) {
            func_15086CBC(func_15086D48(0x49, i), &px, &py, &pz);
            px -= obj->x_position;
            pz -= obj->z_position;
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
        if (image != TEXTURE_NOT_LOADED) {
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
                u8 *color = (u8 *)D_800ABA90 + (i << 2); // matching: shift form, not D_800ABA90[i]
                gDPSetEnvColor(gfx++, color[0], color[1], color[2], alpha);
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
            i = (s32)(&D_D16 + 1); // matching: the loop counter doubles as the texture id
        } else {
            i = (s32)&D_D16;
        }
        image = func_1510D0EC(i, 0, 3, 0);
        if (image != TEXTURE_NOT_LOADED) {
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
        if (image != TEXTURE_NOT_LOADED) {
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

extern s8 D_80087268;
extern s16 D_800E0AA0[];
extern s8 D_800E0C00[];


/* Draws each team's row of score pips; teams with no active player get a separate icon. */
Gfx *func_151E966C(Gfx *gfx, s32 y, s32 extraPipTeam, s8 loadTexture, u8 refresh) {
    s8 inactiveTeams;
    s32 image;
    s32 i;
    s32 j;
    s32 filled;
    s32 pipX;
    s32 x;
    s32 spacing;

    inactiveTeams = 0;
    if (loadTexture != 0) {
        image = func_1510D0EC(&D_D14, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
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
    spacing = 0x124;
    if (D_800E0BB0 > 0) {
        spacing = 0x124 / D_800E0BB0;
    }
    if (refresh) {
        inactiveTeams = 0xF;
        for (i = 0; i < D_8008FD8C; i++) {
            if (func_150859AC(i, 3)) {
                inactiveTeams &= ~(1 << D_800E0C00[i]);
            }
        }
    }
    x = spacing >> 1;
    for (i = 0; i < D_800E0BB0; i++) {
        if (refresh) {
            filled = func_150859AC(i, 6);
            D_800E0AA0[i] = filled;
        } else {
            filled = D_800E0AA0[i];
        }
        if (!(inactiveTeams & (1 << i))) {
            if (i == extraPipTeam) {
                filled++;
            }
            gDPPipeSync(gfx++);
            {
                u8 *color = (u8 *)D_800ABA90 + (i << 2); // matching: shift form, not D_800ABA90[i]
                gDPSetEnvColor(gfx++, color[0], color[1], color[2], 0xFF);
            }
            pipX = x - (D_80087268 * 4);
            for (j = 0; j < D_80087268; j++) {
                if (j == filled) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, 0x40, 0x40, 0x40, 0x40);
                }
                gfx = func_151E86E4(gfx, pipX << 2, y, (pipX + 7) << 2, y + 0x1C, 0, 0x1C0, 0x200, 0x400, 0x400);
                pipX += 8;
            }
        }
        x += spacing;
    }
    if (inactiveTeams) {
        image = func_1510D0EC((s32)&D_D16 + 1, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
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
        x = spacing >> 1;
        for (i = 0; i < D_800E0BB0; i++) {
            if (inactiveTeams & (1 << i)) {
                gDPPipeSync(gfx++);
                {
                    u8 *color = D_800ABA90[i];
                    gDPSetEnvColor(gfx++, color[0], color[1], color[2], 0xFF);
                }
                gfx = func_151E86E4(gfx, (x - 8) << 2, y - 0x18, (x + 8) << 2, y + 0x28, 0, 0x600, 0x280, 0x400, 0x400);
            }
            x += spacing;
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

extern ScoreState *D_8008FDD4;
extern s8 D_8008FDC8;
extern s8 D_8008FD7C;
extern s8 D_8008FE44[];
extern s8 D_800E0AC0[];
extern s16 D_800E0AD0[];
extern u8 D_800ABAA8;
extern u8 D_800ABAAC;
extern u8 D_800ABAB0;
extern u8 D_800ABAB8;
extern u8 D_800ABABC;
extern u8 D_800ABAC0;
extern u8 D_800ABAC4;
extern u8 D_843;
extern u8 D_888;


extern u8 D_800ABAA0;
extern u8 D_800ABAA4;

/* Draws the two team icons with each team's total score beside them. */
Gfx *func_151E9D18(Gfx *gfx, s32 rowY, s32 refresh) {
    u8 *tex;
    s32 iconT;
    s32 image;
    s32 width;
    s32 amount;
    s32 team0Total;
    s32 team1Total;
    s32 y;
    s32 leftIconS;
    s32 rightIconT;
    s32 i;

    y = rowY - 0x10;
    if ((D_8008FDC0 & 0x4000) || (D_8008FDD4->unk42 == 8)) {
        tex = (D_8008FDD4->unk42 == 8) ? &D_D10 + 2 : &D_D10 + 1;
        width = 0x10;
        leftIconS = 0;
        rightIconT = 0x200;
    } else {
        tex = &D_D10;
        width = 0x20;
        leftIconS = 0x200;
        rightIconT = 0;
    }

    image = func_1510D0EC(tex, 0, 3, 0);
    if (image == TEXTURE_NOT_LOADED) {
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

    iconT = 0;
    team0Total = 0;
    team1Total = 0;
    if (refresh != 0) {
        if (D_8008FDC0 & 0x6040) {
            team0Total = func_150859AC(0, 6);
            team1Total = func_150859AC(1, 6);
        } else if (D_8008FDC0 & 0x100) {
            for (i = 0; i < D_8008FD8C; i++) {
                amount = func_150859AC(i, 3);
                if (amount < 0) {
                    amount = 0;
                }
                if (D_800CC2D0[i].team == 0) {
                    team0Total += amount;
                } else {
                    team1Total += amount;
                }
            }
        } else {
            iconT = 0x200;
            for (i = 0; i < D_8008FD8C; i++) {
                amount = D_8008FDD4->unk46[i];
                if (amount < 0) {
                    amount = 0;
                }
                if (D_800E0C00[i] == 0) {
                    team0Total += amount;
                } else {
                    team1Total += amount;
                }
            }
        }
        D_800E0AA0[0] = team0Total;
        D_800E0AA0[1] = team1Total;
    } else {
        team0Total = D_800E0AA0[0];
        team1Total = D_800E0AA0[1];
    }

    gfx = func_151E86E4(gfx, 0x108, y, 0x148, y + 0x40, 0, leftIconS, iconT, 0x400, 0x400);
    gfx = func_151E86E4(gfx, 0x318, y, 0x358, y + 0x40, 0, 0, iconT + rightIconT, 0x400, 0x400);
    func_1504332C(0xC0, 0xC0, 0xC0, 0xFF);
    func_15042D94(0x4F, (y >> 2) + 1, 0x80, &D_800ABAA0, team1Total);
    func_15042D94(0xD3, (y >> 2) + 1, 0x80, &D_800ABAA4, team0Total);
    return gfx;
}

/* Draws the per-player results table (name, team score, percentage and stat columns). */
Gfx *func_151EA15C(Gfx *gfx, s32 topY, s32 alpha, s32 live) {
    s32 image;
    s32 y;
    s32 i;
    s32 team;
    s32 icon;
    s32 xbase;
    s32 x;
    s32 iconY;
    s32 nameBase;
    s32 iconBottom;
    s32 percent;
    s32 total;
    s32 nameIndex;
    s8 showTeam0;
    s8 texturesLoaded;
    s8 refreshScores;

    if ((D_8008FDD4->unk42 == 0xB) || (D_8008FDD4->unk42 == 0xC)) {
        if (live != 0) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
        image = func_1510D0EC(&D_843, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
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
                icon = D_8008FE44[i] & 3;
                gfx = func_151E86E4(gfx, 0x120, (y - 2) << 2, 0x160, (y + 0xE) << 2, 0,
                                    (icon & 1) << 9, (icon >> 1) << 9, 0x400, 0x400);
                y += 0x12;
            }
        }
        return gfx;
    }

    y = topY;
    if (D_8008FDC0 & 1) {
        xbase = 0x1A;
    } else {
        xbase = 0x34;
    }
    if (D_800E0B94 != 0) {
        if (D_8008FDC8 != 0) {
            y = topY - 0x28;
        }
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    if (D_800E0BD3 == 2) {
        nameBase = 9;
        refreshScores = 1;
    } else {
        nameBase = 0x5E;
        refreshScores = 0;
    }
    if (D_8008FDD4->unk42 == 8) {
        showTeam0 = 0;
    } else {
        showTeam0 = 1;
    }
    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
    gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
    texturesLoaded = 0;
    for (i = 0; i < 4; i++) {
        if (D_8008FE44[i] < 0) {
            continue;
        }
        if (D_80087270[i] == 0xA) {
            continue;
        }
        team = D_800E0C00[i];
        if (team != 0 || (team == 0 && showTeam0)) { // matching: redundant team == 0 test is in the original
            if (texturesLoaded == 0) {
                image = func_1510D0EC(&D_887, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
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
                    iconY = (y - 0x11) << 2;
                    gfx = func_151E86E4(gfx, x + 0x1D4, iconY, x + 0x214, iconY + 0x40, 0, 0x200, 0, 0x400, 0x400);
                    x += 0x78;
                }
                iconY = (y - 0x11) << 2;
                x += 0x1D4;
                if (D_8008FDD4->unk42 == 1) {
                    x -= 0x78;
                }
                gfx = func_151E86E4(gfx, x + 0x164, iconY, x + 0x1A4, iconY + 0x3C, 0, 0, 0x200, 0x400, 0x400);
                if (D_8008FDD4->unk42 == 1) {
                    x += 0x78;
                }
                gDPPipeSync(gfx++);
                image = func_1510D0EC(&D_888, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                iconBottom = iconY + 0x40;
                gfx = func_151E86E4(gfx, x, iconY, x + 0x40, iconBottom, 0, 0x200, 0, 0x400, 0x400);
                x += 0x74;
                gfx = func_151E86E4(gfx, x, iconY, x + 0x40, iconBottom, 0, 0, 0, 0x400, 0x400);
                if (D_8008FDD4->unk42 != 1) {
                    x += 0x4C;
                    gfx = func_151E86E4(gfx, x, iconY, x + 0x80, iconBottom, 0, 0, 0x200, 0x400, 0x400);
                }
                gDPPipeSync(gfx++);
                image = func_1510D0EC(&D_843, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
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
                texturesLoaded = 1;
            }
            func_1504332C(D_800ABA90[team][0], D_800ABA90[team][1], D_800ABA90[team][2], alpha);
            percent = 0;
            icon = D_8008FE44[i] & 3;
            gfx = func_151E86E4(gfx, (xbase + 8) << 2, (y - 2) << 2, (xbase + 0x18) << 2, (y + 0xE) << 2, 0,
                                (icon & 1) << 9, (icon >> 1) << 9, 0x400, 0x400);
            if (live != 0) {
                total = D_800CC2D0[i].unk31C->unk1AA;
            } else {
                total = D_8008FDD4->rec[i].unk6;
            }
            if (total > 0) {
                percent = (D_8008FDD4->rec[i].unk0 * 100) / total;
            }
            if (percent >= 10000) {
                percent = 9999;
            }
            if (D_8008FDC8 != 0) {
                team = i;
            }
            x = xbase;
            nameIndex = (s8)(D_800E0AC0[team] - 1);
            if (nameIndex < 0) {
                nameIndex = 0;
            }
            func_15042D94(x + 0x3C, y, 0x81, &D_800ABAA8, D_800E0BD8[nameIndex + nameBase]);
            if (D_8008FDC0 & 1) {
                func_15042D94(xbase + 0x54, y, 0x81, &D_800ABAAC, D_800E0AA0[team]);
                x = xbase + 0x1E;
            }
            x += 0x58;
            func_15042D94(x, y, 0x81, &D_800ABAB0, percent);
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
        gfx = func_151E9D18(gfx, 0x320, refreshScores);
    }
    if (live != 0) {
        if (D_8008FDC0 & 1) {
            gfx = func_151E966C(gfx, 0x320, -1, 1, refreshScores);
        }
    }
    return gfx;
}

typedef struct {
    /* 0x00 */ s32 v[7];
} PlaceValueTable;

extern void *D_80090074[];
extern PlaceValueTable D_8009009C;

/* Draws a number of up to seven digits, one 32x32 digit texture per place, without leading zeros. */
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
            if (image != TEXTURE_NOT_LOADED) {
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
extern HudSprite D_80090060;
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
extern HudModel *D_8009005C;
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
void func_151EDB58(HudModel *model);
HudModel *func_151ED90C(s32, s32, s32, f32);
Gfx *func_151EDBDC(Gfx *, HudModel *, f32, f32, f32);
Gfx *func_151EA15C(Gfx *, s32, s32, s32);
Gfx *func_151EADFC(Gfx *, s32, s32, s32);

/* Draws the single-player HUD (counters and models) after preloading its textures; fades it in. */
Gfx *func_151EB06C(Gfx *gfx) {
    s32 show;
    s32 value;
    s32 x;
    s32 hasValue;
    s32 i;
    s32 start;
    extern s32 func_1510D0EC(); // matching: unprototyped, so the texture-id calls below pass pointers as-is
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
        if (D_80090058 != NULL) {
            func_151EDB58(D_80090058);
            func_151EDB58(D_8009005C);
            D_80090058 = NULL;
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
    D_80090060.texId = &D_891;
    {
        s32 remaining;
        s32 pad; // matching: stack slot
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
            f32 halfWidth;
            value = func_150859AC(0, 6);
            if (value == 0x7D00) {
                value = 1000000;
            }
            if (func_1509BE40(0, 0x5082, 0x1A) && !func_1509BE40(0, 0x5083, 0x1A)) {
                Struct1509B570 *entry = func_1509B570(0x83);
                if (entry != NULL) {
                    if (entry->unk64 == 0) {
                        value = 0;
                    } else if (entry->unk64 < 3) {
                        value = (s32)(value * (u32)entry->unk64) / 3;
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
            if (D_80090058 == NULL) {
                D_80090058 = func_151ED90C(0xA4, 0x17, 0, 1.0f);
                D_8009005C = func_151ED90C(0xA2, 8, 0, 1.0f);
            }
            gSPSetGeometryMode(gfx++, 0x220404);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
            gfx = func_150900F0(gfx, 1);
            halfWidth = D_800BE628[0].width * 0.5f;
            func_151EF954((f32 (*)[4])&D_800E0C38, -halfWidth, halfWidth, -(D_800BE628[0].height * 0.5f), D_800BE628[0].height * 0.5f, 1.0f, 20000.0f, 1.0f);
            D_800E0C30 = D_8008FDCC;
            if (D_80090058 != NULL) {
                i = 10;
                if (hasValue) {
                    x = -0x10;
                    while (i < value) {
                        x -= 0x10;
                        i *= 10;
                    }
                    gfx = func_151EDBDC(gfx, D_80090058, x + -10.0f, -83.0f, 0.2699999809f);
                }
            }
            if (D_8009005C != NULL && show) {
                gfx = func_151EDBDC(gfx, D_8009005C, 70.0f, 80.0f, 0.3000000119f);
            }
            gSPDisplayList(gfx++, &D_80090028);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
            if (show) {
                s32 count;
                count = func_150859AC(0, 3);
                if (count != 0) {
                    count--;
                }
                if (count >= 100) {
                    count = 99;
                }
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
                gfx = func_151EADFC(gfx, 0xEC, 0x1E, count);
            }
            if (hasValue) {
                s32 pad4; // matching: stack slot
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
                D_80090060.texId = &D_7F6;
                gfx = func_151EADFC(func_151ED430(gfx, &D_80090060, x + 0xA5, 0xAA, 1, 1, 1.0f, 0), x + 0xAD, 0x9B, value);
            }
        } else {
            gfx = func_151EA15C(func_151ED430(gfx, &D_80090060, 0x94, 0x12, 3, 1, 1.0f, 0), 0x85, D_8008FDCC, 1);
        }
    }
    return gfx;
}


/* Draws the results table while it is faded in. */
Gfx *func_151EB930(Gfx *gfx) {
    if (D_8008FDCC != 0) {
        gfx = func_151EA15C(gfx, 0x6A, D_8008FDCC, 0);
    }
    return gfx;
}

extern s16 D_800E0A80;
extern s32 D_800E0A90;

/* Draws the current message, a run of text lines ended by a "*" line, fading in and out. */
Gfx *func_151EB96C(Gfx *gfx) {
    s32 pad; // matching: stack slot
    s16 alpha;
    s16 i;
    s32 count;
    u8 alpha8;
    s32 spacing;
    s32 x;
    s32 firstX;
    u8 *line;
    if (D_800E0A80 < 0) {
        return gfx;
    }
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
    alpha8 = alpha;
    while (D_800E0BD8[i][0] != '*') {
        i++;
        count++;
    }
    if (count >= 0xE) {
        spacing = 0xB;
    } else {
        spacing = 0x11;
    }
    func_1504332C(0xFF, 0, 0, alpha8);
    x = (0xD0 - count * spacing) >> 1;
    i = D_800E0A80;
    line = D_800E0BD8[i];
    firstX = x;
    while (*line != '*') {
        func_15042D94(0x94, x, 1, line);
        func_1504332C(0xFF, 0xFF, 0xFF, alpha8);
        if (x == firstX) {
            x += 0xC;
        }
        i++;
        line = D_800E0BD8[i];
        x += spacing;
    }
    return gfx;
}

extern HudSprite D_800917EC;
extern u8 D_85D;
extern u8 D_89C;
extern s32 D_800BE620;
extern s32 D_800BE624;

/* Draws a timed full-screen sequence over black: a growing title, a page of text lines, then an image. */
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
        func_15042D94(0x94, 0x69, 1, D_800E0BD8[0x8E]);
        func_150432BC(1.0f);
    } else if (D_800E0A90 < 0x21C) {
        if (D_800E0A90 < 0x12C) {
            alpha = (D_800E0A90 - 0xF0) << 3;
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
        func_15042D94(0x94, 0x9D, 1, D_800E0BD8[0x81]);
        func_15042D94(0x94, 0xAA, 1, D_800E0BD8[0x82]);
        func_15042D94(0x94, 0xBC, 1, D_800E0BD8[0x83]);
        func_150432BC(0.8999999762f);
        func_15042D94(0x94, 0x2D, 1, D_800E0BD8[0x84]);
        func_15042D94(0x94, 0x37, 1, D_800E0BD8[0x85]);
        func_15042D94(0x94, 0x41, 1, D_800E0BD8[0x86]);
        func_15042D94(0x94, 0x4B, 1, D_800E0BD8[0x87]);
        func_15042D94(0x94, 0x55, 1, D_800E0BD8[0x88]);
        func_15042D94(0x94, 0x5F, 1, D_800E0BD8[0x89]);
        func_15042D94(0x94, 0x69, 1, D_800E0BD8[0x8A]);
        func_15042D94(0x94, 0x73, 1, D_800E0BD8[0x8B]);
        func_15042D94(0x94, 0x7D, 1, D_800E0BD8[0x8C]);
        func_15042D94(0x94, 0x87, 1, D_800E0BD8[0x8D]);
        func_150432BC(1.0f);
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        D_80090060.texId = &D_89C;
        D_80090060.width = 0x10;
        D_80090060.height = 0x10;
        gfx = func_151ED430(gfx, &D_80090060, 0xC7, 0xB0, 1, 1, 1.0f, 0);
        D_80090060.width = 0x20;
        D_80090060.height = 0x20;
    } else {
        if (D_800E0A90 < 0x258) {
            alpha = (D_800E0A90 - 0x21C) << 3;
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
        D_80090060.texId = &D_85D;
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


/* Fades in a text line once the timer passes 0x5DC. */
Gfx *func_151EC178(Gfx *gfx) {
    s32 alpha;
    if (D_800E0A90 >= 0x5DD) {
        alpha = (D_800E0A90 - 0x5DC) << 3;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(0xDC, 0x130, 1, D_800E0BD8[0x74]);
    }
    return gfx;
}

/* Draws a fading image and two overlaid sprites (the second tinted by D_800E0B97); updates the fade level D_800E0B96. */
Gfx *func_151EC1F0(Gfx *gfx) {
    s32 alpha;
    s32 temp;

    if (D_800BE9F0 == 0x21) {
        return gfx;
    }

    gSPDisplayList(gfx++, &D_80090028);
    temp = D_800E0A90; // matching: temp is reused for 0x1AC below
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

extern SaveSlot215960 D_800BE3F8[];

extern u8 D_800E0A95;
extern s8 D_8008FEF8;
extern void *D_800900BC[];
Gfx *func_151EC648(Gfx *);
Gfx *func_151EE184(Gfx *);
Gfx *func_151EEBE8(Gfx *gfx, s32 index);

/* Draws the fading image picked by D_8008FDD4->unk2C (or the func_151EC648 / func_151EE184 screens). */
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
            D_80090060.texId = D_800900BC[D_8008FDD4->unk2C];
            gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x1E, 3, 1, 1.0f, 0);
        }
        if (D_8008FDD4->unk8 >= 0.5f) {
            if (D_8008FDD4->unk2C >= 3) {
                if (D_800BE3F8[D_8008FDD4->unk2C - 3].unk8 != -1) {
                    gfx = func_151EEBE8(gfx, D_800BE3F8[D_8008FDD4->unk2C - 3].unkC);
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
extern Actor *D_800E0BA0[];
extern u8 D_800E0C88[];
extern u8 D_87B;
extern u8 D_88C;
s32 func_151E24F0(s32 *);
void func_151E7F60(s32, s32);

/* Draws the multi-page screen selected by D_8008FDD4: page images, a rotating list, or per-slot panels. */
Gfx *func_151EC648(Gfx *gfx) {
    static s8 D_800900F8 = 0;
    s32 fadeAlpha;
    s32 alpha;
    s32 pad; // matching: stack slot
    s32 y;
    ScoreState *state;
    f32 scale;
    f32 progress;
    u8 **image;
    s8 slotCount;
    s8 page;
    s32 i;
    s32 j;
    s32 prev;
    s32 cur;
    s32 mid;
    s32 v;
    s32 listAlpha;
    f32 angle;
    s32 diff;
    f32 sc;

    fadeAlpha = (D_8008FDD4->unk0 - 0.5f) * 512.0f;
    if (fadeAlpha < 0) {
        fadeAlpha = -fadeAlpha;
    }
    if (fadeAlpha >= 0x100) {
        fadeAlpha = 0xFF;
    }
    gDPPipeSync(gfx++);
    state = D_8008FDD4;
    progress = state->unk8;
    if (0.0f == state->unk8 && 0.0f == state->unkC) {
        page = state->unk2C;
        D_800900F8 = page;
    } else {
        page = state->unk2C;
        if (page == 0 && state->unkC < 0.0f) {
            D_800900F8 = 4;
        }
    }
    if (page == 0) {
        y = 0x6A;
        alpha = 0xFF - (s32)(progress * 300.0f);
    } else {
        alpha = fadeAlpha;
        if (D_800900F8 == 4) {
            sc = 68.0f;
        } else {
            sc = 76.0f;
        }
        y = 0x6A - (s32)(progress * sc);
    }
    if (state->unk40 != 3) {
        if (alpha > 0 && *(image = &D_800900D8[D_800900F8]) != NULL) {
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
            D_80090060.texId = *image;
            gfx = func_151ED430(gfx, &D_80090060, 0x94, y, diff, v, scale, 0);
            state = D_8008FDD4;
            progress = state->unk8;
        }
        alpha = 0xFF - (s32)(progress * 300.0f);
        progress = 2.0f;
        if (alpha > 0) {
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            D_80090060.texId = &D_88C;
            gfx = func_151ED430(gfx, &D_80090060, 0x94, 0x1E, 3, 1, 1.0f, 0);
            state = D_8008FDD4;
        }
    }
    switch (state->unk40) {
    case 0:
        D_800E0C88[0] = 0;
        D_800E0C88[1] = 0xFF;
        D_800E0C88[2] = 0xFF;
        D_800E0C88[3] = 0xFF;
        if (state->unk8 >= 0.5f && state->unk2C == 4) {
            listAlpha = (D_800E0A94 * D_800E0A95) >> 8;
            if (listAlpha >= 0xFE) {
                listAlpha = 0xFF;
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
                    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, listAlpha);
                    mid = (diff >> 1) + prev;
                    sc = diff / 32.0f;
                    D_80090060.texId = D_800900FC[j];
                    gfx = func_151ED430(gfx, &D_80090060, 0x53, mid, 2, 1, sc, -1);
                    if (D_8008FDE8 == 0.0f && i == 5 && D_8008FE2C == 0) {
                        v = (D_800BE9AC * 2) & 0x7F;
                        if (v >= 0x40) {
                            v = 0x7F - v;
                        }
                        v = (s32)((u32)listAlpha * (v + v + v + 0x3F)) >> 8;
                        if (v >= 0x100) {
                            v = 0xFF;
                        }
                        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, v);
                        D_80090060.texId = D_800900FC[j] + 2;
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
        s32 slot;
        s32 valueIndex;
        s32 values[3];
        Actor **slotObj;

        sc = (D_800BE9AC & 0x3F) * 0.0981746912f;
        sc = sinf(sc + 0.7853975296f);
        pulse = (D_800E0A95 * (s32)((sc + 1.0f) * 127.0f)) >> 8;
        if (D_8008FDD4->unk2C == 7) {
            x = 0x70;
            slotCount = 2;
        } else {
            x = 0x28;
            slotCount = 4;
        }
        count = func_151E24F0(values);
        if (count != 0) {
            valueIndex = 0;
        }
        for (slot = 0; slot < slotCount; slot++) {
            mid = -1;
            dir = 1;
            for (v = 0; v < D_8008FE40; v++) {
                if (slot == D_8008FE44[v]) {
                    mid = v;
                    v = D_8008FE40;
                    dir = -1;
                }
            }
            if (slot != 0 && D_800E0BA0[0] != 0) {
                slotObj = &D_800E0BA0[slot];
                if (D_800E0C88[slot] != 0xFF) {
                    if (*slotObj == 0 && mid != -1) {
                        func_151E7F60(slot, D_800E0B90[mid]);
                    }
                } else if (*slotObj != 0) {
                    func_15060F28(*slotObj, 1);
                    *slotObj = 0;
                }
            }
            alpha = D_800E0C88[slot];
            alpha += dir * (D_800BE9E4 << 3);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            } else if (alpha < 0) {
                alpha = 0;
            }
            D_800E0C88[slot] = alpha;
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A95);
            D_80090060.texId = &D_89C + 2;
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
                D_80090060.texId = &D_87B;
                gfx = func_151ED430(gfx, &D_80090060, x, 0x82, 1, 1, 1.0f, 0);
            }
            func_1504332C(0xFF, 0xFF, 0xFF, D_800E0A95);
            func_15042D94(x, 0x41, 0x80, "#", slot + 1);
            if (count != 0 && mid >= 0 && D_8008FE44[mid] >= 0) {
                count--;
                func_15042D94(x, 0xA8, 0, "#", 0x38);
                func_15042D94(x, 0xA2, 0x81, "%d", values[valueIndex] + 1);
                valueIndex++;
            }
            x += 0x48;
        }
    } // matching: the break must follow the block, not end it
        break;
    case 3:
        gfx = func_151EA15C(gfx, 0x6A, 0xFF, 0);
        break;
    }
    return gfx;
}

extern HudSprite D_8009181C;
extern HudSprite D_80091828;

/* Draws a fading-in image with a tinted overlay (strength D_800E0B97). */
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

/* Darkens the whole screen with a black rectangle of alpha D_800E0B96. */
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


extern u8 D_8009DEB0[];
extern u8 D_8009DEB4[];
extern u8 D_8009DEB8[];
extern u8 D_8009DEBC[];

/* Sets up the load and render tiles for a sprite; stores its LoadBlock texel count in *blockSizeOut. */
Gfx *func_151ED29C(Gfx *gfx, HudSprite *sprite, s32 *blockSizeOut) {
    s32 masks;
    s32 maskt;
    s32 i;
    s32 fmt;
    s32 blockSize;

    blockSize = D_8009DEB0[sprite->siz] + sprite->width * sprite->height;
    blockSize >>= D_8009DEB4[sprite->siz];
    *blockSizeOut = blockSize;
    masks = 1;
    for (i = 2; i < sprite->width; i <<= 1) {
        masks++;
    }
    maskt = 1;
    for (i = 2; i < sprite->height; i <<= 1) {
        maskt++;
    }
    fmt = sprite->fmt;
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, fmt, D_8009DEBC[sprite->siz], 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTile(gfx++, fmt, sprite->siz, (D_8009DEB8[sprite->siz] * sprite->width + 7) >> 3, 0, 0, 0, G_TX_CLAMP, maskt, 0, G_TX_CLAMP, masks, 0);
    gDPSetTileSize(gfx++, 0, 0, 0, (sprite->width - 1) << 2, (sprite->height - 1) << 2);
    return gfx;
}

/*
 * Draws a sprite as a columns x rows grid of tiles centred on (x, y), streaming its texture in 4KB
 * loads. A negative frame keeps the horizontal scale at 1.
 */
Gfx *func_151ED430(Gfx *gfx, HudSprite *sprite, s32 x, s32 y, s32 columns, s32 rows, f32 scale, s32 frame) {
    s32 column;
    s32 row;
    s32 width;
    s32 height;
    s32 top;
    s32 blockSize;
    s32 texId;
    s32 dsdx;
    s32 dtdy;
    s32 image;
    s32 midTexture;
    s32 remaining;
    f32 scaleX;

    midTexture = 0;
    gfx = func_151ED29C(gfx, sprite, &blockSize);
    if (frame < 0) {
        scaleX = 1.0f;
        frame = 0;
    } else {
        scaleX = scale;
    }
    texId = (s32)sprite->texId + frame;
    width = (u32)sprite->width * scaleX * D_8008FE1C;
    height = (u32)sprite->height * scale * D_8008FE20;
    dtdy = 1024.0f / (scale * D_8008FE20);
    dsdx = 1024.0f / (scaleX * D_8008FE1C);
    x *= D_8008FE1C;
    y *= D_8008FE20;
    x -= (width * columns) >> 1;
    top = y - ((height * rows) >> 1);
    for (column = 0; column < columns; column++) {
        y = top;
        for (row = 0; row < rows; row++) {
            if (!midTexture) {
                image = func_1510D0EC(texId, (s32)&remaining, 3, 0);
            }
            if (image != TEXTURE_NOT_LOADED) {
                gDPPipeSync(gfx++);
                gDPSetTextureImage(gfx++, sprite->fmt, D_8009DEBC[sprite->siz], 1, image);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, blockSize - 1, 0);
                gSPScisTextureRectangle(gfx++, x << 2, y << 2, (x + width) << 2, (y + height) << 2, 0, 0, 0, dsdx, dtdy);
            }
            y += height;
            if (remaining > 0x1000) {
                midTexture = 1;
                if (image != 0) {
                    image += (blockSize << sprite->siz) >> 1;
                }
                remaining -= (blockSize << sprite->siz) >> 1;
            } else {
                midTexture = 0;
                texId++;
            }
        }
        x += width;
    }
    gDPPipeSync(gfx++);
    return gfx;
}

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1503F62C(s32, s32, void *, void *, void *, void *, void *);
extern void func_1503F5B8(HudModelAnim *, s32, s32, f32, f32, s32);

/*
 * Loads a HUD model and makes patched copies of its display lists (SetOtherMode rewritten,
 * SetCombine turned into a no-op). Returns NULL on failure.
 */
HudModel *func_151ED90C(s32 loadArg0, s32 loadArg1, s32 animArg0, f32 animArg1) {
    HudModel *model;
    f32 (*mtx1)[4];
    f32 (*mtx2)[4];
    Gfx *dst;
    Gfx *src;
    s32 cmd;
    s32 size;
    s32 i;

    model = allocate_memory(sizeof(HudModel), 1, 0, 1);
    if (model == NULL) {
        return NULL;
    }
    model->blinkTimer = 1;
    if (func_1503F62C(loadArg0, loadArg1, model, &model->displayListCount, &model->unk1C, &model->unk20, &model->anim) != 0) {
        func_10004074(model);
        return NULL;
    }

    mtx1 = model->matrices[0];
    guMtxIdentF(mtx1);
    mtx2 = model->matrices[1];
    guMtxIdentF(mtx2);
    model->anim->matrices[0] = mtx1;
    model->anim->matrices[1] = mtx2;
    func_1503F5B8(model->anim, 1, animArg0, animArg1, 0.0f, 0);

    for (i = 0; i < model->displayListCount; i++) {
        src = model->srcDisplayLists[i];
        size = 0;
        do {
            cmd = (src->words.w0 >> 24) & 0xFF;
            src++;
            size += 8;
        } while (cmd != 0xDF);

        dst = allocate_memory(size, 1, 1, 1);
        model->displayLists[i] = dst;
        if (dst == NULL) {
            for (size = 0; size < i; size++) {
                func_10004074(model->displayLists[size]);
            }
            func_10004074(model);
            return NULL;
        }
    }

    for (i = 0; i < model->displayListCount; i++) {
        dst = model->displayLists[i];
        src = model->srcDisplayLists[i];
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

    return model;
}


extern void func_1503F7B8(HudModelAnim *);

/* Frees a HUD model and its display list copies. */
void func_151EDB58(HudModel *model) {
    s32 i;

    if (model != NULL) {
        func_1503F7B8(model->anim);
        func_100043B4((s32 *)model, 4);
        for (i = 0; i < model->displayListCount; i++) {
            func_100043B4((s32 *)model->displayLists[i], 4);
        }
    }
}

extern Mtx D_80089470;
extern Gfx D_80090110;
extern Gfx D_80090128;
extern u8 D_507;
extern u8 D_508;
extern void func_1503F4B0(HudModelAnim *);

/* Draws a HUD model at (x, y) with the given scale; the D_80090058 model also blinks. */
Gfx *func_151EDBDC(Gfx *gfx, HudModel *model, f32 x, f32 y, f32 scale) {
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

    guMtxIdentF(model->matrices[D_800BE9C0]);
    model->matrices[D_800BE9C0][3][0] = x;
    model->matrices[D_800BE9C0][3][1] = y;
    model->matrices[D_800BE9C0][3][2] = -100.0f;
    model->matrices[D_800BE9C0][0][0] = scale;
    model->matrices[D_800BE9C0][1][1] = scale;
    model->matrices[D_800BE9C0][2][2] = scale;
    func_1503F4B0(model->anim);

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_800E0C38, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, &D_80090110);
    gDPSetCombine(gfx++, 0x12FE25, 0xFFFFFBFD);
    gSPLoadGeometryMode(gfx++, 0);
    gSPSetGeometryMode(gfx++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gDPSetPrimColor(gfx++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0C30);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, model->anim->segment3[D_800BE9C0]);
    gSPSegment(gfx++, 1, model->unk1C);

    if (model == D_80090058) {
        model->blinkTimer--;
        if (model->blinkTimer == 0) {
            model->blinkTimer = (func_150ADA20() & 0x7F) + 0xF;
        }
        if (model->blinkTimer < 7) {
            tex = &D_507;
        } else {
            tex = &D_508;
        }
        image = func_1510D0EC(tex, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
            return start;
        }
        gSPSegment(gfx++, 6, image);
        gSPSegment(gfx++, 7, image);
    }

    for (i = 0; i < model->displayListCount; i++) {
        gSPDisplayList(gfx++, model->displayLists[i]);
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

/*
 * Loads the 30 display lists 0x1C5..0x1E2 and patches them: vertex addresses relocated with the
 * first two vertices' colours set to opaque black, SetCombine and SetOtherMode rewritten.
 */
void func_151EDF4C(void) {
    s32 i;
    s32 j;
    s32 pad1; // matching: stack slots
    s32 pad2;
    s32 *data;
    Gfx *dl;
    s32 cmd;
    Vtx *vtx;

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
                    vtx = (Vtx *)(((volatile Gfx *)dl)->words.w1 | 0x80000000); // matching: volatile forces the reload of w1 after the store
                    vtx->v.cn[0] = 0;
                    vtx->v.cn[1] = 0;
                    vtx->v.cn[2] = 0;
                    vtx->v.cn[3] = 0xFF;
                    vtx++;
                    vtx->v.cn[0] = 0;
                    vtx->v.cn[1] = 0;
                    vtx->v.cn[2] = 0;
                    vtx->v.cn[3] = 0xFF;
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

/*
 * Text entry on a 3D letter grid (cursor D_800E0C81 row, D_800E0C80 column, text in D_800E0C7C):
 * moves the cursor, types letters, and on enter matches the text against the code strings
 * 0x90..0xA2 (sets a D_800E9D00 flag) and the word strings 0xA3..0xC7, then plays a response.
 */
Gfx *func_151EE184(Gfx *gfx) {
    s32 sfx;
    s32 cursorMoved;
    s32 rowLength;
    s32 i;
    s32 selectedKey;
    Actor *obj;
    Mtx *mtx;
    s32 j;
    u32 matchMask;
    u16 perspNorm;
    s32 temp;
    s32 cheatType;
    u8 typed;
    s32 width;
    s32 height;
    s32 unused; /* third output of func_150428D4 */
    u8 *str;
    u8 *word;

    sfx = 0;
    if (D_80090138 == NULL) {
        return gfx;
    }
    cursorMoved = 0;
    if (D_8008FDD4->unk20 & 1) {
        cursorMoved = 1;
        D_800E0C81--;
    }
    if (D_8008FDD4->unk20 & 2) {
        D_800E0C81++;
        if (D_800E0C81 == 4 && D_800E0C80 >= 5) {
            D_800E0C80 = 4;
        }
        cursorMoved = 1;
    }
    if (D_800E0C81 < 0) {
        D_800E0C81 = 5;
    }
    if (D_800E0C81 >= 6) {
        D_800E0C81 = 0;
    }
    if (D_8008FDD4->unk20 & 8) {
        D_800E0C80++;
        cursorMoved = 1;
    }
    if (D_8008FDD4->unk20 & 4) {
        D_800E0C80--;
        cursorMoved = 1;
    }
    if (D_800E0C81 == 4) {
        rowLength = 5;
    } else if (D_800E0C81 == 5) {
        rowLength = 1;
    } else {
        rowLength = 6;
    }
    if (D_800E0C80 < 0) {
        D_800E0C80 = rowLength - 1;
    }
    if (D_800E0C80 >= rowLength) {
        D_800E0C80 = 0;
    }
    if (cursorMoved) {
        func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
    }
    selectedKey = D_800E0C81 * 6 + D_800E0C80;
    if (D_800E0C81 == 5) {
        selectedKey--;
    }
    mtx = allocate_memory(0x80, 4, 2, 0);
    if (D_800BE748[0].button & A_BUTTON) {
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
        i = selectedKey + 'A';

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
                matchMask = 0xFFC3FFFE;
                for (i = 0x90; i < 0xA3; i++) {
                    str = D_800E0BD8[i];
                    temp = i - 0x90;
                    if (temp < D_800E0C83) {
                        typed = D_800E0C7C[temp];
                    } else {
                        typed = ' ';
                    }
                    for (j = 0; j < 32; j++) {
                        if (typed != str[j]) {
                            matchMask &= ~(1 << j);
                        }
                    }
                }
                j = -1;
                for (i = 0; i < 33; i++) {
                    if (matchMask & 1) {
                        j = i;
                    }
                    matchMask >>= 1;
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
                lbl1:; // matching: as lbl2 below
                } else {
                    cheatType = 0x11;
                    for (i = 0xA3; i < 0xC8 && sfx == 0; i++) {
                        str = D_800E0C7C;
                        while (str != NULL && (word = D_800E0BD8[i]) != NULL) {
                            j = 0;
                            while (word[j] == *str && *str != 0 && word[j] != 0) {
                                str++;
                                j++;
                            }
                            if (word[j] == 0 && (*str == ' ' || *str == 0)) {
                                str = NULL;
                                sfx = (func_150ADA20() & 1) + 0xC1;
                            } else if (*str == ' ') {
                                str++;
                            } else {
                                while (*str && *str != ' ') {
                                    str++;
                                }
                                if (*str == 0) {
                                    str = NULL;
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
                    lbl2:; // matching: an empty label here ends a basic block for IDO's scheduler
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
        if (i == selectedKey) {
            gDPSetEnvColor(gfx++, 0xFF, 0x00, 0xFF, D_800E0A95);
            gSPMatrix(gfx++, mtx + 1, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        } else {
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A95);
        }
        gSPDisplayList(gfx++, D_80090138[i].dl);
        if (i == selectedKey) {
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

/* Draws a framed panel showing image `index` of table 0x1D (the texture id is cached in D_8009013C). */
Gfx *func_151EEBE8(Gfx *gfx, s32 index) {
    s32 image;

    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_800E0A94);
    image = func_1510D0EC(&D_7FB, 0, 3, 0);
    if (image != TEXTURE_NOT_LOADED) {
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
    if (image != TEXTURE_NOT_LOADED) {
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

    if (D_800E0C85 != index + 1) {
        u16 *table;
        u32 size;

        table = NULL;
        size = func_1502B7F0((s32 *)&table, 1, 0x1D);
        if ((u32)index < size >> 1) {
            D_8009013C.texId = (void *)table[index];
        } else {
            D_8009013C.texId = NULL;
        }
        func_10004074(table);
        D_800E0C85 = index + 1;
    }

    if (D_8009013C.texId != NULL) {
        gDPPipeSync(gfx++);
        gfx = func_151ED430(gfx, &D_8009013C, 0x94, 0x60, 1, 2, 1.0f, 0);
    }
    return gfx;
}

/* Clears the code flags set by func_151EE184. */
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

