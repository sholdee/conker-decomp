#include <ultra64.h>
#define func_150A7CB0 func_150A7CB0_hdr
#include "functions.h"
#undef func_150A7CB0
void func_150A7CB0(f32 mtx[4][4], f32 x, f32 y, f32 z);
#include "variables.h"

struct vtx151857DC;
extern struct vtx151857DC *(*D_8008D498[])(struct vtx151857DC *, struct vtx151857DC *, struct vtx151857DC *);
extern s32 (*D_8008D4C0[])(struct vtx151857DC *);

extern f32 D_800D3688;
extern s32 D_800DE01C;
extern s32 D_800DE020;
extern s32 D_800DE024;
extern s32 D_800DE030;
extern s32 D_800DE034;
extern s32 D_800DE038;
extern f32 D_800DE03C;
extern u8 D_800DE040;
extern u8 D_800DE041;

typedef struct {
    u8 pad0[0x30];
    f32 x;
    f32 y;
    f32 z;
    u8 pad3C[4];
} Bound15184150;

extern s32 func_1502DB20(s32);
extern u8 D_800C3E90;

s32 func_15184150(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3)
{
  s32 id;
  struct127 *obj;
  s32 count;
  s32 i;
  f32 dx;
  f32 dy;
  f32 dz;
  struct255 *new_var;
  obj = &D_800CC2D0[arg0];
  id = obj->id;
  *arg3 = 0.0f;
  *arg2 = 0.0f;
  *arg1 = 0.0f;
  if ((obj->unk1D4 == 0) || (D_800C3E90 != 0))
  {
    return 0;
  }
  count = func_1502DB20(id);
  for (i = 0; i < count; i++)
  {
    new_var = obj->unk1D4;
    dx = ((Bound15184150 *) new_var)[i].x;
    dy = ((Bound15184150 *) new_var)[i].y;
    dz = ((Bound15184150 *) new_var)[i].z;
    dx -= obj->x_position;
    dy -= obj->y_position;
    dz -= obj->z_position;
    dx = (dx < 0.0f) ? (-dx) : (dx);
    if ((*arg1) < dx)
    {
      *arg1 = dx;
    }
    dy = (dy < 0.0f) ? (-dy) : (dy);
    if ((*arg2) < dy)
    {
      *arg2 = dy;
      if (1)
      {
      }
      if (1)
      {
      }
      if (1)
      {
      }
      if (1)
      {
      }
      if (1)
      {
      }
    }
    dz = (dz < 0.0f) ? (-dz) : (dz);
    if (dz > (*arg3))
    {
      *arg3 = dz;
    }
  }

  if ((((*arg1) == 0.0f) || ((*arg2) == 0.0f)) || ((*arg3) == 0.0f))
  {
    dx = D_800D1C90[obj->id]->unk1A;
    dy = obj->xz_scale * dx;
    *arg3 = dy;
    *arg1 = dy;
    dx = D_800D1C90[obj->id]->unk1C + D_800D1C90[obj->id]->unk1E;
    *arg2 = obj->y_scale * dx;
  }
}

typedef struct Node15184368 {
    struct Node15184368 *next;
    u8 unk4;
    u8 pad5[4];
    u8 unk9;
    u8 padA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Node15184368;

extern struct17 D_800DE010;
extern struct17 D_800DDFB0[];
extern f32 D_800DF6F8;
extern f32 D_800DE028;
extern f32 D_800DE02C;
extern u8 D_800DF0E1;
extern f32 D_800DF0F8[][4][4];
extern u16 D_800DF0D0[];
extern Mtx D_800DF2F8[][2];
extern s32 D_800DF088;
extern u8 D_800DE048[];
extern f32 D_800D3680;
extern f32 D_800D3684;
extern f32 D_800D368C;
extern s8 D_8008D3F8[8][3];
extern s32 func_150AD9A0(s32, s32, s32);
extern void func_15047390(f32 mf[4][4], f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

f32 func_15184368(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    struct17 delta;
    struct17 pos;
    struct17 target;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 maxX;
    f32 maxY;
    f32 maxZ;
    f32 best;
    f32 scale;
    f32 minY;
    f32 mtxA[4][4];
    f32 mtxB[4][4];
    s32 i;
    s32 done;
    s32 count;
    Node15184368 *node;
    f32 npos[3];
    f32 fovScale;
    f32 v;
    struct127 *obj;
    struct17 *cam;

    obj = &D_800CC2D0[arg0];
    done = 0;
    best = 5000.0f;
    pos.unk0 = obj->x_position;
    fovScale = 2.0f;
    pos.unk4 = obj->y_position;
    count = 0;
    pos.unk8 = obj->z_position;
    if (D_800DE034 == 0) {
        node = (Node15184368 *)D_800DCD78;
        while (node != NULL) {
            if (node->unk4 == 0 && node->unk9 == 0) {
                npos[0] = node->unkE;
                npos[1] = node->unk10;
                npos[2] = node->unk12;
                v = func_150AD9A0(npos[0] - pos.unk0, npos[1] - pos.unk4, npos[2] - pos.unk8);
                if (v < best && arg5 < v) {
                    best = v;
                    count++;
                    target.unk0 = npos[0];
                    target.unk4 = npos[1];
                    target.unk8 = npos[2];
                }
            }
            node = node->next;
        }
    }
    if (count == 0) {
        if (arg5 != 0.0f) {
            return 0.0f;
        }
        if (D_800DE010.unk4 <= 0.0f) {
            D_800DE010.unk0 = 100.0f;
            D_800DE010.unk4 = 600.0f;
            D_800DE010.unk8 = -200.0f;
        }
        best = 1.0f;
        target.unk0 = D_800DE010.unk0 * D_800DE03C + pos.unk0;
        target.unk4 = D_800DE010.unk4 * D_800DE03C + pos.unk4;
        target.unk8 = D_800DE010.unk8 * D_800DE03C + pos.unk8;
    }
    if (*(s32 *)D_800DBFF4 != 0 || D_800DE038 >= 2) {
        cam = &D_800DDFB0[arg1];
        cam->unk0 = target.unk0;
        cam->unk4 = target.unk4;
        cam->unk8 = target.unk8;
    } else {
        cam = &D_800DDFB0[arg1];
        cam->unk0 = cam->unk0 + (target.unk0 - cam->unk0) * 0.3000000119f;
        cam->unk4 = cam->unk4 + (target.unk4 - cam->unk4) * 0.3000000119f;
        cam->unk8 = cam->unk8 + (target.unk8 - cam->unk8) * 0.3000000119f;
    }
    func_15048F90(&pos, cam, &delta);
    D_800DF6F8 = func_150AD930(&delta.unk0);
    if (D_800DF6F8 < 200.0f) {
        f32 ratio;
        ratio = 200.0f / D_800DF6F8;
        D_800DF6F8 = 200.0f;
        v = delta.unk0 * ratio;
        D_800DDFB0[arg1].unk0 += v - delta.unk0;
        delta.unk0 = v;
        v = delta.unk4 * ratio;
        D_800DDFB0[arg1].unk4 += v - delta.unk4;
        delta.unk4 = v;
        v = delta.unk8 * ratio;
        D_800DDFB0[arg1].unk8 += v - delta.unk8;
        delta.unk8 = v;
    }
    if (D_800DE040 != 0) {
        minY = sqrtf(delta.unk0 * delta.unk0 + delta.unk8 * delta.unk8) * 0.5773500204f;
        if (minY < 10.0f) {
            minY = 10.0f;
        }
    } else {
        minY = 10.0f;
    }
    if (delta.unk4 < minY) {
        D_800DDFB0[arg1].unk4 += minY - delta.unk4;
        delta.unk4 = minY;
        v = func_150AD9A0(delta.unk0, delta.unk4, delta.unk8);
        scale = D_800DF6F8 / v;
        D_800DF6F8 = v;
        D_800DDFB0[arg1].unk0 += delta.unk0 * scale - delta.unk0;
        D_800DDFB0[arg1].unk4 += delta.unk4 * scale - delta.unk4;
        D_800DDFB0[arg1].unk8 += delta.unk8 * scale - delta.unk8;
    }
    if (5000.0f * D_800DE03C < D_800DF6F8) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    if (D_800DF0E1 != 0) {
        arg3 *= 0.25f;
        D_800D3688 = -(D_800DE028 * D_800DE03C * 0.5f) - D_800DF6F8;
    } else {
        D_800D3688 = -D_800DE028 * D_800DE03C - D_800DF6F8;
    }
    func_15047390(D_800DF0F8[arg1], D_800DDFB0[arg1].unk0, D_800DDFB0[arg1].unk4, D_800DDFB0[arg1].unk8, pos.unk0, pos.unk4, pos.unk8, 0.0f, 1.0f, 0.0f);
    maxY = 0.0f;
    maxX = 0.0f;
    maxZ = -1000000.0f;
    for (i = 0; i < 8; i++) {
        func_150A7960((f32 *)D_800DF0F8[arg1], D_8008D3F8[i][0] * arg2 + pos.unk0, D_8008D3F8[i][1] * arg3 + pos.unk4,
                      D_8008D3F8[i][2] * arg4 + pos.unk8, &sx, &sy, &sz);
        if (sz == 0.0f) {
            sz = 9.999999747e-05f;
        }
        sx /= sz;
        sy /= sz;
        if (sx < 0.0f) {
            sx = -sx;
        }
        if (maxX < sx) {
            maxX = sx;
        }
        if (sy < 0.0f) {
            sy = -sy;
        }
        if (maxY < sy) {
            maxY = sy;
        }
        if (maxZ < sz) {
            maxZ = sz;
        }
    }
    if (maxX < 0.009999999776f || maxY < 0.009999999776f) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    D_800D3680 = 1.0f / maxX;
    D_800D3684 = 1.0f / maxY;
    if (D_800D3680 < 1.0f) {
        D_800D3680 = 1.0f;
    }
    if (D_800D3684 < 1.0f) {
        D_800D3684 = 1.0f;
    }
    if (maxZ < -1.0f) {
        D_800D368C = maxZ;
        do {
            f32 m = 1.0f;
            guPerspectiveF(mtxA, &D_800DF0D0[arg1], func_150484A0(1.0f, D_800D3684) * 57.29577637f,
                           func_150484A0(1.0f, D_800D3680) * 57.29577637f, -D_800D368C, -D_800D3688, fovScale);
            func_150A7A48(D_800DF0F8[arg1], mtxA, mtxA);
            for (i = 0; i < 16; i++) {
                v = ((f32 *)mtxA)[i];
                if (v < 0.0f) {
                    v = -v;
                }
                if (m < v) {
                    m = v;
                }
            }
            if (m < 32767.0f) {
                done = 1;
            } else {
                fovScale *= 0.5f;
            }
        } while (done == 0);
        func_150A7CB0(mtxB, 0.5f, 0.5f, 1.0f);
        func_150A7A48(mtxA, mtxB, mtxA);
        guMtxF2L(mtxA, &D_800DF2F8[arg1][D_800BE9C0]);
    } else {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    D_800DF088 = ((s32)D_800DE048 + 0x3F) & ~0x3F;
    D_800DE02C = 0xFF - (*((u8 *)&D_800CC2D0[arg0] + 0x1DD) >> 1);
    if (D_800DE02C > 185.0f) {
        D_800DE02C = 185.0f;
    }
    if (D_800CC2D0[arg0].unkCC < -10.0f) {
        D_800DE02C += D_800DE02C * (D_800CC2D0[arg0].unkCC + 10.0f) * 0.01999999955f;
        if (D_800DE02C <= 5.0f) {
            return 0.0f;
        }
    }
    return best;
}

extern s32 func_150AD9A0(s32, s32, s32);
extern s32 func_15184150(s32, f32 *, f32 *, f32 *);
extern f32 D_800D368C;

s32 func_15184DF0(s32 arg0, s32 arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    f32 x;
    f32 y;
    f32 z;
    s32 dist;

    x = D_800CC2D0[arg0].x_position;
    y = D_800CC2D0[arg0].y_position;
    z = D_800CC2D0[arg0].z_position;
    dist = func_150AD9A0(x - D_800DBFF0[arg1].unk2F8,
                         y - D_800DBFF0[arg1].unk2FC,
                         z - D_800DBFF0[arg1].unk300);
    if (dist > 2000) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    func_15184150(arg0, arg2, arg3, arg4);
    *arg2 *= 1.299999952f;
    *arg3 *= 1.299999952f;
    *arg4 *= 1.299999952f;
    if ((*arg2 == 0.0f) || (*arg3 == 0.0f) || (*arg4 == 0.0f)) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    return dist;
}

extern Gfx D_8008D448[];
extern Vp D_8008D3E8;
extern Mtx D_89470;
extern s16 D_80082FA6;
Gfx *func_1501A680(Gfx *arg0);
extern s32 func_1506196C(struct127 *, s32);
extern Gfx *func_1502CCFC(Gfx *, s32, s32, struct255 *, s32, s32 *, s32, s32);
Gfx *func_1510B7B4(Gfx *gfx, s32 idx);

Gfx *func_15184FA4(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    s32 lod;
    s32 col[3];

    if (D_800CC2D0[arg1].unk127 == 0xFF) {
        lod = D_800CC2D0[arg1].pad1C8 + 1;
    } else {
        lod = D_800CC2D0[arg1].pad1C8;
    }
    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_8b, 64, D_800DF088);
    gSPDisplayList(gfx++, D_8008D448);
    gSPPerspNormalize(gfx++, D_800DF0D0[arg2]);
    gSPViewport(gfx++, &D_8008D3E8);
    gSPMatrix(gfx++, &D_800DF2F8[arg2][D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_89470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    col[0] = col[1] = col[2] = 0;
    gfx = func_1502CCFC(gfx, arg1, arg3, D_800CC2D0[arg1].unk1D4, func_1506196C(&D_800CC2D0[arg1], arg3), col, 3, 1);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    if (lod < 2) {
        gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFCF87C);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, 255);
        gDPSetOtherMode(gfx++, 0x3C3F, 0x0F0A4000);
        gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP, G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, 0, 0, 0xFC, 0xFC, G_TX_RENDERTILE, 0x10, 0x10, 0x400, 0x400);
        if (lod == 0) {
            gDPPipeSync(gfx++);
            gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP, G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gfx++, 0, 0, 0xFC, 0xFC, G_TX_RENDERTILE, 0x10, 0x10, 0x400, 0x400);
        }
        gDPPipeSync(gfx++);
    }
    gSPSetGeometryMode(gfx++, G_SHADING_SMOOTH | G_ZBUFFER);
    gfx = (Gfx *)func_1501A490((s32)func_1501A680(gfx), D_80082FA6, 0, 0, 0, 0);
    gfx = func_1510B7B4(gfx, arg3);
    D_800CC2D0[arg1].unk123 = 1;
    return gfx;
}

struct vtx151857DC {
    f32 x;       /* 0x0 */
    f32 y;       /* 0x4 */
    f32 z;       /* 0x8 */
    s16 unkC;    /* 0xC */
    s16 unkE;    /* 0xE */
    s16 unk10;   /* 0x10 */
};

struct vtx151857DC *func_15185454(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;
    f32 y;

    t = (a0->z - a0->y) / ((a1->y - a0->y) - (a1->z - a0->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    y = a0->y + (a1->y - a0->y) * t;
    a2->z = y;
    a2->y = y;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185554(f32 *arg0) {
    return arg0[2] < arg0[1];
}

struct vtx151857DC *func_1518557C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (a0->z + a0->x) / ((a0->x - a1->x) + (a0->z - a1->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = -a2->x;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_1518567C(f32 *arg0) {
    return arg0[0] < -arg0[2];
}

struct vtx151856A8 {
    f32 x;
    f32 y;
    f32 z;
    s16 unkC;
    s16 unkE;
    s16 unk10;
};

struct vtx151856A8 *func_151856A8(struct vtx151856A8 *a0, struct vtx151856A8 *a1, struct vtx151856A8 *a2) {
    f32 t;

    t = (a0->z + a0->y) / ((a0->y - a1->y) + (a0->z - a1->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = -a2->y;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_151857B0(f32 *arg0) {
    return arg0[1] < -arg0[2];
}

struct vtx151857DC *func_151857DC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (a0->z - a0->x) / ((a1->x - a0->x) - (a1->z - a0->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = a2->x;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_151858D4(f32 *arg0) {
    return arg0[2] < arg0[0];
}

struct vtx151857DC *func_151858FC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (D_800D3688 - a0->z) / (a1->z - a0->z);
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = D_800D3688;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

extern f32 D_800D3688;

s32 func_151859FC(f32 *arg0) {
    return D_800D3688 < arg0[2];
}

extern f32 D_800D368C;

struct vtx151857DC *func_15185A28(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (D_800D368C - a0->z) / (a1->z - a0->z);
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = D_800D368C;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

extern f32 D_800D368C;

s32 func_15185B28(f32 *arg0) {
    return arg0[2] < D_800D368C;
}

struct vtx151857DC *func_15185B54(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = a0->x / (a0->x - a1->x);
    a2->x = 0.0f;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = a0->z + (a1->z - a0->z) * t;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185C44(f32 *arg0) {
    return arg0[0] <= 0.0f;
}

struct vtx151857DC *func_15185C6C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = a0->y / (a0->y - a1->y);
    a2->y = 0.0f;
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->z = a0->z + (a1->z - a0->z) * t;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185D5C(f32 *arg0) {
    return arg0[1] <= 0.0f;
}

s32 func_15185D84(f32 *arg0) {
    return arg0[0] > 0.0f;
}

s32 func_15185DAC(f32 *arg0) {
    return arg0[1] > 0.0f;
}

struct vtx151857DC *func_15185DD4(struct vtx151857DC *arg0, s32 arg1, s32 arg2, struct vtx151857DC *arg3) {
    struct vtx151857DC *cur;
    struct vtx151857DC *prev;
    struct vtx151857DC *(*emit)(struct vtx151857DC *, struct vtx151857DC *, struct vtx151857DC *);
    s32 (*test)(struct vtx151857DC *);
    s32 i;

    prev = &arg0[arg1 - 1];
    emit = D_8008D498[arg2];
    test = D_8008D4C0[arg2];
    cur = arg0;
    i = 0;
    if (arg1 > 0) {
        do {
            if (test(cur) != 0) {
                if (test(prev) != 0) {
                    *arg3++ = *cur;
                } else {
                    arg3 = emit(prev, cur, arg3);
                    *arg3++ = *cur;
                }
            } else {
                if (test(prev) != 0) {
                    arg3 = emit(prev, cur, arg3);
                }
            }
            i++;
            prev = cur;
            cur++;
        } while (i != arg1);
    }
    return arg3;
}

struct vin15185F24 {
    f32 x;
    f32 y;
    f32 z;
    s16 *col;
    s32 unk10;
};

s32 func_15185F24(struct vin15185F24 *arg0, s32 arg1, s32 arg2, Vtx_t *arg3, s32 *arg4, s32 arg5) {
    struct vtx151857DC buf[2][26];
    s32 i;
    s32 j;
    struct vtx151857DC *src;
    struct vtx151857DC *dst;
    f32 scale;
    s16 *col;
    s32 mid[3];
    s32 *p;

    src = (struct vtx151857DC *)arg0;
    dst = buf[0];
    for (i = 0; i < arg1; i++) {
        col = arg0[i].col;
        *dst = *src;
        dst->unkC = col[0];
        dst->unkE = col[1];
        dst->unk10 = col[2];
        dst++;
        src++;
    }
    for (i = 0; i < 6; i++) {
        arg1 = ((u32)func_15185DD4(buf[i & 1], arg1, i, buf[(i & 1) ^ 1]) - (u32)buf[(i & 1) ^ 1]) / 20;
    }
    mid[0] = 0;
    arg4[0] = 0;
    if (arg5 != 0) {
        for (j = 0; j < 2; j++) {
            mid[j + 1] = ((u32)func_15185DD4(buf[i & 1], arg1, j + 6, &buf[(i & 1) ^ 1][mid[j]]) - (u32)buf[(i & 1) ^ 1]) / 20;
        }
        i++;
        for (j = 0, p = arg4; j < 4;) {
            arg1 = mid[1] - mid[0];
            *p = ((u32)func_15185DD4(&buf[i & 1][mid[0]], arg1, (j++ >> 1) + 8, &buf[(i & 1) ^ 1][*p++]) - (u32)buf[(i & 1) ^ 1]) / 20;
            arg1 = mid[2] - mid[1];
            *p = ((u32)func_15185DD4(&buf[i & 1][mid[1]], arg1, (j++ >> 1) + 8, &buf[(i & 1) ^ 1][*p++]) - (u32)buf[(i & 1) ^ 1]) / 20;
        }
        arg1 = *p;
        i++;
    } else {
        arg4[1] = arg4[2] = arg4[3] = arg4[4] = arg1;
    }
    dst = buf[i & 1];
    for (i = 0; i < arg1; i++) {
        arg3[i].ob[0] = dst->unkC;
        arg3[i].ob[1] = dst->unkE;
        arg3[i].ob[2] = dst->unk10;
        scale = 1020.0f / dst->z;
        dst->x *= scale;
        dst->y *= scale;
        arg3[i].tc[0] = 1024.0f - dst->x;
        arg3[i].tc[1] = dst->y + 1024.0f;
        arg3[i].cn[0] = 0;
        arg3[i].cn[1] = 0;
        arg3[i].cn[2] = 0;
        arg3[i].cn[3] = (u32)(D_800DE02C - ((dst->z - D_800D368C) / (D_800D3688 - D_800D368C)) * D_800DE02C);
        arg3[i].flag = 0;
        dst++;
    }
    return arg1;
}

extern s32 func_1510F720(s32, s32, s32, u8 **);
extern s32 func_150A5B90(u32 *, s32, s32, s32);
extern u32 *D_800D3694;
extern void func_1510F800(s32);
extern s32 D_800D3690;
extern s32 D_800DF0E8;
extern s32 D_800DF0EC;
extern s32 D_800DF0F0;
extern s32 D_800DF0F4;

s32 func_1518652C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u8 *list[16];
    s32 count;
    u32 bits[187];
    u32 v;
    u8 *p;
    u32 max;
    s32 c;

    max = 0;
    func_1510F800(0);
    count = func_1510F720(arg0, arg1, arg2, list);
    if (count != 0) {
    bzero(bits, sizeof(bits));
    while (count != 0) {
        v = 0;
        count--;
        p = list[count] + 0xE;
        if (D_800DE041 != 0) {
            while (*p != 0) {
                c = *p;
                if (c & 0x80) {
                    v = *p++ << 8;
                    v |= *p++;
                    v &= 0x7FFF;
                } else {
                    v += c;
                    p += 1;
                }
                if (v < 0x1760) {
                    if ((*(u8 **)&D_800D3668)[v] == 1) {
                        bits[v >> 5] |= 1 << (v & 0x1F);
                        if (max < v) {
                            max = v;
                        }
                    }
                }
            }
        } else {
            while (*p != 0) {
                c = *p;
                if (c & 0x80) {
                    v = *p++ << 8;
                    v |= *p++;
                    v &= 0x7FFF;
                } else {
                    v += c;
                    p += 1;
                }
                if (v < 0x1760) {
                    bits[v >> 5] |= 1 << (v & 0x1F);
                    if (max < v) {
                        max = v;
                    }
                }
            }
        }
    }
    D_800D3690 = D_800DBE3C;
    D_800D3694 = &bits[max >> 5] + 1;
    D_800DF0E8 = arg0 - arg2;
    D_800DF0EC = arg0 + arg2;
    D_800DF0F0 = arg1 - arg2;
    D_800DF0F4 = arg1 + arg2;
    return (func_150A5B90(bits, arg3, arg4, arg5) - arg3) / 20;
    }
    return 0;
}

struct vtx151873E4 {
    f32 x;      /* 0x00 */
    f32 y;      /* 0x04 */
    f32 z;      /* 0x08 */
    s32 unkC;   /* 0x0C */
    s32 unk10;  /* 0x10 */
};              /* size 0x14 */
s32 func_151873E4(s32 arg0, s32 arg1, s32 arg2, struct vtx151873E4 *arg3, f32 *arg4, struct vtx151873E4 *arg5, s32 arg6);

extern s32 D_8008D3E0;
typedef struct { u8 pad0[0x40]; Vp vp[20]; } S180_15186794;
extern Gfx D_8008D410[];
extern s32 D_800C48F0[];
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern Vp D_800DF090[];
extern u8 D_800DF0E0;
extern s32 func_150849CC(struct127 *, s32 *);
extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_10004074(void *);
extern void func_1503DA9C(struct127 *, s32, s32, s32);

Gfx *func_15186794(Gfx *gfx, s32 arg1) {
    struct vtx151873E4 *buf;
    s32 count;
    Vtx *verts;
    u32 total;
    u32 idx;
    s32 flag;
    u32 j;
    s32 cnt;
    s32 k;
    s32 nv;
    s32 arr[5];
    s32 i;
    s32 n;
    s32 base;
    f32 *mtx;
    Gfx *saved;
    s32 v;
    s32 sub;
    f32 f;
    f32 sx;
    f32 sy;
    f32 sz;
    Gfx *vtxcmd;
    Gfx *start;
    s32 id;

    n = 0;
    start = gfx;
    if (arg1 != 0) {
        return gfx;
    }
    if (D_800DE01C < D_8008D3E0) {
        D_8008D3E0 = D_800DE01C;
        D_800DE028 *= 0.8000000119f;
    } else {
        if ((*(u16 *)&D_800B0DF0->pad1C[2]) == 0) {
            (*(u16 *)&D_800B0DF0->pad1C[2]) = 400;
        }
        if (D_8008D3E0 < D_800DE020) {
            D_800DE028 += ((f32)(u32)(*(u16 *)&D_800B0DF0->pad1C[2]) - D_800DE028) * 0.9499999881f;
        }
        if (D_8008D3E0 < D_800DE024) {
            D_8008D3E0 = D_800DE024;
        }
    }
    verts = allocate_memory(D_8008D3E0 * 0x10 + 0x1C0, 3, 1, 1);
    if (verts == NULL) {
        return gfx;
    }
    buf = allocate_memory(D_8008D3E0 * sizeof(struct vtx151873E4), 2, 1, 1);
    if (buf == NULL) {
        func_10004074(verts);
        return gfx;
    }
    memcpy(&D_800DF090[arg1], &((S180_15186794 *)D_800BE628)[D_80082FA4].vp[D_800BE9C0], sizeof(Vp));
    D_800DF090[arg1].vp.vtrans[2] = (s32)(D_800DF090[arg1].vp.vtrans[2] * (u32)D_800DE030) >> 10;
    saved = gfx;
    total = 0;
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 0 || D_800CC2D0[i].unk1D4 == NULL ||
            ((v = D_800CC2D0[i].unk74) & (1 << arg1)) || D_800CC2D0[i].unk5 != 0 ||
            (D_800CC2D0[i].pad66 & 0x10) != 0x10 ||
            (u32)(D_800CC2D0[i].unk74 & (1 << arg1)) == (u32)(1 << arg1) ||
            D_800CC2D0[i].unk84.uh == 0x2D8 ||
            (D_800CC2D0[i].unk31C != NULL && ((u8)D_800CC2D0[i].unk31C->unk95 != 0 || D_800CC2D0[i].unk31C->pad98[0] != 0)) ||
            (D_800DF0E0 & (1 << i))) {
            continue;
        }
        if (D_800CC2D0[i].unk65 != 0 && D_800CC2D0[i].id != 0x62) {
            continue;
        }
        if (D_800CC2D0[i].pad288[0x40] != 0) {
            id = func_150849CC(&D_800CC2D0[i], &sub);
            func_1503DA9C(&D_800CC2D0[i], id, sub, 0);
        } else {
            id = D_800CC2D0[i].id;
            sub = 0;
        }
        if (D_800C48F0[id] == 0) {
            continue;
        }
        if (func_15184DF0(i, arg1, &sx, &sy, &sz) == 0) {
            continue;
        }
        f = 0.0f;
        for (j = 0; j < D_800DE038 && n < 8 && total < D_8008D3E0; j++) {
            f = func_15184368(i, n, sx, sy, sz, f);
            if (f == 0.0f) {
                break;
            }
            mtx = (f32 *)D_800DF0F8[n];
            count = func_151873E4(D_800CC2D0[i].x_position, D_800CC2D0[i].z_position, 0xE6, buf, mtx, &buf[D_8008D3E0 - 2],
                                  func_1518652C(D_800CC2D0[i].x_position, D_800CC2D0[i].z_position, 0xE6, (s32)buf, (s32)mtx, (s32)&buf[D_8008D3E0 - 2]));
            if (count != 0) {
                idx = 0;
                gfx = func_15184FA4(gfx, i, n, arg1);
                cnt = 0;
                if (D_800CC2D0[i].unk127 != 0xFF && D_800CC2D0[i].pad1C8 < 2) {
                    flag = 1;
                } else {
                    if (i + 1 == D_800CC2D0[D_800CC2D0[i].unk124].unk65 && D_800CC2D0[D_800CC2D0[i].unk124].unk127 != 0xFF && D_800CC2D0[i].id == 0x53) {
                        flag = 1;
                    } else {
                        flag = 0;
                    }
                }
                gSPDisplayList(gfx++, D_8008D410);
                gSPViewport(gfx++, &D_800DF090[arg1]);
                gDPSetOtherMode(gfx++, 0x082C3F, 0x5049D8);
                gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                vtxcmd = gfx++;
                for (idx = 0; idx < count && total < D_8008D3E0; idx++) {
                    if ((s32)idx % 3 == 2) {
                        func_15185F24(&buf[idx - 2], 3, 0x1A, verts, arr, flag);
                        for (k = 0; k < 4; k++) {
                            nv = arr[k + 1] - arr[k];
                            cnt += nv;
                            verts += nv;
                            total += nv;
                            if (cnt > 16) {
                                gSPVertex(vtxcmd, &verts[-cnt], cnt - nv, 0);
                                vtxcmd = gfx++;
                                cnt = nv;
                            }
                            base = cnt - nv;
                            nv -= 2;
                            while (nv > 0) {
                                
                                if (nv >= 4) {
                                    { Gfx *_g = gfx++; _g->words.w0 = _SHIFTL(base, 23, 5) | 0x10000000 | _SHIFTL((base + nv) - 3, 18, 5) | _SHIFTL(((base + nv) - 2) >> 2, 15, 3) | _SHIFTL(base, 10, 5) | _SHIFTL((base + nv) - 2, 5, 5) | _SHIFTL((base + nv) - 1, 0, 5); _g->words.w1 = _SHIFTL(((base + nv) - 2), 30, 2) | _SHIFTL(base, 25, 5) | _SHIFTL((base + nv), 20, 5) | _SHIFTL((base + nv) + 1, 15, 5) | _SHIFTL(base, 10, 5) | _SHIFTL((base + nv) - 1, 5, 5) | _SHIFTL((base + nv), 0, 5); }
                                    nv -= 4;
                                } else if (nv >= 2) {
                                    gSP2Triangles(gfx++, base, (base + nv), (base + nv) + 1, 0, base, (base + nv) - 1, (base + nv), 0);
                                    nv -= 2;
                                } else {
                                    gSP1Triangle(gfx++, base, (base + nv), (base + nv) + 1, 0);
                                    nv -= 1;
                                }
                            }
                        }
                    }
                }
                if (cnt != 0) {
                    gSPVertex(vtxcmd, &verts[-cnt], cnt, 0);
                } else {
                    gfx = vtxcmd;
                }
                gDPPipeSync(gfx++);
                n++;
            }
        }
    }
    if (n != 0) {
        gfx = func_1510B7B4(gfx, arg1);
        gSPViewport(gfx++, &((S180_15186794 *)D_800BE628)[D_80082FA4].vp[D_800BE9C0]);
    } else {
        gfx = saved;
    }
    if (total >= D_8008D3E0) {
        D_8008D3E0 = total + 0x30;
    } else {
        D_8008D3E0 = total;
    }
    D_800DF0E0 = 0;
    D_800DF0E1 = 0;
    func_10004074(buf);
    if ((D_800BEBA4 < gfx - D_800BE9C8[D_800BE9C0]) ? 1 : 0) {
        return start;
    }
    return gfx;
}

void func_151872B0(s32 arg0) {
    D_800DE01C = 0x258;
    D_800DE020 = 0xB4;
    D_800DE024 = 0x5A;
    D_800DE030 = 0x3F8;
    D_800DE034 = 0;
    D_800DE038 = 1;
    D_800DE03C = 1.0f;
    D_800DE040 = 1;
    D_800DE041 = 1;

    switch (arg0) {
    case 0x21:
        D_800DE01C = 0x12C0;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE030 = 0x3F2;
        D_800DE034 = 1;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x22:
        D_800DE01C = 0x960;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x14:
        D_800DE041 = 0;
        break;
    case 0x29:
        D_800DE01C = 0x4B0;
        D_800DE020 = 0x258;
        D_800DE024 = 0x12C;
        break;
    }
}


typedef struct {
    u8  pad0[0x10];
    s16 unk10;
    u8  pad12[0x2];
    s16 unk14;
    u8  pad16[0x39];
    u8  unk4F;
    u16 unk50;
    u16 unk52;
    u8  pad54[0x1A];
    u8  unk6E;
    u8  pad6F[0x31];
} unkfunc_151873E4; /* size 0xA0 */

extern void func_1510F800(s32);
extern void func_150A44F0(s32, s16 *, s32);
extern struct vtx151873E4 *func_150A5E44(s16 *, struct vtx151873E4 *, f32 *, struct vtx151873E4 *);

extern s32 D_800D3690;
extern s16 D_800D37E0[];
extern s32 D_800DF0E4;
extern s32 D_800DF0E8;
extern s32 D_800DF0EC;
extern s32 D_800DF0F0;
extern s32 D_800DF0F4;

s32 func_151873E4(s32 arg0, s32 arg1, s32 arg2, struct vtx151873E4 *arg3, f32 *arg4, struct vtx151873E4 *arg5, s32 arg6) {
    unkfunc_151873E4 *entry;
    s32 found;
    s32 i;
    s32 radius;

    if (D_800DBEF0 == 0) {
        return arg6;
    }

    func_1510F800(2);

    arg3 += arg6;
    D_800DF0E8 = arg0 - arg2;
    D_800DF0EC = arg0 + arg2;
    D_800DF0F0 = arg1 - arg2;
    D_800DF0F4 = arg1 + arg2;

    found = 0;
    for (i = 0; i < D_800DBEF0; i++) {
        entry = (unkfunc_151873E4 *)&D_800DBEF4[i];
        if (entry->unk6E != 0) {
            continue;
        }
        if (entry->unk4F & 0x60) {
            continue;
        }
        radius = (entry->unk52 < entry->unk50) ? entry->unk50 : entry->unk52;
        if (D_800DF0EC < entry->unk10 - radius) {
            continue;
        }
        if (entry->unk10 + radius < D_800DF0E8) {
            continue;
        }
        if (D_800DF0F4 < entry->unk14 - radius) {
            continue;
        }
        if (entry->unk14 + radius < D_800DF0F0) {
            continue;
        }
        D_800D37E0[found] = i;
        found++;
    }

    if (found != 0) {
        func_150A44F0(found, D_800D37E0, 0);
        D_800D3690 = D_800DBE3C;
        D_800DF0E4 = found;
        arg6 += func_150A5E44(D_800D37E0, arg3, arg4, arg5) - arg3;
    }
    return arg6;
}

typedef struct {
    u8 pad0[0x80];
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    s16 unk94;
    s16 unk96;
    u8 unk98;
    u8 pad99[7];
} Particle151875E0;

typedef struct {
    u8 pad0[0x90];
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    Particle151875E0 *unkA4;
    s16 unkA8;
    s16 unkAA;
} Obj151875E0;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *allocate_memory(s32, s32, s32, s32);
void func_151875E0(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7);

typedef struct {
    u8  pad0[0x8C];
    f32 unk8C;
    f32 unk90;
    s16 unk94;
    s16 unk96;
    u8  pad98[0x8];
} Elem_15187978; /* size 0xA0 */

typedef struct {
    u8  pad0[0xA4];
    Elem_15187978 *unkA4;
    s16 unkA8;
    s16 unkAA;
} Obj_15187978;

extern s32 func_10004074(void *);
void func_15187978(Obj_15187978 *arg0);

extern void func_151EFE00(f32 *);
void func_15187D6C(f32 *arg0, f32 arg1, f32 arg2, f32 arg3);

