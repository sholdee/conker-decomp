#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One queued reflection: a model's mirrored matrices plus how to draw them. */
typedef struct ReflectionEntry {
    /* 0x00 */ u32 mtx;        // segment address of the mirrored matrices
    /* 0x04 */ u8 alpha;
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 objIndex;    // index into D_800CC2D0
    /* 0x09 */ u8 ownerId;     // unique_id of that object
    /* 0x0A */ u8 setId;       // attachment set id (attachment reflections only)
    /* 0x0B */ u8 isAttachment;
} ReflectionEntry;

extern u8 D_800C3F00;               // number of queued reflections
extern ReflectionEntry D_800C3F08[]; // reflection queue (15 entries)

/* Clears the reflection queue. */
void func_15034F20(void) {
    D_800C3F00 = 0;
}

void func_150A7A48(void *, void *, void *);
void guMtxF2L2(void *, void *);
void func_15034728(struct127 *);
s32 func_150AD960(s32, s32, s32, s32);

/*
 * Mirrors `count` matrices from `srcMtx` into `dstMtx` and sets the entry's alpha from the distance
 * to the mirror. Modes 0 and 1 mirror in the horizontal plane y = planeY; mode 2 mirrors in the
 * vertical plane through (planeX, planeZ) at `angle`. `srcIsFixed`: the sources are fixed point
 * (Mtx); `dstToFixed`: convert the results to fixed point.
 */
void func_15034F30(struct127 *obj, ReflectionEntry *entry, s32 mode, f32 (*srcMtx)[4][4],
                   f32 (*dstMtx)[4][4], s32 count, f32 planeX, f32 planeY, f32 planeZ, f32 unused,
                   f32 angle, f32 objX, f32 objY, f32 objZ, f32 fadeStart, f32 fadeEnd, f32 maxAlpha,
                   s32 srcIsFixed, s32 dstToFixed) {
    f32 flip[4][4];
    f32 rotA[4][4];
    f32 rotB[4][4];
    s32 i;
    f32 (*src)[4];
    f32 dist;
    f32 temp[4][4];
    struct255 *savedMtx;

    guMtxIdentF(flip);
    if (mode == 1 || mode == 0) {
        flip[1][1] = -1.0f;
        // matching: both loops need the byte offset (i << 6) spelled out; srcMtx[i] is CSE'd differently
        for (i = 0; i < count; i++) {
            if (srcIsFixed) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)srcMtx));
                src = temp;
            } else {
                srcMtx[i][0][3] = 0.0f;
                srcMtx[i][1][3] = 0.0f;
                srcMtx[i][2][3] = 0.0f;
                srcMtx[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)srcMtx);
            }
            func_150A7A48(src, flip, dstMtx[i]);
            dstMtx[i][3][1] = src[3][1] - 2.0f * (src[3][1] - planeY);
            if (dstToFixed) {
                bcopy((i << 6) + (u8 *)dstMtx, temp, sizeof(temp));
                guMtxF2L2(temp, (Mtx *)dstMtx + i);
            }
        }
        dist = objY - planeY;
        if (fadeEnd < dist) {
            entry->alpha = 0;
        } else if (dist < fadeStart) {
            if (mode == 0) {
                if (dist < 10.0f) {
                    entry->alpha = maxAlpha / 10.0f * dist;
                } else {
                    entry->alpha = maxAlpha;
                }
            } else {
                entry->alpha = maxAlpha;
            }
        } else {
            dist -= fadeStart;
            dist = maxAlpha * dist / (fadeEnd - fadeStart);
            dist = maxAlpha - dist;
            entry->alpha = dist;
        }
    } else if (mode == 2) {
        flip[0][0] = -1.0f;
        func_150A8050(rotA, 0.0f, -angle, 0.0f);
        func_150A8050(rotB, 0.0f, angle, 0.0f);
        for (i = 0; i < count; i++) {
            if (srcIsFixed) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)srcMtx));
                src = temp;
            } else {
                srcMtx[i][0][3] = 0.0f;
                srcMtx[i][1][3] = 0.0f;
                srcMtx[i][2][3] = 0.0f;
                srcMtx[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)srcMtx);
            }
            bcopy(src, dstMtx[i], sizeof(temp));
            dstMtx[i][3][0] -= planeX;
            dstMtx[i][3][1] -= planeY;
            dstMtx[i][3][2] -= planeZ;
            func_150A7A48(dstMtx[i], rotA, dstMtx[i]);
            func_150A7A48(dstMtx[i], flip, dstMtx[i]);
            func_150A7A48(dstMtx[i], rotB, dstMtx[i]);
            dstMtx[i][3][0] += planeX;
            dstMtx[i][3][1] += planeY;
            dstMtx[i][3][2] += planeZ;
            if (dstToFixed) {
                bcopy((i << 6) + (u8 *)dstMtx, temp, sizeof(temp));
                guMtxF2L2(temp, (Mtx *)dstMtx + i);
            }
        }
        dist = func_150AD960(planeX, planeZ, objX, objZ);
        if (fadeEnd < dist) {
            entry->alpha = 0;
        } else if (dist < fadeStart) {
            entry->alpha = maxAlpha;
        } else {
            dist -= fadeStart;
            dist = maxAlpha * dist / (fadeEnd - fadeStart);
            dist = maxAlpha - dist;
            entry->alpha = dist;
        }
    } else {
        return;
    }
    if (obj != NULL && obj->unk9C != 0) {
        savedMtx = obj->unk1D4;
        obj->unk1D4 = (struct255 *)dstMtx;
        func_15034728(obj);
        obj->unk1D4 = savedMtx;
    }
}


/* Reserves the next reflection queue slot, or returns NULL when the queue is full. */
ReflectionEntry *func_150356C8(void) {
    if (D_800C3F00 == 15) {
        return NULL;
    }
    D_800C3F00++;
    return &D_800C3F08[D_800C3F00 - 1];
}


/* Per model-type bounds, D_800D1C90[id] (only the fields used here). */
typedef struct ModelBounds {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u16 body_radius;
    /* 0x10 */ s16 body_height;
} ModelBounds;

s32 func_150A6360(s32, void *, f32, f32, f32, f32, f32, f32);
extern u8 D_800D9C10; // view matrices

/*
 * Returns 1 when the reflection of `obj`, whose mirrored root matrix is `mtx`, is outside the view.
 * In modes 0 and 1 the bounds centre is mirrored in the plane y = planeY.
 */
s32 func_15035714(s32 mode, struct127 *obj, f32 (*mtx)[4], f32 planeY) {
    f32 height;
    f32 radiusXZ;
    f32 radiusY;
    f32 radius;
    f32 y;
    s32 recp;
    ModelBounds *bounds;
    s32 id;
    f32 delta;

    recp = D_800BE628;
    id = obj->id;
    bounds = (ModelBounds *)D_800D1C90[id];
    radius = bounds->body_radius;
    radiusXZ = radius;
    radiusY = radius;
    radiusXZ *= obj->xz_scale;
    radiusY *= obj->y_scale;
    height = bounds->body_height * obj->y_scale;
    if (mode == 1 || mode == 0) {
        delta = (obj->y_position + height) - planeY;
        y = planeY - delta;
    } else {
        y = obj->y_position + height;
    }
    if (func_150A6360(recp, &D_800D9C10, mtx[3][0], y, mtx[3][2], radiusXZ, radiusY, 20000.0f) == 0) {
        return 1;
    }
    return 0;
}

struct List15035808 {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0x2D];
    s32 unk34;
    u8 pad38[0x10];
    u8 *unk48;
    u32 unk4C[2];
    struct List15035808 *unk54;
};

extern u16 D_800C3E7A;
extern u32 D_800C3E88;
extern u32 D_800C3E8C;
extern void *allocate_memory(s32, s32, s32, s32);
void func_1502D54C(s32, s32 *);
s32 func_1502DB20(u8);
s32 func_15031070(struct List15035808 *, struct127 *, struct255 **, s32 *);
s32 func_150AD9A0(s32, s32, s32);

void func_15035808(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9)
{
  ReflectionEntry *e;
  u32 mem;
  s32 col[3];
  struct255 *sp;
  s32 buf[4];
  struct List15035808 *node;
  struct List15035808 *next;
  s32 range;
  s32 n;
  s32 scale;
  s32 count;
  u8 a;
  u8 flag;
  f32 dist;
  s32 scale2;
  range = 0x578;
  if (D_800C3F00 == 15)
  {
    return;
  }
  if ((arg0 == 0) && (D_800CC2D0[arg1].interaction_state != 1))
  {
    return;
  }
  if (D_800CC2D0[arg1].unk1D4 == 0)
  {
    return;
  }
  if ((D_800CC2D0[arg1].unk74 & 0xF) == 15)
  {
    return;
  }
 goto dummy_label_68452; dummy_label_68452: ;
  a = D_800CC2D0[arg1].unk7;
  if (a == 0)
  {
    return;
  }
  n = func_1502DB20(D_800CC2D0[arg1].id);
  if (((u32) (D_800C3E7C << 6)) < ((D_800C3E88 + (n << 6)) - D_800C3E8C))
  {
    return;
  }
  if (D_800C35EA == 1)
  {
    range = 10000;
  }
  dist = func_150AD9A0(D_800DBFF0->unk2F8 - D_800CC2D0[arg1].x_position, D_800DBFF0->unk2FC - D_800CC2D0[arg1].y_position, D_800DBFF0->unk300 - D_800CC2D0[arg1].z_position);
  if (((f32) range) < dist)
  {
    return;
  }
  if (((f32) (range - 200)) < dist)
  {
    scale = ((((f32) range) - dist) * 256.0f) / ((f32) (range - 200));
  }
  else
  {
    scale = 0x100;
  }
  if (a != 0xFF)
  {
    scale *= a;
    scale >>= 8;
  }
  e = func_150356C8();
  if (e == 0)
  {
    return;
  }
  e->mtx = D_800C3E88;
  func_1502D54C(arg1, col);
  e->color[0] = col[0];
  e->color[1] = col[1];
  e->color[2] = col[2];
  e->objIndex = arg1;
  e->isAttachment = 0;
  e->ownerId = D_800CC2D0[arg1].unique_id;
  sp = D_800CC2D0[arg1].unk1D4;
  mem = e->mtx;
  func_15034F30(&D_800CC2D0[arg1], e, arg0, sp, mem, n, arg2, arg3, arg4, arg5, arg6, D_800CC2D0[arg1].x_position, D_800CC2D0[arg1].y_position, D_800CC2D0[arg1].z_position, arg7, arg8, arg9, 0, 0);
  e->alpha = (e->alpha * scale) >> 8;
  if (func_15035714(arg0, &D_800CC2D0[arg1], (f32 (*)[4]) sp, arg3) != 0)
  {
    D_800C3F00--;
    return;
  }
  D_800C3E88 += (n << 3) << 3;
  D_800C3E7A += n;
  node = (struct List15035808 *) D_800C3EE0;
  while (node != 0)
  {
    next = node->unk54;
    if ((D_800CC2D0[arg1].unique_id != node->unk0) || (node->unk3 == 0))
    {
      node = next;
      continue;
    }
    e = func_150356C8();
    if (e == 0)
    {
      return;
    }
    e->color[0] = col[0];
    e->color[1] = col[1];
    e->color[2] = col[2];
    e->objIndex = arg1;
    e->isAttachment = 1;
    e->ownerId = D_800CC2D0[arg1].unique_id;
    e->setId = node->unk6;
    if (func_15031070(node, &D_800CC2D0[arg1], &sp, buf) == 0)
    {
      node = next;
      continue;
    }
    count = 1;
    if (node->unk48 != 0)
    {
      count = node->unk48[0x3F4];
    }
    if (node->unk4C[D_800BE9C0] == 0)
    {
      node->unk4C[D_800BE9C0] = (u32) allocate_memory(count << 6, 1, 2, 0);
    }
    e->mtx = node->unk4C[D_800BE9C0];
    mem = e->mtx;
    scale2 = (((s32) arg9) * node->unk3) >> 8;
    if ((node->unk34 == 0) && (node->unk48 == 0))
    {
      flag = 0;
    }
    else
    {
      flag = 1;
    }
    func_15034F30(0, e, arg0, sp, mem, count, arg2, arg3, arg4, arg5, arg6, D_800CC2D0[arg1].x_position, D_800CC2D0[arg1].y_position, D_800CC2D0[arg1].z_position, arg7, arg8, scale2, flag, 1);
    node = next;
    e->alpha = (e->alpha * scale) >> 8;
  }

}

extern u8 D_80082FC0[];
extern u8 D_80083140[];

struct Obj15035D6C {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0xC];
    u8 unk13;
    u8 unk14;
    u8 pad15[0xF];
    Gfx **unk24;
};

Gfx *func_15035D6C(Gfx *gfx, struct Obj15035D6C *obj, s32 red, s32 green, s32 blue) {
    ReflectionEntry *entry;
    s32 alpha;
    s32 i;
    s32 j;

    if (D_800C3F00 == 0) {
        return gfx;
    }

    gSPSetGeometryMode(gfx++, G_CULL_FRONT);

    for (i = 0; i < D_800C3F00; i++) {
        entry = &D_800C3F08[i];
        if (entry->isAttachment != 1) {
            continue;
        }
        if (entry->ownerId != obj->unk0) {
            continue;
        }
        if (entry->setId != obj->unk6) {
            continue;
        }

        alpha = (entry->alpha * obj->unk3) >> 8;

        gDPPipeSync(gfx++);
        gSPSegment(gfx++, 3, entry->mtx);
        gDPSetEnvColor(gfx++, red, green, blue, alpha);

        if (alpha < 255) {
            gSPSegment(gfx++, 8, D_80082FC0);
        } else {
            gSPSegment(gfx++, 8, D_80083140);
        }

        for (j = 0; j < obj->unk14; j++) {
            if (!(obj->unk13 & (1 << j))) {
                gSPDisplayList(gfx++, obj->unk24[j]);
            }
        }
    }

    gSPClearGeometryMode(gfx++, G_CULL_FRONT);

    return gfx;
}

extern Gfx *func_1502CCFC(Gfx *, s32, s32, u32, s32, s32 *, s32, s32);

Gfx *func_15035FE8(Gfx *gfx, s32 arg1) {
    ReflectionEntry *e;
    s32 col[4];
    s32 i;

    if (D_800C3F00 == 0) {
        return gfx;
    }
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    for (i = 0; i < D_800C3F00; i++) {
        e = &D_800C3F08[i];
        if (e->isAttachment != 0) {
            continue;
        }
        col[0] = e->color[0];
        col[1] = e->color[1];
        col[2] = e->color[2];
        col[3] = 0xFF;
        gfx = func_1502CCFC(gfx, e->objIndex, arg1, e->mtx, e->alpha, col, 0, 1);
    }
    return gfx;
}

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);

void func_15036148(void) {
    s32 pad;
    s16 height;
    s16 sp90;
    s16 sp8E;
    s32 i;

    if (!(D_800DBFF0->unk5F0 & 1)) {
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state != 0 && (D_800CC2D0[i].unk5 == 0 || D_800CC2D0[i].unk5 == 1)) {
                if (D_800CC2D0[i].unk118 != -10000.0f && D_800CC2D0[i].unk118 < D_800CC2D0[i].y_position && D_800CC2D0[i].unk180 < D_800CC2D0[i].unk118 && D_800CC2D0[i].unk28 > 5.0f) {
                    func_1507C3E0(&D_800CC2D0[i], &height, &sp90, &sp8E);
                    if ((f32)height < D_800CC2D0[i].unk118 - D_800CC2D0[i].unk180) {
                        func_15035808(1, i, D_800CC2D0[i].x_position, D_800CC2D0[i].unk118, D_800CC2D0[i].z_position, 0.0f, 0.0f, 100.0f, 150.0f, 127.0f);
                    }
                }
            }
        }
    }
}
