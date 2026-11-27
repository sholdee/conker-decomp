#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup13(Camera *camera) {
    camera->DistIndex = 2;
    camera->DistMask = 5;
    camera_loadDistancePreset(camera);
    camera->Tightness = 0;
    camera->AvailableModes |= 0x4000;
}

void camera_emptyLevelSetup14(Camera *camera) {
}
