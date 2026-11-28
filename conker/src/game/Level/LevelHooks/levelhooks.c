#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"

/* Per-level hooks, indexed by the level's hook number (gLevelHeader->hookCallback; 0 = none). The handlers take
 * different arguments, so the table has no prototype. */
static void (*gLevelHooks[])() = {
    NULL, camera_levelHookCogLand, camera_levelHookTheWindmill, camera_levelHookDogFish,
    camera_levelHookTankFactory, camera_levelHookBarnOutside, camera_levelHookDinoArena, camera_levelHookBarnInside,
    effect_emptyLevelHookHeist, camera_levelHookHarbour, camera_levelHookDogFishNurseryPart1, camera_resetDistances,
    effect_levelHook12, camera_levelHookInsideSafe, effect_emptyLevelHookCanyon, camera_levelHook15,
    effect_levelHookPooCabin, particle_levelHookWarboss, camera_levelHookBullPaddock, camera_levelHookArena,
    camera_scaleBySpeed, particle_levelHookBarnArena, camera_levelHookTraining, camera_levelHookBank,
    camera_levelHookHorrorIntro, camera_levelHookRaptor, effect_levelHookW3HauntedHouse, effect_levelHookBrontoCavern,
    camera_levelHookEndArena, particle_levelHookWindmillExploded, level_hookUBend, light_levelHookRockSolid,
    camera_levelHookTankTunnel, camera_levelHookMachineGunNest, camera_levelHookBankEntrance, camera_levelHookOutsideMolehole,
};

void level_runHook(void *camera, s32 levelId) {
    if (gLevelHeader->hookCallback) {
        gLevelHooks[gLevelHeader->hookCallback](camera);
    }
}
