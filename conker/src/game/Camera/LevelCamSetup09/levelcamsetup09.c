#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup09(Camera *camera) {
    camera->AvailableModes |= 0x100000;
    camera->AvailableModes &= -5;
    camera->DistIndex = 2;
    camera->DistMask = 5;
    camera_loadDistancePreset(camera);
}
