#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that swing one rotation axis toward a target angle.
 */
/* Behaviour scratch state (WorldObject.work): a damped swing or a bounce. */
typedef union MoverWork {
    struct {
        /* 0x7C */ f32 damping;
        /* 0x80 */ f32 acceleration;
        /* 0x84 */ f32 velocity;
    } swing;
    struct {
        /* 0x7C */ f32 rebound; /* fraction of the velocity kept (reversed) when the target is crossed */
        /* 0x80 */ f32 acceleration; /* per tick */
        /* 0x84 */ f32 velocity;
    } bounce;
} MoverWork;

#define WORK(obj) ((MoverWork *)&(obj)->work)

/* One row of D_80089260, selected by param >> 10. */
typedef struct MoverSounds {
    /* 0x0 */ s16 start; /* played when the object starts moving */
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 stop;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 arrive;
    /* 0xA */ s16 arriveLead;
} MoverSounds;

extern MoverSounds D_80089260[];

extern void func_15114D24(WorldObject *obj, s32 sound, s32 arg2, s16 arg3, s16 arg4, s32 flags);
f32 func_151172D8(WorldObject *obj, f32 angle);
f32 func_15117518(WorldObject *obj, f32 angle);

/* Swings rotX toward the target angle with func_151172D8's damped spring. */
void func_151174F0(WorldObject *obj) {
    obj->rotX = func_151172D8(obj, obj->rotX);
}

/*
 * Accelerates angle toward the target in the low 10 bits of param (signed degrees). When it crosses the
 * target it snaps there and bounces back with -rebound times its velocity; it comes to rest once close and
 * slow. Starts the param >> 10 start sound when it begins moving. Returns the new angle.
 */
f32 func_15117518(WorldObject *obj, f32 angle) {
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
    target = (obj->param.word << 22) >> 22;
    if (angle != target || velocity != 0.0f) {
        soundIndex = obj->param.word >> 10;
        if (velocity == 0.0f && soundIndex != 0 && D_80089260[soundIndex].start != 0) {
            func_15114D24(obj, D_80089260[soundIndex].start, 24000, 200, 2500, 0);
        }
        before = func_15048A70(angle, target);
        angle += velocity * D_800BE9E4;
        after = func_15048A70(angle, target);
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

/* Bounces rotX toward the target angle (func_15117518). */
void func_15117770(WorldObject *obj) {
    obj->rotX = func_15117518(obj, obj->rotX);
}

/* Bounces rotZ toward the target angle (func_15117518). */
void func_15117798(WorldObject *obj) {
    obj->rotZ = func_15117518(obj, obj->rotZ);
}
