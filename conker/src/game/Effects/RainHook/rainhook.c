#include <ultra64.h>

#include "conker/effect.h"


void effect_levelUpdateRain(s32 unused) {
    effect_updateRain(0);
    gRainStateTimer = 0;
}
