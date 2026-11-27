#include <ultra64.h>
#include <math.h>
#include "conker/math.h"


f32 expf(f32 arg0) {
    f32 ax;
    f32 r;
    f32 sum;
    f32 term;
    f32 prev;
    s32 n;
    s32 i;

    ax = fabsf(arg0);
    if (ax < 1.1920929e-7f) {
        return 1.0f;
    }
    n = ax / 0.6931471825f;
    if (n > 0x400) {
        if (arg0 >= 0.0f) {
            return MAXFLOAT;
        }
        return 0.0f;
    }
    r = ax - n * 0.6931471825f;
    sum = 1.0f;
    term = 1.0f;
    i = 1;
    do {
        prev = sum;
        term *= r / i++;
        sum += term;
    } while (sum != prev);
    while (n != 0) {
        sum += sum;
        n--;
    }
    if (arg0 >= 0.0f) {
        return sum;
    }
    return 1.0f / sum;
}
