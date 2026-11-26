#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// particle_spawnKindSprite (game/Particle/Steam/steam.c) is declared in each file that calls it: game/Effects/Blood/blood.c was compiled with a
// different view of it.
SteamParticle *particle_spawnKindSprite(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
                             u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/*
 * Attaches two mirrored blood_spawnStrip effects (unk8 13 and 12) to `owner`, lasting `duration`,
 * or 300 when `duration` is 0.
 */
void blood_attachMirroredStrips(Actor *owner, s16 duration, u8 arg2, s32 arg3) {
    BloodStripDesc desc;

    if (owner == NULL) {
        return;
    }

    effect_sendActorEventToType(0x29, owner, 0x12);

    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.bone = 13;
    desc.offsetA.x = 2.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 34.0f;
    desc.offsetB.x = -15.0f;
    desc.offsetB.y = 0.0f;
    desc.offsetB.z = 34.0f;
    desc.flags = 0;
    desc.sampleCallback = -1;
    desc.emitCallback = 1;
    desc.rateCallback = -1;
    desc.rate = 0.4720000327f;
    desc.mode = 2;
    desc.updateCallback = -1;

    if (duration != 0) {
        desc.lifetime = duration;
        desc.flags = 2;
    } else {
        desc.lifetime = 300;
    }

    desc.duration = math_random() % 66U + 35;
    desc.pause = math_random() % 26U + 25;
    blood_spawnStrip(&desc, 0);

    desc.bone = 12;
    desc.offsetA.x = -2.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 34.0f;
    desc.offsetB.x = 15.0f;
    desc.offsetB.y = 0.0f;
    desc.offsetB.z = 34.0f;

    desc.duration = math_random() % 66U + 35;
    desc.pause = math_random() % 26U + 25;
    blood_spawnStrip(&desc, 0);
}

/* Spawns one particle_spawnKindSprite particle at `from`, moving towards `to` with randomized size and lifetime. */
void blood_emitMirroredStripParticle(f32 *from, f32 *to, s32 arg2, s32 arg3, s32 arg4, EffectHeader *task) {
    f32 vel[3];
    s16 fadeTime;
    s32 lifetime;
    f32 rand2;
    f32 rand1;
    f32 rand0;

    vel[0] = (to[0] - from[0]) * 0.407f;
    vel[1] = (to[1] - from[1]) * 0.407f;
    vel[2] = (to[2] - from[2]) * 0.407f;
    fadeTime = math_random() % 31U + 42;
    rand0 = math_randomFloat();
    rand1 = math_randomFloat();
    rand2 = math_randomFloat();
    lifetime = math_random() % 101U + 155;
    particle_spawnKindSprite(0, from, vel, rand0 * 70.0f + 30.0f, rand1 * 0.020000001f + 1.0400001f, 1.0f,
                  rand2 * 0.194f + -0.35000002f, fadeTime, lifetime, fadeTime, 255 / fadeTime, 1, task->owner,
                  task->kind);
}
