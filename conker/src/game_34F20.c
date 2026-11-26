#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15007A70(s16 arg0, s16 arg1, s16 arg2) {

    D_800BE9B4 = 2;
    func_1503FB08();
    level = gNextLevel = arg2;

    player_resetWarps(gPlayerSpawnPoints[0]);
    func_15008230();
    func_15005270();
    player_initMatch(arg0, arg1);

    func_15015F40(gModePlayerCounts[gMatchState.mode], arg2);
    func_150175E0();
    func_150177F8();
    func_15089BB0();
    func_15000090();
    D_800BE938 = 1;
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
    func_10004308();
    func_15002FB4(gNextLevel);
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
        if (!(D_800D2E4C[0x11] & 4)) {
            value = D_800B0DF0->unk11 + 0xEB;
        }
        break;
    default:
        value = D_800B0DF0->unk11 + 0xEB;
        break;
    }
    func_100014C4(value);
    D_800E0A00 = 0;
    func_1500E738();
    func_1500C2A0();
    D_800DC9F0 = 0;
    D_800E0920 = NULL;
    func_15008840();
    D_8003E384 = 1;
    resource = gPlayerSpawnPoints[0];
    slot = func_1507E114(&resource);
    if (D_800E0B94 == 2) {
        resource = D_800E0BD1;
    }
    gPrevLevel = level;
    level = gNextLevel;
    func_150091D0();
    func_15187F90();
    gFrameDeltaHistoryNext = 0;
    gFrameDeltaHistoryCount = 0;
    gFrameRateDivider = 0;
    func_150081E4();
    if (D_800BE9ED != 0) {
        func_10003ACC(0xFF, 0xFF, 0xFF);
        D_800BE9ED = 0;
    } else {
        func_10003ACC(0, 0, 0);
    }
    if ((gIsMultiplayer == 0) && !(D_800D2E4C[0x18] & 1)) {
        D_800D2457 = D_800D2456 = 3;
        player_setStat(0, 5, 3);
    }
    func_15002F40(level);
    func_1501C53C();
    func_15015E80();
    actor_clearAll();
    func_15017930(level);
    text_resetContextText();
    func_1501A220(D_800BE9E8, 1);
    func_15012F90();
    func_15008A60();
    func_15000AC0();
    func_15012780();
    func_1500BE68();
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
    func_1511F980();
    func_15003668(level);
    func_150038A0();
    func_150039B0(level);
    func_15017578(level);
    func_150045C4(level);
    func_15008248(level);
    music_startLevelMusic();
    sound_initLevel(level);
    func_100125CC(level);
    func_10011FEC();
    func_1501BB20();
    func_15008870(1);
    func_15008930(1);
    player_loadSpawnPoints(level);
    func_150169A0(level);
    func_15005290(level, resource);
    actor_setDefaultSizes();
    func_150006E0(level);
    func_1510B070(0);
    for (i = 0; i <= D_80082FA0; i++) {
        func_15125690(&camera_global_Cameras[i], 1);
    }
    func_1500E70C(level);
    func_150163D0(level);
    func_150092DC();
    func_15000AD0(level);
    if (level == 0x1D) {
        func_151E81EC();
    }
    game_initLevelSubsystems(level);
    func_15009150(level);
    func_15085B70(level);
    func_15008DD0();
    func_1501B640(0);
    func_15080200();
    D_800BE9E4 = 2;
    func_15015910();
    func_15004F10();
    func_1501748C(gMatchState.playerMask);
    func_1500C1E0();
    model_resetParts();
    func_15004D50();
    func_150082CC();
    if (gIsMultiplayer == 0) {
        player_setStat(0, 9, D_800BE3E4);
    }
    if (func_1509C2A4() != 0) {
        D_800BE3E0 = resource & 0xFF;
    }
    D_800D2E43 = 1;
    func_1509BBA0(4);
    func_1509BBA0(1);
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
                func_15181E18(j);
            }
        }
        D_800E0B99 = 0;
    }
    func_15017640();
    func_1501CC3C();
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (movement_obj[i].playernumber != 0xFF)) {
            func_150A11C4(&movement_obj[i]);
        }
    }
    if ((D_800D18A8 != 0) && (level != 0x22)) {
        D_800D18A8 = 0;
        func_151BF340(0xFF, 1);
    }
    func_1509C3A0();
    gNextLevel = level;
    func_151D8DE8();
}

void func_150081E4(void) {
    s32 i;

    D_800D3668 = NULL;
    D_800CBE00 = NULL;
    D_800DDD64 = NULL;

    for (i = 0; i < 2; i++)
    {
        D_800DF7C8[i] = NULL;
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
