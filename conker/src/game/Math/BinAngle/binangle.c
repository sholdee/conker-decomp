#include <ultra64.h>

#include "conker/anim.h"
#include "conker/math.h"

/*
 * Angles here are u8 binary angles: 256 steps per full turn, so 64 is a right angle.
 * gCosTable is a quarter-wave cosine table, gCosTable[i] = cos(i * 2pi / 256) for i in 0..64.
 */

/* Cosine of a u8 binary angle, built from the quarter-wave table by symmetry. */
f32 math_cosBinAngle(u8 angle) {
    f32 value;

    if (angle > 64) {
        if (angle > 128) {
            if (angle > 192) {
                value = gCosTable[256 - angle];
            } else {
                value = -gCosTable[angle - 128];
            }
        } else {
            value = -gCosTable[128 - angle];
        }
    } else {
        value = gCosTable[angle];
    }

    return value;
}

/* Sine of a u8 binary angle: sin(a) = cos(a - quarter turn). */
f32 math_sinBinAngle(u8 angle) {
    return math_cosBinAngle(angle - 64);
}

/* Signed difference target - current in degrees, wrapped to take the short way round. */
f32 math_angleDiff(f32 current, f32 target) {
    f32 diff = current - target;

    if (diff > 180.0f) {
        current -= 360.0f;
    } else if (diff <= -180.0f) {
        target -= 360.0f;
    }
    return target - current;
}

/* Integer-degree version of math_angleDiff. */
s32 math_angleDiffInt(s32 current, s32 target) {
    s32 diff = current - target;

    if (diff > 180) {
        current -= 360;
    } else if (diff <= -180) {
        target -= 360;
    }
    return target - current;
}
