#include <ultra64.h>

#include "conker/camera.h"
#include "conker/game.h"


void camera_levelSetup16(Camera *camera) {
    if (level == LEVEL_W1_VOLCANO_RACE) {
        camera->DistIndex = 2;
        camera->DistMask = 4;
        camera_loadDistancePreset(camera);
    }
}
