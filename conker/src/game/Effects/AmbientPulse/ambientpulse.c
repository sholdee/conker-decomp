#include <ultra64.h>
#include "conker/effect.h"
#include "conker/light.h"
#include "conker/script.h"

void light_updateAmbientRedDrift(AmbientPulse *arg0) {
    f32 *p = &arg0->red;

    math_randomDrift(&arg0->timer, 10.0f, &arg0->target, 86.0f, 170.0f, 255.0f, 0.2489f, p);
    light_setAmbient(*p, gAmbientColour[1], gAmbientColour[2], 0);
}


void light_levelHookRockSolid(s32 *arg0) {
    if (script_query(1, 0x402C, 6, 0x2000) != 0) {
        arg0[0x21] |= 0x10;
    } else {
        arg0[0x21] &= ~0x10;
    }
}
