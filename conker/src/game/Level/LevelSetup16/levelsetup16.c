#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void level_setup16(void) {
    particle_startShakeTimer();
    gSoundFxMixBoost = 0x10;
}
