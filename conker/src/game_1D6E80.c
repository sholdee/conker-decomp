#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c, game_161520.c and
// game_1DC720.c were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

struct ActorTask;

/*
 * The three effects an ActorTask keeps alive. Each effect object carries a payload (copied in at spawn)
 * that points back at its owner task; the owner sets payload.keepAlive every frame, the effect's update
 * clears it and dies once it is no longer refreshed, and its destructor clears the owner's pointer.
 */

/* Payload at +0x80 of the effect spawned by func_151AA48C: a value oscillating around `base`. */
typedef struct {
    /* 0x00 */ struct ActorTask *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5[0x3];
    /* 0x08 */ u32 phase;      // angle in the top byte (16.16)
    /* 0x0C */ u32 phaseStep;  // per frame unit
    /* 0x10 */ f32 base;
    /* 0x14 */ f32 amplitude;
} PulseFxPayload;

struct PulseFx {
    /* 0x00 */ BloodStrip strip;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ PulseFxPayload payload;
};

/* Payload at +0x58 of the effect spawned by func_151AB2C4: holds a looping sound. */
typedef struct {
    /* 0x00 */ struct ActorTask *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 soundHandle;
    /* 0x08 */ f32 unk8;
} LoopSoundFxPayload;

struct LoopSoundFx {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x48];
    /* 0x58 */ LoopSoundFxPayload payload;
};

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PulseFx *pulse;
    /* 0x18 */ FlickerSprite *flicker;
    /* 0x1C */ LoopSoundFx *sound;
} ActorTaskFx;

/* Callback block attached to an actor; the callbacks below drop effects onto the ground under it. */
struct ActorTask {
    /* 0x00 */ CharacterWatcher watcher;
    /* 0x58 */ ActorTaskFx fx;
};

/* Probes the ground under the task's actor and spawns ground effect 3 there. */
void func_151A99D0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (func_15045800(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, &task->watcher.hit, 3, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Ground effect 3 under the actor (with sound 0x5208), then func_151ABE00. */
void func_151A9AA4(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (func_15045800(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, hit, 3, task->watcher.header.owner, task->watcher.header.kind);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    func_151ABE00(actor);
}

/* Ground effect 1 under the actor, with sound 0x5208. */
void func_151A9BA0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (func_15045800(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, hit, 1, task->watcher.header.owner, task->watcher.header.kind);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}


/* Unless the actor's mv->ladder is set: ground effect 1 (sound 0x6978), func_151AA264, func_151ABE00. */
void func_151A9CA0(ActorTask *task) {
    Actor *actor = task->watcher.character;
    Vec3f pos;

    // matching: structs.h types unk95 as s8, but this read is unsigned (lbu)
    if ((actor->mv != NULL) && (actor->mv->ladder != 0)) {
        return;
    }
    pos.x = actor->xpos;
    if (-9500.0f < actor->unk118) {
        pos.y = actor->unk118 + 100.0f;
    } else {
        pos.y = actor->ypos + 150.0f;
    }
    pos.z = actor->zpos;
    if (func_15045800(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
        pos.y = task->watcher.hit.height;
        func_151ABE40(&pos, &task->watcher.hit, 1, task->watcher.header.owner, task->watcher.header.kind);
        func_10010FFC(0, 8, 0x6978, 0, 0, actor);
    }
    func_151AA264(actor, &task->watcher.hit);
    func_151ABE00(actor);
}

/* Ground effect 2 under the actor, with sound 0x5208. */
void func_151A9DC0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (func_15045800(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, hit, 2, task->watcher.header.owner, task->watcher.header.kind);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}

/* Ground effect 2 (sound 0x6978), then func_151AA264 and func_151ABE00. */
void func_151A9EC0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (func_15045800(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, hit, 2, task->watcher.header.owner, task->watcher.header.kind);
            func_10010FFC(0, 8, 0x6978, 0, 0, actor);
        }
    }
    func_151AA264(actor, &task->watcher.hit);
    func_151ABE00(actor);
}

/* Ground effect 4 under the actor. */
void func_151A9FC8(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (func_15045800(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, &task->watcher.hit, 4, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Ground effect 4 under the actor. */
void func_151AA09C(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (func_15045800(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, &task->watcher.hit, 4, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Empty callback. */
void func_151AA170(s32 arg0) {
}

struct Task151AA17C {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
};

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
} Msg151AA17C;

/* Sends message 10 ({unk18, unk1C}) through func_15147D64 and func_151494E0, then func_1519F3B8. */
void func_151AA17C(Task151AA17C *task) {
    s32 word;
    Msg151AA17C msg;

    word = task->unk18;
    msg.unk0 = word;
    msg.unk4 = task->unk1C;
    word = (s32)&msg; // matching: one int local reused for &msg; a pointer local is rematerialized instead
    func_15147D64(word, 10);
    func_151494E0(word, 10);
    func_1519F3B8(task);
}

void func_151AA1D0(void) {
    func_1519F400();
}

void func_151AA1F0(void) {
    func_1519F400();
}

/* Same as func_151AA17C. */
void func_151AA210(Task151AA17C *task) {
    s32 word;
    Msg151AA17C msg;

    word = task->unk18;
    msg.unk0 = word;
    msg.unk4 = task->unk1C;
    word = (s32)&msg; // matching: one int local reused for &msg; a pointer local is rematerialized instead
    func_15147D64(word, 10);
    func_151494E0(word, 10);
    func_1519F3B8(task);
}

/* When the ground hit has flag bit 0 set, spawns a func_1513418C effect on the actor. */
void func_151AA264(Actor *actor, CollisionHit *hit) {
    BloodDripDesc desc;

    if (hit->flags & 1) {
        desc.target[0] = 0;
        desc.target[1] = 0;
        desc.ownerId = actor->unique_id;
        desc.bone = 1;
        desc.offset.x = 0.0f;
        desc.offset.y = 0.0f;
        desc.offset.z = 0.0f;
        desc.radius = 25.0f;
        desc.rate = 1.212440014f;
        desc.lifetime = 30;
        desc.flags = 14;
        desc.emitter = 2;
        desc.callback = -1;
        desc.unk2D = 0;
        desc.owner = actor;
        func_1513418C(&desc, 0, 0xFF, 0);
    }
}

struct Task151AA30C {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ Actor *actor;
};

/* Spawns one falling particle at (x, y, z) when that point is not above the task actor's unk118. */
void func_151AA30C(f32 x, f32 y, f32 z, f32 arg3, f32 scale, f32 arg5, Task151AA30C *task) {
    Vec3f pos;
    Vec3f vel;
    Actor *actor;
    CollisionHit *ground;

    actor = task->actor;
    if (actor->unk118 < y) {
        return;
    }
    if (func_1515A920(actor, &ground) == 0) {
        ground = NULL;
    }
    vel.x = 0.0f;
    vel.y = -((func_150ADA68() * 196.0f + 199.0f) * 0.001000000047f * scale);
    vel.z = 0.0f;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    func_1515A238(&pos, &vel, (func_150ADA68() * 311.0f + 454.0f) * 0.001000000047f, 0.972683012f, ground,
                  func_150ADA68() * 101.0f + 101.0f, func_150ADA20() % 31U + 50, func_150ADA20() % 101U + 100,
                  1, task->header.owner, task->header.kind);
}

/* Spawns the pulse effect owned by `task`. */
PulseFx *func_151AA48C(Actor *actor, ActorTask *task) {
    BloodStripDesc desc;
    PulseFx *fx;
    PulseFxPayload payload;

    payload.owner = task;
    payload.keepAlive = 1;
    desc.ownerId = actor->unique_id;

    desc.owner = actor;
    desc.bone = 15;
    desc.offsetA.x = -11.0f;
    desc.offsetA.y = -2.0f;
    desc.offsetA.z = 8.0f;
    desc.offsetB.x = -11.0f;
    desc.offsetB.y = -2.0f;
    desc.offsetB.z = 20.0f;
    desc.flags = 0;
    desc.duration = 60;
    desc.pause = 60;
    desc.lifetime = 300;
    desc.sampleCallback = 1;
    desc.emitCallback = 0;
    desc.rateCallback = 1;
    desc.rate = 0.5f;
    desc.mode = 1;
    desc.updateCallback = 0;

    payload.phase = 0;
    payload.phaseStep = 0x11111;
    payload.base = 0.200000003f;
    payload.amplitude = 0.3000000119f;

    fx = (PulseFx *)func_15134DAC(&desc, sizeof(payload)); /* a BloodStrip with the payload after it */
    if (fx != NULL) {
        memcpy(&fx->payload, &payload, sizeof(payload));
    }
    return fx;
}

/* PulseFx callback: restarts the pulse and plays sound 10 or 11 (at random) at the effect. */
s32 func_151AA5A4(PulseFx *fx) {
    fx->payload.phase = 0;
    if (func_150ADA20() & 1) {
        func_10010F88(10, 0x55F0, (func_151EF610() % 1200) - 600, 0, 0, fx->strip.posA.x, fx->strip.posA.y, fx->strip.posA.z, 500, 2500);
        return 1;
    }
    func_10010F88(11, 0x55F0, (func_151EF610() % 1200) - 600, 0, 0, fx->strip.posA.x, fx->strip.posA.y, fx->strip.posA.z, 500, 2500);
    return 1;
}

struct Obj151AA6D8 {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ f32 unk74;
};

/* Spawns a particle burst moving from `from` toward `to`, advanced by `t` frames. */
void func_151AA6D8(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, Obj151AA6D8 *obj) {
    ParticleDesc burst;
    void *spawned;
    RiseMotion payload;

    burst.texture = 0x2F;
    burst.renderMode = 0xC01;
    burst.unk0 = 0x200005;
    burst.unk4 = 0;
    burst.b2 = 0;
    burst.g2 = 0;
    burst.r2 = 0;
    burst.a = 0;
    burst.b = 0;
    burst.g = 0;
    burst.r = 0;
    burst.frameRate = 0;
    burst.frame = 0;
    burst.brightness = 0xFF;
    burst.scaleStart = 0;
    burst.scaleRate = 0.0f;
    burst.flags = 0x1207;
    burst.primColorMode = 5;
    burst.envColorMode = 5;
    burst.updateFunc = 4;
    burst.deathFunc = -1;
    burst.offset.x = 0.0f;
    burst.offset.y = 0.0f;
    burst.offset.z = 0.0f;
    burst.fadeStart = 20;
    burst.fadeRate = 12;
    payload.widthPhase = 0;
    payload.heightPhase = 0;
    payload.swayPhaseX = 0;
    payload.swayPhaseZ = 0;
    payload.maxRiseSpeed = 3.5f;
    payload.swayPerSpeed = 0.2857142985f;

    burst.vel.x = (to->x - from->x) * obj->unk74;
    burst.vel.y = (to->y - from->y) * obj->unk74;
    burst.vel.z = (to->z - from->z) * obj->unk74;
    burst.pos = *from;
    burst.pos.x += burst.vel.x * t;
    burst.pos.y += burst.vel.y * t;
    burst.pos.z += burst.vel.z * t;

    if (func_150ADA20() & 1) {
        burst.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        burst.flags |= 0x80;
    }

    burst.timer = (func_150ADA20() % 51U) + 50;
    burst.alpha = (func_150ADA20() % 101U) + 100;
    payload.baseSize = burst.width = burst.height = (func_150ADA68() * 39.0f) + 31.0f;
    payload.widthPhaseStep = (func_150ADA20() % 5U) + 4;
    payload.heightPhaseStep = (func_150ADA20() % 5U) + 4;
    payload.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.swayPhaseStepX = (func_150ADA20() % 5U) + 4;
    payload.swayPhaseStepZ = (func_150ADA20() % 5U) + 4;
    payload.swayX = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    payload.swayZ = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    burst.gravity = ((func_150ADA68() * 70.0f) + 73.0f) * 0.001000000047f;

    spawned = func_151303BC(&burst, 2, sizeof(payload));
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0xA8, &payload, sizeof(payload));
    }
}

/* PulseFx update: returns the current oscillation value and advances the phase. */
f32 func_151AAA4C(PulseFx *fx) {
    PulseFxPayload *pulse;
    f32 value;

    pulse = &fx->payload;
    value = (func_151423D8((pulse->phase >> 16) - 0x40) * pulse->amplitude) + pulse->base;
    pulse->phase += pulse->phaseStep * D_800BE9E4;
    return value;
}

/* Refreshes the task's pulse and loop-sound effects, spawning whichever is missing. */
void func_151AAABC(ActorTask *task) {
    Actor *actor;
    ActorTaskFx *fx;

    actor = task->watcher.character;
    fx = &task->fx;
    if (fx->pulse != NULL) {
        PulseFxPayload *pulse;

        pulse = &fx->pulse->payload;
        pulse->keepAlive = 1;
    } else {
        fx->pulse = func_151AA48C(actor, task);
    }
    if (fx->sound != NULL) {
        LoopSoundFxPayload *sound;

        sound = &fx->sound->payload;
        sound->keepAlive = 1;
    } else {
        fx->sound = func_151AB2C4(actor, task);
    }
}

/* PulseFx destructor (variant 1): detaches from the owner task. */
void func_151AAB50(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    func_151352EC(&fx->strip.header);
}

/* PulseFx destructor (variant 2): detaches from the owner task. */
void func_151AAB78(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    func_1513530C(&fx->strip.header);
}

/* PulseFx keep-alive poll: TRUE if the owner refreshed it since the last call. */
s32 func_151AABA0(PulseFx *fx) {
    s32 alive = TRUE;

    if (fx->payload.keepAlive == 0) {
        alive = FALSE;
    }
    fx->payload.keepAlive = 0;
    return alive;
}

extern u8 D_800A8F70[];

/* Finds point D_800A8F70[index] of the actor's ob_matrix_ptr (func_15142314) and spawns two effects on the ground below it. */
void func_151AABC4(Actor *actor, u8 index) {
    Vec3f pos;
    CollisionHit floor;
    f32 y;
    Vec3f probe;
    u8 color;
    Vec3f hit;

    if ((actor != NULL) && (actor->ob_matrix_ptr != NULL)) {
        color = func_151D8E20();
        func_15142314(actor->ob_matrix_ptr, D_800A8F70[index], &pos, actor);
        func_1504715C(&floor, actor);
        y = pos.y + 50.0f;
        probe.x = pos.x;
        probe.y = y;
        probe.z = pos.z;
        if (func_1504697C(&probe, 0, y - 100.0f, &floor) != 0) {
            hit.x = pos.x;
            hit.y = floor.height;
            hit.z = pos.z;
            func_151DBCBC(color, 40.0f, 150, &floor.tri, &hit, 0xFF, 1);
            {
                SplashSpraySpec c;

                c.spray.pos = hit;
                c.spray.scaleBase = 2.5f;
                c.spray.scaleRange = 1.549999952f;
                c.spray.sizeBase = 0.7910000086f;
                c.spray.sizeRange = 0.2030000091f;
                c.spray.speedBase = 8.039999962f;
                c.spray.speedRange = 3.449999809f;
                c.spray.countBase = 3;
                c.spray.countRange = 3;
                c.angles.yawRange = 0xFF;
                c.angles.pitchBase = -0x40;
                c.angles.pitchRange = 0x1A;
                c.angles.yawBase = 0;
                c.spray.itemCountBase = 3;
                c.spray.itemCountRange = 1;
                c.spray.lifeBase = 0x1E;
                c.spray.lifeRange = 0x14;
                c.spray.unk30 = 0x9B;
                c.spray.unk32 = 0x64;
                c.spray.kind = color;
                c.spray.unk3C = 0x10;
                c.spray.unk3E = 0xF;
                c.spray.unk40 = 0;
                c.spray.unk38 = 0.0f;
                func_15153F18(&c.angles, &c.spray, &floor, 0xFF, 1);
            }
        }
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[0x1B];
    /* 0x1B */ u8 unk1B;
} Obj151AADBC_98;

struct Obj151AADBC {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[0x7A];
    /* 0x98 */ Obj151AADBC_98 *unk98;
};

/* Caps obj->unk98->unk1B at obj->unk1C * 16 (at most 255). */
s32 func_151AADBC(Obj151AADBC *obj) {
    Obj151AADBC_98 *target;
    s32 limit;

    target = obj->unk98;
    limit = obj->unk1C << 4;
    if (limit >= 0x100) {
        limit = 0xFF;
    }
    if (limit < target->unk1B) {
        target->unk1B = limit;
    }
    return 1;
}


/* Keeps the task's flicker effect alive, or spawns it on the ground under the actor. */
void func_151AADF8(ActorTask *task) {
    Actor *actor;
    ActorTaskFx *fx;
    SpriteFlicker *keep;
    Vec3f pos;
    SpriteDef part;
    SpriteFlicker payload;
    f32 size;
    u8 color;

    actor = task->watcher.character;
    fx = &task->fx;
    if (fx->flicker != NULL) {
        keep = &fx->flicker->flicker;
        keep->keepAlive = 1;
    // matching: an unused label before the return makes IDO branch past the epilogue's first load
    refreshed:
        return;
    }
    pos.x = actor->xpos;
    if (-9500.0f < actor->unk118) {
        pos.y = actor->unk118 + 100.0f;
    } else {
        pos.y = actor->ypos + 150.0f;
    }
    pos.z = actor->zpos;
    if (func_15045800(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
        color = func_151D8E20();
        pos.y = task->watcher.hit.height;
        size = (func_150ADA68() * 30.0f) + 50.0f;
        payload.owner = &task->watcher;
        payload.keepAlive = 1;
        payload.minDelay = 10;
        payload.delayRange = 10;
        payload.minTarget = 26214.40039f;
        payload.targetRange = 32768.0f;
        payload.target = 0.0f;
        payload.timer = 0;
        payload.easing = 0.2310000062f;
        part.frameSpeed = (s32)(payload.targetRange * 0.5f + payload.minTarget);
        part.texture = 0x38;
        part.flags = 0x60300;
        part.lifetime = 300;
        part.unk10 = 150;
        part.frame = 0;
        part.unk11 = 0xFF;
        part.rgba[0] = D_800AB414[color].r;
        part.rgba[1] = D_800AB414[color].g;
        part.rgba[2] = D_800AB414[color].b;
        part.rgba[3] = 0xFF;
        part.renderModes = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        fx->flicker = (FlickerSprite *)func_1513C73C(&part, 7, 1, &task->watcher.hit.tri, pos.x, pos.y, pos.z,
                                                 size, size, func_150ADA20() & 0xFF,
                                                 (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), 0x20, 0xFF, 1);
        if (fx->flicker != NULL) {
            memcpy(&fx->flicker->flicker, &payload, sizeof(payload));
        }
    }
}


/* FlickerSprite update: eases the sprite's animation speed toward a target re-rolled after a random delay. */
s32 func_151AB090(FlickerSprite *fx) {
    s8 alive = TRUE;
    SpriteFlicker *flicker = &fx->flicker;

    if (fx->flicker.keepAlive == 0) {
        alive = FALSE;
    }
    flicker->keepAlive = 0;
    flicker->timer -= D_800BE9E4;
    if (flicker->timer < 0) {
        flicker->timer = (func_150ADA20() % (u32)(flicker->delayRange + 1)) + flicker->minDelay;
        flicker->target = func_150ADA68() * flicker->targetRange + flicker->minTarget;
    }
    fx->quad.def.frameSpeed += (s32)((flicker->target - (f32)fx->quad.def.frameSpeed) * flicker->easing);
    return alive;
}

/* FlickerSprite destructor: detaches from the owner task. */
s32 func_151AB180(FlickerSprite *fx) {
    ActorTaskFx *ownerFx = &((ActorTask *)fx->flicker.owner)->fx;

    ownerFx->flicker = NULL;
    fx->quad.def.flags |= 2;
    fx->flicker.owner = NULL;
    func_1513F6C0(&fx->quad, 0, 0);
    return 0;
}

/* Ground effect 3 (sound 0x5208), then func_151AA264. */
void func_151AB1C4(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos.y = actor->unk118 + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (func_15045800(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            func_151ABE40(&pos, hit, 3, task->watcher.header.owner, task->watcher.header.kind);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    func_151AA264(actor, &task->watcher.hit);
}

/* Spawns the looping-sound effect owned by `task`. */
LoopSoundFx *func_151AB2C4(Actor *actor, ActorTask *task) {
    BloodDripDesc desc;
    LoopSoundFxPayload payload;
    LoopSoundFx *fx;

    payload.owner = task;
    payload.soundHandle = 0;
    payload.keepAlive = 1;
    payload.unk8 = actor->unk118;
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.lifetime = 300;
    desc.flags = 10;
    desc.emitter = 3;
    desc.callback = 0;
    desc.unk2D = 1;
    desc.owner = actor;
    desc.radius = 25.0f;
    desc.rate = 0.2649700046f;

    fx = (LoopSoundFx *)func_1513418C(&desc, sizeof(payload), 0xFF, 0); /* a BloodDrip with the payload at 0x58 */
    if (fx != NULL) {
        memcpy(&fx->payload, &payload, sizeof(payload));
    }
    return fx;
}

struct Obj151AB3A4 {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ f32 unk60;
};

/* Spawns a particle burst at (x, y, z) drifting against (dx, dz). */
void func_151AB3A4(f32 x, f32 y, f32 z, f32 dx, f32 arg4, f32 dz, Obj151AB3A4 *obj) {
    ParticleDesc burst;
    void *spawned;
    RiseMotion payload;

    burst.texture = 0x2F;
    burst.renderMode = 0xC01;
    burst.unk0 = 0x200005;
    burst.unk4 = 0;
    burst.b2 = 0;
    burst.g2 = 0;
    burst.r2 = 0;
    burst.a = 0;
    burst.b = 0;
    burst.g = 0;
    burst.r = 0;
    burst.frameRate = 0;
    burst.frame = 0;
    burst.brightness = 0xFF;
    burst.scaleStart = 0;
    burst.scaleRate = 0.0f;
    burst.flags = 0x3207;
    burst.primColorMode = 5;
    burst.envColorMode = 5;
    burst.updateFunc = 4;
    burst.deathFunc = -1;
    burst.pos.x = x;
    burst.pos.y = y;
    burst.offset.x = 0.0f;
    burst.offset.y = 0.0f;
    burst.offset.z = 0.0f;
    burst.fadeStart = 30;
    burst.fadeRate = 8;
    burst.pos.z = z;
    payload.ceiling = obj->unk60;
    payload.maxRiseSpeed = 4.0f;
    payload.swayPerSpeed = 0.25f;
    payload.widthPhase = 0;
    payload.heightPhase = 0;
    payload.swayPhaseX = 0;
    payload.swayPhaseZ = 0;
    burst.vel.x = -dx * 0.2f;
    burst.vel.y = 0.0f;
    burst.vel.z = -dz * 0.2f;
    if (func_150ADA20() & 1) {
        burst.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        burst.flags |= 0x80;
    }
    burst.timer = (func_150ADA20() % 51U) + 106;
    burst.alpha = (func_150ADA20() % 101U) + 100;
    payload.baseSize = burst.width = burst.height = (func_150ADA68() * 62.0f) + 41.0f;
    payload.widthPhaseStep = (func_150ADA20() % 5U) + 4;
    payload.heightPhaseStep = (func_150ADA20() % 5U) + 4;
    payload.widthWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.heightWobble = ((func_150ADA68() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.swayPhaseStepX = (func_150ADA20() % 5U) + 4;
    payload.swayPhaseStepZ = (func_150ADA20() % 5U) + 4;
    payload.swayX = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    payload.swayZ = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    burst.gravity = ((func_150ADA68() * 60.0f) + 21.0f) * 0.001000000047f;
    spawned = func_151303BC(&burst, 2, sizeof(payload));
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0xA8, &payload, sizeof(payload));
    }
}

/* LoopSoundFx update: keeps sound 0x355 playing while camera_global_Cameras->SpecialFlags bit 0 is set. */
s32 func_151AB6B8(LoopSoundFx *fx) {
    u8 alive;
    LoopSoundFxPayload *sound;

    alive = TRUE;
    sound = &fx->payload;
    if ((fx->payload.soundHandle != 0) && !(camera_global_Cameras->SpecialFlags & 1)) {
        func_100111C8(sound->soundHandle);
        sound->soundHandle = 0;
    } else if ((sound->soundHandle == 0) && (camera_global_Cameras->SpecialFlags & 1)) {
        sound->soundHandle = func_10010F30(0x355, 32000, 64, 0, 0);
    }
    if (sound->keepAlive == 0) {
        alive = FALSE;
    }
    sound->keepAlive = 0;
    return alive;
}

/* LoopSoundFx destructor (variant 1): stops the sound and detaches from the owner task. */
void func_151AB788(LoopSoundFx *fx) {
    LoopSoundFxPayload *sound = &fx->payload;
    ActorTaskFx *ownerFx = &sound->owner->fx;

    if (fx->payload.soundHandle != 0) {
        func_100111C8(sound->soundHandle);
    }
    ownerFx->sound = NULL;
    func_151346EC(&fx->header);
}

/* LoopSoundFx destructor (variant 2): stops the sound and detaches from the owner task. */
void func_151AB7D8(LoopSoundFx *fx) {
    LoopSoundFxPayload *sound = &fx->payload;
    ActorTaskFx *ownerFx = &sound->owner->fx;

    if (fx->payload.soundHandle != 0) {
        func_100111C8(sound->soundHandle);
    }
    ownerFx->sound = NULL;
    func_1513470C(&fx->header);
}

/* func_15141DA4(actor, 0, 4). */
void func_151AB828(ActorTask *task) {
    func_15141DA4(task->watcher.character, 0, 4);
}

/* For actors of kind 0-4 or 150: spawns a func_15190770 effect on the actor. */
void func_151AB854(ActorTask *task) {
    Actor *actor;
    ActorLink desc;

    actor = task->watcher.character;
    if ((actor->character_type == 0) || (actor->character_type == 1) || (actor->character_type == 2) || (actor->character_type == 3) || (actor->character_type == 4) ||
        (actor->character_type == 150)) {
        desc.actor = actor;
        desc.actorId = actor->unique_id;
        desc.timer = 300;
        desc.flags = 0;
        desc.unk9 = 0;
        if (actor->character_type == 150) {
            desc.kind = 3;
        } else {
            desc.kind = 0;
        }
        if (actor->playernumber != 0xFF) {
            actor->mv->idle.delay = 500;
        }
        func_15190770(&desc, 0, task->watcher.header.owner, task->watcher.header.kind);
    }
}

/* Empty callback. */
void func_151AB920(Actor *actor, s32 arg1) {
}

/* Payload at +0x28 of the spark emitter task spawned by func_151AB930. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 actorSerial;  // actor->unk84 at spawn
    /* 0x0C */ f32 accum;        // sparks owed; one is emitted per whole unit
    /* 0x10 */ f32 rate;
    /* 0x14 */ f32 rateRandom;
    /* 0x18 */ u8 color;
} EmitterPayload;

struct EmitterTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterPayload payload;
};

/* Spawns a spark emitter task that follows `actor`. */
void func_151AB930(Actor *actor) {
    TimedEffect *task;
    EmitterPayload payload;

    payload.actor = actor;
    payload.actorId = actor->unique_id;
    payload.actorSerial = actor->animseq.uh;
    payload.accum = 0.0f;
    payload.rate = 0.400000006f;
    payload.rateRandom = 3.0f;
    payload.color = func_151D8E20();
    task = func_151491F4(0x3C, -1, 0xE, 1, 9, sizeof(payload), 0xFF, 0);
    if (task != NULL) {
        memcpy(&((EmitterTask *)task)->payload, &payload, sizeof(payload));
    }
}

/* Spark emitter update: emits sparks around the actor, or ends the task once the actor is gone. */
void func_151AB9C8(EmitterTask *task) {
    EmitterPayload *emitter;
    u8 dead;
    CollisionHit floor;
    Vec3f pos;
    Vec3f vel;
    s16 yaw;
    s16 pitch;
    f32 sinP;
    f32 cosP;
    f32 sinY;
    f32 cosY;
    f32 speed;
    s32 randA;
    s32 randB;

    dead = FALSE;
    emitter = &task->payload;
    if (task->payload.actor->type == 0) {
        dead = TRUE;
    } else if (emitter->actor->unique_id != emitter->actorId) {
        dead = TRUE;
    } else if (emitter->actor->animseq.uh != emitter->actorSerial) {
        dead = TRUE;
    }
    if (!dead) {
        emitter->accum += (emitter->rate + func_150ADA68() * emitter->rateRandom) * D_800BE9A4;
        if (emitter->accum > 1.0f) {
            func_1504715C(&floor, emitter->actor);
            do {
                yaw = func_150ADA20() & 0xFF;
                pitch = (func_150ADA20() % 46U) - 0x3F;
                sinP = func_151423D8(pitch);
                cosP = func_151423D8(pitch - 0x40);
                sinY = func_151423D8(yaw);
                cosY = func_151423D8(yaw - 0x40);
                speed = (func_150ADA68() * 713.0f + 500.0f) * 0.009999999776f;
                pos.x = emitter->actor->xpos + 30.0f * sinP * cosY;
                pos.y = emitter->actor->ypos - 75.0f * cosP;
                pos.z = emitter->actor->zpos + 30.0f * sinP * sinY;
                vel.x = speed * sinP * cosY;
                vel.y = -speed * cosP;
                vel.z = speed * sinP * sinY;
                {
                    f32 randT = func_150ADA68();

                    randA = func_150ADA20();
                    randB = func_150ADA20();
                    func_151D9014(&pos, &vel, emitter->color, randT * 0.5f + -1.0f, (u32)randA % 26U + 32,
                                  (u32)randB % 101U + 100, func_150ADA68() * 34.0f + 52.0f, 1, 1.0f, 1.0f, 1, &floor, 1,
                                  0, task->base.header.owner, task->base.header.kind);
                }
                emitter->accum -= 1.0f;
            } while (emitter->accum > 1.0f);
        }
    }
    if (dead) {
        task->base.header.state = -1;
    }
}

/*
 * Message handler for a task whose payload (at +0x28, as in EmitterTask) names an actor:
 * message 0 ends the task when it concerns that actor; message 0x2D swaps actor A and B.
 */
void func_151ABD54(EmitterTask *task, ActorMsg *msg, u8 msgId) {
    EmitterPayload *payload = &task->payload;

    if (msgId == 0) {
        if ((msg->ref.actor == payload->actor) || (msg->ref.uniqueId == payload->actorId)) {
            func_1516972C(&task->base.header);
        }
    } else if (msgId == 0x2D) {
        if (payload->actor == msg->swap.actorA) {
            payload->actor = msg->swap.actorB;
            payload->actorId = msg->swap.idB;
        } else if (payload->actor == msg->swap.actorB) {
            payload->actor = msg->swap.actorA;
            payload->actorId = msg->swap.idA;
        }
    }
}

typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ s8 actorId;
} Msg151ABE00;

/* Broadcasts message 0x20 ({actor, unique_id}) via func_1516944C. */
void func_151ABE00(Actor *actor) {
    Msg151ABE00 msg;

    msg.actor = actor;
    msg.actorId = actor->unique_id;

    func_1516944C(0x20, (s32)&msg, 12);
}
