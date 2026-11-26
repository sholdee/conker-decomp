#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Behaviour scratch state (WorldObject.touch) of the two behaviours below. */
typedef union {
    struct {
        /* 0x7C */ s32 heightPhase;
        /* 0x80 */ s32 swayPhase;
    } bob;
    struct {
        /* 0x7C */ s32 unk7C;
        /* 0x80 */ s32 timer; // > 0: counting down before the push decays, -1: done
    } push;
} Work143870;

#define WORK(obj) ((Work143870 *)&(obj)->touch)

/* Bobs the object (yOffset) and sways it (rotX): param bytes are (height scale, sway scale, height rate, sway rate) from low to high. */
void prop_bobAndSway(WorldObject *obj) {
    f32 heightScale;
    s32 heightRate;
    s32 swayRate;
    f32 swayScale;
    s16 previousY;
    f32 previousSway;
    f32 sway;

    heightRate = (obj->moveData.word >> 16) & 0xFF;
    heightScale = (obj->moveData.word & 0xFF);
    swayRate = (obj->moveData.word >> 24) & 0xFF;
    swayScale = (u32)((obj->moveData.word >> 8) & 0xFF) * 1.40625f;
    previousY = obj->y;
    previousSway = obj->t;
    obj->yOffset = cosf(WORK(obj)->bob.heightPhase * 9.58738e-05f) * heightScale;
    sway = cosf(WORK(obj)->bob.swayPhase * 9.58738e-05f) * swayScale;
    obj->t = sway;
    WORK(obj)->bob.heightPhase += heightRate * D_800BE9E4;
    WORK(obj)->bob.swayPhase += swayRate * D_800BE9E4;
    obj->moveY = obj->y - previousY;
    obj->spinX = sway - previousSway;
}
/*
 * Updates the object's push value (high half of param) from the pull of movement_obj[0], letting it decay
 * back to zero, then keeps a looping sound whose volume follows the object's speed (spinY).
 */
void prop_updatePush(WorldObject *obj, s32 arg1, s32 soundId, f32 pitchScale) {
    s32 push;
    s32 value;
    s32 timer;
    s32 decay;
    f32 speed;
    f32 amount;
    u32 volume;
    u16 handle;

    if (!(obj->object_status & 4)) {
        push = 0;
    } else {
        push = prop_getApproachSpeed(obj->x, obj->z, &WORK(obj)->push.unk7C, movement_obj) >> 3;
    }
    if ((arg1 & 1) && push < 0) {
        push = 0;
    }
    value = obj->moveData.half.hi;
    value += push; // matching: a separate += keeps the lh in the beql delay slots
    if (push != 0) {
        WORK(obj)->push.timer = 0;
    }
    timer = WORK(obj)->push.timer;
    decay = value * -0.0032;
    if (timer <= 0) {
        value += decay;
        if (value > 0) {
            value -= 1;
            if (value <= 0) {
                if (push == 0) {
                    if (timer == 0) {
                        WORK(obj)->push.timer = 20;
                    } else {
                        WORK(obj)->push.timer = 0;
                        value = 0;
                    }
                } else {
                    value = 0;
                }
            }
        } else if (value < 0) {
            value += 1;
            if (value >= 0) {
                if (push == 0) {
                    if (timer == 0) {
                        WORK(obj)->push.timer = 20;
                    } else {
                        WORK(obj)->push.timer = 0;
                        value = 0;
                    }
                } else {
                    value = 0;
                }
            }
        }
    } else if (timer > 0) { // matching: the redundant test is in the original (blezl)
        if (value > 0) {
            value += 1;
            WORK(obj)->push.timer = timer - D_800BE9E4;
            if (WORK(obj)->push.timer <= 0) {
                WORK(obj)->push.timer = -1;
            }
        } else {
            value -= 1;
            WORK(obj)->push.timer = timer - D_800BE9E4;
            if (WORK(obj)->push.timer <= 0) {
                WORK(obj)->push.timer = -1;
            }
        }
    }
    value <<= 16; // matching: shifting in place schedules the param reload before the sll
    obj->moveData.word = (obj->moveData.word & 0xFFFF) | value;
    prop_spinY(obj);
    if (soundId != 0) {
        f32 maxAmount = 1.0f; // matching: loads 1.0f before the sound test
        speed = obj->spinY;
        amount = fabsf(speed) * 0.1f;
        if (amount > maxAmount) {
            amount = maxAmount;
        }
        volume = (12767.0f * amount) + 20000.0f;
        handle = obj->sound_id;
        amount = (amount * pitchScale) - 500.0f;
        if (handle == 0) {
            if (speed > 0.01f) {
                obj->sound_id = func_10010F88(soundId, volume, amount, 0, -1, obj->x, obj->y, obj->z, 1000, 6000);
            }
        } else if (speed > 0.01f) {
            func_1000F91C(handle, volume, amount, 0, -1, obj->x, obj->y, obj->z, 200, 2000);
        } else {
            sound_stop(handle);
            obj->sound_id = 0;
        }
    }
}

/* Returns other's horizontal speed projected onto the direction from (x, z) towards it. */
s32 prop_getApproachSpeed(s32 x, s32 z, s32 *unused, Actor *other) {
    s32 otherX;
    s32 otherZ;
    s32 bearing;
    s32 angle;
    f32 result;
    f32 cosine;
    Vec3f delta;

    otherX = other->xpos;
    otherZ = other->zpos;
    delta.x = otherX - x;
    delta.z = otherZ - z;

    bearing = math_vec3HeadingBin(&delta);
    angle = other->angle;
    angle = 0x40 - (angle >> 8);
    angle -= bearing;
    cosine = math_sinBinAngle(angle);
    result = other->speed * cosine;
    return result;
}

/* Does nothing. */
void func_15116924(s32 arg0) {
}
