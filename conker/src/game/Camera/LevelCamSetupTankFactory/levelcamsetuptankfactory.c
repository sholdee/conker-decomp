#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup04(Camera *camera) {
    camera->DistMask = 8;
    camera->DistIndex = 3;
    camera->AvailableModes = camera->AvailableModes & -5;
    camera_loadDistancePreset(camera);
}
