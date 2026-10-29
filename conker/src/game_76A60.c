#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_76A60/func_150495B0.s")

void func_15049688(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 d;
    f32 ad;
    f32 sign;
    f32 nsign;
    s32 near;
    f32 t;

    d = func_15048A70(*arg0, arg1);
    ad = fabsf(d);
    if (d < 0.0f) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    d *= arg3;
    t = d - *arg2;
    t *= arg4;
    *arg2 += t * arg5;
    near = ad < 10.0f;
    if (near == 0) {
        goto add;
    }
    if (func_15048A70(*arg0 + (*arg2 * arg5), arg1) < 0.0f) {
        nsign = -1.0f;
    } else {
        nsign = 1.0f;
    }
    if (nsign == sign) {
add:
        *arg0 += *arg2 * arg5;
    } else {
        *arg0 = arg1;
        *arg2 = 0;
    }
    func_15048758(arg0);
}
