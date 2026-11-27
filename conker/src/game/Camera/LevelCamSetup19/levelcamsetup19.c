#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup19(Camera *arg0) {
    arg0->DistIndex = 2;
    arg0->DistMask = 0xF;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 1;
}
