#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void camera_levelHook09(Camera *arg0) {
    arg0->AvailableModes &= ~0x4000;
    arg0->AvailableModes |= 0x4;
    arg0->AvailableModes &= ~0x1010;
    if (arg0->CharPtr->inwater != 0) {
        arg0->AvailableModes |= 0x1010;
        arg0->AvailableModes &= ~0x4;
        arg0->IdealTripodMagnitude = 750.0f;
    } else {
        if (750.0f == arg0->IdealTripodMagnitude) {
            arg0->DistIndex = 3;
            camera_loadDistancePreset(arg0);
        }
    }
}
