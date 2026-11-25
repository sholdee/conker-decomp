#include <ultra64.h>
// matching: functions.h declares func_1513D524 as void; it returns the spawned object here
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_1513D524
#include "variables.h"

extern void func_1510F800(s32);
extern void *func_1513D524(void *, u8, u8, u8, u8, u8, s32, u8, s32);
extern void func_1513F680();
extern void func_151429E0(u8, u8 *, u8 *, u8 *);
extern void func_1518CA80(void *, u8);
extern void func_151D5D60();

/* Emitter description that func_15149550 turns into a spawned particle. */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 maxA;
    /* 0x10 */ f32 maxB;
    /* 0x14 */ f32 minA;
    /* 0x18 */ f32 minB;
    /* 0x1C */ f32 altB;
    /* 0x20 */ s16 maxC;
    /* 0x22 */ s16 minC;
    /* 0x24 */ s16 life;
    /* 0x26 */ u8 color[4];
    /* 0x2A */ u16 flags;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ s32 extraSize;
    /* 0x34 */ u8 effect;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 scale;
    /* 0x3C */ f32 maxHeight;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
} ParticleEmitter;

/* Spawn parameters passed to func_1513D524. */
typedef struct {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 life;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 pad46[2];
    /* 0x48 */ s32 region;
    /* 0x4C */ s32 pad4C[3];
} ParticleSpawn;

/*
 * Per-particle motion state, stored at +0x110 of the spawned object. Each channel (A, B, C)
 * picks a new random target between its min and max whenever its timer runs out, and the
 * particle field it drives eases towards that target by `rate` per frame.
 */
typedef struct {
    /* 0x00 */ f32 maxA;
    /* 0x04 */ f32 minA;
    /* 0x08 */ f32 targetA;
    /* 0x0C */ s16 timerA;
    /* 0x0E */ s16 periodA;
    /* 0x10 */ f32 rateA;
    /* 0x14 */ f32 maxB;
    /* 0x18 */ f32 minB;
    /* 0x1C */ f32 altB;
    /* 0x20 */ f32 targetB;
    /* 0x24 */ s16 timerB;
    /* 0x26 */ s16 periodB;
    /* 0x28 */ f32 rateB;
    /* 0x2C */ s32 maxC;
    /* 0x30 */ s32 minC;
    /* 0x34 */ s32 targetC;
    /* 0x38 */ s16 timerC;
    /* 0x3A */ s16 periodC;
    /* 0x3C */ f32 rateC;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
    /* 0x48 */ u8 effect;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 maxHeight;
} ParticleMotion;

/* The object func_1513D524 spawns. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 timer;
    /* 0x01E */ u8 pad1E[6];
    /* 0x024 */ s32 unk24;            // eased by motion channel C
    /* 0x028 */ u8 pad28[4];
    /* 0x02C */ f32 width;            // eased by motion channel A
    /* 0x030 */ f32 height;           // eased by motion channel B
    /* 0x034 */ struct17 pos;
    /* 0x040 */ u8 pad40[0xC];
    /* 0x04C */ f32 accelY;
    /* 0x050 */ f32 velY;
    /* 0x054 */ u8 pad54[4];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ u8 pad5C[0x14];
    /* 0x070 */ u8 unk70;
    /* 0x071 */ u8 pad71;
    /* 0x072 */ u8 unk72;
    /* 0x073 */ u8 unk73;
    /* 0x074 */ u8 growFlags;
    /* 0x075 */ u8 pad75[0x4B];
    /* 0x0C0 */ s16 vtxTemplate[0x20];
    /* 0x100 */ s16 *vtxBuffers[4];
    /* 0x110 */ ParticleMotion motion;
} Particle;

/* Spawns a particle described by `emitter`, with randomly flickering size and channel C. */
Particle *func_15149550(ParticleEmitter *emitter, u8 arg1, u8 useRegion, u8 kind, u8 arg4, s32 arg5) {
    Particle *particle;
    ParticleSpawn spawn;
    ParticleMotion motion;
    s32 flip;

    spawn.flags = emitter->flags | 0x40000000;
    spawn.pos = emitter->pos;
    if (useRegion) {
        spawn.flags |= 0x01000000;
        func_1510F800(0);
        spawn.region = func_1510FD20(spawn.pos.unk0, spawn.pos.unk8);
    } else {
        spawn.flags &= ~0x01000000;
        spawn.region = 0;
    }

    switch (kind) {
    case 1:
        spawn.kind = 0x13;
        break;
    case 2:
        spawn.kind = 0x71;
        break;
    case 0:
    default:
        if (func_150ADA20() & 1) {
            spawn.kind = 0x13;
        } else {
            spawn.kind = 0x14;
        }
        break;
    }

    spawn.unk2 = 0x301;
    spawn.life = emitter->life;
    spawn.color[0] = emitter->color[0];
    spawn.color[1] = emitter->color[1];
    spawn.color[2] = emitter->color[2];
    spawn.color[3] = emitter->color[3];
    spawn.unk44 = emitter->unk2C;
    spawn.unk45 = emitter->unk2D;
    spawn.unk28.unk0 = 0.0f;
    spawn.unk28.unk4 = 0.0f;
    spawn.unk28.unk8 = 0.0f;
    spawn.unk34.unk0 = emitter->scale;
    spawn.unk34.unk4 = 0.0f;
    spawn.unk34.unk8 = 1.0f;

    motion.maxA = emitter->maxA;
    motion.minA = emitter->minA;
    motion.timerA = 0;
    motion.periodA = 6;
    motion.targetA = (motion.maxA + motion.minA) * 0.5f;
    motion.rateA = 0.186f;
    motion.maxB = emitter->maxB;
    motion.minB = emitter->minB;
    motion.altB = emitter->altB;
    motion.targetB = (motion.maxB + motion.minB) * 0.5f;
    motion.timerB = 0;
    motion.periodB = 17;
    motion.rateB = 0.359f;
    motion.maxC = emitter->maxC;
    motion.minC = emitter->minC;
    motion.targetC = 0;
    motion.timerC = 0;
    motion.periodC = 15;
    motion.rateC = 0.231f;
    motion.shrinkRate = emitter->shrinkRate;
    motion.growRate = emitter->growRate;
    spawn.unk8 = 0;
    spawn.unkC = (motion.maxC + motion.minC) >> 1;
    spawn.unk14 = 0.0f;
    spawn.unk18 = 0.0f;
    motion.effect = emitter->effect;
    motion.maxHeight = emitter->maxHeight;

    if (func_150ADA20() & 1) {
        flip = 1;
    } else {
        flip = 0;
    }
    particle = func_1513D524(&spawn, emitter->unk2E, 0xE, emitter->unk2F, arg1, flip | 2,
                             emitter->extraSize + sizeof(ParticleMotion), arg4, arg5);
    if (particle != NULL) {
        memcpy(&particle->motion, &motion, sizeof(motion));
    }
    return particle;
}

/* Per-frame update: re-rolls each motion channel's target when its timer expires and eases
 * width, height and unk24 towards them. Returns 1 (keep alive). */
s32 func_15149838(Particle *particle) {
    ParticleMotion *motion = &particle->motion;

    motion->timerA -= D_800BE9E4;
    if (motion->timerA < 0) {
        motion->timerA = func_150ADA20() % (u32)motion->periodA;
        motion->targetA = func_150ADA68() * (motion->maxA - motion->minA) + motion->minA;
    }
    particle->width += (motion->targetA - particle->width) * motion->rateA;

    motion->timerB -= D_800BE9E4;
    if (motion->timerB < 0) {
        motion->timerB = func_150ADA20() % (u32)motion->periodB;
        if (func_150ADA20() & 3) {
            motion->targetB = func_150ADA68() * (motion->maxB - motion->minB) + motion->minB;
        } else {
            motion->targetB = func_150ADA68() * (motion->altB - motion->maxB) + motion->maxB;
        }
    }
    particle->height += (motion->targetB - particle->height) * motion->rateB;

    motion->timerC -= D_800BE9E4;
    if (motion->timerC < 0) {
        motion->timerC = func_150ADA20() % (u32)motion->periodC;
        motion->targetC = func_150ADA20() % (u32)(motion->maxC - motion->minC + 1) + motion->minC;
    }
    particle->unk24 += (s32)(((f32)motion->targetC - (f32)particle->unk24) * motion->rateC);

    if (particle->timer < 5) {
        func_1513F680(particle, particle->unk70, motion->effect, particle->unk72, particle->unk73);
        particle->timer = 300;
        particle->unk58 &= ~1;
    }
    return 1;
}

typedef struct {
    char pad_0x0[0x8];
    f32 field_0x8;
    char pad_0xC[0x14];
    f32 field_0x20;
    char pad_0x24[0x20];
    f32 field_0x44;
} Struct15149550ObjExtra;

typedef struct {
    char pad_0x0[0x1C];
    s16 field_0x1C;
    char pad_0x1E[0xE];
    f32 field_0x2C;
    f32 field_0x30;
    char pad_0x34[0x24];
    s32 field_0x58;
    char pad_0x5C[0x14];
    u8 field_0x70;
    char pad_0x71[0x1];
    u8 field_0x72;
    u8 field_0x73;
    u8 field_0x74;
    char pad_0x75[0x9B];
    Struct15149550ObjExtra field_0x110;
    u8 field_0x158;
} Struct15149550Obj;

s32 func_15149A94(Struct15149550Obj *arg0) {
    Struct15149550ObjExtra *temp_v0;
    s32 var_v1;
    s32 flags;

    flags = arg0->field_0x74;
    var_v1 = 1;
    temp_v0 = &arg0->field_0x110;
    if (!(flags & 2)) {
        arg0->field_0x2C += (temp_v0->field_0x8 - arg0->field_0x2C) * temp_v0->field_0x44;
        if ((temp_v0->field_0x8 * 0.95f) < arg0->field_0x2C) {
            arg0->field_0x74 |= 2;
            flags = arg0->field_0x74;
        } else {
            var_v1 = 0;
            flags = arg0->field_0x74;
        }
    }
    if (!(flags & 8)) {
        arg0->field_0x30 += (temp_v0->field_0x20 - arg0->field_0x30) * temp_v0->field_0x44;
        if ((temp_v0->field_0x20 * 0.95f) < arg0->field_0x30) {
            arg0->field_0x74 |= 8;
        } else {
            var_v1 = 0;
        }
    }
    if (var_v1) {
        func_1513F680(arg0, arg0->field_0x70, 0xD, arg0->field_0x72, arg0->field_0x73);
    }
    if (arg0->field_0x1C < 5) {
        func_1513F680(arg0, arg0->field_0x70, arg0->field_0x158, arg0->field_0x72, arg0->field_0x73);
        arg0->field_0x1C = 0x64;
        arg0->field_0x58 &= -2;
    }
    return 1;
}

struct Foo15149BF4 {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    char pad34[0x11C];
    f32 unk150;
};

s32 func_15149BF4(struct Foo15149BF4 *arg0) {
    s32 ret;

    arg0->unk2C = arg0->unk2C - (arg0->unk2C * arg0->unk150);
    arg0->unk30 = arg0->unk30 - (arg0->unk30 * arg0->unk150);
    if (arg0->unk2C < 2.0f) {
        goto ret0;
    }
    ret = 1;
    if (arg0->unk30 < 2.0f) {
        ret0:
        return 0;
    }
    return ret;
}

extern f32 D_800BE9A4;
s32 func_15149C58(f32 *arg0) {
    arg0[11] -= arg0[11] * arg0[84];
    arg0[12] -= arg0[12] * arg0[84];
    arg0[20] += arg0[19] * D_800BE9A4;
    arg0[14] += arg0[20] * D_800BE9A4;
    if (arg0[87] < arg0[14]) {
        return 0;
    }
    if ((arg0[11] < 4.0f) || (arg0[12] < 4.0f)) {
        return 0;
    }
    return 1;
}

struct Foo15149D18 {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[1];
};

s16 *func_15149D18(struct Foo15149D18 *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f2;
    f32 temp_f12;
    u8 sp37;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp37);
    ret = var_v0;
    if (var_v0) {
        if (sp37) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_f2 = D_800DD1D8[arg1] * arg0->unk2C;
    temp_f12 = D_800DD1E8[arg1] * arg0->unk2C;

    var_v0[0] = var_v0[0x18] = arg0->unk34 + temp_f12;
    var_v0[1] = var_v0[9] = arg0->unk38;
    var_v0[2] = var_v0[0x1A] = arg0->unk3C - temp_f2;
    var_v0[8] = var_v0[0x10] = arg0->unk34 - temp_f12;
    var_v0[0x11] = var_v0[0x19] = arg0->unk38 + arg0->unk30;
    var_v0[0xA] = var_v0[0x12] = arg0->unk3C + temp_f2;

    return ret;
}

typedef struct {
    char pad0[0x34];
    struct17 unk34;
} Struct15149EC4Arg;

typedef struct {
    struct17 unk0;
    char padC[0xA];
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
} Struct15149EC4Stack;

s32 func_15149EC4(Struct15149EC4Arg *arg0) {
    Struct15149EC4Stack sp1C;

    sp1C.unk0 = arg0->unk34;
    sp1C.unk16 = 0;
    sp1C.unk18 = 0;
    sp1C.unk1A = sp1C.unk1C = (func_150ADA20() % 6U) + 8;
    sp1C.unk1E = 0;
    sp1C.unk20 = 0;
    sp1C.unk22 = (func_150ADA20() % 201U) + 0x64;
    sp1C.unk24 = (func_150ADA20() % 5U) + 3;
    sp1C.unk26 = 0x258;
    func_151429E0(3, &sp1C.unk28, &sp1C.unk29, &sp1C.unk2A);
    func_151429E0(4, &sp1C.unk2B, &sp1C.unk2C, &sp1C.unk2D);
    sp1C.unk2E = 0xFF;
    sp1C.unk2F = (func_150ADA20() % 65U) + 0x5C;
    sp1C.unk30 = (func_150ADA20() % 3U) + 1;
    sp1C.unk31 = 0;
    func_1518CA80(&sp1C, 1);
    return 0;
}
