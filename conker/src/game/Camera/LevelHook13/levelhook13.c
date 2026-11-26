#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelHook13(Camera *arg0) {
    arg0->AvailableModes |= 0x10;
    if (arg0->DistIndex == 3) {
        arg0->DistMask = 5;
        arg0->DistIndex = 2;
        camera_loadDistancePreset(arg0);
    }
}
