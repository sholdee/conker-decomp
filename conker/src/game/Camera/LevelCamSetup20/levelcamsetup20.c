#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup20(Camera *camera) {
    camera->DistIndex = 3;
    camera->DistMask = 15;
    camera_loadDistancePreset(camera);
    camera->Tightness = 1;
}
