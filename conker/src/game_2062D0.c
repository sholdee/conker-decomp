#include <ultra64.h>

// functions.h declares these three with other return types; this file uses the s32 versions below.
#define func_1513D668 func_1513D668_void_proto
#define func_151DA08C func_151DA08C_void_proto
#include "functions.h"
#undef func_1513D668
#undef func_151DA08C
#include "variables.h"
#include "effects.h"

s32 func_1513D668(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, f32 arg7, f32 arg8, s32 arg9, s32 argA, u8 argB, s32 argC, u8 argD, s32 argE);
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, vertex *pos, s32 arg7, s32 arg8);
s32 func_1514672C(struct17 *arg0);
s32 func_15046C80(struct17 *point, s32 arg1, f32 startY, f32 *floorY);
void func_151D9FC0(u8 kind, f32 size, u8 arg2, s32 arg3, struct17 *pos, u8 arg5, s32 arg6);
void func_151DAB58(u8 kind, f32 size, u8 arg2, vertex *pos, volatile u8 arg4, u8 arg5, s32 arg6);
s32 func_15131918(struct17 *vel, f32 scale);

extern u8 (*D_8008FCD0[])(void);
extern u8 (*D_8008FD04[])(void);
extern u8 D_800AB330[]; /* per effect kind flag */
extern u8 D_800AB414[]; /* RGB per effect kind */

/* Start of a live effect created from an ParticleDef. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    ParticleDef desc;
} EffectBase; /* 0x70 */

/* Data of a func_151DA08C effect: wobbles its scale and keeps spawning func_151DB5D0 particles. */
typedef struct {
    u8 phaseX;
    u8 phaseY;
    s8 stepX;
    s8 stepY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 spawnCredit;
    f32 size;
    f32 shrink; /* size multiplier per frame (timed effects) */
    u8 kind;
    u8 pad19[3];
} SteamData; /* 0x1C */

typedef struct {
    EffectBase base;
    u8 pad70[0xB8];
    SteamData steam;
} SteamEffect;


/* Effect kind for the current context: D_800E0A10, or kind 10 when D_800BE9F0 is clear and func_150A29C8(0, 0x1C) fails. */
u8 func_151D8E20(void) {
    if ((D_800BE9F0 == 0) && (func_150A29C8(0, 0x1C) == 0)) {
        return 10;
    }
    return D_800E0A10;
}

/* Id picker for effect kind 0 in the D_8008FCD0 table. */
u8 func_151D8E6C(void) {
    u8 ids[3] = D_800AB340;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 1 in the D_8008FCD0 table. */
u8 func_151D8EB0(void) {
    return 117;
}

/* Id picker for effect kind 2 in the D_8008FCD0 table. */
u8 func_151D8EBC(void) {
    return 29;
}

/* Id picker for effect kind 3 in the D_8008FCD0 table. */
u8 func_151D8EC8(void) {
    return (func_150ADA20() & 1) ? 17 : 147;
}

/* Id picker for effect kind 4 in the D_8008FCD0 table. */
u8 func_151D8EFC(void) {
    return (func_150ADA20() & 1) ? 90 : 91;
}

/* Id picker for effect kind 5 in the D_8008FCD0 table. */
u8 func_151D8F30(void) {
    u8 ids[5] = D_800AB344;
    return ids[func_150ADA20() % 5U];
}

/* Id picker for effect kind 6 in the D_8008FCD0 table. */
u8 func_151D8F7C(void) {
    return (func_150ADA20() & 1) ? 102 : 103;
}

/* Id picker for effect kinds 7 and 8 in the D_8008FCD0 table. */
u8 func_151D8FB0(void) {
    return 149;
}

/* Id picker for effect kind 9 in the D_8008FCD0 table. */
u8 func_151D8FBC(void) {
    return 159;
}

/* Id picker for effect kind 10 in the D_8008FCD0 table. */
u8 func_151D8FC8(void) {
    return 179;
}

/* Id picker for effect kind 11 in the D_8008FCD0 table. */
u8 func_151D8FD4(void) {
    return 117;
}

/* Id picker for effect kind 12 in the D_8008FCD0 table. */
u8 func_151D8FE0(void) {
    u8 ids[4] = D_800AB34C;
    return ids[func_150ADA20() & 3];
}

#define WOBBLE_FLAG_FIXED_SIZE 1 /* skip the size wobble */
#define WOBBLE_FLAG_BOUNCE     2 /* bounce on the floor instead of bursting */

/* Per-particle data of a func_151D9014 particle (Particle.data). */
typedef struct {
    f32 size;
    u8 phaseX;
    u8 phaseY;
    s8 stepX;
    s8 stepY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 burstScale;
    f32 altBurstScale;
    u8 kind;
    u8 flags;
    u8 pad1A[2];
} ParticleWobble; /* 0x1C */

#define SPIN_FLAG_SPIN 1 /* oscillate desc.unk3C */
#define SPIN_FLAG_DRAG 2 /* scale the velocity by `drag` every frame */

/* Per-particle data of a func_151DB5D0 particle (Particle.data). */
typedef struct {
    u8 flags;
    f32 drag;
    u8 angleX;
    u8 angleY;
    u8 angleZ;
    u8 stepX; /* signed steps, but the u8 angle sums wrap the same */
    u8 stepY;
    u8 stepZ;
    struct17 amplitude;
} ParticleSpin; /* 0x1C */

/* A live sprite particle as returned by func_15130280. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    ParticleDesc desc;
    f32 floorY;
    struct17 unk84;
    u8 pad90[0xD];
    u8 unk9D;
    u8 pad9E[0xA];
    union {
        ParticleWobble wobble;
        ParticleSpin spin;
    } data;
} Particle;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
s32 func_151D9450(Particle *particle, struct17 *arg1);
u8 func_151D9534(Particle *particle, struct17 *arg1);

/* Spawns a wobbling sprite particle of effect kind `kind`; lifetime -99 means it never times out. */
Particle *func_151D9014(vertex *pos, vertex *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                        f32 burstScale, f32 altBurstScale, u8 argA, s32 argB, s32 wobbles, s32 bounces, u8 argE,
                        s32 argF) {
    Particle *particle;
    ParticleDesc desc;
    ParticleWobble wobble;

    desc.texture = D_8008FCD0[kind]();
    desc.renderMode = ((argA ? 0x44 : 0x22) << 8) + 3;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = 0;
    desc.g = 0;
    desc.b = 0;
    desc.a = 0xFF;
    desc.r2 = 0;
    desc.g2 = 0;
    desc.b2 = 0;
    desc.alpha = arg5;
    desc.brightness = 0xFF;
    desc.pos = *pos;
    desc.offset = *(vertex *)&D_800A5480;
    desc.vel = *vel;
    desc.scaleStart = 1;
    desc.gravity = gravity;
    desc.scaleRate = 1.0f;
    desc.flags = ((lifetime == -99) ? 0 : 1) | 2 | 4 | ((func_150ADA20() & 1) ? 0x40 : 0) |
                 ((func_150ADA20() & 1) ? 0x80 : 0) | 0x200 | 0x4000 | 0x8000 | (argA ? 0x10000 : 0) |
                 (D_800AB330[kind] ? 0x800000 : 0);
    if (lifetime == -99) {
        desc.timer = 300;
        desc.fadeStart = 1;
        desc.fadeRate = 0xFF;
    } else {
        desc.timer = lifetime + 18;
        desc.fadeStart = 18;
        desc.fadeRate = 14;
    }
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.drawFunc = -1;
    desc.updateFunc = (u8)arg7 ? 0x21 : 0x22;
    desc.deathFunc = -1;
    desc.handlerSet = 0;
    desc.viewMask = 0;
    desc.views = 0xFF;
    desc.minSize = 0;
    desc.maxSize = 10000.0f;
    wobble.phaseX = func_150ADA20() & 0xFF;
    wobble.phaseY = func_150ADA20() & 0xFF;
    if (desc.texture == 0x40 || desc.texture == 0x67) {
        wobble.size = desc.width = desc.height = size * 0.7f;
    } else {
        wobble.size = desc.width = desc.height = size;
    }
    wobble.stepX = (func_150ADA20() % 6U) + 5;
    wobble.stepY = (func_150ADA20() % 6U) + 5;
    wobble.amplitudeX = ((func_150ADA68() * 0.25f) + 0.2f) * wobble.size;
    wobble.amplitudeY = ((func_150ADA68() * 0.25f) + 0.2f) * wobble.size;
    wobble.burstScale = burstScale;
    wobble.kind = kind;
    wobble.altBurstScale = altBurstScale;
    wobble.flags = ((u8)wobbles ? 0 : WOBBLE_FLAG_FIXED_SIZE) | ((u8)bounces ? WOBBLE_FLAG_BOUNCE : 0);
    particle = func_15130280(&desc, 1, argB, sizeof(wobble), argE, argF);
    if (particle != NULL) {
        memcpy(&particle->data.wobble, &wobble, sizeof(wobble));
    }
    return particle;
}

/* Per-frame update of a func_151D9014 particle; returns 0 once it should be removed. */
s32 func_151D93F4(Particle *particle, struct17 *arg1) {
    s32 alive;

    if (func_151D9450(particle, arg1) != 0) {
        if (func_151D9534(particle, arg1) != 0) {
            alive = 1;
        } else {
            alive = 0;
        }
    } else {
        alive = 0;
    }
    return alive;
}

/* Wobbles the particle's two scale axes around its base size. */
s32 func_151D9450(Particle *particle, struct17 *arg1) {
    ParticleWobble *wobble;

    wobble = &particle->data.wobble;
    if (wobble->flags & WOBBLE_FLAG_FIXED_SIZE) {
        return 1;
    }
    wobble->phaseX = wobble->phaseX + wobble->stepX * D_800BE9E4;
    wobble->phaseY = wobble->phaseY + wobble->stepY * D_800BE9E4;
    particle->desc.width = wobble->amplitudeX * func_151423D8((u8)(wobble->phaseX - 0x40)) + wobble->size;
    particle->desc.height = wobble->amplitudeY * func_151423D8((u8)(wobble->phaseY - 0x40)) + wobble->size;
    return 1;
}

/*
 * Floor collision: once the particle falls below arg1's height, finds the floor under it and either
 * bounces (WOBBLE_FLAG_BOUNCE) or bursts into smaller effects. Returns 0 when the particle is gone.
 */
u8 func_151D9534(Particle *particle, struct17 *arg1) {
    volatile u8 alive; // matching: IDO keeps the result in its stack slot and reloads it after each call
    struct17 point;
    ParticleWobble *wobble;
    f32 avgScale;

    alive = 1;
    if (particle->desc.pos.y < arg1->unk4) {
        point.unk0 = particle->desc.pos.x;
        point.unk4 = arg1->unk4;
        point.unk8 = particle->desc.pos.z;
        if (func_1514672C(&point) == 0) {
            return 0;
        }
        if (func_15046C80(&point, 0,
 particle->desc.pos.y, &particle->floorY) != 0) {
            point.unk4 = particle->floorY + 2.0f;
            if (particle->data.wobble.flags & WOBBLE_FLAG_BOUNCE) {
                particle->desc.pos.y = (particle->desc.height * 0.060000002f) + point.unk4;
                particle->desc.vel.x *= 0.39900002f;
                particle->desc.vel.y *= -0.39900002f;
                particle->desc.vel.z *= 0.39900002f;
                if (fabsf(particle->desc.vel.y) < 0.326f) {
                    particle->desc.vel.x = 0.0f;
                    particle->desc.flags &= ~6;
                    particle->desc.vel.y = 0.0f;
                    particle->desc.vel.z = 0.0f;
                    particle->desc.gravity = 0.0f;
                }
            } else {
                alive = 0;
                wobble = &particle->data.wobble;
                avgScale = (particle->desc.width + particle->desc.height) * 0.5f;
                if (particle->unk9D == 3) {
                    func_151D9FC0(wobble->kind, wobble->altBurstScale * avgScale, particle->desc.alpha,
                                  (s32)&particle->unk84, &point, particle->unkC, particle->unk1);
                } else if (func_150ADA20() & 1) {
                    func_151D9B8C(wobble->kind, (avgScale * 0.09f) * wobble->burstScale, particle->desc.alpha,
                                  (s32)&particle->unk84, &point, 100, 0, 1, 0, particle->unkC, particle->unk1);
                } else {
                    func_151DAB58(wobble->kind, (avgScale * 0.025f) * wobble->burstScale, particle->desc.alpha, &point,
                                  1, particle->unkC, particle->unk1);
                }
            }
        }
    }
    return alive;
}

/* Id picker for effect kind 0 in the D_8008FD04 table. */
u8 func_151D97A8(void) {
    s32 ids[7] = D_800AB350;
    return ids[func_150ADA20() % 7U];
}

/* Id picker for effect kind 3 in the D_8008FD04 table. */
u8 func_151D9820(void) {
    s32 ids[3] = D_800AB36C;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 1 in the D_8008FD04 table. */
u8 func_151D9878(void) {
    s32 ids[3] = D_800AB378;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 2 in the D_8008FD04 table. */
u8 func_151D98D0(void) {
    s32 ids[2] = D_800AB384;
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 4 in the D_8008FD04 table. */
u8 func_151D9918(void) {
    s32 ids[2] = D_800AB38C;
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 5 in the D_8008FD04 table. */
u8 func_151D9960(void) {
    s32 ids[5] = D_800AB394;
    return ids[func_150ADA20() % 5U];
}

/* Id picker for effect kind 6 in the D_8008FD04 table. */
u8 func_151D99C8(void) {
    s32 ids[3] = D_800AB3A8;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kinds 7 and 8 in the D_8008FD04 table. */
u8 func_151D9A20(void) {
    s32 ids[2] = D_800AB3B4;
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 9 in the D_8008FD04 table. */
u8 func_151D9A68(void) {
    s32 ids[3] = D_800AB3BC;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 10 in the D_8008FD04 table. */
u8 func_151D9AC0(void) {
    s32 ids[1] = D_800AB3C8;
    return ids[0];
}

/* Id picker for effect kind 11 in the D_8008FD04 table. */
u8 func_151D9ADC(void) {
    s32 ids[3] = D_800AB3CC;
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 12 in the D_8008FD04 table. */
u8 func_151D9B34(void) {
    s32 ids[4] = D_800AB3D8;
    return ids[func_150ADA20() & 3];
}

/* Emitter descriptor consumed by func_1513C650, func_1513C73C and func_1513C5B0. */
typedef struct {
    s32 flags;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 pad21;
    s16 unk22;
    s16 unk24;
} EmitterDesc; /* 0x28 */

/* Data of a func_15149130 effect that drops a func_151D9014 particle whenever its timer runs out. */
typedef struct {
    s16 timer;
    f32 size;
    struct17 pos;
    u8 unk14;
    u8 unk15;
    u8 kind;
} DripData; /* 0x18 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    DripData drip;
} DripEffect;

s32 func_15144E80(s32, struct17 *, struct17 *, struct17 *);
s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
f32 func_15144A74(struct17 *, struct17 *);

/*
 * Spawns an emitter of effect kind `kind` at `pos` (lifetime -1: default). With arg8 set and an
 * upward-facing surface under arg3, also starts a DripEffect there.
 */
void func_151D9B8C(u8 kind, f32 size, u8 arg2, s32 arg3, struct17 *pos, s16 lifetime, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, s32 argA) {
    EmitterDesc desc;
    struct260 *effect;
    struct17 outA;
    struct17 outB;
    struct17 normal;
    struct17 down;

    desc.unk22 = 32;
    desc.unk24 = 7;
    desc.unk6 = D_8008FD04[kind]();
    desc.flags = ((lifetime == -1) ? 0 : 1) | (D_800AB330[kind] ? 0x20000 : 0) | 0x100 | 0x200 | 0x400 | 0x1000 |
                 0x2000 | 0x4000 | 0x8000;
    if (lifetime == -1) {
        desc.unk4 = 300;
    } else {
        desc.unk4 = lifetime;
    }
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = arg2;
    desc.unk11 = 0xFF;
    desc.unk14 = 0;
    desc.unk13 = 0;
    desc.unk12 = 0;
    desc.unk15 = 0xFF;
    desc.unk18 = (arg6 ? 2 : 3) + ((arg7 ? 0x3B : 0x22) << 16);
    desc.unk7 = 0;
    desc.unk16 = 0;
    desc.unk17 = 7;
    desc.unk20 = 0xFF;
    desc.unk1C = 0;
    if (func_1513C650((s32)&desc, 0, 0, arg3, pos->unk0, pos->unk4, pos->unk8, size, size,
                      func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2),
                      arg7 ? 3 : 0, arg7 ? 0xFF : 0, 0, arg9, argA) != NULL &&
        arg8 && func_15144E80(arg3, &outA, &outB, &normal)) {
        down = *(struct17 *)&D_800AB3E8;
        func_15145128(&normal, &normal, NULL, NULL);
        if (func_15144A74(&normal, &down) < -0.5f) {
            s32 pad; // matching: stack slot that places drip at the target offset
            DripData drip;

            drip.timer = 0;
            drip.size = size * 4.5f;
            drip.pos = *pos;
            drip.unk14 = arg2;
            drip.unk15 = arg7;
            drip.kind = kind;
            // the header types arg6 as a pointer; it is the size of the effect's data
            effect = func_15149130((lifetime == -1) ? 300 : lifetime, -1, 0x4A, -1, (lifetime == -1) ? 0 : 1, 0,
                                   sizeof(drip), arg9, argA);
            if (effect != NULL) {
                memcpy(&((DripEffect *)effect)->drip, &drip, sizeof(drip));
            }
        }
    }
}

/* DripEffect update: counts the timer down and drops a particle each time it expires. */
void func_151D9EB0(DripEffect *effect) {
    DripData *drip;
    u32 lifeRand;
    f32 rand;

    drip = &effect->drip;
    drip->timer = drip->timer - D_800BE9E4;
    if (drip->timer < 0) {
        rand = func_150ADA68();
        lifeRand = func_150ADA20();
        func_151D9014(&drip->pos, (struct17 *)&D_800A5480, drip->kind, rand * 0.157f + -0.657f, (lifeRand % 41U) + 35,
                      drip->unk14, drip->size, 0, 1.0f, 1.0f, drip->unk15, 0, 1, 0, effect->unkC, effect->unk1);
        drip->timer = (func_150ADA20() % 111U) + 30;
    }
}

/* Splash for effect kind `kind`: an emitter plus, except for kinds 2 and 5, a func_151DA08C effect. */
void func_151D9FC0(u8 kind, f32 size, u8 arg2, s32 arg3, struct17 *pos, u8 arg5, s32 arg6) {
    func_151DBCBC(kind, size * 0.5f, arg2, arg3, pos, arg5, arg6);
    if ((kind != 5) && (kind != 2)) {
        func_151DA08C(kind, size * 0.0003f, 1.01f, arg2, 100, arg3, pos, arg5, arg6);
    }
}

/* Starts a steam effect of kind `kind` at `pos` (lifetime -1: untimed, shrinking by `shrink` otherwise). */
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, vertex *pos, s32 arg7, s32 arg8) {
    s32 effect;
    ParticleDef desc;
    SteamData steam;
    s32 rand3;
    s32 rand2;
    s32 rand1;

    steam.phaseX = 0;
    steam.phaseY = 0;
    steam.size = size;
    steam.stepX = (func_150ADA20() % 5U) + 2;
    steam.stepY = (func_150ADA20() % 5U) + 2;
    steam.amplitudeX = ((func_150ADA68() * 0.2f) + 0.1f) * steam.size;
    steam.amplitudeY = ((func_150ADA68() * 0.2f) + 0.1f) * steam.size;
    steam.kind = kind;
    steam.spawnCredit = 0.0f;
    desc.texture = D_8008FD04[kind]();
    desc.renderModes = 3;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.rgba[0] = 0;
    desc.rgba[1] = 0;
    desc.rgba[2] = 0;
    desc.rgba[3] = 0xFF;
    desc.height = size;
    desc.width = size;
    desc.pos = *pos;
    desc.flags = 0x0CDC0008;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    if (lifetime == -1) {
        desc.lifetime = 300;
        steam.shrink = 1.0f;
        desc.fadeStart = 1;
        desc.fadeRate = 0xFF;
    } else {
        desc.lifetime = lifetime;
        desc.flags = 0x0CDC0009;
        steam.shrink = shrink;
        if (lifetime >= 0x100) {
            desc.fadeStart = 0xFF;
        } else {
            desc.fadeStart = lifetime;
        }
        if (lifetime >= 0x100) {
            desc.fadeRate = 1;
        } else {
            desc.fadeRate = 0xFF / lifetime;
        }
    }
    desc.unk44 = arg3;
    desc.unk45 = 0xFF;
    desc.type = 0;
    desc.unk46 = 4;
    desc.unk47 = 5;
    desc.unk48 = 0;
    desc.viewportMask = 0xFF;
    desc.unk50 = 0;
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    rand3 = func_150ADA20();
    effect = func_1513D668((s32)&desc, 0, 14, 19, 0, (rand2 & 1) + (rand1 & 1), rand3 & 0xFF, 500.0f, 500.0f, 0,
                           arg5, 0, sizeof(steam), 0xFF, 0);
    if (effect != 0) {
        memcpy(&((SteamEffect *)effect)->steam, &steam, sizeof(steam));
    }
    return effect;
}

Particle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime, u8 arg8,
                        s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/* Steam effect update: wobbles the scale and emits func_151DB5D0 particles around the base. */
s32 func_151DA368(SteamEffect *effect) {
    SteamData *steam;
    s32 pad; // matching: unused stack slot
    f32 pos[3];
    f32 vel[3];
    f32 radius;
    f32 dist;
    f32 c;
    f32 avgScale;
    s16 angle;

    steam = &effect->steam;
    steam->phaseX = steam->phaseX + steam->stepX * D_800BE9E4;
    steam->phaseY = steam->phaseY + steam->stepY * D_800BE9E4;
    effect->base.desc.width = func_151423D8(steam->phaseX - 0x40) * steam->amplitudeX + steam->size;
    effect->base.desc.height = func_151423D8(steam->phaseY - 0x40) * steam->amplitudeY + steam->size;
lbl: // matching: ends the basic block so the scaleY store is not moved into the next call's delay slot
    steam->spawnCredit += (0.15f + func_150ADA68() * 0.1f) * D_800BE9A4;
    avgScale = (effect->base.desc.width + effect->base.desc.height) * 0.5f;
    if (steam->spawnCredit > 1.0f) {
        radius = avgScale * 500.0f;
        do {
            angle = func_150ADA20() & 0xFF;
            dist = func_150ADA68() * radius;
            c = func_151423D8(angle - 0x40);
            // matching: indexing the position as an array orders the add's operands like the target
            pos[0] = func_151423D8(angle) * dist + ((f32 *)&effect->base.desc.pos)[0];
            pos[1] = ((f32 *)&effect->base.desc.pos)[1] + -3.5f;
            pos[2] = ((f32 *)&effect->base.desc.pos)[2] + c * dist;
            {
                s16 life;

                vel[0] = 0.0f;
                vel[1] = 0.0f;
                vel[2] = 0.0f;
                life = (func_150ADA20() % 51U) + 150;
                dist = func_150ADA68();
                func_151DB5D0(steam->kind, pos, vel, avgScale * 471.0f, (dist * 154.0f + 10106.0f) * 0.0001f, 1.0f,
                              (func_150ADA68() * 25.0f + -50.0f) * 0.001f, life, 0x9B, life, 0xFF / life, 1,
                              effect->base.unkC, effect->base.unk1);
            }
            steam->spawnCredit -= 1.0f;
        } while
 (steam->spawnCredit > 1.0f);
    }
    return 1;
}

/* Steam effect update for timed effects: shrinks the size once per elapsed frame. */
s32 func_151DA6A8(SteamEffect *effect) {
    s32 i;
    SteamData *steam;

    if (effect->base.desc.flags & 1) {
        steam = &effect->steam;
        i = D_800BE9E4;
        while (i--) {
            steam->size = steam->size * steam->shrink;
        }
    }
    return 1;
}
typedef struct {
    struct17 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    s8 unk15;
    u8 pad16[2];
    s32 unk18;
} SplashSpawn; /* 0x1C */

typedef struct {
    f32 scale;
    struct17 vel;
    f32 size;
    f32 unk14;
    u8 flags;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} SplashMotion; /* 0x20 */

/* Data a splash carries for the bursts it makes on impact (func_151DA938, func_151DAA88). */
typedef struct {
    f32 burstScale;
    f32 altBurstScale;
    u8 kind;
    u8 pad9[3];
} SplashBurst; /* 0xC */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} SplashStyle; /* 0x20 */

/* The block a live splash points to at +0x98: its SplashMotion copy, state, and SplashBurst. */
typedef struct {
    SplashMotion motion;
    u8 unk20;
    u8 pad21[0x27];
    SplashBurst burst;
} SplashState;

typedef struct {
    struct17 pos;
    f32 unkC;
    f32 unk10;
} SplashNode; /* 0x14 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x20];
    s8 node;
    u8 pad2E[0x66];
    SplashNode *nodes;
    SplashState *state;
} Splash;

extern u8 D_800AB3F4[];
extern u8 D_800AB404[];
extern Splash *func_15147DA0(SplashSpawn *, SplashMotion *, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                             SplashStyle *, s32, s32, s32);

/* Spawns a splash of effect kind `kind` at `pos` moving with `vel`. */
Splash *func_151DA6F8(struct17 *pos, struct17 *vel, f32 size, s16 lifetime, u8 arg4, f32 scale, s32 arg6, u8 arg7,
                      f32 burstScale, f32 altBurstScale, u8 argA, u8 kind, s32 argC, s16 argD, s16 argE, s32 argF,
                      u8 arg10, s32 arg11) {
    Splash *splash;
    SplashSpawn spawn;
    SplashMotion motion;
    SplashBurst burst;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    SplashStyle style;
    s32 flag80;
    s32 flag20;

    spawn.unk15 = arg6;
    spawn.pos = *pos;
    spawn.unkC = lifetime + 16;
    spawn.unkE = 0x35;
    spawn.unk10 = 1;
    spawn.unk14 = -1;
    spawn.unk18 = argF;
    motion.scale = scale;
    motion.vel = *vel;
    motion.size = size;
    motion.unk14 = -10000.0f;
    flag20 = D_800AB404[kind] ? 0x20 : 0;
    flag80 = D_800AB330[kind] ? 0x80 : 0;
    motion.flags = flag80 | 8 | flag20 | 0x40;
    motion.unk19 = D_800AB3F4[kind];
    motion.unk1A = 0xFF;
    motion.unk1B = arg4;
    motion.unk1C = argD;
    motion.unk1E = argE;
    if (arg7 != 0) {
        motion.flags |= 3;
        a = 7;
        b = 4;
    } else {
        a = 0;
        b = 0;
    }
    burst.kind = kind;
    burst.burstScale = burstScale;
    burst.altBurstScale = altBurstScale;
    {
        s32 pad; // matching: unused stack slot

        style.unk0 = 0;
        style.unk4 = 0x220005;
        style.unk8 = 0x50600;
        style.unkC = 3;
        style.unk10 = 0x46;
        style.unk14 = 0x80;
        style.unk18 = 0x20;
        style.unk1C = 0;
        style.unk1D = 0xC;
        if (argA != 0) {
            c = 2;
            d = 0xFF;
        } else {
            c = 0;
            d = 0;
        }
        splash = func_15147DA0(&spawn, &motion, 16, 1, 0, 0, a, b, 0, c, d, &style, argC, arg10, arg11);
    }
    if (splash != NULL) {
        memcpy(&splash->state->burst, &burst, sizeof(burst));
    }
    return splash;
}


/* Splash impact handler: bursts into a func_151D9B8C emitter or a func_151DAB58 effect at floor height. */
s32 func_151DA938(Splash *splash, s32 arg1, s32 arg2, s32 arg3, f32 floorY, s32 arg5) {
    SplashState *state = splash->state;
    SplashNode *nodes = splash->nodes;
    struct17 point;
    SplashBurst *burst = &state->burst;

    point.unk0 = nodes[splash->node].pos.unk0;
    point.unk4 = floorY + 2.0f;
    point.unk8 = nodes[splash->node].pos.unk8;
    if (func_150ADA20() & 1) {
        func_151D9B8C(burst->kind, (state->motion.scale * 3.0f) * burst->burstScale, state->motion.unk1B, arg5,
                      &point, 100, 0, 1, 0, splash->unkC, splash->unk1);
    } else {
        func_151DAB58(burst->kind, (state->motion.scale * 1.1f) * burst->burstScale, state->motion.unk1B, &point, 1,
                      splash->unkC, splash->unk1);
    }
    state->unk20 = 4;
    return 1;
}

/* Splash impact handler: a func_151D9FC0 burst at floor height. */
s32 func_151DAA88(Splash *splash, s32 arg1, s32 arg2, s32 arg3, f32 floorY, s32 arg5) {
    SplashState *state = splash->state;
    SplashNode *nodes = splash->nodes;
    struct17 point;

    point.unk0 = nodes[splash->node].pos.unk0;
    point.unk4 = floorY;
    point.unk8 = nodes[splash->node].pos.unk8;
    func_151D9FC0(state->burst.kind, state->motion.scale * 11.0f * state->burst.altBurstScale, state->motion.unk1B,
                  arg5, &point, splash->unkC, splash->unk1);
    state->unk20 = 4;
    return 1;
}


/* Data of a func_151DAB58 effect: squashes and stretches it on one phase. */
typedef struct {
    u8 phase;
    s8 step;
    u8 pad2[2];
    f32 amplitudeX;
    f32 amplitudeY;
} SquashData; /* 0xC */

typedef struct {
    EffectBase base;
    u8 pad70[0xA0];
    SquashData squash;
} SquashEffect;

/* Spawns a squash-and-stretch effect of kind `kind` at `pos`. */
// matching: volatile arg4 (and the non-volatile byte read of it below) make IDO reload it from its stack slot
void func_151DAB58(u8 kind, f32 size, u8 arg2, vertex *pos, volatile u8 arg4, u8 arg5, s32 arg6) {
    void *effect;
    ParticleDef desc;
    SquashData squash;
    s32 value;
    s32 coinFlip;
    s32 arg7;

    desc.texture = D_8008FCD0[kind]();
    desc.renderModes = ((arg4 ? 0x3B : 0x22) << 8) + 3;


    desc.lifetime = 100;
    desc.rgba[2] = 0;
    desc.rgba[1] = 0;
    desc.rgba[0] = 0;
    desc.rgba[3] = 0xFF;
    desc.unk44 = arg2;
    desc.unk45 = 0xFF;
    desc.pos = *pos;
    desc.flags = (D_800AB330[kind] ? 0x40000000 : 0) | 0x0CDC0009;

    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.height = size;
    desc.width = size;
    desc.unk46 = 0;
    desc.unk47 = 7;
    squash.phase = 0;
    squash.step = (func_150ADA20() % 3U) + 6;
    squash.amplitudeX = (func_150ADA68() * 0.2f) + 0.1f;
    squash.amplitudeY = (func_150ADA68() * 0.2f) + 0.1f;
    desc.type = 0;
    desc.unk48 = 0;
    desc.viewportMask = 0xFF;
    desc.unk50 = 0;
    desc.fadeStart = 32;
    desc.fadeRate = 7;
    coinFlip = (func_150ADA20() & 1) ? 1 : 0;
    value = *(u8 *)&arg4;
    arg7 = value ? 3 : 0;
    value = value ? 0xFF : 0;
    effect = func_1513D2F0(
&desc, (s32)&D_800A4AA0, 0, 20, 0, 14, coinFlip | 2, arg7, value, sizeof(squash), arg5,
                           arg6);
    if (effect != NULL) {
        memcpy(&((SquashEffect *)effect)->squash, &squash, sizeof(squash));
    }
}

/* Squash effect update: advances the phase and sets the X/Y scale multipliers. */
s32 func_151DADA0(SquashEffect *effect) {
    f32 wave;
    SquashData *squash;

    squash = &effect->squash;
    squash->phase = squash->phase + squash->step * D_800BE9E4;
    wave = func_151423D8((u8)(squash->phase - 0x40));
    effect->base.desc.scale.x = squash->amplitudeX * wave + 1.0f;
    effect->base.desc.scale.y = 1.3f - squash->amplitudeY * wave;
    return 1;
}

/* An effect drawn as a camera-facing quad, with a double-buffered vertex block per view. */
typedef struct {
    EffectBase base;
    u8 pad70[0x50];
    Vtx quad[4];
    Vtx *buffers[1]; /* one per view */
} QuadEffect;

void func_151D5D60(Vtx **buffers, s16 view, s32 size, Vtx **vtx, u8 *fresh);

/* Builds this frame's quad vertices
 for view `view`; returns them, or NULL when no buffer is free. */
Vtx *func_151DAE28(QuadEffect *effect, s16 view) {
    Vtx *vtx;
    Vtx *ret;
    f32 halfWidth;
    f32 height;
    f32 dz;
    f32 dx;
    u8 fresh;

    func_151D5D60(effect->buffers, view, sizeof(effect->quad), &vtx, &fresh);
    ret = vtx;
    if (vtx) {
        if (fresh) {
            memcpy(effect->buffers[view], effect->quad, sizeof(effect->quad));
            memcpy(effect->buffers[view] + 4, effect->quad, sizeof(effect->quad));
        }
    } else {
        return NULL;
    }

    halfWidth = effect->base.desc.width * effect->base.desc.scale.x;
    height = effect->base.desc.height * effect->base.desc.scale.y;
    dz = D_800DD1D8[view] * halfWidth;
    dx = D_800DD1E8[view] * halfWidth;

    vtx[0].v.flag = 0;
    vtx[1].v.flag = 0;
    vtx[2].v.flag = 0;
    vtx[3].v.flag = 0;
    vtx[0].v.ob[0] = vtx[3].v.ob[0] = effect->base.desc.pos.x + dx;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = effect->base.desc.pos.y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = effect->base.desc.pos.z - dz;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = effect->base.desc.pos.x - dx;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = effect->base.desc.pos.y + height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = effect->base.desc.pos.z + dz;

    return ret;
}

/* Colour setter for effect kind 0 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB004(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 80;
    arg0->unk15 = 0;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 46U) + 180;
    arg0->unk19 = 0;
    arg0->unk1A = 0;
}

/* Colour setter for effect kinds 1 and 11 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB068(struct218 *arg0) {
    arg0->unk14 = arg0->unk15 = (func_150ADA20() % 56U) + 100;
    arg0->unk16 = 0;
    arg0->unk18 = arg0->unk19 = (func_150ADA20() % 46U) + 180;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 2 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB0CC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 80;
    arg0->unk15 = (func_150ADA20() % 56U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 46U) + 180;
    arg0->unk19 = (func_150ADA20() % 46U) + 180;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 3 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB15C(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 80;
    arg0->unk15 = (func_150ADA20() % 56U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 46U) + 180;
    arg0->unk19 = (func_150ADA20() % 46U) + 180;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 4 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB1EC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 80;
    arg0->unk15 = (func_150ADA20() % 56U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 46U) + 180;
    arg0->unk19 = (func_150ADA20() % 46U) + 180;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 5 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB27C(struct218 *arg0) {
    arg0->unk14 = 255;
    arg0->unk15 = 255;
    arg0->unk16 = 255;
    arg0->unk18 = 180;
    arg0->unk19 = 200;
    arg0->unk1A = 200;
}

/* Colour setter for effect kind 6 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB2A8(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = 200;
    arg0->unk16 = 0;
    arg0->unk18 = 0;
    arg0->unk19 = 200;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 7 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB2CC(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = (func_150ADA20() % 56U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = 0;
    arg0->unk19 = (func_150ADA20() % 46U) + 180;
    arg0->unk1A = 0;
}

/* Colour setter for effect kind 8 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB330(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 21U) + 95;
    arg0->unk15 = (func_150ADA20() % 21U) + 95;
    arg0->unk16 = (func_150ADA20() % 11U) + 45;
    arg0->unk18 = (func_150ADA20() & 0xF) + 58;
    arg0->unk19 = (func_150ADA20() & 0xF) + 60;
    arg0->unk1A = (func_150ADA20() % 11U) + 25;
}

/* Colour setter for effect kind 9 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB3D8(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = arg0->unk16 = (func_150ADA20() % 56U) + 80;
    arg0->unk18 = 0;
    arg0->unk19 = arg0->unk1A = (func_150ADA20() % 46U) + 180;
}

/* Colour setter for effect kind 10 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB43C(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 80;
    arg0->unk15 = 0;
    arg0->unk16 = (func_150ADA20() % 56U) + 80;
    arg0->unk18 = (func_150ADA20() % 46U) + 180;
    arg0->unk19 = 0;
    arg0->unk1A = (func_150ADA20() % 46U) + 180;
}

/* Colour setter for effect kind 12 (D_8008FD38): fills the RGB triples unk14-16 and unk18-1A. */
void func_151DB4CC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 200;
    arg0->unk15 = (func_150ADA20() % 56U) + 200;
    arg0->unk16 = (func_150ADA20() % 56U) + 200;
    arg0->unk18 = (func_150ADA20() % 56U) + 200;
    arg0->unk19 = (func_150ADA20() % 56U) + 200;
    arg0->unk1A = (func_150ADA20() % 56U) + 200;
}

extern void (*D_8008FD38[])(ParticleDesc *);

/*
 * Spawns a sprite particle of effect kind `kind` at `pos` (lifetime -1: never times out). With `spin` set
 * its desc.unk3C oscillates (func_151DB97C); a `drag` other than 1.0f scales its velocity each frame.
 */
Particle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime, u8 arg8,
                        s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD) {
    ParticleDesc desc;
    Particle *particle;
    ParticleSpin data;

    data.flags = (spin ? SPIN_FLAG_SPIN : 0) | ((drag == 1.0f) ? 0 : SPIN_FLAG_DRAG);
    data.drag = drag;
    data.angleX = 0;
    data.angleY = 0;
    data.angleZ = 0;
    if (spin) {
        data.stepX = (func_150ADA20() % 7U) - 3;
        data.stepY = (func_150ADA20() % 7U) - 3;
        data.stepZ = (func_150ADA20() % 7U) - 3;
        data.amplitude.unk0 = (func_150ADA68() * 0.05f + 0.05f) * size;
        data.amplitude.unk4 = (func_150ADA68() * 0.03f + 0.03f) * size;
        data.amplitude.unk8 = (func_150ADA68() * 0.05f + 0.05f) * size;
    } else {
        data.stepX = 0;
        data.stepY = 0;
        data.stepZ = 0;
        data.amplitude = *(struct17 *)&D_800A5480;
    }
    D_8008FD38[(u8)kind](&desc);
    desc.texture = 0x6C;
    desc.renderMode = 0x5103;
    desc.unk0 = 0x200005;
    desc.unk4 = 0x9F0600;
    desc.timer = (lifetime == -1) ? 300 : lifetime;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.a = 0xFF;
    desc.width = desc.height = size;
    desc.alpha = arg8;
    desc.brightness = 0xFF;
    desc.pos = *(vertex *)pos;
    desc.offset = *(vertex *)&D_800A5480;
    desc.vel = *(vertex *)vel;
    desc.gravity = gravity;
    desc.fadeStart = fadeFrames;
    desc.fadeRate = fadeStep;
    desc.scaleStart = (lifetime == -1) ? 1 : lifetime;
    desc.scaleRate = arg4;
    desc.flags = ((lifetime == -1) ? 0 : 1) | 2 | 4 | ((func_150ADA20() & 1) ? 0x40 : 0) |
                 ((func_150ADA20() & 1) ? 0x80 : 0) | 0x200 | 0x400 | 0x800 | (spin ? 0x1000 : 0) | 0x4000 |
                 0x8000 | 0x40000 | 0x800000;
    desc.primColorMode = 8;
    desc.envColorMode = 6;
    desc.updateFunc = 5;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 0;
    desc.viewMask = 0;
    desc.views = 0xFF;
    desc.minSize = 1000;
    desc.maxSize = 1000.0f;
    particle = func_15130280(&desc, 1, 0, sizeof(data), argC, argD);
    if (particle != NULL) {
        memcpy(&particle->data.spin, &data, sizeof(data));
    }
    return particle;
}

/* func_151DB5D0 particle update: applies drag and oscillates desc.unk3C on three axes. */
s32 func_151DB97C(Particle *particle, s32 arg1) {
    ParticleSpin *spin;
    s32 pad; // matching: unused stack slot
    f32 waveY;
    f32 waveX;


    spin = &particle->data.spin;
    if (spin->flags & SPIN_FLAG_DRAG) {
        func_15131918(&particle->desc.vel, spin->drag);
    }
    if (spin->flags & SPIN_FLAG_SPIN) {
        spin->angleX = spin->angleX + (spin->stepX * D_800BE9E4);
        spin->angleY = spin->angleY + (spin->stepY * D_800BE9E4);
        spin->angleZ = spin->angleZ + (spin->stepZ * D_800BE9E4);

        waveX = func_151423D8((u8)(spin->angleX - 0x40));
        waveY = func_151423D8((u8)(spin->angleY - 0x40));
        {
            f32 waveZ;

            waveZ = func_151423D8((u8)(spin->angleZ - 0x40));
            particle->desc.offset.x = spin->amplitude.unk0 * waveX;
            particle->desc.offset.y = spin->amplitude.unk4 * waveY;
            particle->desc.offset.z = spin->amplitude.unk8 * waveZ;
        }
    }
    return 1;
}

/* Direction spread shared by the burst specs: func_15143794 takes a random angle from each range. */
typedef struct {
    s16 angle1;
    s16 angle1Range;
    s16 angle2;
    s16 angle2Range;
} BurstAngles;

/*
 * Parameters for func_15153F18, which throws count + rand(countRange) func_151DA6F8 splashes.
 * Each `x` / `xRange` pair is drawn as x + rand(xRange). func_15153F18 reads `angles` from its first
 * argument and everything else relative to its second (&spec->pos).
 */
typedef struct {
    BurstAngles angles;
    struct17 pos;
    f32 scale;
    f32 scaleRange;
    f32 size;
    f32 sizeRange;
    f32 speed;
    f32 speedRange;
    s16 count;
    s16 countRange;
    s16 unk30; /* splash arg6 */
    s16 unk30Range;
    s16 lifetime;
    s16 lifetimeRange;
    s16 unk38; /* splash arg4 */
    s16 unk38Range;
    s8 kind;
    u8 pad3D[3];
    f32 unk40Chance; /* probability of splash arg7 */
    s16 unk44;
    s16 unk46;
    s32 unk48;
} SplashBurstSpec; /* 0x4C */

/* Parameters for func_15150178, which throws count + rand(countRange) func_151D9014 particles. */
typedef struct {
    BurstAngles angles;
    struct17 pos;
    s16 count;
    s16 countRange;
    f32 speed;
    f32 speedRange;
    s16 lifetime;
    s16 lifetimeRange;
    f32 gravity;
    f32 gravityRange;
    u8 unk2C; /* particle arg5 */
    u8 unk2CRange;
    u8 pad2E[2];
    f32 size;
    f32 sizeRange;
    s8 unk38;
    u8 kind;
    u8 pad3A[2];
    f32 unk3CChance; /* probability of particle arg7 */
    s8 wobble;
    u8 pad41[3];
    f32 bounceChance;
} DropBurstSpec; /* 0x48 */

extern void func_15153F18(SplashBurstSpec *spec, struct17 *params, s32 arg2, u8 arg3, s32 arg4);
extern void func_15150178(DropBurstSpec *spec, struct17 *params, s32 arg2, u8 arg3, s32 arg4);

/* Throws `count` splashes of effect kind `kind` from `pos`. */
void func_151DBAA8(struct17 *pos, s32 count, u8 kind, u8 arg3, s32 arg4) {
    SplashBurstSpec splash;

    splash.pos = *pos;

    splash.count = count;

    splash.angles.angle1Range = 0xFF;
    splash.angles.angle2 = -64;
    splash.angles.angle2Range = 46;

    splash.countRange = 0;
    splash.angles.angle1 = 0;
    splash.unk30 = 3;
    splash.unk30Range = 2;
    splash.lifetime = 30;
    splash.lifetimeRange = 30;
    splash.unk38 = 155;
    splash.scale = 5.5f;

    splash.scaleRange = 2.9399998f;
    splash.size = 0.61300004f;
    splash.sizeRange = 0.60300004f;
    splash.speed = 10.0f;
    splash.speedRange = 10.2f;

    splash.unk38Range = 100;
    splash.unk44 = 16;
    splash.unk46 = 15;
    splash.unk48 = 0;
    splash.unk40Chance = 0.5f;

    splash.kind = kind;

    func_15153F18(&splash, &splash.pos, 0, arg3, arg4);

}

/* Spawns a func_151D9B8C emitter of effect kind *kind just above `pos`, with random size and lifetime. */
void func_151DBBD4(struct17 *pos, s32 arg1, u8 *kind, u8 arg3, s32 arg4) {
    struct17 point;
    u8 emitterKind;
    s32 pad; // matching: unused stack slot
    u32 rand;
    f32 sizeRand;

    point.unk0 = pos->unk0;
    point.unk4 = pos->unk4 + 5.0f;
    point.unk8 = pos->unk8;

    emitterKind = *kind;
    sizeRand = func_150ADA68();
    rand = func_150ADA20();

    func_151D9B8C(emitterKind, (sizeRand * 25.0f) + 10.0f, (rand % 56U) + 200, arg1 + 4, &point,
                  (func_150ADA20() % 151U) + 150, 0, 1, 0, arg3, arg4);
}


/* Spawns an emitter of effect kind `kind` (coloured from D_800AB414) at `pos`, via func_1513C73C when arg3 is set. */
void func_151DBCBC(u8 kind, f32 size, s16 arg2, s32 arg3, void *pos, u8 arg5, s32 arg6) {
    EmitterDesc desc;

    desc.unk6 = 0x38;
    desc.unkC = 0x8000;
    desc.flags = 0x67B02;
    desc.unk4 = 300;
    desc.unk8 = 0;
    desc.unk10 = arg2;
    desc.unk11 = 0xFF;
    desc.unk12 = D_800AB414[kind * 3];
    desc.unk13 = D_800AB414[kind * 3 + 1];
    desc.unk14 = D_800AB414[kind * 3 + 2];
    desc.unk15 = 0xFF;
    desc.unk18 = 0x440001;
    desc.unk7 = 0;
    desc.unk16 = 0;
    desc.unk17 = 6;
    desc.unk22 = 1;
    desc.unk24 = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk1C = 0;
    if (arg3 != 0) {
        func_1513C73C((s32)&desc, 0, 0, arg3, ((struct17 *)pos)->unk0, ((struct17 *)pos)->unk4,
                      ((struct17 *)pos)->unk8, size, size, func_150ADA20() & 0xFF,
                      (func_150ADA20() & 1) + (func_150ADA20() & 1) * 2, 0, arg5, arg6);
    } else {
        func_1513C5B0(&desc, 0, 0, 0, ((struct17 *)pos)->unk0, ((struct17 *)pos)->unk4, ((struct17 *)pos)->unk8,
                      size, size, 0, 0, 0, arg5, arg6);
    }
}

/* Starts an effect of kind `kind` (coloured from D_800AB414) at `pos`. */
void func_151DBE80(u8 kind, f32 size, f32 arg2, s16 lifetime, vertex *pos, s32 arg5, u8 arg6, u8 arg7, u8 arg8,
                   s32 arg9) {
    ParticleDef desc;

    desc.texture = 0x38;
    desc.renderModes = ((arg7 != 0) ? 2 : 1) + 0x440000; // matching: the s16 store truncates 0x440000 away, but dropping it changes the code
    desc.lifetime = lifetime;
    desc.frame = 0;
    desc.frameSpeed = 0x4000;
    desc.rgba[0] = D_800AB414[kind * 3];
    desc.width = 1.0f;
    desc.height = 1.0f;
    desc.rgba[1] = D_800AB414[kind * 3 + 1];
    desc.rgba[2] = D_800AB414[kind * 3 + 2];
    desc.rgba[3] = 0xFF;

    desc.pos = *pos;

    desc.scale.x = arg2;
    desc.flags = 0x466C0001;
    desc.unk44 = arg6;
    desc.unk45 = 0xFF;
    desc.type = 0;
    desc.unk46 = 0;
    desc.unk47 = 6;
    desc.unk48 = 0;
    desc.viewportMask = 0xFF;
    desc.unk50 = 0;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;

    func_1513D668((s32)&desc, 0, 11, 17, 0, (func_150ADA20() & 1) + (func_150ADA20() & 1),
                  (s16)(func_150ADA20() & 0xFF), size, size, 0, arg5, 0, 0, arg8, arg9);
}

extern u8 D_800AB320[];

/* Spawns a plain sprite particle of effect kind `kind` (id from D_800AB320) at `pos`. */
void func_151DC034(vertex *pos, f32 size, u8 arg2, s16 lifetime, u8 kind, u8 arg5, s32 arg6) {
    ParticleDesc desc;
    s32 flag800000;
    s32 flag40;
    s32 flag80;

    desc.texture = D_800AB320[kind];
    desc.renderMode = 0x4403;
    desc.unk0 = 0x200005;
    desc.unk4 = 0x9F0600;
    desc.timer = lifetime;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = 0xFF;
    desc.g = 0xFF;
    desc.b = 0xFF;
    desc.a = 0xFF;
    desc.r2 = 0xFF;
    desc.g2 = 0xFF;
    desc.b2 = 0xFF;
    desc.alpha = arg2;
    desc.brightness = 0xFF;
    desc.height = size;
    desc.width = size;
    desc.pos = *pos;
    desc.offset = *(vertex *)&D_800A5480;
    desc.vel = *(vertex *)&D_800A5480;
    desc.fadeStart = 4;
    desc.fadeRate = 63;
    desc.scaleStart = 1;
    desc.gravity = 0.0f;
    desc.scaleRate = 1.0f;
    flag80 = (func_150ADA20() & 1) ? 0x80 : 0;
    flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
    flag800000 = D_800AB330[kind] ? 0x800000 : 0;
    desc.flags
 = flag800000 | 1 | flag40 | flag80 | 0x4000 | 0x8000 | 0x200 | 0x10000;
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.updateFunc = -1;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 0;
    desc.viewMask = 0;
    desc.views = 0xFF;
    desc.minSize = 1000;
    desc.maxSize = 1000.0f;
    func_15130280(&desc, 1, 0, 0, arg5, arg6);
}

/* Throws a splash burst plus a burst of wobbling drops of effect kind `kind` from `pos`. */
void func_151DC260(struct17 *pos, s32 arg1, u8 kind, u8 arg3, s32 arg4) {
    SplashBurstSpec splash;

    splash.pos = *pos;
    splash.count = 10;
    splash.countRange = 10;
    splash.angles.angle1Range = 0xFF;
    splash.angles.angle2 = -64;
    splash.angles.angle2Range = 40;
    splash.scale = 3.0f;
    splash.angles.angle1 = 0;
    splash.unk30 = 3;
    splash.unk30Range = 1;
    splash.lifetime = 60;
    splash.lifetimeRange = 40;
    splash.unk38 = 100;
    splash.unk38Range = 100;
    splash.unk44 = 16;
    splash.unk46 = 15;
    splash.unk48 = 0;
    splash.scaleRange = 4.0f;
    splash.size = 0.80700004f;
    splash.sizeRange = 0.51000005f;
    splash.speed = 12.0f;
    splash.speedRange = 11.0f;
    splash.unk40Chance = 1.0f;
    splash.kind = kind;
    func_15153F18(&splash, &splash.pos, arg1, arg3, arg4);

    {
        DropBurstSpec drops;

        drops.pos = *pos;
        drops.angles.angle1 = 0;
        drops.count = 12;
        drops.speed = 6.0f;
        drops.countRange = 6;
        drops.angles.angle1Range = 0xFF;
        drops.angles.angle2 = -64;
        drops.angles.angle2Range = 36;
        drops.lifetime = 35;
        drops.lifetimeRange = 15;
        drops.unk2C = 155;
        drops.unk2CRange = 100;
        drops.speedRange = 8.0f;
        drops.unk38 = 1;
        drops.gravity = -1.266f;
        drops.gravityRange = 0.46600002f;
        drops.kind = kind;
        drops.size = 123.0f;
        drops.sizeRange = 134.0f;
        drops.wobble = 1;
        drops.unk3CChance = 0.6f;
        drops.bounceChance = 0.0f;
        func_15150178(&drops, &drops.pos, arg1, arg3, arg4);
    }
}

/* Smaller variant of func_151DC260: splash burst plus wobbling drops of effect kind `kind`. */
void func_151DC484(struct17 *pos, s32 arg1, u8 kind, u8 arg3, s32 arg4) {
    SplashBurstSpec splash;

    splash.pos = *pos;
    splash.count = 8;
    splash.countRange = 6;
    splash.angles.angle1Range = 0xFF;
    splash.angles.angle2 = -64;
    splash.scale = 3.0f;
    splash.angles.angle1 = 0;
    splash.angles.angle2Range = 40;
    splash.unk30 = 3;
    splash.unk30Range = 0;
    splash.lifetime = 60;
    splash.lifetimeRange = 40;
    splash.unk38 = 100;
    splash.unk38Range = 100;
    splash.unk44 = 16;
    splash.unk46 = 15;
    splash.unk48 = 0;
    splash.scaleRange = 2.0f;
    splash.size = 0.642f;
    splash.sizeRange = 0.314f;
    splash.speed = 8.0f;
    splash.speedRange = 5.0f;
    splash.unk40Chance = 1.0f;
    splash.kind = kind;
    func_15153F18(&splash, &splash.pos, arg1, arg3, arg4);

    {
        DropBurstSpec drops;

        drops.pos = *pos;
        drops.count = 12;
        drops.countRange = 6;
        drops.angles.angle1Range = 0xFF;
        drops.angles.angle1 = 0;
        drops.angles.angle2 = -64;
        drops.angles.angle2Range = 26;
        drops.lifetime = 35;
        drops.lifetimeRange = 15;
        drops.unk2C = 155;
        drops.unk2CRange = 100;
        drops.size = 59.0f;
        drops.sizeRange = 59.0f;
        drops.unk38 = 1;
        drops.speed = 7.0f;
        drops.speedRange = 3.0f;
        drops.gravity = -1.23f;
        drops.gravityRange = 0.47500002f;
        drops.kind = kind;
        drops.wobble = 1;
        drops.unk3CChance = 0.6f;
        drops.bounceChance = 0.0f;
        func_15150178(&drops, &drops.pos, arg1, arg3, arg4);
    }
}
