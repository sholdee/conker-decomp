#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelSetup16(Camera *arg0) {
    if (level == 50) {
        arg0->DistIndex = 2;
        arg0->DistMask = 4;
        camera_loadDistancePreset(arg0);
    }
}
