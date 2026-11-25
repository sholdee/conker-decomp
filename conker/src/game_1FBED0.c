#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

typedef struct { f32 x, z; } TrailVec2;

/* The object a trail follows (see func_151CEC10 / func_151CEC54 in game_1FC0C0.c). */
typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
} TrailOwner;

/* Per-trail data block, copied into the spawned object's data pointer (+0x98). */
typedef struct {
    /* 0x00 */ TrailOwner *owner;
    /* 0x04 */ u8 ownerUnk3B; /* owner->unk3B at spawn; the trail dies when it changes */
    /* 0x08 */ void *target;
    /* 0x0C */ TrailVec2 scaled;
    /* 0x14 */ TrailVec2 normal;
    /* 0x1C */ u8 flags;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 angle;
} TrailInfo; /* 0x28 */

typedef struct {
    u8 pad0[0x98];
    TrailInfo *info;
} TrailObject;

/* The object func_151CEA20 integrates: a 1-D position under constant acceleration, and a fade. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 position;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ f32 acceleration;
    /* 0x44 */ f32 velocity;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ f32 fadeRate;
    /* 0x50 */ f32 fade;
} TrailScalarMotion;

extern TrailObject *func_15147A80(EffectTemplate *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_151CEC10(Vec3f *pos, TrailOwner *owner, void *target);

/* Integrates position/velocity under constant acceleration over one frame and ramps fade up to 1. */
s32 func_151CEA20(TrailScalarMotion *motion) {
    {
        f32 prevVelocity = motion->velocity;

        motion->velocity += motion->acceleration * D_800BE9A4;
        motion->position += (prevVelocity + 0.5f * motion->acceleration * D_800BE9A4) * D_800BE9A4;
    }
    motion->fade += motion->fadeRate * D_800BE9A4;
    if (motion->fade > 1.0f) {
        motion->fade = 1.0f;
    }
    return 1;
}

/* Spawns a trail effect that follows owner (or target, when given). */
TrailObject *func_151CEAAC(TrailOwner *owner, void *target, u8 arg2, u8 arg3, s32 arg4) {
    EffectTemplate spawn;
    TrailInfo info;
    TrailObject *trail;
    s32 flags;
    f32 random;

    spawn.itemCount = 25;
    spawn.life = 300;
    spawn.flags = 16;
    spawn.kind = 17;
    spawn.unk14 = 3;

    info.owner = owner;
    if (owner != NULL) {
        info.ownerUnk3B = owner->unk3B;
    } else {
        info.ownerUnk3B = 0;
    }

    info.scaled.z = 1.0f;
    info.normal.z = 1.0f;
    info.target = target;
    info.scaled.x = 0.0f;
    info.normal.x = 0.0f;
    if (arg2) {
        flags = 2;
    } else {
        flags = 0;
    }
    info.flags = flags;

    info.unk20 = func_150ADA68() * 400.0f + 400.0f;
    random = func_150ADA68();
    info.angle = (random + random) * 3.141592741f; /* random angle in [0, 2pi) */

    if (!func_151CEC10(&spawn.pos, owner, target)) {
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
    }

    trail = func_15147A80(&spawn, 40, 40, 0, 15, 15, 0, 0, 0, arg3, arg4);
    if (trail != NULL) {
        memcpy(trail->info, &info, sizeof(info));
    }
    return trail;
}
