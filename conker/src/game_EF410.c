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
    /* 0x00 */ struct17 unk0;
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

void func_15151A38(Struct150C2558 *, u8, s32);
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ struct17 unk8;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E[2];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A[2];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ u8 pad41[3];
    /* 0x44 */ f32 unk44;
} Struct150C2700;
void func_15150178(Struct150C2700 *arg0, struct17 *arg1, s32 arg2, u8 arg3, s32 arg4);

void func_150C1F60(u8 arg0) {
    Struct150C2558 params;

    params.unk0.unk0 = 8152.0f;
    params.unk0.unk4 = 40.0f;
    params.unk0.unk8 = -309.0f;
    params.unk1C = 40.92f;
    params.unk20 = 20.45f;
    params.unk0C = 9.0f;
    params.unk10 = 21.9f;
    params.unk24 = 0xC;
    params.unk26 = 0xD;
    params.unk28 = -0xA0;
    params.unk2C = -0x3C;
    params.unk2A = 0x45;
    params.unk2E = 0x43;
    params.unk30 = 3;
    params.unk32 = 5;
    params.unk34 = 0x32;
    params.unk36 = 0x1F;
    params.unk14 = 1.46f;
    params.unk18 = 0.5f;
    params.unk38 = 0xB;
    params.unk39 = 1;
    params.unk3A = 0x28;
    params.unk3C = 1;
    params.unk40 = 4;
    params.unk44 = 0;
    params.unk48 = 0;
    params.unk4C = 0;
    params.unk50 = 0;
    params.unk54 = 0xFF;
    params.unk55 = 0xFF;
    params.unk56 = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.unk60 = 0;
    params.unk64 = 0x220005;
    params.unk68 = 0x1D0600;
    params.unk6C = 1;
    params.unk70 = 0x3B;
    params.unk74 = 0x80;
    params.unk78 = 0x20;
    params.unk7C = 0;
    params.unk7D = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, arg0, 1);
    params.unk1C = 100.0f;
    params.unk20 = 57.5f;
    params.unk0C = 55.6f;
    params.unk10 = 39.0f;
    params.unk24 = 2;
    params.unk26 = 3;
    params.unk28 = -0x6C;
    params.unk2C = -0x16;
    params.unk2A = 0x15;
    params.unk2E = 0x16;
    params.unk30 = 7;
    params.unk32 = 0;
    params.unk34 = 0x30;
    params.unk36 = 0x15;
    params.unk14 = 0.46f;
    params.unk18 = 1.0f;
    func_15151A38(&params, arg0, 1);
    {
    Struct150C2700 params2;
    params2.unk8 = params.unk0;
    params2.unk14 = 0x19;
    params2.unk16 = 0x12;
    params2.unk0 = -0x9F;
    params2.unk2 = 0x44;
    params2.unk4 = -0x34;
    params2.unk6 = 0x26;
    params2.unk20 = 0x19;
    params2.unk22 = 0x19;
    params2.unk2C = 0x9B;
    params2.unk2D = 0x64;
    params2.unk38 = 1;
    params2.unk39 = 6;
    params2.unk3C = 0.0f;
    params2.unk40 = 1;
    params2.unk44 = 0.0f;
    params2.unk18 = 40.0f;
    params2.unk1C = 35.0f;
    params2.unk24 = -2.0210001f;
    params2.unk28 = 0.89400005f;
    params2.unk30 = 1152.0f;
    params2.unk34 = 848.0f;
    func_15150178(&params2, &params2.unk8, 0, arg0, 1);
}
}

void func_150C2290(u8 arg0) {
    Struct150C2558 params;

    params.unk0.unk0 = 8152.0f;
    params.unk0.unk4 = 40.0f;
    params.unk0.unk8 = 309.0f;
    params.unk1C = 100.0f;
    params.unk20 = 57.5f;
    params.unk0C = 55.6f;
    params.unk10 = 39.0f;
    params.unk24 = 2;
    params.unk26 = 3;
    params.unk28 = -0x29;
    params.unk2C = -0x16;
    params.unk2A = 0x15;
    params.unk2E = 0x16;
    params.unk30 = 7;
    params.unk32 = 0;
    params.unk34 = 0x30;
    params.unk36 = 0x15;
    params.unk14 = 0.46f;
    params.unk18 = 1.0f;
    params.unk38 = 0xB;
    params.unk39 = 1;
    params.unk3A = 0x28;
    params.unk3C = 1;
    params.unk40 = 4;
    params.unk44 = 0;
    params.unk48 = 0;
    params.unk4C = 0;
    params.unk50 = 0;
    params.unk54 = 0xFF;
    params.unk55 = 0xFF;
    params.unk56 = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.unk60 = 0;
    params.unk64 = 0x220005;
    params.unk68 = 0x1D0600;
    params.unk6C = 1;
    params.unk70 = 0x3B;
    params.unk74 = 0x80;
    params.unk78 = 0x20;
    params.unk7C = 0;
    params.unk7D = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, arg0, 1);
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

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ u8 pad34[4];
    /* 0x038 */ f32 unk38;
    /* 0x03C */ u8 pad3C[0x10];
    /* 0x04C */ f32 unk4C;
    /* 0x050 */ f32 unk50;
    /* 0x054 */ u8 pad54[0x44];
    /* 0x098 */ struct133 *unk98;
    /* 0x09C */ u8 pad9C[0xB4];
    /* 0x150 */ f32 unk150;
    /* 0x154 */ u8 pad154[0xC];
    /* 0x160 */ f32 unk160;
} Particle_EF410;

void func_1514C470(f32, f32, f32, f32, f32, f32, f32, s32, s32, f32, s32, u8);

void func_150C2424(u8 arg0) {
    func_1514C470(8500.0f, -490.0f, -328.0f, 8500.0f, -490.0f, 328.0f,
                  (func_150ADA68() * 8.0f) + 8.0f, 1, 0, 0.0f, 0, arg0);
    func_1514C470(8843.0f, -560.0f, -580.0f, 8117.0f, -560.0f, -580.0f,
                  (func_150ADA68() * 3.0f) + 4.0f, 3, 0, 0.0f, 0, arg0);
}

s32 func_150C251C(Particle_EF410 *particle) {
    struct133 *sub = particle->unk98;
    s32 alpha = particle->unk1C << 3;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    sub->unk1B = alpha;
    if (sub->unk1B < 0) {
        return 0;
    }
    return 1;
}


s32 func_150C2558(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    Struct150C2558 params;

    params.unk0.unk0 = x;
    params.unk0.unk4 = y;
    params.unk0.unk8 = (z + 15.0f) - (func_150ADA68() * 30.0f);
    params.unk24 = 1;
    params.unk26 = 0;
    params.unk28 = (s32)(params.unk0.unk8 * 0.07012195f) - 0x40;
    params.unk2A = 9;
    params.unk2C = 0;
    params.unk2E = 0;
    params.unk30 = 3;
    params.unk32 = 3;
    params.unk34 = 0x28;
    params.unk36 = 0x15;
    params.unk1C = 200.0f;
    params.unk20 = 150.0f;
    params.unk0C = 22.0f;
    params.unk14 = 0.0f;
    params.unk18 = 0.0f;
    params.unk10 = 44.0f;
    params.unk38 = 0xB;
    params.unk39 = 1;
    params.unk3A = 0x28;
    params.unk3C = 1;
    params.unk40 = 4;
    params.unk44 = 0;
    params.unk48 = 0;
    params.unk4C = 0;
    params.unk50 = 0;
    params.unk54 = 0xFF;
    params.unk55 = 0xFF;
    params.unk56 = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.unk60 = 0;
    params.unk64 = 0x220005;
    params.unk68 = 0x1D0600;
    params.unk6C = 1;
    params.unk70 = 0x3B;
    params.unk74 = 0x80;
    params.unk78 = 0x20;
    params.unk7C = 0;
    params.unk7D = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, argE, 1);
    return 1;
}


s32 func_150C2700(s32 a0, s32 a1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    Struct150C2700 params;

    params.unk8.unk0 = x;
    params.unk8.unk4 = y;
    params.unk14 = 6;
    params.unk16 = 5;
    params.unk0 = 0x6B;
    params.unk2 = 0x46;
    params.unk4 = -0x1F;
    params.unk6 = 0x2E;
    params.unk20 = 0x64;
    params.unk22 = 0;
    params.unk2C = 0x9B;
    params.unk2D = 0x64;
    params.unk34 = 0.0f;
    params.unk38 = 1;
    params.unk39 = 6;
    params.unk3C = 0.0f;
    params.unk40 = 1;
    params.unk44 = 0.0f;
    params.unk8.unk8 = z;
    params.unk18 = 30.0f;
    params.unk1C = 35.0f;
    params.unk24 = -2.328f;
    params.unk28 = 1.6630001f;
    params.unk30 = 1655.0f;
    func_15150178(&params, &params.unk8, 0, argE, 1);
    return 1;
}

typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 delay;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 unk19;
} Struct150C2804;

extern void func_15134908(Struct150C2804 *, s32, u8, s32);

void func_150C2804(f32 *x, f32 *y, f32 *z, s16 delay, s32 arg4, s32 arg5) {
    Struct150C2804 params;

    params.unk0C = 10.3f;
    params.x = x;
    params.y = y;
    params.z = z;
    params.unk10 = 0.5009f;
    params.delay = delay;
    params.unk16 = 5;
    params.unk17 = 6;
    params.unk18 = 3;
    params.unk19 = -1;
    func_15134908(&params, 0, arg4, arg5);
}
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

void func_150C2898(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter) {
    f32 pos[3];
    f32 vel[3];
    f32 speed;
    f32 rand3;
    u32 rand2;
    u32 rand1;
    f32 rand0;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    speed = (func_150ADA68() * 112.0f + 247.0f) * 0.001f;
    vel[0] = -dirX * speed;
    vel[1] = -dirY * speed;
    vel[2] = -dirZ * speed;
    rand0 = func_150ADA68();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    rand3 = func_150ADA68();
    func_151D9014(pos, vel, 6, rand0 * 0.27f + -0.31f,
                    (rand1 & 0xF) + 25, (rand2 % 101) + 155,
                    rand3 * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    emitter->unkC, emitter->unk1);
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

Particle_EF410 *func_15149550(Struct15149550Arg_EF410 *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);

void func_150C29F0(Struct150C29F0 *emitter) {
    PosSub *pos;
    Struct15149550Arg_EF410 particle;
    f32 scaleA;
    f32 scaleB;
    u8 angle;
    f32 cosine;
    f32 sine;
    f32 radius;

    angle = func_150ADA20() & 0xFF;
    cosine = func_151423D8(angle - 0x40);
    sine = func_151423D8(angle);
    pos = &emitter->unk28;
    radius = func_150ADA68() * pos->scale;
    particle.unk20 = 0x6231; particle.unk22 = 0x1A4D; // matching: one source line (-g3 line numbers steer as1 scheduling)
    particle.unk26 = 0;
    particle.unk27 = 0;
    particle.unk28 = 0;
    particle.unk29 = 0xFF;
    particle.unk2C = 0xFF;
    particle.unk2D = 0xFF;
    particle.unk30 = 0;
    particle.unk2A = 1;
    particle.unk2E = 0;
    particle.unk2F = 1;
    particle.unk24 = (func_150ADA20() % 201U) + 100;
    particle.unk34 = 0xF;
    particle.unk38 = 0.0f;
    particle.unk40 = 0.045f;
    particle.unk44 = 0.7f;
    particle.unk0.unk0 = pos->x + radius * cosine;
    particle.unk0.unk4 = pos->y;
    particle.unk0.unk8 = pos->z + radius * sine;
    particle.unk3C = 10000.0f;
    scaleA = (func_150ADA68() * 0.5f) + 1.0f;
    scaleB = (func_150ADA68() * 0.5f) + 1.0f;
    particle.unkC = 22.6f * scaleA;
    particle.unk14 = 13.786f * scaleA;
    particle.unk10 = 57.969f * scaleB;
    particle.unk18 = 42.714f * scaleB;
    particle.unk1C = 118.763f * scaleB;
    func_15149550(&particle, 0xA, 1, 0, emitter->unkC, emitter->unk1);
    emitter->unkE = (func_150ADA20() % 51U) + 25;
}
void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
struct17 *func_15144B34(s32);
void *memcpy(void *, const void *, u32);

void func_150C2C00(Struct150C29F0 *emitter) {
    f32 *timer;
    f32 *pos;
    f32 distSq;
    Struct15149550Arg_EF410 particle;
    f32 scaleA;
    f32 angle;
    f32 scaleB;
    Particle_EF410 *created;

    timer = &emitter->unk28.x;
    *timer += func_150ADA68() * 20.0f * 1e-9f * D_800BE9A4 * 36447764.0f;
    if (*timer > 1.0f) {
        if (1) { // matching: the extra block makes IDO give emitter $s3 and pos $s2
            pos = (f32 *)func_15144B34(D_80082FA4);
            distSq = (pos[2] * pos[2]) + (pos[0] * pos[0]);
            if (2500000.0f < distSq) {
                particle.unk20 = 0x6231;
                particle.unk22 = 0x1A4D;
                particle.unk0.unk4 = -390.0f;
                particle.unk26 = 0;
                particle.unk27 = 0;
                particle.unk28 = 0;
                particle.unk29 = 0xFF;
                particle.unk2D = 0xFF;
                particle.unk2E = 20;
                particle.unk2F = 1;
                particle.unk30 = 4;
                particle.unk2A = 1;
                particle.unk34 = 25;
                particle.unk3C = 0.0f;
                particle.unk40 = 0.045f;
                particle.unk44 = 0.7f;
                do {
                    angle = (func_150ADA68() * 5.8659425f) + 0.957204f;
                    if (func_15144C8C(func_150484A0(pos[0], pos[2]), angle) < 0.4f) {
                        func_1514373C(angle, (func_150ADA68() * 1278.0f) + 3900.0f, &particle.unk0.unk0, &particle.unk0.unk8);
                        scaleA = ((func_150ADA68() * 1991.0f) + 1985.0f) * 0.001f;
                        scaleB = ((func_150ADA68() * 1493.0f) + 3000.0f) * 0.001f;
                        particle.unkC = 22.6f * scaleA;
                        particle.unk14 = 13.786f * scaleA;
                        particle.unk10 = 57.969f * scaleB;
                        particle.unk18 = 42.714f * scaleB;
                        particle.unk1C = 118.763f * scaleB;
                        particle.unk24 = (func_150ADA20() % 151U) + 50;
                        particle.unk38 = (func_150ADA68() * 500.0f) * 0.001f;
                        particle.unk2C = (func_150ADA20() % 156U) + 100;
                        created = func_15149550(&particle, 0xA, 1, 0, emitter->unkC, emitter->unk1);
                        if (created != NULL) {
                            memcpy(&created->unk160, &angle, sizeof(angle));
                        }
                    }
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            } else {
                do {
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            }
        }
    }
}
s32 func_150C2FCC(Particle_EF410 *particle) {
    particle->unk2C -= particle->unk2C * particle->unk150;
    particle->unk30 -= particle->unk30 * particle->unk150;
    particle->unk38 += (particle->unk50 * D_800BE9A4) + (0.5f * particle->unk4C * D_800BE9A4 * D_800BE9A4);
    particle->unk50 += particle->unk4C * D_800BE9A4;
    if ((particle->unk2C < 10.0f) || (particle->unk30 < 10.0f)) {
        return 0;
    }
    return 1;
}
s32 func_150C308C(Particle_EF410 *particle) {
    u8 triggered;
    f32 value; // matching: one local for the squared distance and then the angle (IDO puts both in $f12)
    struct17 *pos;

    triggered = FALSE;
    if (particle->unk1C >= 6) {
        pos = func_15144B34(D_80082FA4);
        value = pos->unk0 * pos->unk0 + pos->unk8 * pos->unk8;
        if (value < 2500000.0f) {
            triggered = TRUE;
        } else {
            value = func_150484A0(pos->unk0, pos->unk8);
            if (0.4f < func_15144C8C(value, particle->unk160)) {
                triggered = TRUE;
            }
        }
    }
    if (triggered) {
        particle->unk1C = 5;
    }
    return 1;
}

Gfx *func_150C3160(Gfx *gdl, struct127 *obj) {
    s32 total;
    s32 uls;
    s32 ult;
    f32 progress;

    total = obj->unk2E8;
    progress = (total != 0) ? ((f32)obj->unk2E4 / (f32)total) : 1.0f;
    progress = 1.0f - progress;
    uls = (s32)((500.0f * progress) + 2.0f);
    ult = 2 - obj->unk2EC;
    obj->unk2EC = uls / 3;
    while (ult < 0) {
        ult += 64;
    }
    gDPSetTileSize(gdl++, 4, uls, ult, 0x1FE, 0x03E);
    return gdl;
}
typedef struct {
    struct127 *unk0;
    f32 unk4[3];
    s32 unk10;
    s32 unk14;
} Struct150C3230;

void func_15059C84(struct127 *arg0);
void func_1505A770(struct127 *arg0);
void func_1507C324(struct127 *arg0, struct127 *arg1);
void func_1506AC8C(struct127 *arg0, s32 arg1, s32 arg2);

void func_150C3230(struct127 *arg0) {
    s32 index;
    s32 type;
    struct127 *target;
    Struct150C3230 sp3C;

    arg0->unkF8 |= 0x4000000;
    if ((arg0->unk222 != 0) && (arg0->unk65 == 0)) {
        arg0->old_x_position = D_800CC2D0[arg0->unk124 - 1].x_position;
        arg0->old_y_position = D_800CC2D0[arg0->unk124 - 1].y_position;
        arg0->old_z_position = D_800CC2D0[arg0->unk124 - 1].z_position;
        arg0->unk222 = 0;
    }

    func_15059C84(arg0);
    func_1505A770(arg0);
    func_15058898(arg0, arg0->old_y_position);
    func_1505B5F8(arg0, arg0->unk180);

    if (arg0->unk28 == 0.0f) {
        arg0->xz_velocity *= 0.5f;
    }

    index = arg0->unk65;
    if (index != 0) {
        arg0->unk222 = index;
        if (D_800CC2D0[index - 1].unk84.uh == 0x1F9) {
            type = 2;
        } else if (D_800CC2D0[index - 1].unk84.uh == 0x1FB) {
            type = 3;
        } else {
            typedef s32 Type150C3230;
            type = 1;
        }
    } else {
        type = 0;
    }

    func_1505E650(arg0, type, 0.0f, 0.0f, 0.0f, 0.0f, 0);

    if (arg0->unk65 != 0) {
        func_1507C324(arg0, &D_800CC2D0[arg0->unk65 - 1]);
    }

    if (arg0->unk2E4 != 0) {
        if (arg0->unk65 != 0) {
            target = &D_800CC2D0[arg0->unk65 - 1];
            if (target->interaction_state == 5) {
                arg0->unk65 = 0;
                arg0->unk2E4 = 0;
            }
        } else {
            target = NULL;
        }

        if ((D_800BE616 == 0) && (D_800C35EA == 0) && (arg0->unk65 == 0)) {
            arg0->unk2E4 = 0;
        }

        if (D_800BE9E4 < arg0->unk2E4) {
            arg0->unk2E4 -= D_800BE9E4;
            if (D_800BE616 != 0) {
                func_1508B20C(arg0->x_position, arg0->y_position, arg0->z_position, 900.0f);
            }
        } else {
            sp3C.unk4[0] = arg0->x_position;
            sp3C.unk4[1] = arg0->y_position;
            sp3C.unk4[2] = arg0->z_position;
            sp3C.unk0 = &D_800CC2D0[arg0->unk124 - 1];
            sp3C.unk10 = arg0->unk65;
            if (target != NULL) {
                target->immune = target->stunned = 0;
                if (target->health != 0) {
                    target->health = 1;
                }
            }
            func_1506AC8C(arg0, (D_800BE616 != 0) ? 0xB : 0xE, (s32)&sp3C);
            func_15060F28(arg0, 1);
        }
    }
}
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
        func_1505A250(0.0f, 0.0f, 0.15f, &arg0->unk164, &arg0->unk168);
        accel = 0.1f;
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
        arg0->unk44 *= 0.9f;
    } else {
        arg0->unk44 = dist + dist;
    }
    if (arg0->unk13D != 0) {
        arg0->unk44 = 0.0f;
    }
    func_15052490(arg0, arg0->unk78, arg0->unk44, (2.0f - arg0->xz_scale) * scale);
    if (arg0->unk44 < 0.2f) {
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
    if (arg0->unk44 < 0.2f) {
        func_100109D0(arg0);
    }
}
void func_15188810(struct127 *arg0, s32 arg1, s32 arg2);
void func_150C1660(f32 arg0, f32 arg1, f32 arg2, u8 arg3);
void func_1506AC8C(struct127 *arg0, s32 arg1, s32 arg2);
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
    arg0->unk148 *= 0.96f;
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
                  2.0f, 60.0f, 3, 0x3C, -0.7f, 0xFF, 0);
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
