#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800DF7D1;
extern u8 D_800DF7D2;
extern u8 D_800DF7D3[];
extern u8 D_800DF9B3[];
extern u16 D_800BE528;


extern u8 D_800BE530[];
extern s16 D_800BE550[];
extern s32 D_800B0E44;
extern s32 D_800B0E48;
extern s32 D_800B0E4C;
extern u8 D_800BE564;

void func_150127B0(void) {
    s32 total;
    s32 i;
    s32 j;
    s32 k;
    VtxColorGroup *b;
    Vtx *src;

    D_800BE520 = NULL;
    if (D_800B0DF0->vtxColorFlags != 0) {
        total = 0;
        if (D_800B0DF0->vtxColorFlags & 1) {
            for (i = 0; i < 4; i++) {
                total += D_800B0E40[i];
            }
        }
        if (D_800B0DF0->vtxColorFlags & 2) {
            for (i = 0; i < gWorldObjectCount; i++) {
                total += db_Object_array[i].numVertices;
            }
        }
        if ((D_800BE510 = allocate_memory(total * 3, 1, 0, 0)) == NULL) {
            (&D_800BE510)[1] = NULL;
            return;
        }
        D_800BE528 = 0;
        D_800BE564 = 0;
        if (D_800B0DF0->vtxColorFlags & 1) {
            for (i = 0; i < 4; i++) {
                if (D_800B0E10[i] != NULL) {
                    (&D_800BE510)[i] = (void *)&((Color24 *)D_800BE510)[D_800BE528]; // matching: indexing through the Color24 view keeps IDO's addu operand order
                    func_15012C84(D_800B0E10[i], D_800B0E40[i]);
                } else {
                    (&D_800BE510)[i] = NULL;
                }
            }
        }
        if (D_800B0DF0->vtxColorFlags & 2) {
            D_800BE524 = allocate_memory(gWorldObjectCount * 2, 1, 0, 0);
            for (i = 0; i < gWorldObjectCount; i++) {
                if (db_Object_array[i].numVertices != 0) {
                    D_800BE524[i] = D_800BE528;
                    func_15012C84(db_Object_array[i].srcVertices, db_Object_array[i].numVertices);
                } else {
                    D_800BE524[i] = 0xFFFF;
                }
            }
        }
        if (D_800B0DF0->vtxColorFlags & 4) {
            D_800BE520 = allocate_memory(D_800B0E38 * 4, 1, 0, 0);
            bzero(D_800BE520, D_800B0E38 * 4);
            for (i = 0; i < D_800B0E38; i++) {
                if (D_800B0E34[i] != 0) {
                    D_800BE520[i] = allocate_memory(D_800B0E34[i] * 4, 1, 0, 0);
                    b = D_800B0E30[i];
                    for (j = 0; j < D_800B0E34[i]; j++) {
                        D_800BE520[i][j] = allocate_memory(b[j].count * 3, 1, 0, 0);
                        if (i < 4) {
                            src = D_800B0E10[i];
                        } else {
                            src = db_Object_array[i - 4].srcVertices;
                        }
                        for (k = 0; k < b[j].count; k++) {
                            D_800BE520[i][j][k][0] = src[b[j].vertexIndices[k]].v.cn[0];
                            D_800BE520[i][j][k][1] = src[b[j].vertexIndices[k]].v.cn[1];
                            D_800BE520[i][j][k][2] = src[b[j].vertexIndices[k]].v.cn[2];
                        }
                    }
                } else {
                }
            }
        }
        func_15012ED8((u32 *)D_800B0E00[0]);
    }
}

void func_15012C84(Vtx *arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < arg1; i++) {
        D_800BE510[D_800BE528][0] = arg0[i].v.cn[0];
        D_800BE510[D_800BE528][1] = arg0[i].v.cn[1];
        D_800BE510[D_800BE528][2] = arg0[i].v.cn[2];
        D_800BE528++;
    }
}

void func_15012ED8(u32 *arg0) {
    s32 i;
    s32 op;

    for (i = 0; (op = (s8)(arg0[i * 2] >> 24)) != -0x21; i++) {
        if (op == -5) {
            D_800BE550[D_800BE564] = i;
            D_800BE530[D_800BE564 * 3] = (arg0[i * 2 + 1] >> 24) & 0xFF;
            D_800BE530[D_800BE564 * 3 + 1] = (arg0[i * 2 + 1] >> 16) & 0xFF;
            D_800BE530[D_800BE564 * 3 + 2] = (arg0[i * 2 + 1] >> 8) & 0xFF;
            D_800BE564++;
        }
    }
}

void func_15012F90(void) {
    s32 i;

    D_800DF7D0[0] = 0;
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
