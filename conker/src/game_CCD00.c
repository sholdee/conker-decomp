#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_CCD00/func_1509F850.s")

typedef struct {
    u16 unk0;
    u16 unk2;
} Node_1509FE0C;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC[1];
} Arg_1509FE0C;

extern s8 D_800E0BB1;
extern u16 D_800BE930[];
extern u16 D_800BE918[][3];
extern s32 D_800D3840;
extern u8 D_800D2E40;
Node_1509FE0C *func_1509B704(s16 arg0);
s32 func_151E5F64(s32 arg0);

s32 func_1509FE0C(s32 arg0, s32 arg1, Arg_1509FE0C *arg2) {
    s32 pa0;
    s32 pa1;
    s32 pa2;
    s32 i;
    s32 sum;
    s32 count;
    s32 j;
    s32 thresh;
    s32 mask;
    Node_1509FE0C *node;

    switch (arg1) {
    case 26:
        return ((u8 *)(arg0 >> 3))[(s32)D_800D2E4C] & (1 << (arg0 & 7));
    case 27:
        return func_1509B704(arg0)->unk2;
    case 28:
        return D_800BE9F0;
    case 29:
        mask = 0;
        for (j = 0; j < D_8008FD8C; j++) {
            if (D_80087270[j] != 10) {
                mask |= 1 << j;
            }
        }
        mask &= ~D_800D18A0;
        return mask;
    case 30:
        return func_151E5F64(arg2->unk8);
    case 31:
        return D_800BE9F8;
    case 32:
        return (s8)D_800BE3DF;
    case 33:
        return D_800BE3E0;
    case 34:
        sum = 0;
        for (i = 0; i < arg2->unk8; i++) {
            sum += arg2->unkC[i];
        }
        thresh = func_150ADA68() * sum;
        sum = 0;
        for (i = 0; i < arg2->unk8; i++) {
            sum += arg2->unkC[i];
            if (sum >= thresh && arg2->unkC[i] != 0) {
                return i;
            }
        }
        return arg2->unk8 - 1;
    case 35:
        return D_8008FD90;
    case 36:
        return D_800E0BB1;
    case 37:
        return D_8008FDBC & 0x10;
    case 38:
        return D_8008FD8C;
    case 39:
        return D_800BE930[arg2->unk8];
    case 40:
        return D_800BE918[arg2->unk8][0];
    case 41:
        return D_800E0B94 == 2;
    case 42:
        return arg2->unk8 & D_800E9D00;
    case 43:
        node = func_1509B704(arg0);
        if (node != NULL) {
            return (node->unk0 & 0x2000) == 0;
        }
        return 0;
    case 44:
        return func_1517EFAC(D_800D3840 == 3 ? arg2->unk8 : 0);
    case 45:
        return D_800D2E40;
    }
    return 0;
}

