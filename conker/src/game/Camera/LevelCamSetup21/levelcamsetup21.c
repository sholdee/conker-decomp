#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup21(Camera *arg0) {
    arg0->DistIndex = 2;
    camera_loadDistancePreset(arg0);
    arg0->AvailableModes |= 0x4000;
}
