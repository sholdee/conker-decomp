#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Animated RGB colours: each slot blends between a "from" and a "to" colour by a factor in [0, 1].
 * The factor is either set directly (type 0) or animated every frame by effect_updateColorAnims.
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
    COLOR_ANIM_STATIC,  /* blend set by effect_setColorAnimBlend */
    COLOR_ANIM_PULSE,   /* sine wave */
    COLOR_ANIM_FLICKER, /* flashes to "to", waits per gColorFlickerWaits */
    COLOR_ANIM_RANDOM   /* random fades between from, half and to */
};

static ColorAnim gColorAnims[COLOR_ANIM_MAX];
static s32 gColorAnimCount; /* slots in use */
static const u8 gColorFlickerWaits[8] = { 60, 6, 100, 6, 40, 55, 6, 90 }; /* type 2: frames to wait after each flash */


/* Allocates a colour animation slot; returns its index, or -1 when all are in use. */
s32 effect_newColorAnim(s32 type, f32 rate, u8 fromR, u8 fromG, u8 fromB, u8 toR, u8 toG, u8 toB) {
    if (gColorAnimCount < COLOR_ANIM_MAX) {
        gColorAnims[gColorAnimCount].type = type;
        gColorAnims[gColorAnimCount].rate = rate;
        gColorAnims[gColorAnimCount].from[0] = gColorAnims[gColorAnimCount].current[0] = fromR;
        gColorAnims[gColorAnimCount].from[1] = gColorAnims[gColorAnimCount].current[1] = fromG;
        gColorAnims[gColorAnimCount].from[2] = gColorAnims[gColorAnimCount].current[2] = fromB;
        gColorAnims[gColorAnimCount].to[0] = toR;
        gColorAnims[gColorAnimCount].to[1] = toG;
        gColorAnims[gColorAnimCount].to[2] = toB;
        bzero(&gColorAnims[gColorAnimCount].unk18, 0xC); /* unk18, state, timer */
        gColorAnimCount++;
        return gColorAnimCount - 1;
    }
    return -1;
}

/* Frees all colour animation slots. */
void effect_clearColorAnims(void) {
    bzero(gColorAnims, sizeof(gColorAnims));
    gColorAnimCount = 0;
}

/* Reads a slot's current colour as three ints. */
void effect_getColorAnimColor(s32 index, s32 *rgb) {
    if (index < gColorAnimCount && index >= 0) {
        rgb[0] = gColorAnims[index].current[0];
        rgb[1] = gColorAnims[index].current[1];
        rgb[2] = gColorAnims[index].current[2];
    }
}

/* Reads a slot's blend factor. */
void effect_getColorAnimBlend(s32 index, f32 *blend) {
    if (index < gColorAnimCount && index >= 0) {
        *blend = gColorAnims[index].blend;
    }
}

/* Sets a slot's blend factor, clamped to [0, 1]. */
void effect_setColorAnimBlend(s32 index, f32 blend) {
    if (index < gColorAnimCount && index >= 0) {
        if (blend > 1.0f) {
            blend = 1.0f;
        } else if (blend < 0.0f) {
            blend = 0.0f;
        }
        gColorAnims[index].blend = blend;
    }
}

/* Advances every colour animation by one frame and recomputes its current colour. */
void effect_updateColorAnims(void) {
    s32 i;

    for (i = 0; i < gColorAnimCount; i++) {
        switch (gColorAnims[i].type) {
        case COLOR_ANIM_STATIC:
            break;
        case COLOR_ANIM_PULSE:
            gColorAnims[i].blend = (sinf(gColorAnims[i].state.f) + 1.0f) * 0.5f;
            gColorAnims[i].state.f = gColorAnims[i].rate * gFrameDelta + gColorAnims[i].state.f;
            if (gColorAnims[i].state.f >= 6.283185482f) {
                gColorAnims[i].state.f = gColorAnims[i].state.f - 6.283185482f;
            }
            break;
        case COLOR_ANIM_FLICKER:
            if (gColorAnims[i].timer.i == 0) {
                gColorAnims[i].timer.i = -8;
                gColorAnims[i].blend = 1.0f;
            } else if (gColorAnims[i].timer.i < 0) {
                gColorAnims[i].timer.i += D_800BE9E4;
                if (gColorAnims[i].timer.i >= 0) {
                    gColorAnims[i].blend = 0.0f;
                    gColorAnims[i].state.u++;
                    if (gColorAnims[i].state.u >= 8) {
                        gColorAnims[i].state.u = 0;
                    }
                    gColorAnims[i].timer.i = gColorFlickerWaits[gColorAnims[i].state.u];
                }
            } else {
                gColorAnims[i].timer.i -= D_800BE9E4;
                if (gColorAnims[i].timer.i < 0) {
                    gColorAnims[i].timer.i = 0;
                }
            }
            break;
        case COLOR_ANIM_RANDOM:
            switch (gColorAnims[i].state.i) {
            case 0: /* fade up towards the target */
                gColorAnims[i].blend += gColorAnims[i].rate * D_800BE9E4;
                if (gColorAnims[i].timer.f <= gColorAnims[i].blend) {
                    gColorAnims[i].blend = gColorAnims[i].timer.f;
                    if (gColorAnims[i].timer.f == 1.0f) {
                        if (!(func_151EF610() & 1)) {
                            gColorAnims[i].timer.f = 0.0f;
                        } else {
                            gColorAnims[i].timer.f = 0.5f;
                        }
                        gColorAnims[i].state.i = 2;
                    } else {
                        gColorAnims[i].state.i = 1;
                        gColorAnims[i].timer.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 1: /* hold at half, then fade up to "to" or down to "from" */
                gColorAnims[i].timer.i -= D_800BE9E4;
                if (gColorAnims[i].timer.i <= 0) {
                    gColorAnims[i].state.i = (func_151EF610() % 2) * 2;
                    if (gColorAnims[i].state.i == 0) {
                        gColorAnims[i].timer.f = 1.0f;
                    } else {
                        gColorAnims[i].timer.f = 0.0f;
                    }
                }
                break;
            case 2: /* fade down towards the target */
                gColorAnims[i].blend -= gColorAnims[i].rate * D_800BE9E4;
                if (gColorAnims[i].blend <= gColorAnims[i].timer.f) {
                    gColorAnims[i].blend = gColorAnims[i].timer.f;
                    if (gColorAnims[i].timer.f == 0.0f) {
                        gColorAnims[i].state.i = 3;
                        gColorAnims[i].timer.i = func_151EF610() % 30 + 30;
                    } else {
                        gColorAnims[i].state.i = 1;
                        gColorAnims[i].timer.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 3: /* hold at "from", then fade up to half or "to" */
                gColorAnims[i].timer.i -= D_800BE9E4;
                if (gColorAnims[i].timer.i <= 0) {
                    gColorAnims[i].state.i = 0;
                    if (!(func_151EF610() & 1)) {
                        gColorAnims[i].timer.f = 0.5f;
                    } else {
                        gColorAnims[i].timer.f = 1.0f;
                    }
                }
                break;
            }
            break;
        }
        gColorAnims[i].current[0] = gColorAnims[i].from[0] + (gColorAnims[i].to[0] - gColorAnims[i].from[0]) * gColorAnims[i].blend;
        gColorAnims[i].current[1] = gColorAnims[i].from[1] + (gColorAnims[i].to[1] - gColorAnims[i].from[1]) * gColorAnims[i].blend;
        gColorAnims[i].current[2] = gColorAnims[i].from[2] + (gColorAnims[i].to[2] - gColorAnims[i].from[2]) * gColorAnims[i].blend;
    }
}
