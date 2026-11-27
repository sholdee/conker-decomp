#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup17(Camera *camera) {
    camera->DistIndex = 2;
    camera_loadDistancePreset(camera);
    camera->Snap = 1;
}

void camera_levelSetup18(Camera *camera) {
    camera->DistIndex = 1;
    camera->DistMask = 15;
    camera_loadDistancePreset(camera);
    camera->Tightness = 1;
}
