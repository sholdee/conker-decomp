#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"

/* Per-level hooks, indexed by the level's hook number (gLevelHeader->hookCallback; 0 = none). The handlers take
 * different arguments, so the table has no prototype. */
static void (*gLevelHooks[])() = {
    NULL, camera_levelHookCogLand, effect_levelHook02, camera_levelHookDogFish,
    camera_levelHookTankFactory, camera_levelHookBarnOutside, camera_levelHookDinoArena, camera_levelHookBarnInside,
    effect_emptyLevelHookHeist, camera_levelHookHarbour, camera_levelHookDogFishNurseryPart1, camera_resetDistances,
    effect_levelHook12, camera_levelHookInsideSafe, effect_emptyLevelHookCanyon, camera_levelHook15,
    effect_levelHook16, particle_levelHook17, camera_levelHookBullPaddock, camera_levelHookArena,
    camera_scaleBySpeed, particle_levelHook21, camera_levelHookTraining, camera_levelHook23,
    player_levelHook24, camera_levelHookRaptor, effect_levelHook26, effect_levelHook27,
    camera_levelHookEndArena, particle_levelHook29, level_hookUBend, light_levelHook31,
    camera_levelHookTankTunnel, camera_levelHookMachineGunNest, camera_levelHookBankEntrance, camera_levelHookOutsideMolehole,
};

void level_runHook(void *camera, s32 levelId) {
    if (gLevelHeader->hookCallback) {
        gLevelHooks[gLevelHeader->hookCallback](camera);
    }
}
