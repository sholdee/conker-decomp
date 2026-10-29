#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
} Node_1509FE0C;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} Arg_1509F850;

typedef struct {
    u8 unk0;
    u8 unk1;
} Struct_80087410;

extern Struct_80087410 D_80087410;
extern s32 D_800D2E48;
extern s32 D_800D2F58;
extern u8 D_800D2E69;
extern s32 D_800D3000;
extern s32 D_800D3004;
extern s32 D_800D3840;

Node_1509FE0C *func_1509B704(s16 arg0);
void func_150AE280(void);
void func_1509B5AC(s32 arg0, s32 arg1);
void func_1501C730(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_15172E7C(void *arg0, s32 arg1, s32 arg2);
void func_151E53E8(s32 arg0);
void func_1517EE40(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_151E8214(void);
void func_151E4DC4(void);
void func_15042E3C(char *arg0, s32 arg1);

s32 func_1509F850(s32 arg0, s32 arg1, Arg_1509F850 *arg2) {
    Node_1509FE0C *node2;
    Node_1509FE0C *node3;
    Node_1509FE0C *node5;
    Node_1509FE0C *node4;
    Node_1509FE0C *node11;
    s32 pad0;
    s32 i;

    switch (arg1) {
    case 2:
        node2 = func_1509B704(arg0);
        ((u8 *)(arg0 >> 3))[(s32)D_800D2E4C] &= 7 & (1 << (arg0 & 7));
        D_800D2F58 = 1;
        D_800D2E43 = 1;
        if (node2 == NULL) {
            return 0;
        }
        node2->unk0 |= 0x8000;
        return 1;
    case 1:
        if (D_80087410.unk1) {
            D_80087410.unk1 = 0;
            {
                s32 pad;
                return 0;
            }
        }
        ((u8 *)(arg0 >> 3))[(s32)D_800D2E4C] |= (1 << (arg0 & 7));
        D_800D2F58 = 1;
        D_800D2E43 = 1;
    case 3:
        node3 = func_1509B704(arg0);
        if (node3 == NULL) {
            return 0;
        }
        node3->unk0 |= 0x8000;
        if (arg0 == D_800D2E48) {
            func_150AE280();
            return 0;
        }
        return 1;
    case 5:
        node5 = func_1509B704(arg0);
        if (node5 == NULL) {
            return 0;
        }
        bzero((u8 *)node5 + node5->unkA, node5->unk6);
        return 0;
    case 4:
        node4 = func_1509B704(arg0);
        if (node4 != NULL) {
            node4->unk0 |= 0x4000;
            return 1;
        }
        break;
    case 6:
        func_1509B5AC(arg0, 0);
        return 0;
    case 7:
        if (D_800D3840 == 4) {
            func_1501C730(2, arg2->unk8, arg2->unkC, 0, 0);
            return 0;
        }
        break;
    case 8:
        if (D_800D3840 == 4) {
            func_15172E7C(D_800CC2D0, arg2->unk8 & 0xFFF, arg2->unkC & 0xFFF);
            return 0;
        }
        break;
    case 9:
        D_800D2E69 = 1;
        if (arg2->unk8) {
            D_800D2E60[arg2->unkC >> 3] |= (1 << (arg2->unkC & 7));
        } else {
            D_800D2E60[arg2->unkC >> 3] &= 7 & (1 << (arg2->unkC & 7));
        }
        D_800D2E43 = 1;
        break;
    case 10:
        func_151E53E8(arg2->unk8);
        return 0;
    case 11:
        node11 = func_1509B704(arg0);
        ((u8 *)(arg0 >> 3))[(s32)D_800D2E4C] |= (1 << (arg0 & 7));
        D_800D2F58 = 1;
        D_800D2E43 = 1;
        if (node11 == NULL) {
            return 0;
        }
        node11->unk0 |= 0x8000;
        func_151E53E8(0);
        func_150AE280();
        return 0;
    case 12:
        if (arg2->unk8 < 0) {
            pad0 = osGetCount();
            D_800D3000 = pad0;
            D_800D3004 = pad0;
            break;
        }
        osGetCount();
        D_800D3000 = osGetCount();
        return 0;
    case 13:
        D_800DDE38 = 1;
        break;
    case 14:
        D_800E9D00 |= arg2->unk8;
        break;
    case 15:
        if (arg2->unkC == 0) {
            D_800D2120 &= ~(1 << arg2->unk8);
        } else {
            D_800D2124 &= ~(1 << arg2->unk8);
        }
        break;
    case 16:
        if (arg2->unkC == 0) {
            D_800D2120 |= (1 << arg2->unk8);
        } else {
            D_800D2124 |= (1 << arg2->unk8);
        }
        break;
    case 17:
        func_1517EE40(0, 0, 0, 0, 1, 0);
        return 0;
    case 18:
        for (i = 0; i <= D_80082FA0; i++) {
            func_1517EE40(arg2->unk8, arg2->unkC, arg2->unk10, arg2->unk14, arg2->unk18, i);
        }
        break;
    case 19:
        D_800BE3DF = arg2->unk8;
        D_800BE3E0 = arg2->unkC & 0xFF;
        *(s32 *)&D_800BE9F4 = (s8)D_800BE3DF;
        break;
    case 20:
        func_151E8214();
        return 0;
    case 21:
        func_151E4DC4();
        return 0;
    case 22:
        func_15042E3C("%d", arg2->unk8);
        return 0;
    case 23:
        if (arg2->unk8) {
            D_800D2E44 = 1;
        } else {
            D_800D2E44 = 0;
        }
        break;
    }
    return 0;
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC[1];
} Arg_1509FE0C;

extern s8 D_800E0BB1;
extern u16 D_800BE930[];
extern u16 D_800BE918[][3];
extern u8 D_800D2E40;
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

