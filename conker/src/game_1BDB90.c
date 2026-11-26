#include <ultra64.h>
#include "functions.h"

/* Payload of message 0x2D: two objects whose references are exchanged. */
struct ObjectPair {
    /* 0x0 */ Actor *a;
    /* 0x4 */ Actor *b;
};

struct TimedSoundTrigger {
    /* 0x0 */ s32 interval;
    /* 0x4 */ s32 elapsed;
    /* 0x8 */ Actor *target;
};

struct TimedSoundObject {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ struct TimedSoundTrigger trigger;
};

/* Message 0x2D: swap the trigger's target between the two objects in the message. */
void func_151906E0(struct TimedSoundObject *self, ObjectPair *pair, u8 msg) {
    struct TimedSoundTrigger *trigger = &self->trigger;
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
    func_151D343C((s32)&ref, 0x2A);
    func_1518F45C((s32)&ref, 0x49);
}
