#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_151D9014(void *a0, void *a1, s32 a2, f32 a3, s32 sp10, s32 sp14,
                   f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C,
                   s32 sp30, s32 sp34, s32 sp38, s32 sp3C);
void func_151B9BF0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                   s32 arg12, s32 arg13, s32 arg14, s32 arg15);
void func_151EF040(s32 arg0, s32 *arg1);
void func_15171200(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5,
                   f32 arg6, f32 arg7, f32 arg8, s32 arg9, s32 arg10, f32 arg11,
                   s32 arg12, s32 arg13);
void func_151709B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5,
                   s32 arg6);
void func_1516D99C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                   s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16,
                   s32 arg17, s32 arg18, s32 arg19, s32 arg20, s32 arg21,
                   s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26,
                   s32 arg27, s32 arg28, s32 arg29, s32 arg30, s32 arg31,
                   s32 arg32, s32 arg33, s32 arg34, s32 arg35, s32 arg36,
                   s32 arg37, s32 arg38, s32 arg39, s32 arg40, s32 arg41,
                   s32 arg42, s32 arg43, s32 arg44, s32 arg45, s32 arg46,
                   s32 arg47, s32 arg48);
struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3,
                         s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 offset,
                         s32 arg9, s32 argA);
extern f32 D_800A0338;
extern s32 func_10004074[];

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Struct150C3D5CArg;

typedef struct {
    Header2 header2;
    Header header;
    u8 pad12[0xA];
    s32 check;
} Struct150C3D5CLocals;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3A */ s8 unk3A;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s8 unk7C;
    /* 0x7D */ s8 unk7D;
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 pad84;
} Struct150C2558;

typedef struct {
    f32 v[3];
} Vec3Copy_150C1F60;

void func_15151A38(Struct150C2558 *, u8, s32);
typedef struct {
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ f32 unk18[3];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ s8 unk3D;
    /* 0x3E */ u8 unk3E[2];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ s8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A[2];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s8 unk50;
    /* 0x51 */ u8 pad51[3];
    /* 0x54 */ f32 unk54;
} Struct150C2700;
void func_15150178(Struct150C2700 *arg0, f32 *arg1, s32 arg2, u8 arg3, s32 arg4);

void func_150C1F60(u8 arg0) {
    Struct150C2558 sp68;

    sp68.unk00 = 8152.0f;
    sp68.unk04 = 40.0f;
    sp68.unk08 = -309.0f;
    sp68.unk1C = 40.91999817f;
    sp68.unk20 = 20.45000076f;
    sp68.unk0C = 9.0f;
    sp68.unk10 = 21.89999962f;
    sp68.unk24 = 0xC;
    sp68.unk26 = 0xD;
    sp68.unk28 = -0xA0;
    sp68.unk2C = -0x3C;
    sp68.unk2A = 0x45;
    sp68.unk2E = 0x43;
    sp68.unk30 = 3;
    sp68.unk32 = 5;
    sp68.unk34 = 0x32;
    sp68.unk36 = 0x1F;
    sp68.unk14 = 1.460000038f;
    sp68.unk18 = 0.5f;
    sp68.unk38 = 0xB;
    sp68.unk39 = 1;
    sp68.unk3A = 0x28;
    sp68.unk3C = 1;
    sp68.unk40 = 4;
    sp68.unk44 = 0;
    sp68.unk48 = 0;
    sp68.unk4C = 0;
    sp68.unk50 = 0;
    sp68.unk54 = 0xFF;
    sp68.unk55 = 0xFF;
    sp68.unk56 = 0;
    sp68.unk58 = 3;
    sp68.unk5C = 0xFF;
    sp68.unk60 = 0;
    sp68.unk64 = 0x220005;
    sp68.unk68 = 0x1D0600;
    sp68.unk6C = 1;
    sp68.unk70 = 0x3B;
    sp68.unk74 = 0x80;
    sp68.unk78 = 0x20;
    sp68.unk7C = 0;
    sp68.unk7D = 7;
    sp68.unk80 = 0xC;
    sp68.unk82 = 0x15;
    func_15151A38(&sp68, arg0, 1);
    sp68.unk1C = 100.0f;
    sp68.unk20 = 57.5f;
    sp68.unk0C = 55.59999847f;
    sp68.unk10 = 39.0f;
    sp68.unk24 = 2;
    sp68.unk26 = 3;
    sp68.unk28 = -0x6C;
    sp68.unk2C = -0x16;
    sp68.unk2A = 0x15;
    sp68.unk2E = 0x16;
    sp68.unk30 = 7;
    sp68.unk32 = 0;
    sp68.unk34 = 0x30;
    sp68.unk36 = 0x15;
    sp68.unk14 = 0.4600000083f;
    sp68.unk18 = 1.0f;
    func_15151A38(&sp68, arg0, 1);
    {
    Struct150C2700 sp20;
    *(Vec3Copy_150C1F60 *)sp20.unk18 = *(Vec3Copy_150C1F60 *)&sp68;
    sp20.unk24 = 0x19;
    sp20.unk26 = 0x12;
    sp20.unk10 = -0x9F;
    sp20.unk12 = 0x44;
    sp20.unk14 = -0x34;
    sp20.unk16 = 0x26;
    sp20.unk30 = 0x19;
    sp20.unk32 = 0x19;
    sp20.unk3C = 0x9B;
    sp20.unk3D = 0x64;
    sp20.unk48 = 1;
    sp20.unk49 = 6;
    sp20.unk4C = 0.0f;
    sp20.unk50 = 1;
    sp20.unk54 = 0.0f;
    sp20.unk28 = 40.0f;
    sp20.unk2C = 35.0f;
    sp20.unk34 = -2.021000147f;
    sp20.unk38 = 0.8940000534f;
    sp20.unk40 = 1152.0f;
    sp20.unk44 = 848.0f;
    func_15150178(&sp20, sp20.unk18, 0, arg0, 1);
}
}

void func_150C2290(u8 arg0) {
    Struct150C2558 sp18;

    sp18.unk00 = 8152.0f;
    sp18.unk04 = 40.0f;
    sp18.unk08 = 309.0f;
    sp18.unk1C = 100.0f;
    sp18.unk20 = 57.5f;
    sp18.unk0C = 55.59999847f;
    sp18.unk10 = 39.0f;
    sp18.unk24 = 2;
    sp18.unk26 = 3;
    sp18.unk28 = -0x29;
    sp18.unk2C = -0x16;
    sp18.unk2A = 0x15;
    sp18.unk2E = 0x16;
    sp18.unk30 = 7;
    sp18.unk32 = 0;
    sp18.unk34 = 0x30;
    sp18.unk36 = 0x15;
    sp18.unk14 = 0.4600000083f;
    sp18.unk18 = 1.0f;
    sp18.unk38 = 0xB;
    sp18.unk39 = 1;
    sp18.unk3A = 0x28;
    sp18.unk3C = 1;
    sp18.unk40 = 4;
    sp18.unk44 = 0;
    sp18.unk48 = 0;
    sp18.unk4C = 0;
    sp18.unk50 = 0;
    sp18.unk54 = 0xFF;
    sp18.unk55 = 0xFF;
    sp18.unk56 = 0;
    sp18.unk58 = 3;
    sp18.unk5C = 0xFF;
    sp18.unk60 = 0;
    sp18.unk64 = 0x220005;
    sp18.unk68 = 0x1D0600;
    sp18.unk6C = 1;
    sp18.unk70 = 0x3B;
    sp18.unk74 = 0x80;
    sp18.unk78 = 0x20;
    sp18.unk7C = 0;
    sp18.unk7D = 7;
    sp18.unk80 = 0xC;
    sp18.unk82 = 0x15;
    func_15151A38(&sp18, (u8)arg0, 1);
}
// TODO: figure out this struct!
// void func_150C2290(s32 arg0) {
//     s16 sp9A;
//     s16 sp98;
//     s8 sp95;
//     s8 sp94;
//     ?32 sp90;
//     ?32 sp8C;
//     ?32 sp88;
//     ?32 sp84;
//     ?32 sp80;
//     ?32 sp7C;
//     ?32 sp78;
//     ?32 sp74;
//     ?32 sp70;
//     s8 sp6E;
//     s8 sp6D;
//     s8 sp6C;
//     ?32 sp68;
//     ?32 sp64;
//     ?32 sp60;
//     ?32 sp5C;
//     ?32 sp58;
//     ?32 sp54;
//     s8 sp52;
//     s8 sp51;
//     s8 sp50;
//     s16 sp4E;
//     s16 sp4C;
//     s16 sp4A;
//     s16 sp48;
//     s16 sp46;
//     s16 sp44;
//     s16 sp42;
//     s16 sp40;
//     s16 sp3E;
//     s16 sp3C;
//     f32 sp38;
//     f32 sp34;
//     f32 sp30;
//     f32 sp2C;
//     f32 sp28;
//     f32 sp24;
//     f32 sp20;
//     f32 sp1C;
//     f32 sp18;
//
//     sp18 = *(void *)0x800A0258;
//     sp1C = 40.0f;
//     sp3E = 3;
//     sp40 = -0x29;
//     sp44 = -0x16;
//     sp42 = 0x15;
//     sp46 = 0x16;
//     sp48 = 7;
//     sp4E = 0x15;
//     sp50 = 0xB;
//     sp20 = *(void *)0x800A025C;
//     sp3C = 2;
//     sp4C = 0x30;
//     sp52 = 0x28;
//     sp54 = 1;
//     sp58 = 4;
//     sp6C = 0xFF;
//     sp6D = 0xFF;
//     sp70 = 3;
//     sp4A = 0;
//     sp51 = 1;
//     sp5C = 0;
//     sp60 = 0;
//     sp64 = 0;
//     sp68 = 0;
//     sp6E = 0;
//     sp74 = 0xFF;
//     sp78 = 0;
//     sp7C = 0x220005;
//     sp80 = 0x1D0600;
//     sp84 = (?32) (u8)1;
//     sp88 = 0x3B;
//     sp8C = 0x80;
//     sp90 = 0x20;
//     sp94 = 0;
//     sp95 = 7;
//     sp98 = 0xC;
//     sp9A = 0x15;
//     sp34 = 100.0f;
//     sp38 = 57.5f;
//     sp24 = *(void *)0x800A0260;
//     sp28 = 39.0f;
//     sp2C = *(void *)0x800A0264;
//     sp30 = 1.0f;
//     func_15151A38(&sp18, arg0 & 0xFF, 1);
// }

void func_1514C470(f32, f32, f32, f32, f32, f32, f32, s32, s32, f32, s32, u8);
extern f32 D_800A0268;
extern f32 D_800A026C;

void func_150C2424(u8 arg0) {
    f32 *p = &D_800A0268;

    func_1514C470(
        *p,
        -490.0f,
        -328.0f,
        *p,
        -490.0f,
        328.0f,
        (func_150ADA68() * 8.0f) + 8.0f,
        1,
        0,
        0.0f,
        0,
        arg0
    );
    func_1514C470(
        D_800A026C,
        -560.0f,
        -580.0f,
        8117.0f,
        -560.0f,
        -580.0f,
        (func_150ADA68() * 3.0f) + 4.0f,
        3,
        0,
        0.0f,
        0,
        arg0
    );
}
s32 func_150C251C(void *arg0) {
    void *temp_v0 = *(void **)((s32)arg0 + 0x98);
    s32 temp_v1 = *(s16 *)((s32)arg0 + 0x1C) << 3;
    s32 temp_t7;

    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }
    temp_t7 = temp_v1 & 0xFF;
    *(u8 *)((s32)temp_v0 + 0x1B) = temp_v1;
    if (temp_t7 < 0) {
        return 0;
    }
    return 1;
}
extern f32 D_800A0270;


s32 func_150C2558(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    Struct150C2558 sp18;

    sp18.unk00 = arg2;
    sp18.unk04 = arg3;
    sp18.unk08 = (arg4 + 15.0f) - (func_150ADA68() * 30.0f);
    sp18.unk24 = 1;
    sp18.unk26 = 0;
    sp18.unk28 = (s32)(sp18.unk08 * D_800A0270) - 0x40;
    sp18.unk2A = 9;
    sp18.unk2C = 0;
    sp18.unk2E = 0;
    sp18.unk30 = 3;
    sp18.unk32 = 3;
    sp18.unk34 = 0x28;
    sp18.unk36 = 0x15;
    sp18.unk1C = 200.0f;
    sp18.unk20 = 150.0f;
    sp18.unk0C = 22.0f;
    sp18.unk14 = 0.0f;
    sp18.unk18 = 0.0f;
    sp18.unk10 = 44.0f;
    sp18.unk38 = 0xB;
    sp18.unk39 = 1;
    sp18.unk3A = 0x28;
    sp18.unk3C = 1;
    sp18.unk40 = 4;
    sp18.unk44 = 0;
    sp18.unk48 = 0;
    sp18.unk4C = 0;
    sp18.unk50 = 0;
    sp18.unk54 = 0xFF;
    sp18.unk55 = 0xFF;
    sp18.unk56 = 0;
    sp18.unk58 = 3;
    sp18.unk5C = 0xFF;
    sp18.unk60 = 0;
    sp18.unk64 = 0x220005;
    sp18.unk68 = 0x1D0600;
    sp18.unk6C = 1;
    sp18.unk70 = 0x3B;
    sp18.unk74 = 0x80;
    sp18.unk78 = 0x20;
    sp18.unk7C = 0;
    sp18.unk7D = 7;
    sp18.unk80 = 0xC;
    sp18.unk82 = 0x15;
    func_15151A38(&sp18, argE, 1);
    return 1;
}
extern f32 D_800A0274;
extern f32 D_800A0278;
extern f32 D_800A027C;


s32 func_150C2700(s32 a0, s32 a1, f32 a2, f32 a3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    Struct150C2700 sp20;

    sp20.unk18[0] = a2;
    sp20.unk18[1] = a3;
    sp20.unk24 = 6;
    sp20.unk26 = 5;
    sp20.unk10 = 0x6B;
    sp20.unk12 = 0x46;
    sp20.unk14 = -0x1F;
    sp20.unk16 = 0x2E;
    sp20.unk30 = 0x64;
    sp20.unk32 = 0;
    sp20.unk3C = 0x9B;
    sp20.unk3D = 0x64;
    sp20.unk44 = 0.0f;
    sp20.unk48 = 1;
    sp20.unk49 = 6;
    sp20.unk4C = 0.0f;
    sp20.unk50 = 1;
    sp20.unk54 = 0.0f;
    sp20.unk18[2] = arg4;
    sp20.unk28 = 30.0f;
    sp20.unk2C = 35.0f;
    sp20.unk34 = D_800A0274;
    sp20.unk38 = D_800A0278;
    sp20.unk40 = D_800A027C;
    func_15150178(&sp20, sp20.unk18, 0, argE, 1);
    return 1;
}

extern f32 D_800A0280;
extern f32 D_800A0284;
extern void func_15134908(void *, s32, u8, s32);

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 unk19;
} Struct150C2804;

void func_150C2804(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, s32 arg5) {
    Struct150C2804 sp1C;

    sp1C.unk0C = D_800A0280;
    sp1C.unk00 = arg0;
    sp1C.unk04 = arg1;
    sp1C.unk08 = arg2;
    sp1C.unk10 = D_800A0284;
    sp1C.unk14 = arg3;
    sp1C.unk16 = 5;
    sp1C.unk17 = 6;
    sp1C.unk18 = 3;
    sp1C.unk19 = -1;
    func_15134908(&sp1C, 0, arg4, arg5);
}
void func_150C2898(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, u8 *a6) {
    f32 sp6C[3];
    f32 sp60[3];
    f32 r;
    struct { f32 c; s32 a; u32 b; s32 pad; } sp4C;

    sp6C[0] = a0;
    sp6C[1] = a1;
    sp6C[2] = a2;
    r = (func_150ADA68() * 112.0f + 247.0f) * D_800A0288;
    sp60[0] = -a3 * r;
    sp60[1] = -a4 * r;
    sp60[2] = -a5 * r;
    sp4C.c = func_150ADA68();
    sp4C.a = func_150ADA20();
    sp4C.b = func_150ADA20();
    func_151D9014(sp6C, sp60, 6, sp4C.c * D_800A028C + D_800A0290,
                    (sp4C.a & 0xF) + 25, (sp4C.b % 101) + 155,
                    func_150ADA68() * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    a6[0xC], a6[1]);
}
typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg_EF410;

typedef struct {
    f32 x;
    f32 z;
    f32 scale;
    f32 y;
} PosSub;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    PosSub unk28;
} Struct150C29F0;

s32 func_15149550(Struct15149550Arg_EF410 *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);

void func_150C29F0(Struct150C29F0 *arg0) {
    PosSub *pos;
    Struct15149550Arg_EF410 sp3C;
    f32 t;
    f32 t2;
    u8 angle;
    f32 c;
    f32 s;
    f32 scale;

    angle = func_150ADA20() & 0xFF;
    c = func_151423D8(angle - 0x40);
    s = func_151423D8(angle);
    pos = &arg0->unk28;
    scale = func_150ADA68() * pos->scale;
    sp3C.unk20 = 0x6231; sp3C.unk22 = 0x1A4D;
    sp3C.unk26 = 0;
    sp3C.unk27 = 0;
    sp3C.unk28 = 0;
    sp3C.unk29 = 0xFF;
    sp3C.unk2C = 0xFF;
    sp3C.unk2D = 0xFF;
    sp3C.unk30 = 0;
    sp3C.unk2A = 1;
    sp3C.unk2E = 0;
    sp3C.unk2F = 1;
    sp3C.unk24 = (func_150ADA20() % 201U) + 100;
    sp3C.unk34 = 0xF;
    sp3C.unk38 = 0.0f;
    sp3C.unk40 = 0.0450000018f;
    sp3C.unk44 = 0.699999988f;
    sp3C.unk0.unk0 = pos->x + scale * c;
    sp3C.unk0.unk4 = pos->y;
    sp3C.unk0.unk8 = pos->z + scale * s;
    sp3C.unk3C = 10000.0f;
    t = (func_150ADA68() * 0.5f) + 1.0f;
    t2 = (func_150ADA68() * 0.5f) + 1.0f;
    sp3C.unkC = 22.6f * t;
    sp3C.unk14 = 13.786f * t;
    sp3C.unk10 = 57.969f * t2;
    sp3C.unk18 = 42.714f * t2;
    sp3C.unk1C = 118.763f * t2;
    func_15149550(&sp3C, 0xA, 1, 0, arg0->unkC, arg0->unk1);
    arg0->unkE = (func_150ADA20() % 51U) + 25;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2C00.s")
s32 func_150C2FCC(f32 *arg0) {
    arg0[11] -= arg0[11] * arg0[84];
    arg0[12] -= arg0[12] * arg0[84];
    arg0[14] += (arg0[20] * D_800BE9A4) + (((0.5f * arg0[19]) * D_800BE9A4) * D_800BE9A4);
    arg0[20] += arg0[19] * D_800BE9A4;
    if ((arg0[11] < 10.0f) || (arg0[12] < 10.0f)) {
        return 0;
    }
    return 1;
}
s32 func_15144B34(s32);

s32 func_150C308C(void *arg0) {
    u8 flag;
    f32 tmp1;
    u8 sp1F;
    f32 x;
    f32 z;
    f32 magnitude;
    u8 *vector;

    flag = 0;
    if (*(s16 *)((u8 *)arg0 + 0x1C) >= 6) {
        sp1F = 0;
        vector = (u8 *)func_15144B34(D_80082FA4);
        x = *(f32 *)vector;
        tmp1 = *(f32 *)(vector + 8);
        z = (magnitude = tmp1);
        tmp1 = x;
        magnitude = tmp1 * tmp1 + magnitude * z;
        if (magnitude < D_800A0310) {
            flag = 1;
        } else {
            flag = sp1F;
            magnitude = *(f32 *)vector;
            if (D_800A0314 <
                func_15144C8C(func_150484A0(magnitude,
                                           *(f32 *)(vector + 8)),
                              *(f32 *)((u8 *)arg0 + 0x160))) {
                flag = 1;
            }
        }
    }
    if (flag != 0) {
        *(s16 *)((u8 *)arg0 + 0x1C) = 5;
    }
    return 1;
}
// ? func_150C308C(void *arg0) {
//     u8 sp1F;
//     f32 temp_f0;
//     f32 temp_f2;
//     u8 temp_a1;
//     void *temp_v0;
//     u8 phi_a1;
//
//     phi_a1 = (u8)0U;
//     if ((s32) arg0->unk1C >= 6) {
//         sp1F = 0;
//         temp_v0 = func_15144B34(D_80082FA4, (u8)0U);
//         temp_f0 = temp_v0->unk0;
//         temp_f2 = temp_v0->unk8;
//         temp_a1 = sp1F;
//         if (((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < D_800A0310) {
// block_4:
//             phi_a1 = (u8)1U;
//         } else {
//             sp1F = temp_a1;
//             phi_a1 = temp_a1;
//             if (D_800A0314 < func_15144C8C(func_150484A0(temp_v0->unk0, temp_v0->unk8, temp_a1), arg0->unk160)) {
//                 goto block_4;
//             }
//         }
//     }
//     if (phi_a1 != 0) {
//         arg0->unk1C = (u16)5;
//     }
//     return 1;
// }

Gfx *func_150C3160(Gfx *arg0, void *arg1) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a0;
    f32 temp_f0;

    temp_v0 = *(s32 *)((s32)arg1 + 0x2E8);
    temp_f0 = (temp_v0 != 0) ? ((f32)*(s32 *)((s32)arg1 + 0x2E4) / (f32)temp_v0) : 1.0f;
    temp_f0 = 1.0f - temp_f0;
    temp_v1 = (s32)((500.0f * temp_f0) + 2.0f);
    temp_a0 = 2 - *(s32 *)((s32)arg1 + 0x2EC);
    *(s32 *)((s32)arg1 + 0x2EC) = temp_v1 / 3;
    while (temp_a0 < 0) {
        temp_a0 += 0x40;
    }
    gDPSetTileSize(arg0++, 4, temp_v1, temp_a0, 0x1FE, 0x03E);
    return arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3230.s")
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);
f32 func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_15056B08(struct127 *arg0);
void func_150562FC(struct127 *arg0);
u8 func_150599C8(struct127 *arg0, u8 arg1, u16 arg2);
void func_1001091C(struct127 *arg0, s32 arg1);

void func_150C3574(struct127 *arg0) {
    f32 accel;
    f32 dist;
    s32 vol;
    f32 scale;
    u16 angle;

    accel = 0.25f;
    scale = 0.5f;
    arg0->unk80 = 10;
    if (arg0->id == 0x25) {
        func_1505A250(0.0f, 0.0f, 0.150000006f, &arg0->unk164, &arg0->unk168);
        accel = 0.1000000015f;
        if (arg0->unk87 & 1) {
            arg0->unk224 = (s32)arg0->y_position;
        }
        *(s16 *)&arg0->pad160 = 0;
    }
    func_1505A3A8(0.0f, arg0, 2.0f, accel, 0);
    arg0->gravity = 3.0f;
    if (arg0->id == 0x10) {
        scale = 0.375f;
    }
    if (arg0->id == 0xAC) {
        scale = 1.5f;
    }
    scale *= 0.75f;
    func_15056B08(arg0);
    if (arg0->unk13D != 0) {
        func_150562FC(arg0);
    }
    func_15059140(arg0);
    *(f32 *)&arg0->padBC = (arg0->x_position - arg0->old_x_position) * D_800BE9A8;
    arg0->unk148 = (arg0->z_position - arg0->old_z_position) * D_800BE9A8;
    dist = sqrtf(*(f32 *)&arg0->padBC * *(f32 *)&arg0->padBC + arg0->unk148 * arg0->unk148);
    if ((dist + 4.0f) + (dist + 4.0f) < arg0->xz_velocity) {
        arg0->xz_velocity = dist + dist;
    }
    if (arg0->unk87 == 4) {
        D_800D1580 = 0xFF020133;
        D_800D154C = D_800CC2D0;
        func_1506E8D8();
        D_800D154C = arg0;
        arg0->unk164 = *(f32 *)&arg0->padBC;
        arg0->unk168 = arg0->unk148;
        arg0->xz_velocity = 0.0f;
    }
    arg0->unk87 *= 2;
    if ((dist > 0.5f) && (arg0->unk13D == 0)) {
        arg0->unk78 = func_1505A630(arg0->x_position - arg0->old_x_position, arg0->old_z_position - arg0->z_position, 0);
    }
    if (((arg0->unk7A - arg0->unk78) - 0x4000) & 0x8000) {
        angle = arg0->unk78;
    } else {
        angle = arg0->unk78 ^ 0x8000;
    }
    func_150599C8(arg0, 2, angle);
    if (arg0->unk28 > 30.0f) {
        arg0->unk44 *= 0.8999999762f;
    } else {
        arg0->unk44 = dist + dist;
    }
    if (arg0->unk13D != 0) {
        arg0->unk44 = 0.0f;
    }
    func_15052490(arg0, arg0->unk78, arg0->unk44, (2.0f - arg0->xz_scale) * scale);
    if (arg0->unk44 < 0.200000003f) {
        func_100109D0(arg0);
    } else if (func_100107F8(arg0) == 0) {
        if (arg0->id == 0xAC) {
            D_800D1580 = 0x630;
        } else if (arg0->id == 0x25) {
            D_800D1580 = 0x67A;
        } else {
            D_800D1580 = 0x62A;
        }
        func_1506E5FC();
    } else {
        vol = arg0->unk44 * 2500.0f;
        if (vol > 0x6D60) {
            vol = 0x6D60;
        }
        if (arg0->id == 0x25) {
            vol >>= 1;
        }
        func_1001091C(arg0, vol);
    }
    if (arg0->unk44 < 0.200000003f) {
        func_100109D0(arg0);
    }
}
void func_15188810(struct127 *arg0, s32 arg1, s32 arg2);
void func_150C1660(f32 arg0, f32 arg1, f32 arg2, u8 arg3);
void func_1506AC8C(struct127 *arg0, s32 arg1, s32 arg2);
extern f32 D_800A0334;
void func_150C3D5C(Struct150C3D5CArg *arg0);

void func_150C3994(struct127 *arg0) {
    f32 temp_f0;
    f32 temp_f2;

    if (arg0->interaction_state == 0x14) {
        if (arg0->unk2E4 == 0) {
            func_151B9BF0(0x2F, 1, (s16)(s32)arg0->x_position, (s16)(s32)(arg0->y_position + 30.0f), (s32)arg0->z_position, 0x7D0, 0x3E8, 0x190, 0xC8, 0xC00, 0xC00, 1, 1, 0, 0xFF, 0);
        } else {
            func_15188810(arg0, 0x64, 0);
        }
    }
    temp_f0 = arg0->y_velocity;
    if (temp_f0 < 0.0f) {
        temp_f2 = arg0->unk28;
        if ((temp_f2 > 20.0f) && (*(u8 *)((u8 *)arg0 + 0x136) == 0) && (temp_f2 < (-20.0f * temp_f0))) {
            func_10010630(0x92, arg0, 0x7D00, 0xC8, 0x9C4);
            *(u8 *)((u8 *)arg0 + 0x136) = 1;
        }
    }
    if (arg0->unk83 == 0) {
        arg0->unk148 = 70.0f;
        if (arg0->unk2E4 == 0) {
            func_151B9BF0(0x2F, 1, (s16)(s32)arg0->x_position, (s16)(s32)(arg0->y_position + 30.0f), (s32)arg0->z_position, 0x898, 0x500, 0x320, 0x2BC, 0xC00, 0xC00, 0x14, 1, 0, 0xFF, 0);
            func_150C1660(arg0->x_position, arg0->y_position + 30.0f, arg0->z_position, 0xFF);
            func_1506AC8C(arg0, 6, 0);
        } else {
            func_15188810(arg0, 0x64, 0);
        }
        func_10010344(0x1A9, arg0, 0x7FFF, 0x3E8, 0x7D0);
        arg0->unkD0 = 1;
        arg0->unk114 = 500.0f;
        arg0->unkF8 |= 0x01008000;
    }
    arg0->immune = 0xFF;
    arg0->unk83++;
    if (arg0->unk83 >= 0xB) {
        arg0->unkF8 &= ~0x8000;
    }
    arg0->unk40 = (f32)(arg0->unk7A + 0x4000) * 0.005493164f;
    func_15059140(arg0);
    arg0->unkB8 += arg0->unk148;
    arg0->unk148 *= D_800A0334;
    if (((arg0->unk28 == 0.0f) && (arg0->unk83 >= 0x15) && (arg0->unkCC == 0)) || (arg0->unk107 != 0) || (arg0->health == 0)) {
        func_150C3D5C((Struct150C3D5CArg *)arg0);
        func_10010630(0x93, arg0, 0x7D00, 0xC8, 0x9C4);
        func_15060F28(arg0, 1);
    }
}

s32 func_150C3D48(s32 arg0) {
    return arg0 + 0xEDCBA988;
}

void func_150C3D5C(Struct150C3D5CArg *arg0) {
    Struct150C3D5CLocals local;

    func_151B9BF0(0x2F, 1, (s16)(s32)arg0->unk14, (s16)(s32)arg0->unk18,
                  (s32)arg0->unk1C, 0x578, 0x500, 0x320, 0x4B0, 0x200, 0x200,
                  0x1E, 1, 0, 0xFF, 0);

    ((void (*)(s32, s32 *))func_151EF040)(func_150C3D48(local.check = 0xC23459B8), &local.check);
    if (local.check != 0x0330C820) {
        func_10004074[0] = 0x03E00008;
        func_10004074[1] = 0;
    }

    func_15171200(3, 0xCB, arg0->unk14, arg0->unk18, arg0->unk1C, 10.0f, 1.0f,
                  2.0f, 60.0f, 3, 0x3C, D_800A0338, 0xFF, 0);
    func_151709B4((s32)arg0->unk14, (s32)(arg0->unk18 + 20.0f),
                  (s32)arg0->unk1C, 0xA, 2000.0f, 0xFF, 0);
    func_1516D99C((s16)(s32)arg0->unk14, (s16)(s32)arg0->unk18,
                  (s16)(s32)arg0->unk1C, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
                  0, 0xFF, 0, 0, 0, 0x14, 0x14, 2, 0, 0, 0, 0, 0x51, 0x51,
                  0x51, 0x51, 0xA, 1, 0, 0xFF, 0xA, 0x578, 0x190, 1, 1, 0, 1,
                  0, 0, 0, 0, 0, 0xFF, 0);

    local.header.unk0 = 3;
    local.header.unk1 = -1;
    local.header.unk2 = (func_150ADA20() % 11U) + 0xA;
    local.header.unk4 = 0;
    local.header2.unk0 = (s32)arg0->unk14;
    local.header2.unk4 = (s32)arg0->unk18;
    local.header2.unk8 = (s32)arg0->unk1C;
    func_151602C0(&local.header, &local.header2, (func_150ADA20() % 52U) + 0x28,
                  0xFF, 0xFF, 0x5A, 0xFF, 0, 0, 0xFF, 1);
}
