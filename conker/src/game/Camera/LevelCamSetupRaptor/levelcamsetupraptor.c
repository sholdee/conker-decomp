#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup23(Camera *camera) {
    camera->Mode = 8;
    camera->AvailableModes |= 0x300000;
    camera->AvailableModes &= -5;
    camera->DistIndex = 1;
    camera->DistMask = 3;
    camera_loadDistancePreset(camera);
    camera->Tightness = 0;
}
