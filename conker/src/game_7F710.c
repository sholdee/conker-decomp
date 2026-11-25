#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1505A770(struct127 *arg0);

void func_15052260(struct127 *arg0) {
    struct126 *temp_v0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f0;
    f32 temp_f2;

    temp_v0 = arg0->unk31C;
    if ((temp_v0->unk6B == 1) || (temp_v0->unk6B == 3)) {
        temp_f12 = 0.400000006f;
    } else {
        temp_f12 = 1.0f;
    }
    temp_f14 = arg0->y_position;
    arg0->unk1CC = temp_f14;

    if (temp_v0->unk6B < 4) {
        arg0->x_position += (((f32) *(s16 *) ((u8 *) temp_v0 + 0x6C)) - arg0->x_position) * temp_f12;
        arg0->y_position += (((f32) (*(s16 *) ((u8 *) temp_v0 + 0x6E) - 0x50)) - arg0->y_position) * temp_f12;
        arg0->z_position += (((f32) *(s16 *) ((u8 *) temp_v0 + 0x70)) - arg0->z_position) * temp_f12;
        func_1505E650(arg0, 0x14, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        arg0->y_velocity = -10.0f;
    } else {
        if ((s32) arg0->unk28 == 0) {
            temp_v0->unk6B = 0;
        } else {
            func_1505E650(arg0, 0x38, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        }
        func_1505A770(arg0);
    }
    arg0->gravity = 6.199999809f;
    if (arg0->y_position < arg0->unk118) {
        arg0->gravity = 0.0f;
        arg0->y_velocity = 0.0f;
        arg0->unk31C->unk6B = 0;
        arg0->unk83 = 0;
        arg0->in_water = 1;
    }
}

void func_15052408(struct127 *arg0) {
    arg0->unkB2 = 0;
    arg0->unk31C->unk95 = 0;
    if (arg0->health != 0) { // if health not zero
        arg0->unk81 = 0;
        arg0->unk83 = 0;
        arg0->disable_run = 6;
    }
    arg0->in_water = 0;
    arg0->gravity = 4.0f;
    arg0->unkB8 = 0.0f;
    arg0->xz_velocity = 15.0f;
}

void func_15052458(s32 arg0) {
}

void func_15052464(struct127 *arg0) {
    if (arg0->health == 0) {
        func_15060F28(arg0, 1);
    }
}

void func_15052490(struct127 *arg0, u16 arg1, f32 arg2, f32 arg3) {
    arg3 *= D_800D1550[0];

    arg0->unk40 = (arg0->unk7A + 16384) * 0.005493164f;

    if (((arg1 - arg0->unk7A + 16384) & 0x8000) != 0) {
        arg0->unkB8 -= arg2 * arg3;
    } else {
        arg0->unkB8 += arg2 * arg3;
    }
}

void func_1505250C(struct127 *arg0, s32 arg1) {
    s16 temp_v0 = arg0->unkCE;
    s16 temp_v1 = arg0->unkCC;

    if (temp_v0 != temp_v1) {
        if (temp_v1 < temp_v0) {
            arg0->unkCC = temp_v1 + ((temp_v0 - temp_v1) / 0xC) + 1;
            if (temp_v0 < arg0->unkCC) {
                arg0->unkCC = temp_v0;
            }
        } else {
            arg0->unkCC = (temp_v1 + ((temp_v0 - temp_v1) / 0xC)) - 1;
            if (arg0->unkCC < temp_v0) {
                arg0->unkCC = temp_v0;
            }
        }
    }
}
