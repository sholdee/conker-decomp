#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151DB5D0 (game_2062D0.c) is declared in each file that calls it: game/Effects/Blood/blood.c was compiled with a
// different view of it.
SteamParticle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
                             u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/*
 * Attaches two mirrored func_15134DAC effects (unk8 13 and 12) to `owner`, lasting `duration`,
 * or 300 when `duration` is 0.
 */
void func_150C7020(Actor *owner, s16 duration, u8 arg2, s32 arg3) {
    BloodStripDesc desc;

    if (owner == NULL) {
        return;
    }

    func_1516962C(0x29, owner, 0x12);

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

    desc.duration = func_150ADA20() % 66U + 35;
    desc.pause = func_150ADA20() % 26U + 25;
    func_15134DAC(&desc, 0);

    desc.bone = 12;
    desc.offsetA.x = -2.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 34.0f;
    desc.offsetB.x = 15.0f;
    desc.offsetB.y = 0.0f;
    desc.offsetB.z = 34.0f;

    desc.duration = func_150ADA20() % 66U + 35;
    desc.pause = func_150ADA20() % 26U + 25;
    func_15134DAC(&desc, 0);
}

/* Spawns one func_151DB5D0 particle at `from`, moving towards `to` with randomized size and lifetime. */
void func_150C71C0(f32 *from, f32 *to, s32 arg2, s32 arg3, s32 arg4, EffectHeader *task) {
    f32 vel[3];
    s16 fadeTime;
    s32 lifetime;
    f32 rand2;
    f32 rand1;
    f32 rand0;

    vel[0] = (to[0] - from[0]) * 0.407f;
    vel[1] = (to[1] - from[1]) * 0.407f;
    vel[2] = (to[2] - from[2]) * 0.407f;
    fadeTime = func_150ADA20() % 31U + 42;
    rand0 = func_150ADA68();
    rand1 = func_150ADA68();
    rand2 = func_150ADA68();
    lifetime = func_150ADA20() % 101U + 155;
    func_151DB5D0(0, from, vel, rand0 * 70.0f + 30.0f, rand1 * 0.020000001f + 1.0400001f, 1.0f,
                  rand2 * 0.194f + -0.35000002f, fadeTime, lifetime, fadeTime, 255 / fadeTime, 1, task->owner,
                  task->kind);
}
