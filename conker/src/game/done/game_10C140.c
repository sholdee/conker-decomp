#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void effect_levelUpdateRain(s32 arg0) {
    effect_updateRain(0);
    gRainStateTimer = 0;
}
