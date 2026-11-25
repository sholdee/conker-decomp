#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/**
 * Natural logarithm. Scales x into [1, 2) by powers of two (adding or subtracting ln 2 each step),
 * then sums the series ln(x) = 2 * (t + t^3/3 + t^5/5 + ...) with t = (x - 1) / (x + 1) until it
 * stops changing. Returns 0 for x <= 0.
 */
f32 func_1504A620(f32 x) {
    f32 sum;
    f32 prev;
    f32 t;
    f32 t2;
    f32 term = 0.0f; // matching: dead init makes IDO share $f0 between the 0.0f constant and term
    s32 i;

    if (x < 0.0f) {
        return 0;
    }
    if (x == 0.0f) {
        return 0;
    }
    sum = 0.0f;
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
        prev = sum;
        sum += term / i;
        term *= t2;
        i += 2;
    } while (sum != prev);
    return sum;
}
