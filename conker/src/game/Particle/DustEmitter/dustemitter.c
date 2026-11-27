#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/* Spawns a dust emitter effect (particle_updateDustEmitter) on `owner`'s bone; it emits dust particles at `offset`
 * in the bone's space at a randomised rate. Returns the effect, or NULL without an owner. */
DustEmitter *particle_spawnDustEmitter(Actor *owner, const Vec3f *offset, u8 bone, f32 rateBase, f32 rateRandom,
                          s16 duration, f32 gravityBase, f32 gravityRandom, s16 lifetimeBase,
                          s16 lifetimeRandom, u8 alphaBase, u8 alphaRandom, f32 sizeBase, f32 sizeRandom,
                          f32 unk60, u8 kind, u8 arg16, s32 arg17) {
    DustEmitter *effect;
    DustEmitterData data;

    if (owner == NULL) {
        return NULL;
    }

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.offset = *offset;
    data.matrixOffset = bone << 6;
    data.rateBase = rateBase;
    data.accum = 0.0f;
    data.rateRandom = rateRandom;
    collision_initHitFromObject(&data.floor, owner);

    data.gravityBase = gravityBase;
    data.gravityRandom = gravityRandom;
    data.lifetimeBase = lifetimeBase;
    data.lifetimeRandom = lifetimeRandom;
    data.alphaBase = alphaBase;
    data.alphaRandom = alphaRandom;
    data.sizeBase = sizeBase;
    data.sizeRandom = sizeRandom;
    data.unk60 = unk60;
    data.kind = kind;

    effect = effect_newTimedEffect((duration == -1) ? 300 : duration, -1, 0x1D, -1,
                                          (duration == -1) ? 0 : 1, 0x1D, sizeof(data), arg16,
                                          arg17);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return effect;
}

/* Dust emitter effect update: frees the effect once its owner is gone, otherwise accumulates the
 * emission rate and spawns one dust particle (particle_spawnDustParticle) per whole unit at the emitter's bone. */
void particle_updateDustEmitter(DustEmitter *effect) {
    Actor *owner;
    DustEmitterData *data;
    Vec3f position;
    f32 floorY;
    Vec3f velocity;
    s32 hit;
    f32 minY;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    data->accum += (data->rateBase + math_randomFloat() * data->rateRandom) * gFrameDelta;
    if (data->accum > 1.0f) {
        if (owner->ob_matrix_ptr != NULL) {
            math_transformOffset(&data->offset, &position, (u8 *)owner->ob_matrix_ptr + data->matrixOffset);
            if (data->unk60 > 0.0f) {
                if (math_isInWorldBounds(&position) == 0) {
                    floorY = -9500.0f;
                } else {
                    minY = -9500.0f;
                    if (collision_probeFloorWithSet3Cached(&position, 0, minY, &data->floor) != 0) {
                        floorY = data->floor.height;
                    } else {
                        floorY = minY;
                    }
                }
            } else {
                floorY = -9500.0f;
            }
            do {
                hit = (math_randomFloat() < data->unk60) ? TRUE : FALSE; // passed to an unused parameter
                velocity.x = 0.0f;
                velocity.y = 0.0f;
                velocity.z = 0.0f;
                particle_spawnDustParticle(&position, &velocity,
                              math_randomFloat() * data->gravityRandom + data->gravityBase,
                              math_random() % (data->lifetimeRandom + 1) + data->lifetimeBase,
                              math_random() % (data->alphaRandom + 1) + data->alphaBase, 0xFF,
                              math_randomFloat() * data->sizeRandom + data->sizeBase, hit, floorY,
                              &data->floor.tri, data->kind, effect->base.header.owner, effect->base.header.kind);
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        } else {
            while (data->accum > 1.0f) {
                data->accum -= 1.0f;
            }
        }
    }
}

/* Spawns a dust particle at `position` that pulses in size and splashes when it falls below floorY
 * (update func 0xF, particle_updateDustParticle). */
DustParticle *particle_spawnDustParticle(Vec3f *position, Vec3f *velocity, f32 gravity, s16 lifetime, u8 alpha,
                            u8 brightness, f32 size, s32 unused, f32 floorY, CollisionTri *floor, u8 kind,
                            u8 arg11, s32 arg12) {
    DustParticle *particle;
    ParticleDesc desc;
    DustData dust;
    s32 flipT;
    s32 flipS;

    desc.renderMode = 0x4401;
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
    desc.alpha = alpha;
    desc.brightness = brightness;
    desc.pos = *position;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.vel = *velocity;
    desc.gravity = gravity;
    desc.scaleStart = 1;
    desc.scaleRate = 1.0f;
    if (math_random() & 1) {
        flipS = PARTICLE_FLIP_S;
    } else {
        flipS = 0;
    }
    if (math_random() & 1) {
        flipT = PARTICLE_FLIP_T;
    } else {
        flipT = 0;
    }
    desc.flags = flipT | (PARTICLE_LIFETIME | PARTICLE_GRAVITY | PARTICLE_MOVE | PARTICLE_FADE |
                          PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC) | flipS | PARTICLE_LIT;
    desc.timer = lifetime + 18;
    desc.fadeStart = 18;
    desc.fadeRate = 14;
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.updateFunc = 0xF;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 3;
    dust.phase[0] = math_random() & 0xFF;
    dust.phase[1] = math_random() & 0xFF;
    desc.texture = gKindParticleTexturePickers[kind]();
    desc.height = size;
    desc.width = size;
    dust.baseSize = size;
    dust.phaseStep[0] = math_random() % 6U + 5;
    dust.phaseStep[1] = math_random() % 6U + 5;
    dust.amplitude[0] = (math_randomFloat() * 0.25f + 0.2f) * dust.baseSize;
    dust.amplitude[1] = (math_randomFloat() * 0.25f + 0.2f) * dust.baseSize;
    dust.floorY = floorY;
    dust.floor = *floor;
    dust.kind = kind;
    particle = particle_newOwnedSpriteParticle(&desc, 1, sizeof(DustData), arg11, arg12);
    if (particle != NULL) {
        memcpy(&particle->dust, &dust, sizeof(DustData));
    }
    return particle;
}

/* Particle update: pulses the dust particle's width and height around its base size. */
u8 particle_pulseDust(DustParticle *particle, Vec3f *prevPos) {
    particle_wobblePair(particle->dust.phase, particle->dust.phaseStep, particle->dust.baseSize,
                  particle->dust.amplitude, &particle->head.desc.width, &particle->head.desc.height);
    return TRUE;
}

/* Particle update: once the dust particle falls below its floor, splash there and stop. */
u8 particle_landDust(DustParticle *particle, Vec3f *prevPos) {
    u8 alive;
    DustData *dust;
    Vec3f position;
    Vec3f velocity;
    f32 sizeRandom;
    f32 speedRandom;
    f32 angleRandom;
    u32 countRandom;
    u32 durationRandom;

    alive = TRUE;
    dust = &particle->dust;
    if (particle->head.desc.pos.y < dust->floorY) {
        alive = FALSE;
        position.x = particle->head.desc.pos.x;
        position.y = dust->floorY + 2.0f;
        position.z = particle->head.desc.pos.z;
        particle_spawnKindEmitter(dust->kind, dust->baseSize * 0.08308f, particle->head.desc.alpha, &dust->floor,
                      &position, 100, 0, 1, 0, particle->head.header.owner, particle->head.header.kind);
        angleRandom = math_randomFloat();
        speedRandom = math_randomFloat();
        sizeRandom = math_randomFloat();
        math_dirFromAngles(angleRandom * 6.2831855f,
                      (speedRandom * 1000.0f + -1207.0f) * 0.001f,
                      (sizeRandom * 3557.0f + 5336.0f) * 0.001f,
                      &velocity);
        sizeRandom = math_randomFloat();
        durationRandom = math_random();
        countRandom = math_random();
        particle_spawnSplash(&position, &velocity, sizeRandom * 0.20600002f + 0.20600002f,
                      durationRandom % 21U + 35, particle->head.desc.alpha,
                      dust->baseSize * 0.02495f, countRandom % 7U + 5, 0,
                      1.0f, 1.0f, 1, dust->kind, 0, 0x10, 0xF, 0,
                      particle->head.header.owner, particle->head.header.kind);
    }
    return alive;
}

/* Dust particle update func (gSpriteUpdateFuncs): pulse, then land on the floor. */
u8 particle_updateDustParticle(DustParticle *particle, Vec3f *prevPos) {
    u8 alive;

    alive = particle_pulseDust(particle, prevPos);
    if (alive) {
        alive = particle_landDust(particle, prevPos);
    }
    return alive;
}

/* This file's .rodata front-end data: the dust emitter presets, indexed by kind (see particle_startObjDustEmitter). */
static const Vec3f gDustPresetOffsets[] = {
    { 0.0f, -6.0f, 9.0f }, { 0.0f, 12.0f, 30.0f }, { 0.0f, -12.0f, 24.0f }, { 0.0f, 0.0f, -91.0f },
};
static const u8 gDustPresetBones[] = { 15, 5, 8, 2 };
static const f32 gDustPresetRateBases[] = { 0.110549994f, 0.39999998f, 0.110549994f, 0.110549994f };
static const f32 gDustPresetRateRandoms[] = { 0.35678f, 0.48718f, 0.35678f, 0.35678f };
static const f32 gDustPresetGravityBases[] = { -0.259f, -0.65500003f, -0.259f, -0.259f };
static const f32 gDustPresetGravityRandoms[] = { 0.10300001f, 0.30200002f, 0.10300001f, 0.10300001f };
static const s16 gDustPresetLifetimeBases[] = { 40, 25, 40, 50 };
static const s16 gDustPresetLifetimeRandoms[] = { 0, 15, 0, 30 };
static const f32 gDustPresetSizeBases[] = { 126.1f, 350.6f, 126.1f, 124.0f };
static const f32 gDustPresetSizeRandoms[] = { 146.0f, 206.7f, 146.0f, 176.2f };
static const f32 D_800A80D4[] = { 0.011000001f, 0.006f, 0.011000001f, 0.011000001f };
static const u8 gDustPresetKinds[] = { 4, 4, 4, 2 };

/* Spawns the dust emitter preset that suits obj's id (particle_spawnDustEmitter) for `duration` frames, -1 for
 * the default. Returns the effect, or NULL when obj is NULL or has no preset. */
DustEmitter *particle_startObjDustEmitter(Actor *obj, s16 duration, u8 arg2, s32 arg3) {
    s32 kind;
    DustEmitter *effect;

    if (obj == NULL) {
        return NULL;
    }
    kind = -1;
    effect = NULL;
    switch (obj->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        kind = 0;
        break;
    case 0x10:
        kind = 1;
        break;
    case 0x58:
        kind = 2;
        break;
    case 0x79:
        kind = 3;
        break;
    }
    if (kind != -1) {
        effect = particle_spawnDustEmitter(obj, &gDustPresetOffsets[kind], gDustPresetBones[kind], gDustPresetRateBases[kind], gDustPresetRateRandoms[kind],
                               duration, gDustPresetGravityBases[kind], gDustPresetGravityRandoms[kind], gDustPresetLifetimeBases[kind],
                               gDustPresetLifetimeRandoms[kind], 100, 155, gDustPresetSizeBases[kind], gDustPresetSizeRandoms[kind],
                               D_800A80D4[kind], gDustPresetKinds[kind], arg2, arg3);
    }
    return effect;
}

/* Effect callback for the dust emitter: hands effect_handleOwnerActorEvent the owner reference. */
void particle_onDustEmitterEvent(DustEmitter *effect, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}
