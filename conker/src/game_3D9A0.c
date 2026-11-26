#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// ???
void func_150104F0(void) {
    WorldObject *temp_v0;

    D_800D9950[0] = (D_800D9950[1] = (D_800D9950[2] = 0));
    temp_v0 = func_151149AC(0xF6);
    temp_v0->touch.f.a = 2.0f;
    D_80088980 = 0;
}

void func_15010538(Actor *arg0) {
    BoltSource tmp;
    TimedEffect *effect;

    func_15161E24(arg0, 2, 2, 300, 30, 100, 200, 255, 255, 1);
// FAKEMATCH but works...
dummy_label_927029:
    tmp.obj = arg0;
    tmp.id = arg0->unique_id;
    tmp.flashTimer = 0;
    tmp.boltTimer = 0;
    tmp.timer2 = 0;

    effect = func_15149130(300, -1, 80, -1, 0, 61, 12, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}

void func_15010600(void) {
    s32 i;

    D_800D9921 = 0;
    D_800D9920 = 0;
    D_800D9928[0] = 0;
    D_800D9938[0] = 0;
    D_800D9929 = 0;
    for (D_800D9939 = 0, i = 0;;) {
        D_800D992A[i + 1] = 0;
        D_800D993A[i + 1] = 0;
        D_800D992A[i + 2] = 0;
        D_800D993A[i + 2] = 0;
        D_800D992A[i + 3] = 0;
        D_800D993A[i + 3] = 0;
        i += 4;
        D_800D992A[i - 4] = 0;
        D_800D993A[i - 4] = 0;
        if (&D_800D9946[0] != &D_800D993A[i]) {
            continue;
        }
        break;
    }
}
// NON-MATCHING: addresses are wrong :(
// void func_15010600(void) {
//     s32 i;
//
//     for (i = 0; i < 11; i++) {
//         D_800D9930[i] = D_800D9920[i] = 0;
//     }
// }
