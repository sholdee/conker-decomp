#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern void func_1510F800(s32);
extern void func_1513F680(void *obj, u8, u8 effect, u8, u8);
extern void func_151429E0(u8, u8 *r, u8 *g, u8 *b);
extern void func_1518CA80(void *effect, u8 mode);
extern void func_151D5D60(Vtx **buffers, s16 index, s32 size, Vtx **vtx, u8 *fresh);

/* Emitter description that func_15149550 turns into a spawned particle. */
typedef struct {
    /* 0x00 */ Vec3f pos;
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
    /* 0x034 */ Vec3f pos;
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
    /* 0x0C0 */ Vtx quadTemplate[4];
    /* 0x100 */ Vtx *quadBuffers[4];    // each holds two copies of the quad (double-buffered)
    /* 0x110 */ ParticleMotion motion;
} Particle;

/* Spawns a particle described by `emitter`, with randomly flickering size and channel C. */
Particle *func_15149550(ParticleEmitter *emitter, u8 arg1, u8 arg2, u8 kind, u8 arg4, s32 arg5) {
    Particle *particle;
    ParticleDef spawn;
    ParticleMotion motion;
    s32 flip;

    spawn.flags = emitter->flags | 0x40000000;
    spawn.pos = emitter->pos;
    if (arg2) {
        spawn.flags |= 0x01000000;
        func_1510F800(0);
        spawn.unk48 = func_1510FD20(spawn.pos.x, spawn.pos.z);
    } else {
        spawn.flags &= ~0x01000000;
        spawn.unk48 = 0;
    }

    switch (kind) {
    case 1:
        spawn.texture = 0x13;
        break;
    case 2:
        spawn.texture = 0x71;
        break;
    case 0:
    default:
        if (func_150ADA20() & 1) {
            spawn.texture = 0x13;
        } else {
            spawn.texture = 0x14;
        }
        break;
    }

    spawn.renderModes = 0x301;
    spawn.lifetime = emitter->life;
    spawn.rgba[0] = emitter->color[0];
    spawn.rgba[1] = emitter->color[1];
    spawn.rgba[2] = emitter->color[2];
    spawn.rgba[3] = emitter->color[3];
    spawn.unk44 = emitter->unk2C;
    spawn.unk45 = emitter->unk2D;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = emitter->scale;
    spawn.scale.y = 0.0f;
    spawn.scale.z = 1.0f;

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
    spawn.frame = 0;
    spawn.frameSpeed = (motion.maxC + motion.minC) >> 1;
    spawn.width = 0.0f;
    spawn.height = 0.0f;
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

/* Grows width and height towards their targets; once both are within 95%, switches the
 * particle to effect 0xD. Returns 1 (keep alive). */
s32 func_15149A94(Particle *particle) {
    ParticleMotion *motion = &particle->motion;
    s32 flags = particle->growFlags;
    s32 grown = TRUE;

    if (!(flags & 2)) {
        particle->width += (motion->targetA - particle->width) * motion->growRate;
        if (motion->targetA * 0.95f < particle->width) {
            flags = particle->growFlags |= 2;
        } else {
            grown = FALSE;
            flags = particle->growFlags;
        }
    }
    if (!(flags & 8)) {
        particle->height += (motion->targetB - particle->height) * motion->growRate;
        if (motion->targetB * 0.95f < particle->height) {
            particle->growFlags |= 8;
        } else {
            grown = FALSE;
        }
    }
    if (grown) {
        func_1513F680(particle, particle->unk70, 0xD, particle->unk72, particle->unk73);
    }
    if (particle->timer < 5) {
        func_1513F680(particle, particle->unk70, motion->effect, particle->unk72, particle->unk73);
        particle->timer = 100;
        particle->unk58 &= ~1;
    }
    return 1;
}

/* Shrinks the particle; returns 0 (kill) once either side is below 2 units. */
s32 func_15149BF4(Particle *particle) {
    particle->width -= particle->width * particle->motion.shrinkRate;
    particle->height -= particle->height * particle->motion.shrinkRate;
    if (particle->width < 2.0f || particle->height < 2.0f) {
        return 0;
    }
    return 1;
}

/* Shrinks the particle while it accelerates upwards; returns 0 (kill) once it rises past
 * maxHeight or either side drops below 4 units. */
s32 func_15149C58(Particle *particle) {
    particle->width -= particle->width * particle->motion.shrinkRate;
    particle->height -= particle->height * particle->motion.shrinkRate;
    particle->velY += particle->accelY * D_800BE9A4;
    particle->pos.y += particle->velY * D_800BE9A4;
    if (particle->motion.maxHeight < particle->pos.y) {
        return 0;
    }
    if (particle->width < 4.0f || particle->height < 4.0f) {
        return 0;
    }
    return 1;
}

/* Fetches this frame's vertex buffer for billboard quad `index` (initialising both halves from
 * quadTemplate when newly allocated) and positions its four corners around the particle,
 * rotated by the angle tables D_800DD1D8/D_800DD1E8. Returns NULL if no buffer is available. */
Vtx *func_15149D18(Particle *particle, s16 index) {
    Vtx *vtx;
    Vtx *result; // matching: IDO keeps a spilled copy of vtx for the return
    f32 dz;
    f32 dx;
    u8 fresh;

    func_151D5D60(particle->quadBuffers, index, sizeof(particle->quadTemplate), &vtx, &fresh);
    result = vtx;
    if (vtx != NULL) {
        if (fresh) {
            memcpy(particle->quadBuffers[index], particle->quadTemplate, sizeof(particle->quadTemplate));
            memcpy(particle->quadBuffers[index] + 4, particle->quadTemplate, sizeof(particle->quadTemplate));
        }
    } else {
        return NULL;
    }

    dz = D_800DD1D8[index] * particle->width;
    dx = D_800DD1E8[index] * particle->width;

    vtx[0].v.ob[0] = vtx[3].v.ob[0] = particle->pos.x + dx;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = particle->pos.y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = particle->pos.z - dz;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = particle->pos.x - dx;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = particle->pos.y + particle->height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = particle->pos.z + dz;

    return result;
}

/* Effect description passed to func_1518CA80 (mode 1: the position is given in floats). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 padC[0xA];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u8 color1[3];
    /* 0x2B */ u8 color2[3];
    /* 0x2E */ u8 intensity;          // func_1518CA80 scales both colours by intensity / 256
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
} ParticleEffect;

/* Spawns a randomised func_1518CA80 effect at the particle's position. Returns 0. */
s32 func_15149EC4(Particle *particle) {
    ParticleEffect effect;

    effect.pos = particle->pos;
    effect.unk16 = 0;
    effect.unk18 = 0;
    effect.unk1A = effect.unk1C = (func_150ADA20() % 6U) + 8;
    effect.unk1E = 0;
    effect.unk20 = 0;
    effect.unk22 = (func_150ADA20() % 201U) + 100;
    effect.unk24 = (func_150ADA20() % 5U) + 3;
    effect.unk26 = 600;
    func_151429E0(3, &effect.color1[0], &effect.color1[1], &effect.color1[2]);
    func_151429E0(4, &effect.color2[0], &effect.color2[1], &effect.color2[2]);
    effect.intensity = 0xFF;
    effect.unk2F = (func_150ADA20() % 65U) + 92;
    effect.unk30 = (func_150ADA20() % 3U) + 1;
    effect.unk31 = 0;
    func_1518CA80(&effect, 1);
    return 0;
}
