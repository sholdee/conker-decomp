#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15006010(Camera *actor) {
    if (actor->player->unk102 != 0) {
        if (func_15123934(actor, 8, 0, 0, 0) != 0) {
            actor->flags |= 0x100000;
            actor->flags &= ~4;
            actor->preset = 1;
            actor->presetMask = 3;
            func_15124B18(actor);
        }
        actor->height = 143.0f;
        actor->unk34C = 143.0f;
        actor->targetDistance = 150.0f;
        actor->liftTarget = -38.0f;
        return;
    }

    if (actor->zone == 59) {
        if (func_15123934(actor, 8, 0, 0, 0) != 0) {
            actor->flags |= 0x100000;
            actor->flags &= ~4;
            actor->height = 155.0f;
            actor->unk34C = 155.0f;
            actor->liftTarget = 30.0f;
            actor->lift = 30.0f;
            actor->targetDistance = 178.0f;
        }
    } else {
        func_151239CC(actor, 0);
    }
}

void func_15006140(Camera *actor) {
    actor->presetMask = 5;
    actor->preset = 2;
    func_15124B18(actor);
}
