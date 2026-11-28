#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup15(Camera *camera) {
    camera_pushMode(camera, 8, 0, camera->Tightness, CAMERA_MODE_TRANSPORT);
    camera->AvailableModes |= 0x1000000;
    camera->AvailableModes &= -5;
    camera->height = 500.0f;
    camera->idealHeight = 500.0f;
    camera->IdealTripodMagnitude = 800.0f;
    camera->DoingAim = 0;
}
