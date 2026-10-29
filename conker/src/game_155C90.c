#include <ultra64.h>
#include "functions.h"
#include "variables.h"



typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Entry151287E0;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x104];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ u8 pad138[0x104];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D[0x153];
    /* 0x390 */ f32 unk390;
    /* 0x394 */ u8 pad394[0x14];
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x24];
    /* 0x3D0 */ u8 *unk3D0;
    /* 0x3D4 */ u8 *unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x210];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ u8 pad5EC[0x70];
    /* 0x65C */ f32 unk65C;
    /* 0x660 */ f32 unk660;
    /* 0x664 */ u8 pad664[0x64];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ u8 pad6CC[0x30];
    /* 0x6FC */ s32 unk6FC;
    /* 0x700 */ u8 pad700[0x3C];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
} Obj151287E0;

extern Entry151287E0 D_800894F0[];
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

void func_151287E0(Obj151287E0 *arg0, f32 *arg1, f32 *arg2) {
    Entry151287E0 *entry;
    s32 saved;
    f32 e0;
    f32 e1;
    f32 e2;
    f32 e3;
    f32 e4;
    f32 angle;
    f32 sign;
    f32 speed;
    f32 res;
    s32 flag;

    saved = arg0->unk134;
    if (arg0->unk2C & 0x40) {
        arg0->unk134 = 0;
    }
    if ((D_800D2DB4 != 0) || ((arg0->unk73C != 0) && (arg0->unk73C != 3)) || (arg0->unk3D4[0x120] != 0)) {
        func_150495B0(&arg0->unk5E8, 0.0f, &arg0->unk660, 6.0f, 9.0f, arg0->unk7B4);
        func_150495B0(&arg0->unk3A8, 0.0f, &arg0->unk65C, 1.5f, 2.5f, arg0->unk7B4);
        return;
    }
    e0 = D_800894F0[arg0->unk134].unk0;
    e1 = D_800894F0[arg0->unk134].unk4;
    e2 = D_800894F0[arg0->unk134].unk8;
    e3 = D_800894F0[arg0->unk134].unkC;
    e4 = D_800894F0[arg0->unk134].unk10;
    flag = arg0->unk23C;
    if ((flag != 0) || (e0 == e2)) {
        speed = 0.0f;
    } else {
        speed = *(f32 *)(arg0->unk3D0 + 0x3C);
    }
    if (speed > 30.0f) {
        speed = 30.0f;
    }
    angle = arg0->unk390;
    if ((arg0->unk6C8 != 0) && ((arg0->unk6FC == 10) || (arg0->unk6FC == 14))) {
        angle = fabsf(arg0->unk390);
    }
    while (angle > 180.0f) {
        angle = 360.0f - angle;
    }
    if (angle < 90.0f) {
        angle = -180.0f + angle;
    }
    if (angle < 0.0f) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    angle = angle - 90.0f * sign;
    res = angle * 0.01111111138f * e0 - angle * speed * 0.0003703703696f * e2 + e4;
    if (arg0->unk2C & 0x100) {
        res = 0.0f;
    }
    if (arg2 != NULL) {
        *arg2 = res;
    } else if (flag != 0) {
        arg0->unk3A8 = res;
        arg0->unk65C = 0.0f;
    } else {
        func_150495B0(&arg0->unk3A8, res, &arg0->unk65C, 1.5f, 2.5f, arg0->unk7B4);
    }
    res = arg0->unk390;
    while (res > 180.0f) {
        res -= 360.0f;
    }
    if (res > 90.0f) {
        res = 180.0f - res;
    } else if (res < -90.0f) {
        res = -180.0f - res;
    }
    if (arg1 != NULL) {
        *arg1 = -(res * 0.01111111138f * e1) - res * speed * 0.0003703703696f * e3;
    } else if (arg0->unk23C != 0) {
        arg0->unk5E8 = -(res * 0.01111111138f * e1) - res * speed * 0.0003703703696f * e3;
        arg0->unk660 = 0.0f;
    } else if (arg0->unk2C & 0x100) {
        func_150495B0(&arg0->unk5E8, -(res * 0.01111111138f * e1) - res * speed * 0.0003703703696f * e3, &arg0->unk660, 6.0f, 9.0f, arg0->unk7B4);
    } else {
        func_150495B0(&arg0->unk5E8, -(res * 0.01111111138f * e1) - res * speed * 0.0003703703696f * e3, &arg0->unk660, 1.5f, 2.5f, arg0->unk7B4);
    }
    arg0->unk134 = saved;
}
