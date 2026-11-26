#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_1500FE30(void) {
    s32 i;

    D_800BE4E0 = allocate_memory(10 * sizeof(BounceRecord), 1, 0, 0);
    bzero(D_800BE4E0, 10 * sizeof(BounceRecord));
    for (i = 0; i < 10; i++) {
        D_800BE4E0[i].table = i / 5;
        D_800BE4E0[i].dir = -1;
        D_800BE4E0[i].pos = func_150ADA20() % 5;
    }
    func_150C851C(100);
    for (i = 0; i < 10; i++) {
        D_800BE4E0[i].speed = D_800BE4E0[i].targetSpeed;
    }
}

void func_1500FF9C(void) {
    func_1510C4AC(D_800917B8, 0, 0xAD, 0x75);
}

extern s32 D_800902DC;
extern u8 *D_80088870;

void func_1500FFCC(void) {
    f32 sp44;
    TimedEffect *fx;

    func_15195AA8(D_800B0E00[0], D_800902DC, 1, -1, 0, 0, 0, -4);
    func_15195AA8(D_800B0E04, D_800902DC, 1, -1, 0, 1, 0, -4);
    func_1500FE30();
    func_1500FF9C();
    sp44 = 0.0f;
    fx = func_151491F4(0x12C, -1, 0x12, 0, 0xE, 4, 0xFF, 0);
    if (fx != NULL) {
        memcpy((u8 *)fx + 0x28, &sp44, 4);
    }
    {
        struct {
            f32 unk0;
            f32 unk4;
        } sp38;
        TimedEffect *fx2;

        sp38.unk0 = 0.0f;
        sp38.unk4 = 0.0f;
        fx2 = func_15149130(0x12C, -1, 0x17, -1, 0, 0x14, 8, 0xFF, 1);
        if ((D_80088870 = (u8 *)fx2) != NULL) {
            memcpy((u8 *)fx2 + 0x28, &sp38, 8);
        }
    }
}
