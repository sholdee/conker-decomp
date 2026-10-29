#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800DF7D0;
extern u8 D_800DF7D1;
extern u8 D_800DF7D2;
extern u8 D_800DF7D3[];
extern u8 D_800DF9B3[];
extern u8 *D_800BE510;
extern u16 D_800BE528;

typedef struct {
    u8 pad0[0xC];
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 padF;
} struct_func_15012C84;


extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15012ED8(s32);
void func_15012C84(struct_func_15012C84 *arg0, u32 arg1);
extern struct_func_15012C84 *D_800B0E10[];
extern s32 D_800B0E40[];
extern s32 D_800B0E44;
extern s32 D_800B0E48;
extern s32 D_800B0E4C;
typedef struct { u8 c[3]; } struct_func_150127B0_rgb;
extern u8 D_800B0E38;
extern u8 ***D_800BE520;
extern u16 *D_800BE524;
extern u8 D_800BE564;

typedef struct {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x10];
    struct_func_15012C84 *unk28;
    u8 pad2C[0x74];
} struct_func_150127B0_a; /* size 0xA0 */

typedef struct {
    s32 unk0;
    u16 *unk4;
    s32 unk8;
} struct_func_150127B0_b; /* size 0xC */

void func_150127B0(void) {
    s32 total;
    s32 i;
    s32 j;
    s32 k;
    struct_func_150127B0_b *b;
    struct_func_15012C84 *src;

    D_800BE520 = NULL;
    if (D_800B0DF0->pad1C[5] != 0) {
        total = 0;
        if (D_800B0DF0->pad1C[5] & 1) {
            for (i = 0; i < 4; i++) {
                total += D_800B0E40[i];
            }
        }
        if (D_800B0DF0->pad1C[5] & 2) {
            for (i = 0; i < D_800DBEF0; i++) {
                total += ((struct_func_150127B0_a *)D_800DBEF4)[i].unk16;
            }
        }
        if ((D_800BE510 = allocate_memory(total * 3, 1, 0, 0)) == NULL) {
            (&D_800BE510)[1] = NULL;
            return;
        }
        D_800BE528 = 0;
        D_800BE564 = 0;
        if (D_800B0DF0->pad1C[5] & 1) {
            for (i = 0; i < 4; i++) {
                if (D_800B0E10[i] != NULL) {
                    (&D_800BE510)[i] = (u8 *)&((struct_func_150127B0_rgb *)D_800BE510)[D_800BE528];
                    func_15012C84(D_800B0E10[i], D_800B0E40[i]);
                } else {
                    (&D_800BE510)[i] = NULL;
                }
            }
        }
        if (D_800B0DF0->pad1C[5] & 2) {
            D_800BE524 = allocate_memory(D_800DBEF0 * 2, 1, 0, 0);
            for (i = 0; i < D_800DBEF0; i++) {
                if (((struct_func_150127B0_a *)D_800DBEF4)[i].unk16 != 0) {
                    D_800BE524[i] = D_800BE528;
                    func_15012C84(((struct_func_150127B0_a *)D_800DBEF4)[i].unk28, ((struct_func_150127B0_a *)D_800DBEF4)[i].unk16);
                } else {
                    D_800BE524[i] = 0xFFFF;
                }
            }
        }
        if (D_800B0DF0->pad1C[5] & 4) {
            D_800BE520 = allocate_memory(D_800B0E38 * 4, 1, 0, 0);
            bzero(D_800BE520, D_800B0E38 * 4);
            for (i = 0; i < D_800B0E38; i++) {
                if ((*(u8 **)&D_800B0E34)[i] != 0) {
                    D_800BE520[i] = allocate_memory((*(u8 **)&D_800B0E34)[i] * 4, 1, 0, 0);
                    b = (*(struct_func_150127B0_b ***)&D_800B0E30)[i];
                    for (j = 0; j < (*(u8 **)&D_800B0E34)[i]; j++) {
                        D_800BE520[i][j] = allocate_memory(b[j].unk8 * 3, 1, 0, 0);
                        if (i < 4) {
                            src = D_800B0E10[i];
                        } else {
                            src = ((struct_func_150127B0_a *)D_800DBEF4)[i - 4].unk28;
                        }
                        for (k = 0; k < b[j].unk8; k++) {
                            D_800BE520[i][j][k * 3 + 0] = src[b[j].unk4[k]].unkC;
                            D_800BE520[i][j][k * 3 + 1] = src[b[j].unk4[k]].unkD;
                            D_800BE520[i][j][k * 3 + 2] = src[b[j].unk4[k]].unkE;
                        }
                    }
                } else {
                }
            }
        }
        func_15012ED8(D_800B0E00[0]);
    }
}

void func_15012C84(struct_func_15012C84 *arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < arg1; i++) {
        D_800BE510[D_800BE528 * 3] = arg0[i].unkC;
        D_800BE510[(D_800BE528 * 3) + 1] = arg0[i].unkD;
        D_800BE510[(D_800BE528 * 3) + 2] = arg0[i].unkE;
        D_800BE528++;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_3FC60/func_15012ED8.s")

void func_15012F90(void) {
    s32 i;

    D_800DF7D0 = 0;
    D_800DF7D1 = 0;
    for (D_800DF7D2 = 0, i = 0;;) {
        i += 4;
        D_800DF7D3[i - 3] = 0;
        D_800DF7D3[i - 2] = 0;
        D_800DF7D3[i - 1] = 0;
        D_800DF7D3[i - 4] = 0;
        if (&D_800DF9B3[0] != &D_800DF7D3[i]) {
            continue;
        }
        break;
    }
}
