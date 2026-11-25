#include <ultra64.h>

#include "functions.h"
#include "variables.h"

f32 func_1504A620(f32 x) {
    f32 sum;
    f32 prev;
    f32 t;
    f32 t2;
    f32 term;
    s32 i;

    term = 0.0f;
    if (x < term) {
        return 0;
    }
    if (x == term) {
        return 0;
    }
    sum = term;
    while (x >= 2.0f) {
        x *= 0.5f;
        sum += 0.6931471825f;
    }
    while (x < 1.0f) {
        x += x;
        sum -= 0.6931471825f;
    }
    t = (x - 1.0f) / (x + 1.0f);
    t2 = t * t;
    term = t + t;
    i = 1;
    do {
        x = sum;
        sum += term / (f32)i;
        term *= t2;
        i += 2;
    } while (sum != x);
    return sum;
}
