#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/**
 * Moves *value towards target with a damped spring: the desired velocity is the remaining
 * distance times speedGain, and *velocity approaches it at rate accelGain over timestep dt.
 * If the step would overshoot target, *value snaps to target and *velocity is cleared.
 */
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt) {
    f32 desiredVelocity;
    f32 direction;
    f32 accel;
    f32 nextDirection;
    f32 next;

    desiredVelocity = target - *value;
    if (desiredVelocity < 0.0f) {
        direction = -1.0f;
    } else {
        direction = 1.0f;
    }
    desiredVelocity *= speedGain;
    accel = desiredVelocity - *velocity;
    accel *= accelGain;
    *velocity += accel * dt;
    next = *value + (*velocity * dt);
    if (target < next) {
        nextDirection = -1.0f;
    } else {
        nextDirection = 1.0f;
    }
    if (nextDirection == direction) {
        *value = next;
    } else {
        *value = target;
        *velocity = 0; // matching: int 0, not 0.0f (0.0f changes the codegen)
    }
}

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
