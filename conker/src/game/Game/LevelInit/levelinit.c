#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Per-level setup routines, indexed by LevelHeader.unkA (0: none). */
static void (*gLevelSetupFuncs[])(void) = {
    NULL,
    level_setup01, level_setup02, level_emptySetup03,
    level_setup04, level_setup05, level_setup06,
    level_emptySetup07, level_setup08, level_setup09,
    level_setup10, level_setup11, level_setup12,
    level_setup13, level_setup14, level_setup15,
    level_setup16, level_setup17, level_setup18,
    level_setup19, level_setup20, level_setup21,
    level_setup22, level_setup23, level_setup24,
    level_setup25, level_setup26, level_emptySetup27,
    level_setup28, level_setup29, level_setup30,
    level_setup31, level_setup32, level_setup33,
    level_setup34, level_setup35, level_setup36,
    level_setup37,
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

    if (D_800B0DF0->unkA) {
        gLevelSetupFuncs[D_800B0DF0->unkA]();
    }
    light_initParticleSlots(levelId);
    light_buildLevelParticles();
    light_loadParticleTextures();
    shadow_initLevelSettings(levelId);
    effect_resetRain();
    level_resetScreenCopy();

    light_setDaylight(!(gTaskDoneBits[0x4] & 0x80));
}
