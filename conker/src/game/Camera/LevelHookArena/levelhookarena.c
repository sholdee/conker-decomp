#include <ultra64.h>

#include "conker/camera.h"


void camera_levelHookArena(Camera *camera) {
    camera->height = 500.0f;
    camera->idealHeight = 500.0f;
    camera->IdealTripodMagnitude = 800.0f;
}
