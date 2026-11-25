#include <ultra64.h>
#include "functions.h"

/* Message payload: an actor reference (message type 0 asks effects bound to that actor to go away). */
typedef struct {
    Actor *actor;
    u8 actorId;
} ActorRefMsg;

/* An effect bound to an actor. */
typedef struct {
    u8 pad0[0x18];
    Actor *actor;
    u8 actorId;
} BoundEffect;

typedef struct {
    u8 pad0[0x84];
    s32 unk84;
} Game1BD8B0Inner;

typedef struct {
    u8 pad0[0x40];
    Game1BD8B0Inner *inner;
} Game1BD8B0Object;

void func_151617C4(Game1BD8B0Object *arg0);

/* Message handler: type 0 removes the effect when the message names its actor. */
void func_15190400(BoundEffect *effect, ActorRefMsg *msg, u8 type) {
    Actor *actor;
    Actor *msgActor;
    u8 actorId;
    u8 msgActorId;

    if (type == 0) {
        actor = effect->actor;
        msgActor = msg->actor;
        actorId = effect->actorId;
        msgActorId = msg->actorId;
        if (actor == msgActor || actorId == msgActorId) {
            func_1516972C((struct102 *)effect);
        }
    }
}

void func_15190454(Game1BD8B0Object *obj) {
    Game1BD8B0Inner *inner = obj->inner;

    inner->unk84 = 0;
}

void func_15190464(Game1BD8B0Object *obj) {
    func_15190454(obj);
    func_151617C4(obj);
}
