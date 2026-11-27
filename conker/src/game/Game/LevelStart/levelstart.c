#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void game_startMatch(s16 arg0, s16 arg1, s16 arg2) {

    gLevelLoadState = 2;
    debug_resetTaskMenu();
    level = gNextLevel = arg2;

    player_resetWarps(gPlayerSpawnPoints[0]);
    game_clearPendingCutscene();
    zone_resetObjectCursor();
    player_initMatch(arg0, arg1);

    view_setPlayerCount(gModePlayerCounts[gMatchState.mode], arg2);
    player_initHudBlockCount();
    zone_resetTaskWorkingSet();
    mp_clearGame();
    func_15000090();
    gRumbleRedetect = 1;
    D_800D18A8 = 0;
    gClearToWhite = 0;
}




void game_loadLevel(void) {
    s32 i;
    s32 resource;
    s32 slot;
    s32 value;
    s32 j;
    s32 count;

    if (gLevelLoadState == 0) {
        gLevelLoadState = 1;
    }
    heap_freeTaggedBlocks();
    collision_loadMapSettings(gNextLevel);
    switch (gNextLevel) {
    case LEVEL_WMP_HEIST:
    case LEVEL_WMP_TANK:
    case LEVEL_WMP_BEACH:
    case LEVEL_WMP_MAIN_WAR:
    case LEVEL_WMP_BEACH_WAR:
    case LEVEL_WMP_VOLCANO_RACE:
    case LEVEL_WMP_TUNNELS:
    case LEVEL_WMP_RAPTOR:
        value = 0x103;
        if (gNextLevel == LEVEL_WMP_TANK) {
            value = 0xF3;
        }
        if (gNextLevel == LEVEL_WMP_VOLCANO_RACE) {
            value -= 8;
        }
        if (gNextLevel == LEVEL_WMP_RAPTOR) {
            value -= 8;
        }
        if (gNextLevel == LEVEL_WMP_TUNNELS) {
            value += 8;
        }
        break;
    case LEVEL_W2_HARBOUR:
        value = 0xEB;
        if (!(gTaskDoneBits[0x11] & 4)) {
            value = D_800B0DF0->unk11 + 0xEB;
        }
        break;
    default:
        value = D_800B0DF0->unk11 + 0xEB;
        break;
    }
    libc_reinitPaging(value);
    gRumbleSuppressed = 0;
    func_1500E738();
    particle_resetModelList();
    gGlowCount = 0;
    gPendingTrailPairTask = NULL;
    effect_clearHomingEffects();
    gReleaseRawBanksPending = 1;
    resource = gPlayerSpawnPoints[0];
    slot = func_1507E114(&resource);
    if (gFrontEndState == 2) {
        resource = D_800E0BD1;
    }
    gPrevLevel = level;
    level = gNextLevel;
    light_reset();
    effect_clearColorAnims();
    gFrameDeltaHistoryNext = 0;
    gFrameDeltaHistoryCount = 0;
    gFrameRateDivider = 0;
    game_clearLevelPointers();
    if (gClearToWhite != 0) {
        boot_clearFramebuffers(0xFF, 0xFF, 0xFF);
        gClearToWhite = 0;
    } else {
        boot_clearFramebuffers(0, 0, 0);
    }
    if ((gIsMultiplayer == 0) && !(gTaskDoneBits[0x18] & 1)) {
        gHudBlockTarget = gHudBlockCount = 3;
        player_setStat(0, 5, 3);
    }
    collision_setFloorReach(level);
    cont_stopAllRumble();
    gfx_allocDisplayLists();
    actor_clearAll();
    zone_loadObjects(level);
    text_resetContextText();
    view_allocViews(gLastPlayerIndex, 1);
    env_clearAttachModelRefs();
    screen_resetEffects();
    collision_clearUvWobbles();
    level_resetWaveDeformers();
    light_resetParticles();
    func_15044370();
    player_clearIdleRequests();
    model_resetAttachments();
    model_allocBonePools();
    zone_reloadSectionTasks();
    env_initFog(level);
    func_1502AAF0();
    func_1502AAF8(level);
    level_loadModels(level);
    level_spawnObjects(level, 1);
    func_15004574();
    prop_clearLinks();
    level_buildCollisionTris(level);
    level_buildCollisionQuadTrees();
    func_150039B0(level);
    player_loadLinkGroups(level);
    level_prepareObjects(level);
    game_resetCutscenes(level);
    music_startLevelMusic();
    sound_initLevel(level);
    mp3_stopForLevel(level);
    audio_clearMixSelectors();
    cont_startReadTimer();
    effect_clearObjectLists(1);
    effect_runResetCallbacks(1);
    player_loadSpawnPoints(level);
    level_loadSpawnTable(level);
    camera_initViews(level, resource);
    actor_setDefaultSizes();
    music_initLevelViewState(level);
    view_initDefaultProjection(0);
    for (i = 0; i <= gScreenLayout; i++) {
        camera_raiseSnapTimer(&camera_global_Cameras[i], 1);
    }
    func_1500E70C(level);
    collision_loadCylinderSets(level);
    light_applyZones();
    collision_initMapBackground(level);
    if (level == LEVEL_FRONTEND) {
        save_clearMenuCharacters();
    }
    game_initLevelSubsystems(level);
    flare_resetFlares(level);
    path_load(level);
    anim_clearMorphCache();
    light_updatePlayerLight(0);
    face_resetScripted();
    gFrameTicks = 2;
    func_15015910();
    model_resetBoneChains();
    func_1501748C(gMatchState.playerMask);
    swarm_resetModelSlots();
    model_resetParts();
    model_allocActorQuadVtx();
    game_startPendingCutscene();
    if (gIsMultiplayer == 0) {
        player_setStat(0, 9, D_800BE3E4);
    }
    if (script_canAutoSave() != 0) {
        gCheckpointSpawn = resource & 0xFF;
    }
    gAutoSavePending = 1;
    script_runTasks(4);
    script_runTasks(1);
    level_spawnZones();
    if (slot != 0) {
        func_1507DFE4(resource, slot - 1);
    }
    func_151E562C();
    if ((D_800E0B99 != 0) || (D_800DDE38 == 2)) {
        s32 enabled = 1;

        if ((gCutsceneStates[0] == 1) || (D_800DDE38 == 1)) {
            enabled = 0;
        }
        if (enabled) {
            count = save_getScreenCount();
            for (j = 0; j < count; j++) {
                screen_startOpening(j);
            }
        }
        D_800E0B99 = 0;
    }
    player_resetHudBlocks();
    cutscene_loadMicrocodes();
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (movement_obj[i].playernumber != 0xFF)) {
            zone_updateActor(&movement_obj[i]);
        }
    }
    if ((D_800D18A8 != 0) && (level != LEVEL_WFRONTEND_HELL)) {
        D_800D18A8 = 0;
        effect_showStatCounter(0xFF, 1);
    }
    script_autoSave();
    gNextLevel = level;
    cont_killAllRumble();
}

void game_clearLevelPointers(void) {
    s32 i;

    gShadowFaceFlags = NULL;
    gTriggerNodes = NULL;
    gLensFlares = NULL;

    for (i = 0; i < 2; i++)
    {
        gBoneTrailLists[i] = NULL;
    }

    gHudBlockDebris = 0;
    gHudWaterStage.dl = NULL;
}


void game_clearPendingCutscene(void) {
    gPendingCutscene = 0;
    gCutsceneSkipState = 0;
}

void game_resetCutscenes(s32 arg0) {
    s32 i;

    gCutsceneActorLock = 0;
    gSpawnCutsceneActive = 0;
    gCutsceneIgnorePlayed = 0;
    cutscene_resetScene(0);
    cutscene_resetScene(1);
    gCutsceneAnchorSet = 0;

    for (i = 0; i < 3; i++)
    {
        gSubtitles[i].active = 0;
        gSubtitles[i].isOpen = 0;
    }

    gCutsceneHideSky = 0;
    gCutsceneSolidSky = 0;
    gCutsceneNoPause = 0;
}

void game_startPendingCutscene(void) {
    s32 tmp = gPendingCutscene - 1;

    if (gPendingCutscene != 0) {
        if (D_800C35C5 == 0) {
            gCutsceneStates[0] = 2;
        }
        gPendingCutscene = 0;
        cutscene_start(level, tmp, 0, gCutsceneCallback, gCutsceneCallbackArg);
    }
}
