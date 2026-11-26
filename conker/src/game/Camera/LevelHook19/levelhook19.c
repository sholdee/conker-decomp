#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelHook19(Camera *arg0) {
    arg0->height = 500.0f;
    arg0->unk34C = 500.0f;
    arg0->IdealTripodMagnitude = 800.0f;
}
