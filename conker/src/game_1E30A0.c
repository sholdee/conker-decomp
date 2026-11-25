#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Object whose sound handle func_151B5FCC stops. */
typedef struct {
    /* 0x00 */ u8 pad0[0x88];
    /* 0x88 */ s32 sound;
} SoundOwner;

/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c, game_1E6260.c). */
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

/* Extra data of a pulsing particle: its size wobbles around baseSize while phase runs up to 4 pi. */
typedef struct {
    /* 0x0 */ f32 phase;
    /* 0x4 */ f32 baseSize;
} Pulse;

/* Head of a pooled physics particle (full layout in game_15F680.c), carrying a Pulse as its extra data. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ u8 pad20[0x18];
    /* 0x038 */ vertex pos;
    /* 0x044 */ u8 pad44[0x170 - 0x44];
    /* 0x170 */ Pulse pulse;
} Particle;

extern Particle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
extern void D_1500310C(void);
extern f32 sinf(f32);
extern void func_15133894(Particle *);

/* Spawns a pulsing particle of the given size at pos (fading out over 0x100 >> fadeShift frames at the end
 * of its lifetime). Also checksums func_15002FB4's code as an anti-tamper check. */
Particle *func_151B5BF0(vertex *pos, vertex *vel, f32 size, vertex *rot, vertex *rotVel, f32 gravity,
                        s16 lifetime, u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12,
                        s32 arg13) {
    Particle *particle;
    ParticleSpawn spawn;
    Pulse pulse;

    pulse.baseSize = size;
    pulse.phase = 0.0f;
    spawn.radius = 24.70000076f * size;
    spawn.elasticity = elasticity;
    spawn.sizeY = size;
    spawn.sizeXZ = size;
    spawn.rot = *rot;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *pos;
    spawn.gravity = gravity;
    spawn.flags = 0xB908;
    if (lifetime == -1) {
        spawn.lifetime = 300;
    } else {
        spawn.lifetime = lifetime + (0x100 >> fadeShift);
        spawn.flags = 0xB988;
    }
    if (vel != NULL) {
        spawn.vel = *vel;
        spawn.flags |= 0x20;
    } else {
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
    }
    {
        u32 *pad; // matching: unused local reserves a stack slot (frame size)
        u32 *end;
        u32 *p;
        s32 sum;

        sum = 0;
        p = (u32 *)func_15002FB4;
        end = (u32 *)D_1500310C;
        for (; p < end; p++) {
            sum += *p;
            sum <<= 1;
        }
        if (sum != 0x80D2D760) {
            D_8008FDA8 = -1;
        }
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
    spawn.model = 2;
    spawn.variant = 0;
    spawn.zone = 0;
    spawn.alpha = alpha;
    spawn.updateFn = 5;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 6;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 2;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 0x100 >> fadeShift;
    spawn.fadeRate = 255 / (0x100 >> fadeShift);
    particle = func_15132A4C(&spawn, 3, 255, sizeof(Pulse), arg12, arg13);
    if (particle != NULL) {
        memcpy(&particle->pulse, &pulse, sizeof(Pulse));
    }
    return particle;
}

void func_151B5E8C(void) {
}

/* Update of the pulsing particle: advances the phase, playing a sound and restarting each time it passes
 * 29.9; for the first 4 pi the size wobbles by 5% of the base size. */
void func_151B5E94(Particle *particle) {
    f32 size;
    Pulse *pulse;

    pulse = &particle->pulse;
    pulse->phase += 1.002000093f * D_800BE9A4;
    if (pulse->phase > 29.89999962f) {
        pulse->phase = 0.0f;
        func_10010F88(0x502, 0x5DC0, 0, 0, -1, particle->pos.x, particle->pos.y, particle->pos.z, 100, 500);
    }
    if (pulse->phase < 12.56637096f) {
        f32 s = sinf(pulse->phase);

        size = pulse->baseSize + s * (0.05000000075f * pulse->baseSize);
        particle->sizeY = size;
        particle->sizeXZ = size;
    } else {
        size = pulse->baseSize;
        particle->sizeY = size;
        particle->sizeXZ = size;
    }
    func_15133894(particle);
}

/* Stops the object's sound, if one is playing. */
void func_151B5FCC(SoundOwner *obj) {
    if (obj->sound != 0) {
        func_100111C8(obj->sound);
        obj->sound = 0;
    }
}
