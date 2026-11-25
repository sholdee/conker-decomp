#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-model blood settings (game_161520.c's BloodProfile), indexed by func_15134070's profile. */
typedef struct {
    /* 0x0 */ f32 offset[3];
    /* 0xC */ u16 unkC;
    /* 0xE */ u8 unkE;
    /* 0xF */ u8 padF;
} BloodProfile;

extern BloodProfile D_800A3FD8[];

#define BLOOD_PROFILE_NONE 99

/* Descriptor passed to func_1513418C (same layout as game_117490.c's EmitterDesc). */
typedef struct {
    /* 0x00 */ s32 target[2];
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[0x3];
    /* 0x0C */ struct127 *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ f32 offset[3];
    /* 0x20 */ f32 radius;
    /* 0x24 */ f32 rate;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ u8 flags;
    /* 0x2B */ u8 emitter; /* index into D_80089A70 */
    /* 0x2C */ s8 callback;
    /* 0x2D */ u8 unk2D;
} EmitterDesc;

/* The effect func_1513418C returns (BloodDrip in game_161520.c), followed by its payload. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x4B];
    /* 0x58 */ u8 payload[1];
} Emitter;

s32 func_15134070(struct127 *);
void func_1516962C(s32, struct127 *, s32);
Emitter *func_1513418C(EmitterDesc *, s32, u8, s32);
void func_151D9014(f32 *position, f32 *velocity, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, u8,
                   s32);

/* Attaches emitter 8 (func_151B4E4C) to bone 2 of `actor`, unless its blood profile has none. */
Emitter *func_151B4CD0(struct127 *actor, u8 arg1, s32 arg2) {
    Emitter *emitter;
    EmitterDesc desc;
    u8 payload[4];
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
    desc.offset[0] = 0.0f;
    desc.offset[1] = -27.0f;
    desc.offset[2] = 16.0f;
    desc.radius = 15.0f;
    desc.rate = 0.448f;
    desc.flags = 6;
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

void func_151B4EA4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4, u8 arg5);

/* Emitter 8 callback (D_80089A70): spawns one particle at (x, y, z). */
void func_151B4E4C(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Emitter *emitter) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151B4EA4(position, dirX, dirY, dirZ, emitter->payload[0], emitter->unkC);
}

/* Spawns a randomized func_151D9014 particle at `position`, moving against (dirX, dirY, dirZ). */
void func_151B4EA4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4, u8 arg5) {
    f32 speed;
    f32 velocity[3];
    s32 c;
    f32 g;
    u32 b;
    u32 a;
    f32 f;

    speed = 0.20500001f;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    f = func_150ADA68();
    a = func_150ADA20();
    b = func_150ADA20();
    g = func_150ADA68();
    c = func_150ADA20() & 1;
    func_151D9014(position, velocity, arg4, f * 0.497f + -1.0f, a % 31 + 20, b % 101 + 155, g * 152.0f + 109.0f, c,
                  1.7f, 1.7f, 0, 0, 1, 0, arg5, 1);
}

/* Attaches emitter 9 (func_151B50A4) to bone 2 of `actor` for 300 frames. */
Emitter *func_151B4FE0(struct127 *actor, u8 arg1, s32 arg2) {
    Emitter *emitter;
    EmitterDesc desc;

    if (actor == NULL) {
        return NULL;
    }
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 2;
    desc.offset[0] = 0.0f;
    desc.offset[1] = -27.0f;
    desc.offset[2] = 16.0f;
    desc.radius = 9.8f;
    desc.rate = 0.143f;
    desc.flags = 2;
    desc.lifetime = 300;
    desc.emitter = 9;
    desc.callback = -1;
    desc.unk2D = 8;
    emitter = func_1513418C(&desc, 0, arg1, arg2);
    return emitter;
}

void func_151B50F4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4);

/* Emitter 9 callback (D_80089A70): spawns one particle at (x, y, z). */
void func_151B50A4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Emitter *emitter) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151B50F4(position, dirX, dirY, dirZ, emitter->unkC);
}

/* Spawns a randomized func_151D9014 particle at `position`, moving against (dirX, dirY, dirZ). */
void func_151B50F4(f32 *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4) {
    f32 speed;
    f32 velocity[3];
    u8 flag;
    f32 c;
    u32 b;
    u32 a;
    f32 f;

    speed = 0.13000001f;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    flag = (func_150ADA20() & 1) ? 1 : 0;
    f = func_150ADA68();
    a = func_150ADA20();
    b = func_150ADA20();
    c = func_150ADA68() * 202.0f + 142.0f;
    func_151D9014(position, velocity, 0, f * 0.126f + -0.702f, (a & 0xF) + 10, b % 45 + 137, c, flag, 1.7f, 1.7f, 0,
                  0, 1, 0, arg4, 1);
}
