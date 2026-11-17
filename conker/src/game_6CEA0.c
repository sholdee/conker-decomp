#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_1503F9F0(s16 arg0, s16 *arg1, s16 *arg2) {
    static s16 D_80084484;
    s8 *p;

    p = (*(s8 ***)&D_800BE728)[arg0];
    if (--D_80084484 != 0) {
        *arg1 = 0;
        *arg2 = 0;
        return;
    }
    arg0 = p[2];
    D_80084484 = 5;
    if (arg0 < -20 || arg0 > 20) {
        *arg1 = arg0;
    } else {
        *arg1 = 0;
    }
    if (p[3] < -20 || p[3] > 20) {
        *arg2 = p[3];
    } else {
        *arg2 = 0;
    }
    p[2] = 0;
    p[3] = 0;
    *arg1 = (*arg1 < -1) ? -1 : (*arg1 > 1) ? 1 : *arg1;
    *arg2 = (*arg2 < -1) ? -1 : (*arg2 > 1) ? 1 : *arg2;
}

extern s32 D_80084488;
extern f32 D_800C6850;
extern s32 D_8008448C;
extern u8 D_80084490;
extern u8 D_80084494;

void func_1503FB08(void) {
    D_80084488 = 0;
    D_800C6850 = 0.0f;
    D_8008448C = 0;
    D_80084490 = 30;
    D_80084494 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_6CEA0/func_1503FB40.s")
