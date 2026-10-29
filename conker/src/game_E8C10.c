#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
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
            f32 size2;

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

typedef struct GameE8C10SpawnRequest {
    u8 pad0[0x80];
    s32 field80;
    u8 pad84[0x4];
    s16 field88;
    s16 field8A;
    s16 field8C;
    s16 field8E;
    s16 field90;
    s16 field92;
    s16 field94;
    s16 field96;
    s16 field98;
    u8 pad9A[0x6];
    s8 fieldA0;
    s8 fieldA1;
    s8 fieldA2;
    u8 padA3[0x5];
} GameE8C10SpawnRequest;

void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(void *arg0, s32 arg1, s32 arg2);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150BBB5C(u8 *arg0) {
    GameE8C10Particle particle;
    s8 i;
    GameE8C10SpawnRequest request;
    f32 width;
    f32 height;
    s32 count1;
    s32 count2;
    f32 sine;
    f32 cosine;
    u8 alpha;
    f32 offsetX;
    f32 offsetZ;

    if (arg0[4] != 0x20) {
        return 1;
    }
    if (arg0 != NULL && *(s32 *)arg0 != 0) {
        width = *(s16 *)(arg0 + 0xE4);
        height = *(s16 *)(arg0 + 0xE6);
        count1 = (func_150ADA20() & 3) + 6;
        count2 = (func_150ADA20() & 3) + 0xA;
        sine = func_151423D8(*(u16 *)(arg0 + 0x76) >> 8);
        cosine = func_151423D8((*(u16 *)(arg0 + 0x76) >> 8) + 0x40);
        offsetX = *(f32 *)(arg0 + 0x3C) * sine * 0.5f;
        offsetZ = *(f32 *)(arg0 + 0x3C) * cosine * 0.5f;
        alpha = (*(s32 *)(arg0 + 0x1D4) != 0) ? 0xFF - (((*(u32 *)(arg0 + 0x184) >> 5) & 0x7FFFFFF) << 6) : 0xFF;
        request.field8A = 0x80;
        request.field88 = 0;
        request.field8C = *(f32 *)(arg0 + 0x14);
        request.field8E = *(f32 *)(arg0 + 0x180) + 60.0f;
        request.field90 = *(f32 *)(arg0 + 0x1C);
        request.field92 = width;
        request.field94 = height;
        request.field96 = *(f32 *)(arg0 + 0x180);
        request.fieldA2 = -1;
        request.fieldA0 = 0x2E;
        request.field98 = 0x34;
        request.field80 = alpha;
        request.fieldA1 = 0;
        func_1516865C(&request, 0xFF, 0xFF, 0xFF, 0xFF);
        func_15168800(&request, 0xFF, 0);
        particle.field56 = 1;
        particle.field54 = 0x78;
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
        particle.field6A = 0;
        particle.field6C = 0;
        particle.field70 = 0;
        particle.field72 = 0x20;
        particle.field74 = 7;
        particle.field4 = 0.400000006f;
        particle.field1C = 1.0f;
        particle.field20 = 1.0f;
        particle.field24 = 1.0f;
        particle.field48 = 0.0f;
        particle.field4C = -1.5f;
        if (count1 > 0) {
        loop1: {
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
            f32 size2;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = func_150ADA20();
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 4.0f + 2.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = func_150ADA68() * 0.02999999933f + 0.02999999933f;
            size2 = func_150ADA68() * 0.02999999933f + 0.02999999933f;
            particle.field8 = size;
            particle.fieldC = size2;
            particle.field0 = (size + size2) * 0.5f;
            particle.field10 = func_150ADA68() * 360.0f;
            particle.field14 = func_150ADA68() * 360.0f;
            particle.field18 = func_150ADA68() * 360.0f;
            particle.field28 = *(f32 *)(arg0 + 0x14) + spreadX;
            particle.field2C = *(f32 *)(arg0 + 0x18) + func_150ADA68() * height + 5.0f;
            particle.field30 = *(f32 *)(arg0 + 0x1C) + spreadZ;
            particle.field34 = offsetX + velocityX;
            particle.field38 = func_150ADA68() * 7.0f + 13.0f;
            particle.field3C = offsetZ + velocityZ;
            particle.field40 = 25.0f - func_150ADA68() * 50.0f;
            particle.field44 = 25.0f - func_150ADA68() * 50.0f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
            count1--;
            if (count1 > 0) goto loop1;
        }
        }
        while (count2 > 0) {
            f32 pos[3];
            f32 dir[3];
            f32 distance;
            f32 speed;
            s16 angle;
            f32 sine2;
            f32 cosine2;

            distance = func_150ADA68() * width;
            speed = func_150ADA68() * 7.0f + 7.0f;
            angle = func_150ADA20() & 0xFF;
            sine2 = func_151423D8(angle);
            cosine2 = func_151423D8(angle - 0x40);
            pos[0] = *(f32 *)(arg0 + 0x14) + sine2 * distance;
            pos[1] = *(f32 *)(arg0 + 0x18) + func_150ADA68() * height + 5.0f;
            pos[2] = *(f32 *)(arg0 + 0x1C) - cosine2 * distance;
            dir[0] = cosine2 * speed + offsetX;
            dir[1] = func_150ADA68() * 9.0f + 15.0f;
            dir[2] = sine2 * speed + offsetZ;
            func_151DA6F8(pos, dir, 1.600000024f, (func_150ADA20() & 7) + 0x19, (func_150ADA20() & 0x7F) + 0x80,
                          func_150ADA68() * 0.200000003f * width + 13.0f, (func_150ADA20() & 3) + 2, 0,
                          1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, 0xFF, 0);
            count2--;
        }
        particle.field56 = 0;
        particle.field54 = (func_150ADA20() & 0xF) + 0x10;
        particle.field4 = 0.6000000238f;
        particle.field10 = *(f32 *)(arg0 + 0xB8);
        particle.field14 = *(f32 *)(arg0 + 0x40);
        particle.field18 = *(f32 *)(arg0 + 0xC4);
        particle.field50 = 0x39E8;
        particle.field1C = 1.0f;
        particle.field20 = 1.0f;
        particle.field24 = 1.0f;
        particle.field48 = 0.0f;
        particle.field4C = -1.600000024f;
        particle.field28 = *(f32 *)(arg0 + 0x14);
        particle.field2C = *(f32 *)(arg0 + 0x18);
        particle.field30 = *(f32 *)(arg0 + 0x1C);
        particle.field0 = *(f32 *)(arg0 + 0x14C) * 20.0f;
        particle.field8 = *(f32 *)(arg0 + 0x14C) + *(f32 *)(arg0 + 0x14C);
        particle.field58 = 0;
        particle.field5C = 0;
        particle.field60 = 0xFF;
        particle.field61 = 1;
        particle.field62 = 0;
        particle.field63 = 0;
        particle.field64 = 0;
        particle.field65 = 0;
        particle.field66 = 0;
        particle.field67 = 0;
        particle.field68 = 0;
        particle.field6A = 0;
        particle.field6C = 0;
        particle.field70 = 0;
        particle.field72 = 0x10;
        particle.field74 = 0xF;
        particle.fieldC = particle.field8;
        i = 0;
        loop3: {
            f32 speed;
            s16 yaw;
            s16 pitch;
            f32 yawSine;
            f32 yawCosine;
            f32 pitchSine;
            f32 pitchCosine;
            f32 horizontal;

            speed = *(f32 *)(arg0 + 0x150) * (func_150ADA68() * 30.0f) + 10.0f;
            yaw = func_150ADA20() & 0x7F;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            yawSine = func_151423D8(yaw);
            yawCosine = func_151423D8(yaw - 0x40);
            pitchSine = func_151423D8(pitch);
            pitchCosine = func_151423D8(pitch - 0x40);
            horizontal = speed * pitchSine;
            particle.field34 = horizontal * yawCosine + offsetX;
            particle.field38 = -speed * pitchCosine;
            particle.field3C = horizontal * yawSine + offsetZ;
            particle.field40 = 30.0f - func_150ADA68() * 60.0f;
            particle.field44 = 30.0f - func_150ADA68() * 60.0f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
            i++;
        if (i < 2) goto loop3;
        }
        func_10010F88(0xAA, 0x6590, 0, 0, 0, *(f32 *)(arg0 + 0x14), *(f32 *)(arg0 + 0x18),
                      *(f32 *)(arg0 + 0x1C), 0xC8, 0x7D0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8C10/func_150BC488.s")

s32 func_150BCBBC(u8 *arg0) {
    f32 width;
    f32 height;
    s32 count;
    s32 ok;
    GameE8C10Particle particle;

    if (arg0[4] != 0x20) {
        return 1;
    }
    if (arg0 != NULL && *(s32 *)arg0 != 0) {
        width = *(s16 *)(arg0 + 0xE4);
        height = *(s16 *)(arg0 + 0xE6);
        count = func_150ADA20() & 1;
        particle.field56 = 1;
        particle.field54 = 0x96;
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
        particle.field6A = 0;
        particle.field6C = 0;
        particle.field70 = 0;
        particle.field72 = 0x20;
        particle.field74 = 7;
        ok = 1;
        particle.field1C = 1.0f;
        particle.field20 = 1.0f;
        particle.field24 = 1.0f;
        particle.field4 = 0.3f;
        particle.field48 = 0.0f;
        particle.field4C = -1.0f;
        while (count != 0 && ok != 0) {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            f32 sizeA;
            s32 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            distance = func_150ADA68() * 0.03f + 0.01f;
            sizeA = func_150ADA68() * 0.03f + 0.01f;
            if (func_150ADA20() & 1) {
                heading = (u8)((*(u16 *)(arg0 + 0x76) >> 8) + (s32)(8 - ((u32)func_150ADA20() & 0xF)));
            } else {
                heading = (u8)((*(u16 *)(arg0 + 0x76) >> 8) + (s32)(8 - ((u32)func_150ADA20() & 0xF)) - 0x80);
            }
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            particle.field8 = distance;
            particle.fieldC = sizeA;
            particle.field0 = (distance + sizeA) * 0.5f;
            particle.field10 = func_150ADA68() * 360.0f;
            particle.field14 = func_150ADA68() * 360.0f;
            particle.field18 = func_150ADA68() * 360.0f;
            particle.field28 = *(f32 *)(arg0 + 0x14) + spreadX;
            particle.field2C = *(f32 *)(arg0 + 0x18) + func_150ADA68() * height + 4.0f;
            particle.field30 = *(f32 *)(arg0 + 0x1C) + spreadZ;
            particle.field34 = velocityX;
            particle.field38 = func_150ADA68() * 5.0f + 3.0f;
            particle.field3C = velocityZ;
            particle.field40 = 25.0f - func_150ADA68() * 50.0f;
            particle.field44 = 25.0f - func_150ADA68() * 50.0f;
            if (func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
    }
}

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

typedef struct { f32 x, y, z; } Vec3f_BD740;
extern Vec3f_BD740 D_8009FF40[4];
extern Vec3f_BD740 D_8009FF70[4];
extern Vec3f_BD740 D_8009FFA0[4];
void func_15143134(Vec3f_BD740 *src, Vec3f_BD740 *dst, s32 arg2);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 arg6, s32 arg7,
                   s32 arg8, f32 arg9, Vec3f_BD740 *arg10, u8 arg11);

void func_150BD740(void *arg0, u8 arg1, s32 arg2) {
    Vec3f_BD740 pts[4];
    Vec3f_BD740 a[4];
    Vec3f_BD740 b[4];
    s32 i;
    Vec3f_BD740 d[4];

    if (arg0 != NULL) {
        if (*(s32 *)((u8 *)arg0 + 0x1D4) != 0) {
            for (i = 0; i < 4; i++) {
                func_15143134(&D_8009FF40[i], &pts[i], *(s32 *)((u8 *)arg0 + 0x1D4));
                func_15143134(&D_8009FF70[i], &a[i], *(s32 *)((u8 *)arg0 + 0x1D4));
                func_15143134(&D_8009FFA0[i], &b[i], *(s32 *)((u8 *)arg0 + 0x1D4));
            }
            d[2].x = b[2].x - a[2].x;
            d[2].y = b[2].y - a[2].y;
            d[2].z = b[2].z - a[2].z;
            func_1514C470(pts[2].x, pts[2].y, pts[2].z, pts[3].x, pts[3].y, pts[3].z,
                          func_150ADA68() * 6.0f + 14.0f, 0xD, 0, 0.0f, &d[2], arg1);
            d[3].x = b[3].x - a[3].x;
            d[3].y = b[3].y - a[3].y;
            d[3].z = b[3].z - a[3].z;
            func_1514C470(pts[3].x, pts[3].y, pts[3].z, pts[0].x, pts[0].y, pts[0].z,
                          func_150ADA68() * 6.0f + 14.0f, 0xD, 0, 0.0f, &d[3], arg1);
        }
    }
}

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
