#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_151EF610(void) {
    static u32 D_80091970 = 174823885;
    u32 x;

    while (1) {
        x = (D_80091970 << 2) + 2;
        x *= (x + 1);
        x = x >> 2;
        D_80091970 = x;
        return x;
    }
}
