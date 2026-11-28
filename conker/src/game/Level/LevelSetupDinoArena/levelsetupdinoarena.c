#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/particle.h"
#include "conker/texture.h"

BounceRecord *gBounceRecords; /* ten bouncing cursors */



void level_initBounceRecords(void) {
    s32 i;

    gBounceRecords = allocate_memory(10 * sizeof(BounceRecord), 1, 0, 0);
    bzero(gBounceRecords, 10 * sizeof(BounceRecord));
    for (i = 0; i < 10; i++) {
        gBounceRecords[i].table = i / 5;
        gBounceRecords[i].dir = -1;
        gBounceRecords[i].pos = math_random() % 5;
    }
    effect_randomizeBounceRecordSpeeds(100);
    for (i = 0; i < 10; i++) {
        gBounceRecords[i].speed = gBounceRecords[i].targetSpeed;
    }
}

void level_addUvWobbleDinoArena(void) {
    texture_addUvWobble(D_800917B8, 0, 0xAD, 0x75);
}

void level_setupDinoArena(void) {
    f32 accum;
    TimedEffect *fx;

    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902DC, 1, -1, 0, 0, 0, -4);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902DC, 1, -1, 0, 1, 0, -4);
    level_initBounceRecords();
    level_addUvWobbleDinoArena();
    accum = 0.0f;
    fx = effect_newTimedEffectNoDraw(0x12C, -1, 0x12, 0, 0xE, 4, 0xFF, 0);
    if (fx != NULL) {
        memcpy(fx + 1, &accum, sizeof(accum));
    }
    {
        BurstTimer timer;
        DriftBurstEmitter *emitter;

        timer.pending = 0.0f;
        timer.rate = 0.0f;
        emitter = effect_newTimedEffect(0x12C, -1, 0x17, -1, 0, 0x14, sizeof(BurstTimer), 0xFF, 1);
        if ((gDriftBurstEmitter = emitter) != NULL) {
            memcpy(&emitter->timer, &timer, sizeof(timer));
        }
    }
}
