#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct CB3D0_arg1 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

extern f32 D_800D2FC0[];
extern f32 D_800D2FD8[];
extern u8 D_800D2FEC[];
extern s32 D_800D3840;

void func_1509DF20(s32 arg0, struct CB3D0_arg1 *arg1) {
    if (arg1->unk4 == 1) {
        if (D_800D3840 == 3) {
            D_800D2FC0[arg1->unk8] = (f32)arg1->unkC * 0.0000152587890625f;
            D_800D2FD8[arg1->unk8] = (f32)arg1->unkC * 0.0000152587890625f;
            D_800D2FEC[arg1->unk8] = 1;
        }
    }
}

void func_1509DFB4(s32 arg0, s32 arg1) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_CB3D0/func_1509DFC4.s")

struct CB3D0_arg2 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

extern f32 func_150CF040(s32 arg0, s32 arg1);
extern s32 func_1508CA88(void);
extern s32 func_1508C9CC(void);
extern s32 func_1508C5B8(s32 arg0, s32 arg1);
extern s32 func_15089F9C(s32 arg0);
extern s32 func_1508E6D0(s32 arg0);
extern s32 func_150881CC(s32 arg0);
extern s32 func_15088218(s32 arg0);
extern s32 func_15088270(s32 arg0);
extern s32 func_150882B0(s32 arg0);
extern s32 func_150882E4(s32 arg0, s32 arg1);
extern s32 func_150887F8(void);
extern s32 func_1508B194(s32 arg0);
extern s8 D_800E0BD0;
extern s8 D_800E0BE0[];

s32 func_1509E3DC(s32 arg0, s32 arg1, struct CB3D0_arg2 *arg2) {
    struct127 *obj;
    s32 x;
    s32 z;

    switch (arg1) {
    case 0:
        return func_150CF040(arg2->unk8, arg2->unkC);
    case 1:
        x = arg2->unk8 - arg2->unk10;
        z = arg2->unkC - arg2->unk14;
        return sqrtf(x * x + z * z);
    case 2:
        return func_1508CA88();
    case 3:
        return func_1508C9CC();
    case 4:
        return func_1508C5B8(arg2->unk8, arg2->unkC);
    case 5:
        return D_800E0BD0;
    case 6:
        return func_15089F9C(arg2->unk8);
    case 7:
        return func_1508E6D0(arg2->unk8);
    case 8:
        return 0;
    case 9:
        return func_150881CC(arg2->unk8);
    case 10:
        return func_15088218(arg2->unk8);
    case 11:
        return func_15088270(arg2->unk8);
    case 12:
        return func_150882B0(arg2->unk8);
    case 13:
        return func_150882E4(arg2->unk8, arg2->unkC);
    case 14:
        return func_150887F8();
    case 15:
        return D_8008FDBC & 0x40;
    case 16:
        return D_800E0BE0[arg2->unk8];
    case 17:
        return func_1508B194(arg2->unk8);
    case 20:
    case 21:
        return D_800D2FD8[arg2->unk8] * 65536.0f;
    case 18:
        obj = func_15083E90((u8)arg2->unk8);
        if (obj != NULL) {
            x = obj->x_position;
            z = obj->z_position;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = func_15083E90((u8)arg2->unk8);
        if (obj != NULL) {
            return (s16)obj->y_position;
        }
        return 0;
    }
end:
    return 0;
}
