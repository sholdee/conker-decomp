#include <ultra64.h>

#include "functions.h"
#include "variables.h"


f32 math_lerpAngle(f32 arg0, f32 arg1, f32 arg2) {
    f32 res = math_angleDiff(arg1, arg2) * arg0;
    return arg1 + res;
}

void math_normalizeDegrees(f32 *arg0) {
    while (*arg0 < 0.0f) {
        *arg0 = (f32) (*arg0 + 360.0f);
    }
    while (*arg0 >= 360.0f) {
        *arg0 = (f32) (*arg0 - 360.0f);
    }
}
