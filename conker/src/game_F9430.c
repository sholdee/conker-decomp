#include <ultra64.h>
#define func_1513D668 func_1513D668_void_proto
#include "functions.h"
#undef func_1513D668
s32 func_1513D668(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, f32 arg7, f32 arg8, s32 arg9, s32 argA, u8 argB, s32 argC, u8 argD, s32 argE);
#include "variables.h"

extern f32 D_800A06DC;
extern f32 D_800A06CC;
extern f32 D_800A06D0;
extern f32 D_800A06D4;
extern f32 D_800A06D8;
extern f32 D_800A06E0;
extern f32 D_800A06E4;
extern f32 D_800A06E8;
extern f32 D_800A06EC;
extern f32 D_800A06F0;
extern f32 D_800A06F4;
extern f32 D_800A06F8;

void func_15143794(s32, s32, f32, f32 *);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    f32 unk4[3];
    u8 unk10;
    u8 unk11;
    s16 unk12;
    s16 unk14;
    u8 pad16[2];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 pad2A[2];
} CBF80Entry;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[0x7];
} CBF80Ground;

typedef struct {
    s16 unk0;
    f32 unk4;
} CBF80Extra;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} CBF80Col;

typedef struct {
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
    struct17 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
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
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} CBF80Main;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} CBF80Rgb;

typedef struct {
    f32 x, y, z;
} CBF80Vec;

typedef struct {
    CBF80Vec unk0;
    u8 unkC;
    s8 unkD;
    u8 padE[2];
    f32 unk10;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    s16 unk20;
    u8 pad22[2];
    f32 unk24;
    f32 unk28;
    s8 unk2C;
    s8 unk2D;
    u8 pad2E[2];
} CBF80Fx;

extern CBF80Entry D_800A0640[];
extern f32 D_800A06C4;
extern f32 D_800A06C8;
void func_1504715C(CBF80Ground *, void *);
s32 func_15046C80(f32 *, s32, f32, CBF80Ground *);
void func_15143134(void *, f32 *, s32);
void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
void func_150C88D0(void *, s32, s32, s32);
void *memcpy(void *, void *, s32);

void func_150CBF80(void *arg0, u8 arg1, s32 arg2, u8 arg3) {
    f32 pos[3];
    CBF80Entry *e;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    CBF80Ground ground;

    if (arg0 == NULL || *(s32 *)((u8 *)arg0 + 0x1D4) == 0) {
        return;
    }
    func_1504715C(&ground, arg0);
    e = &D_800A0640[arg1];
    func_15143134(e->unk4, pos, *(s32 *)((u8 *)arg0 + 0x1D4) + (e->unk1 << 6));
    {
        f32 ray[3];

        ray[0] = pos[0];
        ray[1] = pos[1] + 200.0f;
        ray[2] = pos[2];
        if (func_15046C80(ray, 0, pos[1] - 2000.0f, &ground) == 0) {
            return;
        }
    }
    pos[1] = ground.unk00;
lbl:
    func_150CCD90(pos[2], &fr, &fg, &fb);
    r = (u8)fr;
    g = (u8)fg;
    b = (u8)fb;
    {
        CBF80Extra extra;
        CBF80Main mn;
        s16 spd;
        CBF80Col cols[4];
        s32 ret;

        cols[0].unk0 = r;
        cols[0].unk2 = g;
        cols[0].unk4 = b;
        cols[0].unk6 = 0xFF;
        cols[0].unk8 = 0;
        cols[1].unk0 = r;
        cols[1].unk2 = g;
        cols[1].unk4 = b;
        cols[1].unk6 = 0xFF;
        cols[1].unk8 = 0;
        cols[2].unk0 = r;
        cols[2].unk2 = g;
        cols[2].unk4 = b;
        cols[2].unk6 = 0xFF;
        cols[2].unk8 = 0;
        cols[3].unk0 = r;
        cols[3].unk2 = g;
        cols[3].unk4 = b;
        cols[3].unk6 = 0xFF;
        cols[3].unk8 = 0;
        mn.unk0 = e->unk0;
        mn.unk2 = 0x2601;
        mn.unk4 = (func_150ADA20() % 51U) + 0x84;
        mn.unk8 = 0;
        mn.unkC = 0;
        mn.unk10 = 0xFF;
        mn.unk11 = 0xFF;
        mn.unk12 = 0xFF;
        mn.unk13 = 0xFF;
        mn.unk14 = mn.unk18 = (func_150ADA68() * e->unk1C) + e->unk18;
        mn.unk1C.unk0 = pos[0];
        mn.unk1C.unk4 = pos[1] + 10.0f;
        mn.unk1C.unk8 = pos[2];
        mn.unk28 = 0.0f;
        mn.unk2C = 0.0f;
        mn.unk30 = 0.0f;
        mn.unk34 = 1.0f;
        mn.unk38 = 1.0f;
        mn.unk3C = 1.0f;
        mn.unk40 = 0x1C0001;
        mn.unk44 = (func_150ADA20() % (u32)(e->unk11 + 1)) + e->unk10;
        mn.unk45 = 0xFF;
        mn.unk46 = 0;
        mn.unk47 = 6;
        extra.unk0 = (mn.unk4 - (func_150ADA20() % (u32)(e->unk14 + 1))) - e->unk12;
        extra.unk4 = ((func_150ADA68() * e->unk24) + e->unk20) * mn.unk14;
        ret = func_1513D668((s32)&mn, (s32)cols, 0, 0x18, 0, 0,
                            (s16)((*(u16 *)((u8 *)arg0 + 0x7A) >> 8) + e->unk28),
                            500.0f, 500.0f, 0, (s32)&ground.unk04, 1, 8, arg3, arg2);
        if (ret != 0) {
            memcpy((u8 *)ret + 0x128, &extra, 8);
        }
        {
            CBF80Fx fx;
            f32 d;

            spd = (func_150ADA20() % 1U) + 0x29;
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 50.0f) + 80.0f,
                          0, 0xFF, spd, 0x11, 0, 0.0f, 0, arg3);
            d = D_800A06C4;
lbl2:
            {
                s16 n1;
                s16 n2;

                fx.unk0 = *(CBF80Vec *)pos;
                fx.unk10 = D_800A06C8;
                fx.unkC = 1;
                fx.unkD = -1;
                fx.unk14 = 0xC;
                fx.unk18 = 0x28;
                fx.unk1A = 0x1E;
                n1 = 10;
                fx.unk1C = d / (f32)n1;
                fx.unk20 = 0x19;
                fx.unk24 = d / fx.unk20;
                fx.unk2C = -1;
                fx.unk2D = -1;
                fx.unk28 = 0.0f;
                func_150C88D0(&fx, 0, 0xFF, 1);
            }
        }
        {
            CBF80Rgb rgb;

            spd = (func_150ADA20() % 14U) + 0xF;
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 70.0f) + 70.0f,
                          0, 0xFF, spd, 0x12, 0, 0.0f, (s32)&rgb, arg3);
        }
        func_151D5404((struct17 *)pos, 500.0f, 5000.0f, 0.0002f, 0xC, 0xF, 0xFF, 0);
    }
}

extern f32 D_800BE9A4;

typedef struct {
    s16 unk0;
    f32 unk4;
} CC638Sub;

typedef struct {
    char pad0[0x1C];
    s16 unk1C;
    char pad1E[0x2C - 0x1E];
    f32 unk2C;
    f32 unk30;
    char pad34[0x58 - 0x34];
    s32 unk58;
    u8 unk5C;
    char pad5D[0x128 - 0x5D];
    CC638Sub unk128;
} CC638Struct;

typedef struct {
    char pad_0[0x58];
    f32 field_0x58;
    char pad_0x5C[0x60 - 0x5C];
    f32 field_0x60;
    char pad_0x64[0xA8 - 0x64];
    f32 field_0xA8;
} EffectScaleState;

s32 func_150CC638(CC638Struct *arg0) {
    CC638Sub *sub;

    if (arg0->unk58 & 1) {
        sub = &arg0->unk128;
        if (arg0->unk1C < 0x20) {
            if ((arg0->unk1C << 3) < arg0->unk5C) {
                *(u8*)((u8*)sub - 0xCC) = arg0->unk1C << 3;
            }
        }
        if (sub->unk0 < arg0->unk1C) {
            f32 v = sub->unk4 * D_800BE9A4;
            arg0->unk2C = arg0->unk2C + v;
            arg0->unk30 = arg0->unk30 + v;
        }
    }
    return 1;
}

s32 func_150CC6B8(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    struct260 *ret;
    struct {
        s32 temp;
        u8 pad28[4];
        f32 sp2C;
        u8 pad30[4];
        struct {
            s32 unk00;
            s32 unk04;
            s16 unk08;
            s16 unk0A;
            s32 unk0C;
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
            f32 unk30;
            f32 unk34;
            f32 unk38;
            u8 pad3C[0xC];
            f32 unk48;
            f32 unk4C;
            f32 unk50;
            f32 unk54;
            s32 unk58;
            u8 pad5C[4];
            u8 unk60;
            u8 unk61;
            u8 unk62;
            s8 unk63;
        } sp34;
        u8 pad98[0xC];
    } stack;

    stack.sp34.unk1D = 0x29;
    stack.sp34.unk08 = 0xE03;
    stack.sp34.unk00 = 0x200005;
    stack.sp34.unk04 = 0;
    stack.sp34.unk0A = (func_150ADA20() & 0xF) + 0x14;
    stack.sp34.unk0C = 0;
    stack.sp34.unk10 = 0;
    stack.sp34.unk18 = 0xB0;
    stack.sp34.unk19 = 0xA0;
    stack.sp34.unk1A = 0x2A;
    stack.sp34.unk14 = 0x40;
    stack.sp34.unk15 = 0xB;
    stack.sp34.unk16 = 0x6A;
    stack.sp34.unk17 = 0xFF;

    stack.sp34.unk1B = (func_150ADA20() % 0x9CU) + 0x64;
    stack.sp34.unk1C = 0xFF;
    stack.sp34.unk60 = 3;
    stack.sp34.unk61 = 3;

    stack.sp34.unk28 = stack.sp34.unk2C = (func_150ADA68() * D_800A06CC) + 300.0f;
    stack.sp34.unk30 = arg2;
    stack.sp34.unk34 = arg3;
    stack.sp34.unk38 = arg4;

    stack.temp = func_150ADA20();
    func_15143794(arg8, (s16)((stack.temp % 0x15U) - 0x19),
                  (func_150ADA68() * 200.0f + 350.0f) * D_800A06D0,
                  &stack.sp34.unk48);

    stack.sp34.unk54 = 0.0f;
    stack.sp34.unk58 = 0xE05;

    if (func_150ADA20() & 1) {
        stack.sp34.unk58 |= 0x40;
    }

    if (func_150ADA20() & 1) {
        stack.sp34.unk58 |= 0x80;
    }

    stack.sp34.unk62 = 8;
    stack.sp34.unk24 = D_800A06D4;
    stack.sp2C = D_800A06D8;
    stack.sp34.unk63 = -1;
    stack.sp34.unk1E = 0xA;
    stack.sp34.unk20 = 0x19;
    stack.sp34.unk22 = 0x1B;

    ret = func_15130374(&stack.sp34, 1, 4, arg14, 1);
    if (ret != 0) {
        memcpy((u8 *)ret + 0xA8, &stack.sp2C, 4);
    }

    return 1;
}

s32 func_150CC8D4(EffectScaleState *arg0, s32 arg1) {
    f32 *scale;
    s32 i;

    scale = &arg0->field_0xA8;
    for (i = D_800BE9E4; i != 0; i--) {
        arg0->field_0x58 *= *scale;
        arg0->field_0x60 *= *scale;
    }
    return 1;
}

s32 func_150CCA7C(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    struct {
            f32 unk00;
            f32 unk04;
            f32 unk08;
            f32 unk0C;
            f32 unk10;
            f32 unk14;
            f32 unk18;
            f32 unk1C;
            f32 unk20;
            f32 unk24;
            f32 unk28;
            f32 unk2C;
            f32 unk30;
            f32 unk34;
            f32 unk38;
            f32 unk3C;
            f32 unk40;
            f32 unk44;
            f32 unk48;
            f32 unk4C;
            s32 unk50;
            s16 unk54;
            s16 unk56;
            u8 unk58;
            u8 pad59[3];
            s32 unk5C;
            u8 unk60;
            u8 unk61;
            u8 unk62;
            u8 unk63;
            u8 unk64;
            u8 unk65;
            u8 unk66;
            u8 unk67;
            u8 unk68;
            u8 pad69;
            u8 unk6A;
            u8 pad6B;
            s32 unk6C;
            u8 unk70;
            u8 pad71;
            s16 unk72;
            s16 unk74;
            u8 pad76[0xA];
    } sp28;

    sp28.unk28 = arg2;
    sp28.unk2C = arg3;
    sp28.unk1C = 1.0f;
    sp28.unk20 = 1.0f;
    sp28.unk24 = 1.0f;
    sp28.unk50 = 0x29E8;
    sp28.unk10 = 0.0f;
    sp28.unk14 = 0.0f;
    sp28.unk18 = 0.0f;
    sp28.unk44 = 0.0f;
    sp28.unk00 = 1.0f;
    sp28.unk56 = 0x20;
    sp28.unk30 = arg4;
    sp28.unk04 = D_800A06DC;

    ((s32 *)&sp28)[-2] = func_150ADA20();
    func_15143794(arg8, (s16)((((s32 *)&sp28)[-2] % 0x2CU) - 0x3F),
                  (func_150ADA68() * 200.0f + 300.0f) * D_800A06E0,
                  &sp28.unk34);

    sp28.unk40 = (func_150ADA68() * 260.0f + -130.0f) * D_800A06E4;
    sp28.unk48 = (func_150ADA68() * 260.0f + -130.0f) * D_800A06E8;
    sp28.unk54 = (func_150ADA20() % 0x33U) + 0x32;
    sp28.unk4C = (func_150ADA68() * D_800A06EC + D_800A06F0) * D_800A06F4;

    sp28.unk0C = (func_150ADA68() * 500.0f + 100.0f) * D_800A06F8;
    sp28.unk08 = sp28.unk0C;
    sp28.unk58 = 0;
    sp28.unk5C = 0;

    sp28.unk60 = (func_150ADA20() % 0x65U) + 0x9B;
    sp28.unk61 = 3;
    sp28.unk62 = 0;
    sp28.unk63 = 0;
    sp28.unk64 = 0;
    sp28.unk65 = 0;
    sp28.unk66 = 0;
    sp28.unk67 = 0;
    sp28.unk68 = 0;
    sp28.unk6A = 2;
    sp28.unk6C = 0;
    sp28.unk70 = 0;
    sp28.unk72 = 0x20;
    sp28.unk74 = 7;

    func_15132A4C(&sp28, 3, 0xFF, 0, arg14, 0);
    return 1;
}

s32 func_150CCCB4(f32 *arg0) {
    arg0[14] += arg0[17] * D_800BE9A4;
    arg0[15] += (arg0[18] * D_800BE9A4) + (((arg0[23] * D_800BE9A4) * D_800BE9A4) * 0.5f);
    arg0[16] += arg0[19] * D_800BE9A4;
    arg0[18] += arg0[23] * D_800BE9A4;
    arg0[8] += arg0[20] * D_800BE9A4;
    arg0[9] += arg0[21] * D_800BE9A4;
    arg0[10] += arg0[22] * D_800BE9A4;
    return 1;
}
