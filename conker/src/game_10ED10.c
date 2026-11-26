#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_80088990[4];
extern f32 D_800889A0[4];
extern f32 D_800889B0[4];
extern s32 D_800889C0[4];
extern s32 D_800889D0[4];

void func_150E1860(Camera *arg0) {
    s32 i;
    s32 flag;

    flag = 0;
    arg0->AvailableModes |= 0x10;
    for (i = 0; i < 4; i++) {
        if (func_1509BE40(1, D_800889C0[i], 6, 0x9000) != 0) {
            D_800889A0[i] = 192.0f;
            arg0->AvailableModes |= 0x80021010;
            flag = 1;
        } else {
            D_800889A0[i] = 255.0f;
        }
        func_150495B0(&D_80088990[i], D_800889A0[i], &D_800889B0[i], 4.0f, 6.0f, arg0->SpringTime);
        func_1509BFB0(1, D_800889D0[i], 0x12, (u8)D_80088990[i]);
    }
    if (flag == 0) {
        arg0->AvailableModes &= ~0x80021000;
    }
    if (func_1509BE40(1, 0x403D, 6, 0x2000) != 0) {
        arg0->AvailableModes &= ~0x80000000;
    } else {
        arg0->AvailableModes |= 0x80000000;
    }
}
