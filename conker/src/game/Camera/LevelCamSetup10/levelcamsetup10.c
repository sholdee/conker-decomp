#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelSetup10(Camera *arg0) {
    arg0->Mode = 8;
    arg0->AvailableModes |= 0x1300000;
    arg0->DistIndex = 1;
    arg0->DistMask = 2;
    arg0->height = 117.0f;
    arg0->unk34C = 117.0f;
    arg0->YOffsetTget = 0.0f;
    arg0->YOffset = 0.0f;
    arg0->Tightness = 0;
    arg0->SpecialFlags |= 0x10;
    arg0->Snap = 1;
    arg0->IdealTripodMagnitude = 370.0f;
}

void camera_emptyLevelSetup11(Camera *arg0) {
}
