#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_1500ED80(void) {
    func_15177410(1, 0xD8, -0xDAC, 0, 0x4BB, 0.0f, 15000, 30.0f, 0x4D, 0xF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F);
    func_150124A0();
}

void func_1500EE18(Actor *arg0, s32 arg1, s32 arg2) {
    struct172 tmp;
    TimedEffect *effect;

    tmp.unk0 = arg0;
    tmp.unk4 = arg0->unique_id;

    effect = func_15149130(0x12C, -1, 0x45, -1, 0, 0, 8, 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, 8);
    }
}

void func_1500EE94(struct16 *arg0) {
    struct173 tmp;
    TimedEffect *effect;

    tmp.unk0 = arg0;
    tmp.unk4 = 0.0f;
    tmp.unk8 = arg0->unk14;
    effect = func_15149130(0x12C, -1, 0x46, -1, 0, 0x39, 0xC, 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, 12);
    }
}
