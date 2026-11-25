#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);
s32 func_150535F4(struct127 *arg0);
void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
void func_1504CA60(struct127 *arg0);
void func_150D7790(u8 *arg0, s32 arg1);

/**
 * Eases the object's unkC4 toward its turn rate: the change from prevAngle to the current moving angle,
 * scaled by 100 / D_800CC264.
 */
void func_15052760(struct127 *obj, s16 prevAngle) {
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

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
} Struct150528C8;


void func_150528C8(struct127 *arg0) {
    f32 targetY;
    f32 scale;
    u32 ret;
    s16 angle;
    f32 floorY;
    s32 flag;
    s32 mode;
    f32 limit;
    f32 diff;
    f32 t;

    scale = (s8)arg0->unk22B;
    angle = arg0->unk76;
    floorY = ((Struct150528C8 **)D_800D2104)[arg0->unk13F][0].unk2;
    flag = 1;
    mode = 0;
    if ((arg0->id == 0x4F) && ((u32)arg0->unk65 != 0) && (arg0->unk1D4 != NULL)) {
        arg0->y_position = D_800CC2D0[arg0->unk65].y_position + (D_800CC2D0[arg0->unk65 - 1].y_scale * 284.0f);
    }
    arg0->unkF8 |= 0x10000;
    arg0->unk80 = 10;
    if (arg0->id == 10) {
        targetY = D_800CC2D0[arg0->unk222].y_position + 80.0f;
    } else {
        targetY = ((Struct150528C8 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].unk2;
    }
    if ((s16)arg0->unk224 != 0) {
        targetY = (s16)arg0->unk224;
    }
    if ((arg0->unk223 == 14) || (arg0->unk223 == 3)) {
        targetY = D_800CC2D0[arg0->unk222].y_position - 40.0f;
    }
    if ((arg0->id == 0x4F) || (arg0->id == 0x83)) {
        if (arg0->unk251 == 1) {
            flag = 0;
            ret = func_1505A630(arg0->xz_velocity, arg0->y_velocity, 0);
            arg0->unkB8 = ret * -0.005493164f;
        } else {
            mode = 1;
            if (D_800CC2D0[arg0->unk124 - 1].unk127 == 0xFF) {
                arg0->unkB8 *= 0.200000003f;
            }
        }
    }
    if (arg0->id == 8) {
        targetY = D_800CC2D0[arg0->unk222].y_position - 40.0f;
        if ((s16)arg0->unk224 != 0) {
            targetY = (s16)arg0->unk224;
        }
        if ((floorY < arg0->y_position) && (arg0->y_velocity > 0.0f)) {
            arg0->y_velocity *= 0.8000000119f;
        }
        if (arg0->unk223 != 3) {
            mode = 1;
        }
    }
    if (floorY < targetY) {
        targetY = floorY;
    }
    if (mode != 0) {
        limit = scale * D_800BE9A4;
        diff = targetY - arg0->y_position;
        if ((diff < limit) && (-limit < diff)) {
            mode = 2;
            arg0->y_position += diff * 0.150000006f;
            arg0->y_velocity = 0.0f;
            arg0->gravity = 0.0f;
        }
    }
    if (arg0->unk226 == 0xFF) {
        f32 vel;
        if (arg0->unk31C != NULL) {
            if (D_800BE9E4 < *(u16 *)((u8 *)arg0->unk31C + 0x12C)) {
                *(u16 *)((u8 *)arg0->unk31C + 0x12C) -= D_800BE9E4;
                if (arg0->health > 0) {
                    arg0->xz_velocity = 254.0f;
                }
            } else {
                *(u16 *)((u8 *)arg0->unk31C + 0x12C) = 0;
            }
            if ((arg0->unk31C->unk8C & 0x2000) && (arg0->health != 0)) {
                func_150D7790((u8 *)arg0, arg0->unk31C->unk75 & 0x7F);
            }
        }
        func_1504CA60(arg0);
        vel = -arg0->y_velocity;
        if (vel > 30.0f) {
            vel = 30.0f;
        }
        arg0->unkB8 += (vel - arg0->unkB8) * 0.400000006f;
    } else {
        if ((mode != 2) && ((u32)arg0->unk226 != 0)) {
            t = (u32)arg0->unk226 * 0.1000000015f;
            func_15058EA4(arg0, targetY, t, targetY, -t, scale, -scale);
        }
        if ((flag != 0) && (arg0->id != 0x49) && !(arg0->unk22C & 8)) {
            arg0->unkB8 += ((arg0->y_velocity * -2.5f) - arg0->unkB8) * 0.1800000072f;
        }
    }
    if (arg0->stunned != 0) {
        func_150585F0(arg0, 0.05000000074505806f);
    } else {
        func_15056B08(arg0);
    }
    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.005493164f;
    func_15059140(arg0);
    if (flag != 0) {
        func_15052760(arg0, angle);
    }
    if (arg0->id == 0x9E) {
        arg0->unk2E4 = 0;
    }
    if (((D_800BE616 != 0) && (D_800C3E78 < D_8008FD90)) || (func_150535F4(arg0) == 0)) {
        func_1502178C(arg0, 0, -1);
    }
}
