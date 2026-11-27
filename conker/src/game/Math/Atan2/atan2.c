#include <ultra64.h>
#include <math.h>
#include "conker/math.h"

/* The calls to sqrtf in this file are real calls (jal sqrtf), not the inline sqrt.s of gu.h's #pragma intrinsic. */
#pragma function sqrtf

#define PI 3.1415927410125732f

f32 math_atan2(f32 arg0, f32 arg1) {
    f32 ret;

    if (arg0 == 0.0f) {
        if (arg1 >= 0.0f) {
            ret = 0.0f;
        } else {
            ret = PI;
        }
    } else if (arg1 == 0.0f) {
        if (arg0 > 0.0f) {
            ret = PI/2;
        } else {
            ret = 3*PI/2;
        }
    } else {
        ret = sqrtf((arg0 * arg0) + (arg1 * arg1));
        if (arg1 < arg0) {
            ret = math_acosLookup(arg1 / ret);
            if (arg0 < 0.0f) {
                ret = 2*PI - ret;
            }
        } else {
            ret = PI/2 - math_acosLookup(arg0 / ret);
            if (arg1 < 0.0f) {
                ret = PI - ret;
            }
            if (ret < 0.0f) {
                ret += 2*PI;
            }
        }
    }
    return ret;
}
