#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

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

s32 func_151CEC10(Vec3f *pos, Actor *owner, DescParticleHead *anchor);

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

/* Spawns a trail effect that follows owner (or anchor, when given). */
WaveTrail *func_151CEAAC(Actor *owner, DescParticleHead *anchor, u8 arg2, u8 arg3, s32 arg4) {
    EffectTemplate spawn;
    WaveTrailData info;
    WaveTrail *trail;
    s32 flags;
    f32 random;

    spawn.itemCount = 25;
    spawn.life = 300;
    spawn.flags = 16;
    spawn.kind = 17;
    spawn.unk14 = 3;

    info.owner = owner;
    if (owner != NULL) {
        info.ownerId = owner->unique_id;
    } else {
        info.ownerId = 0;
    }

    info.scaled.z = 1.0f;
    info.normal.z = 1.0f;
    info.anchor = anchor;
    info.scaled.x = 0.0f;
    info.normal.x = 0.0f;
    if (arg2) {
        flags = WAVE_TRAIL_SMOOTH;
    } else {
        flags = 0;
    }
    info.flags = flags;

    info.maxLength = func_150ADA68() * 400.0f + 400.0f;
    random = func_150ADA68();
    info.phase = (random + random) * 3.141592741f; /* random angle in [0, 2pi) */

    if (!func_151CEC10(&spawn.pos, owner, anchor)) {
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
    }

    trail = func_15147A80(&spawn, 40, 40, 0, 15, 15, 0, 0, 0, arg3, arg4);
    if (trail != NULL) {
        memcpy(trail->data, &info, sizeof(info));
    }
    return trail;
}
