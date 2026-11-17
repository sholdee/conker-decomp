#include <ultra64.h>
#include "functions.h"
#define D_800CC2D0 D_800CC2D0_hdr
#define D_800D19A0 D_800D19A0_hdr
#include "variables.h"
#undef D_800CC2D0
#undef D_800D19A0


typedef struct Entry1517ABB0 {
    s32 unk0;
    u8 unk4;
    u8 pad5[3];
} Entry1517ABB0;

typedef struct Rec1517ABB0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
} Rec1517ABB0;

/* D_800DD460[i] holds a table of row pointers into Entry1517ABB0 arrays. */
#define ENTRY_1517ABB0(i, j, k) (((Entry1517ABB0 **)D_800DD460[i])[j][k])

extern Rec1517ABB0 D_800A7230[];
extern u16 D_800C4310[];
extern void func_10004074(s32);

void func_1517ABB0(void) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 2; i++) {
        if (D_800DD460[i] != 0) {
            for (j = 0; j < D_800C4310[D_800A7230[i].unk0]; j++) {
                for (k = 0; k < D_800A7230[i].unk1; k++) {
                    if (ENTRY_1517ABB0(i, j, k).unk0 != 0) {
                        if (ENTRY_1517ABB0(i, j, k).unk4 != 0) {
                            ENTRY_1517ABB0(i, j, k).unk4--;
                        } else {
                            func_10004074(ENTRY_1517ABB0(i, j, k).unk0);
                            ENTRY_1517ABB0(i, j, k).unk0 = 0;
                        }
                    }
                }
            }
        }
    }
}

typedef struct {
    f32 blend;
    f32 rate;
    u8 prev;
    u8 cur;
    u8 hold;
} Game1A8060Blend;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x1C8 - 0x5];
    u8 unk1C8;
    u8 pad1C9[0x260 - 0x1C9];
    Game1A8060Blend *blend;
    u8 pad264[0x28C - 0x264];
    s32 unk28C[2][2];
    u8 pad29C[0x32C - 0x29C];
} Game1A8060Actor;

typedef struct {
    u8 set;
    u8 slots;
    u16 vertexCounts[3];
} Game1A8060Config;

typedef struct {
    s32 resource;
    u8 timer;
} Game1A8060Entry;

extern Game1A8060Actor D_800CC2D0[];

extern s32 D_800D19A0[];
#define G1517AD00_ACTOR D_800CC2D0
#define G1517AD00_TBL ((Game1A8060Entry ***)D_800DD460)
#define G1517AD00_D19A0 D_800D19A0
#define G1517AD00_CFG ((u8 *)D_800A7230)

extern s32 *D_800C4020[];
s32 func_150849A0(void *);
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1502B5C8(u32 *, s32, ...);

s32 func_1517AD00(s32 arg0, s32 arg1, s32 arg2) {
    Game1A8060Blend *blend;
    Game1A8060Config *config;
    s32 type;
    u32 size;
    s32 kind;
    s32 index;
    s32 i;
    s32 j;
    Vtx *vtx;
    s32 group;
    s16 (*a)[3];
    s16 (*b)[3];
    Vtx *vtxBase;

    type = G1517AD00_ACTOR[arg2].unk4;
    kind = func_150849A0(&G1517AD00_ACTOR[arg2]);
    index = G1517AD00_ACTOR[arg2].unk1C8;
    config = NULL;
    for (i = 0; i < 2; i++) {
        if (kind == G1517AD00_CFG[i * 8]) {
            config = (Game1A8060Config *)&G1517AD00_CFG[i * 8];
            group = i;
            break;
        }
    }
    if (config == NULL) {
        return -1;
    }
    blend = G1517AD00_ACTOR[arg2].blend;
    if (index != 0) {
        index = 0;
        type = kind;
        if (G1517AD00_ACTOR[arg2].unk28C[index][D_800BE9C0] == 0 || G1517AD00_D19A0[type] == 0) {
            if (arg0 == blend->cur) {
                if (D_800BEAC0 == 0) {
                    blend->blend += blend->rate * D_800BE9E4;
                }
                if (blend->blend > 1.0f) {
                    blend->blend = 1.0f;
                }
            } else {
                if (blend->cur != 0xFF) {
                    blend->prev = blend->cur;
                }
                blend->cur = arg0;
                blend->blend = 0.0f;
                if (arg1 <= 0) {
                    arg1 = 1;
                }
                blend->rate = 1.0f / arg1;
            }
            blend->hold = 3;
            return -1;
        }
    }
    if (G1517AD00_D19A0[type] == 0) {
        return -1;
    }
    if (G1517AD00_ACTOR[arg2].unk28C[index][D_800BE9C0] == 0) {
        return -1;
    }
    if (arg0 == blend->cur && blend->hold == 0) {
        blend->prev = blend->cur;
        return 0;
    }
    if (arg0 != blend->cur) {
        if (blend->cur != 0xFF) {
            blend->prev = blend->cur;
        }
        blend->cur = arg0;
        blend->blend = 0.0f;
        if (arg1 <= 0) {
            arg1 = 1;
        }
        blend->hold = 3;
        blend->rate = 1.0f / arg1;
    }
    if (G1517AD00_TBL[group] == NULL) {
        s32 failed;

        G1517AD00_TBL[group] = allocate_memory(D_800C4310[config->set] * 4, 1, 0, 2);
        if (G1517AD00_TBL[group] == NULL) {
            return -1;
        }
        failed = 0;
        for (i = 0; i < D_800C4310[config->set]; i++) {
            G1517AD00_TBL[group][i] = allocate_memory(config->slots * 8, 1, 0, 2);
            if (G1517AD00_TBL[group][i] != NULL) {
                bzero(G1517AD00_TBL[group][i], config->slots * 8);
            } else {
                failed = 1;
            }
        }
        if (failed) {
            for (i = 0; i < D_800C4310[config->set]; i++) {
                if (G1517AD00_TBL[group][i] != NULL) {
                    func_10004074((s32)G1517AD00_TBL[group][i]);
                }
            }
            return -1;
        }
    }
    if (D_800BEAC0 == 0) {
        blend->blend += blend->rate * D_800BE9E4;
    }
    if (blend->blend > 1.0f) {
        blend->blend = 1.0f;
    }
    vtxBase = (Vtx *)G1517AD00_ACTOR[arg2].unk28C[index][D_800BE9C0];
    for (j = 0; j < D_800C4310[config->set]; j++) {
        if (G1517AD00_TBL[group][j][blend->cur].resource == 0) {
            G1517AD00_TBL[group][j][blend->cur].resource = func_1502B5C8(&size, 4, 0x13, group, j, blend->cur);
            if (G1517AD00_TBL[group][j][blend->cur].resource == 0) {
                return -1;
            }
        }
        if (G1517AD00_TBL[group][j][blend->prev].resource == 0) {
            G1517AD00_TBL[group][j][blend->prev].resource = func_1502B5C8(&size, 4, 0x13, group, j, blend->prev);
            if (G1517AD00_TBL[group][j][blend->prev].resource == 0) {
                func_10004074(G1517AD00_TBL[group][j][blend->cur].resource);
                G1517AD00_TBL[group][j][blend->cur].resource = 0;
                return -1;
            }
        }
        G1517AD00_TBL[group][j][blend->cur].timer = 3;
        G1517AD00_TBL[group][j][blend->prev].timer = 3;
        a = (s16 (*)[3])G1517AD00_TBL[group][j][blend->prev].resource;
        b = (s16 (*)[3])G1517AD00_TBL[group][j][blend->cur].resource;
        vtx = &vtxBase[D_800C4020[type][j]];
        for (i = 0; i < config->vertexCounts[j]; i++) {
            vtx[i].v.ob[0] = a[i][0] + ((f32)b[i][0] - a[i][0]) * blend->blend;
            vtx[i].v.ob[1] = a[i][1] + ((f32)b[i][1] - a[i][1]) * blend->blend;
            vtx[i].v.ob[2] = a[i][2] + ((f32)b[i][2] - a[i][2]) * blend->blend;
        }
        osWritebackDCache(vtx, config->vertexCounts[j] * 0x10);
    }
    if (blend->blend == 1.0f) {
        blend->hold--;
    } else {
        blend->hold = 3;
    }
    return 0;
}
