#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup21(Camera *camera) {
    camera->DistIndex = 2;
    camera_loadDistancePreset(camera);
    camera->AvailableModes |= 0x4000;
}
