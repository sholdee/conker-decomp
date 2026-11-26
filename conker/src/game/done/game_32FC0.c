#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005B10(Camera *arg0) {
    arg0->DistMask = 8;
    arg0->DistIndex = 3;
    arg0->AvailableModes = arg0->AvailableModes & -5;
    camera_loadDistancePreset(arg0);
}
