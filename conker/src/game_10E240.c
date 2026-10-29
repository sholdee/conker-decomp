#include <ultra64.h>
/* functions.h declares func_1509BFB0 void; it ends by tail-dispatching through D_800884D4
 * via jalr and leaves that result in v0. Declaring it s32 matches the caller's regalloc. */
#define func_1509BFB0 func_1509BFB0_hdr
#include "functions.h"
#undef func_1509BFB0
s32 func_1509BFB0(s32 num, s32, s32, ...);
#include "variables.h"

extern u8 D_800A0FE0[];
extern void *func_151149AC(u8);
extern void func_1510F800(s32);


s32 func_1509BE40(s32, s32, s32, s32);

void func_150E0D90(struct108 *arg0) {
    if (arg0->unk23E == 9) {
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 1);
        }
        if ((arg0->unk2C != 0x100) && (arg0->unk6C8 == 0)) {
            if (func_15123934(arg0, 8, 0, 0, 0) != 0) {
                arg0->unk84 |= 0x300000;
                arg0->unk84 &= ~4;
                arg0->unk1B4 = 1;
                arg0->unk1E0 = 3;
                func_15124B18(arg0);
            }
            arg0->unk348 = 125.0f;
            arg0->unk34C = 125.0f;
            arg0->unk374 = 220.0f;
            arg0->unk190 = 30.0f;
        } else {
            arg0->unk190 = 0.0f;
        }
    } else {
        if ((arg0->unk2C == 8) && (arg0->unk6C8 == 0) && (func_151239CC(arg0, 0) != 0)) {
            arg0->unk34C = 110.0f;
            arg0->unk348 = 110.0f;
            arg0->unk374 = 300.0f;
        }
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 0);
        }
    }
    if (D_800BE9F0 == 0x2F) {
        arg0->unk84 &= 0x7FFFFFFF;
    } else if (D_800BE9F0 == 0x1E) {
        if (func_1509BE40(1, 0x4042, 6, 0x9000) != 0) {
            arg0->unk84 &= 0x7FFFFFFF;
        } else {
            arg0->unk84 |= 0x80000000;
        }
        if (func_1509BE40(1, 0x4044, 6, 0x2000) != 0) {
            arg0->unk84 |= 0x20001010;
            arg0->unk84 &= ~8;
        } else {
            arg0->unk84 &= 0xDFFFEFEF;
            arg0->unk84 |= 8;
        }
    }
    if (D_800BE9F0 == 0x1B) {
        if (func_1509BE40(1, 0x406E, 6, 0x9000) != 0) {
            arg0->unk84 |= 0x200;
        } else {
            arg0->unk84 &= ~0x200;
        }
        if (func_1509BE40(1, 0x406F, 6, 0x9000) != 0) {
            arg0->unk84 |= 0x80000000;
        } else {
            arg0->unk84 &= 0x7FFFFFFF;
        }
    }
}

struct260 *func_150E1060(u8 arg0, u8 arg1, s32 arg2) {
    struct260 *temp_v0;
    struct {
        u8 unk0;
        u8 pad1;
        u8 pad2;
        u8 pad3;
        void *unk4;
        s32 unk8;
        f32 unkC;
    } sp34;

    if ((s32)arg0 < 0) {
        return NULL;
    }

    if (arg0 >= 2) {
        return NULL;
    }

    sp34.unk0 = arg0;
    sp34.unk4 = func_151149AC(D_800A0FE0[arg0]);
    if (sp34.unk4 == NULL) {
        return NULL;
    }

    func_1510F800(0);
    sp34.unk8 = func_1510FD20(*(s16 *)((s32)sp34.unk4 + 0x10), *(s16 *)((s32)sp34.unk4 + 0x14));
    sp34.unkC = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x1F, -1, 0, 0x1F, (struct37 *)0x10, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x10);
    }
    return temp_v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_10E240/func_150E114C.s")
