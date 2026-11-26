#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005DB0(Camera *arg0) {
    camera_pushMode(arg0, 8, 0, arg0->Tightness, CAMERA_MODE_TRANSPORT);
    arg0->AvailableModes |= 0x1000000;
    arg0->AvailableModes &= -5;
    arg0->height = 500.0f;
    arg0->unk34C = 500.0f;
    arg0->IdealTripodMagnitude = 800.0f;
    arg0->DoingAim = 0;
}
