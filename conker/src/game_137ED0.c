#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Behaviour scratch state (WorldObject.touch) of the swinging object. */
typedef struct {
    /* 0x7C */ s32 secondHalf; // phase has passed pi
    /* 0x80 */ f32 phase;      // radians, 0 .. 2pi
} SwingWork;

/* Behaviour scratch state (WorldObject.touch) of the clock's minute hand; its param word is the time. */
typedef struct {
    /* 0x7C */ s32 chimesLeft;
    /* 0x80 */ s32 lastChimeSecond;
    /* 0x84 */ s32 synced; // time has been set from D_800BE3E4
} ClockWork;

#define SWING_WORK(obj) ((SwingWork *)&(obj)->touch)
#define CLOCK_WORK(obj) ((ClockWork *)&(obj)->touch)

void func_1510AA20(s32 arg0) {
    effect_updateRain(0);
}

/*
 * Clock update, run on the minute hand; the hour hand is the next object. Advances the time, sets both hands,
 * and chimes the hour on the hour (12 for 12:00) or once at each other quarter hour.
 */
void func_1510AA44(WorldObject *minuteHand) {
    s32 oldMinute;
    s32 minute;
    s32 second;
    f32 hourAngle;

    oldMinute = minuteHand->moveData.word / 3600 % 60;
    if (!CLOCK_WORK(minuteHand)->synced && D_800BE3E4 != 0) {
        CLOCK_WORK(minuteHand)->synced = 1;
        minuteHand->moveData.word = D_800BE3E4 * 180 % (12 * 60 * 60);
        minuteHand->moveData.word *= 60; // matching: a separate statement (one expression mismatches)
        oldMinute = minuteHand->moveData.word / 3600 % 60;
    }
    if (oldMinute < 0) {
        oldMinute += 60;
    }
    minuteHand->moveData.word += D_800BE9E4;
    if (minuteHand->moveData.word >= 12 * 60 * 60 * 60) {
        minuteHand->moveData.word -= 12 * 60 * 60 * 60;
    }
    second = minuteHand->moveData.word / 60;
    minute = second / 60;
    hourAngle = (second / 300) * 2.5f;
    if (hourAngle > 360.0f) {
        minuteHand->moveData.word -= 12 * 60 * 60 * 60;
    }
    minute = minute % 60;
    if (minute < 0) {
        minute += 60;
    }
    if (oldMinute != minute && oldMinute / 15 != minute / 15) {
        if ((minute / 15 & 3) == 0) {
            CLOCK_WORK(minuteHand)->chimesLeft = hourAngle / 30.0f;
            if (CLOCK_WORK(minuteHand)->chimesLeft == 0) {
                CLOCK_WORK(minuteHand)->chimesLeft = 12;
            }
        } else {
            CLOCK_WORK(minuteHand)->chimesLeft = 1;
        }
        CLOCK_WORK(minuteHand)->lastChimeSecond = -1;
    }
    if (CLOCK_WORK(minuteHand)->chimesLeft != 0 && CLOCK_WORK(minuteHand)->lastChimeSecond != second) {
        CLOCK_WORK(minuteHand)->lastChimeSecond = second;
        CLOCK_WORK(minuteHand)->chimesLeft--;
        gfx_playObjectSound(minuteHand, 0x4CC, 0x7FFF, 200, 1000, 12);
    }
    minuteHand[0].spinZ = 5.0f;
    minuteHand[1].spinZ = 5.0f;
    minuteHand[0].v = -(minute * 6.0f);
    minuteHand[1].v = -hourAngle;
}

/* Swings obj->t (x rotation) by +-10 with a 2 second period, playing sound 0x4CA / 0x4CB at each end of the swing. */
void func_1510ADD8(WorldObject *obj) {
    f32 rot;

    SWING_WORK(obj)->phase += (3.1415927f * D_800BE9E4) / 60.0f;
    rot = cosf(SWING_WORK(obj)->phase) * 10.0f;
    obj->spinX = obj->t - rot;
    obj->t = rot;
    if (SWING_WORK(obj)->secondHalf == 0) {
        if (3.1415927f <= SWING_WORK(obj)->phase) {
            SWING_WORK(obj)->secondHalf = 1;
            gfx_playObjectSound(obj, 0x4CA, 12000, 200, 1000, 4);
        }
    } else {
        if (6.2831855f <= SWING_WORK(obj)->phase) {
            SWING_WORK(obj)->phase -= 6.2831855f;
            SWING_WORK(obj)->secondHalf = 0;
            gfx_playObjectSound(obj, 0x4CB, 12000, 200, 1000, 4);
        }
    }
}

/*
 * Tests the point (x, y, z) against a view frustum given by the view matrix `mtx`: depth must lie in
 * [nearZ, farZ] and the projected offsets within depth * slopeX / slopeY. Returns 1 when outside, else 0
 * and (optionally) how far inside the frustum the point lies horizontally / vertically.
 */
s32 func_1510AEE0(MtxF mtx, f32 x, f32 y, f32 z, f32 nearZ, f32 farZ, f32 slopeX, f32 slopeY,
                  f32 *marginX, f32 *marginY) {
    f32 offsetY;
    f32 offsetX;
    f32 depth;

    depth = -((mtx[0][2] * x) + (mtx[1][2] * y) + (mtx[2][2] * z) + mtx[3][2]);
    if (depth < nearZ) {
        return 1;
    }
    if (farZ < depth) {
        return 1;
    }

    offsetX = (mtx[0][0] * x) + (mtx[1][0] * y) + (mtx[2][0] * z) + mtx[3][0];
    offsetX = fabsf(offsetX) * slopeX;
    if (depth < offsetX) {
        return 1;
    }

    offsetY = (mtx[0][1] * x) + (mtx[1][1] * y) + (mtx[2][1] * z) + mtx[3][1];
    offsetY = fabsf(offsetY) * slopeY;
    if (depth < offsetY) {
        return 1;
    }

    if (marginX != NULL) {
        *marginX = 1.0f - (offsetX / depth);
    }
    if (marginY != NULL) {
        *marginY = 1.0f - (offsetY / depth);
    }
    return 0;
}
