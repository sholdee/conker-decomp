#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"


typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x83];
    /* 0x90 */ f32 unk90;
    /* 0x94 */ f32 unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ f32 unkA4;
    /* 0xA8 */ f32 unkA8;
    /* 0xAC */ f32 unkAC;
    /* 0xB0 */ f32 unkB0;
    /* 0xB4 */ f32 unkB4;
    /* 0xB8 */ f32 unkB8;
    /* 0xBC */ f32 unkBC;
    /* 0xC0 */ u8 padC0[4];
    /* 0xC4 */ f32 unkC4;
    /* 0xC8 */ u8 padC8[4];
    /* 0xCC */ s32 unkCC;
    /* 0xD0 */ u8 padD0[4];
    /* 0xD4 */ f32 unkD4;
    /* 0xD8 */ f32 unkD8;
    /* 0xDC */ f32 unkDC;
    /* 0xE0 */ f32 unkE0;
    /* 0xE4 */ u8 padE4;
    /* 0xE5 */ u8 unkE5;
    /* 0xE6 */ u8 padE6;
    /* 0xE7 */ u8 unkE7;
    /* 0xE8 */ u8 unkE8;
    /* 0xE9 */ u8 unkE9;
    /* 0xEA */ u8 unkEA;
    /* 0xEB */ u8 unkEB;
    /* 0xEC */ u8 unkEC;
} Struct15169C70;

extern f32 func_15048A40(u8 arg0);
extern s32 func_150ADA20(void);
extern s32 func_1510F800(s32);
extern s32 func_1510F8D8(s32, s32, s32, s32);
void func_15171200(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5,
                   f32 arg6, f32 arg7, f32 arg8, s32 arg9, s32 arg10, f32 arg11,
                   s32 arg12, s32 arg13);
void func_151709B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5,
                   s32 arg6);
void func_151718F0(f32, f32, f32, f32, s32, f32, s32, s32, s32, s32);
void func_1518CA04(s32 arg0);

void func_15169C70(Struct15169C70 *arg0) {
    f32 dt;
    s32 temp;
    s32 cc;
    s32 e5;

    dt = D_800BE9E4;
    arg0->unk90 += arg0->unkA8 * dt;
    arg0->unk98 += arg0->unkB0 * dt;
    arg0->unk9C += arg0->unkB4 * dt;
    arg0->unkA0 += arg0->unkB8 * dt;
    arg0->unkA4 += arg0->unkBC * dt;
    arg0->unkAC += -0.5f * dt;
    arg0->unk94 += arg0->unkAC * dt;
    if (arg0->unkE8 & 1) {
        arg0->unkE7 += D_800BE9E4 * 8;
        arg0->unkC4 = (func_15048A40(arg0->unkE7) * 0.400000006f + 1.0f) * arg0->unkD4;
    }
    if (arg0->unkE8 & 2) {
        arg0->unkA0 = func_150484A0(arg0->unkA8, arg0->unkB0) * 57.29577637f;
        arg0->unk9C = func_150484A0(-arg0->unkAC, sqrtf(arg0->unkA8 * arg0->unkA8 + arg0->unkB0 * arg0->unkB0)) * 57.29577637f;
    }
    func_1510F800(0);
    temp = func_1510F8D8(arg0->unk90, arg0->unk94, arg0->unk98, 0) + arg0->unkE9;
    if (arg0->unk94 < temp && arg0->unkAC < 0.0f) {
        arg0->unk94 = temp;
        if (arg0->unkEB == 1) {
            func_15171200(4, 0xB, arg0->unk90, arg0->unk94, arg0->unk98, arg0->unkAC * arg0->unkDC, 0.5f, 1.0f, 60.0f, 2, 0xFF, -0.400000006f, arg0->unkC, arg0->unk1);
            func_151718F0(arg0->unk90, (arg0->unk94 - (f32)(u32)arg0->unkE9) + 30.0f, arg0->unk98, 50.0f, 0x28, 0.0030517578125f, 0, 0x14, arg0->unkC, arg0->unk1);
            func_151718F0(arg0->unk90, (arg0->unk94 - (f32)(u32)arg0->unkE9) + 15.0f, arg0->unk98, 30.0f, 0x28, 0.0030517578125f, 0, 0xA, arg0->unkC, arg0->unk1);
            func_1518CA04(arg0->unkEA);
            func_1516972C((struct102 *)arg0);
            return;
        }
        if (arg0->unkEB == 4) {
            func_15171200(3, 0xB, arg0->unk90, arg0->unk94, arg0->unk98, arg0->unkAC * arg0->unkDC, 0.5f, 0.6999999881f, 40.0f, 2, 0x3C, -0.5f, arg0->unkC, arg0->unk1);
            func_1518CA04(arg0->unkEA);
            func_1516972C((struct102 *)arg0);
            return;
        }
        if (arg0->unkEB == 3) {
            func_15171200(2, 0xCB, arg0->unk90, arg0->unk94, arg0->unk98, 6.0f, 0.5f, 1.0f, 60.0f, 5, 0x3C, -0.6999999881f, arg0->unkC, arg0->unk1);
            func_151709B4(arg0->unk90, arg0->unk94 + 10.0f, arg0->unk98, 4, arg0->unkAC * 200.0f, arg0->unkC, arg0->unk1);
            func_1518CA04(arg0->unkEA);
            func_1516972C((struct102 *)arg0);
            return;
        }
        if (arg0->unkEB == 6) {
            arg0->unkB0 = 0.0f;
            arg0->unkAC = 0.0f;
            arg0->unkA8 = 0.0f;
            arg0->unkBC = 0.0f;
            arg0->unkB8 = 0.0f;
            arg0->unkB4 = 0.0f;
            arg0->unkDC = 0.0f;
            arg0->unkEB = 0;
            arg0->unkCC = 0x3C;
            return;
        }
        if (arg0->unkEB == 8) {
            func_1518CA04(arg0->unkEA);
            func_1516972C((struct102 *)arg0);
            return;
        }
        arg0->unkA8 *= arg0->unkD8;
        arg0->unkB0 *= arg0->unkD8;
        arg0->unkAC *= arg0->unkDC;
        if (arg0->unkAC < 2.0f) {
            arg0->unkA8 = 0;
            arg0->unkAC = 0;
            arg0->unkB0 = 0;
            arg0->unkB4 = 0;
            arg0->unkB8 = 0;
            arg0->unkBC = 0;
        } else {
            func_151718F0(arg0->unk90, (arg0->unk94 - (f32)(u32)arg0->unkE9) + 15.0f, arg0->unk98, 20.0f, 0x32, 0.000457763671875f, 0, 4, arg0->unkC, arg0->unk1);
            if (arg0->unkEB == 2) {
                if (func_150ADA20() & 0x8000) {
                    arg0->unkB4 = (f32)(func_150ADA20() >> 16) * 0.000152587890625f + 40.0f;
                }
            } else if (arg0->unkEB == 5) {
                func_151709B4(arg0->unk90, arg0->unk94 + 10.0f, arg0->unk98, 4, arg0->unkAC * 200.0f, arg0->unkC, arg0->unk1);
            }
        }
        arg0->unkB4 *= arg0->unkE0;
        arg0->unkBC *= arg0->unkE0;
    }
    cc = arg0->unkCC;
    if (cc != 0) {
        cc -= D_800BE9E4;
        if (cc <= 0) {
            arg0->unkEC = 2;
            e5 = arg0->unkE5;
            e5 -= D_800BE9E4 * 4;
            if (e5 <= 0) {
                func_1518CA04(arg0->unkEA);
                func_1516972C((struct102 *)arg0);
                return;
            }
            arg0->unkE5 = e5;
            cc = 1;
        }
        arg0->unkCC = cc;
    }
}

struct Struct1516A3F4 {
    u8 pad0;
    u8 unk1;
    u8 padb[0xA];
    u8 unkC;
    u8 padd[0x7];
    s32 unk14;
    u8 pad18[0x8];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    u8 pad2c[0xC];
    s16 unk38;
    u8 pad3a[0x1];
    u8 unk3B;
};

struct Struct15167AD8 {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    u8 pad13[0x1];
    s8 unk14;
    u8 unk15;
};

extern s32 func_1510F800(s32);
extern s32 func_1510F8D8(s32, s32, s32, s32);
extern void func_15167AD8(struct Struct15167AD8 *, s32, s32);
extern s32 D_8008CADC;
extern f32 D_800A6CC8;

void func_1516A3F4(struct Struct1516A3F4 *arg0) {
    s32 temp;
    s32 t0;
    s32 v1;
    struct Struct15167AD8 sp;

    t0 = arg0->unk22 + arg0->unk2A;
    v1 = arg0->unk14;
    if (v1 >= t0) {
        func_1510F800(0);
        v1 = arg0->unk14 = func_1510F8D8(arg0->unk20, arg0->unk22, arg0->unk24, 0);
    }
    if (v1 >= t0) {
        arg0->unk38 = 0;
        if (t0 - v1 < 0x32) {
            sp.unk0 = D_8008CADC;
            sp.unk4 = 0;
            sp.unk6 = 0x100;
            sp.unk8 = arg0->unk20;
            sp.unkA = arg0->unk14 + 0xA;
            sp.unkC = arg0->unk24;
            temp = arg0->unk3B;
            sp.unk12 = 0;
            v1 = (temp << 12) / 0x13EC + 0xCC;
            sp.unkE = v1;
            sp.unk10 = v1;
            sp.unk15 = temp;
            sp.unk14 = 3;
            func_15167AD8(&sp, arg0->unkC, arg0->unk1);
        }
    } else {
        f32 sc = D_800A6CC8;
        arg0->unk26 = (s16)((f32)(s32)arg0->unk26 * sc);
        arg0->unk28 = (s16)((f32)(s32)arg0->unk28 * sc);
    }
}

struct Struct1516A538 {
    u8 pad0;
    u8 unk1;
    u8 padb[0xA];
    u8 unkC;
    u8 padd[0x7];
    s16 unk14;
    u8 pad16[0x2];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    u8 pad1e[0x7];
    u8 unk25;
};

extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);
extern void func_1516F864(s32);
extern void func_1516F94C(s32, s32);

void func_1516A538(struct Struct1516A538 *arg0) {
    u32 b;
    u32 a;
    s32 c;

    if (arg0->unk14 >= 0x300) {
        a = (u32)func_150ADA20() % 3;
        c = arg0->unk25;
        b = (u32)func_150ADA20() % 0x168;
        func_15171D4C(
            (f32)(s32)arg0->unk18,
            (f32)((s32)arg0->unk1A - 0xA),
            (f32)(s32)arg0->unk1C,
            0x64,
            0,
            a + 0x20,
            (f32)(u32)b,
            0,
            (c * 25) / 255 + 0x19,
            0,
            0x100,
            0,
            arg0->unkC,
            arg0->unk1);
    }
}

typedef struct {
    char pad_0[0xE];
    s16 field_0xE;
    s16 field_0x10;
    s16 field_0x12;
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    char pad_0x1A[0x5];
    u8 field_0x1F;
    char pad_0x20[0x4];
    u8 field_0x24;
    char pad_0x25[0x7];
    u8 field_0x2C;
    u8 field_0x2D;
} Obj1516A648;

s32 func_1516A648(Obj1516A648 *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v1;

    temp_a1 = arg0->field_0x24;
    temp_v0 = arg0->field_0x1F;
    if (temp_a1 != 0) {
        if (temp_v0 != 0xFF) {
            temp_v0 += D_800BE9E4 << 4;
            if (temp_v0 >= 0x100) {
                temp_v0 = 0xFF;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516A648 *)arg0)->field_0x24;
        }
    } else {
        if (temp_v0 != 0) {
            temp_v0 -= D_800BE9E4 << 2;
            if (temp_v0 < 0) {
                temp_v0 = 0;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516A648 *)arg0)->field_0x24;
        }
    }

    if ((temp_a1 == 0) && (temp_v0 == 0)) {
        return 1;
    }

    arg0->field_0x18 = arg0->field_0x18 - 0x12C;
    func_1516F864((s32)arg0);
    temp_v1 = func_1510F8D8(arg0->field_0xE, arg0->field_0x10, arg0->field_0x12, 0) + 0xA;
    arg0->field_0x2C = temp_v1 >> 8;
    arg0->field_0x2D = temp_v1 & 0xFF;
    if (arg0->field_0x10 < temp_v1) {
        arg0->field_0x10 = temp_v1;
        arg0->field_0x18 = (s16)(s32)((f32)(s32)arg0->field_0x18 * -0.5f);
        func_1516F94C((s32)arg0, 0xE6);
    }
    return 0;
}
