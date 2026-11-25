#include <ultra64.h>
#define func_1515D4D4 func_1515D4D4_prototype
#define func_1515D6D0 func_1515D6D0_prototype
#include "functions.h"
#undef func_1515D4D4
#undef func_1515D6D0
#define D_800DCD20 D_800DCD20_hdr
#define D_800DCD24 D_800DCD24_hdr
#define D_800DCD30 D_800DCD30_hdr
#define D_800DCD40 D_800DCD40_hdr
#define D_800DCD78 D_800DCD78_hdr
#include "variables.h"
#undef D_800DCD20
#undef D_800DCD24
#undef D_800DCD30
#undef D_800DCD40
#undef D_800DCD78

/*
 * A dynamic point light (0x34 bytes). Every allocated light sits in the singly
 * linked list D_800DCD78. struct226 in structs.h is a partial view of this.
 */
typedef struct PointLight {
    /* 0x00 */ struct PointLight *next;
    /* 0x04 */ u8 unk4;              // only lights with 0 get a position / distance
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 unk8;              // matched against the caller's light mask; bit 0 = per-player
    /* 0x09 */ u8 disabled;
    /* 0x0A */ u8 unkA;              // per-player bit: hidden from that player
    /* 0x0B */ u8 group;             // id used by func_1515F170
    /* 0x0C */ u8 unkC;              // per-player bit, cleared every frame by func_1515D69C
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;                // x == -0x8000: (y << 16 | z) is a pointer to an f32[3] position
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 viewPos[4][3];    // position transformed by player i's view matrix
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 radius;
    /* 0x30 */ u8 viewPosValid;      // per-player bit: viewPos[i] is up to date
} PointLight;

/* A point light as the microcode reads it (0x30 bytes, loaded with G_MV_LIGHT). */
typedef struct {
    /* 0x00 */ u8 col[3];
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u8 colc[3];
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s8 dir[3];
    /* 0x0B */ u8 padB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x13];
    /* 0x20 */ s16 pos[4];
    /* 0x28 */ s16 pos2[4];
} PointLightGfx;

extern u8 D_800DCD20[3];            // ambient colour
extern u8 D_800DCD24[3];
extern u8 D_800DCD27;               // priority of the current ambient colour
extern s8 D_800DCD30[4][3];         // per-player directional light direction
extern PointLightGfx D_800DCD40;    // padding light for odd light counts
extern PointLight *D_800DCD78;      // list of all point lights

void *allocate_memory(s32 size, s32, s32, s32);
s32 func_150A1DA0(u8 *, struct178 *, s32);
f32 func_1515F008(s16 *, s32);
void func_1515F040(f32, s32);
void func_1515F0AC(f32, s32);
extern void func_10004074(void *);
extern f32 D_800A6530;

struct Struct1515FBC4 {
    u8 pad0[0x98];
    struct127 *unk98;
};

struct Struct1515FBC4 *func_15105C24(s32);

/* Allocates a zeroed 0x10-byte block. */
void *func_1515D440(void) {
    void *block;

    block = allocate_memory(0x10, 1, 2, 0);
    bzero(block, 0x10);
    return block;
}

/* Allocates a zeroed array of count 0x60-byte entries. */
void *func_1515D480(s32 count) {
    void *block;

    block = allocate_memory(count * 0x60, 1, 2, 0);
    bzero(block, count * 0x60);
    return block;
}


/* Sets the ambient colour unless a higher-priority one is already set. */
void func_1515D4D4(s32 r, s32 g, s32 b, u8 priority) {
    if (priority >= D_800DCD27) {
        D_800DCD20[0] = r;
        D_800DCD20[1] = g;
        D_800DCD20[2] = b;
        D_800DCD7C = 1;
        D_800DCD27 = priority;
    }
}

/* Allocates a zeroed point light and appends it to the light list. */
PointLight *func_1515D520(void) {
    PointLight *light;
    PointLight *tail;
    PointLight *next;

    light = allocate_memory(sizeof(PointLight), 1, 2, 2);
    if (light != NULL) {
        bzero(light, sizeof(PointLight));
        if (D_800DCD78 != NULL) {
            tail = D_800DCD78;
            next = tail->next;
            while (next != NULL) {
                tail = next;
                next = next->next;
            }
            tail->next = light;
        } else {
            D_800DCD78 = light;
        }
        light->next = NULL;
    }
    return light;
}

PointLight *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group);

PointLight *func_1515D5AC(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    return func_1515D5F8(x, y, z, radius, r, g, b, arg7, disabled, group);
}

/* Creates a point light at (x, y, z). */
PointLight *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    PointLight *light = func_1515D520();

    if (light != NULL) {
        light->unk4 = 0;
        light->color[0] = r;
        light->color[1] = g;
        light->color[2] = b;
        light->unk8 = arg7;
        light->disabled = disabled;
        light->group = group;
        light->unkA = 0;
        light->x = x;
        light->y = y;
        light->z = z;
        light->radius = radius;
        light->unk2C = 0x7F;
        light->unk2D = 0;
        light->unk2E = 0;
    }
    return light;
}

/* Clears every light's per-frame player masks. */
void func_1515D69C(void) {
    PointLight *light;

    for (light = D_800DCD78; light != NULL; light = light->next) {
        light->unkC = 0;
        light->viewPosValid = 0;
    }
}

void func_1515D6C8(void) {
}

extern s32 D_800DCD10[];
void func_1515EF74(s16 *arg0);

/* G_MOVEWORD index this microcode uses for the light-space scale matrix. */
#define G_MW_LIGHTSCALE 0x10

/* Enables lighting and uploads the light-space scale matrix of player `player`. */
Gfx *func_1515D6D0(Gfx *gfx, s32 player) {
    gMoveWd(gfx++, G_MW_NUMLIGHT, 0, 0);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    // each struct259 holds one Mtx per frame buffer at +0x100
    func_1515EF74((s16 *)((Mtx *)((u8 *)((struct259 *)D_800BE628 + player) + 0x100) + D_800BE9C0));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x00, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x08, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x04, (D_800DCD10[2] >> 16) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x0C, (D_800DCD10[2] >> 16) << 16);
    // matching: the doubled & 0xFFFF steers IDO's register choice for the low halves
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x10, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x18, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x14, (D_800DCD10[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x1C, (D_800DCD10[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x20, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x24, D_800DCD10[3] << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x28, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x2C, D_800DCD10[3] << 16);
    return gfx;
}

/*MWSTART*/
#ifndef CMDDEF
#define CMD(a, b) { Gfx *_g = arg0++; _g->words.w0 = (a); _g->words.w1 = (b); }
#endif
struct Obj1515EC78;
struct Light1515EC78;
void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7);
void func_1515E278(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6);
void func_1515EC78(struct Obj1515EC78 *arg0, s32 arg1, struct Light1515EC78 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
struct Node1515D914 {
    struct Node1515D914 *next;
    u8 unk4;
    u8 pad5[3];
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 padB;
    u8 unkC;
    u8 padD;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 pad14[0x1B];
    u8 unk2F;
};

Gfx *func_1515D914(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, s32 arg7, u8 *arg8, s8 *arg9, s32 argA, u8 *argB, s32 argC, s32 *argD) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 special;
    u8 *ent;
    s16 pos[3];
    u8 colA[3];
    u8 colB[3];
    struct Node1515D914 *node;
    struct Node1515D914 *lights[11];
    s32 dists[11];
    s32 dist;
    u8 spB0[3];
    u8 spAC[3];
    u8 spAB;
    u8 spAA;
    f32 d;
    f32 vec[3];
    s32 lpos[3];
    f32 *fp;

    func_1515E278(arg2, arg3, arg4, argA, colA, arg5, (u8 *)argC);
    j = 1 << arg1;
    if (argC & 2) {
        func_1515E43C(arg2, arg3, arg4, argA, spB0, spAC, &spAB, &spAA);
        for (i = 0; i < 3; i++) {
            s32 k;
            k = (u8)((((0xFF - colA[i]) * spAB) >> 8) + colA[i]);
            colB[i] = (spB0[i] * k) >> 8;
            k = (u8)((((0xFF - colA[i]) * spAA) >> 8) + colA[i]);
            colA[i] = (spAC[i] * k) >> 8;
        }
    }
    for (i = 0; i < 3; i++) {
        if (argB != 0) {
            if (!(argC & 8)) {
                colA[i] = (colA[i] + argB[i] * 3) >> 2;
            }
            argB[i] = colA[i];
        }
        arg8[i] = arg8[i + 4] = colA[i];
        continue;
    }
    for (i = 0; i < arg7; i++) {
        dists[i] = 0x7FFFFFFF;
    }
    if (!(argC & 1)) {
        pos[0] = arg2;
        pos[1] = arg3;
        pos[2] = arg4;
    } else {
        vec[0] = D_800DBFF0[arg1].unk2BC - D_800DBFF0[arg1].unk2F8;
        vec[1] = D_800DBFF0[arg1].unk2C0 - D_800DBFF0[arg1].unk2FC;
        vec[2] = D_800DBFF0[arg1].unk2C4 - D_800DBFF0[arg1].unk300;
        d = sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        if (d != 0.0f) {
            d = 1000.0f / d;
            pos[0] = vec[0] * d + (f32)arg2;
            pos[1] = vec[1] * d + (f32)arg3;
            pos[2] = vec[2] * d + (f32)arg4;
        } else {
            pos[0] = arg2;
            pos[1] = arg3;
            pos[2] = arg4;
        }
    }
    node = (struct Node1515D914 *)D_800DCD78;
    cnt = 0;
    if (node != 0) {
        do {
            if (((node->unkC & (1 << arg1)) || (argC & 0x20) || !(node->unk8 & 1)) && (node->unk8 & argA) && (node->unk9 == 0) && !(node->unkA & (1 << arg1))) {
                if (node->unk4 == 0) {
                G:
                    lpos[0] = node->unkE;
                    if (lpos[0] != -0x8000) {
                        lpos[1] = node->unk10;
                        lpos[2] = node->unk12;
                    } else {
                        fp = (f32 *)((node->unk12 & 0xFFFF) | (node->unk10 << 16));
                        lpos[0] = fp[0];
                        lpos[1] = fp[1];
                        lpos[2] = fp[2];
                    }
                    dist = (pos[0] - lpos[0]) * (pos[0] - lpos[0]) + (pos[1] - lpos[1]) * (pos[1] - lpos[1]) + (pos[2] - lpos[2]) * (pos[2] - lpos[2]);
                    if (!(argC & 4) && ((node->unk2F << 17) < dist)) {
                        dist = 0x7FFFFFFF;
                    }
                }
                for (i = 0; i < arg7 - 1; i++) {
                    if (dist < dists[i]) {
                        for (j = arg7 - 2; j > i; j--) {
                            dists[j] = dists[j - 1];
                            lights[j] = lights[j - 1];
                        }
                        dists[i] = dist;
                        lights[i] = node;
                        if (cnt != arg7 - 1) {
                            cnt++;
                        }
                        goto found;
                    }
                }
            found:;
            }
            node = node->next;
        } while (node != 0);
    }
    if (arg7 != 0) {
        if (cnt & 1) {
            special = 1;
            if (argD != 0) {
                argD[cnt] = 0;
            }
        } else {
            special = 0;
        }
        if (arg9 != 0) {
            if (special != 0) {
                *arg9 = (cnt + special + 1) | 0x80;
            } else {
                *arg9 = cnt + special + 1;
            }
        }
        CMD(0xDB020000, (cnt + special) * 0x30 + 0x30);
    } else {
        if (arg9 != 0) {
            *arg9 = cnt;
        }
        CMD(0xDB020000, cnt * 0x30);
    }
    if (arg7 != 0) {
        u8 *p;
        p = arg6 + (D_800BE9C0 * arg7 + cnt) * 0x30;
        p[8] = ((s8 *)&D_800DCD30)[arg1 * 3];
        p[9] = ((s8 *)&D_800DCD30)[arg1 * 3 + 1];
        p[0xA] = ((s8 *)&D_800DCD30)[arg1 * 3 + 2];
        if (argC & 2) {
            p[0] = p[4] = colB[0];
            p[1] = p[5] = colB[1];
            p[2] = p[6] = colB[2];
        } else {
            p[0] = p[4] = 0;
            p[1] = p[5] = 0;
            p[2] = p[6] = 0;
        }
        p -= cnt * 0x30;
        for (i = 0; i < cnt; i++, p += 0x30) {
            node = lights[i];
            if (argD != 0) {
                argD[i] = (s32)node;
            }
            if (argC & 0x20) {
                node->unkC |= 1 << arg1;
            }
            if (node->unk4 == 0) {
                func_1515EC78((struct Obj1515EC78 *)node, arg1, (struct Light1515EC78 *)p, arg2, arg3, arg4, argC);
            }
            CMD(((((u32)(i * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)p);
            continue;
        }
        if (special != 0) CMD(((((u32)(i * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)&D_800DCD40);
        CMD(((((u32)((i + special + 1) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, ((D_800BE9C0 * arg7 + i) * 0x30) + (s32)arg6);
        CMD(((((u32)((i + special + 2) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)arg8);
    }
    return arg0;
}


struct Node1515E43C {
    char pad0[0x18];
    u32 unk18;
    u32 unk1C;
};

struct Node1515EB84 {
    struct Node1515EB84 *next;
    struct178 *unk4;
};

struct Node1515E43C *func_1515EB84(s32, s32, s32, s32, struct Node1515EB84 *);
void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7);

void func_1515E278(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6) {
    struct Node1515E43C *node;
    s32 i;
    s32 t;
    u8 sp50[3];
    u8 sp4C[3];
    u8 sp48[3];
    u8 sp47;
    u8 sp46;
    s32 s;
    u8 *dst;
    s32 v;

    node = func_1515EB84(arg0, arg1, arg2, arg3, (struct Node1515EB84 *)D_800DCD80);
    if (node != NULL) {
        arg4[0] = node->unk18 & 0xFF;
        arg4[1] = (node->unk18 >> 8) & 0xFF;
        arg4[2] = (node->unk18 >> 16) & 0xFF;
    } else {
        v = (0x100 - (s32)((((u32)arg5 >> 5) & 3) << 6));
        dst = arg4;
        for (i = 0; i < 3; i++) {
            *dst++ = ((*(u8 (*)[3])&D_800DCD20)[i] * v) >> 8;
        }
    }
    if ((s32)arg6 & 0x10) {
        func_1515E43C(arg0, arg1, arg2, arg3, sp4C, sp48, &sp47, &sp46);
        dst = arg4;
        for (i = 0; i < 3; i++) {
            s = (u8)((((0xFF - *dst) * sp47) >> 8) + *dst);
            sp50[i] = (sp4C[i] * s) >> 8;
            s = (u8)((((0xFF - *dst) * sp46) >> 8) + *dst);
            *dst = (sp48[i] * s) >> 8;
            t = *dst + ((sp50[i] * 7) >> 3);
            if (t > 0xFF) {
                t = 0xFF;
            }
            *dst = t;
            dst++;
        }
    }
}


void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7) {
    struct Node1515E43C *node;

    node = func_1515EB84(arg0, arg1, arg2, 0, D_800DCD84);
    if (node != NULL) {
        arg4[0] = node->unk18 & 0xFF;
        arg4[1] = (node->unk18 >> 8) & 0xFF;
        arg4[2] = (node->unk18 >> 16) & 0xFF;
        arg5[0] = node->unk1C & 0xFF;
        arg5[1] = (node->unk1C >> 8) & 0xFF;
        arg5[2] = (node->unk1C >> 16) & 0xFF;
        arg6[0] = (node->unk1C >> 24) & 0xFF;
        arg7[0] = (node->unk18 >> 24) & 0xFF;
    } else {
        arg4[0] = ((u8 *)D_800DCD24)[0];
        arg4[1] = ((u8 *)D_800DCD24)[1];
        arg4[2] = ((u8 *)D_800DCD24)[2];
        arg5[0] = D_800DCD28[0];
        arg5[1] = D_800DCD28[1];
        arg5[2] = D_800DCD28[2];
        arg6[0] = D_800DCD3C;
        arg7[0] = D_800DCD3D;
    }
}

u32 *func_1515E544(u32 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    s32 count;
    s32 special;
    s32 index;

    count = arg3 & 0x7F;
    if (count != arg3) {
        special = 1;
    } else {
        special = 0;
    }
    { u32 *command = arg0; arg0 += 2; command[0] = 0xDB020000; command[1] = count * 0x30; }
    for (index = 0; index < count - special - 1; index++) {
        u32 *command = arg0; arg0 += 2; command[0] = ((((u32)(index * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = ((D_800BE9C0 * arg2 + index) * 0x30) + arg1;
    }
    if (special != 0) {
        u32 *command = arg0; arg0 += 2; command[0] = ((((u32)(index * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = (u32)&D_800DCD40;
    }
    { u32 *command = arg0; arg0 += 2; command[0] = ((((u32)((index + special + 1) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = ((D_800BE9C0 * arg2 + index) * 0x30) + arg1; }
    { u32 *command = arg0; arg0 += 2; command[0] = ((((u32)((index + special + 2) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = arg4; }
    return arg0;
}

struct Light1515E888 {
    u8 pad0[5];
    u8 r;
    u8 g;
    u8 b;
    u8 pad8[6];
    s16 x;
    s16 y;
    s16 z;
    u8 pad14[0x1B];
    u8 radius;
};

extern u8 D_800D9E21;
extern struct Light1515E888 *D_800D9E28[];
void func_1515E278(s32, s32, s32, s32, u8 *, u8 *, u8 *);

void func_1515E888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, u8 *arg7)
{
  f32 sq;
  f32 dist;
  f32 pos[3];
  f32 color[3];
  s32 i;
  f32 *new_var;
  s32 n;
  struct Light1515E888 *light;
  struct Light1515E888 **p;
  f32 f;
  s32 v;
  func_1515E278(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  color[0] = arg5[0];
  color[1] = arg5[1];
  color[2] = arg5[2];
  v = D_800D9E21 & 0x7F;
  n = v - 1;
  i = 0;
  if (n > 0)
  {
    p = D_800D9E28;
    new_var = pos;
    loop:
    light = *p;

    if (light != 0)
    {
      pos[0] = light->x;
      pos[1] = light->y;
      pos[2] = light->z;
      dist = light->radius * 2048.0f;
      sq = (((new_var[0] - arg1) * (new_var[0] - arg1)) + ((new_var[1] - arg2) * (new_var[1] - arg2))) + ((new_var[2] - arg3) * (new_var[2] - arg3));
      f = dist / sq;
      if (f > 1.0f)
      {
        f = 1.0f;
      }
      color[0] += f * light->r;
      color[1] += f * light->g;
      color[2] += f * light->b;
    }
    i++;
    p++;
    if (i != n)
    {
      goto loop;
    }
  }
  v = color[0];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[0] = v;
  v = color[1];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[1] = v;
  v = color[2];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[2] = v;
}

struct Node1515E43C *func_1515EB84(s32 arg0, s32 arg1, s32 arg2, s32 arg3, struct Node1515EB84 *node) {
    u8 sp34[0x32C];
    struct178 *entry;

    *(f32 *)&sp34[0x14] = (f32)arg0;
    *(f32 *)&sp34[0x18] = (f32)arg1;
    *(f32 *)&sp34[0x180] = (f32)arg1;
    *(f32 *)&sp34[0x2C] = (f32)arg0;
    *(f32 *)&sp34[0x30] = (f32)arg1;
    *(f32 *)&sp34[0x1C] = (f32)arg2;
    *(f32 *)&sp34[0x34] = (f32)arg2;

    while (node != 0) {
        entry = node->unk4;
        if (entry->unk6[0xE] == 0) {
            if ((arg3 == 0) || (((*(u32 *)&entry->unk6[0x12] >> 24) & arg3) != 0)) {
                if ((0x18 == ((s32)entry->unk6[0xF] >> 2)) || ((*(u32 *)&entry->unk6[0x12] >> 31) == 0)) {
                    if (func_150A1DA0(sp34, entry, 0) == 0) {
                        return (struct Node1515E43C *)node->unk4;
                    }
                }
            }
        }
        node = node->next;
    }
    return 0;
}

struct Light1515EC78 {
    u8 col[3];
    u8 pad3;
    u8 colc[3];
    u8 pad7;
    s8 dir[3];
    u8 padB;
    u8 unkC;
    u8 padD[0x13];
    s16 unk20[4];
    s16 unk28[4];
};

struct Obj1515EC78 {
    u8 pad0[5];
    u8 col[3];
    u8 pad8[6];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14[4][3];
    u8 pad2C[3];
    u8 unk2F;
    u8 unk30;
};

extern f32 D_800D9C10[][4][4];
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_1515EC78(struct Obj1515EC78 *arg0, s32 arg1, struct Light1515EC78 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 i;
    f32 out[3];
    f32 dx;
    f32 dy;
    f32 dz;
    f32 s;
    s32 n[3];
    s32 pos[3];
    f32 *p;

    pos[0] = arg0->unkE;
    if (pos[0] != -0x8000) {
        pos[1] = arg0->unk10;
        pos[2] = arg0->unk12;
    } else {
        p = (f32 *)((arg0->unk12 & 0xFFFF) | (arg0->unk10 << 16));
        pos[0] = p[0];
        pos[1] = p[1];
        pos[2] = p[2];
    }
    if (arg6 & 2) {
        dx = pos[0] - arg3;
        dy = pos[1] - arg4;
        dz = pos[2] - arg5;
        s = sqrtf(dx * dx + dy * dy + dz * dz);
        if (s) {
            s = 127.0f / s;
            dx *= s;
            dy *= s;
            dz *= s;
        } else {
            dz = dy = 0.0f;
            dx = 127.0f;
        }
        n[0] = dx;
        n[1] = dy;
        n[2] = dz;
    } else {
        n[0] = 0;
        n[1] = 0;
        n[2] = 0x7F;
    }
        if (arg0->unk30 & (1 << arg1)) {
        pos[0] = arg0->unk14[arg1][0];
        pos[1] = arg0->unk14[arg1][1];
        pos[2] = arg0->unk14[arg1][2];
    } else {
        func_150A7960(D_800D9C10[arg1][0], pos[0], pos[1], pos[2], &out[0], &out[1], &out[2]);
        pos[0] = out[0];
        pos[1] = out[1];
        pos[2] = out[2];
        arg0->unk14[arg1][0] = pos[0];
        arg0->unk14[arg1][1] = pos[1];
        arg0->unk14[arg1][2] = pos[2];
        arg0->unk30 |= (1 << arg1);
    }
    for (i = 0; i < 3; i++) {
        arg2->col[i] = arg0->col[i];
        arg2->colc[i] = arg0->col[i];
        arg2->dir[i] = n[i];
        arg2->unk20[i] = pos[i];
        arg2->unk28[i] = pos[i];
    }
    arg2->unkC = arg0->unk2F;
}

void func_1515EF74(s16 *arg0) {
    func_1515F040(1.0f / func_1515F008(arg0, 0), 0);
    func_1515F040(1.0f / func_1515F008(arg0, 5), 1);
    func_1515F040(1.0f / func_1515F008(arg0, 0xA), 2);
    func_1515F0AC(-func_1515F008(arg0, 0xE), 3);
}

f32 func_1515F008(s16 *arg0, s32 arg1) {
    s32 hi = arg0[arg1];
    hi = (hi << 16) | (u16)arg0[arg1 + 0x10];
    return (f32)hi * 1.52587890625e-05f;
}

extern s32 D_800DCD10[];

void func_1515F040(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (var_fv0 <= arg0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    arg0 *= 65536.0f;
    D_800DCD10[arg1] = (s32) arg0;
}

extern s32 D_800DCD10[];

void func_1515F0AC(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (arg0 >= var_fv0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    D_800DCD10[arg1] = (s32) arg0;
}

struct Node1515F10C {
    struct Node1515F10C *next;
};

void func_1515F10C(struct Node1515F10C *arg0) {
    struct Node1515F10C *node;
    struct Node1515F10C *prev;

    node = (struct Node1515F10C *)D_800DCD78;
    prev = 0;
    if (node != arg0) {
        do {
            prev = node;
            node = node->next;
        } while (node != arg0);
    }
    if (prev != 0) {
        prev->next = node->next;
    } else {
        D_800DCD78 = (s32)node->next;
    }
    func_10004074(node);
}

struct Node1515F170 {
    struct Node1515F170 *next;
    char pad4[5];
    s8 unk9;
    char padA[1];
    u8 unkB;
};

void func_1515F170(s32 arg0, u8 arg1) {
    struct Node1515F170 *p;

    for (p = (struct Node1515F170 *)D_800DCD78; p != 0; p = p->next) {
        if (arg0 == p->unkB) {
            p->unk9 = arg1;
        }
    }
}

struct Node1515F1B0 {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    s32 unkC;
};

struct Node1515F1B0 *func_1515F1B0(s32 arg0) {
    struct Node1515F1B0 *temp;

    temp = (struct Node1515F1B0 *)func_10003C6C(0x10, 1, 2, 0, 1);
    if (temp == 0) {
        return 0;
    }
    temp->unk0 = arg0;
    if (arg0 != 0) {
        temp->unk4 = func_1514462C(arg0);
    } else {
        temp->unk4 = 0.0f;
    }
    if (arg0 != 0) {
        temp->unk8 = func_15144598((struct134 *)arg0);
    } else {
        temp->unk8 = 0.0f;
    }
    temp->unkC = 0;
    return temp;
}

void func_1515F25C(void **arg0, void *arg1) {
    *(void **)((u8 *)arg1 + 0xC) = *arg0;
    *arg0 = arg1;
}

extern void (*D_8008B090[])(s32);

void func_1515F270(s32 arg0, s32 *arg1) {
    s32 idx = arg1[6];

    if (idx >= 0 && idx < 12) {
        if (D_8008B090[idx] != 0) {
            D_8008B090[idx](arg0);
        }
    }
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515F2B8(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6001D, arg0->unk7A, -1);
}

extern void (*D_8008B0C0[])(void);

void func_1515F2E8(u8 *arg0, s32 *arg1) {
    s32 idx;

    if (arg0[0x3B] == 1) {
        idx = arg1[7];
        if (idx >= 0 && idx < 3) {
            D_8008B0C0[idx]();
        }
    }
}

void func_1515F338(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 97.0f) {
        D_800DCD94 += (97.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 96.0f) {
        D_800DCD98 += (96.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 98.0f) {
        D_800DCD9C += (98.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

void func_1515F5C4(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 229.0f) {
        D_800DCD94 += (229.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 253.0f) {
        D_800DCD98 += (253.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 160.0f) {
        D_800DCD9C += (160.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

struct Stack1515F850 {
    s32 pad14;
    s32 pad18;
    u8 sp1C;
    u8 sp1D;
    u8 sp1E;
    u8 pad1F;
    f32 sp20;
    f32 sp24;
    f32 sp28;
};

void func_1515F850(s32 arg0, s32 arg1) {
    s32 pad;
    struct Stack1515F850 stack;
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;

    stack.sp1C = ((u8 *)&D_800DCD20)[0];
    stack.sp1D = ((u8 *)&D_800DCD20)[1];
    stack.sp1E = ((u8 *)&D_800DCD20)[2];
    temp_f0 = sinf(D_800DCDA0);
    temp_f14 = (temp_f0 * 29.5f) + 127.5f;
    temp_f16 = (temp_f0 * 71.0f) + 109.0f;
    stack.sp28 = temp_f14;
    stack.sp24 = temp_f16;
    temp_f18 = (temp_f0 * 26.0f) + 26.0f;
    stack.sp20 = temp_f18;
    D_800DCDA0 += D_800A6530 * D_800BE9A4;
    D_800DCDA0 = func_15144B68(D_800DCDA0);
    if (stack.sp28 != D_800DCD94) {
        D_800DCD94 += (stack.sp28 - D_800DCD94) * 0.5f;
        stack.sp1C = (u8)(u32)D_800DCD94;
    }
    if (stack.sp24 != D_800DCD98) {
        D_800DCD98 += (stack.sp24 - D_800DCD98) * 0.5f;
        stack.sp1D = (u8)(u32)D_800DCD98;
    }
    if (stack.sp20 != D_800DCD9C) {
        D_800DCD9C += (stack.sp20 - D_800DCD9C) * 0.5f;
        stack.sp1E = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(stack.sp1C, stack.sp1D, stack.sp1E, 0);
}

long func_1515FB70(u8 *a0, u8 *a1)
{
  long offset;
  s32 v0;
  s32 *slot;
  v0 = (*((u8 *) (a0 + 0x3B))) == 1;
  if ((*((u8 *) (a0 + 0x3B))) == 1)
  {
    v0 = *((s32 *) (a1 + 0x1C));
    if (v0 >= 0)
    {
      offset = 0x1C;
      if (!v0)
      {
        slot = (s32 *) (a1 + offset);
      }
    }
  }
}


s32 func_1515FB94(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6002D, arg0->unk7A, -1);
}

void func_1515FBC4(struct127 *arg0, s32 arg1) {
    struct Struct1515FBC4 *temp_v0;
    struct127 *temp_v1;
    s32 temp_a3;

    temp_v1 = 0;
    temp_v0 = func_15105C24(arg1);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0->unk98;
    }
    if (temp_v1 != 0) {
        temp_a3 = ((s32)temp_v1 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
    } else {
        temp_a3 = -1;
    }
    func_1505D024(arg0, 0x6002E, arg0->unk7A, temp_a3);
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515FC34(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x33, 0xC000, -1);
}

struct Struct1515FC60 {
    s16 unk0;
    u8 pad2[2];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x14];
    u32 unk1C;
};

union Union1515FC60 {
    f32 f;
    f32 pad[2];
};

extern f32 D_800A6534;
s32 func_150AD960(s32, s32, s32, s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(struct127 *, s32);

void func_1515FC60(struct127 *arg0, struct Struct1515FC60 *arg1) {
    f32 temp_f0;
    union Union1515FC60 temp_f18;
    s32 temp_v0;

    if (arg0->unk31C != NULL) {
        if (arg0->unk31C->unk120 == 0) {
            temp_v0 = arg1->unk1C;
            if (temp_v0 != 0) {
                temp_f18.f = (f32)(u32)temp_v0 * D_800A6534;
                temp_v0 = func_150AD960(arg1->unk0, arg1->unk4, (s32)arg0->x_position, (s32)arg0->z_position);
                temp_f0 = ((f32)arg1->unk6 * temp_f18.f) - (f32)arg0->unkE4;
                if (temp_f0 > (f32)temp_v0) {
                    return;
                }
            }
            func_1505D024(arg0, 0x2F, 0, -1);
            func_15136C3C(arg0, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(arg0);
            func_1507CD64(arg0, 6);
        }
    }
}
