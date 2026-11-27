#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Resets this module's state: runs three sub-resets, then clears its flags, tables and pointers. */
void level_resetSetupState(void) {
    s32 i;
    s32 j;

    view_initGroundDecalQueue();
    screen_resetKickableTargets();
    func_15012770();
    for (i = 0; i < 20; i++) {
        gAnchorRefCounts[i] = 0;
    }
    D_800D9921 = 0;
    D_800D9920 = 0;
    for (j = 0; j < 14; j++) {
        D_800D9928[j] = 0;
        D_800D9938[j] = 0;
    }
    {
        // matching: IDO materializes &gLevelSprayEffects before the two scalar stores only via a local
        EffectHeader **entries = gLevelSprayEffects;

        gSpriteQueuedCmd = 0;
        gSpriteTintEffect = NULL;
        entries[0] = NULL;
        entries[1] = NULL;
        entries[2] = NULL;
        entries[3] = NULL;
    }
    gDriftBurstEmitter = NULL;
    bzero(gFlagLightStates, sizeof(gFlagLightStates));
    gMaterialGlowLevels[0] = gMaterialGlowLevels[1] = gMaterialGlowLevels[2] = 0;
    gWarpModelSetupDone = 0;
    gSprayAreas[0] = NULL;
    gSprayAreas[1] = NULL;
    gSprayAreas[2] = NULL;
    gSetup33TexScrolls = NULL;
    gZoneFlag = 0;
}

/* Called with level; id 43 also runs level_setup28. */
void func_1500E70C(s32 id) {
    if (id == 43) {
        level_setup28();
    }
}

/* Sets two (shift, scale = 1 / 2^shift) pairs: D_800DCA20/24 from gScreenLayout, D_800DCA28/2C from gPlayerCount. */
void func_1500E738(void) {
    switch (gScreenLayout) {
    case 0:
        D_800DCA20 = 0;
        D_800DCA24 = 1.0f;
        break;
    case 1:
        D_800DCA20 = 1;
        D_800DCA24 = 0.5f;
        break;
    case 2:
    case 3:
        D_800DCA20 = 2;
        D_800DCA24 = 0.25f;
        break;
    }
    switch (gPlayerCount) {
    case 2:
        D_800DCA28 = 1;
        D_800DCA2C = 0.5f;
        return;
    case 3:
    case 4:
        D_800DCA28 = 2;
        D_800DCA2C = 0.25f;
        return;
    case 5:
    case 6:
    case 7:
    case 8:
        D_800DCA28 = 3;
        D_800DCA2C = 0.125f;
        return;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        D_800DCA28 = 4;
        D_800DCA2C = 0.0625f;
        return;
    case 1:
    default:
        D_800DCA28 = 0;
        D_800DCA2C = 1.0f;
        return;
    }
}

void level_setup21(void) {
    prop_clearWaveChains();
    prop_startWaveChain(0);
    prop_startWaveChain(1);
}

/* Starts two flamethrower_addTexScroll entries, spawns one trail_createSprayEmitter emitter and calls sound_addDynamicSound. */
void level_setup01(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902E8, 0, -1, 0, 0, 0, -6);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902E8, 0, -1, 0, 1, 0, -6);
    {
        SprayDesc descriptor;

        descriptor.rate = 0.2142857164f;
        descriptor.halfWidth = 5.0f;
        descriptor.halfWidthRange = 6.0f;
        descriptor.speed = 8.0f;
        descriptor.speedRange = 7.0f;
        descriptor.rateRange = 0.0f;
        descriptor.pointCount = 3;
        descriptor.pointCountRange = 2;
        descriptor.yaw = 0x34;
        descriptor.yawSpread = 0x12;
        descriptor.pitch = -0x28;
        descriptor.pitchRange = 0xF;
        descriptor.unk1B = 155;
        descriptor.unk1BRange = 100;
        descriptor.unkC = 41;
        descriptor.unkCRange = 41;
        descriptor.origin.z = 400.0f;
        descriptor.extent.x = 0.0f;
        descriptor.extent.y = 0.0f;
        descriptor.gravity = 0.6000000238f;
        descriptor.gravityRange = 0.3999999762f;
        descriptor.origin.x = 800.0f;
        descriptor.origin.y = -3000.0f;
        descriptor.extent.z = 900.0f - descriptor.origin.z;
        trail_createSprayEmitter(&descriptor, 1);
    }
    sound_addDynamicSound(1567, (level == 6) ? 52 : 7, 0, 0, 12000, 1000, 400, sound_stopWhenSilentCallback, 0, 0, 72, 0);
}

void level_setup36(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_80090320, 0, -1, 0, 0, 0, -8);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_80090320, 0, -1, 0, 1, 0, -8);
}
