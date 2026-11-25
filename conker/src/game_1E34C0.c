#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (as in game_1E30A0.c). */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ s16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s32 zone;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x78 */ s32 unk78;
} ParticleSpawn; // size 0x7C

/* Extra data of a pulsing particle: its size wobbles around baseSize as phase cycles through 2 pi. */
typedef struct {
    /* 0x0 */ f32 phase;
    /* 0x4 */ f32 baseSize;
} Pulse;

/* Head of a pooled physics particle (full layout in game_15F680.c), carrying a Pulse as its extra data. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ u8 pad20[0x170 - 0x20];
    /* 0x170 */ Pulse pulse;
} Particle;

extern Particle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
extern f32 sinf(f32);
extern void func_15133894(Particle *);

/* Spawns a pulsing particle of the given size at pos (fading out over 0x100 >> fadeShift frames at the end
 * of its lifetime). */
Particle *func_151B6010(vertex *pos, vertex *vel, f32 size, vertex *rot, vertex *rotVel, f32 gravity,
                        s16 lifetime, u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12,
                        s32 arg13) {
    Particle *particle;
    ParticleSpawn spawn;
    Pulse pulse;
    s32 fadeTime;

    pulse.baseSize = size;
    pulse.phase = 0.0f;
    spawn.radius = 8.100000381f * size;
    spawn.elasticity = elasticity;
    spawn.sizeY = size;
    spawn.sizeXZ = size;
    spawn.rot = *rot;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *pos;
    spawn.gravity = gravity;
    spawn.flags = 0x3900;
    fadeTime = 0x100 >> fadeShift;
    if (lifetime == -1) {
        spawn.lifetime = 300;
    } else {
        spawn.lifetime = lifetime + fadeTime;
        spawn.flags = 0x3980;
    }
    if (vel != NULL) {
        spawn.vel = *vel;
        spawn.flags |= 0x20;
    } else {
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
    }
    if (rotVel != NULL) {
        spawn.rotVel = *rotVel;
        spawn.flags |= 0x40;
    } else {
        spawn.rotVel.x = 0.0f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = 0.0f;
    }
    if (arg11) {
        spawn.flags |= 1;
    }
    spawn.model = 3;
    spawn.variant = 0;
    spawn.zone = 0;
    spawn.alpha = alpha;
    spawn.updateFn = 6;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 6;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = fadeTime;
    spawn.fadeRate = 255 / fadeTime;
    particle = func_15132A4C(&spawn, 3, 255, sizeof(Pulse), arg12, arg13);
    if (particle != NULL) {
        memcpy(&particle->pulse, &pulse, sizeof(Pulse));
    }
    return particle;
}

/* Update of the pulsing particle: advances the phase (wrapping at 2 pi) and wobbles the size by 6.1% of the
 * base size. */
void func_151B6254(Particle *particle) {
    Pulse *pulse = &particle->pulse;

    pulse->phase += 0.5560000539f * D_800BE9A4;
    while (pulse->phase > 6.283185482f) {
        pulse->phase -= 6.283185482f;
    }
    {
        f32 wave = sinf(pulse->phase);
        f32 size = pulse->baseSize + wave * (0.06100000441f * pulse->baseSize);

        particle->sizeY = size;
        particle->sizeXZ = size;
    }
    func_15133894(particle);
}
