#include <ultra64.h>

#define func_15048A40 func_15048A40_hdr /* functions.h declares it returning void */
#include "functions.h"
#include "variables.h"
#undef func_15048A40

/*
 * Angles here are u8 binary angles: 256 steps per full turn, so 64 is a right angle.
 * D_8009A220 is a quarter-wave cosine table, D_8009A220[i] = cos(i * 2pi / 256) for i in 0..64.
 */

/* Cosine of a u8 binary angle, built from the quarter-wave table by symmetry. */
f32 func_150489B0(u8 angle) {
    f32 value;

    if (angle > 64) {
        if (angle > 128) {
            if (angle > 192) {
                value = D_8009A220[256 - angle];
            } else {
                value = -D_8009A220[angle - 128];
            }
        } else {
            value = -D_8009A220[128 - angle];
        }
    } else {
        value = D_8009A220[angle];
    }

    return value;
}

/* Sine of a u8 binary angle: sin(a) = cos(a - quarter turn). */
f32 func_15048A40(u8 angle) {
    return func_150489B0(angle - 64);
}

/* Signed difference target - current in degrees, wrapped to take the short way round. */
f32 func_15048A70(f32 current, f32 target) {
    f32 diff = current - target;

    if (diff > 180.0f) {
        current -= 360.0f;
    } else if (diff <= -180.0f) {
        target -= 360.0f;
    }
    return target - current;
}

/* Integer-degree version of func_15048A70. */
s32 func_15048AD0(s32 current, s32 target) {
    s32 diff = current - target;

    if (diff > 180) {
        current -= 360;
    } else if (diff <= -180) {
        target -= 360;
    }
    return target - current;
}
