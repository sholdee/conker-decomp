#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern struct127 *func_1505EEF4(s32);
extern void func_15022190(s16, s16, s16, f32);
extern void func_150221E8(s16, s16, s16, f32);

typedef struct {
    s32 unk0;
    s32 unk4;
    union { s32 w; struct { s16 hi; s16 lo; } h; } unk8;
    union { s32 w; struct { s16 hi; s16 lo; } h; } unkC;
    union { s32 w; struct { s16 hi; s16 lo; } h; } unk10;
    union { s32 w; struct { s16 hi; s16 lo; } h; } unk14;
    union { s32 w; struct { s16 hi; s16 lo; } h; } unk18;
    s32 unk1C;
} Struct_1509C440;

extern void func_1501D348(s32, s32, s32, s32, s32);
extern s32 D_800D3840;
extern u8 D_800C35C2;
extern u8 D_800C35C3;
extern u8 D_800C3C99;
extern u8 D_800C3C9C;
void func_1509C8FC(s32 arg0, s32 arg1);

s32 func_1509C440(s32 arg0, s32 arg1, Struct_1509C440 *arg2) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 temp;

    switch (arg1) {
    case 1:
        if (D_800C35EA == 0) {
            sp3C = D_800BE9F0;
            if (D_800D3840 == 6) {
                func_15022190(arg2->unk8.h.lo, arg2->unkC.h.lo, arg2->unk10.h.lo, arg2->unk14.w * 1.52587890625e-05f);
            } else if (D_800D3840 == 3) {
                if ((arg2->unk8.w & 0xF000) == 0x2000) {
                    func_1509C8FC(arg2->unk8.w, 0);
                } else {
                    sp3C = arg2->unk8.w;
                }
            }
            func_1501D348(sp3C, arg0, 0, 0, 0);
            return 1;
        }
        return 0;
    case 4:
        if (D_800C35EA == 0) {
            if (D_800D3840 == 6) {
                func_15022190(arg2->unk8.h.lo, arg2->unkC.h.lo, arg2->unk10.h.lo, arg2->unk14.w * 1.52587890625e-05f);
            }
            sp38 = D_800BE9F0;
            D_800BE9F0 = 0x25;
            func_1501D348(D_800BE9F0, arg0, 0, 0, 0);
            D_800BE9F0 = sp38;
            return 1;
        }
        return 0;
    case 3:
        if (D_800C35EA == 0) {
            sp30 = arg2->unk8.w;
            sp2C = arg2->unkC.w;
            sp34 = D_800BE9F0;
            if (D_800D3840 == 8) {
                func_15022190(arg2->unk10.h.lo, arg2->unk14.h.lo, arg2->unk18.h.lo, arg2->unk1C * 1.52587890625e-05f);
            } else if (D_800D3840 == 5) {
                sp34 = arg2->unk10.w;
            }
            func_1501D348(sp34, arg0, 0, sp30, sp2C);
            return 1;
        }
        return 0;
    case 2:
        {
        s32 t;
        t = D_800BE9F0;
        D_800C35C2 = arg0 + 1;
        D_800C35C3 = t;
        D_800C3C99 = 1;
        if (D_800D3840 == 6) {
            func_150221E8(arg2->unk8.h.lo, arg2->unkC.h.lo, arg2->unk10.h.lo, arg2->unk14.w * 1.52587890625e-05f);
        } else if (D_800D3840 == 3) {
            if ((arg2->unk8.w & 0xF000) == 0x2000) {
                func_1509C8FC(arg2->unk8.w, 1);
            } else {
                D_800C35C3 = arg2->unk8.w;
            }
        }
        }
        break;
    case 5:
        D_800C35C2 = arg0 + 1;
        D_800C35C3 = 0x25;
        if (D_800D3840 == 6) {
            func_150221E8(arg2->unk8.h.lo, arg2->unkC.h.lo, arg2->unk10.h.lo, arg2->unk14.w * 1.52587890625e-05f);
        }
        break;
    case 6:
        D_800C3C9C = 1;
        break;
    }
}

extern u8 D_800C35E8;
extern u8 D_800C3C98;
extern u8 D_800C3672;

s32 func_1509C7C8(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg1) {
    case 1:
        if (D_800C35C4 != 0) {
            return 0;
        }
        if (D_800C35EA == 0) {
            return 1;
        }
        if (arg0 == D_800C3C98) {
            return 0;
        }
        return 1;
    case 2:
        if ((D_800C35EA == 1) && (arg0 == D_800C3C98)) {
            return 1;
        }
        return 0;
    case 3:
        if ((D_800C35EA == 1) && (arg0 == D_800C35E8)) {
            return 1;
        }
        return 0;
    case 4:
        return D_800C35EA != 1;
    case 5:
        return D_800C3672;
    case 6:
        if ((D_800C35EA == 1) && (arg0 == D_800C35E8)) {
            return 1;
        }
        return 0;
    }
}

void func_1509C8FC(s32 arg0, s32 arg1) {
    struct127 *temp_v1;
    struct108 *temp_v0;
    f32 temp_f0;

    temp_v1 = func_1505EEF4(arg0 & 0xFFF);
    if (temp_v1 != 0) {
        temp_v0 = D_800DBFF0;
        if (temp_v0->unk2C & 0x100) {
            temp_f0 = *(f32 *)((u8 *)temp_v0 + 0x760);
        } else {
            temp_f0 = temp_v0->unk37C;
        }

        if (arg1 != 0) {
            func_150221E8(temp_v1->x_position, temp_v1->y_position, temp_v1->z_position, temp_f0 + 180.0f);
        } else {
            func_15022190(temp_v1->x_position, temp_v1->y_position, temp_v1->z_position, temp_f0 + 180.0f);
        }
    }
}
