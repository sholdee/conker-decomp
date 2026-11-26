#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

void func_1500FF9C(void) {
    texture_addUvWobble(D_800917B8, 0, 0xAD, 0x75);
}

void level_setup12(void) {
    f32 sp44;
    TimedEffect *fx;

    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902DC, 1, -1, 0, 0, 0, -4);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902DC, 1, -1, 0, 1, 0, -4);
    level_initBounceRecords();
    func_1500FF9C();
    sp44 = 0.0f;
    fx = effect_newTimedEffectNoDraw(0x12C, -1, 0x12, 0, 0xE, 4, 0xFF, 0);
    if (fx != NULL) {
        memcpy(fx + 1, &sp44, sizeof(sp44));
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
