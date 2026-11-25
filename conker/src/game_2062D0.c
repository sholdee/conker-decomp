#include <ultra64.h>

#define func_150ADA20 func_150ADA20_orig
#define func_1513D668 func_1513D668_void_proto
#define func_151DA08C func_151DA08C_void_proto
#include "functions.h"
#undef func_150ADA20
#undef func_1513D668
#undef func_151DA08C
#include "variables.h"

s32 func_150ADA20(void);
s32 func_1513D668(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, f32 arg7, f32 arg8, s32 arg9, s32 argA, u8 argB, s32 argC, u8 argD, s32 argE);
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, struct17 *pos, s32 arg7, s32 arg8);
s32 func_1514672C(struct17 *arg0);
s32 func_15046C80(f32 *arg0, s32 arg1, f32 arg2, void *arg3);
void func_151D9FC0(u8 arg0, f32 arg1, u8 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
void func_151DAB58(u8 arg0, f32 arg1, u8 arg2, struct17 *arg3, volatile u8 arg4, u8 arg5, s32 arg6);
s32 func_15131918(s32 arg0, s32 arg1);
struct three;
struct struct218XXX;
extern void func_15153F18(struct struct218XXX *arg0, struct three *arg1, s32 arg2, u8 arg3, s32 arg4);

extern u8 (*D_8008FCD0[])(void);
extern u8 (*D_8008FD04[])(void);
extern u8 D_800AB330[];
extern u8 D_800AB414[];
extern f32 D_800AB44C;
extern f32 D_800AB450;
extern f32 D_800AB454;
extern f32 D_800AB458;
extern f32 D_800AB45C;
extern f32 D_800AB460;
extern f32 D_800AB470;
extern f32 D_800AB474;
extern f32 D_800AB478;
extern f32 D_800AB47C;
extern f32 D_800AB4A0;
extern f32 D_800AB4A4;
extern f32 D_800AB4A8;
extern f32 D_800AB4AC;


/* Effect descriptor consumed by func_1513D668 and func_1513D2F0 (copied to EffectBase.desc). */
typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 scaleX;
    f32 scaleY;
    struct17 pos;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34; /* scaleX multiplier */
    f32 unk38; /* scaleY multiplier */
    f32 unk3C;
    s32 flags;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} EffectDesc; /* 0x58 */

/* Start of a live effect created from an EffectDesc. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    EffectDesc desc;
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

/* Sprite particle descriptor, consumed by func_15130280 (copied to Particle.desc). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 scaleX;
    f32 scaleY;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    s32 flags;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} ParticleDesc; /* 0x70 */

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
    ParticleWobble wobble;
} Particle;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
s32 func_151D9450(Particle *particle, struct17 *arg1);
u8 func_151D9534(Particle *particle, struct17 *arg1);

/* Spawns a wobbling sprite particle of effect kind `kind`; lifetime -99 means it never times out. */
Particle *func_151D9014(struct17 *pos, struct17 *vel, u8 kind, f32 arg3, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                        f32 arg8, f32 arg9, u8 argA, s32 argB, s32 argC, s32 argD, u8 argE, s32 argF) {
    Particle *particle;
    ParticleDesc desc;
    ParticleWobble wobble;

    desc.unk1D = D_8008FCD0[kind]();
    desc.unk8 = ((argA ? 0x44 : 0x22) << 8) + 3;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.unkC = 0;
    desc.unk10 = 0;
    desc.unk14 = 0;
    desc.unk15 = 0;
    desc.unk16 = 0;
    desc.unk17 = 0xFF;
    desc.unk18 = 0;
    desc.unk19 = 0;
    desc.unk1A = 0;
    desc.unk1B = arg5;
    desc.unk1C = 0xFF;
    desc.pos = *pos;
    desc.unk3C = *(struct17 *)&D_800A5480;
    desc.vel = *vel;
    desc.unk22 = 1;
    desc.unk54 = arg3;
    desc.unk24 = 1.0f;
    desc.flags = ((lifetime == -99) ? 0 : 1) | 2 | 4 | ((func_150ADA20() & 1) ? 0x40 : 0) |
                 ((func_150ADA20() & 1) ? 0x80 : 0) | 0x200 | 0x4000 | 0x8000 | (argA ? 0x10000 : 0) |
                 (D_800AB330[kind] ? 0x800000 : 0);
    if (lifetime == -99) {
        desc.unkA = 300;
        desc.unk1E = 1;
        desc.unk20 = 0xFF;
    } else {
        desc.unkA = lifetime + 18;
        desc.unk1E = 18;
        desc.unk20 = 14;
    }
    desc.unk60 = 6;
    desc.unk61 = 8;
    desc.unk64 = -1;
    desc.unk62 = (u8)arg7 ? 0x21 : 0x22;
    desc.unk63 = -1;
    desc.unk65 = 0;
    desc.unk5C = 0;
    desc.unk66 = 0xFF;
    desc.unk68 = 0;
    desc.unk6C = 10000.0f;
    wobble.phaseX = func_150ADA20() & 0xFF;
    wobble.phaseY = func_150ADA20() & 0xFF;
    if (desc.unk1D == 0x40 || desc.unk1D == 0x67) {
        wobble.size = desc.scaleX = desc.scaleY = size * 0.7f;
    } else {
        wobble.size = desc.scaleX = desc.scaleY = size;
    }
    wobble.stepX = (func_150ADA20() % 6U) + 5;
    wobble.stepY = (func_150ADA20() % 6U) + 5;
    wobble.amplitudeX = ((func_150ADA68() * 0.25f) + 0.2f) * wobble.size;
    wobble.amplitudeY = ((func_150ADA68() * 0.25f) + 0.2f) * wobble.size;
    wobble.burstScale = arg8;
    wobble.kind = kind;
    wobble.altBurstScale = arg9;
    wobble.flags = ((u8)argC ? 0 : WOBBLE_FLAG_FIXED_SIZE) | ((u8)argD ? WOBBLE_FLAG_BOUNCE : 0);
    particle = func_15130280(&desc, 1, argB, sizeof(wobble), argE, argF);
    if (particle != NULL) {
        memcpy(&particle->wobble, &wobble, sizeof(wobble));
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

    wobble = &particle->wobble;
    if (wobble->flags & WOBBLE_FLAG_FIXED_SIZE) {
        return 1;
    }
    wobble->phaseX = wobble->phaseX + wobble->stepX * D_800BE9E4;
    wobble->phaseY = wobble->phaseY + wobble->stepY * D_800BE9E4;
    particle->desc.scaleX = wobble->amplitudeX * func_151423D8((u8)(wobble->phaseX - 0x40)) + wobble->size;
    particle->desc.scaleY = wobble->amplitudeY * func_151423D8((u8)(wobble->phaseY - 0x40)) + wobble->size;
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
    if (particle->desc.pos.unk4 < arg1->unk4) {
        point.unk0 = particle->desc.pos.unk0;
        point.unk4 = arg1->unk4;
        point.unk8 = particle->desc.pos.unk8;
        if (func_1514672C(&point) == 0) {
            return 0;
        }
        if (func_15046C80((f32 *)&point, 0, particle->desc.pos.unk4, &particle->floorY) != 0) {
            point.unk4 = particle->floorY + 2.0f;
            if (particle->wobble.flags & WOBBLE_FLAG_BOUNCE) {
                particle->desc.pos.unk4 = (particle->desc.scaleY * 0.060000002f) + point.unk4;
                particle->desc.vel.unk0 *= 0.39900002f;
                particle->desc.vel.unk4 *= -0.39900002f;
                particle->desc.vel.unk8 *= 0.39900002f;
                if (fabsf(particle->desc.vel.unk4) < 0.326f) {
                    particle->desc.vel.unk0 = 0.0f;
                    particle->desc.flags &= ~6;
                    particle->desc.vel.unk4 = 0.0f;
                    particle->desc.vel.unk8 = 0.0f;
                    particle->desc.unk54 = 0.0f;
                }
            } else {
                alive = 0;
                wobble = &particle->wobble;
                avgScale = (particle->desc.scaleX + particle->desc.scaleY) * 0.5f;
                if (particle->unk9D == 3) {
                    func_151D9FC0(wobble->kind, wobble->altBurstScale * avgScale, particle->desc.unk1B,
                                  (s32)&particle->unk84, (s32)&point, particle->unkC, particle->unk1);
                } else if (func_150ADA20() & 1) {
                    func_151D9B8C(wobble->kind, (avgScale * 0.09f) * wobble->burstScale, particle->desc.unk1B,
                                  (s32)&particle->unk84, &point, 100, 0, 1, 0, particle->unkC, particle->unk1);
                } else {
                    func_151DAB58(wobble->kind, (avgScale * 0.025f) * wobble->burstScale, particle->desc.unk1B, &point,
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
    struct17 spA0;
    struct17 sp94;
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
        arg8 && func_15144E80(arg3, &spA0, &sp94, &normal)) {
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
                                   (struct37 *)sizeof(drip), arg9, argA);
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
void func_151D9FC0(u8 kind, f32 size, u8 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    func_151DBCBC(kind, size * 0.5f, arg2, arg3, arg4, arg5, arg6);
    if ((kind != 5) && (kind != 2)) {
        func_151DA08C(kind, size * 0.0003f, 1.01f, arg2, 100, arg3, arg4, arg5, arg6);
    }
}

/* Starts a steam effect of kind `kind` at `pos` (lifetime -1: untimed, shrinking by `shrink` otherwise). */
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, struct17 *pos, s32 arg7, s32 arg8) {
    s32 effect;
    EffectDesc desc;
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
    desc.unk0 = D_8008FD04[kind]();
    desc.unk2 = 3;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = 0;
    desc.unk11 = 0;
    desc.unk12 = 0;
    desc.unk13 = 0xFF;
    desc.scaleY = size;
    desc.scaleX = size;
    desc.pos = *pos;
    desc.flags = 0x0CDC0008;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    desc.unk28 = 0.0f;
    desc.unk2C = 0.0f;
    desc.unk30 = 0.0f;
    if (lifetime == -1) {
        desc.unk4 = 300;
        steam.shrink = 1.0f;
        desc.unk54 = 1;
        desc.unk56 = 0xFF;
    } else {
        desc.unk4 = lifetime;
        desc.flags = 0x0CDC0009;
        steam.shrink = shrink;
        if (lifetime >= 0x100) {
            desc.unk54 = 0xFF;
        } else {
            desc.unk54 = lifetime;
        }
        if (lifetime >= 0x100) {
            desc.unk56 = 1;
        } else {
            desc.unk56 = 0xFF / lifetime;
        }
    }
    desc.unk44 = arg3;
    desc.unk45 = 0xFF;
    desc.unk1 = 0;
    desc.unk46 = 4;
    desc.unk47 = 5;
    desc.unk48 = 0;
    desc.unk4C = 0xFF;
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

void *func_151DB5D0(s32, f32 *, f32 *, f32, f32, f32, f32, s16, u8, s16, s16, u8, u8, s32);

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
    effect->base.desc.scaleX = func_151423D8(steam->phaseX - 0x40) * steam->amplitudeX + steam->size;
    effect->base.desc.scaleY = func_151423D8(steam->phaseY - 0x40) * steam->amplitudeY + steam->size;
lbl: // matching: ends the basic block so the scaleY store is not moved into the next call's delay slot
    steam->spawnCredit += (0.15f + func_150ADA68() * 0.1f) * D_800BE9A4;
    avgScale = (effect->base.desc.scaleX + effect->base.desc.scaleY) * 0.5f;
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

extern void func_151DAB58(u8, f32, u8, struct17 *, volatile u8, u8, s32);

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
                  arg5, (s32)&point, splash->unkC, splash->unk1);
    state->unk20 = 4;
    return 1;
}
typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} Struct151DAB58Local;

typedef struct {
    u8 field_0x00;
    u8 field_0x01;
    u8 pad_0x02[0x2];
    f32 field_0x04;
    f32 field_0x08;
} Struct151DAB58Extra;

typedef struct {
    u8 field_0x00;
    u8 pad_0x01[0x3];
    s32 field_0x04;
    u8 field_0x08;
    u8 field_0x09;
    u8 field_0x0A;
    u8 field_0x0B;
    u8 field_0x0C;
    u8 field_0x0D;
    u8 pad_0x0E[0x2];
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
} AxisOscillator;

typedef struct {
    u8 pad_0x00[0x4C];
    f32 field_0x4C;
    f32 field_0x50;
    f32 field_0x54;
    u8 field_0x58;
    u8 pad_0x59[0x4F];
    AxisOscillator field_0xA8;
    u8 pad_0xC4[0x4C];
    Struct151DAB58Extra field_0x110;
} AxisOscillatorOwner;

typedef struct {
    u32 field_0x00;
} FrameDeltaWord;

void func_151DAB58(u8 arg0, f32 arg1, u8 arg2, struct17 *arg3, volatile u8 arg4, u8 arg5, s32 arg6) {
    void *ret;
    Struct151DAB58Local sp54;
    Struct151DAB58Extra sp48;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_t0;

    sp54.unk0 = D_8008FCD0[arg0]();
    if (arg4 != 0) {
        temp_v0 = 0x3B;
    } else {
        temp_v0 = 0x22;
    }
    sp54.unk2 = (temp_v0 << 8) + 3;
    sp54.unk4 = 0x64;
    sp54.unk12 = 0;
    sp54.unk11 = 0;
    sp54.unk10 = 0;
    sp54.unk13 = 0xFF;
    sp54.unk44 = arg2;
    sp54.unk45 = 0xFF;
    sp54.unk1C = *arg3;
    temp_v0 = D_800AB330[arg0] != 0 ? 0x40000000 : 0;
    sp54.unk40 = temp_v0 | 0x0CDC0009;
    sp54.unk28 = 0.0f;
    sp54.unk2C = 0.0f;
    sp54.unk30 = 0.0f;
    sp54.unk34 = 1.0f;
    sp54.unk38 = 1.0f;
    sp54.unk3C = 1.0f;
    sp54.unk8 = 0;
    sp54.unkC = 0;
    sp54.unk18 = arg1;
    sp54.unk14 = arg1;
    sp54.unk46 = 0;
    sp54.unk47 = 7;
    sp48.field_0x00 = 0;
    sp48.field_0x01 = (func_150ADA20() % 3U) + 6;
    sp48.field_0x04 = (func_150ADA68() * D_800AB4A0) + D_800AB4A4;
    sp48.field_0x08 = (func_150ADA68() * D_800AB4A8) + D_800AB4AC;
    sp54.unk1 = 0;
    sp54.unk48 = 0;
    sp54.unk4C = 0xFF;
    sp54.unk50 = 0;
    sp54.unk54 = 0x20;
    sp54.unk56 = 7;
    if (func_150ADA20() & 1) {
        temp_v1 = 1;
    } else {
        temp_v1 = 0;
    }
    temp_v0 = *(u8 *)&arg4;
    if (temp_v0 != 0) {
        temp_t0 = 3;
    } else {
        temp_t0 = 0;
    }
    if (temp_v0 != 0) {
        temp_v0 = 0xFF;
    } else {
        temp_v0 = 0;
    }
    ret = func_1513D2F0(&sp54, (s32)&D_800A4AA0, 0, 0x14, 0, 0xE, temp_v1 | 2, temp_t0, temp_v0, 0xC, arg5, arg6);
    if (ret != 0) {
        memcpy((u8 *)ret + 0x110, &sp48, sizeof(sp48));
    }
}
extern f32 D_800AB4B0;

s32 func_151DADA0(AxisOscillatorOwner *arg0) {
    f32 f0;
    Struct151DAB58Extra *p;

    p = &arg0->field_0x110;
    p->field_0x00 = p->field_0x00 + (s8)p->field_0x01 * D_800BE9E4;
    f0 = func_151423D8((u8)(p->field_0x00 - 0x40));
    arg0->field_0x4C = p->field_0x04 * f0 + 1.0f;
    arg0->field_0x50 = D_800AB4B0 - p->field_0x08 * f0;
    return 1;
}

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0xC];
    f32 unk4C;
    f32 unk50;
    char pad54[0x6C];
    char unkC0[0x40];
    s16 *unk100[1];
} Struct151DAE28;

s16 *func_151DAE28(Struct151DAE28 *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f0;
    f32 temp_f14;
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

    temp_f0 = arg0->unk2C * arg0->unk4C;
    temp_f14 = arg0->unk30 * arg0->unk50;
    temp_f2 = D_800DD1D8[arg1] * temp_f0;
    temp_f12 = D_800DD1E8[arg1] * temp_f0;

    var_v0[3] = 0;
    var_v0[0xB] = 0;
    var_v0[0x13] = 0;
    var_v0[0x1B] = 0;
    var_v0[0] = var_v0[0x18] = arg0->unk34 + temp_f12;
    var_v0[1] = var_v0[9] = arg0->unk38;
    var_v0[2] = var_v0[0x1A] = arg0->unk3C - temp_f2;
    var_v0[8] = var_v0[0x10] = arg0->unk34 - temp_f12;
    var_v0[0x11] = var_v0[0x19] = arg0->unk38 + temp_f14;
    var_v0[0xA] = var_v0[0x12] = arg0->unk3C + temp_f2;

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

typedef struct {
    f32 x, y, z;
} Vec3f151DB5D0;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec3f151DB5D0 unk30;
    Vec3f151DB5D0 unk3C;
    Vec3f151DB5D0 unk48;
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} Spawn151DB5D0;

typedef struct {
    u8 unk0;
    f32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    s8 unkB;
    s8 unkC;
    s8 unkD;
    Vec3f151DB5D0 unk10;
} Extra151DB5D0;

extern void (*D_8008FD38[])(Spawn151DB5D0 *);

void *func_151DB5D0(s32 arg0, f32 *arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s16 arg7, u8 arg8,
                    s16 arg9, s16 argA, u8 argB, u8 argC, s32 argD) {
    Spawn151DB5D0 spawn;
    void *ret;
    Extra151DB5D0 extra;

    extra.unk0 = (argB ? 1 : 0) | ((arg5 == 1.0f) ? 0 : 2);
    extra.unk4 = arg5;
    extra.unk8 = 0;
    extra.unk9 = 0;
    extra.unkA = 0;
    if (argB) {
        extra.unkB = (func_150ADA20() % 7U) - 3;
        extra.unkC = (func_150ADA20() % 7U) - 3;
        extra.unkD = (func_150ADA20() % 7U) - 3;
        extra.unk10.x = (func_150ADA68() * 0.05000000075f + 0.05000000075f) * arg3;
        extra.unk10.y = (func_150ADA68() * 0.02999999933f + 0.02999999933f) * arg3;
        extra.unk10.z = (func_150ADA68() * 0.05000000075f + 0.05000000075f) * arg3;
    } else {
        extra.unkB = 0;
        extra.unkC = 0;
        extra.unkD = 0;
        extra.unk10 = *(Vec3f151DB5D0 *)&D_800A5480;
    }
    D_8008FD38[(u8)arg0](&spawn);
    spawn.unk1D = 0x6C;
    spawn.unk8 = 0x5103;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x9F0600;
    spawn.unkA = (arg7 == -1) ? 300 : arg7;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk17 = 0xFF;
    spawn.unk28 = spawn.unk2C = arg3;
    spawn.unk1B = arg8;
    spawn.unk1C = 0xFF;
    spawn.unk30 = *(Vec3f151DB5D0 *)arg1;
    spawn.unk3C = *(Vec3f151DB5D0 *)&D_800A5480;
    spawn.unk48 = *(Vec3f151DB5D0 *)arg2;
    spawn.unk54 = arg6;
    spawn.unk1E = arg9;
    spawn.unk20 = argA;
    spawn.unk22 = (arg7 == -1) ? 1 : arg7;
    spawn.unk24 = arg4;
    spawn.unk58 = ((arg7 == -1) ? 0 : 1) | 2 | 4 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x200 | 0x400 | 0x800 | (argB ? 0x1000 : 0) | 0x4000 | 0x8000 | 0x40000 | 0x800000;
    spawn.unk60 = 8;
    spawn.unk61 = 6;
    spawn.unk62 = 5;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk68 = 1000;
    spawn.unk6C = 1000.0f;
    ret = func_15130280(&spawn, 1, 0, 0x1C, argC, argD);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &extra, 0x1C);
    }
    return ret;
}

s32 func_151DB97C(AxisOscillatorOwner *arg0, s32 arg1) {
    AxisOscillator *p;
    volatile FrameDeltaWord *delta;
    f32 sp24;
    f32 sp20;

    if (!(arg0->field_0xA8.field_0x00 & 2)) {
        p = &arg0->field_0xA8;
    } else {
        func_15131918((s32)&arg0->field_0x58, arg0->field_0xA8.field_0x04);
        p = &arg0->field_0xA8;
    }

    delta = (volatile FrameDeltaWord *)&D_800BE9E4;
    if (p->field_0x00 & 1) {
        p->field_0x08 = p->field_0x08 + (p->field_0x0B * delta->field_0x00);
        p->field_0x09 = p->field_0x09 + (p->field_0x0C * delta->field_0x00);
        p->field_0x0A = p->field_0x0A + (p->field_0x0D * delta->field_0x00);

        sp20 = func_151423D8((u8)(p->field_0x08 - 0x40));
        sp24 = func_151423D8((u8)(p->field_0x09 - 0x40));
        {
            f32 sp28;

            sp28 = func_151423D8((u8)(p->field_0x0A - 0x40));
            arg0->field_0x4C = p->field_0x10 * sp20;
            arg0->field_0x50 = p->field_0x14 * sp24;
            arg0->field_0x54 = p->field_0x18 * sp28;
        }
    }
    return 1;
}

struct three {
    s32 a;
    s32 b;
    s32 c;
};

struct struct218XXX {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct three unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    u8 pad3D[0x3];
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
};

void func_151DBAA8(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct struct218XXX tmp;
    struct three *tmpVec;

    tmpVec = &tmp.unk8;
    *tmpVec = *(struct three *)arg0;

    tmp.unk2C = arg1;

    tmp.unk2 = 0xFF;
    tmp.unk4 = -0x40;
    tmp.unk6 = 0x2E;

    tmp.unk2E = 0;
    tmp.unk0 = 0;
    tmp.unk30 = 3;
    tmp.unk32 = 2;
    tmp.unk34 = 0x1E;
    tmp.unk36 = 0x1E;
    tmp.unk38 = 0x9B;
    tmp.unk14 = 5.5f;

    tmp.unk18 = D_800AB4C0;
    tmp.unk1C = D_800AB4C4;
    tmp.unk20 = D_800AB4C8;
    tmp.unk24 = 10.0f;
    tmp.unk28 = D_800AB4CC;

    tmp.unk3A = 0x64;
    tmp.unk44 = 0x10;
    tmp.unk46 = 0xF;
    tmp.unk48 = 0;
    tmp.unk40 = 0.5f;

    tmp.unk3C = arg2;

    func_15153F18(&tmp, tmpVec, 0, arg3, arg4);
}

// typedef struct {
//     s16 unk0; // sp24
//     s16 unk2;
//     s16 unk4;
//     s16 unk6;
//     // s32 unk8[3]; // sp2C
//     s32 unk8;
//     s32 unkC;
//     s32 unk10;
//     f32 unk14;
//     f32 unk18;
//     f32 unk1C;
//     f32 unk20; // sp44
//     f32 unk24;
//     f32 unk28;
//     s16 unk2C; // sp50
//     s16 unk2E;
//     s16 unk30; // sp54
//     s16 unk32;
//     s16 unk34;
//     s16 unk36;
//     s16 unk38;
//     s16 unk3A;
//     s8  unk3C; // sp60
//     u8  pad3D[0x3];
//     f32 unk40; // sp64
//     s16 unk44;
//     s16 unk46;
//     s32 unk48;
// } struct218XXX;
//
// void func_15153F18(s32, s32, s32, u8, s32);
// void func_151DBAA8(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
//
//     struct218XXX tmp;
//
//     // tmp.unk8[0] = arg0->unk0;
//     // tmp.unk8[1] = arg0->unk4;
//     // tmp.unk8[2] = arg0->unk8;
//
//     tmp.unk2C = arg1;
//     tmp.unk3C = arg2;
//
//     tmp.unk0 = 0;
//     tmp.unk2 = 0xFF;
//     tmp.unk4 = -0x40;
//     tmp.unk6 = 0x2E;
//     tmp.unk8 = arg0->unk0;
//     tmp.unkC = arg0->unk4;
//     tmp.unk10 = arg0->unk8;
//
//     tmp.unk30 = 3;
//     tmp.unk2E = 0;
//     tmp.unk32 = 2;
//     tmp.unk34 = 0x1E;
//     tmp.unk36 = 0x1E;
//     tmp.unk38 = 0x9B;
//     tmp.unk14 = 5.5f;
//
//     tmp.unk18 = D_800AB4C0;
//     tmp.unk1C = D_800AB4C4;
//     tmp.unk20 = D_800AB4C8;
//     tmp.unk24 = 10.0f;
//     tmp.unk28 = D_800AB4CC;
//
//     tmp.unk3A = 0x64;
//     tmp.unk44 = 0x10;
//     tmp.unk46 = 0xF;
//     tmp.unk48 = 0;
//     tmp.unk40 = 0.5f;
//
//     func_15153F18(&tmp, &tmp.unk8, 0, arg3, arg4);
// }

void func_151DBBD4(struct17 *arg0, s32 arg1, u8 *arg2, u8 arg3, s32 arg4) {
    struct17 tmp;
    struct217 tmp2;

    tmp.unk0 = arg0->unk0;
    tmp.unk4 = arg0->unk4 + 5.0f;
    tmp.unk8 = arg0->unk8;

    tmp2.unkF = *arg2;
    tmp2.unk0 = func_150ADA68();
    tmp2.unk4 = func_150ADA20();

    func_151D9B8C(tmp2.unkF, (tmp2.unk0 * 25.0f) + 10.0f, ((tmp2.unk4 % 0x38U) + 200), arg1 + 4, &tmp, (func_150ADA20() % 0x97U) + 150, 0, 1, 0, arg3, arg4);
}

struct Struct151DBCBC {
    s32 unk0;
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
};

void func_151DBCBC(u8 arg0, f32 arg1, s16 arg2, s32 arg3, void *arg4, u8 arg5, s32 arg6) {
    struct Struct151DBCBC tmp;

    tmp.unk6 = 0x38;
    tmp.unkC = 0x8000;
    tmp.unk0 = 0x67B02;
    tmp.unk4 = 0x12C;
    tmp.unk8 = 0;
    tmp.unk10 = arg2;
    tmp.unk11 = 0xFF;
    tmp.unk12 = D_800AB414[arg0 * 3];
    tmp.unk13 = D_800AB414[arg0 * 3 + 1];
    tmp.unk14 = D_800AB414[arg0 * 3 + 2];
    tmp.unk15 = 0xFF;
    tmp.unk18 = 0x440001;
    tmp.unk7 = 0;
    tmp.unk16 = 0;
    tmp.unk17 = 6;
    tmp.unk22 = 1;
    tmp.unk24 = 0xFF;
    tmp.unk20 = 0xFF;
    tmp.unk1C = 0;
    if (arg3 != 0) {
        func_1513C73C((s32)&tmp, 0, 0, arg3, ((struct17 *)arg4)->unk0, ((struct17 *)arg4)->unk4, ((struct17 *)arg4)->unk8, arg1, arg1,
                      func_150ADA20() & 0xFF, (func_150ADA20() & 1) + (func_150ADA20() & 1) * 2, 0, arg5, arg6);
    } else {
        func_1513C5B0((s32)&tmp, 0, 0, 0, ((struct17 *)arg4)->unk0, ((struct17 *)arg4)->unk4, ((struct17 *)arg4)->unk8, arg1, arg1, 0, 0, 0, arg5, arg6);
    }
}

struct Struct151DBE80 {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    s8 unk46;
    s8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    u16 unk56;
};

void func_151DBE80(u8 arg0, f32 arg1, f32 arg2, s16 arg3, struct17 *arg4, s32 arg5, u8 arg6, u8 arg7, u8 arg8, s32 arg9) {
    struct Struct151DBE80 tmp;

    tmp.unk0 = 0x38;
    tmp.unk2 = ((arg7 != 0) ? 2 : 1) + 0x440000;
    tmp.unk4 = arg3;
    tmp.unk8 = 0;
    tmp.unkC = 0x4000;
    tmp.unk10 = D_800AB414[arg0 * 3];
    tmp.unk14 = 1.0f;
    tmp.unk18 = 1.0f;
    tmp.unk11 = D_800AB414[arg0 * 3 + 1];
    tmp.unk12 = D_800AB414[arg0 * 3 + 2];
    tmp.unk13 = 0xFF;

    *(struct three *)&tmp.unk1C = *(struct three *)arg4;

    tmp.unk34 = arg2;
    tmp.unk40 = 0x466C0001;
    tmp.unk44 = arg6;
    tmp.unk45 = 0xFF;
    tmp.unk1 = 0;
    tmp.unk46 = 0;
    tmp.unk47 = 6;
    tmp.unk48 = 0;
    tmp.unk4C = 0xFF;
    tmp.unk50 = 0;
    tmp.unk54 = 1;
    tmp.unk56 = 0xFF;
    tmp.unk38 = 1.0f;
    tmp.unk3C = 1.0f;
    tmp.unk28 = 0.0f;
    tmp.unk2C = 0.0f;
    tmp.unk30 = 0.0f;

    func_1513D668((s32)&tmp, 0, 0xB, 0x11, 0, (func_150ADA20() & 1) + (func_150ADA20() & 1), (s16)(func_150ADA20() & 0xFF), arg1, arg1, 0, arg5, 0, 0, arg8, arg9);
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} Struct151DC034Local;

extern u8 D_800AB320[];

void func_151DC034(struct17 *arg0, f32 arg1, u8 arg2, s16 arg3, u8 arg4, u8 arg5, s32 arg6) {
    Struct151DC034Local sp30;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_t0;

    sp30.unk1D = D_800AB320[arg4];
    sp30.unk8 = 0x4403;
    sp30.unk0 = 0x200005;
    sp30.unk4 = 0x9F0600;
    sp30.unkA = arg3;
    sp30.unkC = 0;
    sp30.unk10 = 0;
    sp30.unk14 = 0xFF;
    sp30.unk15 = 0xFF;
    sp30.unk16 = 0xFF;
    sp30.unk17 = 0xFF;
    sp30.unk18 = 0xFF;
    sp30.unk19 = 0xFF;
    sp30.unk1A = 0xFF;
    sp30.unk1B = arg2;
    sp30.unk1C = 0xFF;
    sp30.unk2C = arg1;
    sp30.unk28 = arg1;
    sp30.unk30 = *arg0;
    sp30.unk3C = *(struct17 *)&D_800A5480;
    sp30.unk48 = *(struct17 *)&D_800A5480;
    sp30.unk1E = 4;
    sp30.unk20 = 0x3F;
    sp30.unk22 = 1;
    sp30.unk54 = 0.0f;
    sp30.unk24 = 1.0f;
    if (func_150ADA20() & 1) {
        temp_t0 = 0x80;
    } else {
        temp_t0 = 0;
    }
    if (func_150ADA20() & 1) {
        temp_v1 = 0x40;
    } else {
        temp_v1 = 0;
    }
    if (D_800AB330[arg4] != 0) {
        temp_v0 = 0x800000;
    } else {
        temp_v0 = 0;
    }
    sp30.unk58 = temp_v0 | 1 | temp_v1 | temp_t0 | 0x4000 | 0x8000 | 0x200 | 0x10000;
    sp30.unk60 = 6;
    sp30.unk61 = 8;
    sp30.unk62 = -1;
    sp30.unk63 = -1;
    sp30.unk64 = -1;
    sp30.unk65 = 0;
    sp30.unk5C = 0;
    sp30.unk66 = 0xFF;
    sp30.unk68 = 1000;
    sp30.unk6C = 1000.0f;
    func_15130280(&sp30, 1, 0, 0, arg5, arg6);
}
struct struct217XXX {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct three unk8;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    f32 unk30;
    f32 unk34;
    s8 unk38;
    u8 unk39;
    u8 pad3A[2];
    f32 unk3C;
    s8 unk40;
    u8 pad41[3];
    f32 unk44;
};

extern void func_15150178(struct struct217XXX *arg0, struct three *arg1, s32 arg2, u8 arg3, s32 arg4);

void func_151DC260(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct struct218XXX tmp;

    tmp.unk8 = *(struct three *)arg0;
    tmp.unk2C = 0xA;
    tmp.unk2E = 0xA;
    tmp.unk2 = 0xFF;
    tmp.unk4 = -0x40;
    tmp.unk6 = 0x28;
    tmp.unk14 = 3.0f;
    tmp.unk0 = 0;
    tmp.unk30 = 3;
    tmp.unk32 = 1;
    tmp.unk34 = 0x3C;
    tmp.unk36 = 0x28;
    tmp.unk38 = 0x64;
    tmp.unk3A = 0x64;
    tmp.unk44 = 0x10;
    tmp.unk46 = 0xF;
    tmp.unk48 = 0;
    tmp.unk18 = 4.0f;
    tmp.unk1C = 0.807000041f;
    tmp.unk20 = 0.5100000501f;
    tmp.unk24 = 12.0f;
    tmp.unk28 = 11.0f;
    tmp.unk40 = 1.0f;
    tmp.unk3C = arg2;
    func_15153F18(&tmp, &tmp.unk8, arg1, arg3, arg4);

    {
        struct struct217XXX tmp2;

        tmp2.unk8 = *(struct three *)arg0;
        tmp2.unk0 = 0;
        tmp2.unk14 = 0xC;
        tmp2.unk18 = 6.0f;
        tmp2.unk16 = 6;
        tmp2.unk2 = 0xFF;
        tmp2.unk4 = -0x40;
        tmp2.unk6 = 0x24;
        tmp2.unk20 = 0x23;
        tmp2.unk22 = 0xF;
        tmp2.unk2C = 0x9B;
        tmp2.unk2D = 0x64;
        tmp2.unk1C = 8.0f;
        tmp2.unk38 = 1;
        tmp2.unk24 = -1.266000032f;
        tmp2.unk28 = 0.4660000205f;
        tmp2.unk39 = arg2;
        tmp2.unk30 = 123.0f;
        tmp2.unk34 = 134.0f;
        tmp2.unk40 = 1;
        tmp2.unk3C = 0.6000000238f;
        tmp2.unk44 = 0.0f;
        func_15150178(&tmp2, &tmp2.unk8, arg1, arg3, arg4);
    }
}
void func_151DC484(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct struct218XXX tmp;

    tmp.unk8 = *(struct three *)arg0;
    tmp.unk2C = 8;
    tmp.unk2E = 6;
    tmp.unk2 = 0xFF;
    tmp.unk4 = -0x40;
    tmp.unk14 = 3.0f;
    tmp.unk0 = 0;
    tmp.unk6 = 0x28;
    tmp.unk30 = 3;
    tmp.unk32 = 0;
    tmp.unk34 = 0x3C;
    tmp.unk36 = 0x28;
    tmp.unk38 = 0x64;
    tmp.unk3A = 0x64;
    tmp.unk44 = 0x10;
    tmp.unk46 = 0xF;
    tmp.unk48 = 0;
    tmp.unk18 = 2.0f;
    tmp.unk1C = 0.6420000196f;
    tmp.unk20 = 0.3140000105f;
    tmp.unk24 = 8.0f;
    tmp.unk28 = 5.0f;
    tmp.unk40 = 1.0f;
    tmp.unk3C = arg2;
    func_15153F18(&tmp, &tmp.unk8, arg1, arg3, arg4);

    {
        struct struct217XXX tmp2;

        tmp2.unk8 = *(struct three *)arg0;
        tmp2.unk14 = 0xC;
        tmp2.unk16 = 6;
        tmp2.unk2 = 0xFF;
        tmp2.unk0 = 0;
        tmp2.unk4 = -0x40;
        tmp2.unk6 = 0x1A;
        tmp2.unk20 = 0x23;
        tmp2.unk22 = 0xF;
        tmp2.unk2C = 0x9B;
        tmp2.unk2D = 0x64;
        tmp2.unk30 = 59.0f;
        tmp2.unk34 = 59.0f;
        tmp2.unk38 = 1;
        tmp2.unk18 = 7.0f;
        tmp2.unk1C = 3.0f;
        tmp2.unk24 = -1.230000019f;
        tmp2.unk28 = 0.4750000238f;
        tmp2.unk39 = arg2;
        tmp2.unk40 = 1;
        tmp2.unk3C = 0.6000000238f;
        tmp2.unk44 = 0.0f;
        func_15150178(&tmp2, &tmp2.unk8, arg1, arg3, arg4);
    }
}

// typedef struct {
//     s16 unk10; // sp24
//     s16 unk12; // sp26
//     s16 unk14;
//     s16 unk16; // sp2A
//     s32 unk18[3];
//     s16 unk24; // sp38;
//     s16 unk26; // sp3A
//     f32 unk28; // sp3C
//     f32 unk2C; // sp40
//     s16 unk30; // sp44
//     s16 unk32; // sp46
//     f32 unk34; // sp48;
//     f32 unk38; // sp4C;
//     s8  unk3C; // sp50;
//     s8  unk3D; // sp51;
//     u8  unk3E[2]; //
//     f32 unk40; // sp54
//     f32 unk44; // sp58
//     s8  unk48; // sp5C
//     u8  unk49; // sp5D
//     u8  unk4A[2];
//     f32 unk4C; // sp60
//     s8  unk50; // sp64
//     u8  pad51[3];
//     f32 unk54; // sp68
// } struct217;
//
// typedef struct {
//     s16 unk58; // sp6C
//     s16 unk5A; // sp6E
//     s16 unk5C; // sp70
//     s16 unk5E; // sp72
//     s32 unk60[3]; // sp74
//     f32 unk6C; // sp80
//     f32 unk70; // sp84
//     f32 unk74; // sp88
//     f32 unk78; // sp8C
//     f32 unk7C; // sp90
//     f32 unk80; // sp94
//     s16 unk84; // sp98
//     s16 unk86; // sp9A
//     s16 unk88; // sp9C
//     s16 unk8A; // sp9E
//     s16 unk8C; // spA0
//     s16 unk8E; // spA2
//     s16 unk90; // spA4
//     s16 unk92; // spA6
//     u8  unk94; // spA8;
//     u8  pad95[3];
//     f32 unk98; // spAC;
//     s16 unk9C; // spB0
//     s16 unk9E; // spB2
//     s32 unkA0; // spB4
// } struct218;
//
// void func_151DC484(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
//     struct218 tmp2;
//     struct217 tmp;
//
//     tmp2.unk60[0] = arg0->unk0; // sp74.unk0 = (s32) arg0->unk0;
//     tmp2.unk60[1] = arg0->unk4; // sp74.unk4 = (s32) arg0->unk4;
//     tmp2.unk60[2] = arg0->unk8; // sp74.unk8 = (s32) arg0->unk8;
//
//     tmp2.unk5A = 0xFF;   // sp6E = 0xFF;
//     tmp2.unk5C = -0x40;  // sp70 = -0x40;
//     tmp2.unk84 = 8;      // sp98 = 8;
//     tmp2.unk86 = 6;      // sp9A = 6;
//     tmp2.unk58 = 0;      // sp6C = 0;
//     tmp2.unk88 = 3;      // sp9C = 3;
//     tmp2.unk8A = 0;      // sp9E = 0;
//     tmp2.unk5E = 0x28;   // sp72 = 0x28;
//     tmp2.unk8C = 0x3C;   // spA0 = 0x3C;
//     tmp2.unk8E = 0x28;   // spA2 = 0x28;
//
//     tmp2.unk6C = 3.0f;   // sp80 = 3.0f;
//     tmp2.unk90 = 100;     // spA4 = 0x64;
//     tmp2.unk92 = 100;     // spA6 = 0x64;
//     tmp2.unk9C = 16;      // spB0 = 0x10;
//     tmp2.unk9E = 15;      // spB2 = 0xF;
//     tmp2.unk70 = 2.0f;       // sp84 = 2.0f;
//     tmp2.unk74 = D_800AB4E4; // sp88 = D_800AB4E4;
//     tmp2.unk78 = D_800AB4E8; // sp8C = D_800AB4E8;
//     tmp2.unk7C = 8.0f;       // sp90 = 8.0f;
//     tmp2.unk80 = 5.0f;       // sp94 = 5.0f;
//     tmp2.unk98 = 1.0f;       // spAC = 1.0f; 0x3f80
//     tmp2.unk94 = arg2;       // spA8 = arg2;
//     tmp2.unkA0 = 0;      // spB4 = 0;
//
//     // func_15153F18(&sp6C, &sp74, arg1, arg3, arg4);
//     func_15153F18(&tmp2, &tmp2, arg1, arg3, arg4);
//
//     tmp.unk18[0] = arg0->unk0; // sp2C.unk0 = (s32) arg0->unk0;
//     tmp.unk18[1] = arg0->unk4; // sp2C.unk4 = (s32) arg0->unk4;
//     tmp.unk18[2] = arg0->unk8; // sp2C.unk8 = (s32) arg0->unk8;
//
//     tmp.unk24 = 0xC; // sp38 = 0xC;
//     tmp.unk12 = 0xFF;   // sp26 = 0xFF;
//     tmp.unk26 = 6;      // sp3A = 6;
//     tmp.unk10 = 0;      // sp24 = 0;
//     tmp.unk14 = -0x40;  // sp28 = -0x40;
//     tmp.unk16 = 0x1A;   // sp2A = 0x1A;
//     tmp.unk30 = 0x23;   // sp44 = 0x23;
//     tmp.unk32 = 0xF;    // sp46 = 0xF;
//     tmp.unk3C = 0x9B;   // sp50 = 0x9B;
//     tmp.unk3D = 0x64;   // sp51 = 0x64;
//     tmp.unk40 = 59.0f;  // sp54 = 59.0f;
//     tmp.unk44 = 59.0f;  // sp58 = 59.0f;
//     tmp.unk48 = 1;      // sp5C = 1;
//     tmp.unk50 = 1;      // sp64 = 1;
//     tmp.unk54 = 0.0f;   // sp68 = 0.0f;
//     tmp.unk28 = 7.0f;   // sp3C = 7.0f;
//     tmp.unk2C = 3.0f;   // sp40 = 3.0f;
//     tmp.unk34 = D_800AB4EC; // sp48 = D_800AB4EC;
//     tmp.unk38 = D_800AB4F0; // sp4C = D_800AB4F0;
//     tmp.unk49 = arg2;       // sp5D = arg2;
//     tmp.unk4C = D_800AB4F4; // sp60 = D_800AB4F4;
//     // func_15150178(&sp24, &sp2C, arg1, arg3, arg4);
//     func_15150178(&tmp, &tmp.unk18, arg1, arg3, arg4);
// }
