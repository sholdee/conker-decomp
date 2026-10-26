#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_10DD20/func_150E0870.s")

extern u16 D_800CC346;

void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
u8 func_150599C8(struct127 *arg0, u8 arg1, u16 arg2);
s32 func_150535F4(struct127 *arg0);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);

void func_150E0BE0(struct127 *arg0) {
    s32 temp_v0;
    u16 *temp_t1;

    arg0->unkB0 = 0xF;
    arg0->unk80 = 0xA;

    if (arg0->unk87 != 0) {
        temp_v0 = arg0->unk83;
        if (temp_v0 < 0x64) {
            temp_v0 = (arg0->unk83 = temp_v0 + (u32)D_800BE9A0);
            temp_v0 &= 0xFF;
        }
        if (temp_v0 < 5) {
        } else {
            temp_v0 = arg0->unk251;
            temp_t1 = &D_800CC346;
            if (temp_v0 != 3) {
                arg0->unk78 = *temp_t1;
                arg0->unk218 = 0;
                arg0->unk232 = 4;
                if (temp_v0 == 2) {
                    arg0->unk232 = 2;
                }
            }
        }
    } else {
        if (arg0->unk83 >= 0x3D) {
            D_800D1580 = 0xFF020133;
            D_800D154C = D_800CC2D0;
            func_1506E8D8();
            D_800D154C = arg0;
        }
        arg0->unk83 = 0;
    }

    if (arg0->unk232 == 4) {
        if (arg0->unk83 == 0) {
            arg0->unk78 = arg0->unk76;
        }
        func_150599C8(arg0, 3, arg0->unk78);
    }

    arg0->unk87 = 0;
    if (arg0->stunned == 0) {
        func_15056B08(arg0);
    } else {
        func_150585F0(arg0, 0.05000000074505806f);
    }

    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
    func_15059140(arg0);
    *(f32 *)((u8 *)arg0 + 0xBC) = arg0->x_position - arg0->old_x_position;
    arg0->unk148 = arg0->z_position - arg0->old_z_position;
    if (func_150535F4(arg0) == 0) {
        func_1502178C(arg0, 0, -1);
    }
}

