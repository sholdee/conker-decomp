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

extern f32 D_8009F8C4;
extern f32 D_8009F8C8;

struct127 *func_1505EEB0(s32 state, s32 *arg1);
void func_15056B08(struct127 *arg0);
void func_1505A770(struct127 *arg0);

void func_150B17DC(struct127 *arg0) {
    s32 sp3C;
    f32 angle;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    struct127 *other;

    other = func_1505EEB0(0xA, &sp3C);
    func_15056B08(arg0);
    if (arg0->unk1E5 == 2) {
        arg0->x_position = other->x_position;
        arg0->z_position = other->z_position;
        arg0->y_position = other->y_position + ((f32)(arg0->unk21C / 5) + 160.0f);
        arg0->y_velocity = 0.0f;
        arg0->gravity = D_8009F8C4;
        arg0->unk7E = 0;
    } else {
        angle = (f32)(u32)arg0->unk7E;
        angle *= D_8009F8C8;
        other->unkB8 = func_150AD78C(angle) * 6.0f;
        func_1505A770(arg0);
        other->y_position = arg0->y_position - 170.0f;
        other->unk13C = D_800C3E78 + 100;
        func_1505A184(other->unk7A, other->unkB8 * -7.0f, 0.0f, &sp34, &sp30, &sp2C);
        other->x_position = arg0->x_position + sp34;
        other->z_position = arg0->z_position + sp30;
        other->unk7A = other->unk7A + (D_800BE9A0 << 8);
        arg0->unk7E = arg0->unk7E + D_800BE9A0;
        other->in_water = 0;
        other->xz_velocity = 0.0f;
        other->unk16C = 0.0f;
        other->unk170 = 0.0f;
    }
    arg0->unk7A = func_1505A630(D_800CC2D0[0].x_position - arg0->x_position, arg0->z_position - D_800CC2D0[0].z_position, 0);
    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_DE5A0/func_150B19E0.s")
