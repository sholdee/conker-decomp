#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Number of live strobe effects (read by game_58F80.c). */
extern u8 D_800D9900;

/* Per-effect data of a strobe: the screen alternates between lit (litTime) and dark (darkTime). */
typedef struct {
    /* 0x00 */ u8 lit;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 timer; /* position in the current period */
    /* 0x08 */ f32 litTime;
    /* 0x0C */ f32 darkTime;
    /* 0x10 */ f32 period; /* litTime + darkTime */
    /* 0x14 */ u8 litAlpha;
    /* 0x15 */ u8 darkAlpha;
    /* 0x16 */ u8 pad16[2];
} StrobeData; /* size 0x18 */

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ StrobeData data;
} StrobeEffect;

/* Per-effect data of a strobe spawner: spawns a strobe after a random delay, then rearms. */
typedef struct {
    /* 0x00 */ s32 minDelay;
    /* 0x04 */ s32 delayRange;
    /* 0x08 */ s32 delay; /* frames until the next strobe */
    /* 0x0C */ s16 minDuration;
    /* 0x0E */ s16 durationRange;
    /* 0x10 */ f32 litTime;
    /* 0x14 */ f32 darkTime;
    /* 0x18 */ u8 litAlpha;
    /* 0x19 */ u8 darkAlpha;
} StrobeSpawnerData;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x28 - 0xD];
    /* 0x28 */ StrobeSpawnerData data;
} StrobeSpawner;

Gfx *func_1517F08C(Gfx *gfx, s32 alpha, s32 r, s32 g, s32 b, s32 view);

/* Spawns a strobe effect lasting `duration` frames. */
void func_150D2110(s16 duration, f32 litTime, f32 darkTime, u8 litAlpha, u8 darkAlpha, u8 owner, s32 kind) {
    StrobeData data;
    StrobeEffect *effect;

    D_800D9900++;

    data.litTime = litTime;
    data.darkTime = darkTime;
    data.period = litTime + darkTime;
    data.lit = FALSE;
    data.timer = 0.0f;
    data.litAlpha = litAlpha;
    data.darkAlpha = darkAlpha;

    effect = (StrobeEffect *)func_15149130(duration, -1, 0x2F, 2, 1, 0x26, sizeof(data), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Strobe update: advances the timer and sets `lit` for the first litTime of each period. */
void func_150D21CC(StrobeEffect *effect) {
    StrobeData *data = &effect->data;

    data->timer += D_800BE9A4;
    while (data->timer > data->period) {
        data->timer -= data->period;
    }
    if (data->timer <= data->litTime) {
        data->lit = TRUE;
        func_1515D4D4(255, 255, 255, 255);
    } else {
        data->lit = FALSE;
    }
}

void func_150D22D4(StrobeEffect *effect);

/* Strobe free handler. */
void func_150D227C(StrobeEffect *effect) {
    func_150D22D4(effect);
    func_1514933C((struct260 *)effect);
}

void func_150D22A8(StrobeEffect *effect) {
    func_150D22D4(effect);
    func_15149368((struct260 *)effect);
}

/* Unregisters a strobe from the live count. */
void func_150D22D4(StrobeEffect *effect) {
    D_800D9900--;
}

/* Strobe draw: fills `view` with white at litAlpha while lit, else black at darkAlpha. */
Gfx *func_150D22F4(Gfx *gfx, StrobeEffect *effect, s16 view) {
    if (effect->data.lit == TRUE) {
        gfx = func_1517F08C(gfx, effect->data.litAlpha, 255, 255, 255, view);
    } else {
        gfx = func_1517F08C(gfx, effect->data.darkAlpha, 0, 0, 0, view);
    }
    return gfx;
}

/* Strobe spawner update: when the delay runs out, spawns a strobe of random duration and rearms. */
void func_150D2374(StrobeSpawner *spawner) {
    StrobeSpawnerData *data = &spawner->data;

    data->delay -= D_800BE9E4;
    if (data->delay < 0) {
        func_150D2110(func_150ADA20() % (u32)(data->durationRange + 1) + data->minDuration, data->litTime,
                      data->darkTime, data->litAlpha, data->darkAlpha, spawner->unkC, spawner->unk1);
        data->delay = func_150ADA20() % (u32)(data->delayRange + 1) + data->minDelay;
    }
}
