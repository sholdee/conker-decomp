#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void view_buildOrthoMatrices(void) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f24;
    f32 one;
    f32 half;
    s32 i;

    i = 0;
    if (gScreenLayout >= 0) {
        temp_f24 = 20000.0f;
        one = 1.0f;
        half = 0.5f;
        do {
            func_151EF954(
                &gViewOrthoMtxs[i],
                -(temp_f0 = gViews[i].width * half),
                temp_f0,
                -(temp_f2 = gViews[i].height * half),
                temp_f2,
                one,
                temp_f24,
                one);
            i = (i + 1) & 0xFF;
        } while (gScreenLayout >= i);
    }
}

void func_15012470(void) {
    gGroundDecalQueue = effect_newQueue(4, 300, 0);
}
