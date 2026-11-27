#include <ultra64.h>
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/trail.h"
#include "libc/string.h"

/* matching: no header declares particle_spawnSteamParticle (steam.c): modelbeam.c and blood.c were compiled with other views */
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

struct ActorTask;

/*
 * The three effects an ActorTask keeps alive. Each effect object carries a payload (copied in at spawn)
 * that points back at its owner task; the owner sets payload.keepAlive every frame, the effect's update
 * clears it and dies once it is no longer refreshed, and its destructor clears the owner's pointer.
 */

/* Payload at +0x80 of the effect spawned by effect_spawnTaskPulse: a value oscillating around `base`. */
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

/* Payload at +0x58 of the effect spawned by effect_spawnTaskLoopSound: holds a looping sound. */
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
void effect_watcherStepIntoShallowWater(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, &task->watcher.hit, 3, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Ground effect 3 under the actor (with sound 0x5208), then effect_stopActorDripEmitters. */
void effect_watcherStepIntoDeepWater(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, hit, 3, task->watcher.header.owner, task->watcher.header.kind);
            sound_playOnActor(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    effect_stopActorDripEmitters(actor);
}

/* Ground effect 1 under the actor, with sound 0x5208. */
void effect_watcherFallIntoShallowWater(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, hit, 1, task->watcher.header.owner, task->watcher.header.kind);
            sound_playOnActor(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}


/* Unless the actor's mv->ladder is set: ground effect 1 (sound 0x6978), effect_attachFallingDriftIfSolid, effect_stopActorDripEmitters. */
void effect_watcherFallIntoDeepWater(ActorTask *task) {
    Actor *actor = task->watcher.character;
    Vec3f pos;

    // matching: structs.h types unk95 as s8, but this read is unsigned (lbu)
    if ((actor->mv != NULL) && (actor->mv->ladder != 0)) {
        return;
    }
    pos.x = actor->xpos;
    if (-9500.0f < actor->waterHeight) {
        pos.y = actor->waterHeight + 100.0f;
    } else {
        pos.y = actor->ypos + 150.0f;
    }
    pos.z = actor->zpos;
    if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
        pos.y = task->watcher.hit.height;
        effect_spawnImpact(&pos, &task->watcher.hit, 1, task->watcher.header.owner, task->watcher.header.kind);
        sound_playOnActor(0, 8, 0x6978, 0, 0, actor);
    }
    effect_attachFallingDriftIfSolid(actor, &task->watcher.hit);
    effect_stopActorDripEmitters(actor);
}

/* Ground effect 2 under the actor, with sound 0x5208. */
void func_151A9DC0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, hit, 2, task->watcher.header.owner, task->watcher.header.kind);
            sound_playOnActor(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}

/* Ground effect 2 (sound 0x6978), then effect_attachFallingDriftIfSolid and effect_stopActorDripEmitters. */
void func_151A9EC0(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, hit, 2, task->watcher.header.owner, task->watcher.header.kind);
            sound_playOnActor(0, 8, 0x6978, 0, 0, actor);
        }
    }
    effect_attachFallingDriftIfSolid(actor, &task->watcher.hit);
    effect_stopActorDripEmitters(actor);
}

/* Ground effect 4 under the actor. */
void effect_watcherLeaveShallowWater(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, &task->watcher.hit, 4, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Ground effect 4 under the actor. */
void effect_watcherLeaveDeepWater(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, &task->watcher.hit, 4, task->watcher.header.owner, task->watcher.header.kind);
        }
    }
}

/* Empty callback. */
void effect_watcherNoAction(s32 arg0) {
}

typedef struct {
    /* 0x00 */ Actor *character;
    /* 0x04 */ u8 characterId;
} Msg151AA17C;

/* Sends message 10 ({unk18, unk1C}) through effect_sendEventToEffectObjs and effect_sendEventToTimedEffects, then trail_startSurfaceTrails. */
void effect_watcherStartSurfaceTrails1(SurfaceTrailOwner *owner) {
    s32 word;
    Msg151AA17C msg;

    // matching: one int local carries the character and then &msg (a pointer local is rematerialized)
    word = (s32)owner->watcher.character;
    msg.character = (Actor *)word;
    msg.characterId = owner->watcher.characterId;
    word = (s32)&msg;
    effect_sendEventToEffectObjs(word, 10);
    effect_sendEventToTimedEffects(word, 10);
    trail_startSurfaceTrails(owner);
}

void effect_watcherReleaseSurfaceTrails1(void) {
    trail_releaseSurfaceTrails();
}

void effect_watcherReleaseSurfaceTrails2(void) {
    trail_releaseSurfaceTrails();
}

/* Same as effect_watcherStartSurfaceTrails1. */
void effect_watcherStartSurfaceTrails2(SurfaceTrailOwner *owner) {
    s32 word;
    Msg151AA17C msg;

    // matching: one int local carries the character and then &msg (a pointer local is rematerialized)
    word = (s32)owner->watcher.character;
    msg.character = (Actor *)word;
    msg.characterId = owner->watcher.characterId;
    word = (s32)&msg;
    effect_sendEventToEffectObjs(word, 10);
    effect_sendEventToTimedEffects(word, 10);
    trail_startSurfaceTrails(owner);
}

/* When the ground hit has flag bit 0 set, spawns a blood_spawnDrip effect on the actor. */
void effect_attachFallingDriftIfSolid(Actor *actor, CollisionHit *hit) {
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
        blood_spawnDrip(&desc, 0, 0xFF, 0);
    }
}

struct Task151AA30C {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ Actor *actor;
};

/* Spawns one falling particle at (x, y, z) when that point is not above the task actor's waterHeight. */
void effect_emitFallingDriftParticle(f32 x, f32 y, f32 z, f32 arg3, f32 scale, f32 arg5, Task151AA30C *task) {
    Vec3f pos;
    Vec3f vel;
    Actor *actor;
    CollisionHit *ground;

    actor = task->actor;
    if (actor->waterHeight < y) {
        return;
    }
    if (effect_getWatcherFloorHit(actor, &ground) == 0) {
        ground = NULL;
    }
    vel.x = 0.0f;
    vel.y = -((math_randomFloat() * 196.0f + 199.0f) * 0.001000000047f * scale);
    vel.z = 0.0f;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    effect_spawnDriftMotionParticle(&pos, &vel, (math_randomFloat() * 311.0f + 454.0f) * 0.001000000047f, 0.972683012f, ground,
                  math_randomFloat() * 101.0f + 101.0f, math_random() % 31U + 50, math_random() % 101U + 100,
                  1, task->header.owner, task->header.kind);
}

/* Spawns the pulse effect owned by `task`. */
PulseFx *effect_spawnTaskPulse(Actor *actor, ActorTask *task) {
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

    fx = blood_spawnStrip(&desc, sizeof(payload)); /* a BloodStrip with the payload after it */
    if (fx != NULL) {
        memcpy(&fx->payload, &payload, sizeof(payload));
    }
    return fx;
}

/* PulseFx callback: restarts the pulse and plays sound 10 or 11 (at random) at the effect. */
s32 effect_restartTaskPulse(PulseFx *fx) {
    fx->payload.phase = 0;
    if (math_random() & 1) {
        sound_playAt(10, 0x55F0, (math_rand() % 1200) - 600, 0, 0, fx->strip.posA.x, fx->strip.posA.y, fx->strip.posA.z, 500, 2500);
        return 1;
    }
    sound_playAt(11, 0x55F0, (math_rand() % 1200) - 600, 0, 0, fx->strip.posA.x, fx->strip.posA.y, fx->strip.posA.z, 500, 2500);
    return 1;
}

struct Obj151AA6D8 {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ f32 unk74;
};

/* Spawns a particle burst moving from `from` toward `to`, advanced by `t` frames. */
void effect_emitStripRiseParticle(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, Obj151AA6D8 *obj) {
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

    if (math_random() & 1) {
        burst.flags |= 0x40;
    }
    if (math_random() & 1) {
        burst.flags |= 0x80;
    }

    burst.timer = (math_random() % 51U) + 50;
    burst.alpha = (math_random() % 101U) + 100;
    payload.baseSize = burst.width = burst.height = (math_randomFloat() * 39.0f) + 31.0f;
    payload.widthPhaseStep = (math_random() % 5U) + 4;
    payload.heightPhaseStep = (math_random() % 5U) + 4;
    payload.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.swayPhaseStepX = (math_random() % 5U) + 4;
    payload.swayPhaseStepZ = (math_random() % 5U) + 4;
    payload.swayX = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    payload.swayZ = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    burst.gravity = ((math_randomFloat() * 70.0f) + 73.0f) * 0.001000000047f;

    spawned = particle_newSpriteParticle(&burst, 2, sizeof(payload));
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0xA8, &payload, sizeof(payload));
    }
}

/* PulseFx update: returns the current oscillation value and advances the phase. */
f32 effect_stepTaskPulse(PulseFx *fx) {
    PulseFxPayload *pulse;
    f32 value;

    pulse = &fx->payload;
    value = (math_cosBinAngleMasked((pulse->phase >> 16) - 0x40) * pulse->amplitude) + pulse->base;
    pulse->phase += pulse->phaseStep * gFrameTicks;
    return value;
}

/* Refreshes the task's pulse and loop-sound effects, spawning whichever is missing. */
void effect_refreshTaskEffects(ActorTask *task) {
    Actor *actor;
    ActorTaskFx *fx;

    actor = task->watcher.character;
    fx = &task->fx;
    if (fx->pulse != NULL) {
        PulseFxPayload *pulse;

        pulse = &fx->pulse->payload;
        pulse->keepAlive = 1;
    } else {
        fx->pulse = effect_spawnTaskPulse(actor, task);
    }
    if (fx->sound != NULL) {
        LoopSoundFxPayload *sound;

        sound = &fx->sound->payload;
        sound->keepAlive = 1;
    } else {
        fx->sound = effect_spawnTaskLoopSound(actor, task);
    }
}

/* PulseFx destructor (variant 1): detaches from the owner task. */
void effect_destroyTaskPulse(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    blood_destroyStripDefault(&fx->strip.header);
}

/* PulseFx destructor (variant 2): detaches from the owner task. */
void effect_freeTaskPulse(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    blood_freeStripDefault(&fx->strip.header);
}

/* PulseFx keep-alive poll: TRUE if the owner refreshed it since the last call. */
s32 effect_isTaskPulseAlive(PulseFx *fx) {
    s32 alive = TRUE;

    if (fx->payload.keepAlive == 0) {
        alive = FALSE;
    }
    fx->payload.keepAlive = 0;
    return alive;
}

/* The points of the actor's matrices that effect_spawnBoneGroundMark can drop effects from. */
static const u8 gGroundMarkBones[] = { 5, 10, 19, 23 };

/* Finds point gGroundMarkBones[index] of the actor's ob_matrix_ptr (math_getBoneTranslation) and spawns two effects on the ground below it. */
void effect_spawnBoneGroundMark(Actor *actor, u8 index) {
    Vec3f pos;
    CollisionHit floor;
    f32 y;
    Vec3f probe;
    u8 color;
    Vec3f hit;

    if ((actor != NULL) && (actor->ob_matrix_ptr != NULL)) {
        color = particle_getContextEffectKind();
        math_getBoneTranslation(actor->ob_matrix_ptr, gGroundMarkBones[index], &pos, actor);
        collision_initHitFromObject(&floor, actor);
        y = pos.y + 50.0f;
        probe.x = pos.x;
        probe.y = y;
        probe.z = pos.z;
        if (collision_probeFloorSurfABSet0(&probe, 0, y - 100.0f, &floor) != 0) {
            hit.x = pos.x;
            hit.y = floor.height;
            hit.z = pos.z;
            particle_spawnKindMark(color, 40.0f, 150, &floor.tri, &hit, 0xFF, 1);
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
                particle_spawnSplashSprayAtAngles(&c.angles, &c.spray, &floor, 0xFF, 1);
            }
        }
    }
}

/* Trail update handler (gMotionTrailUpdateProcs): caps the trail's alpha at its remaining life * 16 (at most 255). */
s32 effect_fadeTrailByLife(MotionTrail *trail) {
    TrailParams *params;
    s32 limit;

    params = trail->params;
    limit = trail->head.tmpl.life << 4;
    if (limit >= 0x100) {
        limit = 0xFF;
    }
    if (limit < params->motion.alpha) {
        params->motion.alpha = limit;
    }
    return 1;
}


/* Keeps the task's flicker effect alive, or spawns it on the ground under the actor. */
void effect_keepTaskFlicker(ActorTask *task) {
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
    if (-9500.0f < actor->waterHeight) {
        pos.y = actor->waterHeight + 100.0f;
    } else {
        pos.y = actor->ypos + 150.0f;
    }
    pos.z = actor->zpos;
    if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, &task->watcher.hit) != 0) {
        color = particle_getContextEffectKind();
        pos.y = task->watcher.hit.height;
        size = (math_randomFloat() * 30.0f) + 50.0f;
        payload.owner = &task->watcher;
        payload.keepAlive = 1;
        payload.minDelay = 10;
        payload.delayRange = 10;
        payload.minTarget = 26214.40039f;
        payload.targetRange = 32768.0f;
        payload.target = 0.0f;
        payload.timer = 0;
        payload.easing = 0.2310000062f;
        part.frameSpeed = payload.targetRange * 0.5f + payload.minTarget;
        part.texture = 0x38;
        part.flags = 0x60300;
        part.lifetime = 300;
        part.unk10 = 150;
        part.frame = 0;
        part.unk11 = 0xFF;
        part.rgba[0] = gKindColors[color].r;
        part.rgba[1] = gKindColors[color].g;
        part.rgba[2] = gKindColors[color].b;
        part.rgba[3] = 0xFF;
        part.renderModes = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        fx->flicker = (FlickerSprite *)particle_spawnSurfaceQuadSpriteNoModel(&part, 7, 1, &task->watcher.hit.tri, pos.x, pos.y, pos.z,
                                                 size, size, math_random() & 0xFF,
                                                 (math_random() & 1) + ((math_random() & 1) * 2), 0x20, 0xFF, 1);
        if (fx->flicker != NULL) {
            memcpy(&fx->flicker->flicker, &payload, sizeof(payload));
        }
    }
}


/* FlickerSprite update: eases the sprite's animation speed toward a target re-rolled after a random delay. */
s32 effect_updateTaskFlicker(FlickerSprite *fx) {
    s8 alive = TRUE;
    SpriteFlicker *flicker = &fx->flicker;

    if (fx->flicker.keepAlive == 0) {
        alive = FALSE;
    }
    flicker->keepAlive = 0;
    flicker->timer -= gFrameTicks;
    if (flicker->timer < 0) {
        flicker->timer = (math_random() % (flicker->delayRange + 1)) + flicker->minDelay;
        flicker->target = math_randomFloat() * flicker->targetRange + flicker->minTarget;
    }
    fx->quad.def.frameSpeed += (s32)((flicker->target - fx->quad.def.frameSpeed) * flicker->easing);
    return alive;
}

/* FlickerSprite destructor: detaches from the owner task. */
s32 effect_detachTaskFlicker(FlickerSprite *fx) {
    ActorTaskFx *ownerFx = &((ActorTask *)fx->flicker.owner)->fx;

    ownerFx->flicker = NULL;
    fx->quad.def.flags |= 2;
    fx->flicker.owner = NULL;
    particle_setQuadSpriteFuncs(&fx->quad, 0, 0);
    return 0;
}

/* Ground effect 3 (sound 0x5208), then effect_attachFallingDriftIfSolid. */
void effect_watcherSubmerge(ActorTask *task) {
    Actor *actor = task->watcher.character;

    {
        Vec3f pos;
        CollisionHit *hit;

        pos.x = actor->xpos;
        if (-9500.0f < actor->waterHeight) {
            pos.y = actor->waterHeight + 100.0f;
        } else {
            pos.y = actor->ypos + 150.0f;
        }
        pos.z = actor->zpos;
        hit = &task->watcher.hit;
        if (collision_probeFloorSurfBCached(&pos, 0, pos.y - 300.0f, hit) != 0) {
            pos.y = task->watcher.hit.height;
            effect_spawnImpact(&pos, hit, 3, task->watcher.header.owner, task->watcher.header.kind);
            sound_playOnActor(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    effect_attachFallingDriftIfSolid(actor, &task->watcher.hit);
}

/* Spawns the looping-sound effect owned by `task`. */
LoopSoundFx *effect_spawnTaskLoopSound(Actor *actor, ActorTask *task) {
    BloodDripDesc desc;
    LoopSoundFxPayload payload;
    LoopSoundFx *fx;

    payload.owner = task;
    payload.soundHandle = 0;
    payload.keepAlive = 1;
    payload.unk8 = actor->waterHeight;
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

    fx = (LoopSoundFx *)blood_spawnDrip(&desc, sizeof(payload), 0xFF, 0); /* a BloodDrip with the payload at 0x58 */
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
void effect_emitDriftRiseParticle(f32 x, f32 y, f32 z, f32 dx, f32 arg4, f32 dz, Obj151AB3A4 *obj) {
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
    if (math_random() & 1) {
        burst.flags |= 0x40;
    }
    if (math_random() & 1) {
        burst.flags |= 0x80;
    }
    burst.timer = (math_random() % 51U) + 106;
    burst.alpha = (math_random() % 101U) + 100;
    payload.baseSize = burst.width = burst.height = (math_randomFloat() * 62.0f) + 41.0f;
    payload.widthPhaseStep = (math_random() % 5U) + 4;
    payload.heightPhaseStep = (math_random() % 5U) + 4;
    payload.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * payload.baseSize;
    payload.swayPhaseStepX = (math_random() % 5U) + 4;
    payload.swayPhaseStepZ = (math_random() % 5U) + 4;
    payload.swayX = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    payload.swayZ = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * payload.baseSize;
    burst.gravity = ((math_randomFloat() * 60.0f) + 21.0f) * 0.001000000047f;
    spawned = particle_newSpriteParticle(&burst, 2, sizeof(payload));
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0xA8, &payload, sizeof(payload));
    }
}

/* LoopSoundFx update: keeps sound 0x355 playing while camera_global_Cameras->SpecialFlags bit 0 is set. */
s32 effect_updateLoopSound(LoopSoundFx *fx) {
    u8 alive;
    LoopSoundFxPayload *sound;

    alive = TRUE;
    sound = &fx->payload;
    if ((fx->payload.soundHandle != 0) && !(camera_global_Cameras->SpecialFlags & 1)) {
        sound_stop(sound->soundHandle);
        sound->soundHandle = 0;
    } else if ((sound->soundHandle == 0) && (camera_global_Cameras->SpecialFlags & 1)) {
        sound->soundHandle = sound_play(0x355, 32000, 64, 0, 0);
    }
    if (sound->keepAlive == 0) {
        alive = FALSE;
    }
    sound->keepAlive = 0;
    return alive;
}

/* LoopSoundFx destructor (variant 1): stops the sound and detaches from the owner task. */
void effect_destroyLoopSound(LoopSoundFx *fx) {
    LoopSoundFxPayload *sound = &fx->payload;
    ActorTaskFx *ownerFx = &sound->owner->fx;

    if (fx->payload.soundHandle != 0) {
        sound_stop(sound->soundHandle);
    }
    ownerFx->sound = NULL;
    blood_destroyDripDefault(&fx->header);
}

/* LoopSoundFx destructor (variant 2): stops the sound and detaches from the owner task. */
void effect_freeLoopSound(LoopSoundFx *fx) {
    LoopSoundFxPayload *sound = &fx->payload;
    ActorTaskFx *ownerFx = &sound->owner->fx;

    if (fx->payload.soundHandle != 0) {
        sound_stop(sound->soundHandle);
    }
    ownerFx->sound = NULL;
    blood_freeDripDefault(&fx->header);
}

/* effect_tryAttachActorEffect(actor, 0, 4). */
void effect_watcherStartFootSplashes(ActorTask *task) {
    effect_tryAttachActorEffect(task->watcher.character, 0, 4);
}

/* For actors of kind 0-4 or 150: spawns a particle_newDripEmitter effect on the actor. */
void effect_startPlayerDripEmitter(ActorTask *task) {
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
        particle_newDripEmitter(&desc, 0, task->watcher.header.owner, task->watcher.header.kind);
    }
}

/* Empty callback. */
void func_151AB920(Actor *actor, s32 arg1) {
}

/* Payload at +0x28 of the spark emitter task spawned by effect_spawnSparkEmitterTask. */
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
void effect_spawnSparkEmitterTask(Actor *actor) {
    TimedEffect *task;
    EmitterPayload payload;

    payload.actor = actor;
    payload.actorId = actor->unique_id;
    payload.actorSerial = actor->animseq.uh;
    payload.accum = 0.0f;
    payload.rate = 0.400000006f;
    payload.rateRandom = 3.0f;
    payload.color = particle_getContextEffectKind();
    task = effect_newTimedEffectNoDraw(0x3C, -1, 0xE, 1, 9, sizeof(payload), 0xFF, 0);
    if (task != NULL) {
        memcpy(&((EmitterTask *)task)->payload, &payload, sizeof(payload));
    }
}

/* Spark emitter update: emits sparks around the actor, or ends the task once the actor is gone. */
void effect_updateSparkEmitterTask(EmitterTask *task) {
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
        emitter->accum += (emitter->rate + math_randomFloat() * emitter->rateRandom) * gFrameDelta;
        if (emitter->accum > 1.0f) {
            collision_initHitFromObject(&floor, emitter->actor);
            do {
                yaw = math_random() & 0xFF;
                pitch = (math_random() % 46U) - 0x3F;
                sinP = math_cosBinAngleMasked(pitch);
                cosP = math_cosBinAngleMasked(pitch - 0x40);
                sinY = math_cosBinAngleMasked(yaw);
                cosY = math_cosBinAngleMasked(yaw - 0x40);
                speed = (math_randomFloat() * 713.0f + 500.0f) * 0.009999999776f;
                pos.x = emitter->actor->xpos + 30.0f * sinP * cosY;
                pos.y = emitter->actor->ypos - 75.0f * cosP;
                pos.z = emitter->actor->zpos + 30.0f * sinP * sinY;
                vel.x = speed * sinP * cosY;
                vel.y = -speed * cosP;
                vel.z = speed * sinP * sinY;
                {
                    f32 randT = math_randomFloat();

                    randA = math_random();
                    randB = math_random();
                    particle_spawnSteamParticle(&pos, &vel, emitter->color, randT * 0.5f + -1.0f, (u32)randA % 26U + 32,
                                  (u32)randB % 101U + 100, math_randomFloat() * 34.0f + 52.0f, 1, 1.0f, 1.0f, 1, &floor, 1,
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
void effect_emitterTaskMessage(EmitterTask *task, ActorMsg *msg, u8 msgId) {
    EmitterPayload *payload = &task->payload;

    if (msgId == 0) {
        if ((msg->ref.actor == payload->actor) || (msg->ref.uniqueId == payload->actorId)) {
            effect_destroyObject(&task->base.header);
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

/* Broadcasts message 0x20 ({actor, unique_id}) via effect_sendEventToType. */
void effect_stopActorDripEmitters(Actor *actor) {
    Msg151ABE00 msg;

    msg.actor = actor;
    msg.actorId = actor->unique_id;

    effect_sendEventToType(0x20, (s32)&msg, 12);
}
