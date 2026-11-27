#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup09(Camera *arg0) {
    arg0->AvailableModes |= 0x100000;
    arg0->AvailableModes &= -5;
    arg0->DistIndex = 2;
    arg0->DistMask = 5;
    camera_loadDistancePreset(arg0);
}
