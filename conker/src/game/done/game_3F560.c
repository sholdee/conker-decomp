#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "effects.h"


void func_150120B0(s32 arg0) {
    TimedEffect *effect;
    struct184 tmp;

    tmp.unk0 = 50.0f;
    tmp.unk4 = 50.0f;
    tmp.unk8 = 50.0f;
    tmp.unkC = 50.0f;
    tmp.unk10 = 50.0f;
    tmp.unk14 = 50.0f;
    tmp.unk18 = 0.0f;
    tmp.unk1C = D_80096540;

    effect = func_15149130(300, -1, 30, -1, 0, 65, 32, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, 32);
    }
}

/* Attaches a drip emitter (func_15190770) to the actor, except in level 24. */
void func_1501214C(Actor *arg0) {
    ActorLink tmp;

    if (D_800BE9F0 != 24) {
        tmp.actor = arg0;
        tmp.actorId = arg0->unique_id;
        tmp.timer = 300;
        tmp.flags = 0;
        tmp.unk9 = 0;
        tmp.kind = 2;
        func_15190770(&tmp, 0, 255, 1);
    }
}
