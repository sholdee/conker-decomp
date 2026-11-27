#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup24(Camera *camera) {
    if (camera->CharPtr->unk102 != 0) {
        if (camera_pushMode(camera, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            camera->AvailableModes |= 0x100000;
            camera->AvailableModes &= ~4;
            camera->DistIndex = 1;
            camera->DistMask = 3;
            camera_loadDistancePreset(camera);
        }
        camera->height = 143.0f;
        camera->idealHeight = 143.0f;
        camera->IdealTripodMagnitude = 150.0f;
        camera->YOffsetTget = -38.0f;
        return;
    }

    if (camera->CharContext == 59) {
        if (camera_pushMode(camera, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            camera->AvailableModes |= 0x100000;
            camera->AvailableModes &= ~4;
            camera->height = 155.0f;
            camera->idealHeight = 155.0f;
            camera->YOffsetTget = 30.0f;
            camera->YOffset = 30.0f;
            camera->IdealTripodMagnitude = 178.0f;
        }
    } else {
        camera_popMode(camera, CAMERA_MODE_NORMAL);
    }
}

void camera_levelSetup25(Camera *camera) {
    camera->DistMask = 5;
    camera->DistIndex = 2;
    camera_loadDistancePreset(camera);
}
