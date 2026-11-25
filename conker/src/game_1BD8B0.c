#include <ultra64.h>
#include "functions.h"

/* An effect bound to an actor. */
struct BoundEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ ActorRef ref;
};

typedef struct {
    u8 pad0[0x84];
    s32 unk84;
} Game1BD8B0Inner;

struct Game1BD8B0Object {
    u8 pad0[0x40];
    Game1BD8B0Inner *inner;
};

/* Message handler: type 0 removes the effect when the message names its actor. */
void func_15190400(BoundEffect *effect, ActorRef *msg, u8 type) {
    Actor *actor;
    Actor *msgActor;
    u8 actorId;
    u8 msgActorId;

    if (type == 0) {
        actor = effect->ref.actor;
        msgActor = msg->actor;
        actorId = effect->ref.uniqueId;
        msgActorId = msg->uniqueId;
        if (actor == msgActor || actorId == msgActorId) {
            func_1516972C(&effect->header);
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
