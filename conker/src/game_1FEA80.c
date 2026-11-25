#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    char pad_0x0[0x30];
    s32 field_0x30;
} ConkerField30Object;

typedef struct {
    ConkerField30Object *field_0x0;
} ConkerField0Slot;

typedef struct {
    char pad_0x0[0x98];
    ConkerField0Slot *field_0x98;
} ConkerField98Owner;

extern void func_151478F4(void *);
extern void func_15147928(s32);

typedef struct {
    f32 x, y, z;
} Vec3_151D15D0;

typedef struct {
    f32 x, z;
} Vec2_151D15D0;

typedef struct {
    Vec3_151D15D0 pos;
    f32 speed;
    f32 invLen;
    u8 alpha;
    u8 pad15[3];
    f32 unk18;
    f32 unk1C;
    Vec2_151D15D0 n;
} Rec_151D15D0;

typedef struct {
    u8 pad0[0x18];
    Vec3_151D15D0 pos;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 speed;
} Inner_151D15D0;

typedef struct {
    u8 pad0[0x28];
    Inner_151D15D0 inner;
} Base_151D15D0;

typedef struct {
    Base_151D15D0 *base;
    f32 angle;
} Src_151D15D0;

typedef struct {
    u8 pad0[0x10];
    Vec3_151D15D0 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Rec_151D15D0 *recs;
    Src_151D15D0 *src;
} Obj_151D15D0;

extern f32 func_15144528(f32, f32, f32);

s32 func_151D15D0(Obj_151D15D0 *arg0) {
    Src_151D15D0 *src;
    Rec_151D15D0 *recs;
    Base_151D15D0 *base;
    Inner_151D15D0 *inner;

    src = arg0->src;
    recs = arg0->recs;
    base = src->base;
    inner = &base->inner;
    if (D_800BE9E4 > 0) {
        arg0->pos = inner->pos;
        arg0->flags |= 4;
        {
        Rec_151D15D0 *rec;
        f32 len;

        rec = &recs[arg0->head];
        rec->pos = arg0->pos;
        rec->speed = inner->speed;
        len = sqrtf(inner->vx * inner->vx + inner->vz * inner->vz);
        if (len == 0.0f) {
            rec->invLen = 1.0f;
        } else {
            rec->invLen = 1.0f / len;
        }
        rec->unk1C = 100.0f;
        rec->unk18 = src->angle;
        rec->alpha = 0xFF;
        src->angle = src->angle + rec->speed * 21.85000038f;
        src->angle = func_15144528(src->angle, 8192.0f, 0.0f);
        if (len) {
            rec->n.x = -inner->vz * rec->invLen;
            rec->n.z = inner->vx * rec->invLen;
        } else {
            rec->n.x = 0.0f;
            rec->n.z = 0.0f;
        }
        arg0->head++;
        if (arg0->head == arg0->count) {
            arg0->head = 0;
        }
        arg0->n++;
        if (arg0->head == arg0->tail) {
            arg0->tail++;
            if (arg0->tail == arg0->count) {
                arg0->tail = 0;
            }
            arg0->n--;
        }
        if (arg0->n >= 3) {
            s16 i0;
            s16 i1;
            s16 i2;

            i1 = arg0->head - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i1 = i1 - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i2 = i1 - 1;
            if (i2 < 0) {
                i2 = arg0->count - 1;
            }
            if (rec->speed) {
                f32 dx0;
                f32 dz0;
                f32 inv1;
                Vec2_151D15D0 d1;
                Rec_151D15D0 *p1;

                p1 = &recs[i1];
                if (p1->speed) {
                    Rec_151D15D0 *p2;
                    Vec2_151D15D0 nrm;
                    f32 lenSq;

                    p2 = &recs[i2];
                    inv1 = p1->invLen;
                    dx0 = (rec->pos.x - p1->pos.x) * rec->invLen;
                    dz0 = (rec->pos.z - p1->pos.z) * rec->invLen;
                    d1.x = (p2->pos.x - p1->pos.x) * inv1;
                    d1.z = (p2->pos.z - p1->pos.z) * inv1;
                    nrm.x = -(dz0 - d1.z);
                    nrm.z = dx0;
                    nrm.z -= d1.x;
                    lenSq = nrm.x * nrm.x + nrm.z * nrm.z;
                    if (lenSq > 9.999999747e-05f) {
                        f32 inv;

                        inv = 1.0f / sqrtf(lenSq);
                        nrm.x *= inv;
                        nrm.z *= inv;
                        p1->n = nrm;
                    } else {
                        p1->n.x = 0.0f;
                        p1->n.z = 0.0f;
                    }
                }
            }
        }
        }
    }
    return 1;
}

typedef struct {
    u8 pad0[0x14C];
    f32 unk14C;
} Inner_151D197C;

typedef struct {
    u8 pad0[0x28];
    Inner_151D197C *unk28;
} Base_151D197C;

typedef struct {
    Base_151D197C *base;
    u8 pad4[4];
    s8 unk8;
    u8 pad9[3];
    f32 maxLen;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Src_151D197C;

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Rec_151D15D0 *recs;
    Src_151D197C *src;
} Obj_151D197C;

s32 func_151D197C(Obj_151D197C *arg0) {
    Src_151D197C *src;
    Rec_151D15D0 *recs;
    f32 total;
    f32 inv;
    s32 i;
    s32 prev;
    Rec_151D15D0 *p;
    Rec_151D15D0 *q;
    f32 t;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 qx;
    f32 qy;
    f32 qz;

    src = arg0->src;
    recs = arg0->recs;
    if (src->base != NULL) {
        Inner_151D197C *inner = src->base->unk28;
        src->unk1C = inner->unk14C;
    }
    if (arg0->n >= 2) {
        total = 0;
        i = arg0->head - 1;
        if (i < 0) {
            i = arg0->count - 1;
        }
        while (i != arg0->tail) {
            prev = i;
            i--;
            if (i < 0) {
                i = arg0->count - 1;
            }
            p = &recs[prev];
            total += p->speed;
            if (src->maxLen < total) {
                if (p->speed != 0.0f) {
                    t = (total - src->maxLen) / p->speed;
                    q = &recs[i];
                    qx = q->pos.x;
                    qy = q->pos.y;
                    qz = q->pos.z;
                    dx = qx - p->pos.x;
                    dy = qy - p->pos.y;
                    dz = qz - p->pos.z;
                    q->pos.x = qx - dx * t;
                    q->pos.y = qy - dy * t;
                    q->pos.z = qz - dz * t;
                    p->speed = p->speed * (1.0f - t);
                }
                while (i != arg0->tail) {
                    arg0->tail++;
                    if (arg0->tail == arg0->count) {
                        arg0->tail = 0;
                    }
                    arg0->n--;
                }
                total = src->maxLen;
            }
        }
        inv = (total != 0.0f) ? 1.0f / total : 0;
        ;
    }
    if (arg0->n >= 2) {
        f32 a;
        f32 b;
        f32 d;
        s32 j;

        j = arg0->head;
        d = total;
        a = src->unk14 * src->unk1C * inv;
        b = src->unk18 * inv;
        do {
            j--;
            if (j < 0) {
                j = arg0->count - 1;
            }
            {
            Rec_151D15D0 *r = &recs[j];
            r->unk1C = src->unk10 * src->unk1C + d * a;
            r->alpha = d * b;
            d -= r->speed;
            }
        } while (j != arg0->tail);
    }
    if ((arg0->flags & 1) && (arg0->flags & 8)) {
        src->unk8 = arg0->unk1C * 6;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3f_151D1C98;

typedef struct Game1FEA80RibbonPoint {
    Vec3f_151D1C98 position;
    u8 pad0C[8];
    u8 alpha;
    u8 pad15[3];
    f32 texture;
    f32 width;
    f32 directionX;
    f32 directionZ;
} Game1FEA80RibbonPoint;

typedef struct Game1FEA80RibbonVertex {
    s16 x, y, z;
    s16 flag;
    s16 s, t;
    u8 r, g, b, a;
} Game1FEA80RibbonVertex;

typedef struct Game1FEA80RibbonState {
    u8 pad0[8];
    u8 alpha;
} Game1FEA80RibbonState;

void *memcpy(void *, const void *, u32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80091430[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

Gfx *func_151D1C98(u8 *arg0, Gfx *arg1, s16 arg2) {
    Game1FEA80RibbonState *state;
    Vec3f_151D1C98 next;
    Vec3f_151D1C98 first;
    Game1FEA80RibbonPoint *points;
    s32 index;
    s32 previous;
    u8 flag;
    u8 nextAlpha;
    f32 firstX;
    f32 firstZ;
    f32 nextX;
    f32 nextZ;
    f32 firstTexture;
    f32 nextTexture;
    u16 firstAlpha;
    Game1FEA80RibbonVertex *vtx;

    if (*(s8 *)(arg0 + 0x2C) < 2) {
        return arg1;
    }
    state = *(Game1FEA80RibbonState **)(arg0 + 0x98);
    points = *(Game1FEA80RibbonPoint **)(arg0 + 0x94);
    func_151D5D60(arg0 + 0x84, arg2, (arg0[0x25] * 2 + 10) * sizeof(Game1FEA80RibbonVertex), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    flag = 1;
    arg1 = (Gfx *)func_15142E24((s32)arg1, (s32)D_80091430, 0, 0, 0, 0, 0xBC, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x220005, 0x1D0600);
    arg1 = func_1513F4E4(arg1, 0x4C, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[1][0] | D_800A4AC8[1][1], &flag);
    previous = *(s8 *)(arg0 + 0x2E) - 1;
    if (previous < 0) {
        previous = arg0[0x25] - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = arg0[0x25] - 1;
    }
    first = points[previous].position;
    firstX = points[previous].width * points[previous].directionX;
    firstZ = points[previous].width * points[previous].directionZ;
    firstAlpha = (points[previous].alpha * state->alpha) >> 8;
    firstTexture = points[previous].texture;
    next = points[index].position;
    nextX = points[index].width * points[index].directionX;
    nextZ = points[index].width * points[index].directionZ;
    nextAlpha = (points[index].alpha * state->alpha) >> 8;
    nextTexture = points[index].texture;
    vtx->x = first.x + firstX;
    vtx->y = first.y;
    vtx->z = first.z + firstZ;
    vtx->a = firstAlpha;
    vtx->t = firstTexture;
    vtx->s = 0x7FF;
    vtx->flag = 0;
    vtx++;
    vtx->x = first.x - firstX;
    vtx->y = first.y;
    vtx->z = first.z - firstZ;
    vtx->a = firstAlpha;
    vtx->t = firstTexture;
    vtx->s = 0;
    vtx->flag = 0;
    vtx++;
    do {
        vtx->x = next.x + nextX;
        vtx->y = next.y;
        vtx->z = next.z + nextZ;
        vtx->a = nextAlpha;
        vtx->t = nextTexture;
        vtx->s = 0x7FF;
        vtx->flag = 0;
        vtx++;
        vtx->x = next.x - nextX;
        vtx->y = next.y;
        vtx->z = next.z - nextZ;
        vtx->a = nextAlpha;
        vtx->t = nextTexture;
        vtx->s = 0;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP2Triangles(arg1++, 0, 1, 2, 0, 1, 3, 2, 0);
        if (firstTexture < nextTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].t -= 0x2000;
            vtx++;
            vtx[-2].t -= 0x2000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        if (previous != *(s8 *)(arg0 + 0x2D)) {
            next = points[index].position;
            nextX = points[index].width * points[index].directionX;
            firstTexture = points[previous].texture;
            nextZ = points[index].width * points[index].directionZ;
            nextTexture = points[index].texture;
            nextAlpha = (points[index].alpha * state->alpha) >> 8;
        }
    } while (previous != *(s8 *)(arg0 + 0x2D));
    return arg1;
}

void func_151D223C(ConkerField98Owner *arg0) {
    ConkerField0Slot *temp_v0;
    ConkerField30Object *temp_v1;

    temp_v0 = arg0->field_0x98;
    temp_v1 = temp_v0->field_0x0;
    if (temp_v1 != 0) {
        temp_v1->field_0x30 = 0;
    }
}

extern void func_151478F4(void *);

void func_151D2258(s32 arg0) {
    func_151D223C(arg0);
    func_151478F4((void *)arg0);
}

extern void func_15147928(s32);

void func_151D2284(s32 arg0) {
    func_151D223C(arg0);
    func_15147928(arg0);
}
