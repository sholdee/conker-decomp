#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15008A10(void) {
    D_80088720 = func_15083E90(1);
    D_80088724.x = 0.0f;
    D_80088724.y = 0.0f;
    D_80088724.z = 0.0f;
    D_80088730 = func_15083E90(1);
}

/* Resets the screen effects of every view: the tint, the colour fades, the irises and the letterbox. */
void func_15008A60(void) {
    s32 i;

    D_800DDE08 = 0;
    D_800DDE40 = 0;
    for (i = 0; i < 4; i++) {
        D_800DDDAC[i] = 0;
        D_800DDE28[i] = 0;
        D_800DDDB0[i] = 0;
        D_800DDE1C[i] = 0;
        D_800DDE20[i] = 0;
        D_800DDDC8[i] = 0.0f;
        D_800DDDD8[i] = 0.0f;
        D_800DDE3C[i] = 0;
    }
    D_800DDE18 = NULL;
    D_800DDE38 = 0;
}
