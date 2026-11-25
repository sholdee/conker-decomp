#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_15179008(s32);
extern f32 D_800A26B0;
extern f32 D_800A26B4;
extern f32 D_800A26B8;
extern void func_15114D24(s32, s32, s32, s16, s32, s32);

typedef struct {
    f32 field_0x00;
    u8 pad_0x04[0x5C];
    f32 field_0x60;
    u8 pad_0x64[0x18];
    s32 field_0x7C;
    f32 field_0x80;
} OscillatorState;

typedef struct {
    u8  pad00[0x8];
    f32 unk8;
    u8  pad0C[0x30];
    s32 unk3C;
    u8  pad40[0x28];
    f32 unk68;
    u8  pad6C[0x10];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8  pad88[0x20];
    f32 unkA8;
    u8  padAC[0x5C];
    f32 unk108;
} ClockActor;

extern u32 D_800BE3E4;

void func_1510AA20(s32 arg0) {
    func_15179008(0);
}

void func_1510AA44(ClockActor *arg0) {
    s32 oldMin;
    s32 min;
    s32 sec;
    f32 angle;

    oldMin = arg0->unk3C / 3600 % 60;
    if (arg0->unk84 == 0) {
        if (D_800BE3E4 != 0) {
            arg0->unk84 = 1;
            arg0->unk3C = D_800BE3E4 * 180 % 43200;
            arg0->unk3C = arg0->unk3C * 60;
            oldMin = arg0->unk3C / 3600 % 60;
        }
    }
    if (oldMin < 0) {
        oldMin += 60;
    }
    arg0->unk3C = arg0->unk3C + D_800BE9E4;
    if (arg0->unk3C >= 2592000) {
        arg0->unk3C = arg0->unk3C - 2592000;
    }
    sec = arg0->unk3C / 60;
    min = sec / 60;
    angle = (f32) (sec / 300) * 2.5f;
    if (angle > 360.0f) {
        arg0->unk3C = arg0->unk3C - 2592000;
    }
    min = min % 60;
    if (min < 0) {
        min += 60;
    }
    if (oldMin != min) {
        if (oldMin / 15 != min / 15) {
            if ((min / 15 & 3) == 0) {
                arg0->unk7C = (s32) (angle / 30.0f);
                if (arg0->unk7C == 0) {
                    arg0->unk7C = 12;
                }
            } else {
                arg0->unk7C = 1;
            }
            arg0->unk80 = -1;
        }
    }
    if (arg0->unk7C != 0) {
        if (arg0->unk80 != sec) {
            arg0->unk80 = sec;
            arg0->unk7C = arg0->unk7C - 1;
            func_15114D24((s32) arg0, 0x4CC, 0x7FFF, 0xC8, 0x3E8, 0xC);
        }
    }
    arg0->unk68 = 5.0f;
    arg0->unk108 = 5.0f;
    arg0->unk8 = -((f32) min * 6.0f);
    arg0->unkA8 = -angle;
}

void func_1510ADD8(OscillatorState *arg0) {
    f32 temp;

    arg0->field_0x80 += (D_800A26B0 * D_800BE9E4) / 60.0f;
    temp = cosf(arg0->field_0x80) * 10.0f;
    arg0->field_0x60 = arg0->field_0x00 - temp;
    arg0->field_0x00 = temp;
    if (arg0->field_0x7C == 0) {
        if (D_800A26B4 <= arg0->field_0x80) {
            arg0->field_0x7C = 1;
            func_15114D24((s32)arg0, 0x4CA, 0x2EE0, 0xC8, 0x3E8, 4);
        }
    } else {
        temp = D_800A26B8;
        if (temp <= arg0->field_0x80) {
            arg0->field_0x80 -= temp;
            arg0->field_0x7C = 0;
            func_15114D24((s32)arg0, 0x4CB, 0x2EE0, 0xC8, 0x3E8, 4);
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
