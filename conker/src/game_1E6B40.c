#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#define func_150ADA20 func_150ADA20_u8_unused
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"


typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    s8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s8 unk30;
    s8 unk31;
    s8 unk32;
    s16 unk34;
} Packet151B9690;

extern s32 D_8008CA4C;
extern s32 func_150ADA20(void);
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_151B9690(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, f32 arg6, s16 arg7,
                   s16 arg8, s16 arg9, s16 argA, s16 argB, s16 argC, s16 argD, s16 argE, u8 argF,
                   s32 arg10) {
    Packet151B9690 pkt;
    s32 saved;
    s16 *pa;
    f32 speed;
    f32 spd;
    f32 w4;
    f32 sn;
    f32 cs;
    f32 w5;
    f32 dist;
    s32 y;
    u8 r;
    u8 g;
    u8 b;

    saved = arg3;
    arg6 += 180.0f;
    arg6 += (f32)(s32)((func_150ADA20() % (u32)arg7) - (arg7 >> 1));
    sn = sinf(arg6 * 0.01745329238f);
    cs = cosf(arg6 * 0.01745329238f);
    dist = argA;
    if (argB == 0) {
        argB = 1;
    }
    dist += (f32)(s32)((func_150ADA20() % (u32)argB) - (argB >> 1));
    speed = dist;
    spd = argC;
    arg2 = (s32)(arg2 + spd * sn);
    arg4 = (s32)(arg4 + spd * cs);
    arg3 += 1;
    y = ((func_150ADA20() % (u32)arg9) - (arg9 >> 1));
    y = arg8 + y;
    switch (arg1) {
    case 2:
        r = 0x68; g = 0x38; b = 0x10;
        break;
    case 0:
    case 1:
    case 3:
    default:
        r = g = b = 0xFF;
        break;
    }
    pkt.unk00 = (&D_8008CA4C)[arg0];
    pkt.unk04 = (argD << 16) | (saved & 0xFFFF);
    pkt.unk08 = arg1;
    pkt.unk0C = 0;
    pkt.unk0E = 0x100;
    pkt.unk10 = arg2;
    pkt.unk12 = arg3;
    pkt.unk14 = arg4;
    pkt.unk16 = (s32)(speed * sn);
    pkt.unk18 = (s32)(speed * cs);
    pkt.unk1A = argE;
    pkt.unk1F = 0;
    pkt.unk20 = y;
    pkt.unk22 = -0xA0;
    pkt.unk24 = argE;
    pkt.unk26 = argE;
    pkt.unk28 = 0x190;
    pkt.unk2A = 0;
    pkt.unk2B = func_150ADA20();
    pkt.unk2F = 0xFF;
    pkt.unk34 = 0;
    pkt.unk2C = r;
    pkt.unk2D = g;
    pkt.unk2E = b;
    func_15167D84(&pkt, 0, 0, -1, argF, arg10);
}

typedef struct {
    u8  unk0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD[0x7];
    s32 unk14;
    s32 unk18;
    u8  pad1C[0x4];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8  pad26[0x4];
    s16 unk2A;
    u8  pad2C[0x4];
    s16 unk30;
    u8  pad32[0x2];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8  pad3A;
    u8  unk3B;
    u8  pad3C[0x3];
    u8  unk3F;
} struct_151B9964;

extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);
extern s32 func_150ADA20(void);
extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);

void func_151B9964(struct_151B9964 *arg0) {
    s32 cur;
    s32 top;
    s32 hi;
    s32 v;
    f32 sp5C;
    s16 y;
    s32 diff;
    s32 mode;
    u8 angle;

    mode = arg0->unk18;
    if (mode == 1) {
        angle = arg0->unk3B;
        angle += D_800BE9E4 * 4 + D_800BE9E4 * 2;
        arg0->unk3B = angle;
        arg0->unk2A = (s32)(arg0->unk2A * 0.9900000095f);
        sp5C = arg0->unk2A;
        arg0->unk34 = (s32)((func_15048A40(angle) + 3.0f) * sp5C * 0.3333333433f);
        arg0->unk36 = (s32)((func_150489B0(angle) + 3.0f) * sp5C * 0.3333333433f);
    } else {
        cur = arg0->unk34;
        hi = arg0->unk14 >> 16;
        if (hi != cur) {
            v = cur - D_800BE9E4 * 4;
            if (v < hi) {
                v = hi;
            }
            arg0->unk36 = v;
            arg0->unk34 = v;
        }
    }
    v = arg0->unk22;
    y = arg0->unk14 & 0xFFFF;
    if (y >= v) {
        arg0->unk38 = 0;
        if (mode == 0) {
            if ((func_150ADA20() & 0xFF) < 0x40) {
                func_15171D4C(arg0->unk20, y, arg0->unk24, 0xA, 0, 0x13, 0.0f, 0, 0x32, 0xF, 0x100, 0, arg0->unkC, arg0->unk1);
            }
            if ((func_150ADA20() & 0xFF) < 0xF) {
                func_150ADA20();
                func_10010F88(0x70, 0x1388, 0, 0, 0, arg0->unk20, y, arg0->unk24, 0x1F4, 0x7D0);
            }
        }
    } else {
        diff = v - y;
        if (arg0->unk30 < 0 && diff < 0x40) {
            arg0->unk3F = diff * 4;
        }
    }
}

typedef struct {
    u8  unk0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD[0x3];
    s32 unk10;
    u8  unk14;
    u8  unk15;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
} struct_151B9BF0;

s32 func_15167A68(s32, s32, s32, s32, s32, s32);
void func_151B9690(s32, s32, s16, s16, s16, s16, f32, s16, s16, s16, s16, s16, s16, s16, s16, u8, s32);

void func_151B9BF0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5,
                   s16 arg6, s16 arg7, s16 arg8, s16 arg9, s16 arg10, s16 arg11,
                   s16 arg12, s32 arg13, u8 arg14, s32 arg15) {
    struct_151B9BF0 *ret;

    ret = (struct_151B9BF0 *)func_15167A68(7, arg15, 0x2C, 0, arg14, 1);
    if (ret != 0) {
        ret->unk14 = arg0;
        ret->unk15 = arg1;
        ret->unk16 = arg2;
        ret->unk18 = arg3;
        ret->unk1A = arg4;
        ret->unk1C = arg5;
        ret->unk1E = arg6;
        ret->unk20 = arg7;
        ret->unk22 = arg8;
        ret->unk24 = arg9;
        ret->unk26 = arg10;
        ret->unk28 = arg11;
        ret->unk2A = arg12;
        ret->unk10 = arg13;
    }
}

void func_151B9CB0(struct_151B9BF0 *arg0) {
    f32 *temp_v0;
    struct_151B9BF0 *temp_s5;
    s16 temp_s2;
    s16 temp_s3;
    s16 temp_s4;
    s32 i;
    f32 temp_f20;
    s32 temp_s5_2;
    s32 temp_s6;
    s32 temp_v0_2;

    temp_v0 = (f32 *)arg0->unk10;
    temp_s5 = arg0;
    if (temp_v0 == 0) {
        temp_s2 = arg0->unk16;
        temp_s3 = arg0->unk18;
        temp_s4 = arg0->unk1A;
    } else {
        temp_s2 = (s16)(s32)temp_v0[0];
        temp_s3 = (s16)(s32)temp_v0[1];
        temp_s4 = (s16)(s32)temp_v0[2];
    }

    i = 0;
    if (arg0->unk28 > 0) {
        temp_f20 = 0.0f;
        do {
            func_151B9690(arg0->unk14, arg0->unk15, temp_s2, temp_s3, temp_s4, -0x50,
                          temp_f20, 0x168, arg0->unk1C, arg0->unk1E, arg0->unk20,
                          arg0->unk22, 0xF, arg0->unk24, arg0->unk26, arg0->unkC,
                          arg0->unk1);
            i++;
        } while (i < temp_s5->unk28);
        i = 0;
    }
    temp_f20 = 0.0f;

    temp_s6 = 4;
    temp_s5_2 = 3;
    while (1) {
        func_151B9690(arg0->unk14, arg0->unk15, temp_s2, temp_s3, temp_s4, -0x50,
                      temp_f20, 0x168, (arg0->unk1C * temp_s5_2) / 2,
                      arg0->unk1E * 2, arg0->unk20, arg0->unk22, 0xF,
                      arg0->unk24, arg0->unk26, arg0->unkC, arg0->unk1);
        i++;
        if (i == temp_s6) {
            break;
        }
    }

    temp_v0_2 = arg0->unk2A;
    temp_v0_2 -= D_800BE9E4;
    if (temp_v0_2 < 0) {
        func_1516972C((struct102 *)arg0);
    } else {
        arg0->unk2A = temp_v0_2;
    }
}
