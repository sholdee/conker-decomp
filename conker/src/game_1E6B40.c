#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* A random offset in [-range / 2, range / 2). */
#define RAND_SPREAD(range) ((s32)((func_150ADA20_u8() % (u32)(range)) - ((range) >> 1)))

/*
 * Spawns one particle around (x, y, z): placed `radius` out at `angle` + 180 degrees (+- angleSpread / 2),
 * moving outwards at `speed` (+- speedRange / 2) and upwards at `velY` (+- velYRange / 2), falling to
 * the floor at the spawn height.
 */
void func_151B9690(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 unused, f32 angle, s16 angleSpread,
                   s16 velY, s16 velYRange, s16 speed, s16 speedRange, s16 radius, s16 minSize, s16 size,
                   u8 owner, s32 list) {
    BillboardTemplate template;
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
    template.anim = gEffectAnimTextures[animIndex];
    template.unk4 = (minSize << 16) | (floorY & 0xFFFF);
    template.unk8 = kind;
    template.frame = 0;
    template.frameStep = 0x100;
    template.x = x;
    template.y = y;
    template.z = z;
    template.velX = outSpeed * sine;
    template.velZ = outSpeed * cosine;
    template.unk1A = size;
    template.callback = 0; /* func_151B9964 */
    template.velY = upSpeed;
    template.accelY = -160;
    template.width = size;
    template.height = size;
    template.life = 400;
    template.fadeStep = 0;
    template.unk2B = func_150ADA20_u8();
    template.primA = 0xFF;
    template.flags = 0;
    template.primR = r;
    template.primG = g;
    template.primB = b;
    effect_spawnBillboard(&template, 0, 0, -1, owner, list);
}

/*
 * Particle callback 0 (gParticleCallbacks[0]) for func_151B9690's particles. Kind 1 pulses in size while the base
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
            if ((func_150ADA20_u8() & 0xFF) < 64) {
                func_15171D4C(particle->x, floorY, particle->z, 10, 0, 19, 0.0f, 0, 50, 15, 0x100, 0,
                              particle->owner, particle->list);
            }
            if ((func_150ADA20_u8() & 0xFF) < 15) {
                func_150ADA20_u8();
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

/* Creates a particle emitter (see func_151B9CB0); the parameters are its fields. */
void func_151B9BF0(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 velY, s16 velYRange, s16 speed,
                   s16 speedRange, s16 minSize, s16 size, s16 count, s16 timer, f32 *followPos, u8 owner,
                   s32 list) {
    FallingEmitter *emitter;

    emitter = effect_newObject(7, list, sizeof(FallingEmitter), 0, owner, 1);
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
void func_151B9CB0(FallingEmitter *emitter) {
    f32 *followPos;
    FallingEmitter *self; // matching: the target reloads count through a second copy of `emitter`
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
                      emitter->size, emitter->header.owner, emitter->header.kind);
    }
    for (i = 0; i < 4; i++) {
        func_151B9690(emitter->animIndex, emitter->kind, x, y, z, -80, 0.0f, 360,
                      (emitter->velY * 3) / 2, emitter->velYRange * 2, emitter->speed,
                      emitter->speedRange, 15, emitter->minSize, emitter->size, emitter->header.owner, emitter->header.kind);
    }

    // matching: `timer = emitter->timer - D_800BE9E4;` schedules the end differently
    timer = emitter->timer;
    timer -= D_800BE9E4;
    if (timer < 0) {
        effect_destroyObject(&emitter->header);
    } else {
        emitter->timer = timer;
    }
}
