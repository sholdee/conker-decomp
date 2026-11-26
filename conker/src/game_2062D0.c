#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/particle.h describes it. */
u8 D_800E0A10; /* the effect kind of the current context (particle_getContextEffectKind); game/Game/LevelInit/levelinit sets it */

// functions.h declares these three with other return types; this file uses the s32 versions below.
#include "functions.h"
#include "variables.h"

// particle_spawnKindSprite (game_2062D0.c) is declared in each file that calls it: game/Effects/Blood/blood.c was compiled with a
// different view of it.
SteamParticle *particle_spawnKindSprite(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
                             u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/* Per effect kind (0..12): sprite texture id, used by game_1BDC20 and game_1DD500 emitters. */
const u8 gKindSpriteTextures[13] = { 176, 164, 172, 169, 168, 170, 174, 175, 164, 177, 178, 171, 173 };
/* Per effect kind: texture id of a plain sprite particle (particle_spawnKindPlainSprite). */
static const u8 gKindPlainTextures[13] = { 119, 118, 29, 147, 91, 45, 103, 149, 119, 159, 179, 118, 97 };
/* Per effect kind flag. */
static const u8 D_800AB330[13] = { 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 };

/*
 * Per effect kind (0..12): texture id pickers. No header declares gKindParticleTexturePickers: game/Beam/AimedBeam/aimedbeam was compiled
 * with a view that passes the kind 12 picker two arguments (u8 slot by prototype), so each file that
 * reads it declares its own view.
 */
u8 (*gKindParticleTexturePickers[13])(void) = {
    func_151D8E6C, func_151D8EB0, func_151D8EBC, func_151D8EC8, func_151D8EFC, func_151D8F30, func_151D8F7C,
    func_151D8FB0, func_151D8FB0, func_151D8FBC, func_151D8FC8, func_151D8FD4, func_151D8FE0,
};
u8 (*D_8008FD04[13])(void) = {
    func_151D97A8, func_151D9878, func_151D98D0, func_151D9820, func_151D9918, func_151D9960, func_151D99C8,
    func_151D9A20, func_151D9A20, func_151D9A68, func_151D9AC0, func_151D9ADC, func_151D9B34,
};
/* Per effect kind: the colour setters of particle_spawnKindSprite's particles. */
static void (*gEffectKindColorFuncs[13])(ParticleDesc *) = {
    func_151DB004, func_151DB068, func_151DB0CC, func_151DB15C, func_151DB1EC, func_151DB27C, func_151DB2A8,
    func_151DB2CC, func_151DB330, func_151DB3D8, func_151DB43C, func_151DB068, func_151DB4CC,
};



/* Effect kind for the current context: D_800E0A10, or kind 10 when level is clear and zone_isActorOutsideId(0, 0x1C) fails. */
s32 particle_getContextEffectKind(void) {
    if ((level == 0) && (zone_isActorOutsideId(0, 0x1C) == 0)) {
        return 10;
    }
    return D_800E0A10;
}

/* Id picker for effect kind 0 in the gKindParticleTexturePickers table. */
u8 func_151D8E6C(void) {
    const u8 ids[3] = { 88, 64, 62 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 1 in the gKindParticleTexturePickers table. */
u8 func_151D8EB0(void) {
    return 117;
}

/* Id picker for effect kind 2 in the gKindParticleTexturePickers table. */
u8 func_151D8EBC(void) {
    return 29;
}

/* Id picker for effect kind 3 in the gKindParticleTexturePickers table. */
u8 func_151D8EC8(void) {
    return (func_150ADA20() & 1) ? 17 : 147;
}

/* Id picker for effect kind 4 in the gKindParticleTexturePickers table. */
u8 func_151D8EFC(void) {
    return (func_150ADA20() & 1) ? 90 : 91;
}

/* Id picker for effect kind 5 in the gKindParticleTexturePickers table. */
u8 func_151D8F30(void) {
    const u8 ids[5] = { 46, 48, 45, 51, 52 };
    return ids[func_150ADA20() % 5U];
}

/* Id picker for effect kind 6 in the gKindParticleTexturePickers table. */
u8 func_151D8F7C(void) {
    return (func_150ADA20() & 1) ? 102 : 103;
}

/* Id picker for effect kinds 7 and 8 in the gKindParticleTexturePickers table. */
u8 func_151D8FB0(void) {
    return 149;
}

/* Id picker for effect kind 9 in the gKindParticleTexturePickers table. */
u8 func_151D8FBC(void) {
    return 159;
}

/* Id picker for effect kind 10 in the gKindParticleTexturePickers table. */
u8 func_151D8FC8(void) {
    return 179;
}

/* Id picker for effect kind 11 in the gKindParticleTexturePickers table. */
u8 func_151D8FD4(void) {
    return 117;
}

/* Id picker for effect kind 12 in the gKindParticleTexturePickers table. */
u8 func_151D8FE0(void) {
    const u8 ids[4] = { 96, 97, 98, 99 };
    return ids[func_150ADA20() & 3];
}

#define WOBBLE_FLAG_FIXED_SIZE 1 /* skip the size wobble */
#define WOBBLE_FLAG_BOUNCE     2 /* bounce on the floor instead of bursting */

#define SPIN_FLAG_SPIN 1 /* oscillate desc.unk3C */
#define SPIN_FLAG_DRAG 2 /* scale the velocity by `drag` every frame */

/* Spawns a wobbling sprite particle of effect kind `kind`; lifetime -99 means it never times out. */
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                        f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces, u8 argE,
                        s32 argF) {
    SteamParticle *particle;
    ParticleDesc desc;
    SteamWobble wobble;

    desc.texture = gKindParticleTexturePickers[kind]();
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
    desc.offset = gZeroVec3f;
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
    particle = particle_newSpriteParticleWithFloor(&desc, 1, ground, sizeof(wobble), argE, argF);
    if (particle != NULL) {
        memcpy(&particle->data.wobble, &wobble, sizeof(wobble));
    }
    return particle;
}

/* Per-frame update of a particle_spawnSteamParticle particle; returns 0 once it should be removed. */
s32 particle_updateSteamParticle(SteamParticle *particle, Vec3f *arg1) {
    s32 alive;

    if (particle_wobbleSteamSize(particle, arg1) != 0) {
        if (particle_steamFloorHit(particle, arg1) != 0) {
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
s32 particle_wobbleSteamSize(SteamParticle *particle, Vec3f *arg1) {
    SteamWobble *wobble;

    wobble = &particle->data.wobble;
    if (wobble->flags & WOBBLE_FLAG_FIXED_SIZE) {
        return 1;
    }
    wobble->phaseX = wobble->phaseX + wobble->stepX * D_800BE9E4;
    wobble->phaseY = wobble->phaseY + wobble->stepY * D_800BE9E4;
    particle->head.desc.width = wobble->amplitudeX * math_cosBinAngleMasked(wobble->phaseX - 0x40) + wobble->size;
    particle->head.desc.height = wobble->amplitudeY * math_cosBinAngleMasked(wobble->phaseY - 0x40) + wobble->size;
    return 1;
}

/*
 * Floor collision: once the particle falls below arg1's height, finds the floor under it and either
 * bounces (WOBBLE_FLAG_BOUNCE) or bursts into smaller effects. Returns 0 when the particle is gone.
 */
u8 particle_steamFloorHit(SteamParticle *particle, Vec3f *arg1) {
    volatile u8 alive; // matching: IDO keeps the result in its stack slot and reloads it after each call
    Vec3f point;
    SteamWobble *wobble;
    f32 avgScale;

    alive = 1;
    if (particle->head.desc.pos.y < arg1->y) {
        point.x = particle->head.desc.pos.x;
        point.y = arg1->y;
        point.z = particle->head.desc.pos.z;
        if (math_isInWorldBounds(&point) == 0) {
            return 0;
        }
        if (collision_probeFloorSurfABSet0Cached(&point, 0,
 particle->head.desc.pos.y, &particle->head.floor) != 0) {
            point.y = particle->head.floor.height + 2.0f;
            if (particle->data.wobble.flags & WOBBLE_FLAG_BOUNCE) {
                particle->head.desc.pos.y = (particle->head.desc.height * 0.060000002f) + point.y;
                particle->head.desc.vel.x *= 0.39900002f;
                particle->head.desc.vel.y *= -0.39900002f;
                particle->head.desc.vel.z *= 0.39900002f;
                if (fabsf(particle->head.desc.vel.y) < 0.326f) {
                    particle->head.desc.vel.x = 0.0f;
                    particle->head.desc.flags &= ~6;
                    particle->head.desc.vel.y = 0.0f;
                    particle->head.desc.vel.z = 0.0f;
                    particle->head.desc.gravity = 0.0f;
                }
            } else {
                alive = 0;
                wobble = &particle->data.wobble;
                avgScale = (particle->head.desc.width + particle->head.desc.height) * 0.5f;
                if (particle->head.floor.source == 3) {
                    particle_spawnKindSplash(wobble->kind, wobble->altBurstScale * avgScale, particle->head.desc.alpha,
                                  &particle->head.floor.tri, &point, particle->head.header.owner, particle->head.header.kind);
                } else if (func_150ADA20() & 1) {
                    particle_spawnKindEmitter(wobble->kind, (avgScale * 0.09f) * wobble->burstScale, particle->head.desc.alpha,
                                  &particle->head.floor.tri, &point, 100, 0, 1, 0, particle->head.header.owner, particle->head.header.kind);
                } else {
                    particle_spawnSquash(wobble->kind, (avgScale * 0.025f) * wobble->burstScale, particle->head.desc.alpha, &point,
                                  1, particle->head.header.owner, particle->head.header.kind);
                }
            }
        }
    }
    return alive;
}

/* Id picker for effect kind 0 in the D_8008FD04 table. */
u8 func_151D97A8(void) {
    const s32 ids[7] = { 0, 1, 2, 58, 59, 60, 61 };
    return ids[func_150ADA20() % 7U];
}

/* Id picker for effect kind 3 in the D_8008FD04 table. */
u8 func_151D9820(void) {
    const s32 ids[3] = { 15, 16, 17 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 1 in the D_8008FD04 table. */
u8 func_151D9878(void) {
    const s32 ids[3] = { 143, 144, 145 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 2 in the D_8008FD04 table. */
u8 func_151D98D0(void) {
    const s32 ids[2] = { 29, 30 };
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 4 in the D_8008FD04 table. */
u8 func_151D9918(void) {
    const s32 ids[2] = { 8, 9 };
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 5 in the D_8008FD04 table. */
u8 func_151D9960(void) {
    const s32 ids[5] = { 49, 50, 51, 13, 14 };
    return ids[func_150ADA20() % 5U];
}

/* Id picker for effect kind 6 in the D_8008FD04 table. */
u8 func_151D99C8(void) {
    const s32 ids[3] = { 150, 151, 152 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kinds 7 and 8 in the D_8008FD04 table. */
u8 func_151D9A20(void) {
    const s32 ids[2] = { 27, 28 };
    return ids[func_150ADA20() & 1];
}

/* Id picker for effect kind 9 in the D_8008FD04 table. */
u8 func_151D9A68(void) {
    const s32 ids[3] = { 160, 161, 162 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 10 in the D_8008FD04 table. */
u8 func_151D9AC0(void) {
    const s32 ids[1] = { 179 };
    return ids[0];
}

/* Id picker for effect kind 11 in the D_8008FD04 table. */
u8 func_151D9ADC(void) {
    const s32 ids[3] = { 185, 186, 187 };
    return ids[func_150ADA20() % 3U];
}

/* Id picker for effect kind 12 in the D_8008FD04 table. */
u8 func_151D9B34(void) {
    const s32 ids[4] = { 96, 97, 98, 99 };
    return ids[func_150ADA20() & 3];
}


static const Vec3f D_800AB3E8 = { 0.0f, -1.0f, 0.0f }; /* straight down */

/*
 * Spawns an emitter of effect kind `kind` at `pos` (lifetime -1: default). With arg8 set and an
 * upward-facing surface under tri, also starts a DripEffect there.
 */
void particle_spawnKindEmitter(u8 kind, f32 size, u8 arg2, void *tri, Vec3f *pos, s16 lifetime, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, s32 argA) {
    SpriteDef desc;
    TimedEffect *effect;
    Vec3f outA;
    Vec3f outB;
    Vec3f normal;
    Vec3f down;

    desc.fadeStart = 32;
    desc.fadeRate = 7;
    desc.texture = D_8008FD04[kind]();
    desc.flags = ((lifetime == -1) ? 0 : 1) | (D_800AB330[kind] ? 0x20000 : 0) | 0x100 | 0x200 | 0x400 | 0x1000 |
                 0x2000 | 0x4000 | 0x8000;
    if (lifetime == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = lifetime;
    }
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.unk10 = arg2;
    desc.unk11 = 0xFF;
    desc.rgba[2] = 0;
    desc.rgba[1] = 0;
    desc.rgba[0] = 0;
    desc.rgba[3] = 0xFF;
    desc.renderModes = (arg6 ? 2 : 3) + ((arg7 ? 0x3B : 0x22) << 16);
    desc.unk7 = 0;
    desc.unk16 = 0;
    desc.unk17 = 7;
    desc.viewportMask = 0xFF;
    desc.unk1C = 0;
    if (particle_spawnSurfaceQuadSprite(&desc, 0, 0, tri, pos->x, pos->y, pos->z, size, size,
                      func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2),
                      arg7 ? 3 : 0, arg7 ? 0xFF : 0, 0, arg9, argA) != NULL &&
        arg8 && math_getTriEdges(tri, &outA, &outB, &normal)) {
        down = D_800AB3E8;
        math_vec3Normalize(&normal, &normal, NULL, NULL);
        if (math_vec3Dot(&normal, &down) < -0.5f) {
            s32 pad; // matching: stack slot that places drip at the target offset
            DripData drip;

            drip.timer = 0;
            drip.size = size * 4.5f;
            drip.pos = *pos;
            drip.unk14 = arg2;
            drip.unk15 = arg7;
            drip.kind = kind;
            effect = effect_newTimedEffect((lifetime == -1) ? 300 : lifetime, -1, 0x4A, -1, (lifetime == -1) ? 0 : 1, 0,
                                   sizeof(drip), arg9, argA);
            if (effect != NULL) {
                memcpy(&((DripEffect *)effect)->drip, &drip, sizeof(drip));
            }
        }
    }
}

/* DripEffect update: counts the timer down and drops a particle each time it expires. */
void particle_updateDripEffect(DripEffect *effect) {
    DripData *drip;
    u32 lifeRand;
    f32 rand;

    drip = &effect->drip;
    drip->timer = drip->timer - D_800BE9E4;
    if (drip->timer < 0) {
        rand = func_150ADA68();
        lifeRand = func_150ADA20();
        particle_spawnSteamParticle(&drip->pos, &gZeroVec3f, drip->kind, rand * 0.157f + -0.657f, (lifeRand % 41U) + 35,
                      drip->unk14, drip->size, 0, 1.0f, 1.0f, drip->unk15, 0, 1, 0, effect->base.header.owner, effect->base.header.kind);
        drip->timer = (func_150ADA20() % 111U) + 30;
    }
}

/* Splash for effect kind `kind`: an emitter plus, except for kinds 2 and 5, a particle_startSteamEffect effect. */
void particle_spawnKindSplash(u8 kind, f32 size, u8 arg2, CollisionTri *surface, Vec3f *pos, u8 arg5, s32 arg6) {
    particle_spawnKindMark(kind, size * 0.5f, arg2, surface, pos, arg5, arg6);
    if ((kind != 5) && (kind != 2)) {
        particle_startSteamEffect(kind, size * 0.0003f, 1.01f, arg2, 100, surface, pos, arg5, arg6);
    }
}

/* Starts a steam effect of kind `kind` at `pos` (lifetime -1: untimed, shrinking by `shrink` otherwise). */
SteamEffect *particle_startSteamEffect(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, CollisionTri *surface, Vec3f *pos,
                          s32 arg7, s32 arg8) {
    SteamEffect *effect;
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
    effect = particle_spawnAxisQuadParticleNoModel(&desc, 0, 14, 19, 0, (rand2 & 1) + (rand1 & 1), rand3 & 0xFF, 500.0f, 500.0f, NULL,
                           surface, 0, sizeof(steam), 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->steam, &steam, sizeof(steam));
    }
    return effect;
}

/* Steam effect update: wobbles the scale and emits particle_spawnKindSprite particles around the base. */
s32 particle_updateSteamEffect(SteamEffect *effect) {
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
    effect->head.def.width = math_cosBinAngleMasked(steam->phaseX - 0x40) * steam->amplitudeX + steam->size;
    effect->head.def.height = math_cosBinAngleMasked(steam->phaseY - 0x40) * steam->amplitudeY + steam->size;
lbl: // matching: ends the basic block so the scaleY store is not moved into the next call's delay slot
    steam->spawnCredit += (0.15f + func_150ADA68() * 0.1f) * gFrameDelta;
    avgScale = (effect->head.def.width + effect->head.def.height) * 0.5f;
    if (steam->spawnCredit > 1.0f) {
        radius = avgScale * 500.0f;
        do {
            angle = func_150ADA20() & 0xFF;
            dist = func_150ADA68() * radius;
            c = math_cosBinAngleMasked(angle - 0x40);
            // matching: indexing the position as an array orders the add's operands like the target
            pos[0] = math_cosBinAngleMasked(angle) * dist + ((f32 *)&effect->head.def.pos)[0];
            pos[1] = ((f32 *)&effect->head.def.pos)[1] + -3.5f;
            pos[2] = ((f32 *)&effect->head.def.pos)[2] + c * dist;
            {
                s16 life;

                vel[0] = 0.0f;
                vel[1] = 0.0f;
                vel[2] = 0.0f;
                life = (func_150ADA20() % 51U) + 150;
                dist = func_150ADA68();
                particle_spawnKindSprite(steam->kind, pos, vel, avgScale * 471.0f, (dist * 154.0f + 10106.0f) * 0.0001f, 1.0f,
                              (func_150ADA68() * 25.0f + -50.0f) * 0.001f, life, 0x9B, life, 0xFF / life, 1,
                              effect->head.header.owner, effect->head.header.kind);
            }
            steam->spawnCredit -= 1.0f;
        } while
 (steam->spawnCredit > 1.0f);
    }
    return 1;
}

/* Per effect kind: particle texture id (particle_splashBurst splashes, game_1D92F0). */
const u8 gKindSplashTextures[13] = { 13, 0, 9, 6, 4, 5, 11, 12, 12, 14, 15, 8, 10 };
/* Per effect kind flag (colour mode). */
const u8 D_800AB404[13] = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0 };
/* Per effect kind RGB colour. */
const Color24 gKindColors[13] = {
    { 255, 255, 255 }, { 255, 255, 255 }, { 190, 100, 0 },  { 255, 255, 255 }, { 255, 255, 255 },
    { 255, 255, 255 }, { 30, 200, 10 },   { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 },
    { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 },
};

/* Steam effect update for timed effects: shrinks the size once per elapsed frame. */
s32 particle_shrinkSteamEffect(SteamEffect *effect) {
    s32 i;
    SteamData *steam;

    if (effect->head.def.flags & 1) {
        steam = &effect->steam;
        i = D_800BE9E4;
        while (i--) {
            steam->size = steam->size * steam->shrink;
        }
    }
    return 1;
}

/* Spawns a splash of effect kind `kind` at `pos` moving with `vel`. */
Splash *particle_spawnSplash(Vec3f *pos, Vec3f *vel, f32 size, s16 lifetime, u8 arg4, f32 scale, s32 arg6, u8 arg7,
                      f32 burstScale, f32 altBurstScale, u8 argA, u8 kind, CollisionHit *ground, s16 argD, s16 argE,
                      s32 material, u8 arg10, s32 arg11) {
    Splash *splash;
    EffectTemplate spawn;
    TrailMotion motion;
    SplashBurst burst;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    TrailRender style;
    s32 flag80;
    s32 flag20;

    spawn.itemCount = arg6;
    spawn.pos = *pos;
    spawn.life = lifetime + 16;
    spawn.flags = 0x35;
    spawn.kind = 1;
    spawn.unk14 = -1;
    spawn.material = material;
    motion.halfWidth = scale;
    motion.velocity = *vel;
    motion.gravity = size;
    motion.unk14 = -10000.0f;
    flag20 = D_800AB404[kind] ? 0x20 : 0;
    flag80 = D_800AB330[kind] ? 0x80 : 0;
    motion.flags = flag80 | 8 | flag20 | 0x40;
    motion.texture = gKindSplashTextures[kind];
    motion.shade = 0xFF;
    motion.alpha = arg4;
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
        style.setGeometryMode = 0x220005;
        style.clearGeometryMode = 0x50600;
        style.renderMode = 3;
        style.unk10 = 0x46;
        style.unk14 = 0x80;
        style.unk18 = 0x20;
        style.primMode = 0;
        style.envMode = 0xC;
        if (argA != 0) {
            c = 2;
            d = 0xFF;
        } else {
            c = 0;
            d = 0;
        }
        splash = trail_createMotionTrail(&spawn, &motion, 16, 1, 0, 0, a, b, 0, c, d, &style, ground, arg10, arg11);
    }
    if (splash != NULL) {
        memcpy(&splash->state->burst, &burst, sizeof(burst));
    }
    return splash;
}


/* Splash impact handler: bursts into a particle_spawnKindEmitter emitter or a particle_spawnSquash effect at floor height. */
s32 particle_splashBurst(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface) {
    SplashState *state = splash->state;
    SplashNode *nodes = splash->nodes;
    Vec3f point;
    SplashBurst *burst = &state->burst;

    point.x = nodes[splash->head.start].pos.x;
    point.y = floorY + 2.0f;
    point.z = nodes[splash->head.start].pos.z;
    if (func_150ADA20() & 1) {
        particle_spawnKindEmitter(burst->kind, (state->motion.halfWidth * 3.0f) * burst->burstScale, state->motion.alpha, surface,
                      &point, 100, 0, 1, 0, splash->head.header.owner, splash->head.header.kind);
    } else {
        particle_spawnSquash(burst->kind, (state->motion.halfWidth * 1.1f) * burst->burstScale, state->motion.alpha, &point, 1,
                      splash->head.header.owner, splash->head.header.kind);
    }
    state->unk20 = 4;
    return 1;
}

/* Splash impact handler: a particle_spawnKindSplash burst at floor height. */
s32 particle_splashSpray(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface) {
    SplashState *state = splash->state;
    SplashNode *nodes = splash->nodes;
    Vec3f point;

    point.x = nodes[splash->head.start].pos.x;
    point.y = floorY;
    point.z = nodes[splash->head.start].pos.z;
    particle_spawnKindSplash(state->burst.kind, state->motion.halfWidth * 11.0f * state->burst.altBurstScale, state->motion.alpha,
                  surface, &point, splash->head.header.owner, splash->head.header.kind);
    state->unk20 = 4;
    return 1;
}


/* Spawns a squash-and-stretch effect of kind `kind` at `pos`. */
// matching: volatile arg4 (and the non-volatile byte read of it below) make IDO reload it from its stack slot
void particle_spawnSquash(u8 kind, f32 size, u8 arg2, Vec3f *pos, volatile u8 arg4, u8 arg5, s32 arg6) {
    void *effect;
    ParticleDef desc;
    SquashData squash;
    s32 value;
    s32 coinFlip;
    s32 arg7;

    desc.texture = gKindParticleTexturePickers[kind]();
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
    effect = particle_newQuadParticle(
&desc, &gWhiteVtxColors, 0, 20, 0, 14, coinFlip | 2, arg7, value, sizeof(squash), arg5,
                           arg6);
    if (effect != NULL) {
        memcpy(&((SquashEffect *)effect)->squash, &squash, sizeof(squash));
    }
}

/* Squash effect update: advances the phase and sets the X/Y scale multipliers. */
s32 particle_updateSquashEffect(SquashEffect *effect) {
    f32 wave;
    SquashData *squash;

    squash = &effect->squash;
    squash->phase = squash->phase + squash->step * D_800BE9E4;
    wave = math_cosBinAngleMasked(squash->phase - 0x40);
    effect->head.def.scale.x = squash->amplitudeX * wave + 1.0f;
    effect->head.def.scale.y = 1.3f - squash->amplitudeY * wave;
    return 1;
}

/* Vertex function (gQuadParticleVertexFuncs) of a particle drawn as a camera-facing quad, double-buffered per view.
 * Builds this frame's quad vertices for view `view`; returns them, or NULL when no buffer is free. */
Vtx *particle_buildSquashQuad(DefParticleHead *effect, s16 view) {
    Vtx *vtx;
    Vtx *ret;
    f32 halfWidth;
    f32 height;
    f32 dz;
    f32 dx;
    u8 fresh;

    particle_getViewBuffer(effect->vtxBuffers, view, sizeof(effect->vtxTemplate), &vtx, &fresh);
    ret = vtx;
    if (vtx) {
        if (fresh) {
            memcpy(effect->vtxBuffers[view], effect->vtxTemplate, sizeof(effect->vtxTemplate));
            memcpy(effect->vtxBuffers[view] + 4, effect->vtxTemplate, sizeof(effect->vtxTemplate));
        }
    } else {
        return NULL;
    }

    halfWidth = effect->def.width * effect->def.scale.x;
    height = effect->def.height * effect->def.scale.y;
    dz = D_800DD1D8[view] * halfWidth;
    dx = D_800DD1E8[view] * halfWidth;

    vtx[0].v.flag = 0;
    vtx[1].v.flag = 0;
    vtx[2].v.flag = 0;
    vtx[3].v.flag = 0;
    vtx[0].v.ob[0] = vtx[3].v.ob[0] = effect->def.pos.x + dx;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = effect->def.pos.y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = effect->def.pos.z - dz;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = effect->def.pos.x - dx;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = effect->def.pos.y + height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = effect->def.pos.z + dz;

    return ret;
}

/* Colour setter for effect kind 0 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB004(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 80;
    desc->g = 0;
    desc->b = 0;
    desc->r2 = (func_150ADA20() % 46U) + 180;
    desc->g2 = 0;
    desc->b2 = 0;
}

/* Colour setter for effect kinds 1 and 11 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB068(ParticleDesc *desc) {
    desc->r = desc->g = (func_150ADA20() % 56U) + 100;
    desc->b = 0;
    desc->r2 = desc->g2 = (func_150ADA20() % 46U) + 180;
    desc->b2 = 0;
}

/* Colour setter for effect kind 2 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB0CC(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 80;
    desc->g = (func_150ADA20() % 56U) + 80;
    desc->b = 0;
    desc->r2 = (func_150ADA20() % 46U) + 180;
    desc->g2 = (func_150ADA20() % 46U) + 180;
    desc->b2 = 0;
}

/* Colour setter for effect kind 3 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB15C(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 80;
    desc->g = (func_150ADA20() % 56U) + 80;
    desc->b = 0;
    desc->r2 = (func_150ADA20() % 46U) + 180;
    desc->g2 = (func_150ADA20() % 46U) + 180;
    desc->b2 = 0;
}

/* Colour setter for effect kind 4 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB1EC(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 80;
    desc->g = (func_150ADA20() % 56U) + 80;
    desc->b = 0;
    desc->r2 = (func_150ADA20() % 46U) + 180;
    desc->g2 = (func_150ADA20() % 46U) + 180;
    desc->b2 = 0;
}

/* Colour setter for effect kind 5 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB27C(ParticleDesc *desc) {
    desc->r = 255;
    desc->g = 255;
    desc->b = 255;
    desc->r2 = 180;
    desc->g2 = 200;
    desc->b2 = 200;
}

/* Colour setter for effect kind 6 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB2A8(ParticleDesc *desc) {
    desc->r = 0;
    desc->g = 200;
    desc->b = 0;
    desc->r2 = 0;
    desc->g2 = 200;
    desc->b2 = 0;
}

/* Colour setter for effect kind 7 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB2CC(ParticleDesc *desc) {
    desc->r = 0;
    desc->g = (func_150ADA20() % 56U) + 80;
    desc->b = 0;
    desc->r2 = 0;
    desc->g2 = (func_150ADA20() % 46U) + 180;
    desc->b2 = 0;
}

/* Colour setter for effect kind 8 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB330(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 21U) + 95;
    desc->g = (func_150ADA20() % 21U) + 95;
    desc->b = (func_150ADA20() % 11U) + 45;
    desc->r2 = (func_150ADA20() & 0xF) + 58;
    desc->g2 = (func_150ADA20() & 0xF) + 60;
    desc->b2 = (func_150ADA20() % 11U) + 25;
}

/* Colour setter for effect kind 9 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB3D8(ParticleDesc *desc) {
    desc->r = 0;
    desc->g = desc->b = (func_150ADA20() % 56U) + 80;
    desc->r2 = 0;
    desc->g2 = desc->b2 = (func_150ADA20() % 46U) + 180;
}

/* Colour setter for effect kind 10 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB43C(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 80;
    desc->g = 0;
    desc->b = (func_150ADA20() % 56U) + 80;
    desc->r2 = (func_150ADA20() % 46U) + 180;
    desc->g2 = 0;
    desc->b2 = (func_150ADA20() % 46U) + 180;
}

/* Colour setter for effect kind 12 (gEffectKindColorFuncs): fills the colours rgb and rgb2. */
void func_151DB4CC(ParticleDesc *desc) {
    desc->r = (func_150ADA20() % 56U) + 200;
    desc->g = (func_150ADA20() % 56U) + 200;
    desc->b = (func_150ADA20() % 56U) + 200;
    desc->r2 = (func_150ADA20() % 56U) + 200;
    desc->g2 = (func_150ADA20() % 56U) + 200;
    desc->b2 = (func_150ADA20() % 56U) + 200;
}

/*
 * Spawns a sprite particle of effect kind `kind` at `pos` (lifetime -1: never times out). With `spin` set
 * its desc.unk3C oscillates (particle_updateKindSprite); a `drag` other than 1.0f scales its velocity each frame.
 */
SteamParticle *particle_spawnKindSprite(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime, u8 arg8,
                        s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD) {
    ParticleDesc desc;
    SteamParticle *particle;
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
        data.amplitude.x = (func_150ADA68() * 0.05f + 0.05f) * size;
        data.amplitude.y = (func_150ADA68() * 0.03f + 0.03f) * size;
        data.amplitude.z = (func_150ADA68() * 0.05f + 0.05f) * size;
    } else {
        data.stepX = 0;
        data.stepY = 0;
        data.stepZ = 0;
        data.amplitude = gZeroVec3f;
    }
    gEffectKindColorFuncs[(u8)kind](&desc);
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
    desc.pos = *(Vec3f *)pos;
    desc.offset = gZeroVec3f;
    desc.vel = *(Vec3f *)vel;
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
    particle = particle_newSpriteParticleWithFloor(&desc, 1, 0, sizeof(data), argC, argD);
    if (particle != NULL) {
        memcpy(&particle->data.spin, &data, sizeof(data));
    }
    return particle;
}

/* particle_spawnKindSprite particle update: applies drag and oscillates desc.unk3C on three axes. */
s32 particle_updateKindSprite(SteamParticle *particle, s32 arg1) {
    ParticleSpin *spin;
    s32 pad; // matching: unused stack slot
    f32 waveY;
    f32 waveX;


    spin = &particle->data.spin;
    if (spin->flags & SPIN_FLAG_DRAG) {
        particle_applyDragXZ(&particle->head.desc.vel, spin->drag);
    }
    if (spin->flags & SPIN_FLAG_SPIN) {
        spin->angleX = spin->angleX + (spin->stepX * D_800BE9E4);
        spin->angleY = spin->angleY + (spin->stepY * D_800BE9E4);
        spin->angleZ = spin->angleZ + (spin->stepZ * D_800BE9E4);

        waveX = math_cosBinAngleMasked(spin->angleX - 0x40);
        waveY = math_cosBinAngleMasked(spin->angleY - 0x40);
        {
            f32 waveZ;

            waveZ = math_cosBinAngleMasked(spin->angleZ - 0x40);
            particle->head.desc.offset.x = spin->amplitude.x * waveX;
            particle->head.desc.offset.y = spin->amplitude.y * waveY;
            particle->head.desc.offset.z = spin->amplitude.z * waveZ;
        }
    }
    return 1;
}

/* Throws `count` splashes of effect kind `kind` from `pos`. */
void particle_throwSplashes(Vec3f *pos, s32 count, u8 kind, u8 arg3, s32 arg4) {
    SplashSpraySpec splash;

    splash.spray.pos = *pos;

    splash.spray.countBase = count;

    splash.angles.yawRange = 0xFF;
    splash.angles.pitchBase = -64;
    splash.angles.pitchRange = 46;

    splash.spray.countRange = 0;
    splash.angles.yawBase = 0;
    splash.spray.itemCountBase = 3;
    splash.spray.itemCountRange = 2;
    splash.spray.lifeBase = 30;
    splash.spray.lifeRange = 30;
    splash.spray.unk30 = 155;
    splash.spray.scaleBase = 5.5f;

    splash.spray.scaleRange = 2.9399998f;
    splash.spray.sizeBase = 0.61300004f;
    splash.spray.sizeRange = 0.60300004f;
    splash.spray.speedBase = 10.0f;
    splash.spray.speedRange = 10.2f;

    splash.spray.unk32 = 100;
    splash.spray.unk3C = 16;
    splash.spray.unk3E = 15;
    splash.spray.unk40 = 0;
    splash.spray.unk38 = 0.5f;

    splash.spray.kind = kind;

    particle_spawnSplashSprayAtAngles(&splash.angles, &splash.spray, 0, arg3, arg4);

}

/* Spawns a particle_spawnKindEmitter emitter of effect kind *kind just above `pos`, with random size and lifetime. */
void particle_spawnKindEmitterAbove(Vec3f *pos, CollisionHit *hit, u8 *kind, u8 arg3, s32 arg4) {
    Vec3f point;
    u8 emitterKind;
    s32 pad; // matching: unused stack slot
    u32 rand;
    f32 sizeRand;

    point.x = pos->x;
    point.y = pos->y + 5.0f;
    point.z = pos->z;

    emitterKind = *kind;
    sizeRand = func_150ADA68();
    rand = func_150ADA20();

    particle_spawnKindEmitter(emitterKind, (sizeRand * 25.0f) + 10.0f, (rand % 56U) + 200, &hit->tri, &point,
                  (func_150ADA20() % 151U) + 150, 0, 1, 0, arg3, arg4);
}


/* Spawns an emitter of effect kind `kind` (coloured from gKindColors) at `pos`, via particle_spawnSurfaceQuadSpriteNoModel on surface when it
 * is set. */
void particle_spawnKindMark(u8 kind, f32 size, s16 arg2, CollisionTri *surface, void *pos, u8 arg5, s32 arg6) {
    SpriteDef desc;

    desc.texture = 0x38;
    desc.frameSpeed = 0x8000;
    desc.flags = 0x67B02;
    desc.lifetime = 300;
    desc.frame = 0;
    desc.unk10 = arg2;
    desc.unk11 = 0xFF;
    desc.rgba[0] = gKindColors[kind].r;
    desc.rgba[1] = gKindColors[kind].g;
    desc.rgba[2] = gKindColors[kind].b;
    desc.rgba[3] = 0xFF;
    desc.renderModes = 0x440001;
    desc.unk7 = 0;
    desc.unk16 = 0;
    desc.unk17 = 6;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.viewportMask = 0xFF;
    desc.unk1C = 0;
    if (surface != NULL) {
        particle_spawnSurfaceQuadSpriteNoModel(&desc, 0, 0, surface, ((Vec3f *)pos)->x, ((Vec3f *)pos)->y,
                      ((Vec3f *)pos)->z, size, size, func_150ADA20() & 0xFF,
                      (func_150ADA20() & 1) + (func_150ADA20() & 1) * 2, 0, arg5, arg6);
    } else {
        particle_spawnFlatQuadSpriteNoModel(&desc, 0, 0, 0, ((Vec3f *)pos)->x, ((Vec3f *)pos)->y, ((Vec3f *)pos)->z,
                      size, size, 0, 0, 0, arg5, arg6);
    }
}

/* Starts an effect of kind `kind` (coloured from gKindColors) at `pos`. */
void particle_startKindEffect(u8 kind, f32 size, f32 arg2, s16 lifetime, Vec3f *pos, CollisionTri *tri, u8 arg6, u8 arg7,
                   u8 arg8, s32 arg9) {
    ParticleDef desc;

    desc.texture = 0x38;
    desc.renderModes = ((arg7 != 0) ? 2 : 1) + 0x440000; // matching: the s16 store truncates 0x440000 away, but dropping it changes the code
    desc.lifetime = lifetime;
    desc.frame = 0;
    desc.frameSpeed = 0x4000;
    desc.rgba[0] = gKindColors[kind].r;
    desc.width = 1.0f;
    desc.height = 1.0f;
    desc.rgba[1] = gKindColors[kind].g;
    desc.rgba[2] = gKindColors[kind].b;
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

    particle_spawnAxisQuadParticleNoModel(&desc, 0, 11, 17, 0, (func_150ADA20() & 1) + (func_150ADA20() & 1),
                  func_150ADA20() & 0xFF, size, size, NULL, tri, 0, 0, arg8, arg9);
}

/* Spawns a plain sprite particle of effect kind `kind` (id from gKindPlainTextures) at `pos`. */
void particle_spawnKindPlainSprite(Vec3f *pos, f32 size, u8 arg2, s16 lifetime, u8 kind, u8 arg5, s32 arg6) {
    ParticleDesc desc;
    s32 flag800000;
    s32 flag40;
    s32 flag80;

    desc.texture = gKindPlainTextures[kind];
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
    desc.offset = gZeroVec3f;
    desc.vel = gZeroVec3f;
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
    particle_newSpriteParticleWithFloor(&desc, 1, 0, 0, arg5, arg6);
}

/* Throws a splash burst plus a burst of wobbling drops of effect kind `kind` from `pos`. */
void particle_spawnSplashAndDrops(pos, ground, kind, arg3, arg4)
    Vec3f *pos;
    CollisionHit *ground;
    u8 kind;
    u8 arg3;
    s32 arg4;
{
    SplashSpraySpec splash;

    splash.spray.pos = *pos;
    splash.spray.countBase = 10;
    splash.spray.countRange = 10;
    splash.angles.yawRange = 0xFF;
    splash.angles.pitchBase = -64;
    splash.angles.pitchRange = 40;
    splash.spray.scaleBase = 3.0f;
    splash.angles.yawBase = 0;
    splash.spray.itemCountBase = 3;
    splash.spray.itemCountRange = 1;
    splash.spray.lifeBase = 60;
    splash.spray.lifeRange = 40;
    splash.spray.unk30 = 100;
    splash.spray.unk32 = 100;
    splash.spray.unk3C = 16;
    splash.spray.unk3E = 15;
    splash.spray.unk40 = 0;
    splash.spray.scaleRange = 4.0f;
    splash.spray.sizeBase = 0.80700004f;
    splash.spray.sizeRange = 0.51000005f;
    splash.spray.speedBase = 12.0f;
    splash.spray.speedRange = 11.0f;
    splash.spray.unk38 = 1.0f;
    splash.spray.kind = kind;
    particle_spawnSplashSprayAtAngles(&splash.angles, &splash.spray, ground, arg3, arg4);

    {
        DropSpraySpec drops;

        drops.spray.pos = *pos;
        drops.angles.yawBase = 0;
        drops.spray.countBase = 12;
        drops.spray.speedBase = 6.0f;
        drops.spray.countRange = 6;
        drops.angles.yawRange = 0xFF;
        drops.angles.pitchBase = -64;
        drops.angles.pitchRange = 36;
        drops.spray.lifeBase = 35;
        drops.spray.lifeRange = 15;
        drops.spray.unk24 = 155;
        drops.spray.unk25 = 100;
        drops.spray.speedRange = 8.0f;
        drops.spray.unk30 = 1;
        drops.spray.gravityBase = -1.266f;
        drops.spray.gravityRange = 0.46600002f;
        drops.spray.kind = kind;
        drops.spray.sizeBase = 123.0f;
        drops.spray.sizeRange = 134.0f;
        drops.spray.wobbles = 1;
        drops.spray.unk34 = 0.6f;
        drops.spray.bounceChance = 0.0f;
        particle_spawnDropSprayAtAngles(&drops.angles, &drops.spray, ground, arg3, arg4);
    }
}

/* Smaller variant of particle_spawnSplashAndDrops: splash burst plus wobbling drops of effect kind `kind`. */
void particle_spawnSmallSplashAndDrops(pos, ground, kind, arg3, arg4)
    Vec3f *pos;
    CollisionHit *ground;
    u8 kind;
    u8 arg3;
    s32 arg4;
{
    SplashSpraySpec splash;

    splash.spray.pos = *pos;
    splash.spray.countBase = 8;
    splash.spray.countRange = 6;
    splash.angles.yawRange = 0xFF;
    splash.angles.pitchBase = -64;
    splash.spray.scaleBase = 3.0f;
    splash.angles.yawBase = 0;
    splash.angles.pitchRange = 40;
    splash.spray.itemCountBase = 3;
    splash.spray.itemCountRange = 0;
    splash.spray.lifeBase = 60;
    splash.spray.lifeRange = 40;
    splash.spray.unk30 = 100;
    splash.spray.unk32 = 100;
    splash.spray.unk3C = 16;
    splash.spray.unk3E = 15;
    splash.spray.unk40 = 0;
    splash.spray.scaleRange = 2.0f;
    splash.spray.sizeBase = 0.642f;
    splash.spray.sizeRange = 0.314f;
    splash.spray.speedBase = 8.0f;
    splash.spray.speedRange = 5.0f;
    splash.spray.unk38 = 1.0f;
    splash.spray.kind = kind;
    particle_spawnSplashSprayAtAngles(&splash.angles, &splash.spray, ground, arg3, arg4);

    {
        DropSpraySpec drops;

        drops.spray.pos = *pos;
        drops.spray.countBase = 12;
        drops.spray.countRange = 6;
        drops.angles.yawRange = 0xFF;
        drops.angles.yawBase = 0;
        drops.angles.pitchBase = -64;
        drops.angles.pitchRange = 26;
        drops.spray.lifeBase = 35;
        drops.spray.lifeRange = 15;
        drops.spray.unk24 = 155;
        drops.spray.unk25 = 100;
        drops.spray.sizeBase = 59.0f;
        drops.spray.sizeRange = 59.0f;
        drops.spray.unk30 = 1;
        drops.spray.speedBase = 7.0f;
        drops.spray.speedRange = 3.0f;
        drops.spray.gravityBase = -1.23f;
        drops.spray.gravityRange = 0.47500002f;
        drops.spray.kind = kind;
        drops.spray.wobbles = 1;
        drops.spray.unk34 = 0.6f;
        drops.spray.bounceChance = 0.0f;
        particle_spawnDropSprayAtAngles(&drops.angles, &drops.spray, ground, arg3, arg4);
    }
}
