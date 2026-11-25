#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct { f32 x, y, z; } TrailVec3;
typedef struct { f32 x, z; } TrailVec2;

/* The object a trail follows. */
typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    TrailVec3 pos;
    u8 pad20[0x1B];
    u8 unk3B;
} TrailOwner;

/* Optional anchor that overrides the owner's position. */
typedef struct {
    u8 pad0[0x40];
    TrailVec3 pos;
} TrailAnchor;

typedef struct {
    TrailOwner *owner;
    u8 unk4;
    u8 pad5[3];
    TrailAnchor *anchor;
    TrailVec2 scaled;
    TrailVec2 normal;
    u8 flags;
} TrailInfo;

typedef struct {
    TrailVec3 pos;
    f32 dist;
    f32 invLen;
    f32 unk14;
    TrailVec2 scaled;
    TrailVec2 normal;
} TrailEntry;

typedef struct {
    u8 pad0[0x10];
    TrailVec3 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 num;
    s8 tail;
    s8 head;
    u8 pad2F[0x65];
    TrailEntry *entries;
    TrailInfo *info;
} TrailObj;

/* Writes the point a trail is attached to: the anchor's position if there is one, else the owner's. */
s32 func_151CEC10(TrailVec3 *out, TrailOwner *owner, TrailAnchor *anchor) {
    if (anchor != NULL) {
        *out = anchor->pos;
    } else {
        out->x = owner->pos.x;
        out->y = owner->pos.y;
        out->z = owner->pos.z;
    }
    return TRUE;
}

extern f32 D_800AB00C;
extern f32 D_800AB010;
f32 func_15143E64(TrailVec3 *);

s32 func_151CEC54(TrailObj *trail) {
    TrailInfo *info;
    TrailEntry *entries;
    TrailOwner *owner;
    TrailAnchor *anchor;
    TrailVec3 prev;
    TrailVec3 delta;
    f32 dist;
    f32 len;
    f32 inv;
    f32 dx;
    f32 dz;
    f32 nx;
    f32 nz;

    info = trail->info;
    entries = trail->entries;
    owner = info->owner;
    anchor = info->anchor;
    if (owner != NULL) {
        if (owner->unk0 == 0 || owner->unk3B != info->unk4) {
            return 0;
        }
    }
    if (D_800BE9E4 > 0) {
        prev = trail->pos;
        if (func_151CEC10(&trail->pos, owner, anchor) != 0) {
            if (!(trail->flags & 4)) {
                prev = trail->pos;
            } else if (!(info->flags & 1)) {
                dx = trail->pos.x - prev.x;
                dz = trail->pos.z - prev.z;
                len = sqrtf(dx * dx + dz * dz);
                if (len != 0.0f) {
                    inv = 1.0f / len;
                    info->flags |= 1;
                    nz = dx * inv;
                    nx = -(dz * inv);
                    info->normal.x = nx;
                    info->scaled.x = nx * 51.0f;
                    info->normal.z = nz;
                    info->scaled.z = nz * 51.0f;
                }
            }
            trail->flags |= 4;
        } else {
            trail->flags &= ~4;
            return 1;
        }
        delta.x = trail->pos.x - prev.x;
        delta.y = trail->pos.y - prev.y;
        delta.z = trail->pos.z - prev.z;
        dist = func_15143E64(&delta);
        if (dist > D_800AB00C) {
            TrailEntry *e;

            e = &entries[trail->head];
            e->pos = trail->pos;
            e->dist = dist;
            len = sqrtf(delta.x * delta.x + delta.z * delta.z);
            if (len == 0.0f) {
                e->invLen = 1.0f;
            } else {
                e->invLen = 1.0f / len;
            }
            e->unk14 = 0.0f;
            e->scaled = info->scaled;
            e->normal = info->normal;
            trail->head++;
            if (trail->head == trail->count) {
                trail->head = 0;
            }
            trail->num++;
            if (trail->head == trail->tail) {
                trail->tail++;
                if (trail->tail == trail->count) {
                    trail->tail = 0;
                }
                trail->num--;
            }
            if ((info->flags & 2) && trail->num >= 3) {
                TrailEntry *e1;
                TrailEntry *e2;
                f32 headInv;
                s16 j;
                s16 k;
                f32 d1z;
                f32 d1x;
                f32 d2z;
                f32 d2x;
                f32 midInv;
                f32 scale;
                TrailVec2 n;
                f32 lenSq;

                j = trail->head - 1;
                if (j < 0) {
                    j = trail->count - 1;
                }
                j = j - 1;
                if (j < 0) {
                    j = trail->count - 1;
                }
                k = j - 1;
                if (k < 0) {
                    k = trail->count - 1;
                }
                e1 = &entries[j];
                e2 = &entries[k];
                headInv = e->invLen;
                midInv = e1->invLen;
                d1x = (e->pos.x - e1->pos.x) * headInv;
                d1z = (e->pos.z - e1->pos.z) * headInv;
                d2x = (e2->pos.x - e1->pos.x) * midInv;
                d2z = (e2->pos.z - e1->pos.z) * midInv;
                n.x = -(d1z - d2z);
                n.z = d1x - d2x;
                lenSq = n.x * n.x + n.z * n.z;
                if (lenSq > D_800AB010) {
                    f32 normInv;

                    scale = 51.0f;
                    normInv = 1.0f / sqrtf(lenSq);
                    n.x *= normInv;
                    n.z *= normInv;
                    e1->normal = n;
                    e1->scaled.x = n.x * scale;
                    e1->scaled.z = n.z * scale;
                } else {
                    e1->scaled.x = 0.0f;
                    e1->scaled.z = 0.0f;
                    e1->normal.x = 0.0f;
                    e1->normal.z = 0.0f;
                }
            }
        }
    }
    return 1;
}
