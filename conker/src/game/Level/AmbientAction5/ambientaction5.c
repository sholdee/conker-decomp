#include <ultra64.h>

#include "conker/effect.h"
#include "conker/level.h"


void level_startAmbientPulse(s32 unused) {
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
