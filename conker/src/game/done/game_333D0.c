#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005F20(Camera *arg0) {
    arg0->DistIndex = 3;
    arg0->DistMask = 15;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 1;
}
