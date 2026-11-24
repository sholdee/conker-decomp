#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0_43D00
#include "functions.h"
#undef func_1502B7F0
#include "variables.h"


typedef struct {
    u8 pad0[0x4];
    u8 unk4;
    u8 pad5[0x2B];
} Struct43D00;

extern s32 func_1502B7F0(s32 *, s32, s32, s32);
extern u8 D_800BE590;
extern u16 D_800BE598[];
extern s32 D_800BE5A8[];
extern u8 D_800D2100;

void func_15016850(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 sp50;
    s32 id;

    D_800BE590 = 0;
    sp50 = 0;
    for (i = 0; i < D_800D2100; i++) {
        id = ((Struct43D00 *)D_800D20FC)[i].unk4;
        found = 0;
        for (j = 0; j < D_800BE590; j++) {
            if (id == D_800BE598[j]) {
                found = 1;
                break;
            }
        }
        if (found) {
        } else {
            if (id < 0xBB) {
                func_1502B7F0(&sp50, 2, 0x12, id);
            }
            if (sp50 != 0) {
                D_800BE5A8[D_800BE590] = sp50;
                D_800BE598[D_800BE590] = id;
                D_800BE590++;
            }
        }
    }
}

typedef struct {
    u8 unk0[2];
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 padF;
    u8 unk10;
    u8 pad11[7];
    s32 unk18;
    u8 pad1C[4];
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 pad29[7];
} Slot43D00;

extern u8 D_1A33F0[];
extern u8 D_1A34E0[];
extern u8 D_1A37E0[];
extern u8 D_800D2101;
extern u8 *D_800D210C;
extern s32 func_1502B9B4(s32, s32, ...);
extern u32 func_1502B8E0(void *, s32, s32, s32, ...);
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15017114(s32 arg0);
extern u8 func_15083DA0(void);
extern void func_1508295C(s32 arg0, s32 arg1, s32 arg2);
extern void func_15172CA8(s32 arg0);

#define SLOTS ((Slot43D00 *)D_800D20FC)

void func_150169A0(s32 arg0) {
    s32 j;
    u8 *end;
    u8 *start;
    u32 size;
    struct127 *obj;
    s32 n;
    s32 i;

    func_15004F30();
    switch (arg0) {
    case 26:
    case 36:
    case 43:
    case 45:
    case 48:
    case 51:
    case 52:
    case 63:
        start = D_1A34E0;
        end = D_1A37E0;
        break;
    default:
        start = D_1A33F0;
        end = D_1A34E0;
        break;
    }
    size = end - start;
    D_800D20FC = allocate_memory(func_1502B9B4(2, 0xE, arg0) + size, 1, 2, 0);
    func_10004514((s32)start, D_800D20FC, size, 1);
    D_800D2100 = size / 0x30;
    D_800D2101 = D_800D2100;
    D_800D2100 += func_1502B8E0(D_800D2100 + SLOTS, 0, 2, 0xE, arg0) / 0x30;
    func_15017114(arg0);
    n = D_800D2100;
    D_800D210C = allocate_memory(n, 1, 0, 0);
    size = n * 2;
    *(s16 **)&D_800D2110 = allocate_memory(size, 1, 0, 0);
    bzero(D_800D210C, n);
    bzero(*(s16 **)&D_800D2110, size);
    D_800D1548 = 0;
lbl:
    for (i = 0; i < D_8008FD8C; i++) {
        if ((1 << i) & (u16)D_800D2340) {
            s32 type = func_150859AC(i, 1);

            SLOTS[i].unk2 = 0;
            switch (type) {
            case 0:
                if (D_800BE9F0 == 0x36 || D_800BE9F0 == 0x3A || D_800BE9F0 == 0x40) {
                    SLOTS[i].unk4 = 0x96;
                } else {
                    SLOTS[i].unk4 = 0;
                }
                break;
            case 16:
                SLOTS[i].unk4 = 0x96;
                break;
            case 1:
                SLOTS[i].unk4 = 0x3B;
                break;
            case 2:
                SLOTS[i].unk4 = 0x98;
                break;
            case 3:
                SLOTS[i].unk4 = 0x75;
                break;
            case 4:
                SLOTS[i].unk4 = 0x80;
                break;
            case 37:
                SLOTS[i].unk4 = 0xB0;
                SLOTS[i].unk24 = 0.6000000238f;
                SLOTS[i].unk20 = SLOTS[i].unk24;
                break;
            case 38:
                SLOTS[i].unk4 = 0xB1;
                SLOTS[i].unk24 = 0.6000000238f;
                SLOTS[i].unk20 = SLOTS[i].unk24;
                break;
            case 39:
                SLOTS[i].unk4 = 0xB2;
                break;
            case 40:
                SLOTS[i].unk4 = 0xB4;
                break;
            case 33:
                SLOTS[i].unk4 = 0x9F;
                break;
            case 34:
                SLOTS[i].unk4 = 0xA0;
                break;
            case 11:
                SLOTS[i].unk4 = 0x88;
                break;
            case 35:
                SLOTS[i].unk4 = 0x9B;
                SLOTS[i].unk3 = 0x15;
                break;
            case 36:
                SLOTS[i].unk4 = 0x9A;
                SLOTS[i].unk3 = 0x15;
                break;
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
                SLOTS[i].unk4 = 0x9C;
                break;
            case 29:
            case 30:
            case 31:
            case 32:
                SLOTS[i].unk4 = 0x9D;
                break;
            case 12:
            case 13:
            case 14:
            case 15:
                SLOTS[i].unk4 = 0x90;
                if (i < D_8008FD90) {
                    SLOTS[i].unk3 = 1;
                } else {
                    SLOTS[i].unk3 = 0xC;
                }
                SLOTS[i].unk18 |= 4;
                SLOTS[i].unkD = 0xD;
                break;
            case 8:
                SLOTS[i].unk4 = 0x77;
                SLOTS[i].unk3 = 0x1E;
                SLOTS[i].unk10 = 0xAA;
                if (i >= D_8008FD90) {
                    SLOTS[i].unkD = 3;
                } else {
                    SLOTS[i].unkD = 1;
                }
                SLOTS[i].unk18 |= 0x180000;
                SLOTS[i].unkE |= 0x80;
                break;
            case 10:
                SLOTS[i].unk4 = 0x77;
                SLOTS[i].unk3 = 0x25;
                SLOTS[i].unk10 = 0xA8;
                SLOTS[i].unkD = 3;
                SLOTS[i].unk18 |= 0x100000;
                break;
            case 9:
                SLOTS[i].unk4 = 0x53;
                SLOTS[i].unk3 = 0x1F;
                SLOTS[i].unk10 = 0xA8;
                SLOTS[i].unkD = 2;
                SLOTS[i].unk20 = 0.6999999881f;
                SLOTS[i].unk24 = 0.6999999881f;
                SLOTS[i].unk18 |= 1;
                break;
            }
            if (D_800BE9F0 == 0x33 || D_800BE9F0 == 0x32) {
                SLOTS[i].unk18 |= 0x4000000;
            }
            obj = D_800CC2D0;
            obj += i | 0;
            SLOTS[i].unk6 = obj->x_position;
            SLOTS[i].unk8 = obj->y_position;
            SLOTS[i].unkA = obj->z_position;
            SLOTS[i].unkC = obj->unk76 >> 8;
            continue;
        }
        SLOTS[i].unk2 = 1;
    }
    func_15016850();
    for (j = 0; j < D_800D2100; j++) {
        if (SLOTS[j].unk28 == 0) {
            SLOTS[j].unk28 = func_15083DA0();
        }
    }
    func_1508295C(arg0, 1, 1);
    func_15172CA8(0);
    func_1508295C(arg0, 1, 0);
}

extern u8 D_800D2101;
extern void *allocate_memory(s32, s32, s32, s32);

typedef struct {
    s32 offset;
    s32 val;
} Entry43D00;

void func_15017114(s32 arg0) {
    s16 n;
    s32 sp40;
    s32 i;
    u32 m;

    n = D_800D2100;
    sp40 = 0;
    D_800D2104 = allocate_memory(n * 4, 1, 0, 0);
    m = n;
    *(u8 **)&D_800D2108 = allocate_memory(m, 1, 0, 0);
    bzero(D_800D2104, n * 4);
    bzero(*(u8 **)&D_800D2108, m);
    for (i = 0; i < D_8008FD8C; i++) {
        D_800D2104[i] = (s32)allocate_memory(0x10, 1, 2, 0);
        bzero((void *)D_800D2104[i], 0x10);
        (*(u8 **)&D_800D2108)[i] = 2;
    }
    if (func_1502B7F0(&sp40, 2, 0x10, arg0) != 0) {
        for (i = D_800D2101; i < D_800D2100; i++) {
            if (((Entry43D00 *)sp40)[i - D_800D2101].offset != 0) {
                (*(u8 **)&D_800D2108)[i] = ((Entry43D00 *)sp40)[i - D_800D2101].val;
                D_800D2104[i] = ((Entry43D00 *)sp40)[i - D_800D2101].offset + sp40;
            }
        }
    }
}
