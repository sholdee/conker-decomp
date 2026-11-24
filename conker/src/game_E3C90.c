#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_1509BFB0(s32 num, s32, s32, ...);
s32 func_15123934(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_151239CC(struct108 *arg0, s32 arg1);
s32 func_1509BE40();
void func_151254F4(struct108 *arg0, s32 arg1);
extern s32 D_80088710;

void func_150B67E0(struct108 *arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 i;

    if (func_1509BE40(0, 0x503C, 0x1A) == 0) {
        if (func_15123934(arg0, arg0->unk2C, 0, 0, 0) != 0) {
            arg0->unk84 |= 0x20000;
            arg0->unk84 &= ~4;
            arg0->unk1B4 = 3;
            func_15124B18(arg0);
            arg0->unk23C = 1;
        }
        arg0->unk374 = 650.0f;
        arg0->unk348 = 450.0f;
        arg0->unk34C = 450.0f;
    } else {
        func_1509BFB0(0, 0x4000, 0);
        if ((arg0->unk2C == 1) && (func_151239CC(arg0, 0) != 0)) {
            arg0->unk1B4 = 2;
            func_15124B18(arg0);
        }
        if (func_1509BE40(1, 0x4024, 6, 0x2000) != 0) {
            D_80088710 = 0x9003;
        } else if (func_1509BE40(1, 0x4025, 6, 0x2000) != 0) {
            D_80088710 = 0x9009;
        } else if (func_1509BE40(1, 0x4026, 6, 0x2000) != 0) {
            D_80088710 = 0x900A;
        } else if (func_1509BE40(1, 0x4027, 6, 0x2000) != 0) {
            D_80088710 = 0x900B;
        }
        {
            s32 *p = &D_80088710;
            if (*p != 0x3E7) {
                func_1509BFB0(5, 0x4000, 4, 2, 0, *p & 0xFFF, 0, 0);
            }
        }
        for (i = 0; i < 3; i++) {
            if ((func_1509BE40(1, i + 0x400C, 6, 0x2000) != 0) || (func_1509BE40(1, 0x4014, 6, 0x2000) != 0)) {
                func_1509BFB0(1, 0x2000, 0x3B, 2);
            }
        }
        if (func_1509BE40(1, 0x4000, 6, 0x2000) != 0) {
            func_1509BFB0(2, 0x9000, 6, 1, 0x80000);
            func_1509BFB0(1, 0x9000, 0x10, 0x55);
        } else {
            func_1509BFB0(2, 0x9000, 6, 0, 0x80000);
            func_1509BFB0(1, 0x9000, 0x10, 0);
        }
    }
    temp_s0 = func_1509BE40(0, func_1509BE40(0, 0x200A, 0xB7) | 0x2000, 0xBC);
    temp_v0 = func_1509BE40(0, 0x2000, 0xBB);
    if ((temp_s0 != 0) && (temp_v0 != -1)) {
        arg0->unk5F0 |= 0x100;
    } else {
        arg0->unk5F0 &= ~0x100;
    }
    if ((temp_s0 != 0) && (temp_v0 != -1)) {
        if (func_15123934(arg0, arg0->unk2C, 0, 0, 3) != 0) {
            arg0->unk84 |= 0x100002;
            arg0->unk674 = 0.0f;
            func_151254F4(arg0, arg0->unk3D0->unk65 - 1);
            arg0->unk134 = 0;
        }
        arg0->unk374 = 650.0f;
        arg0->unk34C = 450.0f;
        arg0->unk348 = 450.0f;
        if (*arg0->unk36C & 4) {
            arg0->unk190 = 0.0f;
        } else {
            arg0->unk190 = 135.0f;
        }
        return;
    }
    if (func_151239CC(arg0, 3) != 0) {
        func_151254F4(arg0, arg0->unk23D);
        arg0->unk674 = 0.0f;
        arg0->unk190 = 0.0f;
    }
}
