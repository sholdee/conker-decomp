#include <ultra64.h>

#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/sound.h"


void level_queueObjectDrop17(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ObjectEmitter *emitter = effect_newTimedEffectNoDraw((math_random() & 0x7F) + 10, 1, -1, 1, 0, 10, 255, 0);
    if (emitter != NULL) {
        emitter->pos.x = arg0;
        emitter->pos.y = arg1;
        emitter->pos.z = arg2;
        emitter->pos.floorY = arg3;
        emitter->pos.markOnLanding = 1;
    }
}

void level_setup17(void) {
    level_queueObjectDrop17(1257, 338, -2988, -367);
    level_queueObjectDrop17(1901, 338, -2753, -367);
    light_setGroupDisabled(1, 0);
    light_setGroupDisabled(2, 1);
    sound_addDynamicSound(77, 765, 55, -89, 12000, 600, 300, sound_stopWhenSilentCallback, 0, 0, 8, 0);
}
