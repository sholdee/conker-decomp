#include <ultra64.h>

#include "conker/camera.h"


void camera_levelHook19(Camera *camera) {
    camera->height = 500.0f;
    camera->unk34C = 500.0f;
    camera->IdealTripodMagnitude = 800.0f;
}
