#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15007A70(s16 arg0, s16 arg1, s16 arg2) {

    D_800BE9B4 = 2;
    debug_resetTaskMenu();
    level = gNextLevel = arg2;

    player_resetWarps(gPlayerSpawnPoints[0]);
    func_15008230();
    zone_resetObjectCursor();
    player_initMatch(arg0, arg1);

    view_setPlayerCount(gModePlayerCounts[gMatchState.mode], arg2);
    func_150175E0();
    func_150177F8();
    mp_clearGame();
    func_15000090();
    gRumbleRedetect = 1;
    D_800D18A8 = 0;
    D_800BE9ED = 0;
}




void game_loadLevel(void) {
    s32 i;
    s32 resource;
    s32 slot;
    s32 value;
    s32 j;
    s32 count;

    if (D_800BE9B4 == 0) {
        D_800BE9B4 = 1;
    }
    heap_freeTaggedBlocks();
    collision_loadMapSettings(gNextLevel);
    switch (gNextLevel) {
    case 0x1A:
    case 0x24:
    case 0x2B:
    case 0x2D:
    case 0x30:
    case 0x33:
    case 0x34:
    case 0x3F:
        value = 0x103;
        if (gNextLevel == 0x24) {
            value = 0xF3;
        }
        if (gNextLevel == 0x33) {
            value -= 8;
        }
        if (gNextLevel == 0x3F) {
            value -= 8;
        }
        if (gNextLevel == 0x34) {
            value += 8;
        }
        break;
    case 0x13:
        value = 0xEB;
        if (!(gTaskDoneBits[0x11] & 4)) {
            value = D_800B0DF0->unk11 + 0xEB;
        }
        break;
    default:
        value = D_800B0DF0->unk11 + 0xEB;
        break;
    }
    func_100014C4(value);
    gRumbleSuppressed = 0;
    func_1500E738();
    particle_resetModelList();
    D_800DC9F0 = 0;
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
    func_150081E4();
    if (D_800BE9ED != 0) {
        boot_clearFramebuffers(0xFF, 0xFF, 0xFF);
        D_800BE9ED = 0;
    } else {
        boot_clearFramebuffers(0, 0, 0);
    }
    if ((gIsMultiplayer == 0) && !(gTaskDoneBits[0x18] & 1)) {
        D_800D2457 = D_800D2456 = 3;
        player_setStat(0, 5, 3);
    }
    collision_setFloorReach(level);
    cont_stopAllRumble();
    gfx_allocDisplayLists();
    actor_clearAll();
    zone_loadObjects(level);
    text_resetContextText();
    view_allocViews(D_800BE9E8, 1);
    func_15012F90();
    screen_resetEffects();
    collision_clearUvWobbles();
    func_15012780();
    light_resetParticles();
    func_15044370();
    func_15016670();
    func_15004F00();
    model_allocBonePools();
    func_15017868();
    func_151732E0(level);
    func_1502AAF0();
    func_1502AAF8(level);
    func_150031EC(level);
    func_150039E0(level, 1);
    func_15004574();
    prop_clearLinks();
    func_15003668(level);
    func_150038A0();
    func_150039B0(level);
    func_15017578(level);
    func_150045C4(level);
    func_15008248(level);
    music_startLevelMusic();
    sound_initLevel(level);
    mp3_stopForLevel(level);
    audio_clearMixSelectors();
    cont_startReadTimer();
    effect_clearObjectLists(1);
    effect_runResetCallbacks(1);
    player_loadSpawnPoints(level);
    func_150169A0(level);
    camera_initViews(level, resource);
    actor_setDefaultSizes();
    func_150006E0(level);
    view_initDefaultProjection(0);
    for (i = 0; i <= gScreenLayout; i++) {
        func_15125690(&camera_global_Cameras[i], 1);
    }
    func_1500E70C(level);
    collision_loadCylinderSets(level);
    light_applyZones();
    collision_initMapBackground(level);
    if (level == 0x1D) {
        save_clearMenuCharacters();
    }
    game_initLevelSubsystems(level);
    func_15009150(level);
    func_15085B70(level);
    anim_clearMorphCache();
    func_1501B640(0);
    func_15080200();
    D_800BE9E4 = 2;
    func_15015910();
    func_15004F10();
    func_1501748C(gMatchState.playerMask);
    swarm_resetModelSlots();
    model_resetParts();
    func_15004D50();
    func_150082CC();
    if (gIsMultiplayer == 0) {
        player_setStat(0, 9, D_800BE3E4);
    }
    if (script_canAutoSave() != 0) {
        D_800BE3E0 = resource & 0xFF;
    }
    D_800D2E43 = 1;
    script_runTasks(4);
    script_runTasks(1);
    func_15013000();
    if (slot != 0) {
        func_1507DFE4(resource, slot - 1);
    }
    func_151E562C();
    if ((D_800E0B99 != 0) || (D_800DDE38 == 2)) {
        s32 enabled = 1;

        if ((D_800C35EA[0] == 1) || (D_800DDE38 == 1)) {
            enabled = 0;
        }
        if (enabled) {
            count = func_151E5FAC();
            for (j = 0; j < count; j++) {
                screen_startOpening(j);
            }
        }
        D_800E0B99 = 0;
    }
    func_15017640();
    func_1501CC3C();
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (movement_obj[i].playernumber != 0xFF)) {
            zone_updateActor(&movement_obj[i]);
        }
    }
    if ((D_800D18A8 != 0) && (level != 0x22)) {
        D_800D18A8 = 0;
        effect_showStatCounter(0xFF, 1);
    }
    script_autoSave();
    gNextLevel = level;
    cont_killAllRumble();
}

void func_150081E4(void) {
    s32 i;

    D_800D3668 = NULL;
    gTriggerNodes = NULL;
    D_800DDD64 = NULL;

    for (i = 0; i < 2; i++)
    {
        gBoneTrailLists[i] = NULL;
    }

    D_800D245C = 0;
    D_800D24C8.dl = NULL;
}


void func_15008230(void) {
    D_800C35C4 = 0;
    D_800C3C88 = 0;
}

void func_15008248(s32 arg0) {
    s32 i;

    D_800C3638 = 0;
    D_800C3671 = 0;
    D_800C3670 = 0;
    func_1501D044(0);
    func_1501D044(1);
    D_800C3663 = 0;

    for (i = 0; i < 3; i++)
    {
        D_800C3CA0[i].active = 0;
        D_800C3CA0[i].isOpen = 0;
    }

    D_800C3681 = 0;
    D_800C3682 = 0;
    D_800C3683 = 0;
}

void func_150082CC(void) {
    s32 tmp = D_800C35C4 - 1;

    if (D_800C35C4 != 0) {
        if (D_800C35C5 == 0) {
            D_800C35EA[0] = 2;
        }
        D_800C35C4 = 0;
        func_1501D348(level, tmp, 0, D_800C3C90, D_800C3C94);
    }
}
