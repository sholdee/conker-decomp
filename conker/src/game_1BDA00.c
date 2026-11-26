#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * This file's .data: the message handlers by AnchorEmitterParams.unk5A, (obj, msg, event); optional. Handler 1 is
 * func_151906E0 (game_1BDB90.c), which takes the object and the message as its own views, so the table has no
 * prototype; the call keeps the prototype, as the ROM passes the event as a u8.
 */
static void (*D_8008D684[])() = { NULL, func_151906E0, NULL };

/* Runs func_151D33FC for event 0x2A, then the object's handler from D_8008D684 if it has one. */
void func_15190550(AnchorEmitter *obj, ActorMsg *msg, u8 event) {
    void (*handler)(AnchorEmitter *, ActorMsg *, u8);

    if (event == 0x2A) {
        func_151D33FC(&obj->group, msg);
    }
    handler = D_8008D684[obj->params.unk5A];
    if (handler != NULL) {
        handler(obj, msg, event);
    }
}

/*
 * Advances the trigger timer by D_800BE9E4 and calls actor_hitScripted (with the target's index in
 * movement_obj) once per elapsed interval; then sets the linked state's unk46 to 1000 if unk84 is clear.
 */
s32 func_151905BC(PeriodicObject *obj) {
    PeriodicTrigger *trigger;
    ActorCtl *state;

    obj->trigger.elapsed += D_800BE9E4;
    trigger = &obj->trigger;
    while (trigger->elapsed >= trigger->interval) {
        actor_hitScripted(obj->emitter.group.actor, 0x60021, 0, trigger->target - movement_obj);
        trigger->elapsed -= trigger->interval;
    }
    state = obj->emitter.group.actor->mv;
    if (state != NULL && state->bot == 0) {
        state->bitten = 1000;
    }
    return TRUE;
}

/* Clears unk1AC of the linked object's state, if it has one. */
void func_151906C4(PeriodicObject *obj) {
    ActorCtl *state = obj->emitter.group.actor->mv;

    if (state != NULL) {
        state->on_fire = 0;
    }
}
