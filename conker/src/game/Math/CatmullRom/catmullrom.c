#include <ultra64.h>
#include "functions.h"
#include "variables.h"


f32 math_catmullRom(f32 *p, s32 i, f32 t) {
    /* Catmull-Rom segment through p[i..i+3] at parameter t */
    f32 a;
    f32 c;
    f32 b;
    f32 r;

    a = -0.5f * p[i] + 1.5f * p[i + 1] + -1.5f * p[i + 2] + 0.5f * p[i + 3];
    b = p[i] + -2.5f * p[i + 1] + (p[i + 2] + p[i + 2]) + -0.5f * p[i + 3];
    c = -0.5f * p[i] + 0.5f * p[i + 2];
    r = ((a * t + b) * t + c) * t;
    r += p[i + 1];
    return r;
}


f32 math_catmullRomDeriv(f32 *p, s32 i, f32 t, f32 *deriv) {
    f32 b;
    f32 c;
    f32 r;
    f32 a;

    a = -0.5f * p[i] + 1.5f * p[i + 1] + -1.5f * p[i + 2] + 0.5f * p[i + 3];
    b = p[i] + -2.5f * p[i + 1] + (p[i + 2] + p[i + 2]) + -0.5f * p[i + 3];
    c = -0.5f * p[i] + 0.5f * p[i + 2];
    r = p[i + 1];
    *deriv = (a * 3.0f * t + (b + b)) * t + c;
    r = ((a * t + b) * t + c) * t + r;
    return r;
}
