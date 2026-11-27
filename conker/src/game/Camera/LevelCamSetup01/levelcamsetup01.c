#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup01(Camera *arg0) {
    arg0->DistIndex = 1;
    arg0->DistMask = 3;
    arg0->height = 230.0f;
    arg0->unk34C = 230.0f;
    arg0->AvailableModes |= 0x4000;
    arg0->IdealTripodMagnitude = 290.0f;
}

void camera_emptyLevelSetup02(Camera *arg0) {
}
