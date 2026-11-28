#include <ultra64.h>

#include "conker/camera.h"
#include "conker/level.h"

/* Per-level camera setup hooks, indexed by LevelHeader.cameraSetupCallback (0 = none). */
static void (*gLevelCameraSetups[])(Camera *) = {
    NULL,
    camera_levelSetupCogLand,
    camera_emptyLevelSetupTheWindmill,
    camera_emptyLevelSetupDogFish,
    camera_levelSetupTankFactory,
    camera_emptyLevelSetupBarnOutside,
    camera_emptyLevelSetupDinoArena,
    camera_levelSetupBarnInside,
    camera_levelSetupHeist,
    camera_levelSetup09,
    camera_levelSetupTank,
    camera_emptyLevelSetupCanyon,
    camera_levelSetup12,
    camera_levelSetupPooCabin,
    camera_emptyLevelSetupWarboss,
    camera_levelSetupArena,
    camera_levelSetupVolcanoRace,
    camera_levelSetupTraining,
    camera_levelSetup18,
    camera_levelSetup19,
    camera_levelSetup20,
    camera_levelSetupUBend,
    camera_emptyLevelSetupHorrorIntro,
    camera_levelSetupRaptor,
    camera_levelSetup24,
    camera_levelSetupDogFishNurseryPart1,
    camera_levelSetup26,
    NULL,
};


void camera_runLevelSetup(Camera *camera, s32 resource) {
    if (gLevelHeader->cameraSetupCallback != 0) {
        gLevelCameraSetups[gLevelHeader->cameraSetupCallback](camera);
    }
}
