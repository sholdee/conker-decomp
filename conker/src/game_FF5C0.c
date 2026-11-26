#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Number of live strobe effects (read by game_58F80.c). */

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

    effect = func_15149130(duration, -1, 0x2F, 2, 1, 0x26, sizeof(data), owner, kind);
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

/* Strobe free handler. */
void func_150D227C(StrobeEffect *effect) {
    func_150D22D4(effect);
    func_1514933C(&effect->base);
}

void func_150D22A8(StrobeEffect *effect) {
    func_150D22D4(effect);
    func_15149368(&effect->base);
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
