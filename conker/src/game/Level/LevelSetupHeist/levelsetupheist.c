#include <ultra64.h>

#include "conker/level.h"

u8 gFlagLightStates[5]; /* level flags (game/Effects/FlagLights/flaglights toggles them) */


void level_setup14(void) {
    bzero(&gFlagLightStates, 5); // bzero
}
