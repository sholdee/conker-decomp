#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011B00(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ObjectEmitter *emitter = effect_newTimedEffectNoDraw((func_150ADA20() & 0x7F) + 10, 1, -1, 1, 0, 10, 255, 0);
    if (emitter != NULL) {
        emitter->pos.x = arg0;
        emitter->pos.y = arg1;
        emitter->pos.z = arg2;
        emitter->pos.floorY = arg3;
        emitter->pos.unk8 = 1;
    }
}

void func_15011B94(void) {
    func_15011B00(1257, 338, -2988, -367);
    func_15011B00(1901, 338, -2753, -367);
    light_setGroupDisabled(1, 0);
    light_setGroupDisabled(2, 1);
    sound_addDynamicSound(77, 765, 55, -89, 12000, 600, 300, func_1000EF40, 0, 0, 8, 0);
}
