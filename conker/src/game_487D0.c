#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800BE6C0[];
extern f32 D_800BE6D0[];

void func_1501B320(s32 arg0) {
    f32 x;
    f32 z;
    f32 ang;
    f32 rate;
    f32 yaw;
    f32 y;
    f32 c;
    f32 diff;
    f32 pitch;

    if (D_800BEAC0 == 0) {
        if (D_800DBFF0[arg0].unk698 != 0) {
            rate = 0.1000000015f;
        } else if (*(s32 *)D_800DBFF4 != 0) {
            rate = 1.0f;
        } else {
            rate = 0.02999999933f;
        }
        yaw = D_800DBFF0[arg0].unk388;
        pitch = D_800DBFF0[arg0].unk380;
        if (yaw < 0.0f) {
            yaw += 360.0f;
        } else if (yaw >= 360.0f) {
            yaw -= 360.0f;
        }
        if (yaw > 180.0f) {
            yaw -= 360.0f;
        }
        diff = yaw - D_800BE6C0[arg0];
        D_800BE6C0[arg0] += diff * rate;
        diff = func_15048A70(D_800BE6D0[arg0], pitch);
        D_800BE6D0[arg0] += rate * diff;
        if (D_800BE6D0[arg0] < 0.0f) {
            D_800BE6D0[arg0] += 360.0f;
        } else if (D_800BE6D0[arg0] >= 360.0f) {
            D_800BE6D0[arg0] -= 360.0f;
        }
        diff = func_15048A70(D_800BE6D0[arg0], pitch);
        if (fabsf(diff) > 90.0f) {
            D_800BE6D0[arg0] = pitch - ((diff < 0) ? -1 : 1) * 90.0f;
        }
        if (D_800BE6D0[arg0] < 0.0f) {
            D_800BE6D0[arg0] += 360.0f;
        } else if (D_800BE6D0[arg0] >= 360.0f) {
            D_800BE6D0[arg0] -= 360.0f;
        }
        ang = D_800BE6C0[arg0] * 0.01745329238f;
        y = -sinf(ang);
        c = cosf(ang);
        ang = D_800BE6D0[arg0] * 0.01745329238f;
        x = sinf(ang) * c;
        c = cosf(ang) * c;
        ((s8 (*)[3])&D_800DCD30)[arg0][0] = x * 127.0f;
        ((s8 (*)[3])&D_800DCD30)[arg0][1] = y * 127.0f;
        ((s8 (*)[3])&D_800DCD30)[arg0][2] = c * 127.0f;
    }
}

void func_1501B640(s32 arg0) {
    func_1501B320(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_487D0/func_1501B660.s")
