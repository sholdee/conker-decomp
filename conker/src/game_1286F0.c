#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Layout of Effect (game_1844C0.c), as far as func_150FB29C uses it. */
typedef struct {
    /* 0x000 */ u8 pad0[0x16];
    /* 0x016 */ s16 lifetime;  /* EffectParams.lifetime, counts down */
    /* 0x018 */ u8 pad18[0x2B];
    /* 0x043 */ u8 alpha;      /* EffectParams.color0 alpha */
    /* 0x044 */ u8 pad44[0x20];
    /* 0x064 */ s16 fadeStart; /* EffectParams.fadeStart */
    /* 0x066 */ s16 fadeRate;  /* EffectParams.fadeRate */
    /* 0x068 */ u8 pad68[0xB8];
    /* 0x120 */ s16 duration;  /* Effect.extra: the lifetime's starting value */
} FadeInEffect;

/*
 * Fades `alpha` in: while `timer` (counting down from `duration`) is within its first `fadeLength`
 * ticks, alpha ramps up by `fadeRate` per elapsed tick; after that it is fully opaque.
 */
void func_150FB240(u8 *alpha, s16 timer, s16 duration, s16 fadeLength, s16 fadeRate) {
    if (duration - fadeLength < timer) {
        *alpha = (duration - timer) * fadeRate;
    } else {
        *alpha = 255;
    }
}

/* D_8008AD90 effect update callback: fades the effect's alpha in over its first fadeStart ticks. */
s32 func_150FB29C(FadeInEffect *effect) {
    func_150FB240(&effect->alpha, effect->lifetime, effect->duration, effect->fadeStart, effect->fadeRate);
    return 1;
}
