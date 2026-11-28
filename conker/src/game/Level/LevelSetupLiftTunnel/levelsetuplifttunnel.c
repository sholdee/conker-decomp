#include <ultra64.h>

#include "conker/level.h"
#include "conker/particle.h"
#include "conker/sound.h"


void level_setupLiftTunnel(void) {
    particle_startShakeTimer();
    gSoundFxMixBoost = 0x10;
}
