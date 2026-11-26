#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Per-level setup routines, indexed by LevelHeader.unkA (0: none). */
static void (*gLevelSetupFuncs[])(void) = {
    NULL,
    func_1500E8C0, func_15010600, func_1500EB20,
    func_1500EBC4, func_1500EF20, func_15010680,
    func_150106A0, func_1500F40C, func_1500ED80,
    func_150106B0, func_1500FA64, func_1500FFCC,
    func_150113E0, func_15011C40, func_150104F0,
    func_15011330, func_15011B94, func_15010780,
    func_15010110, func_15010880, func_1500E890,
    func_150109D0, func_15010A60, func_15010240,
    func_15011CC0, func_15011D40, func_150102C0,
    func_15011C70, func_15011CA0, func_150102D0,
    func_15011360, func_150103E0, func_15010FB0,
    func_1500F210, func_150110F0, func_1500EAA0,
    func_15011170,
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
    D_800BE4E0 = NULL;
    {
        // matching: IDO schedules the store differently without the block-scoped temp
        u8 result = func_15012720();

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

    func_151645C4(!(gTaskDoneBits[0x4] & 0x80));
}
