#include <ultra64.h>
#include "conker/math.h"


/**
 * x raised to the power y. Integral exponents use repeated multiplication (or division for negative y);
 * other exponents go through e^(y * ln x). Returns 0 for 0^y and for a negative x with a fractional y.
 */
f32 powf(f32 x, f32 y) {
    f32 acc;

    if (x != 0.0f && y == 0.0f) {
        return 1.0f;
    }
    if (x == 0.0f) {
        return 0.0f;
    }
    // matching: the redundant 0^0 test is in the original and its compares are emitted
    if ((x == 0.0f && y == 0.0f) || (x < 0.0f && y != (s32) y)) {
        return 0.0f;
    }

    if (y != (s32) y) {
        x = expf(logf(x) * y);
    } else if (y > 0.0f) {
        acc = x;
        y--;
        while (y--) {
            x *= acc;
        }
    } else {
        acc = 1.0f;
        while (y++) {
            acc /= x;
        }
        x = acc;
    }
    return x;
}
