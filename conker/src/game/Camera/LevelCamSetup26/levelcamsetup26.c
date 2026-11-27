#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup26(Camera *camera) {
    camera->DistMask = 5;
    camera->DistIndex = 2;
    camera_loadDistancePreset(camera);
    camera->AvailableModes |= 0x4000;
}
