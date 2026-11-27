#include <ultra64.h>

#include "conker/effect.h"


void effect_levelUpdateRain(s32 arg0) {
    effect_updateRain(0);
    gRainStateTimer = 0;
}
