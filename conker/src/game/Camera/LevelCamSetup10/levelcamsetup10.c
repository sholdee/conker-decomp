#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup10(Camera *camera) {
    camera->Mode = 8;
    camera->AvailableModes |= 0x1300000;
    camera->DistIndex = 1;
    camera->DistMask = 2;
    camera->height = 117.0f;
    camera->idealHeight = 117.0f;
    camera->YOffsetTget = 0.0f;
    camera->YOffset = 0.0f;
    camera->Tightness = 0;
    camera->SpecialFlags |= 0x10;
    camera->Snap = 1;
    camera->IdealTripodMagnitude = 370.0f;
}

void camera_emptyLevelSetup11(Camera *camera) {
}
