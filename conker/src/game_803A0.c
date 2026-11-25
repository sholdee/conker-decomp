#include <ultra64.h>

#include "functions.h"
#include "variables.h"

u8 *func_15033E84(struct127 *arg0);

void func_15052EF0(struct127 *arg0) {
    arg0->immune = 100;
    arg0->unk40 =  ((s16) (arg0->unk7A + 16384)) * 0.005493164f;
    func_1505E650(arg0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

struct127 *func_15052F58(s32 arg0, s32 arg1) {
    struct127 *temp_v0;

    temp_v0 = &D_800CC2D0[arg0];
    temp_v0->unk13C = (u8)0;
    temp_v0->unk218 = 0;
    temp_v0->unk232 = arg1;
    return temp_v0;
}

void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    struct127 *obj;
    s32 anim;
    s32 flag;
    u8 *temp;
    u16 rot;
    s32 state;

    flag = 0;
    anim = -1;
    if (arg0->id == 0x48) {
        temp = func_15033E84(arg0);
        if (temp != NULL) {
            anim = temp[6];
        }
    }
    obj = &D_800CC2D0[arg0->unk124];
    if ((arg8 != 0) || ((obj->y_position < (arg0->y_position + (arg1 * arg0->y_scale))) && (obj->y_velocity < 0.0f))) {
        arg0->unk13C = arg0->unk124 + 0x64;
        if (arg9 != 0) {
            D_800D1580 = arg9;
            func_1506E5FC();
        }
        obj->unk1CC = obj->y_position;
        obj->unk76 = arg0->unk7A;
        obj->unk31C->unk30 = 0;
        if (arg0->interaction_state == 0x20) {
            obj->y_velocity = 0.0f;
            state = 2;
            arg0->y_velocity = 0.0f;
            if (arg0->id == 0x48) {
                if (anim == 0x7A) {
                    arg0->unk65 = arg0->unk124 + 1;
                    arg0->unk5C = arg2;
                    arg0->unk101 |= arg3;
                    func_1505E650(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
                    func_15083568(obj, 0x79, 1.0f, 0);
                    flag = 1;
                }
            } else {
                arg0->y_scale = 0.75f;
                obj->y_position = arg0->y_position;
                state = 1;
                if (D_800BE9F0 == 6) {
                    state = 0x81;
                }
            }
            obj->unk76 = arg0->unk7A;
            obj->unk78 = obj->unk76;
            obj->unk7A = obj->unk76;
            obj->unk40 = (s16)(obj->unk76 + 0x4000) * 0.005493164f;
            obj->unk31C->unk4E = state;
            obj->unk83 = 0;
            obj->disable_run = 0;
        lbl:;
        } else {
            obj->unk65 = D_800C3E78 + 1;
            obj->unk5C = arg2;
            obj->unk101 |= arg3;
            obj->disable_run = 0;
        }
        obj->immune = arg6;
        obj->unk239 = arg7;
        obj->unk31C->unk27 = 0;
        arg0->unk232 = arg5;
        arg0->unk218 = 0;
        if (arg4 != 0) {
            func_1506160C(obj, 1, 0, 0, 0);
            obj->unk101 |= 0x40;
        }
    }
    if (flag != 0) {
        func_15060F28(arg0, 0);
    }
}

void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4) {
    vertex sp3C;
    struct127 *temp_s0;
    struct126 *temp_v0;
    struct255 *temp_v0_2;
    struct113 *temp_a1;
    register s32 temp_t8;

    temp_s0 = &D_800CC2D0[arg0->unk124];
    if ((temp_s0->disable_run == 0) && (temp_s0->stunned == 0)) {
        temp_v0 = temp_s0->unk31C;
        if (temp_v0 != 0) {
            if (temp_v0->unk6B != 0) {
                return;
            }
        }
            temp_s0->immune = 0xFF;
            temp_s0->y_velocity = arg1;
            temp_s0->gravity = arg2;
            temp_s0->unk83 = 0xFF;
            temp_s0->disable_run = 0xFF;
            temp_s0->unk76 = arg0->unk7A;
            arg0->unk232 = arg3;
            arg0->unk218 = 0;
            func_1505E650(temp_s0, 0x32, 1.2999999523162842f, 4.0f, 0.0f, 0.0f, 0);
            D_800CC2D0[arg0->unk124].unk31C->unk30 = 0x46;
            *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk28 = arg0->x_position;
            *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk2C = arg0->z_position;
            temp_v0_2 = arg0->unk1D4;
            if (temp_v0_2 != 0) {
                temp_t8 = arg4 << 6;
                if (arg4 != -1) {
block_1505327C:
                temp_a1 = (struct113 *) ((u8 *) temp_v0_2 + temp_t8);
                func_15043FF0((vertex *) ((u8 *) &sp3C - 8), temp_a1);
                *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk28 = ((vertex *) ((u8 *) &sp3C - 8))->x;
                *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk2C = ((vertex *) ((u8 *) &sp3C - 8))->z;
                }
            }
            D_800CC2D0[arg0->unk124].unk31C->unk27 = 1;
    }
}
