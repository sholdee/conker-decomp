#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u8 r, g, b;
} Col3;

typedef struct {
    /* 0x0 */ Col3 *unk0;
    /* 0x4 */ u16 *unk4;
    /* 0x8 */ s32 unk8;
} Struct_151739B0; // size 0xC

extern Vtx *D_800B0E10;
#define E34 (*(u8 **)&D_800B0E34)
#define E30 (*(Struct_151739B0 ***)&D_800B0E30)
extern Col3 *D_800BE510;
extern Col3 ***D_800BE520;
extern u16 *D_800BE524;

s32 func_151739B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 j;
    s32 start;
    s32 end;
    Vtx *vtx;
    Struct_151739B0 *entries;
    Col3 *colors;
    u16 *indices;
    Col3 *src;
    u16 idx;
    Vtx *v;

    if (E34[arg0] == 0) {
        return;
    }
    if (!(D_800B0DF0->pad1C[5] & 4) && ((arg1 == 0 && !(D_800B0DF0->pad1C[5] & 1)) || (arg1 == 1 && !(D_800B0DF0->pad1C[5] & 2)))) {
        return;
    }
    if (arg3 == 0xFF) {
        start = 0;
        end = E34[arg0];
    } else {
        start = arg3;
        if (arg3 >= E34[arg0]) {
            return;
        }
        end = arg3 + 1;
    }
    if (arg1 == 0) {
        vtx = D_800B0E10;
    } else {
        vtx = ((Vtx **)(D_800DBEF4[arg4].pad20))[D_800BE9C0];
    }
    entries = E30[arg0];
    for (i = start; i < end; i++) {
        indices = entries[i].unk4;
        colors = entries[i].unk0;
        for (j = 0; j < entries[i].unk8; j++) {
            idx = indices[j];
            if (!(D_800B0DF0->pad1C[5] & 4)) {
                if (arg1 == 0) {
                    src = (Col3 *)((u8 *)D_800BE510 + idx * 3);
                } else {
                    src = (Col3 *)(&((u8 (*)[3])D_800BE510)[D_800BE524[arg4]][idx * 3]);
                }
            } else {
                src = &D_800BE520[arg0][i][j];
            }
            v = &vtx[idx];
            v->v.cn[0] = (((colors[j].r - src->r) * arg2) >> 8) + src->r;
            v->v.cn[1] = (((colors[j].g - src->g) * arg2) >> 8) + src->g;
            v->v.cn[2] = (((colors[j].b - src->b) * arg2) >> 8) + src->b;
        }
    }
}

extern s32 func_151739B0(s32, s32, s32, s32, s32);

s32 func_15173C60(s32 arg0, s32 arg1) {
    return func_151739B0(0, 0, arg0, arg1, 0);
}

extern s32 func_151149AC(u32);

void func_15173C90(s32 arg0, s32 arg1, s32 arg2) {
    struct131 *temp_v0;

    temp_v0 = (struct131 *)func_151149AC(arg2 & 0xFF);
    if (temp_v0 != NULL) {
        u32 temp;
        temp = *(u16 *)((u8 *)temp_v0 + 0x54) & 0xFFFF7FFF;
        func_151739B0(temp, 1, arg0, arg1, temp_v0 - D_800DBEF4);
    }
}
