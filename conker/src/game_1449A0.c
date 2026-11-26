#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that swing one rotation axis toward a target angle.
 */

#define WORK(obj) ((MoverWork *)&(obj)->touch)


/* Swings rotX toward the target angle with prop_springAngle's damped spring. */
void prop_swingX(WorldObject *obj) {
    obj->t = prop_springAngle(obj, obj->t);
}

/*
 * Accelerates angle toward the target in the low 10 bits of param (signed degrees). When it crosses the
 * target it snaps there and bounces back with -rebound times its velocity; it comes to rest once close and
 * slow. Starts the param >> 10 start sound when it begins moving. Returns the new angle.
 */
f32 prop_bounceAngle(WorldObject *obj, f32 angle) {
    f32 velocity;
    s32 soundIndex;
    f32 target;
    f32 after;
    f32 before;
    f32 rebound;
    f32 acceleration;

    rebound = WORK(obj)->bounce.rebound;
    acceleration = WORK(obj)->bounce.acceleration * D_800BE9E4;
    velocity = WORK(obj)->bounce.velocity;
    target = (obj->moveData.word << 22) >> 22;
    if (angle != target || velocity != 0.0f) {
        soundIndex = obj->moveData.word >> 10;
        if (velocity == 0.0f && soundIndex != 0 && gPropSounds[soundIndex].open != 0) {
            gfx_playObjectSound(obj, gPropSounds[soundIndex].open, 24000, 200, 2500, 0);
        }
        before = math_angleDiff(angle, target);
        angle += velocity * D_800BE9E4;
        after = math_angleDiff(angle, target);
        if ((after <= 0.0f && before > 0.0f) || (after >= 0.0f && before < 0.0f)) {
            angle = target;
            velocity = -rebound * velocity;
            after = 0.0; // matching: a double 0.0 (or int 0) gets IDO to reuse $f18 for the zero; 0.0f does not
        } else if (after > 0.0f) {
            velocity += acceleration;
        } else {
            velocity -= acceleration;
        }
        if (fabsf(after) < 2.0f && fabsf(velocity) < 1.0f) {
            angle = target;
            velocity = 0.0f;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        WORK(obj)->bounce.velocity = velocity;
    }
    return angle;
}

/* Bounces rotX toward the target angle (prop_bounceAngle). */
void prop_bounceX(WorldObject *obj) {
    obj->t = prop_bounceAngle(obj, obj->t);
}

/* Bounces rotZ toward the target angle (prop_bounceAngle). */
void prop_bounceZ(WorldObject *obj) {
    obj->v = prop_bounceAngle(obj, obj->v);
}
