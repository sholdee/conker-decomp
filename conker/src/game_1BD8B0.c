#include <ultra64.h>
#include "functions.h"

/* An effect bound to an actor. */
struct BoundEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ ActorRef ref;
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

/* Detaches the light from the AnchorEmitter that owns it. */
void func_15190454(LightEffect *obj) {
    AnchorEmitter *emitter = obj->data.actorJitter.user;

    emitter->params.child = NULL;
}

void func_15190464(LightEffect *obj) {
    func_15190454(obj);
    func_151617C4(obj);
}
