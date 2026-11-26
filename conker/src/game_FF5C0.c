#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .bss. */
u8 gStrobeCount; /* number of live strobe effects (read by game_58F80.c) */

/* Spawns a strobe effect lasting `duration` frames. */
void screen_spawnStrobe(s16 duration, f32 litTime, f32 darkTime, u8 litAlpha, u8 darkAlpha, u8 owner, s32 kind) {
    StrobeData data;
    StrobeEffect *effect;

    gStrobeCount++;

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
void screen_updateStrobe(StrobeEffect *effect) {
    StrobeData *data = &effect->data;

    data->timer += gFrameDelta;
    while (data->timer > data->period) {
        data->timer -= data->period;
    }
    if (data->timer <= data->litTime) {
        data->lit = TRUE;
        light_setAmbient(255, 255, 255, 255);
    } else {
        data->lit = FALSE;
    }
}

/* Strobe free handler. */
void screen_freeStrobe(StrobeEffect *effect) {
    screen_uncountStrobe(effect);
    func_1514933C(&effect->base);
}

void func_150D22A8(StrobeEffect *effect) {
    screen_uncountStrobe(effect);
    func_15149368(&effect->base);
}

/* Unregisters a strobe from the live count. */
void screen_uncountStrobe(StrobeEffect *effect) {
    gStrobeCount--;
}

/* Strobe draw: fills `view` with white at litAlpha while lit, else black at darkAlpha. */
Gfx *screen_drawStrobe(Gfx *gfx, StrobeEffect *effect, s16 view) {
    if (effect->data.lit == TRUE) {
        gfx = screen_fillView(gfx, effect->data.litAlpha, 255, 255, 255, view);
    } else {
        gfx = screen_fillView(gfx, effect->data.darkAlpha, 0, 0, 0, view);
    }
    return gfx;
}

/* Strobe spawner update: when the delay runs out, spawns a strobe of random duration and rearms. */
void screen_updateStrobeSpawner(StrobeSpawner *spawner) {
    StrobeSpawnerData *data = &spawner->data;

    data->delay -= D_800BE9E4;
    if (data->delay < 0) {
        screen_spawnStrobe(func_150ADA20() % (data->durationRange + 1) + data->minDuration, data->litTime,
                      data->darkTime, data->litAlpha, data->darkAlpha, spawner->owner, spawner->kind);
        data->delay = func_150ADA20() % (data->delayRange + 1) + data->minDelay;
    }
}
