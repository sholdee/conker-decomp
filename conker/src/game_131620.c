#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u8  pad0[0x10];
    s32 unk10;
    s32 unk14;
    s16 unk18;
    u8  unk1A;
    u8  unk1B;
    u8  unk1C;
} struct_15104170;

s32 func_15167A68(s32, s32, s32, s32, s32, s32);

struct_15104170 *func_15104170(s32 arg0, s32 arg1, s32 arg2) {
    struct_15104170 *ret;

    ret = (struct_15104170 *)func_15167A68(0x64, 0, 0x20, 0, 0xFF, 1);
    if (ret != 0) {
        ret->unk18 = 0xF;
        ret->unk1A = 0;
        ret->unk1B = 0;
        ret->unk10 = arg1;
        ret->unk14 = arg2;
        ret->unk1C = arg0;
    }
}

typedef struct {
    u8  pad0[0x10];
    u8 *unk10;
    u8 *unk14;
    u16 unk18;
    u8  unk1A;
    u8  unk1B;
    u8  unk1C;
} struct_151041E4;

extern u8 D_800BEA0C;
void func_1000E2F4(s32 arg0);
void func_151254F4(void *arg0, s32 arg1);
void func_151D66F0(s32 arg0, s32 arg1);

void func_151041E4(struct_151041E4 *arg0) {
    s32 var_a0;
    s32 kind;
    u8 *p2D0;
    f32 y;

    switch (arg0->unk1A) {
    case 0:
        p2D0 = *(u8 **)(arg0->unk10 + 0x2D0);
        y = *(f32 *)(p2D0 + 8);
        kind = arg0->unk1C;
        var_a0 = 0;
        if (kind == 0) {
            if (y >= 19.0f) {
                var_a0 = 1;
            }
        } else if (kind == 1) {
            if (D_800BE9E4 < arg0->unk18) {
                arg0->unk18 -= D_800BE9E4;
            } else {
                var_a0 = 1;
            }
        }
        if (var_a0) {
            u8 *sp34;
            u8 *sp30;

            sp34 = arg0->unk14;
            sp30 = arg0->unk10;
            arg0->unk1A = 1;
            D_800BEA0C = 1;
            func_100176C4();
            func_10010F30(0x5B2, 0x7FFF, 0, 0, 0);
            func_10010F30(0x5B2, 0x7FFF, 0x7F, -0x64, 0);
            func_10010F30(0x5B2, 0x7FFF, 0, 0, 0);
            func_10010F30(0x5B2, 0x7FFF, 0x7F, -0x64, 0);
            func_151D66F0(0xD2, 2);
            func_151254F4(*(u8 **)(sp30 + 0x318), sp34[0x13F]);
            *(f32 *)(*(u8 **)(sp30 + 0x318) + 0x674) = 0.0f;
        lbl0: ;
        }
        break;
    case 1: {
        s32 v;

        v = arg0->unk1B;
        v += D_800BEA08 * 10;
        if (v >= 0xFF) {
            arg0->unk1B = 0xFF;
            arg0->unk1A = 2;
            arg0->unk18 = 0xB4;
        } else {
            arg0->unk1B = v;
        lbl1: ;
        }
    }
        break;
    case 2: {
        u8 *sp28;

        if (D_800BEA08 < arg0->unk18) {
            arg0->unk18 -= D_800BEA08;
        } else {
            sp28 = arg0->unk10;
            D_800BEA0C = 0;
            func_1000E2F4(0);
            func_151254F4(*(u8 **)(sp28 + 0x318), sp28[0x13F]);
            *(f32 *)(*(u8 **)(sp28 + 0x318) + 0x674) = 0.0f;
            arg0->unk1A = 3;
        lbl2: ;
        }
    }
        break;
    case 3: {
        s32 sp24;

        sp24 = arg0->unk1B;
        sp24 -= D_800BEA08 * 10;
        if (sp24 <= 0) {
            func_1516972C((struct102 *)arg0);
            func_151D66F0(0, 0);
        } else {
            func_151D66F0((sp24 * 0xD2) >> 8, 2);
            arg0->unk1B = sp24;
        }
    }
        break;
    }
}


s32 func_1517F08C(s32, s32, s32, s32, s32, s32);

s32 func_1510448C(s32 arg0, s32 arg1, s32 arg2) {
    s32 *p1 = &arg1;
    s32 *p2 = &arg2;
    s32 v0;
    if ((s16)*p2 != 0 || (v0 = *(u8 *)(*p1 + 0x1B)) == 0) {
        return arg0;
    }
    return func_1517F08C(arg0, ((v0 * 0x3F) >> 8), 0, 0, 0, *((s16 *)&arg2 + 1));
}

extern struct126 *D_800CC5EC;

u8 func_151044F4(void) {
    if (D_800CC5EC != 0) {
        return D_800CC5EC->matrix_physics;
    }
    return 0;
}
