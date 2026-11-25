#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern u8 D_A48[];

#define WGFX151D2830(pkt, a, b)    \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

/*
 * Object type 0x3C (list 1): a full-screen overlay that fades in, scrolls its texture vertically and fades
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
    /* 0x12 */ s16 scroll; /* texture T offset, wraps at 256 */
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

Gfx *func_151D2830(Gfx *gfx, ScreenOverlay *arg1, s16 arg2) {
    s32 temp_v0;

    if ((arg2 != arg1->id) || (arg1->alpha == 0)) {
        return gfx;
    }

    WGFX151D2830(gfx++, 0xD7000002, 0xFFFFFFFF);
    WGFX151D2830(gfx++, 0xE7000000, 0);
    WGFX151D2830(gfx++, 0xFC12D225, 0xFFA7FFFF);
    WGFX151D2830(gfx++, 0xEF002C3F, 0x00504244);

    temp_v0 = func_1510D0EC((s32)D_A48, 0, 3, 0);

    WGFX151D2830(gfx++, 0xFD700000, temp_v0);
    WGFX151D2830(gfx++, 0xF5700000, 0x07018060);
    WGFX151D2830(gfx++, 0xE6000000, 0);
    WGFX151D2830(gfx++, 0xF3000000, 0x077FF000);
    WGFX151D2830(gfx++, 0xE7000000, 0);
    WGFX151D2830(gfx++, 0xF5681000, 0x00018060);
    WGFX151D2830(gfx++, 0xF2000000, 0x000FC0FC);
    WGFX151D2830(gfx++, 0xFB000000, (arg1->alpha & 0xFF) | 0x00FF0000);
    gSPTextureRectangle(gfx++, 0, 0, 0x500, 0x3C0, 0, arg1->scroll << 3, 0x400, 0x400, 0x400);
    WGFX151D2830(gfx++, 0xE7000000, 0);
    WGFX151D2830(gfx++, 0xFB000000, ((arg1->alpha >> 1) & 0xFF) | 0xFF000000);
    gSPTextureRectangle(gfx++, 0, 0, 0x500, 0x3C0, 0, arg1->scroll << 3, 0, 0x200, 0x200);

    return gfx;
}
