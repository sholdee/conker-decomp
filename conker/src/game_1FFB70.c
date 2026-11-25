#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern u8 D_A48[];

/*
 * Object type 0x3C (list 1): a full-screen overlay that fades in, scrolls its texture horizontally and fades
 * out again. Several overlays can coexist; each is drawn only for the matching id.
 */
typedef struct ScreenOverlay {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ struct ScreenOverlay *prev;
    /* 0x08 */ struct ScreenOverlay *next;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ s16 timer;  /* advances by speed each frame; the overlay dies outside [0, 300] */
    /* 0x10 */ s16 id;
    /* 0x12 */ s16 scroll; /* horizontal texture offset in 1/4 texels, wraps at 256 */
    /* 0x14 */ s16 alpha;  /* 0..128 */
    /* 0x16 */ s8 speed;   /* 1 while running, -2 once stopped (fades back out) */
} ScreenOverlay;

/* Object lists (see game_1944C0.c): D_800DCE50[list][type] heads a linked list of objects of that type. */
extern ScreenOverlay *D_800DCE50[2][104];

/* Spawns an overlay for the given id. */
ScreenOverlay *func_151D26C0(s16 id) {
    ScreenOverlay *overlay;

    overlay = func_15167A68(0x3C, 1, sizeof(ScreenOverlay), 0, 0xFF, 1);
    overlay->alpha = 0;
    overlay->scroll = 0;
    overlay->timer = 0;
    overlay->speed = 1;
    overlay->id = id;
    return overlay;
}

/* Makes every overlay with the given id run backwards, fading it out. */
void func_151D2718(s16 id) {
    ScreenOverlay *overlay;

    for (overlay = D_800DCE50[1][0x3C]; overlay != NULL; overlay = overlay->next) {
        if (id == overlay->id) {
            overlay->speed = -2;
        }
    }
}

/* Per-frame update: advances the timer, derives the fade alpha and scrolls the texture. */
void func_151D275C(ScreenOverlay *overlay) {
    s32 speed;

    // matching: assigning speed inside the expression schedules its lb before the timer's lh
    overlay->timer += D_800BE9E4 * (speed = overlay->speed);

    if (speed > 0 && overlay->timer >= 237) {
        overlay->alpha = (296 - overlay->timer) << 2;
        if (overlay->alpha < 0) {
            overlay->alpha = 0;
        }
    } else {
        overlay->alpha = overlay->timer * 2;
    }

    if (overlay->alpha >= 128) {
        overlay->alpha = 128;
    }

    overlay->scroll++;
    if (overlay->scroll >= 256) {
        overlay->scroll -= 256;
    }

    if (overlay->timer > 300 || overlay->timer < 0) {
        func_1516972C((struct102 *)overlay);
    }
}

/*
 * Draws the overlay if it belongs to the given id: the 64x64 IA8 texture D_A48 tiled over the whole
 * 320x240 screen twice, scrolling horizontally: green at full alpha, then red at half alpha and 2x scale.
 */
Gfx *func_151D2830(Gfx *gfx, ScreenOverlay *overlay, s16 id) {
    s32 texture;

    if (id != overlay->id || overlay->alpha == 0) {
        return gfx;
    }

    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0,
                      TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0);
    gDPSetOtherMode(gfx++, 0x002C3F, G_RM_XLU_SURF | G_RM_XLU_SURF2 | G_ZS_PRIM);

    texture = func_1510D0EC((s32)D_A48, NULL, 3, 0);

    gDPLoadTextureBlockS(gfx++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP | G_TX_NOMIRROR,
                         G_TX_WRAP | G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetEnvColor(gfx++, 0, 255, 0, overlay->alpha);
    gSPTextureRectangle(gfx++, 0, 0, 320 << 2, 240 << 2, G_TX_RENDERTILE, overlay->scroll << 3, 32 << 5, 1 << 10,
                        1 << 10);
    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 255, 0, 0, overlay->alpha >> 1);
    gSPTextureRectangle(gfx++, 0, 0, 320 << 2, 240 << 2, G_TX_RENDERTILE, overlay->scroll << 3, 0, 1 << 9,
                        1 << 9);

    return gfx;
}
