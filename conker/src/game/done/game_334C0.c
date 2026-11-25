#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15006010(struct108 *actor) {
    if (actor->unk3D0->unk102 != 0) {
        if (func_15123934(actor, 8, 0, 0, 0) != 0) {
            actor->unk84 |= 0x100000;
            actor->unk84 &= ~4;
            actor->unk1B4 = 1;
            actor->unk1E0 = 3;
            func_15124B18(actor);
        }
        actor->unk348 = 143.0f;
        actor->unk34C = 143.0f;
        actor->unk374 = 150.0f;
        actor->unk190 = -38.0f;
        return;
    }

    if (actor->unk23E == 59) {
        if (func_15123934(actor, 8, 0, 0, 0) != 0) {
            actor->unk84 |= 0x100000;
            actor->unk84 &= ~4;
            actor->unk348 = 155.0f;
            actor->unk34C = 155.0f;
            actor->unk190 = 30.0f;
            actor->unk18C = 30.0f;
            actor->unk374 = 178.0f;
        }
    } else {
        func_151239CC(actor, 0);
    }
}

void func_15006140(struct108 *actor) {
    actor->unk1E0 = 5;
    actor->unk1B4 = 2;
    func_15124B18(actor);
}
