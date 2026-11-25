#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#define func_150ADA20 func_150ADA20_u8_unused
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"


/*
 * Spawn template of a billboard particle: func_15167D84 copies these 0x38 bytes to Particle.anim
 * onwards (layout as Particle in game_1944C0.c, minus its 0x10-byte object link).
 */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ s32 floorAndMinSize; /* this file: low half floor height, high half minimum size */
    /* 0x08 */ s32 kind;            /* this file: 0-3, see func_151B9964 */
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 size;            /* this file: base size of a pulsing (kind 1) particle */
    /* 0x1C */ u8 subX;
    /* 0x1D */ u8 subZ;
    /* 0x1E */ u8 subY;
    /* 0x1F */ s8 callback;         /* index into D_8008C9C8, -1 = none */
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 phase;            /* this file: pulse phase, 1/256ths of a turn */
    /* 0x2C */ u8 primR;
    /* 0x2D */ u8 primG;
    /* 0x2E */ u8 primB;
    /* 0x2F */ u8 primA;
    /* 0x30 */ u8 envR;
    /* 0x31 */ u8 envG;
    /* 0x32 */ u8 envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ u16 flags;
    /* 0x36 */ u8 pad36[2];
} ParticleTemplate; /* size 0x38 */

extern void *D_8008CA4C[];
extern s32 func_150ADA20(void);
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/* A random offset in [-range / 2, range / 2). */
#define RAND_SPREAD(range) ((s32)((func_150ADA20() % (u32)(range)) - ((range) >> 1)))

/*
 * Spawns one particle around (x, y, z): placed `radius` out at `angle` + 180 degrees (+- angleSpread / 2),
 * moving outwards at `speed` (+- speedRange / 2) and upwards at `velY` (+- velYRange / 2), falling to
 * the floor at the spawn height.
 */
void func_151B9690(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 unused, f32 angle, s16 angleSpread,
                   s16 velY, s16 velYRange, s16 speed, s16 speedRange, s16 radius, s16 minSize, s16 size,
                   u8 owner, s32 list) {
    ParticleTemplate template;
    s32 floorY;
    s16 *pad1; // matching: pad1-3 are unused stack slots of the target's frame
    f32 outSpeed;
    f32 radiusF;
    f32 pad2;
    f32 sine;
    f32 cosine;
    f32 pad3;
    f32 dist;
    s32 upSpeed;
    u8 r;
    u8 g;
    u8 b;

    floorY = y;
    angle += 180.0f;
    angle += RAND_SPREAD(angleSpread);
    sine = sinf(angle * 0.01745329238f);
    cosine = cosf(angle * 0.01745329238f);
    // matching: going through `dist` hoists and spills the (f32)speed conversion like the target
    dist = speed;
    if (speedRange == 0) {
        speedRange = 1;
    }
    dist += RAND_SPREAD(speedRange);
    outSpeed = dist;
    radiusF = radius;
    x = x + radiusF * sine;
    z = z + radiusF * cosine;
    y += 1;
    // matching: computing the random part first gives the target's addu operand order
    upSpeed = RAND_SPREAD(velYRange);
    upSpeed = velY + upSpeed;
    switch (kind) {
    case 2:
        r = 0x68; g = 0x38; b = 0x10;
        break;
    case 0:
    case 1:
    case 3:
    default:
        r = g = b = 0xFF;
        break;
    }
    template.anim = D_8008CA4C[animIndex];
    template.floorAndMinSize = (minSize << 16) | (floorY & 0xFFFF);
    template.kind = kind;
    template.frame = 0;
    template.frameStep = 0x100;
    template.x = x;
    template.y = y;
    template.z = z;
    template.velX = outSpeed * sine;
    template.velZ = outSpeed * cosine;
    template.size = size;
    template.callback = 0; /* func_151B9964 */
    template.velY = upSpeed;
    template.accelY = -160;
    template.width = size;
    template.height = size;
    template.life = 400;
    template.fadeStep = 0;
    template.phase = func_150ADA20();
    template.primA = 0xFF;
    template.flags = 0;
    template.primR = r;
    template.primG = g;
    template.primB = b;
    func_15167D84(&template, 0, 0, -1, owner, list);
}

typedef struct {
    u8  unk0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD[0x7];
    s32 unk14;
    s32 unk18;
    u8  pad1C[0x4];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8  pad26[0x4];
    s16 unk2A;
    u8  pad2C[0x4];
    s16 unk30;
    u8  pad32[0x2];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8  pad3A;
    u8  unk3B;
    u8  pad3C[0x3];
    u8  unk3F;
} struct_151B9964;

extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);
extern s32 func_150ADA20(void);
extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);

void func_151B9964(struct_151B9964 *arg0) {
    s32 cur;
    s32 top;
    s32 hi;
    s32 v;
    f32 sp5C;
    s16 y;
    s32 diff;
    s32 mode;
    u8 angle;

    mode = arg0->unk18;
    if (mode == 1) {
        angle = arg0->unk3B;
        angle += D_800BE9E4 * 4 + D_800BE9E4 * 2;
        arg0->unk3B = angle;
        arg0->unk2A = (s32)(arg0->unk2A * 0.9900000095f);
        sp5C = arg0->unk2A;
        arg0->unk34 = (s32)((func_15048A40(angle) + 3.0f) * sp5C * 0.3333333433f);
        arg0->unk36 = (s32)((func_150489B0(angle) + 3.0f) * sp5C * 0.3333333433f);
    } else {
        cur = arg0->unk34;
        hi = arg0->unk14 >> 16;
        if (hi != cur) {
            v = cur - D_800BE9E4 * 4;
            if (v < hi) {
                v = hi;
            }
            arg0->unk36 = v;
            arg0->unk34 = v;
        }
    }
    v = arg0->unk22;
    y = arg0->unk14 & 0xFFFF;
    if (y >= v) {
        arg0->unk38 = 0;
        if (mode == 0) {
            if ((func_150ADA20() & 0xFF) < 0x40) {
                func_15171D4C(arg0->unk20, y, arg0->unk24, 0xA, 0, 0x13, 0.0f, 0, 0x32, 0xF, 0x100, 0, arg0->unkC, arg0->unk1);
            }
            if ((func_150ADA20() & 0xFF) < 0xF) {
                func_150ADA20();
                func_10010F88(0x70, 0x1388, 0, 0, 0, arg0->unk20, y, arg0->unk24, 0x1F4, 0x7D0);
            }
        }
    } else {
        diff = v - y;
        if (arg0->unk30 < 0 && diff < 0x40) {
            arg0->unk3F = diff * 4;
        }
    }
}

typedef struct {
    u8  unk0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD[0x3];
    s32 unk10;
    u8  unk14;
    u8  unk15;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
} struct_151B9BF0;

s32 func_15167A68(s32, s32, s32, s32, s32, s32);
void func_151B9690(s32, s32, s16, s16, s16, s16, f32, s16, s16, s16, s16, s16, s16, s16, s16, u8, s32);

void func_151B9BF0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5,
                   s16 arg6, s16 arg7, s16 arg8, s16 arg9, s16 arg10, s16 arg11,
                   s16 arg12, s32 arg13, u8 arg14, s32 arg15) {
    struct_151B9BF0 *ret;

    ret = (struct_151B9BF0 *)func_15167A68(7, arg15, 0x2C, 0, arg14, 1);
    if (ret != 0) {
        ret->unk14 = arg0;
        ret->unk15 = arg1;
        ret->unk16 = arg2;
        ret->unk18 = arg3;
        ret->unk1A = arg4;
        ret->unk1C = arg5;
        ret->unk1E = arg6;
        ret->unk20 = arg7;
        ret->unk22 = arg8;
        ret->unk24 = arg9;
        ret->unk26 = arg10;
        ret->unk28 = arg11;
        ret->unk2A = arg12;
        ret->unk10 = arg13;
    }
}

void func_151B9CB0(struct_151B9BF0 *arg0) {
    f32 *temp_v0;
    struct_151B9BF0 *temp_s5;
    s16 temp_s2;
    s16 temp_s3;
    s16 temp_s4;
    s32 i;
    f32 temp_f20;
    s32 temp_s5_2;
    s32 temp_s6;
    s32 temp_v0_2;

    temp_v0 = (f32 *)arg0->unk10;
    temp_s5 = arg0;
    if (temp_v0 == 0) {
        temp_s2 = arg0->unk16;
        temp_s3 = arg0->unk18;
        temp_s4 = arg0->unk1A;
    } else {
        temp_s2 = (s16)(s32)temp_v0[0];
        temp_s3 = (s16)(s32)temp_v0[1];
        temp_s4 = (s16)(s32)temp_v0[2];
    }

    i = 0;
    if (arg0->unk28 > 0) {
        temp_f20 = 0.0f;
        do {
            func_151B9690(arg0->unk14, arg0->unk15, temp_s2, temp_s3, temp_s4, -0x50,
                          temp_f20, 0x168, arg0->unk1C, arg0->unk1E, arg0->unk20,
                          arg0->unk22, 0xF, arg0->unk24, arg0->unk26, arg0->unkC,
                          arg0->unk1);
            i++;
        } while (i < temp_s5->unk28);
        i = 0;
    }
    temp_f20 = 0.0f;

    temp_s6 = 4;
    temp_s5_2 = 3;
    while (1) {
        func_151B9690(arg0->unk14, arg0->unk15, temp_s2, temp_s3, temp_s4, -0x50,
                      temp_f20, 0x168, (arg0->unk1C * temp_s5_2) / 2,
                      arg0->unk1E * 2, arg0->unk20, arg0->unk22, 0xF,
                      arg0->unk24, arg0->unk26, arg0->unkC, arg0->unk1);
        i++;
        if (i == temp_s6) {
            break;
        }
    }

    temp_v0_2 = arg0->unk2A;
    temp_v0_2 -= D_800BE9E4;
    if (temp_v0_2 < 0) {
        func_1516972C((struct102 *)arg0);
    } else {
        arg0->unk2A = temp_v0_2;
    }
}
