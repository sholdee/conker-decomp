#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetupTankFactory(Camera *camera) {
    camera->DistMask = 8;
    camera->DistIndex = 3;
    camera->AvailableModes = camera->AvailableModes & -5;
    camera_loadDistancePreset(camera);
}
