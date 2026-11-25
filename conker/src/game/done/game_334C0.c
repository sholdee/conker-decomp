#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15006010(Camera *actor) {
    if (actor->CharPtr->unk102 != 0) {
        if (func_15123934(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            actor->AvailableModes |= 0x100000;
            actor->AvailableModes &= ~4;
            actor->DistIndex = 1;
            actor->DistMask = 3;
            func_15124B18(actor);
        }
        actor->height = 143.0f;
        actor->unk34C = 143.0f;
        actor->IdealTripodMagnitude = 150.0f;
        actor->YOffsetTget = -38.0f;
        return;
    }

    if (actor->CharContext == 59) {
        if (func_15123934(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            actor->AvailableModes |= 0x100000;
            actor->AvailableModes &= ~4;
            actor->height = 155.0f;
            actor->unk34C = 155.0f;
            actor->YOffsetTget = 30.0f;
            actor->YOffset = 30.0f;
            actor->IdealTripodMagnitude = 178.0f;
        }
    } else {
        func_151239CC(actor, CAMERA_MODE_NORMAL);
    }
}

void func_15006140(Camera *actor) {
    actor->DistMask = 5;
    actor->DistIndex = 2;
    func_15124B18(actor);
}
