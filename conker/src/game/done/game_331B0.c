#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005D00(Camera *arg0) {
    arg0->Mode = 8;
    arg0->AvailableModes |= 0x300000;
    arg0->AvailableModes &= -5;
    arg0->DistIndex = 1;
    arg0->DistMask = 3;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 0;
}
