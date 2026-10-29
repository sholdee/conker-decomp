#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


extern u8 D_800AA490;
extern f32 D_800AA4C8;
extern f32 D_800DCA24;
extern void func_15134908(void *, s32, u8, s32);

struct blk12_B8400 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct arg0_B8400 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x2B];
    struct blk12_B8400 unk38;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
};

s32 func_151B8400(struct arg0_B8400 *arg0) {
    s32 temp_v0;
    struct {
        u8 unk0;
        u8 unk1;
        s16 unk2;
        s16 unk4;
        u8 pad6[2];
        s32 unk8;
        s32 unkC;
        u8 unk10;
        u8 unk11;
        u8 unk12;
        u8 unk13;
        f32 unk14;
        f32 unk18;
        struct blk12_B8400 unk1C;
        struct blk12_B8400 unk28;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        s32 unk40;
        u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        s32 unk48;
        u8 unk4C;
        u8 pad4Dh[3];
        s32 unk50;
        s16 unk54;
        s16 unk56;
        s32 unk58;
    } sp50;

    sp50.unk58 = (s32)arg0;
    sp50.unk0 = 0xFF;
    sp50.unk1 = 0;
    sp50.unk2 = 0x5901;
    sp50.unk4 = 0x32;
    sp50.unk8 = 0;
    sp50.unkC = 0;
    sp50.unk10 = 0xFF;
    sp50.unk11 = 0xE6;
    sp50.unk12 = 0xB6;
    sp50.unk13 = 0xFF;
    sp50.unk14 = 9.0f;
    sp50.unk18 = 1.0f;
    sp50.unk1C = sp50.unk28 = arg0->unk38;
    sp50.unk34 = arg0->unk44 * 0.25f;
    sp50.unk38 = arg0->unk48 * 0.25f;
    sp50.unk3C = arg0->unk4C * 0.25f;
    sp50.unk40 = 0x0CCC0000;
    sp50.unk44 = 0xC8;
    sp50.unk45 = 0xFF;
    sp50.unk46 = 0;
    sp50.unk47 = 0x6;
    sp50.unk48 = 0;
    sp50.unk4C = 0xFF;
    sp50.unk50 = 0;
    sp50.unk54 = 0x32;
    sp50.unk56 = 0x5;

    temp_v0 = (s32)func_1513D2F0(&sp50, (s32)&D_800AA490, 0x1B, 0, 0, 0x19, 0, 0, 0, 4,
                                 arg0->unkC, arg0->unk1);
    if (temp_v0 != 0) {
        memcpy((void *)(temp_v0 + 0x110), &sp50.unk58, 4);
    }
    return temp_v0;
}

struct vec3w_151B85AC {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct blk151B85AC {
    u8 pad0[0x38];
    struct vec3w_151B85AC unk38;
};

struct arg0151B85AC {
    u8 pad0[0x34];
    struct vec3w_151B85AC unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[0xB4];
    struct blk151B85AC *unk110;
};

s32 func_151B85AC(struct arg0151B85AC *arg0) {
    struct blk151B85AC *temp_v0;

    temp_v0 = arg0->unk110;
    if (temp_v0 != 0) {
        arg0->unk34 = temp_v0->unk38;
        arg0->unk40 += arg0->unk4C * D_800BE9A4;
        arg0->unk44 += arg0->unk50 * D_800BE9A4;
        arg0->unk48 += arg0->unk54 * D_800BE9A4;
    } else {
        arg0->unk58 |= 1;
    }
    return 1;
}

void func_1513FA70(s32, s32);

void func_151B863C(s32 a0, s16 a1) {
    func_1513FA70(a0, a1);
}

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    f32 unk0C;
    f32 unk10;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 pad19[7];
} Struct151B8668;

void func_151B8668(s32 arg0, u8 arg1, s32 arg2) {
    Struct151B8668 sp18;

    sp18.unk0C = 10.0f;
    sp18.unk00 = arg0 + 0x38;
    sp18.unk04 = arg0 + 0x3C;
    sp18.unk08 = arg0 + 0x40;
    sp18.unk10 = D_800AA4C8 * D_800DCA24;
    sp18.unk14 = 0x12C;
    sp18.unk16 = 0;
    sp18.unk17 = 3;
    sp18.unk18 = 0;
    func_15134908(&sp18, 0, arg1, arg2);
}

void func_15143794(s16, s16, f32, f32 *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150ADA20(void);

void func_151B86F4(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, u8 *a6) {
    f32 sp84[3];
    f32 sp78[3];
    f32 sp6C[3];
    f32 r;

    sp84[0] = a0;
    sp84[1] = a1;
    sp84[2] = a2;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 65U) - 32),
                  func_150ADA68() * 945.0f * 0.00999999977648f, sp78);
    r = (func_150ADA68() * 157.0f + 604.0f) * 0.00100000005f;
    sp6C[0] = 0.0f;
    sp6C[1] = 0.0f;
    sp6C[2] = 0.0f;
    sp78[0] += -a3 * D_800BE9A8 * r;
    sp78[1] += -a4 * D_800BE9A8 * r;
    sp78[2] += -a5 * D_800BE9A8 * r;
    func_151A26EC(sp84, sp6C, sp78, 1.0f,
                  (func_150ADA68() * 157.0f + -151.0f) * 0.00100000005f,
                  func_150ADA68() * 55.0f + 75.0f, (func_150ADA20() % 26U) + 25,
                  (func_150ADA20() % 101U) + 100, 10, 25, 0, -1, 0, 0, 0, a6[0xC], a6[1]);
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Struct151B8908Particle; /* 0x70 */

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x2B];
    struct17 unk38;
} Struct151B8908;

typedef struct {
    s32 v[4];
} Struct151B8908Tbl;

extern Struct151B8908Tbl D_800AA4B8;
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151B8908(Struct151B8908 *arg0) {
    s32 pad;
    Struct151B8908Particle spawn;
    Struct151B8908Tbl tbl;

    tbl = D_800AA4B8;

    spawn.unk1D = tbl.v[func_150ADA20() & 3];
    spawn.unk8 = 0x1303;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkA = 300;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 500.0f + 900.0f;
    spawn.pos = arg0->unk38;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.vel = *(struct17 *)&D_800A5480;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    spawn.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 | 0x40000 | 0x800000;
    spawn.unk60 = 6;
    spawn.unk61 = 5;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    func_15130280(&spawn, 1, 0, 0, arg0->unkC, arg0->unk1);
}
