#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1502178C(Actor *arg0, s32 arg1, s32 arg2);
s32 func_150535F4(Actor *arg0);
void func_15056B08(Actor *arg0);
void func_150585F0(Actor *arg0, f32 arg1);
void func_1504CA60(Actor *arg0);
void func_150D7790(u8 *arg0, s32 arg1);

/**
 * Eases the object's unkC4 toward its turn rate: the change from prevAngle to the current moving angle,
 * scaled by 100 / D_800CC264.
 */
void func_15052760(Actor *obj, s16 prevAngle) {
    f32 blend;
    s8 direction;
    s32 divisor;
    s32 turnRate;
    s32 id;
    s32 rate;

    prevAngle = (prevAngle & 0xFFFF) - obj->unk76;
    turnRate = (prevAngle * 100) / D_800CC264;
    id = obj->id;
    // matching: the redundant id == 0x29 test is emitted as its own branch
    if (((id == 0x29) || (id != 8)) && !(obj->unk22C & 8)) {
        blend = 0.07999999821f;
        divisor = 7;
        if (id == 10) {
            divisor = 40;
        }
        if (obj->unk226 == 0xFF) {
            direction = turnRate >> 8;
            if ((obj->unk87 ^ direction) & 0x80) {
                turnRate = 0;
            }
            blend = 0.3000000119f;
            divisor = 4;
            obj->unk87 = direction;
        }
        rate = obj->unk1E5;
        if (rate != 0) {
            obj->unkC4 += ((f32) (turnRate / (rate * divisor)) - obj->unkC4) * blend;
        }
    }
}


/**
 * Per-frame movement for a path-following object: picks a target height (the next path point, or the
 * object in unk222), eases y_position toward it, updates the pitch-like unkB8, then moves the object.
 */
void func_150528C8(Actor *obj) {
    f32 targetY;
    f32 climbSpeed;
    u32 pitch;
    s16 prevAngle;
    f32 floorY;
    s32 autoTilt;
    s32 heightMode; // 0: free, 1: approach targetY, 2: snapped to targetY
    f32 maxStep;
    f32 dy;
    f32 range;

    climbSpeed = (s8)obj->unk22B;
    prevAngle = obj->unk76;
    floorY = D_800D2104[obj->unk13F][0].y;
    autoTilt = 1;
    heightMode = 0;
    // matching: the u32 compare (a no-op on a u8) steers IDO's register allocation
    if ((obj->id == 0x4F) && ((u32)obj->unk65 != 0) && (obj->unk1D4 != NULL)) {
        obj->y_position = movement_obj[obj->unk65].y_position + (movement_obj[obj->unk65 - 1].y_scale * 284.0f);
    }
    obj->unkF8 |= 0x10000;
    obj->unk80 = 10;
    if (obj->id == 10) {
        targetY = movement_obj[obj->unk222].y_position + 80.0f;
    } else {
        targetY = D_800D2104[obj->unk13F][obj->unk21E + 1].y;
    }
    if ((s16)obj->unk224 != 0) {
        targetY = (s16)obj->unk224;
    }
    if ((obj->unk223 == 14) || (obj->unk223 == 3)) {
        targetY = movement_obj[obj->unk222].y_position - 40.0f;
    }
    if ((obj->id == 0x4F) || (obj->id == 0x83)) {
        if (obj->unk251 == 1) {
            autoTilt = 0;
            pitch = func_1505A630(obj->xz_velocity, obj->y_velocity, 0);
            obj->unkB8 = pitch * -0.005493164f;
        } else {
            heightMode = 1;
            if (movement_obj[obj->unk124 - 1].unk127 == 0xFF) {
                obj->unkB8 *= 0.200000003f;
            }
        }
    }
    if (obj->id == 8) {
        targetY = movement_obj[obj->unk222].y_position - 40.0f;
        if ((s16)obj->unk224 != 0) {
            targetY = (s16)obj->unk224;
        }
        if ((floorY < obj->y_position) && (obj->y_velocity > 0.0f)) {
            obj->y_velocity *= 0.8000000119f;
        }
        if (obj->unk223 != 3) {
            heightMode = 1;
        }
    }
    if (floorY < targetY) {
        targetY = floorY;
    }
    if (heightMode != 0) {
        maxStep = climbSpeed * D_800BE9A4;
        dy = targetY - obj->y_position;
        if ((dy < maxStep) && (-maxStep < dy)) {
            heightMode = 2;
            obj->y_position += dy * 0.150000006f;
            obj->y_velocity = 0.0f;
            obj->gravity = 0.0f;
        }
    }
    if (obj->unk226 == 0xFF) {
        f32 vel;
        if (obj->unk31C != NULL) {
            if (D_800BE9E4 < obj->unk31C->unk12C) {
                obj->unk31C->unk12C -= D_800BE9E4;
                if (obj->health > 0) {
                    obj->xz_velocity = 254.0f;
                }
            } else {
                obj->unk31C->unk12C = 0;
            }
            if ((obj->unk31C->unk8C & 0x2000) && (obj->health != 0)) {
                func_150D7790((u8 *)obj, obj->unk31C->context_b & 0x7F);
            }
        }
        func_1504CA60(obj);
        vel = -obj->y_velocity;
        if (vel > 30.0f) {
            vel = 30.0f;
        }
        obj->unkB8 += (vel - obj->unkB8) * 0.400000006f;
    } else {
        // matching: as above, the compare needs the u32 cast
        if ((heightMode != 2) && ((u32)obj->unk226 != 0)) {
            range = (u32)obj->unk226 * 0.1000000015f;
            func_15058EA4(obj, targetY, range, targetY, -range, climbSpeed, -climbSpeed);
        }
        if ((autoTilt != 0) && (obj->id != 0x49) && !(obj->unk22C & 8)) {
            obj->unkB8 += ((obj->y_velocity * -2.5f) - obj->unkB8) * 0.1800000072f;
        }
    }
    if (obj->stunned != 0) {
        func_150585F0(obj, 0.0500000007f);
    } else {
        func_15056B08(obj);
    }
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    func_15059140(obj);
    if (autoTilt != 0) {
        func_15052760(obj, prevAngle);
    }
    if (obj->id == 0x9E) {
        obj->unk2E4 = 0;
    }
    if (((D_800BE616 != 0) && (dc_glbl_CharNum < D_8008FD90)) || (func_150535F4(obj) == 0)) {
        func_1502178C(obj, 0, -1);
    }
}
