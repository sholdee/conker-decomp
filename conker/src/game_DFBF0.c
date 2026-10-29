#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_DFBF0/func_150B2740.s")

extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

s32 func_150B2EB4(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 spBC;
    f32 spB8;
    s16 angle;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 temp_f2;
    f32 spA0;
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
        u8 pad76[6];
    } sp2C;

    spBC = func_151423D8(arg8 - 0x40);
    spB8 = func_151423D8((u8)arg8);

    sp2C.unk50 = 0x9E8;
    sp2C.unk10 = 0.0f;
    sp2C.unk14 = 0.0f;
    sp2C.unk18 = 0.0f;
    sp2C.unk1C = 1.0f;
    sp2C.unk20 = 1.0f;
    sp2C.unk24 = 1.0f;
    sp2C.unk28 = arg2;
    sp2C.unk2C = arg3;
    sp2C.unk30 = arg4;
    sp2C.unk04 = 0.200000003f;

    spA0 = (func_150ADA68() * 31.0f) + 18.0f;
    angle = (arg9 - 0x40) + (s16)(60.0f * spB8 * argA * argC);
    spB0 = func_151423D8(angle - 0x40);
    spAC = func_151423D8((u8)angle);
    angle = (s16)(s32)(48.0f * spBC) - 0x14;
    spA8 = func_151423D8(angle - 0x40);
    temp_f2 = func_151423D8((u8)angle);

    sp2C.unk34 = (spA0 * temp_f2) * spB0;
    sp2C.unk38 = -spA0 * spA8;
    sp2C.unk3C = (spA0 * temp_f2) * spAC;

    sp2C.unk40 = (func_150ADA68() * 80.0f) + -40.0f;
    sp2C.unk44 = 0.0f;
    sp2C.unk48 = (func_150ADA68() * 80.0f) + -40.0f;
    sp2C.unk54 = 100;

    sp2C.unk4C = (func_150ADA68() * -1.752000093f) + -0.8370000124f;
    sp2C.unk00 = 1.0f;

    temp_f2 = (func_150ADA68() * 72.5f) + 29.80000114f;
    sp2C.unk56 = 0x1C;
    sp2C.unk58 = 0;
    sp2C.unk08 = temp_f2;
    sp2C.unk5C = 0;
    sp2C.unk60 = 0xFF;
    sp2C.unk61 = 1;
    sp2C.unk62 = 0;
    sp2C.unk63 = 0;
    sp2C.unk64 = 0;
    sp2C.unk65 = 0;
    sp2C.unk66 = 0;
    sp2C.unk67 = 0;
    sp2C.unk68 = 0;
    sp2C.unk6A = 2;
    sp2C.unk6C = 0;
    sp2C.unk70 = 0;
    sp2C.unk72 = 1;
    sp2C.unk74 = 0xFF;
    sp2C.unk0C = temp_f2;

    func_15132A4C(&sp2C.unk00, 3, 0xFF, 0, argE, 0);
    return 1;
}

void *memcpy(void *, const void *, u32);

s32 func_150B3188(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 spF4;
    f32 spF0;
    s16 angle;
    f32 spE8;
    f32 spE4;
    f32 spE0;
    f32 temp_f2;
    f32 speed;
    s16 life;
    f32 minGrav;
    f32 gravRange;
    f32 minSpeed;
    f32 speedRange;
    f32 minScale;
    f32 scaleRange;
    f32 xzAngle;
    f32 yzAngle;
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
        u8 pad76[6];
    } sp38;
    void *ret;
    f32 sp30;

    spF4 = func_151423D8(arg8 - 0x40);
    spF0 = func_151423D8((u8)arg8);

    life = func_151422DC(6, "life", 1, 500, 300, "../Effects/PipeExplode/pipeexplode.c", 0x742);
    minGrav = func_151422DC(7, "min grav", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x750) * 0.001000000047f;
    gravRange = func_151422DC(8, "grav range", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x752) * 0.001000000047f;
    minSpeed = func_151422DC(9, "min speed", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75A);
    speedRange = func_151422DC(10, "speeed range", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75C);
    minScale = func_151422DC(11, "min scale", 0, 5000, 1000, "../Effects/PipeExplode/pipeexplode.c", 0x761) * 0.1000000015f;
    scaleRange = func_151422DC(12, "scale  range", 0, 5000, 0, "../Effects/PipeExplode/pipeexplode.c", 0x763) * 0.1000000015f;
    xzAngle = func_151422DC(13, "xz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x769);
    yzAngle = func_151422DC(14, "yz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x76F);

    sp38.unk50 = 0x29E9;
    sp38.unk1C = 1.0f;
    sp38.unk20 = 1.0f;
    sp38.unk24 = 1.0f;
    sp38.unk10 = 0.0f;
    sp38.unk14 = 0.0f;
    sp38.unk18 = 0.0f;
    sp30 = -18.0f;
    sp38.unk28 = arg2;
    sp38.unk2C = arg3;
    sp38.unk30 = arg4;
    sp38.unk04 = 0.200000003f;

    speed = (func_150ADA68() * speedRange) + minSpeed;
    angle = (arg9 - 0x40) + (s16)(xzAngle * spF0 * argA * argC);
    spE8 = func_151423D8(angle - 0x40);
    spE4 = func_151423D8((u8)angle);
    angle = (s16)(s32)(yzAngle * spF4) - 0x14;
    spE0 = func_151423D8(angle - 0x40);
    temp_f2 = func_151423D8((u8)angle);

    sp38.unk34 = (speed * temp_f2) * spE8;
    sp38.unk38 = -speed * spE0;
    sp38.unk3C = (speed * temp_f2) * spE4;

    sp38.unk40 = (func_150ADA68() * 4.0f) + -2.0f;
    sp38.unk44 = 0.0f;
    sp38.unk48 = (func_150ADA68() * 4.0f) + -2.0f;
    sp38.unk54 = life + 0x20;

    sp38.unk4C = (func_150ADA68() * gravRange) + minGrav;

    temp_f2 = (func_150ADA68() * scaleRange) + minScale;
    sp38.unk08 = temp_f2;
    sp38.unk56 = 0x1C;
    sp38.unk58 = 0;
    sp38.unk5C = 0;
    sp38.unk60 = 0xFF;
    sp38.unk61 = 1;
    sp38.unk00 = temp_f2 * 0.1000000015f;
    sp38.unk62 = 0;
    sp38.unk63 = 5;
    sp38.unk64 = 0;
    sp38.unk65 = 0;
    sp38.unk66 = 0;
    sp38.unk67 = 0;
    sp38.unk68 = 0;
    sp38.unk6A = 2;
    sp38.unk6C = 0;
    sp38.unk70 = 0;
    sp38.unk72 = 0x20;
    sp38.unk74 = 7;
    sp38.unk0C = temp_f2;

    ret = func_15132A4C(&sp38.unk00, 3, 0xFF, 4, argE, 0);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x170, &sp30, 4);
    }
    return 1;
}

struct Obj150B36AC {
    u8 pad0[0x10];
    f32 x10;
    f32 x14;
    u8 pad18[0x3C - 0x18];
    f32 x3C;
    u8 pad40[0x44 - 0x40];
    f32 x44;
    f32 x48;
    f32 x4C;
    f32 x50;
    f32 x54;
    f32 x58;
    u8 pad5C[0x60 - 0x5C];
    s32 x60;
    s16 x64;
    u8 pad66[0x170 - 0x66];
    f32 x170;
};

s32 func_150B36AC(struct Obj150B36AC *arg0, s32 a1, s32 a2, s32 a3, f32 arg5) {
    arg0->x3C = arg0->x10 + arg5;
    if (arg0->x170 < arg0->x48) {
        arg0->x60 &= ~0x7;
        arg0->x60 &= ~0x8;
        arg0->x60 &= ~0x40;
        arg0->x60 &= ~0x20;
        arg0->x44 = 0.0f;
        arg0->x48 = 0.0f;
        arg0->x4C = 0.0f;
        if (arg0->x64 >= 0x21) {
            arg0->x64 = 0x20;
        }
    } else {
        arg0->x44 = arg0->x44 * arg0->x14;
        arg0->x48 = -arg0->x48 * arg0->x14;
        arg0->x4C = arg0->x4C * arg0->x14;
        arg0->x50 = arg0->x50 * arg0->x14;
        arg0->x54 = arg0->x54 * arg0->x14;
        arg0->x58 = arg0->x58 * arg0->x14;
    }
    return 1;
}

s32 func_150B378C(void *a0) {
    u8 *p;
    s32 v1;
    p = *(u8 **)((u8 *)a0 + 0x98);
    v1 = *(s16 *)((u8 *)a0 + 0x1C) << 4;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    if (v1 < p[0x1B]) {
        p[0x1B] = v1;
    }
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 unk0C;
    f32 unk10;
} Pt_150B37C8;

typedef struct {
    f32 unk00;
    f32 unk04;
    u8 pad08[4];
    f32 unk0C;
    f32 unk10;
    u8 pad14[5];
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
    u8 unk20;
} State_150B37C8;

typedef struct {
    u8 pad00[1];
    u8 unk01;
    u8 pad02[0xA];
    u8 unk0C;
    u8 pad0D[0x18];
    u8 unk25;
    u8 pad26[7];
    s8 unk2D;
    u8 pad2E[0x66];
    Pt_150B37C8 *unk94;
    State_150B37C8 *unk98;
} Src_150B37C8;

typedef struct {
    s32 unk00;
    s16 unk04;
    s8 unk06;
    u8 pad07;
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
    u8 pad1C[0xC];
    Pt_150B37C8 *unk28;
} Params_150B37C8;

typedef struct {
    s16 v[3];
} S16x3_150B37C8;

extern S16x3_150B37C8 D_80088700;

s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14);

s32 func_150B37C8(Src_150B37C8 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    State_150B37C8 *state;
    Params_150B37C8 tmp;
    f32 scale;
    S16x3_150B37C8 tbl;
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; s32 pad18; } pos;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; u8 pad14[4]; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s32 pad1C; } motion;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } style;

    {
    s32 rand2;
    s32 rand1;
    s32 rand0;
    state = arg0->unk98;
    tmp.unk28 = arg0->unk94;
    tbl = D_80088700;
    state->unk20 = 4;
    scale = func_150ADA68() * 50.0f + 50.0f;
    tmp.unk06 = *(tbl.v + func_150ADA20() % 3U);
    tmp.unk08 = 0;
    tmp.unk0C = 0;
    tmp.unk00 = 0x11;
    tmp.unk04 = 0x96;
    tmp.unk10 = state->unk1B;
    tmp.unk11 = state->unk1A;
    tmp.unk12 = 0;
    tmp.unk13 = 0;
    tmp.unk14 = 0;
    tmp.unk15 = 0xFF;
    tmp.unk18 = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 1, 0, arg5,
                  tmp.unk28[arg0->unk2D].x,
                  arg4,
                  tmp.unk28[arg0->unk2D].z,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->unk0C, arg0->unk01);

    }
    style.unk00 = 0;
    style.unk04 = 1;
    style.unk08 = 0x160600;
    style.unk0C = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    pos.unk10 = 1;
    pos.unk0E = 1;
    motion.unk19 = state->unk19;
    motion.unk1A = state->unk1A;
    motion.unk18 = 8;
    motion.unk1B = state->unk1B;
    pos.x = tmp.unk28[arg0->unk2D].x;
    pos.y = arg4;
    pos.z = tmp.unk28[arg0->unk2D].z;
    pos.unk15 = arg0->unk25;
    pos.unk0C = 0x3C;
    motion.unk00 = state->unk00 * 0.75f;
    motion.unk10 = state->unk10;
    motion.unk04 = state->unk04;
    motion.unk08 = func_150ADA68() * 15.0f + 15.0f;
    motion.unk0C = state->unk0C;
    func_15147DA0(&pos, &motion, 0, 1, 7, 0, 0, 0, 0, 0, 0, (s32 *)&style, 0, arg0->unk0C, arg0->unk01);
    return 1;
}
