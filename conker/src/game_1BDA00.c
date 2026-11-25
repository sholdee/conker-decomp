#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct HandlerObject {
    /* 0x00 */ u8 pad0[0x8A];
    /* 0x8A */ u8 handlerIndex;
} HandlerObject;

typedef void (*HandlerFunc)(HandlerObject *, s32, u8);

extern void func_151D33FC(void *, s32);
extern HandlerFunc D_8008D684[];

/* Runs func_151D33FC for event 0x2A, then the object's handler from D_8008D684 if it has one. */
void func_15190550(HandlerObject *obj, s32 arg1, u8 event) {
    HandlerFunc handler;

    if (event == 0x2A) {
        func_151D33FC(obj, arg1);
    }
    handler = D_8008D684[obj->handlerIndex];
    if (handler != NULL) {
        handler(obj, arg1, event);
    }
}

typedef struct PeriodicTrigger {
    /* 0x0 */ s32 interval;
    /* 0x4 */ s32 elapsed;
    /* 0x8 */ Actor *target;
} PeriodicTrigger;

typedef struct PeriodicObject {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Actor *linked;
    /* 0x14 */ u8 pad14[0x7C];
    /* 0x90 */ PeriodicTrigger trigger;
} PeriodicObject;

extern s32 func_1505D024(void *, s32, u16, s32);

/*
 * Advances the trigger timer by D_800BE9E4 and calls func_1505D024 (with the target's index in
 * D_800CC2D0) once per elapsed interval; then sets the linked state's unk46 to 1000 if unk84 is clear.
 */
s32 func_151905BC(PeriodicObject *obj) {
    PeriodicTrigger *trigger;
    ActorCtl *state;

    obj->trigger.elapsed += D_800BE9E4;
    trigger = &obj->trigger;
    while (trigger->elapsed >= trigger->interval) {
        func_1505D024(obj->linked, 0x60021, 0, trigger->target - D_800CC2D0);
        trigger->elapsed -= trigger->interval;
    }
    state = obj->linked->unk31C;
    if (state != NULL && state->unk84 == 0) {
        state->unk46 = 1000;
    }
    return TRUE;
}

/* Clears unk1AC of the linked object's state, if it has one. */
void func_151906C4(PeriodicObject *obj) {
    ActorCtl *state = obj->linked->unk31C;

    if (state != NULL) {
        state->unk1AC = 0;
    }
}
