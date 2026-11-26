#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005D60(Camera *arg0) {
    arg0->DistIndex = 2;
    arg0->DistMask = 5;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 0;
    arg0->AvailableModes |= 0x4000;
}

void func_15005DA0(Camera *arg0) {
}
