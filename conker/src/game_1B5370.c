#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Animated RGB colours: each slot blends between a "from" and a "to" colour by a factor in [0, 1].
 * The factor is either set directly (type 0) or animated every frame by func_151880C0.
 */

typedef union ColorAnimWord {
    f32 f;
    s32 i;
    u32 u;
} ColorAnimWord;

typedef struct ColorAnim {
    /* 0x00 */ u8 from[3];
    /* 0x03 */ u8 to[3];
    /* 0x06 */ u8 current[3];
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ f32 blend; /* 0.0f = from, 1.0f = to */
    /* 0x10 */ s32 type;
    /* 0x14 */ f32 rate;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ ColorAnimWord state; /* type 1: sine phase (radians); types 2, 3: step */
    /* 0x20 */ ColorAnimWord timer; /* frame countdown, or the blend target while type 3 fades */
} ColorAnim; /* size = 0x24 */

#define COLOR_ANIM_MAX 5

enum ColorAnimType {
    COLOR_ANIM_STATIC,  /* blend set by func_1518804C */
    COLOR_ANIM_PULSE,   /* sine wave */
    COLOR_ANIM_FLICKER, /* flashes to "to", waits per D_800A7390 */
    COLOR_ANIM_RANDOM   /* random fades between from, half and to */
};

static ColorAnim D_800DF700[COLOR_ANIM_MAX];
static s32 D_800DF7B4; /* slots in use */
static const u8 D_800A7390[8] = { 60, 6, 100, 6, 40, 55, 6, 90 }; /* type 2: frames to wait after each flash */


/* Allocates a colour animation slot; returns its index, or -1 when all are in use. */
s32 func_15187EC0(s32 type, f32 rate, u8 fromR, u8 fromG, u8 fromB, u8 toR, u8 toG, u8 toB) {
    if (D_800DF7B4 < COLOR_ANIM_MAX) {
        D_800DF700[D_800DF7B4].type = type;
        D_800DF700[D_800DF7B4].rate = rate;
        D_800DF700[D_800DF7B4].from[0] = D_800DF700[D_800DF7B4].current[0] = fromR;
        D_800DF700[D_800DF7B4].from[1] = D_800DF700[D_800DF7B4].current[1] = fromG;
        D_800DF700[D_800DF7B4].from[2] = D_800DF700[D_800DF7B4].current[2] = fromB;
        D_800DF700[D_800DF7B4].to[0] = toR;
        D_800DF700[D_800DF7B4].to[1] = toG;
        D_800DF700[D_800DF7B4].to[2] = toB;
        bzero(&D_800DF700[D_800DF7B4].unk18, 0xC); /* unk18, state, timer */
        D_800DF7B4++;
        return D_800DF7B4 - 1;
    }
    return -1;
}

/* Frees all colour animation slots. */
void func_15187F90(void) {
    bzero(D_800DF700, sizeof(D_800DF700));
    D_800DF7B4 = 0;
}

/* Reads a slot's current colour as three ints. */
void func_15187FC0(s32 index, s32 *rgb) {
    if (index < D_800DF7B4 && index >= 0) {
        rgb[0] = D_800DF700[index].current[0];
        rgb[1] = D_800DF700[index].current[1];
        rgb[2] = D_800DF700[index].current[2];
    }
}

/* Reads a slot's blend factor. */
void func_15188010(s32 index, f32 *blend) {
    if (index < D_800DF7B4 && index >= 0) {
        *blend = D_800DF700[index].blend;
    }
}

/* Sets a slot's blend factor, clamped to [0, 1]. */
void func_1518804C(s32 index, f32 blend) {
    if (index < D_800DF7B4 && index >= 0) {
        if (blend > 1.0f) {
            blend = 1.0f;
        } else if (blend < 0.0f) {
            blend = 0.0f;
        }
        D_800DF700[index].blend = blend;
    }
}

/* Advances every colour animation by one frame and recomputes its current colour. */
void func_151880C0(void) {
    s32 i;

    for (i = 0; i < D_800DF7B4; i++) {
        switch (D_800DF700[i].type) {
        case COLOR_ANIM_STATIC:
            break;
        case COLOR_ANIM_PULSE:
            D_800DF700[i].blend = (sinf(D_800DF700[i].state.f) + 1.0f) * 0.5f;
            D_800DF700[i].state.f = D_800DF700[i].rate * gFrameDelta + D_800DF700[i].state.f;
            if (D_800DF700[i].state.f >= 6.283185482f) {
                D_800DF700[i].state.f = D_800DF700[i].state.f - 6.283185482f;
            }
            break;
        case COLOR_ANIM_FLICKER:
            if (D_800DF700[i].timer.i == 0) {
                D_800DF700[i].timer.i = -8;
                D_800DF700[i].blend = 1.0f;
            } else if (D_800DF700[i].timer.i < 0) {
                D_800DF700[i].timer.i += D_800BE9E4;
                if (D_800DF700[i].timer.i >= 0) {
                    D_800DF700[i].blend = 0.0f;
                    D_800DF700[i].state.u++;
                    if (D_800DF700[i].state.u >= 8) {
                        D_800DF700[i].state.u = 0;
                    }
                    D_800DF700[i].timer.i = D_800A7390[D_800DF700[i].state.u];
                }
            } else {
                D_800DF700[i].timer.i -= D_800BE9E4;
                if (D_800DF700[i].timer.i < 0) {
                    D_800DF700[i].timer.i = 0;
                }
            }
            break;
        case COLOR_ANIM_RANDOM:
            switch (D_800DF700[i].state.i) {
            case 0: /* fade up towards the target */
                D_800DF700[i].blend += D_800DF700[i].rate * D_800BE9E4;
                if (D_800DF700[i].timer.f <= D_800DF700[i].blend) {
                    D_800DF700[i].blend = D_800DF700[i].timer.f;
                    if (D_800DF700[i].timer.f == 1.0f) {
                        if (!(func_151EF610() & 1)) {
                            D_800DF700[i].timer.f = 0.0f;
                        } else {
                            D_800DF700[i].timer.f = 0.5f;
                        }
                        D_800DF700[i].state.i = 2;
                    } else {
                        D_800DF700[i].state.i = 1;
                        D_800DF700[i].timer.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 1: /* hold at half, then fade up to "to" or down to "from" */
                D_800DF700[i].timer.i -= D_800BE9E4;
                if (D_800DF700[i].timer.i <= 0) {
                    D_800DF700[i].state.i = (func_151EF610() % 2) * 2;
                    if (D_800DF700[i].state.i == 0) {
                        D_800DF700[i].timer.f = 1.0f;
                    } else {
                        D_800DF700[i].timer.f = 0.0f;
                    }
                }
                break;
            case 2: /* fade down towards the target */
                D_800DF700[i].blend -= D_800DF700[i].rate * D_800BE9E4;
                if (D_800DF700[i].blend <= D_800DF700[i].timer.f) {
                    D_800DF700[i].blend = D_800DF700[i].timer.f;
                    if (D_800DF700[i].timer.f == 0.0f) {
                        D_800DF700[i].state.i = 3;
                        D_800DF700[i].timer.i = func_151EF610() % 30 + 30;
                    } else {
                        D_800DF700[i].state.i = 1;
                        D_800DF700[i].timer.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 3: /* hold at "from", then fade up to half or "to" */
                D_800DF700[i].timer.i -= D_800BE9E4;
                if (D_800DF700[i].timer.i <= 0) {
                    D_800DF700[i].state.i = 0;
                    if (!(func_151EF610() & 1)) {
                        D_800DF700[i].timer.f = 0.5f;
                    } else {
                        D_800DF700[i].timer.f = 1.0f;
                    }
                }
                break;
            }
            break;
        }
        D_800DF700[i].current[0] = D_800DF700[i].from[0] + (D_800DF700[i].to[0] - D_800DF700[i].from[0]) * D_800DF700[i].blend;
        D_800DF700[i].current[1] = D_800DF700[i].from[1] + (D_800DF700[i].to[1] - D_800DF700[i].from[1]) * D_800DF700[i].blend;
        D_800DF700[i].current[2] = D_800DF700[i].from[2] + (D_800DF700[i].to[2] - D_800DF700[i].from[2]) * D_800DF700[i].blend;
    }
}
