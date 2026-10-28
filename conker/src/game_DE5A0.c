#include <ultra64.h>
#include "functions.h"
#define D_800D1550 D_800D1550_hdr
#include "variables.h"
#undef D_800D1550
extern f32 D_800D1550;


void func_150B10F0(u8 *arg0) {
    f32 temp_fv0;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 temp_fv0_2;
    f32 temp_ft4;
    f32 temp_fv1;
    s32 temp_v1;

    *(f32 *)((u8 *)arg0 + 0x174) = (f32) *(f32 *)((u8 *)arg0 + 0x14);
    *(f32 *)((u8 *)arg0 + 0x178) = (f32) *(f32 *)((u8 *)arg0 + 0x1C);
    *(s8 *)((u8 *)arg0 + 0x125) = 0x64;
    *(f32 *)((u8 *)arg0 + 0x40) = (f32) ((f32) (s16) (*(u16 *)((u8 *)arg0 + 0x7A) + 0x4000) * 0.005493164f);
    if (func_1505A6F8(D_800CC2D0, (struct127 *)arg0) < 1100.0f) {
        temp_fv0 = func_1505A6F8((struct127 *)arg0, D_800CC2D0);
        temp_ft4 = temp_fv0;
        temp_fv1 = *(f32 *)((u8 *)arg0 + 0x18) - *(f32 *)((u8 *)D_800CC2D0 + 0x18);
        if (temp_fv1 < 0.0f) {
            temp_fv1 = 1000.0f;
        } else {
            temp_fv1 = 1050.0f - (temp_fv1 * 0.7f);
        }
        if ((temp_fv0 < temp_fv1) && (*(u8 *)((u8 *)D_800CC2D0 + 0xAD) == 1)) {
            temp_fv0_2 = 1100.0f - temp_ft4;
            temp_v1 = func_1505A630(*(f32 *)((u8 *)arg0 + 0x174) - *(f32 *)((u8 *)D_800CC2D0 + 0x14), *(f32 *)((u8 *)D_800CC2D0 + 0x1C) - *(f32 *)((u8 *)arg0 + 0x178), 0) & 0xFFFF;
            if (850.0f < temp_fv0_2) {
                D_800CC2E8[0] = *(f32 *)((u8 *)D_800CC2D0 + 0x18) - ((temp_fv0_2 - 820.0f) * 0.09f * D_800D1550);
            }
            func_1505A184(temp_v1 - 0x3000, (temp_fv0_2 * 0.045f) + 5.0f, 0.0f, &sp50, &sp4C, &sp48);
            *(f32 *)((u8 *)D_800CC2D0 + 0x164) = sp50;
            *(f32 *)((u8 *)D_800CC2D0 + 0x168) = sp4C;
        }
    }
    func_1505E650((struct127 *)arg0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_DE5A0/func_150B12FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_DE5A0/func_150B1484.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_DE5A0/func_150B17DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_DE5A0/func_150B19E0.s")
