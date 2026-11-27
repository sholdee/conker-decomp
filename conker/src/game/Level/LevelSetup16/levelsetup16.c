#include <ultra64.h>

#include "conker/level.h"
#include "conker/particle.h"
#include "conker/sound.h"


void level_setup16(void) {
    particle_startShakeTimer();
    gSoundFxMixBoost = 0x10;
}
