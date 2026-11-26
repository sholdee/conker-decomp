#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Data of an object that emits particles around an actor while that actor has bones. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId; // the actor's unique_id when the emitter was attached
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnCredit; // one particle is emitted per whole unit
} ActorEmitterState;

struct ActorEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ActorEmitterState state;
};


/* Emitter update: removes itself once its actor is gone or replaced, otherwise emits particles
   from a sphere around the actor's center at a random rate. */
void func_150CF140(ActorEmitter *emitter) {
    ActorEmitterState *state;
    Actor *actor;
    Vec3f center;
    f32 radius;
    CollisionHit ground;
    f32 extentA;
    f32 extentB;
    Vec3f pos;
    Vec3f vel;
    u8 yaw;

    actor = emitter->state.actor;
    state = &emitter->state;
    if ((actor->type == 0) || (actor->unique_id != state->actorId)) {
        emitter->base.header.state = -1;
        return;
    }
    if (actor->ob_matrix_ptr != NULL) {
        state->spawnCredit += (0.06f + (func_150ADA68() * 0.0704f)) * gFrameDelta;
        sprite_getActorBodyBounds(actor, &center, &extentA, &extentB);
        radius = (extentA + extentB) * 0.5f;
        collision_initHitFromObject(&ground, actor);
        while (state->spawnCredit > 1.0f) {
            yaw = func_150ADA20() & 0xFF;
            func_15143794(yaw, (func_150ADA20() % 76U) - 64, radius, &pos);
            pos.x += center.x;
            pos.y += center.y;
            pos.z += center.z;
            func_15143794(yaw, (func_150ADA20() % 54U) - 56, (func_150ADA68() * 1.4430001f) + 2.2680001f, &vel);
            func_151D9014(&pos, &vel, 5, (func_150ADA68() * 0.286f) + -0.78700006f, (func_150ADA20() % 21U) + 30,
                          (func_150ADA20() % 156U) + 100, (func_150ADA68() * 50.0f) + 48.0f, 1, 1.0f, 1.0f, 0,
                          &ground, 1, 0, emitter->base.header.owner, emitter->base.header.kind);
            state->spawnCredit -= 1.0f;
        }
    }
}

/* Emitter message handler: dies with its actor and follows it through a swap. */
void func_150CF484(ActorEmitter *emitter, ActorMsg *msg, u8 type) {
    ActorEmitterState *state = &emitter->state;
    Actor *actorA;

    if (type == 0) {
        actorA = msg->ref.actor; // matching: loaded before the state field, via a named local
        if ((actorA == state->actor) || (msg->ref.uniqueId == state->actorId)) {
            func_1516972C(&emitter->base.header);
        }
    } else if (type == 0x2D) {
        if (state->actor == msg->swap.actorA) {
            state->actor = msg->swap.actorB;
            state->actorId = msg->swap.idB;
        } else if (state->actor == msg->swap.actorB) {
            state->actor = msg->swap.actorA;
            state->actorId = msg->swap.idA;
        }
    }
}
