#include <ultra64.h>
// functions.h declares func_15048A40 as returning void; it returns f32 (declared below)
#define func_15048A40 func_15048A40_void_unused
#include "functions.h"
#undef func_15048A40
#include "variables.h"


/*
 * Spawn template of a billboard particle: func_15167D84 copies these 0x38 bytes to Particle.anim
 * onwards (layout as Particle in game_1944C0.c, minus its 0x10-byte object link).
 */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ s32 floorAndMinSize; /* this file: low half floor height, high half minimum size */
    /* 0x08 */ s32 kind;            /* this file: 0-3, see func_151B9964 */
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 size;            /* this file: base size of a pulsing (kind 1) particle */
    /* 0x1C */ u8 subX;
    /* 0x1D */ u8 subZ;
    /* 0x1E */ u8 subY;
    /* 0x1F */ s8 callback;         /* index into D_8008C9C8, -1 = none */
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 phase;            /* this file: pulse phase, 1/256ths of a turn */
    /* 0x2C */ u8 primR;
    /* 0x2D */ u8 primG;
    /* 0x2E */ u8 primB;
    /* 0x2F */ u8 primA;
    /* 0x30 */ u8 envR;
    /* 0x31 */ u8 envG;
    /* 0x32 */ u8 envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ u16 flags;
    /* 0x36 */ u8 pad36[2];
} ParticleTemplate; /* size 0x38 */

extern void *D_8008CA4C[];
void *func_15167D84(void *, s32, s32, s32, s32, s32);

/* A random offset in [-range / 2, range / 2). */
#define RAND_SPREAD(range) ((s32)((func_150ADA20() % (u32)(range)) - ((range) >> 1)))

/*
 * Spawns one particle around (x, y, z): placed `radius` out at `angle` + 180 degrees (+- angleSpread / 2),
 * moving outwards at `speed` (+- speedRange / 2) and upwards at `velY` (+- velYRange / 2), falling to
 * the floor at the spawn height.
 */
void func_151B9690(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 unused, f32 angle, s16 angleSpread,
                   s16 velY, s16 velYRange, s16 speed, s16 speedRange, s16 radius, s16 minSize, s16 size,
                   u8 owner, s32 list) {
    ParticleTemplate template;
    s32 floorY;
    s16 *pad1; // matching: pad1-3 are unused stack slots of the target's frame
    f32 outSpeed;
    f32 radiusF;
    f32 pad2;
    f32 sine;
    f32 cosine;
    f32 pad3;
    f32 dist;
    s32 upSpeed;
    u8 r;
    u8 g;
    u8 b;

    floorY = y;
    angle += 180.0f;
    angle += RAND_SPREAD(angleSpread);
    sine = sinf(angle * 0.01745329238f);
    cosine = cosf(angle * 0.01745329238f);
    // matching: going through `dist` hoists and spills the (f32)speed conversion like the target
    dist = speed;
    if (speedRange == 0) {
        speedRange = 1;
    }
    dist += RAND_SPREAD(speedRange);
    outSpeed = dist;
    radiusF = radius;
    x += radiusF * sine;
    z += radiusF * cosine;
    y++;
    // matching: computing the random part first gives the target's addu operand order
    upSpeed = RAND_SPREAD(velYRange);
    upSpeed = velY + upSpeed;
    // matching: listing cases 0, 1 and 3 keeps the target's case dispatch
    switch (kind) {
    case 2:
        r = 0x68; g = 0x38; b = 0x10;
        break;
    case 0:
    case 1:
    case 3:
    default:
        r = g = b = 0xFF;
        break;
    }
    template.anim = D_8008CA4C[animIndex];
    template.floorAndMinSize = (minSize << 16) | (floorY & 0xFFFF);
    template.kind = kind;
    template.frame = 0;
    template.frameStep = 0x100;
    template.x = x;
    template.y = y;
    template.z = z;
    template.velX = outSpeed * sine;
    template.velZ = outSpeed * cosine;
    template.size = size;
    template.callback = 0; /* func_151B9964 */
    template.velY = upSpeed;
    template.accelY = -160;
    template.width = size;
    template.height = size;
    template.life = 400;
    template.fadeStep = 0;
    template.phase = func_150ADA20();
    template.primA = 0xFF;
    template.flags = 0;
    template.primR = r;
    template.primG = g;
    template.primB = b;
    func_15167D84(&template, 0, 0, -1, owner, list);
}

/* A particle spawned by func_151B9690 (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];        /* object link */
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ void *anim;
    /* 0x14 */ s32 floorAndMinSize; /* low half floor height, high half minimum size */
    /* 0x18 */ s32 kind;
    /* 0x1C */ s16 frame;
    /* 0x1E */ s16 frameStep;
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ s16 size;
    /* 0x2C */ u8 subX;
    /* 0x2D */ u8 subZ;
    /* 0x2E */ u8 subY;
    /* 0x2F */ s8 callback;
    /* 0x30 */ s16 velY;
    /* 0x32 */ s16 accelY;
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ s8 fadeStep;
    /* 0x3B */ u8 phase;
    /* 0x3C */ u8 primR;
    /* 0x3D */ u8 primG;
    /* 0x3E */ u8 primB;
    /* 0x3F */ u8 primA;
} FallingParticle;

extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);
extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);

/*
 * Particle callback 0 (D_8008C9C8[0]) for func_151B9690's particles. Kind 1 pulses in size while the base
 * size decays; other kinds shrink towards their minimum size. On reaching the floor the particle dies
 * (kind 0 may leave a splash and play a sound); while falling just above it, it fades out.
 */
void func_151B9964(FallingParticle *particle) {
    s32 width;
    s32 unused; // matching: unused stack slot of the target's frame
    s32 minSize;
    s32 v; // matching: one local for the clamped size and y gives the target's registers
    f32 size;
    s16 floorY;
    s32 heightAbove;
    s32 kind;
    u8 phase;

    kind = particle->kind;
    if (kind == 1) {
        phase = particle->phase;
        // matching: `* 6` is emitted as a different shift/add sequence
        phase += D_800BE9E4 * 4 + D_800BE9E4 * 2;
        particle->phase = phase;
        particle->size *= 0.99f;
        size = particle->size;
        particle->width = (func_15048A40(phase) + 3.0f) * size * (1.0f / 3.0f);
        particle->height = (func_150489B0(phase) + 3.0f) * size * (1.0f / 3.0f);
    } else {
        width = particle->width;
        minSize = particle->floorAndMinSize >> 16;
        if (minSize != width) {
            v = width - D_800BE9E4 * 4;
            if (v < minSize) {
                v = minSize;
            }
            particle->height = v;
            particle->width = v;
        }
    }
    v = particle->y;
    floorY = particle->floorAndMinSize & 0xFFFF;
    if (floorY >= v) {
        particle->life = 0;
        if (kind == 0) {
            // matching: the target masks the u8 random value again
            if ((func_150ADA20() & 0xFF) < 64) {
                func_15171D4C(particle->x, floorY, particle->z, 10, 0, 19, 0.0f, 0, 50, 15, 0x100, 0,
                              particle->owner, particle->list);
            }
            if ((func_150ADA20() & 0xFF) < 15) {
                func_150ADA20();
                func_10010F88(0x70, 5000, 0, 0, 0, particle->x, floorY, particle->z, 500, 2000);
            }
        }
    } else {
        heightAbove = v - floorY;
        if (particle->velY < 0 && heightAbove < 64) {
            particle->primA = heightAbove * 4;
        }
    }
}

/* Type 7 object (0x2C bytes): emits func_151B9690 particles every frame until its timer runs out. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];   /* object link */
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ f32 *followPos; /* when set, emit from this position instead of x/y/z */
    /* 0x14 */ u8 animIndex;
    /* 0x15 */ u8 kind;
    /* 0x16 */ s16 x;
    /* 0x18 */ s16 y;
    /* 0x1A */ s16 z;
    /* 0x1C */ s16 velY;
    /* 0x1E */ s16 velYRange;
    /* 0x20 */ s16 speed;
    /* 0x22 */ s16 speedRange;
    /* 0x24 */ s16 minSize;
    /* 0x26 */ s16 size;
    /* 0x28 */ s16 count;      /* particles emitted per frame besides the 4 fast ones */
    /* 0x2A */ s16 timer;
} ParticleEmitter;

void *func_15167A68(s32, s32, s32, s32, s32, s32);
void func_151B9690(s32, s32, s16, s16, s16, s16, f32, s16, s16, s16, s16, s16, s16, s16, s16, u8, s32);

/* Creates a particle emitter (see func_151B9CB0); the parameters are its fields. */
void func_151B9BF0(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 velY, s16 velYRange, s16 speed,
                   s16 speedRange, s16 minSize, s16 size, s16 count, s16 timer, f32 *followPos, u8 owner,
                   s32 list) {
    ParticleEmitter *emitter;

    emitter = func_15167A68(7, list, sizeof(ParticleEmitter), 0, owner, 1);
    if (emitter != NULL) {
        emitter->animIndex = animIndex;
        emitter->kind = kind;
        emitter->x = x;
        emitter->y = y;
        emitter->z = z;
        emitter->velY = velY;
        emitter->velYRange = velYRange;
        emitter->speed = speed;
        emitter->speedRange = speedRange;
        emitter->minSize = minSize;
        emitter->size = size;
        emitter->count = count;
        emitter->timer = timer;
        emitter->followPos = followPos;
    }
}

/*
 * Type 7 update (D_8008B614): each frame emits `count` particles and 4 faster, higher ones in a full
 * circle around the emitter, then counts the timer down and destroys the emitter when it runs out.
 */
void func_151B9CB0(ParticleEmitter *emitter) {
    f32 *followPos;
    ParticleEmitter *self; // matching: the target reloads count through a second copy of `emitter`
    s16 x;
    s16 y;
    s16 z;
    s32 i;
    s32 timer;

    followPos = emitter->followPos;
    self = emitter;
    if (followPos == NULL) {
        x = emitter->x;
        y = emitter->y;
        z = emitter->z;
    } else {
        x = followPos[0];
        y = followPos[1];
        z = followPos[2];
    }

    for (i = 0; i < self->count; i++) {
        func_151B9690(emitter->animIndex, emitter->kind, x, y, z, -80, 0.0f, 360, emitter->velY,
                      emitter->velYRange, emitter->speed, emitter->speedRange, 15, emitter->minSize,
                      emitter->size, emitter->owner, emitter->list);
    }
    for (i = 0; i < 4; i++) {
        func_151B9690(emitter->animIndex, emitter->kind, x, y, z, -80, 0.0f, 360,
                      (emitter->velY * 3) / 2, emitter->velYRange * 2, emitter->speed,
                      emitter->speedRange, 15, emitter->minSize, emitter->size, emitter->owner, emitter->list);
    }

    // matching: `timer = emitter->timer - D_800BE9E4;` schedules the end differently
    timer = emitter->timer;
    timer -= D_800BE9E4;
    if (timer < 0) {
        func_1516972C((struct102 *)emitter);
    } else {
        emitter->timer = timer;
    }
}
