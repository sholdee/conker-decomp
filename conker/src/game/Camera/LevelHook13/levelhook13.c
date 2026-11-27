#include <ultra64.h>

#include "conker/camera.h"


void camera_levelHook13(Camera *camera) {
    camera->AvailableModes |= 0x10;
    if (camera->DistIndex == 3) {
        camera->DistMask = 5;
        camera->DistIndex = 2;
        camera_loadDistancePreset(camera);
    }
}
