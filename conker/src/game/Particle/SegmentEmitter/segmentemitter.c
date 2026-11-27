#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/particle.h"

/*
 * Object-attached particle emitters: effects 0x1A, 0x1B and 0x1C (TimedEffects with their own data
 * at 0x28), the per-frame updates that emit their particles, and their spawn callbacks.
 */

/* Live particle returned by particle_newOwnedSpriteParticle; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 unkA8;
} Particle1368C0;

/* This file's .rodata. Emission segments of effect 0x1A: particles start on segment i (gSegmentEmitterPoints[i] to
 * gSegmentEmitterPoints[i + 1]), picked with weight gSegmentEmitterWeights[i]; gSegmentEmitterWeights[6] is the weight total. */
static const Vec3f gSegmentEmitterPoints[7] = {
    { 10.0f, -2.0f, 73.0f },   { 10.0f, 7.0f, 211.0f },   { 10.0f, 0.0f, 231.0f }, { 10.0f, -17.0f, 238.0f },
    { 10.0f, -36.0f, 231.0f }, { 10.0f, -41.0f, 209.0f }, { 10.0f, -34.0f, 78.0f },
};
static const f32 gSegmentEmitterWeights[7] = {
    138.2931213f, 21.18962097f, 18.38477707f, 20.24845695f, 22.56102753f, 131.1868896f, 351.8638916f,
};
static const Vec3f gSegmentEmitterTips[6] = {
    { 10.0f, 22.0f, 211.0f },  { 10.0f, 7.0f, 236.0f },   { 10.0f, -19.0f, 248.0f },
    { 10.0f, -40.0f, 243.0f }, { 10.0f, -50.0f, 213.0f }, { 10.0f, -47.0f, 79.0f },
};
/* The same for effect 0x1B. */
static const Vec3f gMatrixSegmentEmitterPoints[7] = {
    { 0.0f, 6.764999866f, -3.0f },        { 0.0f, 10.82400036f, 79.80000305f }, { 0.0f, 7.667000294f, 91.80000305f },
    { 0.0f, 0.0f, 96.0f },                { 0.0f, -8.569000244f, 91.80000305f }, { 0.0f, -10.82400036f, 78.6000061f },
    { 0.0f, -7.667000294f, 0.0f },
};
static const Vec3f gMatrixSegmentEmitterTips[6] = {
    { 0.0f, 17.5890007f, 79.80000305f },  { 0.0f, 10.82400036f, 94.80000305f }, { 0.0f, -0.90200001f, 102.0000076f },
    { 0.0f, -10.37300014f, 99.00000763f }, { 0.0f, -14.88300037f, 81.0f },      { 0.0f, -13.52999973f, 0.6000000238f },
};
/* Effect 0x1C's two emission points and the points their particles head for. */
static const Vec3f gPointPairEmitPoint0 = { -11.0f, -24.0f, 43.0f };
static const Vec3f gPointPairEmitPoint1 = { 31.0f, -24.0f, 43.0f };
static const Vec3f gPointPairTarget0 = { -51.0f, -24.0f, 43.0f };
static const Vec3f gPointPairTarget1 = { 81.0f, -24.0f, 43.0f };

/* This file's .data: the per-frame effect callbacks (none), and the particle emitters called with
 * (effect, point, segment start, segment end, segment tip, direction). */
static void (*gSegmentEmitterTickFuncs[])(Effect1368C0 *effect) = { NULL };
static void (*gSegmentEmitFuncs[])(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir) = {
    particle_emitAlongDir,
    particle_emitDebris,
    NULL,
};
/* not referenced: two 16-byte records */
static u32 D_80088C70[2][4] = {
    { 0xE7FFFF00, 0x866B1C00, 0x7D000000, 0x32000000 },
    { 0xE7FFFF00, 0x866B1C00, 0x7D000000, 0x32000000 },
};

/*
 * Creates effect 0x1A on `object`, lasting `duration` frames (negative: 300), emitting
 * particles at `spawnRate` through gSegmentEmitFuncs[emitFunc]. `extraSize` more data bytes are reserved.
 */
TimedEffect *particle_startSegmentEmitter(Actor *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
                         u8 owner, s32 kind) {
    Effect1368C0 *effect;
    ObjectEmitterData data;
    s16 lifetime;
    u8 timed;

    if (object == NULL) {
        return NULL;
    }

    timed = FALSE;
    if (duration < 0) {
        lifetime = 300;
    } else {
        timed = TRUE;
        lifetime = duration;
    }

    data.spawnAccum = 0.0f;
    data.spawnRate = spawnRate;
    data.tickFunc = tickFunc;
    data.emitFunc = emitFunc;
    data.object = object;
    data.objectId = object->unique_id;

    effect = effect_newTimedEffect(lifetime, -1, 0x1A, -1, timed, 0x1A,
                                           (extraSize + sizeof(data)), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return &effect->base;
}

/*
 * Effect 0x1A update: ends the effect when its object is gone, otherwise emits one particle per
 * whole unit of spawnAccum from a random point on the object's emission segments.
 */
void particle_updateSegmentEmitter(Effect1368C0 *effect) {
    ObjectEmitterData *data;
    Actor *object;
    Vec3f point;
    Vec3f start;
    Vec3f end;
    Vec3f tip;
    Vec3f dir;
    Vec3f local;
    const Vec3f *segment;
    void *matrix;
    s32 i;
    f32 r;
    f32 t;

    data = &effect->data.object;
    object = data->object;
    if (object->type == 0 || object->unique_id != data->objectId) {
        effect->base.header.state = -1;
        return;
    }
    data->spawnAccum += data->spawnRate * gSegmentEmitterWeights[6] * gFrameDelta;
    if (data->emitFunc != -1 && object->ob_matrix_ptr != NULL) {
        while (data->spawnAccum > 1.0f) {
            r = math_randomFloat() * gSegmentEmitterWeights[6];
            for (i = 0; gSegmentEmitterWeights[i] < r; i++) {
                r -= gSegmentEmitterWeights[i];
            }
            t = math_randomFloat();
            segment = &gSegmentEmitterPoints[i];
            local.x = segment[0].x + (segment[1].x - segment[0].x) * t;
            local.y = segment[0].y + (segment[1].y - segment[0].y) * t;
            local.z = segment[0].z + (segment[1].z - segment[0].z) * t;
            matrix = &object->ob_matrix_ptr[9];
            math_transformOffset(&local, &point, matrix);
            math_transformOffset(&segment[0], &start, matrix);
            math_transformOffset(&segment[1], &end, matrix);
            math_transformOffset(&gSegmentEmitterTips[i], &tip, matrix);
            t = math_randomFloat();
            dir.x = (tip.x - end.x) * t + end.x - start.x;
            dir.y = (tip.y - end.y) * t + end.y - start.y;
            dir.z = (tip.z - end.z) * t + end.z - start.z;
            math_vec3Normalize(&dir, &dir, NULL, NULL);
            gSegmentEmitFuncs[data->emitFunc](effect, &point, &start, &end, &tip, &dir);
            data->spawnAccum -= 1.0f;
        }
    } else {
        while (data->spawnAccum > 1.0f) {
            data->spawnAccum -= 1.0f;
        }
    }
    if (data->tickFunc != -1) {
        gSegmentEmitterTickFuncs[data->tickFunc](effect);
    }
}

/* Particle emitter: a particle flying along `dir` from `point`, plus a particle_spawnTrailBurst emitter there. */
void particle_emitAlongDir(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir) {
    ParticleDesc spawn;
    f32 speed;
    TrailBurst emitter;
    s32 flip80;
    s32 flip40;

    speed = (math_randomFloat() * 851.0f + 525.0f) * 0.001000000047f;
    spawn.texture = 0x2B;
    spawn.renderMode = 0x4401;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x20000;
    spawn.timer = (math_random() & 0xF) + 10;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.alpha = 0xFF;
    spawn.brightness = 0xFF;
    spawn.width = spawn.height = math_randomFloat() * 25.0f + 35.0f;
    spawn.pos = *point;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.vel.x = dir->x * speed;
    spawn.vel.y = dir->y * speed;
    spawn.vel.z = dir->z * speed;
    spawn.fadeStart = 5;
    spawn.fadeRate = 0x33;
    spawn.scaleStart = 1;
    spawn.gravity = 0.0f;
    spawn.scaleRate = 1.0f;
    /* random flips */
    flip40 = math_random() & 1 ? 0x40 : 0;
    flip80 = math_random() & 1 ? 0x80 : 0;
    spawn.flags = flip80 | 5 | flip40 | 0x8000 | 0x4000 | 0x200;
    spawn.primColorMode = 6;
    spawn.envColorMode = 6;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 4;
    particle_newOwnedSpriteParticle(&spawn, 1, 0, effect->base.header.owner, effect->base.header.kind);

    emitter.countBase = 1;
    emitter.countRange = 4;
    emitter.pos = *point;
    emitter.sizeBase = 1.049999952f;
    emitter.sizeRange = 1.889999986f;
    emitter.gravityBase = -0.8150000572f;
    emitter.gravityRange = 0.5770000219f;
    emitter.speedBase = 2.0f;
    emitter.speedRange = 6.0f;
    emitter.angles.yawBase = 0;
    emitter.angles.yawRange = 0xFF;
    emitter.angles.pitchBase = -0x40;
    emitter.angles.pitchRange = 0x2E;
    emitter.capacityBase = 4;
    emitter.capacityRange = 3;
    emitter.lifeBase = 20;
    emitter.lifeRange = 15;
    emitter.headerFlags = 1;
    emitter.flags = 0xC;
    emitter.primMode = 2;
    emitter.envMode = 3;
    emitter.colorBase.red = 0xFF;
    emitter.colorBase.green = 0xFF;
    emitter.colorBase.blue = 0xFF;
    emitter.colorBase.alpha = 0xFF;
    emitter.colorRange.red = 0;
    emitter.colorRange.green = 0;
    emitter.colorRange.blue = 0;
    emitter.colorRange.alpha = 0;
    emitter.color2Base.red = 0xFF;
    emitter.color2Base.green = 0xFF;
    emitter.color2Base.blue = 0xFF;
    emitter.color2Base.alpha = 0xFF;
    emitter.color2Range.red = 0;
    emitter.color2Range.green = 0;
    emitter.color2Range.blue = 0;
    emitter.color2Range.alpha = 0;
    emitter.colorLevelBase = 0xFF;
    emitter.colorLevelRange = 0;
    emitter.renderMode = 3;
    emitter.unk21 = 0x24;
    emitter.setGeometryMode = 0x200005;
    emitter.clearGeometryMode = 0x60600;
    emitter.fadeStart = 10;
    emitter.fadeRate = 25;
    emitter.shrinkStart = 1;
    emitter.shrinkPaused = 0;
    emitter.shrinkFactor = 1.0f;
    emitter.unk38 = -1;
    emitter.unk39 = 0;
    emitter.unk72 = -1;
    emitter.unk73 = -1;
    particle_spawnTrailBurst(&emitter, effect->base.header.owner, effect->base.header.kind);
}

/* Particle emitter: a randomly sized and spun piece of debris thrown along `dir` from `point`. */
void particle_emitDebris(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir) {
    s32 padA4; // matching: unused stack slots in the original frame
    Actor *object;
    f32 speed;
    ParticleSpawn debris;

    object = effect->data.tracked.actor;
    speed = (math_randomFloat() * 4611.0f + 1581.0f) * 0.001000000047f;
    debris.radius = 1.0f;
    debris.elasticity = 1.0f;
    debris.sizeXZ = debris.sizeY = (math_randomFloat() * 3522.0f + 4599.0f) * 9.999999975e-07f;
    debris.rot.x = math_randomFloat() * 360.0f;
    debris.rot.y = math_randomFloat() * 360.0f;
    debris.rot.z = math_randomFloat() * 360.0f;
    debris.scale.x = 1.0f;
    debris.scale.y = 1.0f;
    debris.scale.z = 1.0f;
    debris.pos = *point;
    debris.vel.x = dir->x * speed;
    debris.vel.y = dir->y * speed;
    debris.vel.z = dir->z * speed;
    debris.rotVel.x = (math_randomFloat() * 24546.0f + -12273.0f) * 0.001000000047f;
    debris.rotVel.y = 0.0f;
    debris.rotVel.z = (math_randomFloat() * 24546.0f + -12273.0f) * 0.001000000047f;
    debris.gravity = (math_randomFloat() * 124.0f + -231.0f) * 0.001000000047f;
    debris.flags = 0x29E8;
    debris.lifetime = math_random() % 15U + 20;

    if (math_random() & 1) {
        debris.model = 0x23;
    } else {
        debris.model = 0x24;
    }

    debris.variant = 0;
    debris.unk5C = 0;
    debris.alpha = 0xFF;
    debris.updateFn = 8;
    debris.update2Fn = 0;
    debris.floorHitFn = 0;
    debris.floorType3Fn = 0;
    debris.wallHitFn = 0;
    debris.destroyFn = 0;
    debris.squashFn = 0;
    debris.matrixFn = 2;
    if (effect->base.callbackSet == 0x1A) { /* emitted by effect 0x1A (particle_updateSegmentEmitter) */
        debris.unk6A = 1;
    } else {
        debris.unk6A = 2;
    }
    debris.owner = object;
    debris.ownerId = object->unique_id;
    debris.fadeStart = 10;
    debris.fadeRate = 25;

    particle_newModelParticle(&debris, 3, 0xFF, 0, effect->base.header.owner, effect->base.header.kind);
}

/*
 * Creates effect 0x1B for `arg0`, lasting `duration` frames (negative: 300), emitting particles at
 * `spawnRate` through gSegmentEmitFuncs[emitFunc]. `extraSize` more data bytes are reserved.
 */
TimedEffect *particle_startMatrixSegmentEmitter(void *arg0, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
                         u8 owner, s32 kind) {
    Effect1368C0 *effect;
    MatrixEmitterData data;
    s16 lifetime;
    u8 timed;

    if (arg0 == NULL) {
        return NULL;
    }

    timed = FALSE;
    if (duration < 0) {
        lifetime = 300;
    } else {
        timed = TRUE;
        lifetime = duration;
    }

    data.spawnAccum = 0.0f;
    data.spawnRate = spawnRate;
    data.tickFunc = tickFunc;
    data.emitFunc = emitFunc;
    data.unk0 = arg0;

    effect = effect_newTimedEffect(lifetime, -1, 0x1B, -1, timed, 0x1B,
                                           (extraSize + sizeof(data)), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return &effect->base;
}

/*
 * Effect 0x1B update: emits one particle per whole unit of spawnAccum from a random point on the
 * emission segments, transformed by the matrix gfx_getObjectModelMtx builds from data.unk0.
 */
void particle_updateMatrixSegmentEmitter(Effect1368C0 *effect) {
    MatrixEmitterData *data;

    data = &effect->data.matrix;
    data->spawnAccum += data->spawnRate * gSegmentEmitterWeights[6] * gFrameDelta;
    if (data->emitFunc != -1) {
        while (data->spawnAccum > 1.0f) {
            MtxF mtx;
            Vec3f point;
            Vec3f start;
            Vec3f end;
            Vec3f tip;
            Vec3f dir;
            const Vec3f *segment;
            s32 i;
            f32 r;
            f32 t;
            f32 x;
            f32 y;
            f32 z;

            gfx_getObjectModelMtx(mtx, data->unk0);
            r = math_randomFloat() * gSegmentEmitterWeights[6];
            for (i = 0; gSegmentEmitterWeights[i] < r; i++) {
                r -= gSegmentEmitterWeights[i];
            }
            t = math_randomFloat();
            segment = &gMatrixSegmentEmitterPoints[i];
            x = segment[0].x + (segment[1].x - segment[0].x) * t;
            y = segment[0].y + (segment[1].y - segment[0].y) * t;
            z = segment[0].z + (segment[1].z - segment[0].z) * t;
            math_transformPoint(mtx, x, y, z, &point.x, &point.y, &point.z);
            math_transformPoint(mtx, segment[0].x, segment[0].y, segment[0].z, &start.x, &start.y, &start.z);
            math_transformPoint(mtx, segment[1].x, segment[1].y, segment[1].z, &end.x, &end.y, &end.z);
            math_transformPoint(mtx, gMatrixSegmentEmitterTips[i].x, gMatrixSegmentEmitterTips[i].y, gMatrixSegmentEmitterTips[i].z, &tip.x, &tip.y, &tip.z);
            t = math_randomFloat();
            dir.x = (tip.x - end.x) * t + end.x - start.x;
            dir.y = (tip.y - end.y) * t + end.y - start.y;
            dir.z = (tip.z - end.z) * t + end.z - start.z;
            math_vec3Normalize(&dir, &dir, NULL, NULL);
            gSegmentEmitFuncs[data->emitFunc](effect, &point, &start, &end, &tip, &dir);
            data->spawnAccum -= 1.0f;
        }
    } else {
        while (data->spawnAccum > 1.0f) {
            data->spawnAccum -= 1.0f;
        }
    }
    if (data->tickFunc != -1) {
        gSegmentEmitterTickFuncs[data->tickFunc](effect);
    }
}

/* Creates effect 0x1C on `object`, lasting `duration` frames. */
TimedEffect *particle_startPointPairEmitter(Actor *object, s16 duration, u8 owner, s32 kind) {
    Effect1368C0 *effect;
    PairEmitterData data;

    if (object == NULL) {
        return NULL;
    }

    data.object = object;
    data.objectId = object->unique_id;
    data.spawnAccum = 0.0f;
    data.spawnRate = 0.2040000111f;
    data.spawnRateRandom = 0.8220000267f;

    effect = effect_newTimedEffect(duration, -1, 0x1C, -1, TRUE, 0x1C, sizeof(data), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return &effect->base;
}

/*
 * Effect 0x1C update: ends the effect when its object is gone, otherwise emits particles from one
 * of the object's two emission points (picked at random), flying towards that point's target.
 */
void particle_updatePointPairEmitter(Effect1368C0 *effect) {
    PairEmitterData *data;
    Actor *object;
    Vec3f points[2];
    Vec3f dirs[2];
    void *matrix;
    ParticleDesc spawn;
    f32 extra;
    f32 speed;
    u8 side;
    Particle1368C0 *particle;

    data = &effect->data.pair;
    object = data->object;
    if (object->type == 0 || object->unique_id != data->objectId) {
        effect->base.header.state = -1;
        return;
    }
    data->spawnAccum += (data->spawnRate + math_randomFloat() * data->spawnRateRandom) * gFrameDelta;
    if (data->spawnAccum > 1.0f && object->ob_matrix_ptr != NULL) {
        matrix = &object->ob_matrix_ptr[9];
        math_transformOffset(&gPointPairEmitPoint0, &points[0], matrix);
        math_transformOffset(&gPointPairEmitPoint1, &points[1], matrix);
        math_transformOffset(&gPointPairTarget0, &dirs[0], matrix);
        math_transformOffset(&gPointPairTarget1, &dirs[1], matrix);
        dirs[0].x -= points[0].x;
        dirs[0].y -= points[0].y;
        dirs[0].z -= points[0].z;
        dirs[1].x -= points[1].x;
        dirs[1].y -= points[1].y;
        dirs[1].z -= points[1].z;
        extra = 0.9108009934f;
        spawn.texture = 0x29;
        spawn.renderMode = 0xE03;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.fadeStart = 25;
        spawn.fadeRate = 10;
        spawn.flags = 0xCE05;
        spawn.primColorMode = 3;
        spawn.envColorMode = 3;
        spawn.updateFunc = 0x10;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 25;
        spawn.r = 0xDD;
        spawn.g = 0xD3;
        spawn.b = 0xCD;
        spawn.a = 0xFF;
        spawn.r2 = 0x57;
        spawn.g2 = 0x55;
        spawn.b2 = 0x5A;
        spawn.alpha = 0xFF;
        spawn.brightness = 0xFF;
        spawn.scaleRate = 1.043205976f;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.gravity = 0.0f;
        do {
            if (math_random() & 1) {
                spawn.flags |= 0x40;
            } else {
                spawn.flags &= ~0x40;
            }
            if (math_random() & 1) {
                spawn.flags |= 0x80;
            } else {
                spawn.flags &= ~0x80;
            }
            side = math_random() & 1;
            speed = (math_randomFloat() * 203.0f + 96.0f) * 0.001f;
            spawn.timer = (math_random() & 0xF) + 20;
            spawn.width = spawn.height = math_randomFloat() * 60.0f + 60.0f;
            spawn.pos = points[side];
            spawn.vel.x = dirs[side].x * speed;
            spawn.vel.y = dirs[side].y * speed;
            spawn.vel.z = dirs[side].z * speed;
            particle = particle_newOwnedSpriteParticle(&spawn, 1, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->unkA8, &extra, sizeof(extra));
            }
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    } else {
        while (data->spawnAccum > 1.0f) {
            data->spawnAccum -= 1.0f;
        }
    }
}

/* Message handler: on message 0x2D (two objects swapped), moves the effect to the other object. */
void particle_onSegmentEmitterSwap(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType) {
    ActorRef *tracked = &effect->data.tracked;

    if (msgType == 0x2D) {
        if (tracked->actor == msg->actorA) {
            tracked->actor = msg->actorB;
            tracked->uniqueId = msg->idB;
        } else if (tracked->actor == msg->actorB) {
            tracked->actor = msg->actorA;
            tracked->uniqueId = msg->idA;
        }
    }
}

/* Message handler: the same as particle_onSegmentEmitterSwap. */
void particle_onPointPairEmitterSwap(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType) {
    ActorRef *tracked = &effect->data.tracked;

    if (msgType == 0x2D) {
        if (tracked->actor == msg->actorA) {
            tracked->actor = msg->actorB;
            tracked->uniqueId = msg->idB;
        } else if (tracked->actor == msg->actorB) {
            tracked->actor = msg->actorA;
            tracked->uniqueId = msg->idA;
        }
    }
}
