#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
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

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
} Point3s; /* size = 0x6 */

typedef struct {
    /* 0x000 */ u8 pad0[0x90];
    /* 0x090 */ Vtx vtx[2][4];
    /* 0x110 */ u8 *unk110;
    /* 0x114 */ f32 unk114;
    /* 0x118 */ f32 unk118;
    /* 0x11C */ f32 unk11C;
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 unk121;
    /* 0x122 */ u8 unk122;
    /* 0x123 */ u8 unk123;
    /* 0x124 */ u8 unk124;
} Struct_1510DA84;

extern f32 func_15048A40(u8 arg0);
extern f32 func_150489B0(u8 arg0);
extern s32 func_150A3FC4(f32, f32, s32, void *, f32 *);
s32 func_1510E388(Point3s **pts, Point3s *verts, f32 *outA, f32 *outB);
extern void func_1510E7A4(s32, Point3s *, f32 *, s32, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);

void func_1510DA84(Struct_1510DA84 *arg0) {
    s32 i;
    s32 alpha;
    f32 bx;
    f32 bz;
    u8 *o2;
    s32 pad2;
    s32 pad3;
    f32 v[2][3];
    f32 h;
    s32 pad5;
    f32 ratio;
    f32 outB;
    f32 outA;
    f32 tmp;
    f32 lenB;
    f32 lenA;
    f32 dist;
    f32 x;
    f32 top;
    f32 z;
    f32 y;
    f32 scale;
    Point3s **pts;
    Point3s *verts;
    u8 angle;
    f32 s;
    f32 c;
    s32 pad6;
    u8 *o;

    arg0->unk123 = 0;
    switch (arg0->unk120) {
        case 0:
            o2 = arg0->unk110;
#define o o2
            x = *(f32 *)(o + 0x14);
            top = *(f32 *)(o + 0x18);
            z = *(f32 *)(o + 0x1C);
            y = *(f32 *)(o + 0x180);
            if (o[4] == 0x25) {
                top = y;
            }
            pts = *(Point3s ***)(o + 0x188);
            verts = (Point3s *)(o + 0x18C);
            scale = *(f32 *)(o + 0x14C) * (f32)(u32)*(u16 *)(o + 0x160);
            angle = (*(u16 *)(o + 0x7A) >> 8) + 0x20;
            break;
#undef o
        case 1:
            o = arg0->unk110;
            if (o[0x6E] == 1) {
                return;
            }
            verts = (Point3s *)(*(u8 **)(o + 0x94) + 0x128);
            if (func_150A3FC4(((Point3s *)(o + 0x10))->x, ((Point3s *)(o + 0x10))->z, 0, verts, &y) == 0) {
                func_1510E7A4(0, verts, &y, 0, 0, 0, ((Point3s *)(o + 0x10))->x, ((Point3s *)(o + 0x10))->y,
                              ((Point3s *)(o + 0x10))->z, ((Point3s *)(o + 0x10))->y, o[0x71] + 1, 0, -10000.0f,
                              ((Point3s *)(o + 0x10))->y);
            }
            pts = NULL;
            x = ((Point3s *)(o + 0x10))->x;
            top = ((Point3s *)(o + 0x10))->y;
            z = ((Point3s *)(o + 0x10))->z;
            scale = *(f32 *)(o + 0x2C) * (f32)(o[0x4D] * 4);
            angle = 0;
            break;
    }
    arg0->unk114 = x;
    arg0->unk118 = z;
    arg0->unk11C = y;
    if (y > top) {
        return;
    }
lbl:
    if (func_1510E388(pts, verts, &outA, &outB)) {
    h = top - y;
    if (scale == 0.0f) {
        scale = 1.0f;
    }
    dist = 200.0f + h;
    if (dist == 0.0f) {
        dist = 9.99999974738e-05f;
    }
    if (dist > 800.0f) {
        dist = 800.0f;
    }
    ratio = scale / dist;
    if (arg0->unk124 & 1) {
        alpha = arg0->unk122;
        if (alpha != 0xFF) {
            alpha += D_800BE9E4 * 5;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
    } else {
        alpha = 32000.0f / dist;
        if (alpha > 0xA0) {
            alpha = 0xA0;
        }
    }
    arg0->unk122 = alpha;

    v[0][0] = 0.0;
    v[0][2] = -100.0f;
    if (angle != 0) {
        tmp = v[0][0];
        s = func_15048A40(angle);
        c = func_150489B0(angle);
        v[0][0] = v[0][2] * s + v[0][0] * c;
        v[0][2] = v[0][2] * c - tmp * s;
    }
    v[1][0] = -100.0f;
    v[1][2] = 0.0;
    v[0][1] = outA * v[0][0] + v[0][2] * outB;
    if (angle != 0) {
        tmp = v[1][0];
        v[1][0] = v[1][2] * s + v[1][0] * c;
        v[1][2] = v[1][2] * c - tmp * s;
    }
    v[1][1] = outA * v[1][0] + v[1][2] * outB;

    lenA = v[0][0] * v[0][0] + v[0][1] * v[0][1] + v[0][2] * v[0][2];
    if (lenA > 100000000.0f) {
        lenA = 100000000.0f;
    }
    if (lenA < 10000.0f) {
        lenA = 10000.0f;
    }
    lenA = sqrtf(lenA);
    lenB = v[1][0] * v[1][0] + v[1][1] * v[1][1] + v[1][2] * v[1][2];
    if (lenB > 100000000.0f) {
        lenB = 100000000.0f;
    }
    if (lenB < 10000.0f) {
        lenB = 10000.0f;
    }
    lenB = sqrtf(lenB);

    for (i = 0; i < 3; i++) {
        v[0][i] = v[0][i] * 100.0f * ratio / lenA;
        v[1][i] = v[1][i] * 100.0f * ratio / lenB;
        arg0->vtx[D_800BE9C0][0].v.ob[i] = v[0][i];
        arg0->vtx[D_800BE9C0][1].v.ob[i] = v[1][i];
        arg0->vtx[D_800BE9C0][2].v.ob[i] = -v[0][i];
        arg0->vtx[D_800BE9C0][3].v.ob[i] = -v[1][i];
    }
    arg0->unk123 = 1;
    }
}

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
