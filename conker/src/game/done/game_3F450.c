#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011FA0(s32 arg0) {
    Vec3f tmp;
    TimedEffect *effect;

    tmp.x = 50.0f;
    tmp.y = 50.0f;
    tmp.z = 0.0f;
    effect = effect_newTimedEffect(300, -1, 0x48, -1, 0, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, 12);
    }
}
