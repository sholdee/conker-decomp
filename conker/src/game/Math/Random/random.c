#include <ultra64.h>
#include "conker/math.h"


s32 math_rand(void) {
    static u32 gRandSeed = 174823885;
    u32 x;

    while (1) {
        x = (gRandSeed << 2) + 2;
        x *= (x + 1);
        x = x >> 2;
        gRandSeed = x;
        return x;
    }
}
