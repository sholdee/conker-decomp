#include <ultra64.h>
#include "conker/camera.h"


void camera_levelHook09(Camera *camera) {
    camera->AvailableModes &= ~0x4000;
    camera->AvailableModes |= 0x4;
    camera->AvailableModes &= ~0x1010;
    if (camera->CharPtr->inwater != 0) {
        camera->AvailableModes |= 0x1010;
        camera->AvailableModes &= ~0x4;
        camera->IdealTripodMagnitude = 750.0f;
    } else {
        if (750.0f == camera->IdealTripodMagnitude) {
            camera->DistIndex = 3;
            camera_loadDistancePreset(camera);
        }
    }
}
