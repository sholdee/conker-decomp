#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup01(Camera *camera) {
    camera->DistIndex = 1;
    camera->DistMask = 3;
    camera->height = 230.0f;
    camera->unk34C = 230.0f;
    camera->AvailableModes |= 0x4000;
    camera->IdealTripodMagnitude = 290.0f;
}

void camera_emptyLevelSetup02(Camera *camera) {
}
