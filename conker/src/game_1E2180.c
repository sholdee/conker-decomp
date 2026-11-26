#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c, game_161520.c and
// game_1DC720.c were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

#define BLOOD_PROFILE_NONE 99

/* Attaches emitter 8 (func_151B4E4C) to bone 2 of `actor` for 40-60 frames, unless its blood profile
 * rules it out (unkE == 2). The emitter's 1-byte payload records whether unkE == 1. */
BloodDrip *func_151B4CD0(Actor *actor, u8 arg1, s32 arg2) {
    BloodDrip *emitter;
    BloodDripDesc desc;
    u8 payload[1];
    s32 profile;

    if (actor == NULL) {
        return NULL;
    }

    profile = func_15134070(actor);
    if (profile == BLOOD_PROFILE_NONE) {
        return NULL;
    }

    if (D_800A3FD8[profile].unkE == 2) {
        return NULL;
    }

    func_1516962C(40, actor, 22);

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 2;
    desc.offset.x = 0.0f;
    desc.offset.y = -27.0f;
    desc.offset.z = 16.0f;
    desc.radius = 15.0f;
    desc.rate = 0.448f;
    desc.flags = 0x06; /* ON_BONE | LIFETIME (BLOOD_DRIP_* in game_161520.c) */
    desc.lifetime = func_150ADA20() % 21U + 40;
    desc.emitter = 8;
    desc.callback = -1;
    desc.unk2D = 7;

    if (D_800A3FD8[profile].unkE == 1) {
        payload[0] = 1;
    } else {
        payload[0] = 0;
    }

    emitter = func_1513418C(&desc, 1, arg1, arg2);
    if (emitter != NULL) {
        memcpy(emitter->payload, payload, 1);
    }
    return emitter;
}

/* Emitter 8 callback (D_80089A70): spawns one particle at (x, y, z). */
void func_151B4E4C(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151B4EA4(position, dirX, dirY, dirZ, emitter->payload[0], emitter->header.owner);
}

/* Spawns a randomized func_151D9014 particle at `position`, moving against (dirX, dirY, dirZ). */
void func_151B4EA4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4, u8 arg5) {
    f32 speed;
    f32 velocity[3];
    s32 coin;
    f32 fracB;
    u32 randB;
    u32 randA;
    f32 fracA;

    // matching: named temporaries; passing the calls inline changes the stack frame
    speed = 0.20500001f;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    fracA = func_150ADA68();
    randA = func_150ADA20();
    randB = func_150ADA20();
    fracB = func_150ADA68();
    coin = func_150ADA20() & 1;
    func_151D9014(position, velocity, arg4, fracA * 0.497f + -1.0f, randA % 31 + 20, randB % 101 + 155,
                  fracB * 152.0f + 109.0f, coin, 1.7f, 1.7f, 0, 0, 1, 0, arg5, 1);
}

/* Attaches emitter 9 (func_151B50A4) to bone 2 of `actor`. */
BloodDrip *func_151B4FE0(Actor *actor, u8 arg1, s32 arg2) {
    BloodDrip *emitter;
    BloodDripDesc desc;

    if (actor == NULL) {
        return NULL;
    }
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 2;
    desc.offset.x = 0.0f;
    desc.offset.y = -27.0f;
    desc.offset.z = 16.0f;
    desc.radius = 9.8f;
    desc.rate = 0.143f;
    desc.flags = 0x02; /* ON_BONE */
    desc.lifetime = 300;
    desc.emitter = 9;
    desc.callback = -1;
    desc.unk2D = 8;
    emitter = func_1513418C(&desc, 0, arg1, arg2); // matching: returning the call directly changes the stack frame
    return emitter;
}

/* Emitter 9 callback (D_80089A70): spawns one particle at (x, y, z). */
void func_151B50A4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151B50F4(position, dirX, dirY, dirZ, emitter->header.owner);
}

/* Spawns a randomized func_151D9014 particle at `position`, moving against (dirX, dirY, dirZ). */
void func_151B50F4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4) {
    f32 speed;
    f32 velocity[3];
    u8 coin;
    f32 scaledB;
    u32 randB;
    u32 randA;
    f32 fracA;

    // matching: named temporaries; passing the calls inline changes the stack frame
    speed = 0.13000001f;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    if (func_150ADA20() & 1) {
        coin = 1;
    } else {
        coin = 0;
    }
    fracA = func_150ADA68();
    randA = func_150ADA20();
    randB = func_150ADA20();
    scaledB = func_150ADA68() * 202.0f + 142.0f;
    func_151D9014(position, velocity, 0, fracA * 0.126f + -0.702f, (randA & 0xF) + 10, randB % 45 + 137, scaledB,
                  coin, 1.7f, 1.7f, 0, 0, 1, 0, arg4, 1);
}
