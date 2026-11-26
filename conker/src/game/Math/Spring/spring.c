#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/**
 * Moves *value towards target with a damped spring: the desired velocity is the remaining
 * distance times speedGain, and *velocity approaches it at rate accelGain over timestep dt.
 * If the step would overshoot target, *value snaps to target and *velocity is cleared.
 */
void math_springTo(f32 *value, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt) {
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

/**
 * Angle version of math_springTo: springs the angle *angle (degrees) towards target along the
 * shortest way round, then re-wraps it. The overshoot snap only applies within 10 degrees.
 */
void math_springAngleTo(f32 *angle, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt) {
    f32 desiredVelocity;
    f32 distance;
    f32 direction;
    s32 isNear;
    f32 accel;

    desiredVelocity = math_angleDiff(*angle, target);
    distance = fabsf(desiredVelocity);
    if (desiredVelocity < 0.0f) {
        direction = -1.0f;
    } else {
        direction = 1.0f;
    }
    desiredVelocity *= speedGain;
    accel = desiredVelocity - *velocity;
    accel *= accelGain;
    *velocity += accel * dt;
    // matching: IDO materializes the flag (or/addiu) only for `isNear == 0`, not for `!isNear`
    isNear = distance < 10.0f;
    if (isNear == 0 || ((math_angleDiff(*angle + *velocity * dt, target) < 0.0f) ? -1.0f : 1.0f) == direction) {
        *angle += *velocity * dt;
    } else {
        *angle = target;
        *velocity = 0;
    }
    math_normalizeDegrees(angle);
}
