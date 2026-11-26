#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15012370(void) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f24;
    f32 one;
    f32 half;
    s32 i;

    i = 0;
    if (D_80082FA0 >= 0) {
        temp_f24 = D_80096560;
        one = 1.0f;
        half = 0.5f;
        do {
            func_151EF954(
                &D_800DCC10[i],
                -(temp_f0 = D_800BE628[i].width * half),
                temp_f0,
                -(temp_f2 = D_800BE628[i].height * half),
                temp_f2,
                one,
                temp_f24,
                one);
            i = (i + 1) & 0xFF;
        } while (D_80082FA0 >= i);
    }
}
// void func_15012370(void) {
//     f32 temp_f0;
//     f32 temp_f2;
//     u8 i;
//
//     f32 foo = D_80096560; // 20000.0f
//     f32 **tmp = D_800BE628;
//
//     for (i = 0; i <= D_80082FA0; i++) {
//         temp_f0 = tmp[i][1] * 0.5f;
//         temp_f2 = tmp[i][2] * 0.5f;
//         func_151EF954(D_800DCC10[i], -temp_f0, temp_f0, -temp_f2, temp_f2, 1.0f, foo, 1.0f);
//     }
// }

void func_15012470(void) {
    D_80088750 = func_1518AADC(4, 300, 0);
}

void *memcpy(void *, const void *, u32);

void func_150124A0(void) {
    s32 row;
    s16 yaw;
    s16 pitch;
    f32 yawCos;
    f32 yawSin;
    void *result;
    Vec3f hit;
    f32 pitchCos;
    f32 radius;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    s16 timer[3];

    for (row = 0; row < 3; row++) {
        s32 i;

        for (i = 0; i < 10; i++) {
            yaw = D_8009F920[row].yaw + func_150ADA20() % 13U - 0x86;
            pitch = (func_150ADA20() & 0xF) - 10;
            yawCos = func_151423D8(yaw - 0x40);
            yawSin = func_151423D8(yaw);
            pitchCos = func_151423D8(pitch - 0x40);
            radius = func_151423D8(pitch);
            radius = 10.0f * radius;
            dirX = radius * yawCos;
            dirY = -10.0f * pitchCos;
            dirZ = radius * yawSin;
            if (func_150AC9C0(0.0f, 300.0f, 0.0f, dirX, dirY, dirZ, NULL, NULL,
                              &hit.x, &hit.y, &hit.z, NULL, NULL, NULL, 0.0f) == 0) {
                D_800DCA30[row][i].pos.x = 0.0f;
                D_800DCA30[row][i].pos.y = 0.0f;
                D_800DCA30[row][i].pos.z = 0.0f;
            } else {
                D_800DCA30[row][i].pos.x = hit.x * 0.97f;
                D_800DCA30[row][i].pos.y = (hit.y - 300.0f) * 0.97f + 300.0f;
                D_800DCA30[row][i].pos.z = hit.z * 0.97f;
            }
        }
    }
    timer[0] = 0x14;
    timer[1] = 0x32;
    timer[2] = 0;
    result = func_151491F4(0x12C, -1, 7, 0, 0, 6, 0xFF, 0);
    if (result != NULL) {
        memcpy((u8 *)result + 0x28, timer, 6);
    }
}
