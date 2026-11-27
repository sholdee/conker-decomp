#include <ultra64.h>
#include "conker/math.h"

#define PI 3.1415927410125732f


f32 math_acosLookup(f32 arg0) {
    s16 phi_a0;

    if (arg0 >= 1.0f) {
        phi_a0 = 0x7FFF;
    } else if (arg0 <= -1.0f) {
        phi_a0 = -0x7FFF;
    } else {
        phi_a0 = arg0 * 32767.0f;
    }
    return math_acosFixed(phi_a0) * (PI / 65535);
}

f32 math_asinLookup(f32 arg0) {
    s16 phi_a0;

    if (arg0 >= 1.0f) {
        phi_a0 = 0x7FFF;
    } else if (arg0 <= -1.0f) {
        phi_a0 = -0x7FFF;
    } else {
        phi_a0 = arg0 * 32767.0f;
    }
    return math_asinFixed(phi_a0) * (PI / 65535);
}
