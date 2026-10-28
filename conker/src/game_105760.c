#include <ultra64.h>
/* functions.h types func_150ADA20 as returning u8; golden spills/uses the full u32 return
 * (same shadow-prototype idiom as game_121A20.c). */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_150D82B0(s32 arg0) {
}

extern s32 D_80090298[];
extern f32 D_800A0B10; /* 1/17; stays in asm/data/2455D0.rodata.s (shared with func_150D83D8/func_150D85AC) */

#pragma GLOBAL_ASM("asm/nonmatchings/game_105760/func_150D82BC.s")

typedef struct Draw150D83D8 {
    u8 pad0[0x18];
    s16 unk18;
    u8 pad1A[0xA];
    Gfx **unk24;
    u8 pad28[0x10];
    s32 unk38;
    s32 unk3C;
} Draw150D83D8;

extern f32 D_800A0B14; /* 0.01f; asm/data/2455D0.rodata.s */

#pragma GLOBAL_ASM("asm/nonmatchings/game_105760/func_150D83D8.s")

s16 *func_150D8590(s16 *arg0, s32 arg1) {
    arg0[0] = 0x42;
    arg0[1] = 0;
    return arg0 + 2;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_150D85AC;

typedef struct {
    f32 unk0;
    u8 unk4[0x18];
    u8 unk1C;
    u8 unk1D;
    s32 unk20;
} Ground_150D85AC;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3F_150D85AC unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    u8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} Struct150D85AC;

void func_1504715C(Ground_150D85AC *, struct127 *);
s32 func_15046C80(Vec3F_150D85AC *, s32, f32, Ground_150D85AC *);
void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150E83AC(Vec3F_150D85AC *, s16, u8, s32);
void func_15153634(Struct150D85AC *, u8, u8, s32);

void func_150D85AC(struct127 *arg0) {
    Vec3F_150D85AC pos;

    pos.x = arg0->x_position;
    pos.y = arg0->unk180;
    pos.z = arg0->z_position;
    {
        Vec3F_150D85AC probe;
        Ground_150D85AC ground;

        probe.x = pos.x;
        probe.y = pos.y + 100.0f;
        probe.z = pos.z;
        func_1504715C(&ground, arg0);
        {
            Struct150D85AC sp48;

            {
                union { u32 w; f64 _a; } r1;
                f32 sp3C;

                if (func_15046C80(&probe, 0, pos.y - 200.0f, &ground) != 0 && (ground.unk1C & 1)) {
                    pos.x = probe.x;
                    pos.y = ground.unk0;
                    pos.z = probe.z;
                    sp3C = func_150ADA68();
                    r1.w = func_150ADA20();
                    func_150E7FEC(sp3C * 109.0f + 140.0f, (u8)((r1.w % 86U) + 170), ground.unk4, &pos,
                                  (func_150ADA20() % 251U) + 500, 0, 1, 0, 0, 0, 0xFF, 0);
                }
                func_150E83AC(&pos, (func_150ADA20() % 62U) + 120, 0xFF, 1);
            }
            sp48.unk0 = 0x1A;
            sp48.unk2 = 7;
            sp48.unk4 = 0x6C;
            sp48.unk6 = 0x5103;
            sp48.unk8 = 0x200005;
            sp48.unkC = 0;
            sp48.unk10 = 0x1E;
            sp48.unk12 = 0xA;
            sp48.unk14 = 0;
            sp48.unk18 = 0;
            sp48.unk1F = 0xFF;
            sp48.unk1C = 0xB9;
            sp48.unk1D = 0xC7;
            sp48.unk1E = 0xC4;
            sp48.unk20 = 0x95;
            sp48.unk21 = 0x91;
            sp48.unk22 = 0x7E;
            sp48.unk23 = 0x64;
            sp48.unk24 = 0x9B;
            sp48.unk25 = 0xFF;
            sp48.unk26 = 0x1E;
            sp48.unk28 = 8;
            sp48.unk2A = 0x1E;
            sp48.unk2C = 1.022876024f;
            sp48.unk30 = 150.0f;
            sp48.unk34 = 298.0f;
            sp48.unk38.x = pos.x;
            sp48.unk38.y = pos.y + 35.0f;
            sp48.unk38.z = pos.z;
            sp48.unk44 = 0;
            sp48.unk46 = -0x10;
            sp48.unk48 = 0xFF;
            sp48.unk4A = 0x14;
            sp48.unk4C = 18.0f;
            sp48.unk50 = 18.0f;
            sp48.unk54 = 0.09300000221f;
            sp48.unk58 = 0.302000016f;
            sp48.unk5C = 0x840E07;
            sp48.unk60 = 0x10;
            sp48.unk61 = -1;
            sp48.unk62 = 8;
            sp48.unk63 = 6;
            sp48.unk64 = 1;
            sp48.unk68 = 0.9054819942f;
            func_15153634(&sp48, 0xFF, 0xFF, 1);
        }
    }
}

struct S150D88AC {
    u8 pad0[0x14];
    u8 *unk14;
    u8 *unk18;
};

s32 func_150D88AC(struct S150D88AC *arg0) {
    if (arg0->unk18[0x6F] != 0) {
        arg0->unk14[0x9] = 0;
    } else {
        arg0->unk14[0x9] = 1;
    }
    return 1;
}

s32 func_150D88E0(void *arg0, struct127 *arg1, u8 arg2) {
    struct {
        f32 z;
        f32 y;
        f32 x;
        s32 temp_a1;
    } sp30;

    switch (arg2) {
    case 3:
        sp30.temp_a1 = 0x12;
        sp30.x = -7.0f;
        sp30.z = 30.0f;
        break;
    case 4:
        sp30.temp_a1 = 0x15;
        sp30.x = -4.0f;
        sp30.z = 35.0f;
        break;
    case 5:
        sp30.temp_a1 = 0xF;
        sp30.x = 5.0f;
        sp30.z = 20.0f;
        break;
    case 6:
        sp30.temp_a1 = 0x18;
        sp30.x = 5.0f;
        sp30.z = 31.0f;
        break;
    }

    sp30.y = 0.0f;
    func_150A7960((f32 *)((u8 *)arg1->unk1D4 + (sp30.temp_a1 << 6)), sp30.x, sp30.y, sp30.z, &sp30.x, &sp30.y, &sp30.z);
    ((f32 *)arg0)[0] = sp30.x;
    ((f32 *)arg0)[1] = sp30.y;
    ((f32 *)arg0)[2] = sp30.z;
    return 1;
}

s32 func_150D8A20(s32 arg0, s32 arg1) {
    return 0x8;
}

extern f32 D_800A0B30;
extern f32 D_800A0B34;
s32 func_150D88E0(void *, struct127 *, u8);
void func_151875E0(f32, f32, f32, s32, s32, s32, f32, f32);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150D8A34(struct127 *arg0, s32 arg1, s32 arg2) {
    struct { f32 x; f32 y; f32 z; } sp34;

    if (arg0->unk1D4 != NULL) {
        func_150D88E0(&sp34, arg0, (u8)arg1);
        func_151875E0(sp34.x, sp34.y, sp34.z, 0x1E, 0xF, 0x7, D_800A0B30, D_800A0B34);
        func_15165F80(-1, (s32)sp34.x, (s32)(arg0->unk180 + 4.0f), (s32)sp34.z, 4, 0x32, 0, 0xFF, 0);
    }
}
