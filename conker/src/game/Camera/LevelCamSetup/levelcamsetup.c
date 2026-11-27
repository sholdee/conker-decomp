#include <ultra64.h>

#include "conker/camera.h"
#include "conker/level.h"

/* Per-level camera setup hooks, indexed by LevelHeader.cameraSetupCallback (0 = none). */
static void (*gLevelCameraSetups[])(Camera *) = {
    NULL,
    camera_levelSetup01,
    camera_emptyLevelSetup02,
    camera_emptyLevelSetup03,
    camera_levelSetup04,
    camera_emptyLevelSetup05,
    camera_emptyLevelSetup06,
    camera_levelSetup07,
    camera_levelSetup08,
    camera_levelSetup09,
    camera_levelSetup10,
    camera_emptyLevelSetup11,
    camera_levelSetup12,
    camera_levelSetup13,
    camera_emptyLevelSetup14,
    camera_levelSetup15,
    camera_levelSetup16,
    camera_levelSetup17,
    camera_levelSetup18,
    camera_levelSetup19,
    camera_levelSetup20,
    camera_levelSetup21,
    camera_emptyLevelSetup22,
    camera_levelSetup23,
    camera_levelSetup24,
    camera_levelSetup25,
    camera_levelSetup26,
    NULL,
};


void camera_runLevelSetup(Camera *arg0, s32 arg1) {
    if (gLevelHeader->cameraSetupCallback != 0) {
        gLevelCameraSetups[gLevelHeader->cameraSetupCallback](arg0);
    }
}
