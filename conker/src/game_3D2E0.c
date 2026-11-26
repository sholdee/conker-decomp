#include <ultra64.h>
#include "functions.h"
#include "variables.h"

BounceRecord *D_800BE4E0; /* ten bouncing cursors */



void func_1500FE30(void) {
    s32 i;

    D_800BE4E0 = allocate_memory(10 * sizeof(BounceRecord), 1, 0, 0);
    bzero(D_800BE4E0, 10 * sizeof(BounceRecord));
    for (i = 0; i < 10; i++) {
        D_800BE4E0[i].table = i / 5;
        D_800BE4E0[i].dir = -1;
        D_800BE4E0[i].pos = func_150ADA20() % 5;
    }
    func_150C851C(100);
    for (i = 0; i < 10; i++) {
        D_800BE4E0[i].speed = D_800BE4E0[i].targetSpeed;
    }
}

void func_1500FF9C(void) {
    texture_addUvWobble(D_800917B8, 0, 0xAD, 0x75);
}

void func_1500FFCC(void) {
    f32 sp44;
    TimedEffect *fx;

    flamethrower_addTexScroll(D_800B0E00[0], D_800902DC, 1, -1, 0, 0, 0, -4);
    flamethrower_addTexScroll(D_800B0E00[1], D_800902DC, 1, -1, 0, 1, 0, -4);
    func_1500FE30();
    func_1500FF9C();
    sp44 = 0.0f;
    fx = func_151491F4(0x12C, -1, 0x12, 0, 0xE, 4, 0xFF, 0);
    if (fx != NULL) {
        memcpy(fx + 1, &sp44, sizeof(sp44));
    }
    {
        BurstTimer timer;
        DriftBurstEmitter *emitter;

        timer.pending = 0.0f;
        timer.rate = 0.0f;
        emitter = func_15149130(0x12C, -1, 0x17, -1, 0, 0x14, sizeof(BurstTimer), 0xFF, 1);
        if ((D_80088870 = emitter) != NULL) {
            memcpy(&emitter->timer, &timer, sizeof(timer));
        }
    }
}
