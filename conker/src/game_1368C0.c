#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Object-attached particle emitters: effects 0x1A, 0x1B and 0x1C (TimedEffects with their own data
 * at 0x28), the per-frame updates that emit their particles, and their spawn callbacks.
 */

/* Live particle returned by func_15130374; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 unkA8;
} Particle1368C0;

/* Emission segments of effect 0x1A: particles start on segment i (D_800A24B0[i] to
 * D_800A24B0[i + 1]), picked with weight D_800A2504[i]; D_800A2504[6] is the weight total. */
extern const Vec3f D_800A24B0[7];
extern const f32 D_800A2504[7];
extern const Vec3f D_800A2520[6];
/* The same for effect 0x1B. */
extern const Vec3f D_800A2568[7];
extern const Vec3f D_800A25BC[6];
/* Effect 0x1C's two emission points and the points their particles head for. */
extern Vec3f D_800A2604;
extern Vec3f D_800A2610;
extern Vec3f D_800A261C;
extern Vec3f D_800A2628;

/* Per-frame effect callbacks, and particle emitters called with
 * (effect, point, segment start, segment end, segment tip, direction). */
extern void (*D_80088C60[])(void *);
extern void (*D_80088C64[])(void *, Vec3f *, Vec3f *, Vec3f *, Vec3f *, Vec3f *);

/*
 * Creates effect 0x1A on `object`, lasting `duration` frames (negative: 300), emitting
 * particles at `spawnRate` through D_80088C64[emitFunc]. `extraSize` more data bytes are reserved.
 */
TimedEffect *func_15109410(Actor *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
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

    effect = (Effect1368C0 *)func_15149130(lifetime, -1, 0x1A, -1, timed, 0x1A,
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
void func_151094FC(Effect1368C0 *effect) {
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
    data->spawnAccum += data->spawnRate * D_800A2504[6] * D_800BE9A4;
    if (data->emitFunc != -1 && object->ob_matrix_ptr != NULL) {
        while (data->spawnAccum > 1.0f) {
            r = func_150ADA68() * D_800A2504[6];
            for (i = 0; D_800A2504[i] < r; i++) {
                r -= D_800A2504[i];
            }
            t = func_150ADA68();
            segment = &D_800A24B0[i];
            local.x = segment[0].x + (segment[1].x - segment[0].x) * t;
            local.y = segment[0].y + (segment[1].y - segment[0].y) * t;
            local.z = segment[0].z + (segment[1].z - segment[0].z) * t;
            matrix = &object->ob_matrix_ptr[9];
            func_15143134(&local, &point, matrix);
            func_15143134(&segment[0], &start, matrix);
            func_15143134(&segment[1], &end, matrix);
            func_15143134(&D_800A2520[i], &tip, matrix);
            t = func_150ADA68();
            dir.x = (tip.x - end.x) * t + end.x - start.x;
            dir.y = (tip.y - end.y) * t + end.y - start.y;
            dir.z = (tip.z - end.z) * t + end.z - start.z;
            func_15145128(&dir, &dir, NULL, NULL);
            D_80088C64[data->emitFunc](effect, &point, &start, &end, &tip, &dir);
            data->spawnAccum -= 1.0f;
        }
    } else {
        while (data->spawnAccum > 1.0f) {
            data->spawnAccum -= 1.0f;
        }
    }
    if (data->tickFunc != -1) {
        D_80088C60[data->tickFunc](effect);
    }
}

/* Particle emitter: a particle flying along `dir` from `point`, plus a func_15152B38 emitter there. */
void func_15109848(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir) {
    ParticleDesc spawn;
    f32 speed;
    TrailBurst emitter;
    s32 flip80;
    s32 flip40;

    speed = (func_150ADA68() * 851.0f + 525.0f) * 0.001000000047f;
    spawn.texture = 0x2B;
    spawn.renderMode = 0x4401;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x20000;
    spawn.timer = (func_150ADA20() & 0xF) + 10;
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
    spawn.width = spawn.height = func_150ADA68() * 25.0f + 35.0f;
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
    flip40 = func_150ADA20() & 1 ? 0x40 : 0;
    flip80 = func_150ADA20() & 1 ? 0x80 : 0;
    spawn.flags = flip80 | 5 | flip40 | 0x8000 | 0x4000 | 0x200;
    spawn.primColorMode = 6;
    spawn.envColorMode = 6;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 4;
    func_15130374(&spawn, 1, 0, effect->base.header.owner, effect->base.header.kind);

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
    func_15152B38(&emitter, effect->base.header.owner, effect->base.header.kind);
}

/* Particle emitter: a randomly sized and spun piece of debris thrown along `dir` from `point`. */
void func_15109C20(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir) {
    s32 padA4; // matching: unused stack slots in the original frame
    Actor *object;
    f32 speed;
    s32 pad98;
    ParticleSpawn78 debris;

    object = effect->data.tracked.actor;
    speed = (func_150ADA68() * 4611.0f + 1581.0f) * 0.001000000047f;
    debris.radius = 1.0f;
    debris.elasticity = 1.0f;
    debris.sizeXZ = debris.sizeY = (func_150ADA68() * 3522.0f + 4599.0f) * 9.999999975e-07f;
    debris.rot.x = func_150ADA68() * 360.0f;
    debris.rot.y = func_150ADA68() * 360.0f;
    debris.rot.z = func_150ADA68() * 360.0f;
    debris.scale.x = 1.0f;
    debris.scale.y = 1.0f;
    debris.scale.z = 1.0f;
    debris.pos = *point;
    debris.vel.x = dir->x * speed;
    debris.vel.y = dir->y * speed;
    debris.vel.z = dir->z * speed;
    debris.rotVel.x = (func_150ADA68() * 24546.0f + -12273.0f) * 0.001000000047f;
    debris.rotVel.y = 0.0f;
    debris.rotVel.z = (func_150ADA68() * 24546.0f + -12273.0f) * 0.001000000047f;
    debris.gravity = (func_150ADA68() * 124.0f + -231.0f) * 0.001000000047f;
    debris.flags = 0x29E8;
    debris.lifetime = func_150ADA20() % 15U + 20;

    if (func_150ADA20() & 1) {
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
    if (effect->base.callbackSet == 0x1A) { /* emitted by effect 0x1A (func_151094FC) */
        debris.unk6A = 1;
    } else {
        debris.unk6A = 2;
    }
    debris.owner = object;
    debris.ownerId = object->unique_id;
    debris.fadeStart = 10;
    debris.fadeRate = 25;

    func_15132A4C(&debris, 3, 0xFF, 0, effect->base.header.owner, effect->base.header.kind);
}

/*
 * Creates effect 0x1B for `arg0`, lasting `duration` frames (negative: 300), emitting particles at
 * `spawnRate` through D_80088C64[emitFunc]. `extraSize` more data bytes are reserved.
 */
TimedEffect *func_15109ED4(void *arg0, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
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

    effect = (Effect1368C0 *)func_15149130(lifetime, -1, 0x1B, -1, timed, 0x1B,
                                           (extraSize + sizeof(data)), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return &effect->base;
}

/*
 * Effect 0x1B update: emits one particle per whole unit of spawnAccum from a random point on the
 * emission segments, transformed by the matrix func_1511490C builds from data.unk0.
 */
void func_15109FB8(Effect1368C0 *effect) {
    MatrixEmitterData *data;

    data = &effect->data.matrix;
    data->spawnAccum += data->spawnRate * D_800A2504[6] * D_800BE9A4;
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

            func_1511490C(mtx, data->unk0);
            r = func_150ADA68() * D_800A2504[6];
            for (i = 0; D_800A2504[i] < r; i++) {
                r -= D_800A2504[i];
            }
            t = func_150ADA68();
            segment = &D_800A2568[i];
            x = segment[0].x + (segment[1].x - segment[0].x) * t;
            y = segment[0].y + (segment[1].y - segment[0].y) * t;
            z = segment[0].z + (segment[1].z - segment[0].z) * t;
            func_150A7960(mtx, x, y, z, &point.x, &point.y, &point.z);
            func_150A7960(mtx, segment[0].x, segment[0].y, segment[0].z, &start.x, &start.y, &start.z);
            func_150A7960(mtx, segment[1].x, segment[1].y, segment[1].z, &end.x, &end.y, &end.z);
            func_150A7960(mtx, D_800A25BC[i].x, D_800A25BC[i].y, D_800A25BC[i].z, &tip.x, &tip.y, &tip.z);
            t = func_150ADA68();
            dir.x = (tip.x - end.x) * t + end.x - start.x;
            dir.y = (tip.y - end.y) * t + end.y - start.y;
            dir.z = (tip.z - end.z) * t + end.z - start.z;
            func_15145128(&dir, &dir, NULL, NULL);
            D_80088C64[data->emitFunc](effect, &point, &start, &end, &tip, &dir);
            data->spawnAccum -= 1.0f;
        }
    } else {
        while (data->spawnAccum > 1.0f) {
            data->spawnAccum -= 1.0f;
        }
    }
    if (data->tickFunc != -1) {
        D_80088C60[data->tickFunc](effect);
    }
}

/* Creates effect 0x1C on `object`, lasting `duration` frames. */
TimedEffect *func_1510A344(Actor *object, s16 duration, u8 owner, s32 kind) {
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

    effect = (Effect1368C0 *)func_15149130(duration, -1, 0x1C, -1, TRUE, 0x1C, sizeof(data), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return &effect->base;
}

/*
 * Effect 0x1C update: ends the effect when its object is gone, otherwise emits particles from one
 * of the object's two emission points (picked at random), flying towards that point's target.
 */
void func_1510A40C(Effect1368C0 *effect) {
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
    data->spawnAccum += (data->spawnRate + func_150ADA68() * data->spawnRateRandom) * D_800BE9A4;
    if (data->spawnAccum > 1.0f && object->ob_matrix_ptr != NULL) {
        matrix = &object->ob_matrix_ptr[9];
        func_15143134(&D_800A2604, &points[0], matrix);
        func_15143134(&D_800A2610, &points[1], matrix);
        func_15143134(&D_800A261C, &dirs[0], matrix);
        func_15143134(&D_800A2628, &dirs[1], matrix);
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
            if (func_150ADA20() & 1) {
                spawn.flags |= 0x40;
            } else {
                spawn.flags &= ~0x40;
            }
            if (func_150ADA20() & 1) {
                spawn.flags |= 0x80;
            } else {
                spawn.flags &= ~0x80;
            }
            side = func_150ADA20() & 1;
            speed = (func_150ADA68() * 203.0f + 96.0f) * 0.001f;
            spawn.timer = (func_150ADA20() & 0xF) + 20;
            spawn.width = spawn.height = func_150ADA68() * 60.0f + 60.0f;
            spawn.pos = points[side];
            spawn.vel.x = dirs[side].x * speed;
            spawn.vel.y = dirs[side].y * speed;
            spawn.vel.z = dirs[side].z * speed;
            particle = func_15130374(&spawn, 1, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
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
void func_1510A870(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType) {
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

/* Message handler: the same as func_1510A870. */
void func_1510A8CC(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType) {
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
