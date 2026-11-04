#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct { f32 x, y, z; unsigned char u8c; } F2740Ent;
typedef struct { s32 v[3]; } F2740V3i;
typedef struct { f32 x, y, z; } F2740V3;

typedef struct {
    s16 a00; s16 padA;
    F2740V3 pos;
    s16 a10; s16 padB;
    f32 a14;
    s16 a18, a1A, a1C, a1E;
    f32 a20, a24, a28, a2C, a30, a34, a38, a3C, a40, a44, a48, a4C, a50;
    u8 a54, a55;
    s16 a56, a58;
} F2740A;
typedef struct {
    s16 b00, b02;
    F2740V3 pos;
    s16 b10, b12, b14, b16;
    f32 b18, b1C, b20, b24;
    u8 b28, b29, b2A, b2B, b2C, b2D, b2E, b2F, b30;
    u8 padC;
    s16 b32, b34, b36;
    f32 b38, b3C;
    u8 b40, b41;
    s16 b42, b44;
    u8 b46;
    s32 b48;
} F2740B;
typedef struct {
    F2740V3 pos;
    f32 c0C, c10, c14, c18, c1C, c20;
    s16 c24, c26, c28, c2A, c2C, c2E, c30, c32, c34, c36;
    u8 c38, c39;
    s16 padD;
    s32 c3C, c40, c44, c48, c4C, c50;
    u8 c54, c55;
} F2740C;
typedef struct {
    F2740V3 pos;
    f32 d0C;
    u8 d10;
    f32 d14, d18;
    u8 d1C;
    f32 d20;
    u8 d24;
    s16 d26;
    f32 d28, d2C;
    u8 d30, d31, d32, d33, d34, d35;
    s16 d36;
    u8 d38;
    f32 d3C;
} F2740D;

extern F2740Ent D_8009F920[];
extern F2740V3i D_8009F950;
extern F2740V3i D_8009F95C;
extern f32 D_8009FB40, D_8009FB44, D_8009FB48, D_8009FB4C, D_8009FB50, D_8009FB54, D_8009FB58, D_8009FB5C;
extern f32 D_8009FB60, D_8009FB64, D_8009FB68, D_8009FB6C, D_8009FB70, D_8009FB74, D_8009FB78, D_8009FB7C;
extern f32 D_8009FB80, D_8009FB84, D_8009FB88;
extern void func_151A6F00(void *, s32, u8, s32);
extern void func_1514C858(f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, f32, s32, u8);
extern void func_151539B4(void *, u8);
extern void func_15152F70(void *, u8);
extern void func_150B3F5C(void *, void *, s32);

#define E (&D_8009F920[arg0])
void func_150B2740(u8 arg0, u8 arg1) {
    F2740A sA;
    s16 i;
    F2740B sB;
    F2740C sC;
    f32 f20;
    f32 f0;
    F2740V3 sE;
    F2740D sD;

    func_1512D748(&D_800DBFF0[D_800BE9E8], 7, 1);
    sA.a00 = (func_150ADA20() & 1) + 3;
    sA.pos = *(F2740V3 *)E;
    sA.a14 = 170.0f;
    sA.a20 = 16.20000076f;
    sA.a24 = 17.60000038f;
    sA.a28 = 19.30000114f;
    sA.a2C = 21.30000114f;
    sA.a10 = 0x12C;
    sA.a18 = E->u8c - 0x20;
    sA.a1A = 0x41;
    sA.a1C = -0x28;
    sA.a1E = 0x50;
    sA.a54 = 1;
    sA.a55 = 0;
    sA.a56 = 0x32;
    sA.a58 = 0x19;
    sA.a30 = 0.0f;
    sA.a34 = 0.0f;
    sA.a38 = 0.0f;
    sA.a3C = 0.4670000076f;
    sA.a40 = 0.5139999986f;
    sA.a44 = -1.0f;
    sA.a48 = -2.600000143f;
    sA.a4C = -5.0f;
    sA.a50 = 10.0f;
    func_151A6F00(&sA, 0, arg1, 1);
    i = (func_150ADA20() & 1) + 1;
    while (i--) {
        f20 = (func_150ADA68() * 73.0f) + 31.0f;
        func_1514C858(E->x, E->y, E->z, f20, E->u8c + 0x40, 0, 0xFF, (func_150ADA20() % 3U) + 4, 9, 0, 0.009615384974f, 0, arg1);
    }
    sB.b00 = 0x1E;
    sB.b02 = 0xF;
    sB.pos = *(F2740V3 *)E;
    sB.b10 = E->u8c - 0x40;
    sB.b14 = 0x80;
    sB.b12 = -0x2D;
    sB.b16 = 0x5A;
    sB.b28 = 0xFF;
    sB.b29 = 0xFF;
    sB.b2A = 0xFF;
    sB.b2B = 0xFF;
    sB.b2C = 0xFF;
    sB.b2D = 0xFF;
    sB.b2E = 0xFF;
    sB.b2F = 0xFF;
    sB.b30 = 9;
    sB.b32 = 0x10;
    sB.b34 = 0x16;
    sB.b36 = 0x1601;
    sB.b40 = 0x7F;
    sB.b41 = 0x7F;
    sB.b42 = 0xA;
    sB.b44 = 0x19;
    sB.b46 = 0;
    sB.b18 = 6.0f;
    sB.b1C = 2.549999952f;
    sB.b20 = 2.700000048f;
    sB.b24 = 3.419999838f;
    sB.b38 = 4.139999866f;
    sB.b3C = 4.859999657f;
    func_151539B4(&sB, arg1);
    {
    extern void func_15152F70(void *, u8);
    sC.pos = *(F2740V3 *)E;
    sC.c24 = 0xF;
    sC.c26 = 8;
    sC.c28 = E->u8c - 0x19;
    sC.c2A = 0x32;
    sC.c2C = -0x32;
    sC.c2E = 0x3C;
    sC.c30 = 3;
    sC.c32 = 2;
    sC.c34 = 0x3C;
    sC.c36 = 0x14;
    sC.c38 = 6;
    sC.c39 = 8;
    sC.c3C = 1;
    sC.c40 = 7;
    sC.c44 = 0;
    sC.c48 = 0;
    sC.c4C = 0;
    sC.c50 = 0;
    sC.c54 = 0xFF;
    sC.c55 = 0xA0;
    sC.c1C = 20.0f;
    sC.c20 = 40.0f;
    sC.c0C = 10.0f;
    sC.c10 = 30.0f;
    sC.c14 = 1.5f;
    sC.c18 = 1.0f;
    func_15152F70(&sC, arg1);
    }
    sC.c24 = 0xA;
    sC.c26 = 6;
    sC.c34 = 0x64;
    sC.c36 = 0;
    sC.c39 = 9;
    sC.c48 = 5;
    func_15152F70(&sC, arg1);
    f20 = func_151423D8(E->u8c);
    f0 = func_151423D8(E->u8c - 0x40);
    sE.x = E->x - 150.0f * f0;
    sE.y = E->y;
    sE.z = E->z - 150.0f * f20;
    sD.d24 = 0x80;
    sD.pos.x = 10.0f * f0;
    sD.pos.y = 3.0f;
    sD.pos.z = 10.0f * f20;
    sD.d0C = -3.710000038f;
    sD.d26 = 0xFF;
    sD.d30 = 2;
    sD.d31 = 5;
    sD.d10 = 4;
    sD.d38 = 0;
    sD.d36 = 0x1E;
    sD.d1C = 6;
    sD.d32 = 5;
    sD.d33 = 0x33;
    sD.d34 = 3;
    sD.d35 = 0x55;
    sD.d20 = 0.400000006f;
    sD.d2C = 47.0f;
    sD.d28 = 1.840000033f;
    sD.d14 = 3.299999952f;
    sD.d18 = 0.6999999881f;
    sD.d3C = 0.3000000119f;
    {
        u8 sp9C[4];
        F2740V3i sp90;
        F2740V3i sp84;

        sp90 = D_8009F950;
        sp84 = D_8009F95C;
        sp9C[0] = ((s32 *)&sp90)[arg0];
        func_151494E0((s32)sp9C, 0x17);
        sp9C[0] = ((s32 *)&sp84)[arg0];
        func_151494E0((s32)sp9C, 0x17);
    }
    func_150B3F5C(&sD, &sE, 0x14);
    func_10010F88(0xE7, 0x7FFF, 0, 2, 0, (s16)(s32)E->x, (s16)(s32)E->y, (s16)(s32)E->z, 0x3E8, 0xBB8);
    func_10010F88(0x22, 0x7FFF, 0, 0, 0, (s16)(s32)E->x, (s16)(s32)E->y, (s16)(s32)E->z, 0x3E8, 0xBB8);
}

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
