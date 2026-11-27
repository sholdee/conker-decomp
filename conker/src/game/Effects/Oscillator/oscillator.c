#include <ultra64.h>
#include "effects.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/particle.h"
#include "libc/math.h"

/* A sine oscillator: value = base + amplitude * sin(phase); phase advances by phaseStep per frame. */
typedef struct {
    /* 0x00 */ u32 phase;
    /* 0x04 */ s32 phaseStep;
    /* 0x08 */ f32 base;
    /* 0x0C */ f32 amplitude;
} FrameOscillator;

/* An object created by blood_spawnStrip with a FrameOscillator in its user data. */
struct OscillatorObject {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ FrameOscillator oscillator;
};

/* The owner whose +0x74 scales the spawned particle's velocity. */
struct struct_EA11C_arg5 {
    /* 0x00 */ u8  pad0[0x74];
    /* 0x74 */ f32 unk74;
};

/* Creates a blood_spawnStrip object attached to `owner`, with a slowly pulsing oscillator. */
OscillatorObject *effect_attachBone8SegmentStrip(Actor *owner, s16 lifetime, u8 fxOwner, s32 fxList) {
    BloodStripDesc desc;
    OscillatorObject *obj;
    FrameOscillator oscillator;

    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.bone = 8;
    desc.offsetA.x = -9.0f;
    desc.offsetA.y = -14.0f;
    desc.offsetA.z = 27.0f;
    desc.offsetB.x = -21.0f;
    desc.offsetB.y = -14.0f;
    desc.offsetB.z = 41.0f;
    desc.flags = 2;
    desc.duration = 60;
    desc.pause = 60;
    desc.lifetime = lifetime;
    desc.sampleCallback = 2;
    desc.emitCallback = 2;
    desc.rateCallback = 2;
    desc.rate = 0.5f;
    desc.mode = 3;
    desc.updateCallback = -1;

    oscillator.phase = 0;
    oscillator.phaseStep = 0x11111;
    oscillator.base = 0.2f;
    oscillator.amplitude = 0.060000002f;

    obj = blood_spawnStrip(&desc, 0x10);
    if (obj != NULL) {
        memcpy(&obj->oscillator, &oscillator, sizeof(oscillator));
    }
    return obj;
}

/* Restarts the object's oscillator from phase 0. */
s32 effect_resetOscillator(OscillatorObject *obj) {
    obj->oscillator.phase = 0;
    return 1;
}

/* Spawns a particle on the segment `from`->`to`, `t` of the way along, drifting that way. */
void effect_spawnSegmentParticle(Vec3f *from, Vec3f *to, Vec3f *velocityA, Vec3f *velocityB, f32 t, struct_EA11C_arg5 *strip) {
    ParticleDesc desc;
    u8 *particle;
    RiseMotion extra;

    desc.texture = 0x2F;
    desc.renderMode = 0xC01;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.b2 = 0;
    desc.g2 = 0;
    desc.r2 = 0;
    desc.a = 0;
    desc.b = 0;
    desc.g = 0;
    desc.r = 0;
    desc.frameRate = 0;
    desc.frame = 0;
    desc.brightness = 0xFF;
    desc.scaleStart = 0;
    desc.scaleRate = 0.0f;
    desc.flags = 0x1207;
    desc.primColorMode = 5;
    desc.envColorMode = 5;
    desc.updateFunc = 4;
    desc.deathFunc = -1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.fadeStart = 20;
    desc.fadeRate = 12;
    extra.widthPhase = 0;
    extra.heightPhase = 0;
    extra.swayPhaseX = 0;
    extra.swayPhaseZ = 0;
    extra.maxRiseSpeed = 3.5f;
    extra.swayPerSpeed = 0.2857143f;

    desc.vel.x = (to->x - from->x) * strip->unk74;
    desc.vel.y = (to->y - from->y) * strip->unk74;
    desc.vel.z = (to->z - from->z) * strip->unk74;
    desc.pos = *from;
    desc.pos.x += desc.vel.x * t;
    desc.pos.y += desc.vel.y * t;
    desc.pos.z += desc.vel.z * t;

    if (math_random() & 1) {
        desc.flags |= 0x40;
    }
    if (math_random() & 1) {
        desc.flags |= 0x80;
    }

    desc.timer = math_random() % 51U + 50;
    desc.alpha = math_random() % 101U + 100;
    extra.baseSize = desc.width = desc.height = math_randomFloat() * 39.0f + 31.0f;
    extra.widthPhaseStep = math_random() % 5U + 4;
    extra.heightPhaseStep = math_random() % 5U + 4;
    extra.widthWobble = (math_randomFloat() * 0.25f + 0.15f) * extra.baseSize;
    extra.heightWobble = (math_randomFloat() * 0.25f + 0.15f) * extra.baseSize;
    extra.swayPhaseStepX = math_random() % 5U + 4;
    extra.swayPhaseStepZ = math_random() % 5U + 4;
    extra.swayX = (math_randomFloat() * 0.1f + 0.08f) * extra.baseSize;
    extra.swayZ = (math_randomFloat() * 0.1f + 0.08f) * extra.baseSize;
    desc.gravity = (math_randomFloat() * 70.0f + 73.0f) * 0.001f;

    particle = particle_newSpriteParticle(&desc, 2, 0x28);
    if (particle != NULL) {
        memcpy(particle + 0xA8, &extra, sizeof(extra));
    }
}

/* Returns the oscillator's current value and advances its phase by this frame's step. */
f32 effect_stepOscillator(OscillatorObject *obj) {
    FrameOscillator *oscillator = &obj->oscillator;
    f32 value;

    value = math_cosBinAngleMasked((oscillator->phase >> 16) - 0x40) * oscillator->amplitude + oscillator->base;
    oscillator->phase += oscillator->phaseStep * gFrameTicks;
    return value;
}

/* Attaches an emitter to bone 1 of `owner` for `lifetime` frames (-1: 300 frames). */
void *effect_attachDriftEmitter(Actor *owner, s16 lifetime, u8 fxOwner, s32 fxList) {
    BloodDripDesc desc;

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 25.0f;
    desc.rate = 0.26497f;
    if (lifetime == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = lifetime;
    }
    desc.flags = ((lifetime == -1) ? 0 : 4) | 0xA;
    desc.emitter = 7;
    desc.callback = -1;
    desc.handlerSet = 6;
    return blood_spawnDrip(&desc, 0, 0xFF, 0);
}

/* Spawns a particle at (x, y, z) drifting against the velocity (vx, vy, vz). */
void effect_spawnDriftParticle(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz) {
    ParticleDesc desc;
    u8 *particle;
    RiseMotion extra;

    desc.pos.z = z;
    extra.ceiling = 1000.0f;
    extra.maxRiseSpeed = 4.0f;
    extra.swayPerSpeed = 0.25f;
    desc.texture = 0x2F;
    desc.renderMode = 0xC01;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.b2 = 0;
    desc.g2 = 0;
    desc.r2 = 0;
    desc.a = 0;
    desc.b = 0;
    desc.g = 0;
    desc.r = 0;
    desc.frameRate = 0;
    desc.frame = 0;
    desc.brightness = 0xFF;
    desc.scaleStart = 0;
    desc.scaleRate = 0.0f;
    desc.flags = 0x3207;
    desc.primColorMode = 5;
    desc.envColorMode = 5;
    desc.updateFunc = 4;
    desc.deathFunc = -1;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.fadeStart = 30;
    desc.fadeRate = 8;
    extra.widthPhase = 0;
    extra.heightPhase = 0;
    extra.swayPhaseX = 0;
    extra.swayPhaseZ = 0;
    desc.vel.x = -vx * 0.2f;
    desc.vel.y = 0.0f;
    desc.vel.z = -vz * 0.2f;

    if (math_random() & 1) {
        desc.flags |= 0x40;
    }
    if (math_random() & 1) {
        desc.flags |= 0x80;
    }

    desc.timer = math_random() % 51U + 106;
    desc.alpha = math_random() % 101U + 100;
    extra.baseSize = desc.width = desc.height = math_randomFloat() * 62.0f + 41.0f;
    extra.widthPhaseStep = math_random() % 5U + 4;
    extra.heightPhaseStep = math_random() % 5U + 4;
    extra.widthWobble = (math_randomFloat() * 0.25f + 0.15f) * extra.baseSize;
    extra.heightWobble = (math_randomFloat() * 0.25f + 0.15f) * extra.baseSize;
    extra.swayPhaseStepX = math_random() % 5U + 4;
    extra.swayPhaseStepZ = math_random() % 5U + 4;
    extra.swayX = (math_randomFloat() * 0.1f + 0.08f) * extra.baseSize;
    extra.swayZ = (math_randomFloat() * 0.1f + 0.08f) * extra.baseSize;
    desc.gravity = (math_randomFloat() * 60.0f + 21.0f) * 0.001f;

    particle = particle_newSpriteParticle(&desc, 2, 0x28);
    if (particle != NULL) {
        memcpy(particle + 0xA8, &extra, sizeof(extra));
    }
}

/* Callback that ignores its argument and calls effect_updateDistantTracers. */
void effect_levelUpdateDistantTracers(s32 unused) {
    effect_updateDistantTracers();
}

/* Level hook gProjectileHitCallback: sets task_flag bits 0 and 1 on db_Object_array entry `index` if its id is 0xE0. */
void func_150EA904(Vec3f *pos, s32 index) {
    // matching: the no-op cast makes IDO load the array base before scaling index
    WorldObject *entry = (WorldObject *)db_Object_array + index;

    if (entry->unique_id == 0xE0) {
        entry->task_flag = (entry->task_flag & 0xFF) | TASK_FLAG_ON; // matching: the no-op mask costs IDO one extra temp register
    }
}

/* While task_flag bits 0-1 are set: spins and moves the entry, applies gravity (param, y kept in 1/8
 * units in touch.i.a) and fades its alpha out, then marks it inactive. */
void func_150EA944(WorldObject *entry) {
    f32 frames;
    f32 angle;
    s32 timer;

    if ((entry->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        frames = gFrameTicks;

        entry->t += entry->spinX * frames;
        angle = entry->t;
        if (angle < 0.0f) {
            entry->t = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->t = angle - 360.0f;
        }

        entry->u += entry->spinY * frames;
        angle = entry->u;
        if (angle < 0.0f) {
            entry->u = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->u = angle - 360.0f;
        }

        entry->v += entry->spinZ * frames;
        angle = entry->v;
        if (angle < 0.0f) {
            entry->v = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->v = angle - 360.0f;
        }

        timer = entry->alpha;
        entry->moveY -= entry->moveData.word * gFrameTicks;
        entry->x += entry->moveX * gFrameTicks;
        entry->touch.i.a += entry->moveY * gFrameTicks;
        entry->y = entry->touch.i.a >> 3;
        entry->z += entry->moveZ * gFrameTicks;

        timer -= gFrameTicks * 4;
        if (timer > 0) {
            entry->alpha = timer;
        } else {
            entry->inactive = 1;
        }
    }
}
