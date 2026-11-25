#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * Object-attached particle emitters: effects 0x1A, 0x1B and 0x1C (struct260s with their own data
 * at 0x28), the per-frame updates that emit their particles, and their spawn callbacks.
 */

/* Data of effect 0x1A (func_15109410 / func_151094FC). */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId; /* object->unique_id when attached; a mismatch means the slot was reused */
    /* 0x08 */ f32 spawnAccum;
    /* 0x0C */ f32 spawnRate;
    /* 0x10 */ s8 tickFunc; /* index into D_80088C60, -1: none */
    /* 0x11 */ s8 emitFunc; /* index into D_80088C64, -1: none */
} ObjectEmitterData; /* size 0x14 */

/* Data of effect 0x1B (func_15109ED4 / func_15109FB8). */
typedef struct {
    /* 0x00 */ void *unk0; /* passed to func_1511490C to build the emitter's matrix */
    /* 0x04 */ f32 spawnAccum;
    /* 0x08 */ f32 spawnRate;
    /* 0x0C */ s8 tickFunc; /* index into D_80088C60, -1: none */
    /* 0x0D */ s8 emitFunc; /* index into D_80088C64, -1: none */
} MatrixEmitterData; /* size 0x10 */

/* Data of effect 0x1C (func_1510A344 / func_1510A40C). */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId;
    /* 0x08 */ f32 spawnAccum;
    /* 0x0C */ f32 spawnRate;
    /* 0x10 */ f32 spawnRateRandom; /* added to spawnRate scaled by a random [0, 1) */
} PairEmitterData; /* size 0x14 */

/* Head shared by the data of the effects that follow an object. */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId;
} TrackedObject;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* -1 ends the effect */
    /* 0x10 */ u8 pad10[0x13 - 0x10];
    /* 0x13 */ u8 unk13; /* compared with 0x1A, probably the effect's type */
    /* 0x14 */ u8 pad14[0x28 - 0x14];
    /* 0x28 */ union {
        TrackedObject tracked;
        ObjectEmitterData object;
        MatrixEmitterData matrix;
        PairEmitterData pair;
    } data;
} Effect1368C0;

/* Live particle returned by func_15130374; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 unkA8;
} Particle1368C0;

/* Spawn descriptor for func_15152B38 (same layout as EmitterSpawn in game_124920.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} EmitterSpawn; /* size 0x74 */

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
typedef struct ParticleSpawn78 {
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
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s16 *unk5C;
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
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

/* Payload of message 0x2D: two objects trade places. */
typedef struct {
    /* 0x00 */ Actor *from;
    /* 0x04 */ Actor *to;
    /* 0x08 */ u8 fromId;
    /* 0x09 */ u8 toId;
} ObjectSwapMessage;

/* Emission segments of effect 0x1A: particles start on segment i (D_800A24B0[i] to
 * D_800A24B0[i + 1]), picked with weight D_800A2504[i]; D_800A2504[6] is the weight total. */
extern const vertex D_800A24B0[7];
extern const f32 D_800A2504[7];
extern const vertex D_800A2520[6];
/* The same for effect 0x1B. */
extern const vertex D_800A2568[7];
extern const vertex D_800A25BC[6];
/* Effect 0x1C's two emission points and the points their particles head for. */
extern vertex D_800A2604;
extern vertex D_800A2610;
extern vertex D_800A261C;
extern vertex D_800A2628;

/* Per-frame effect callbacks, and particle emitters called with
 * (effect, point, segment start, segment end, segment tip, direction). */
extern void (*D_80088C60[])(void *);
extern void (*D_80088C64[])(void *, vertex *, vertex *, vertex *, vertex *, vertex *);

extern void func_15143134(const vertex *, vertex *, void *);
extern s32 func_15145128(vertex *, vertex *, f32 *, f32 *);
extern Particle1368C0 *func_15130374(ParticleDesc *, u8, s32, u8, s32);
extern void func_15132A4C(ParticleSpawn78 *, s32, s32, s32, u8, s32);
extern void func_15152B38(EmitterSpawn *, u8, s32);
extern void func_1511490C(f32 mtx[4][4], void *arg1);
extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/*
 * Creates effect 0x1A on `object`, lasting `duration` frames (negative: 300), emitting
 * particles at `spawnRate` through D_80088C64[emitFunc]. `extraSize` more data bytes are reserved.
 */
struct260 *func_15109410(Actor *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
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
    return (struct260 *)effect;
}

/*
 * Effect 0x1A update: ends the effect when its object is gone, otherwise emits one particle per
 * whole unit of spawnAccum from a random point on the object's emission segments.
 */
void func_151094FC(Effect1368C0 *effect) {
    ObjectEmitterData *data;
    Actor *object;
    vertex point;
    vertex start;
    vertex end;
    vertex tip;
    vertex dir;
    vertex local;
    const vertex *segment;
    void *matrix;
    s32 i;
    f32 r;
    f32 t;

    data = &effect->data.object;
    object = data->object;
    if (object->interaction_state == 0 || object->unique_id != data->objectId) {
        effect->state = -1;
        return;
    }
    data->spawnAccum += data->spawnRate * D_800A2504[6] * D_800BE9A4;
    if (data->emitFunc != -1 && object->unk1D4 != NULL) {
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
            matrix = &object->unk1D4->unk240;
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
void func_15109848(Effect1368C0 *effect, vertex *point, vertex *start, vertex *end, vertex *tip, vertex *dir) {
    ParticleDesc spawn;
    f32 speed;
    EmitterSpawn emitter;
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
    func_15130374(&spawn, 1, 0, effect->owner, effect->kind);

    emitter.unk0 = 1;
    emitter.unk4 = 4;
    emitter.pos = *point;
    emitter.unk14 = 1.049999952f;
    emitter.unk18 = 1.889999986f;
    emitter.unk1C = -0.8150000572f;
    emitter.unk20 = 0.5770000219f;
    emitter.unk24 = 2.0f;
    emitter.unk28 = 6.0f;
    emitter.unk2C = 0;
    emitter.unk2E = 0xFF;
    emitter.unk30 = -0x40;
    emitter.unk32 = 0x2E;
    emitter.unk34 = 4;
    emitter.unk38 = 3;
    emitter.unk3C = 20;
    emitter.unk3E = 15;
    emitter.unk40 = 1;
    emitter.unk42 = 0xC;
    emitter.unk43 = 2;
    emitter.unk44 = 3;
    emitter.unk45 = 0xFF;
    emitter.unk46 = 0xFF;
    emitter.unk47 = 0xFF;
    emitter.unk48 = 0xFF;
    emitter.unk49 = 0;
    emitter.unk4A = 0;
    emitter.unk4B = 0;
    emitter.unk4C = 0;
    emitter.unk4D = 0xFF;
    emitter.unk4E = 0xFF;
    emitter.unk4F = 0xFF;
    emitter.unk50 = 0xFF;
    emitter.unk51 = 0;
    emitter.unk52 = 0;
    emitter.unk53 = 0;
    emitter.unk54 = 0;
    emitter.unk55 = 0xFF;
    emitter.unk56 = 0;
    emitter.unk57 = 3;
    emitter.unk58 = 0x24;
    emitter.unk5C = 0x200005;
    emitter.unk60 = 0x60600;
    emitter.unk64 = 10;
    emitter.unk66 = 25;
    emitter.unk68 = 1;
    emitter.unk6A = 0;
    emitter.unk6C = 1.0f;
    emitter.unk70 = -1;
    emitter.unk71 = 0;
    emitter.unk72 = -1;
    emitter.unk73 = -1;
    func_15152B38(&emitter, effect->owner, effect->kind);
}

/* Particle emitter: a randomly sized and spun piece of debris thrown along `dir` from `point`. */
void func_15109C20(Effect1368C0 *effect, vertex *point, vertex *start, vertex *end, vertex *tip, vertex *dir) {
    s32 padA4; // matching: unused stack slots in the original frame
    Actor *object;
    f32 speed;
    s32 pad98;
    ParticleSpawn78 debris;

    object = effect->data.tracked.object;
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
    if (effect->unk13 == 0x1A) { /* emitted by effect 0x1A (func_151094FC) */
        debris.unk6A = 1;
    } else {
        debris.unk6A = 2;
    }
    debris.owner = object;
    debris.ownerId = object->unique_id;
    debris.fadeStart = 10;
    debris.fadeRate = 25;

    func_15132A4C(&debris, 3, 0xFF, 0, effect->owner, effect->kind);
}

/*
 * Creates effect 0x1B for `arg0`, lasting `duration` frames (negative: 300), emitting particles at
 * `spawnRate` through D_80088C64[emitFunc]. `extraSize` more data bytes are reserved.
 */
struct260 *func_15109ED4(void *arg0, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
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
    return (struct260 *)effect;
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
            f32 mtx[4][4];
            vertex point;
            vertex start;
            vertex end;
            vertex tip;
            vertex dir;
            const vertex *segment;
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
struct260 *func_1510A344(Actor *object, s32 duration, u8 owner, s32 kind) {
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
    return (struct260 *)effect;
}

/*
 * Effect 0x1C update: ends the effect when its object is gone, otherwise emits particles from one
 * of the object's two emission points (picked at random), flying towards that point's target.
 */
void func_1510A40C(Effect1368C0 *effect) {
    PairEmitterData *data;
    Actor *object;
    vertex points[2];
    vertex dirs[2];
    void *matrix;
    ParticleDesc spawn;
    f32 extra;
    f32 speed;
    u8 side;
    Particle1368C0 *particle;

    data = &effect->data.pair;
    object = data->object;
    if (object->interaction_state == 0 || object->unique_id != data->objectId) {
        effect->state = -1;
        return;
    }
    data->spawnAccum += (data->spawnRate + func_150ADA68() * data->spawnRateRandom) * D_800BE9A4;
    if (data->spawnAccum > 1.0f && object->unk1D4 != NULL) {
        matrix = &object->unk1D4->unk240;
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
            particle = func_15130374(&spawn, 1, sizeof(extra), effect->owner, effect->kind);
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
void func_1510A870(Effect1368C0 *effect, ObjectSwapMessage *msg, u8 msgType) {
    TrackedObject *tracked = &effect->data.tracked;

    if (msgType == 0x2D) {
        if (tracked->object == msg->from) {
            tracked->object = msg->to;
            tracked->objectId = msg->toId;
        } else if (tracked->object == msg->to) {
            tracked->object = msg->from;
            tracked->objectId = msg->fromId;
        }
    }
}

/* Message handler: the same as func_1510A870. */
void func_1510A8CC(Effect1368C0 *effect, ObjectSwapMessage *msg, u8 msgType) {
    TrackedObject *tracked = &effect->data.tracked;

    if (msgType == 0x2D) {
        if (tracked->object == msg->from) {
            tracked->object = msg->to;
            tracked->objectId = msg->toId;
        } else if (tracked->object == msg->to) {
            tracked->object = msg->from;
            tracked->objectId = msg->fromId;
        }
    }
}
