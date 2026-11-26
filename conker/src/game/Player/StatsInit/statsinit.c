#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void player_initMatch(s16 arg0, s16 arg1) {
    s32 i;
    s16 tmp;

    tmp = arg0;
    bzero(&gMatchState, 524);
    gMatchState.mode = tmp;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & arg1) {
            player_setStat(i, 0, D_80087270[i]);
            D_800D2457 = D_800D2456 = 6;
            player_setStat(i, 5, D_8008726C);
            player_setStat(i, 2, D_80087260);
            if (gIsMultiplayer != 0) {
                player_setStat(i, 9, D_80087264);
            }
        }
        func_1501748C(arg1);
    }
    gMatchState.playerMask = arg1;
    D_800D2132 = 0;
}

void func_1501748C(s16 arg0) {
}

void player_clearMatch(void) {
    bzero(&gMatchState, 524);
}

void func_150174C0(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < (s32)D_80087380; i++) {
        for (j = 0; j < D_800D23C0[i].count; j++) {
            if ((D_800D23C0[i].members[j] >> 12) == 2) {
                D_800D23C0[i].members[j] += arg0;
            }
        }
    }
}

void func_15017578(s32 arg0) {
    u32 tmp = 0;
    D_800D23C0 = rom_loadPath(&tmp, 3, 12, arg0, 4);
    D_80087380 = tmp / 24;
    func_150174C0(gUngroupedObjectCount);
}
