#include <ultra64.h>

#include "conker/camera.h"


void camera_levelSetup20(Camera *arg0) {
    arg0->DistIndex = 3;
    arg0->DistMask = 15;
    camera_loadDistancePreset(arg0);
    arg0->Tightness = 1;
}
