#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* asin(i / 256) scaled so that 65536 is pi/2, for i = 0..255. */
extern u16 D_80098E00[256];

/* Arcsine of x in [-1, 1], in radians, from the 256-entry table. */
f32 func_150487E0(f32 x) {
    f32 angle = (D_80098E00[(s32)(fabsf(x) * 255.99998f)] * 1.5707964f) / 65536.0f;

    return (x < 0.0f) ? -angle : angle;
}

/* Arcsine of x in 256ths of a turn (64 = pi/2); negative inputs give 255 - angle. */
f32 func_15048864(f32 x) {
    f32 angle = (s32)D_80098E00[(s32)(fabsf(x) * 255.99998f)] >> 10;

    return (x < 0.0f) ? (255.0f - angle) : angle;
}

/* Arcsine of x in [-1, 1], in radians, linearly interpolated between table entries. */
f32 func_150488C8(f32 x) {
    f32 scaled;
    f32 fraction;
    f32 lower;
    f32 upper;
    s32 index;

    scaled = fabsf(x) * 255.99998f;
    index = scaled;
    fraction = scaled - (f32)index;
    lower = D_80098E00[index];

    if (index < 255) {
        upper = D_80098E00[index + 1];
    } else {
        upper = 65535.0f;
    }

    lower = (((upper - lower) * fraction) + lower) * 1.5707964f / 65536.0f;
    if (x < 0.0f) {
        return -lower;
    }
    return lower;
}
