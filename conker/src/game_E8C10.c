#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_8009FFD0;
extern f32 D_8009FFD4;
extern f32 D_8009FFD8;
extern f32 D_8009FFDC;
extern f32 D_8009FFE0;
extern f32 D_8009FFE4;
s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14);

typedef struct GameE8C10Particle {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    s32 field50;
    s16 field54;
    s16 field56;
    u8 field58;
    u8 pad59[0x3];
    s32 field5C;
    u8 field60;
    u8 field61;
    u8 field62;
    u8 field63;
    u8 field64;
    u8 field65;
    u8 field66;
    u8 field67;
    u8 field68;
    u8 pad69[0x1];
    u8 field6A;
    u8 pad6B[0x1];
    s32 field6C;
    u8 field70;
    u8 pad71[0x1];
    s16 field72;
    s16 field74;
    u8 pad76[0x6];
} GameE8C10Particle;

void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern f32 D_8009FE94;
extern f32 D_8009FE98;
extern f32 D_8009FE9C;
extern f32 D_8009FEA0;
extern f32 D_8009FEA4;

s32 func_150BB760(u8 *arg0) {
    GameE8C10Particle particle;
    f32 width;
    f32 height;
    f32 sine;
    f32 cosine;
    f32 offsetX;
    f32 offsetZ;

    if (arg0[4] != 0x20) {
        return 1;
    }
    if (arg0 != NULL && *(s32 *)arg0 != 0 && *(s32 *)(arg0 + 0x1D4) != 0) {
        width = *(s16 *)(arg0 + 0xE4);
        height = *(s16 *)(arg0 + 0xE6);
        sine = func_151423D8(*(u16 *)(arg0 + 0x76) >> 8);
        cosine = func_151423D8((*(u16 *)(arg0 + 0x76) >> 8) + 0x40);
        offsetX = *(f32 *)(arg0 + 0x3C) * sine * 0.5f;
        offsetZ = *(f32 *)(arg0 + 0x3C) * cosine * 0.5f;
        particle.field56 = 1;
        particle.field50 = 0x29E9;
        particle.field58 = 0;
        particle.field5C = 0;
        particle.field60 = 0xFF;
        particle.field61 = 1;
        particle.field62 = 0;
        particle.field63 = 2;
        particle.field64 = 0;
        particle.field65 = 0;
        particle.field66 = 0;
        particle.field67 = 0;
        particle.field68 = 0;
        particle.field6A = 1;
        particle.field6C = (s32)arg0;
        particle.field1C = 1.0f;
        particle.field20 = 1.0f;
        particle.field24 = 1.0f;
        particle.field48 = 0.0f;
        particle.field4C = -1.0f;
        particle.field70 = arg0[0x3B];
        particle.field72 = 0x20;
        particle.field74 = 7;
        {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            u8 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;
            f32 size;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = (*(u16 *)(arg0 + 0x76) >> 8) - (func_150ADA20() & 0x3F) - 0x20;
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = func_150ADA68() * D_8009FE94 + D_8009FE98;
            distance = func_150ADA68() * D_8009FE9C + D_8009FEA0;
            particle.field8 = size;
            particle.fieldC = distance;
            particle.field0 = (size + distance) * 0.5f;
            particle.field10 = func_150ADA68() * 360.0f;
            particle.field14 = func_150ADA68() * 360.0f;
            particle.field18 = func_150ADA68() * 360.0f;
            particle.field28 = *(f32 *)(arg0 + 0x14) + spreadX;
            particle.field2C = *(f32 *)(arg0 + 0x18) + func_150ADA68() * height + 4.0f;
            particle.field30 = *(f32 *)(arg0 + 0x1C) + spreadZ;
            particle.field34 = velocityX + offsetX;
            particle.field38 = func_150ADA68() * 5.0f + 3.0f;
            particle.field3C = velocityZ + offsetZ;
            particle.field40 = 25.0f - func_150ADA68() * 50.0f;
            particle.field44 = 25.0f - func_150ADA68() * 50.0f;
            particle.field54 = (func_150ADA20() & 0x1F) + 0x3C;
            particle.field4 = func_150ADA68() * D_8009FEA4 + 0.25f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BBB5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BC488.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BCBBC.s")

extern f32 D_8009FEF8;

struct Obj150BCFB8 {
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
};

s32 func_150BCFB8(struct Obj150BCFB8 *arg0, s32 a1, s32 a2, s32 a3, f32 arg5) {
    arg0->x3C = (arg0->x10 * D_8009FEF8) + arg5;
    if (-2.0f < arg0->x48) {
        arg0->x44 = 0.0f;
        arg0->x60 &= ~0x6F;
        arg0->x48 = 0.0f;
        arg0->x4C = 0.0f;
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BD070.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BD740.s")

s32 func_150BD954(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *arg13, u8 arg14) {
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; s32 pad18; s32 pad1C; } sp90;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; f32 unk14; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s32 pad1C; } sp70;
    f32 r;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } sp4C;

    sp4C.unk00 = 0;
    sp4C.unk04 = 1;
    sp4C.unk08 = 0x160600;
    sp4C.unk0C = 3;
    sp4C.unk10 = 0x10;
    sp4C.unk14 = 0x80;
    sp4C.unk18 = 0x20;
    sp4C.unk1C = 0;
    sp4C.unk1D = 9;
    sp90.unk10 = 1;
    sp90.unk0E = 1;
    sp90.x = arg2;
    sp90.y = arg3;
    sp70.unk14 = arg3;
    sp70.unk19 = 9;
    sp70.unk1A = 0xFF;
    sp70.unk18 = 8;
    sp90.z = arg4;

    r = (func_150ADA68() * 127.0f + 85.0f) * D_8009FFD0;
    sp70.unk04 = arg13[0] * r;
    sp70.unk08 = arg13[1] * r;
    sp70.unk0C = arg13[2] * r;
    sp70.unk1B = ((u32) func_150ADA20() % 86) + 180;
    sp90.unk15 = (func_150ADA20() & 3) + 3;
    sp90.unk0C = ((u32) func_150ADA20() % 31) + 39;
    sp70.unk00 = (func_150ADA68() * D_8009FFD4 + D_8009FFD8) * D_8009FFDC;
    sp70.unk10 = (func_150ADA68() * 356.0f + D_8009FFE0) * D_8009FFE4;
    func_15147DA0(&sp90, &sp70, 0, 1, 0xB, 0, 0, 0, 0, 0, 0, (s32 *)&sp4C, 0, arg14, 1);
    return 1;
}

struct Sub150BDB3C {
    u8 pad1B[0x1B];
    u8 unk1B;
};

struct Obj150BDB3C {
    u8 pad98[0x1C];
    s16 unk1C;
    u8 pad1E[0x98 - 0x1E];
    struct Sub150BDB3C *unk98;
};

s32 func_150BDB3C(struct Obj150BDB3C *arg0) {
    struct Sub150BDB3C *sp = arg0->unk98;
    s32 val = arg0->unk1C << 3;

    if (val < sp->unk1B) {
        sp->unk1B = val;
    }
    return 1;
}
