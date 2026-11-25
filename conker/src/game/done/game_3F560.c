#include <ultra64.h>

#include "functions.h"
#include "variables.h"


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

void func_1501214C(Actor *arg0) {
    struct185 tmp;

    if (D_800BE9F0 != 24) {
        tmp.unk0 = arg0;
        tmp.unk4 = arg0->unique_id;
        tmp.unk6 = 300;
        tmp.unk8 = 0;
        tmp.unk9 = 0;
        tmp.unkA = 2;
        func_15190770(&tmp, 0, 255, 1);
    }
}
