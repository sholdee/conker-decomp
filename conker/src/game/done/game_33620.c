#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15006170(Camera *arg0) {
    arg0->DistMask = 5;
    arg0->DistIndex = 2;
    camera_loadDistancePreset(arg0);
    arg0->AvailableModes |= 0x4000;
}
