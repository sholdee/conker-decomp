#include <ultra64.h>

#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/hud.h"
#include "conker/math.h"
#include "conker/player.h"
#include "conker/rom.h"


void player_initMatch(s16 arg0, s16 arg1) {
    s32 i;
    s16 tmp;

    tmp = arg0;
    bzero(&gMatchState, 524);
    gMatchState.mode = tmp;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & arg1) {
            player_setStat(i, 0, D_80087270[i]);
            gHudBlockTarget = gHudBlockCount = 6;
            player_setStat(i, 5, gDefaultHealth);
            player_setStat(i, 2, D_80087260);
            if (gIsMultiplayer != 0) {
                player_setStat(i, 9, D_80087264);
            }
        }
        func_1501748C(arg1);
    }
    gMatchState.playerMask = arg1;
    gCarriedUnderwaterTime = 0;
}

void func_1501748C(s16 arg0) {
}

void player_clearMatch(void) {
    bzero(&gMatchState, 524);
}

void player_offsetLinkGroupObjects(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < (s32)gLinkGroupCount; i++) {
        for (j = 0; j < gLinkGroups[i].count; j++) {
            if ((gLinkGroups[i].members[j] >> 12) == 2) {
                gLinkGroups[i].members[j] += arg0;
            }
        }
    }
}

void player_loadLinkGroups(s32 arg0) {
    u32 tmp = 0;
    gLinkGroups = rom_loadPath(&tmp, 3, 12, arg0, 4);
    gLinkGroupCount = tmp / 24;
    player_offsetLinkGroupObjects(gUngroupedObjectCount);
}
