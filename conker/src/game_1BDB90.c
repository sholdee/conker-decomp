#include <ultra64.h>
#include "functions.h"

/* Payload of message 0x2D: two objects whose references are exchanged. */
struct ObjectPair {
    /* 0x0 */ Actor *a;
    /* 0x4 */ Actor *b;
};

/* Message 0x2D: swap the trigger's target between the two objects in the message. */
void effect_periodicTriggerMessage(PeriodicObject *self, ObjectPair *pair, u8 msg) {
    PeriodicTrigger *trigger = &self->trigger;
    Actor *a;

    if (msg == 0x2D) {
        a = pair->a;
        if (trigger->target == a) {
            trigger->target = pair->b;
        } else if (trigger->target == pair->b) {
            trigger->target = a;
        }
    }
}

/* Sends messages 0x2A and 0x49 carrying a reference to this object. */
void func_1519072C(Actor *self) {
    ActorRef ref;

    ref.actor = self;
    ref.uniqueId = self->unique_id;
    effect_sendEventToFollowerGroups((s32)&ref, 0x2A);
    func_1518F45C((s32)&ref, 0x49);
}
