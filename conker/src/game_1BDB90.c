#include <ultra64.h>
#include "functions.h"

/* An object plus its unique id, the payload of the 0x2A / 0x49 messages. */
typedef struct ObjectRef {
    /* 0x0 */ struct127 *obj;
    /* 0x4 */ u8 uniqueId;
} ObjectRef;

/* Payload of message 0x2D: two objects whose references are exchanged. */
typedef struct ObjectPair {
    /* 0x0 */ struct127 *a;
    /* 0x4 */ struct127 *b;
} ObjectPair;

struct TimedSoundTrigger {
    /* 0x0 */ s32 interval;
    /* 0x4 */ s32 elapsed;
    /* 0x8 */ struct127 *target;
};

struct TimedSoundObject {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ struct TimedSoundTrigger trigger;
};

extern void func_151D343C(ObjectRef *, s32);
extern void func_1518F45C(ObjectRef *, u8);

/* Message 0x2D: swap the trigger's target between the two objects in the message. */
void func_151906E0(struct TimedSoundObject *self, ObjectPair *pair, u8 msg) {
    struct TimedSoundTrigger *trigger = &self->trigger;
    struct127 *a;

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
void func_1519072C(struct127 *self) {
    ObjectRef ref;
    ObjectRef *refPtr[1]; // matching: an array keeps &ref in a stack slot across the first call

    ref.obj = self;
    ref.uniqueId = self->unique_id;
    refPtr[0] = &ref;
    func_151D343C(refPtr[0], 0x2A);
    func_1518F45C(refPtr[0], 0x49);
}
