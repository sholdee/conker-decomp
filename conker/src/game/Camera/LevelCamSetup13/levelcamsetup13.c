#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup13(Camera *arg0) {
    arg0->DistIndex = 2;
    arg0->DistMask = 5;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 0;
    arg0->AvailableModes |= 0x4000;
}

void camera_emptyLevelSetup14(Camera *arg0) {
}
