#include <ultra64.h>

#include "conker/camera.h"
#include "conker/game.h"


void camera_levelSetup16(Camera *arg0) {
    if (level == LEVEL_W1_VOLCANO_RACE) {
        arg0->DistIndex = 2;
        arg0->DistMask = 4;
        camera_loadDistancePreset(arg0);
    }
}
