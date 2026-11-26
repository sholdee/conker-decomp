#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Four channels: the current and target values (255, or 192 while the D_800889C0 query holds), the
 * func_150495B0 spring velocity, the command words queried with func_1509BE40 and those that receive
 * the current value through func_1509BFB0. */
static f32 D_80088990[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 D_800889A0[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 D_800889B0[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static s32 D_800889C0[4] = { 0x4039, 0x403A, 0x403B, 0x403C };
static s32 D_800889D0[4] = { 0x30E4, 0x30E5, 0x30E6, 0x30E7 };

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
