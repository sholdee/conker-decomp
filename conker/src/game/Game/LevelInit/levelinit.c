#include <ultra64.h>

#include "conker/aim.h"
#include "conker/effect.h"
#include "conker/env.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/os.h"
#include "conker/particle.h"
#include "conker/script.h"
#include "conker/shadow.h"
#include "conker/sprite.h"
#include "conker/view.h"
#include "conker/weapon.h"

/* Per-level setup routines, indexed by LevelHeader.setupCallback (0: none). */
static void (*gLevelSetupFuncs[])(void) = {
    NULL,
    level_setup01, level_setupBoilerRoom, level_emptySetupBarnInside,
    level_setupBarnOutside, level_setupBrontoCavern, level_setupOutsideMolehole,
    level_emptySetup07, level_setupDogFish, level_setupBarnArena,
    level_setupTankFactory, level_setupCogLand, level_setupDinoArena,
    level_setupThroneRoom, level_setupHeist, level_setupHarbour,
    level_setupLiftTunnel, level_setupFrontEnd, level_setupWarboss,
    level_setupPooCabin, level_setupCanyon, level_setup21,
    level_setupMachineGunNest, level_setupLab, level_setupW1VolcanoRace,
    level_setupWmpVolcanoRace, level_setupMainWar, level_emptySetupRockSolid,
    level_setup28, level_setup29, level_setupPooBoss,
    level_setupBankEntrance, level_setupUBend, level_setupW3HauntedHouse,
    level_setupBrontoThroat, level_setupHauntedOuthouse, level_setup36,
    level_setupHorrorIntro,
};

/* Resets game globals, then runs the subsystem init routines for the given level. */
void game_initLevelSubsystems(s32 levelId) {
    gObjectWalkDepth = -1;
    bzero(gObjectWalkNext, 24);
    D_800DD404 = 0xFF;
    gLevelTexFrame = 0;
    gLevelTexFrameTime = 0;
    gBulletHitTest = 0;
    gBulletImpactHook = NULL;
    gProjectileHitCallback = 0;
    D_8008CD00 = 0;
    gLineEmitterStarted = 0;
    gBounceRecords = NULL;
    {
        // matching: IDO schedules the store differently without the block-scoped temp
        u8 result = level_getEffectKind();

        gLevelEffectKind = result;
    }
    view_buildOrthoMatrices();

    osViSetSpecialFeatures(64);
    env_initFog(levelId);
    aim_loadPoints(levelId);
    env_initVtxColours();

    flamethrower_clearTexScrolls();
    func_15008BE0();
    effect_initQueue();
    level_resetSetupState();

    if (gLevelHeader->setupCallback) {
        gLevelSetupFuncs[gLevelHeader->setupCallback]();
    }
    sprite_initParticleSlots(levelId);
    sprite_buildLevelParticles();
    sprite_loadParticleTextures();
    shadow_initLevelSettings(levelId);
    effect_resetRain();
    level_resetScreenCopy();

    light_setDaylight(!(gTaskDoneBits[0x4] & 0x80));
}
