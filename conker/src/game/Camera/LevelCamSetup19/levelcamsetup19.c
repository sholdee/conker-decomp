#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup19(Camera *camera) {
    camera->DistIndex = 2;
    camera->DistMask = 0xF;
    camera_loadDistancePreset(camera);
    camera->Tightness = 1;
}
