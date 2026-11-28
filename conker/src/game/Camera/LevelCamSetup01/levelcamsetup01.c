#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetupCogLand(Camera *camera) {
    camera->DistIndex = 1;
    camera->DistMask = 3;
    camera->height = 230.0f;
    camera->idealHeight = 230.0f;
    camera->AvailableModes |= 0x4000;
    camera->IdealTripodMagnitude = 290.0f;
}

void camera_emptyLevelSetupTheWindmill(Camera *camera) {
}
