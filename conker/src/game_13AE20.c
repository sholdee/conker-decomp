#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

void *func_1510D970(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 size[2];
    union { s32 w; f64 d; } flag;
    s32 i;

    size[0] = 0x128;
    flag.w = arg3 & 1;
    if (flag.w != 0) {
        size[0] = 0x13A;
    }

    arg3 = (s32)func_15167A68(0x4B, 0, size[0], 1, 0xFF, 1);
    if (arg3 != 0) {
        for (i = 0; i < 2; i++) {
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0x96) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0x98) = 0x800;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0x9A) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xA6) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xA8) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xAA) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xB6) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xB8) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xBA) = 0x800;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xC6) = 0;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xC8) = 0x800;
            *(s16 *)((u8 *)arg3 + (i * 0x40) + 0xCA) = 0x800;
        }

        *((u8 *)arg3 + 0x120) = arg0;
        *(s32 *)((u8 *)arg3 + 0x110) = arg1;
        *((u8 *)arg3 + 0x121) = arg2;
        *((u8 *)arg3 + 0x124) = arg4;

        if (flag.w != 0) {
            for (i = 0; i < 9; i++) {
                *(s16 *)((u8 *)arg3 + 0x128 + (i * 2)) = 0x7FFF;
            }
        }
    }

    return (void *)arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_13AE20/func_1510DA84.s")

extern void func_150A7D00(void *, s32, s32, s32);
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern s32 func_1506196C(u8 *arg0, s32 arg1);
extern s32 D_80091770[];
extern s32 D_800DD1B4;
extern s32 D_800DD1B0[];
extern s16 D_800DD1C6;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ Mtx mtx[2];
    /* 0x090 */ Vtx vtx[2][4];
    /* 0x110 */ u8 *unk110;
    /* 0x114 */ s32 unk114;
    /* 0x118 */ s32 unk118;
    /* 0x11C */ s32 unk11C;
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 unk121;
    /* 0x122 */ u8 unk122;
    /* 0x123 */ u8 unk123;
} Struct_1510E120;

Gfx *func_1510E120(Gfx *gfx, Struct_1510E120 *arg1, s16 arg2) {
    s32 scale;
    s32 alpha;
    s32 tex;

    if (arg1->unk123 != 0) {
        func_150A7D00(&arg1->mtx[D_800BE9C0], arg1->unk114, arg1->unk11C, arg1->unk118);
        gSPMatrix(gfx++, &arg1->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (D_800DD1B4 != arg1->unk121) {
            D_800DD1B0[1] = arg1->unk121; /* same address as D_800DD1B4; a distinct symbol keeps the split %hi/%lo */
            tex = func_1510D0EC(D_80091770[arg1->unk121], 0, 0x3E, 0);
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, tex);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
        }
        gDPPipeSync(gfx++);
        gSPClearGeometryMode(gfx++, G_CULL_BOTH);
        scale = arg1->unk122;
        switch (arg1->unk120) {
            case 0:
                alpha = func_1506196C(arg1->unk110, arg2);
                break;
            case 1:
                alpha = (arg1->unk110[0x8A] * arg1->unk110[arg2 + 0x8B] + 0xFF) >> 8;
                break;
        }
        scale = (scale * alpha + 0xFF) >> 8;
        if (scale != D_800DD1C6) {
            D_800DD1C6 = scale;
            gDPPipeSync(gfx++);
            gDPSetPrimColor(gfx++, 1, 0, 0, 0, 0, scale);
        }
        gSPVertex(gfx++, arg1->vtx[D_800BE9C0], 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        gDPPipeSync(gfx++);
    }
    return gfx;
}

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
} Point3s; /* size = 0x6 */

s32 func_1510E388(Point3s **pts, Point3s *verts, f32 *outA, f32 *outB) {
    f32 ax;
    f32 ay;
    f32 az;
    f32 bx;
    f32 by;
    f32 bz;
    f32 tmp;
    f32 d0;
    f32 d1;
    f32 t;
    s32 i;
    s32 i0;
    s32 i1;
    s32 i2;
    s32 sel;

    i = 0;
    if ((pts == NULL) && (verts == NULL)) {
        *outB = 0.0f;
        *outA = 0.0f;
        return 0;
    }

    do {
        sel = 1;
        if (i >= 3) {
            i0 = i - 3;
        } else {
            i0 = i;
        }
        if (i0 != 2) {
            i1 = i0 + 1;
        } else {
            i1 = 0;
        }
        if (i1 != 2) {
            i2 = i1 + 1;
        } else {
            i2 = 0;
        }

        if (verts != NULL) {
            ax = verts[i1].x - verts[i0].x;
            ay = verts[i1].y - verts[i0].y;
            az = verts[i1].z - verts[i0].z;
            bx = verts[i2].x - verts[i0].x;
            by = verts[i2].y - verts[i0].y;
            bz = verts[i2].z - verts[i0].z;
        } else {
            ax = pts[i1]->x - pts[i0]->x;
            ay = pts[i1]->y - pts[i0]->y;
            az = pts[i1]->z - pts[i0]->z;
            bx = pts[i2]->x - pts[i0]->x;
            by = pts[i2]->y - pts[i0]->y;
            bz = pts[i2]->z - pts[i0]->z;
        }

        if (i >= 3) {
            tmp = ax;
            ax = bx;
            bx = tmp;
            tmp = ay;
            ay = by;
            by = tmp;
            tmp = az;
            az = bz;
            bz = tmp;
        }
        d0 = bx * az;
        d1 = ax * bz;
        if (d0 == d1) {
            sel = 0;
        }
        if (bx == 0.0f) {
            sel = 0;
        }
        i++;
        if (i == 6) {
            sel = 2;
        }
    } while (sel == 0);

    if (sel == 2) {
        *outA = 0.0f;
        *outB = 0.0f;
        return 0;
    }

    t = ((bx * ay) - (ax * by)) / (d0 - d1);
    *outB = t;
    *outA = (by - (t * bz)) / bx;
    return 1;
}

extern Mtx D_80089470;

Gfx *func_1510E634(Gfx *pkt, s32 arg1, s32 arg2) {
    gSPMatrix(pkt++, &D_80089470, G_MTX_LOAD);
    return pkt;
}
