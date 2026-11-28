#include <ultra64.h>

#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/sound.h"


void level_queueObjectDropFrontEnd(s32 x, s32 y, s32 z, s32 floorY) {
    ObjectEmitter *emitter = effect_newTimedEffectNoDraw((math_random() & 0x7F) + 10, 1, -1, 1, 0, 10, 255, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.markOnLanding = 1;
    }
}

void level_setupFrontEnd(void) {
    level_queueObjectDropFrontEnd(1257, 338, -2988, -367);
    level_queueObjectDropFrontEnd(1901, 338, -2753, -367);
    light_setGroupDisabled(1, 0);
    light_setGroupDisabled(2, 1);
    sound_addDynamicSound(77, 765, 55, -89, 12000, 600, 300, sound_stopWhenSilentCallback, 0, 0, 8, 0);
}
