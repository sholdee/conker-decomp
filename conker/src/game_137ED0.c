#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u32 D_800BE3E4;

void func_15179008(s32);
extern void func_15114D24(void *obj, s32 sfx, s32 volume, s16 nearDistance, s32 farDistance, s32 flags);

/* A swinging object: one 0xA0-byte entry of the object array (see game_13F9D0.c's Game13F9D0Obj). */
typedef struct SwingObj {
    /* 0x00 */ f32 rotX;
    /* 0x04 */ u8 pad04[0x5C];
    /* 0x60 */ f32 spinX; // rotation this frame
    /* 0x64 */ u8 pad64[0x18];
    /* 0x7C */ s32 secondHalf; // phase has passed pi
    /* 0x80 */ f32 phase; // radians, 0 .. 2pi
} SwingObj;

/* A clock hand: one 0xA0-byte entry of the object array (see game_13F9D0.c's Game13F9D0Obj). */
typedef struct ClockHand {
    /* 0x00 */ u8 pad00[0x8];
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ u8 pad0C[0x30];
    /* 0x3C */ s32 time; // 1/60 s ticks on a 12-hour dial
    /* 0x40 */ u8 pad40[0x28];
    /* 0x68 */ f32 spinZ;
    /* 0x6C */ u8 pad6C[0x10];
    /* 0x7C */ s32 chimesLeft;
    /* 0x80 */ s32 lastChimeSecond;
    /* 0x84 */ s32 synced; // time has been set from D_800BE3E4
    /* 0x88 */ u8 pad88[0x18];
} ClockHand; // size 0xA0

void func_1510AA20(s32 arg0) {
    func_15179008(0);
}

/*
 * Clock update, run on the minute hand; the hour hand is the next object. Advances the time, sets both hands,
 * and chimes the hour on the hour (12 for 12:00) or once at each other quarter hour.
 */
void func_1510AA44(ClockHand *minuteHand) {
    s32 oldMinute;
    s32 minute;
    s32 second;
    f32 hourAngle;

    oldMinute = minuteHand->time / 3600 % 60;
    if (!minuteHand->synced && D_800BE3E4 != 0) {
        minuteHand->synced = 1;
        minuteHand->time = D_800BE3E4 * 180 % (12 * 60 * 60);
        minuteHand->time *= 60; // matching: a separate statement (one expression mismatches)
        oldMinute = minuteHand->time / 3600 % 60;
    }
    if (oldMinute < 0) {
        oldMinute += 60;
    }
    minuteHand->time += D_800BE9E4;
    if (minuteHand->time >= 12 * 60 * 60 * 60) {
        minuteHand->time -= 12 * 60 * 60 * 60;
    }
    second = minuteHand->time / 60;
    minute = second / 60;
    hourAngle = (second / 300) * 2.5f;
    if (hourAngle > 360.0f) {
        minuteHand->time -= 12 * 60 * 60 * 60;
    }
    minute = minute % 60;
    if (minute < 0) {
        minute += 60;
    }
    if (oldMinute != minute && oldMinute / 15 != minute / 15) {
        if ((minute / 15 & 3) == 0) {
            minuteHand->chimesLeft = hourAngle / 30.0f;
            if (minuteHand->chimesLeft == 0) {
                minuteHand->chimesLeft = 12;
            }
        } else {
            minuteHand->chimesLeft = 1;
        }
        minuteHand->lastChimeSecond = -1;
    }
    if (minuteHand->chimesLeft != 0 && minuteHand->lastChimeSecond != second) {
        minuteHand->lastChimeSecond = second;
        minuteHand->chimesLeft--;
        func_15114D24(minuteHand, 0x4CC, 0x7FFF, 200, 1000, 12);
    }
    minuteHand[0].spinZ = 5.0f;
    minuteHand[1].spinZ = 5.0f;
    minuteHand[0].rotZ = -(minute * 6.0f);
    minuteHand[1].rotZ = -hourAngle;
}

/* Swings obj->rotX by +-10 with a 2 second period, playing sound 0x4CA / 0x4CB at each end of the swing. */
void func_1510ADD8(SwingObj *obj) {
    f32 rot;

    obj->phase += (3.1415927f * D_800BE9E4) / 60.0f;
    rot = cosf(obj->phase) * 10.0f;
    obj->spinX = obj->rotX - rot;
    obj->rotX = rot;
    if (obj->secondHalf == 0) {
        if (3.1415927f <= obj->phase) {
            obj->secondHalf = 1;
            func_15114D24(obj, 0x4CA, 12000, 200, 1000, 4);
        }
    } else {
        if (6.2831855f <= obj->phase) {
            obj->phase -= 6.2831855f;
            obj->secondHalf = 0;
            func_15114D24(obj, 0x4CB, 12000, 200, 1000, 4);
        }
    }
}

/*
 * Tests the point (x, y, z) against a view frustum given by the view matrix `mtx`: depth must lie in
 * [nearZ, farZ] and the projected offsets within depth * slopeX / slopeY. Returns 1 when outside, else 0
 * and (optionally) how far inside the frustum the point lies horizontally / vertically.
 */
s32 func_1510AEE0(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 nearZ, f32 farZ, f32 slopeX, f32 slopeY,
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
