#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetupUBend(Camera *camera) {
    camera->DistIndex = 2;
    camera_loadDistancePreset(camera);
    camera->AvailableModes |= 0x4000;
}
