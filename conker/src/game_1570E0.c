#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_15129C30;

typedef struct {
    u16 buttons;
    s8 stickX;
    s8 stickY;
} Pad_15129C30;

typedef struct {
    /* 0x000 */ u8 pad0[0x23D];
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 pad23E[0x2A4 - 0x23E];
    /* 0x2A4 */ Vec3f_15129C30 unk2A4;
    /* 0x2B0 */ u8 pad2B0[0xC];
    /* 0x2BC */ Vec3f_15129C30 unk2BC;
    /* 0x2C8 */ u8 pad2C8[0x2F8 - 0x2C8];
    /* 0x2F8 */ f32 unk2F8;
    /* 0x2FC */ f32 unk2FC;
    /* 0x300 */ f32 unk300;
    /* 0x304 */ u8 pad304[0x344 - 0x304];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ u8 pad348[0x360 - 0x348];
    /* 0x360 */ f32 unk360;
    /* 0x364 */ u8 pad364[0x36A - 0x364];
    /* 0x36A */ u16 unk36A;
    /* 0x36C */ Pad_15129C30 *unk36C;
    /* 0x370 */ u8 pad370[0x37C - 0x370];
    /* 0x37C */ f32 unk37C;
    /* 0x380 */ f32 unk380;
    /* 0x384 */ u8 pad384[4];
    /* 0x388 */ f32 unk388;
    /* 0x38C */ u8 pad38C[4];
    /* 0x390 */ f32 unk390;
    /* 0x394 */ f32 unk394;
    /* 0x398 */ f32 unk398;
    /* 0x39C */ f32 unk39C;
    /* 0x3A0 */ f32 unk3A0;
    /* 0x3A4 */ u8 pad3A4[0x5EC - 0x3A4];
    /* 0x5EC */ f32 unk5EC;
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x640 - 0x5F4];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
} Cam_15129C30;

extern f32 D_80089560[];
extern f32 D_80089570[];
extern f32 D_80089580[];
void func_1510E7A4(void *a0, void *a1, void *a2, void *a3, void *arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, u16 arg10, s32 arg11, f32 arg12, f32 arg13);

void func_15129C30(Cam_15129C30 *arg0) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 fwd;
    f32 strafe;
    f32 sx;
    f32 sy;
    Vec3f_15129C30 sp68;
    u8 idx;
    u16 buttons;
    f32 f;

    strafe = 0.0f;
    idx = arg0->unk23D;
    buttons = arg0->unk36C->buttons;
    if (arg0->unk36A & 0x400) {
        if (arg0->unk5F0 & 0x80000000) {
            arg0->unk5F0 &= ~0x80000000;
        } else {
            arg0->unk5F0 |= 0x80000000;
        }
    }
    if (!(arg0->unk5F0 & 0x80000000)) {
        if ((arg0->unk36C->stickX < -4) || (arg0->unk36C->stickX >= 5)) {
            sx = arg0->unk36C->stickX;
        } else {
            sx = 0.0f;
        }
        if ((arg0->unk36C->stickY < -4) || (arg0->unk36C->stickY >= 5)) {
            sy = arg0->unk36C->stickY;
        } else {
            sy = 0.0f;
        }
        f = sx / 80.0f;
        if (f > 0.0f) {
            arg0->unk37C -= f * f * D_800BEA08;
        } else {
            arg0->unk37C += f * f * D_800BEA08;
        }
        arg0->unk39C = arg0->unk37C * 0.01745329238f;
        f = (sy / 80.0f) * 7.0f;
        if (f > 0.0f) {
            fwd = f * f * D_800BEA08;
        } else {
            fwd = -f * f * D_800BEA08;
        }
        if (buttons & 4) {
            D_80089560[idx] += 0.200000003f;
            arg0->unk2FC -= D_80089560[idx] * D_800BEA08;
        } else if (buttons & 8) {
            D_80089560[idx] += 0.200000003f;
            arg0->unk2FC += D_80089560[idx] * D_800BEA08;
        } else {
            D_80089560[idx] = 0.0f;
        }
        if (buttons & 0x8000) {
            D_80089580[idx] += 0.01250000019f;
            arg0->unk388 -= D_80089580[idx] * D_800BEA08;
        } else if (buttons & 0x4000) {
            D_80089580[idx] += 0.01250000019f;
            arg0->unk388 += D_80089580[idx] * D_800BEA08;
        } else {
            D_80089580[idx] = 0.0f;
        }
        if (arg0->unk388 < -89.5f) {
            arg0->unk388 = -89.5f;
        } else {
            f = (arg0->unk388 > 89.5f) ? 89.5f : arg0->unk388;
            arg0->unk388 = f;
        }
        arg0->unk398 = arg0->unk388 * 0.01745329238f;
        if (buttons & 0x2000) {
            if ((buttons & 2) && (buttons & 0x2000)) {
                D_80089570[idx] += 0.200000003f;
                arg0->unk5EC -= D_80089570[idx] * 0.1000000015f;
            } else if ((buttons & 1) && (buttons & 0x2000)) {
                D_80089570[idx] += 0.200000003f;
                arg0->unk5EC += D_80089570[idx] * 0.1000000015f;
            } else {
                D_80089570[idx] = 0.0f;
            }
        } else {
            if (buttons & 2) {
                D_80089570[idx] += 0.400000006f;
                strafe -= D_80089570[idx];
            } else if (buttons & 1) {
                D_80089570[idx] += 0.400000006f;
                strafe += D_80089570[idx];
            } else {
                D_80089570[idx] = 0.0f;
            }
        }
        D_80089570[idx] = (D_80089570[idx] > 100.0f) ? 100.0f : D_80089570[idx];
        D_80089580[idx] = (D_80089580[idx] > 8.0f) ? 8.0f : D_80089580[idx];
        D_80089560[idx] = (D_80089560[idx] > 40.0f) ? 40.0f : D_80089560[idx];
        if (arg0->unk5EC < -45.0f) {
            arg0->unk5EC = -45.0f;
        } else {
            f = (arg0->unk5EC > 45.0f) ? 45.0f : arg0->unk5EC;
            arg0->unk5EC = f;
        }
        dx = -sinf(arg0->unk39C);
        dz = -cosf(arg0->unk39C);
        arg0->unk2F8 += dx * fwd;
        arg0->unk300 += dz * fwd;
        arg0->unk2F8 += -dz * strafe;
        arg0->unk300 += dx * strafe;
        arg0->unk344 = 0.0f;
        dx *= 600.0f;
        dz *= 600.0f;
        f = sqrtf(dx * dx + dz * dz);
        dy = sinf(arg0->unk398) * f;
        dx = cosf(arg0->unk398) * dx;
        dz = cosf(arg0->unk398) * dz;
        sx = arg0->unk2F8 + dx;
        arg0->unk2A4.x = sx;
        sy = arg0->unk2FC + dy;
        arg0->unk2BC.x = sx;
        f = arg0->unk300 + dz;
        arg0->unk390 = 0.0f;
        arg0->unk2A4.z = arg0->unk2BC.z = f;
        arg0->unk2A4.y = arg0->unk2BC.y = sy;
        arg0->unk2A4 = arg0->unk2BC;
    } else {
        arg0->unk394 = arg0->unk37C;
    }
    arg0->unk380 = arg0->unk37C;
    arg0->unk3A0 = arg0->unk39C = arg0->unk37C * 0.01745329238f;
    func_1510E7A4(&arg0->unk644, &arg0->unk648, &sp68, &arg0->unk360, &arg0->unk640, 0, arg0->unk2F8, arg0->unk2FC,
                  arg0->unk300, arg0->unk2FC, 0, 0, -10000.0f, arg0->unk2FC);
}

extern s32 func_151E6C1C(s32);

s32 func_1512A360(u8 *arg0) {
    return func_151E6C1C(arg0[0x23D]);
}
