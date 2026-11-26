#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelSetup17(Camera *arg0) {
    arg0->DistIndex = 2;
    camera_loadDistancePreset(arg0);
    arg0->Snap = 1;
}

void camera_levelSetup18(Camera *arg0) {
    arg0->DistIndex = 1;
    arg0->DistMask = 15;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 1;
}
