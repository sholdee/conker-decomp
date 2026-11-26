#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005E30(Camera *arg0) {
    if (level == 50) {
        arg0->DistIndex = 2;
        arg0->DistMask = 4;
        camera_loadDistancePreset(arg0);
    }
}
