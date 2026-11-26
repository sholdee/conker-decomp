#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15012370(void) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f24;
    f32 one;
    f32 half;
    s32 i;

    i = 0;
    if (D_80082FA0 >= 0) {
        temp_f24 = 20000.0f;
        one = 1.0f;
        half = 0.5f;
        do {
            func_151EF954(
                &D_800DCC10[i],
                -(temp_f0 = D_800BE628[i].width * half),
                temp_f0,
                -(temp_f2 = D_800BE628[i].height * half),
                temp_f2,
                one,
                temp_f24,
                one);
            i = (i + 1) & 0xFF;
        } while (D_80082FA0 >= i);
    }
}

void func_15012470(void) {
    D_80088750 = func_1518AADC(4, 300, 0);
}
