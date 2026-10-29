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

#pragma GLOBAL_ASM("asm/nonmatchings/game_43D00/func_150169A0.s")

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
