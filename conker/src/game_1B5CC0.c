#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Joint-matrix trails ("afterimages") drawn behind objects. Each trail keeps
 * numFrames past copies of its owner's joint matrices, double-buffered on
 * D_800BE9C0, and draws them with increasing transparency.
 * D_800DF7C8[0] lists type-0 trails (struct127 owners),
 * D_800DF7C8[1] lists type-1 trails (struct131 owners, one matrix).
 */

/* Mtx viewed as its s16 integer and fraction halves. */
typedef struct MtxS16 {
    s16 intPart[4][4];
    s16 fracPart[4][4];
} MtxS16;

typedef struct Trail {
    /* 0x00 */ u8 type;      /* index into the update/free/draw tables */
    /* 0x01 */ u8 numFrames; /* history length */
    /* 0x02 */ u8 numFilled; /* history frames captured so far */
    /* 0x03 */ u8 alpha;
    /* 0x04 */ u8 maxDist;   /* max spacing between frames, see func_15189118 */
    /* 0x06 */ s16 timer;
    /* 0x08 */ MtxS16 *frames;
    /* 0x0C */ struct Trail *next;
    /* 0x10 */ union {
        struct127 *object;
        struct131 *s131;
    } owner;
    /* 0x14 */ s32 unk14;    /* type 0 only: swapped into owner.object->unk94 while drawing */
} Trail; // size 0x18 (type 1 trails are allocated without unk14)

#define sTrailLists ((Trail **)D_800DF7C8)

extern void *allocate_memory();
extern void func_10004074(void *);
extern s32 func_1502DB20(s32);
void func_15188A58(Trail *trail, Trail **list);

/* Starts (or extends) a type-0 trail behind a game object. */
void func_15188810(struct127 *object, s32 duration, s32 maxDist) {
    Trail *trail;
    s32 numJoints;
    s32 size;
    s32 pad; // matching: unused slot, moves size's spill to sp+0x20
    MtxS16 *frames;

    for (trail = sTrailLists[0]; trail != NULL; trail = trail->next) {
        if (object == trail->owner.object) {
            if (trail->timer < duration) {
                trail->timer = duration;
            }
            trail->maxDist = maxDist;
            return;
        }
    }

    trail = allocate_memory(sizeof(Trail), 1, 0, 0);
    if (trail != NULL) {
        numJoints = func_1502DB20(object->id);
        if (numJoints == 0) {
            func_10004074(trail);
            return;
        }

        size = numJoints * 3 * 2 * sizeof(Mtx);
        frames = allocate_memory(size, 1, 1, 1);
        if (frames == NULL) {
            func_10004074(trail);
            return;
        }

        trail->type = 0;
        trail->numFrames = 3;
        trail->numFilled = 0;
        trail->alpha = 20;
        trail->maxDist = maxDist;
        trail->timer = duration;
        trail->frames = frames;
        trail->owner.object = object;
        trail->unk14 = 0;
        bzero(frames, size);
        func_15188A58(trail, &sTrailLists[0]);
    }
}

/* Starts (or extends) a type-1 trail behind a struct131 object. */
void func_1518894C(struct131 *owner, s32 duration, s32 maxDist) {
    Trail *trail;
    MtxS16 *frames;

    for (trail = sTrailLists[1]; trail != NULL; trail = trail->next) {
        if (owner == trail->owner.s131) {
            if (trail->timer < duration) {
                trail->timer = duration;
            }
            trail->maxDist = maxDist;
            return;
        }
    }

    trail = allocate_memory(0x14, 1, 0, 0);
    if (trail != NULL) {
        frames = allocate_memory(3 * 2 * sizeof(Mtx), 1, 1, 0);
        if (frames == NULL) {
            func_10004074(trail);
            return;
        }

        trail->type = 1;
        trail->numFrames = 3;
        trail->numFilled = 0;
        trail->alpha = 20;
        trail->maxDist = maxDist;
        trail->timer = duration;
        trail->frames = frames;
        trail->owner.s131 = owner;
        bzero(frames, 3 * 2 * sizeof(Mtx));
        func_15188A58(trail, &sTrailLists[1]);
    }
}

/* Appends a trail to the end of a list. */
void func_15188A58(Trail *trail, Trail **list) {
    Trail *last;

    trail->next = NULL;
    if (*list != NULL) {
        last = *list;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = trail;
    } else {
        *list = trail;
    }
}

/* Lets an object's type-0 trail start fading out. */
void func_15188A9C(struct127 *object) {
    Trail *trail;

    for (trail = sTrailLists[0]; trail != NULL; trail = trail->next) {
        if (object == trail->owner.object) {
            trail->timer = 0;
        }
    }
}

/* Removes and frees an object's type-0 trails immediately. */
void func_15188AD0(struct127 *object) {
    Trail *trail;
    Trail *prev;
    Trail *next;

    prev = NULL;
    for (trail = sTrailLists[0]; trail != NULL; trail = next) {
        next = trail->next;
        if (object == trail->owner.object) {
            if (prev == NULL) {
                sTrailLists[0] = next;
            } else {
                prev->next = next;
            }
            func_100043B4((s32 *)trail->frames, 2);
            func_10004074(trail);
        } else {
            prev = trail;
        }
    }
}

extern void (*D_8008D580[])(Trail *); /* per-type update: capture a history frame */
extern void (*D_8008D588[])(Trail *); /* per-type free */

/* Advances every trail in list `listIndex`: counts down timers, captures history,
 * fades alpha in while the timer runs and out after it expires, freeing the
 * trail once fully faded. */
void func_15188B74(s32 listIndex) {
    Trail **head;
    Trail *trail;
    Trail *prev;
    Trail *next;
    s32 alpha;

    head = &sTrailLists[listIndex];
    prev = NULL;
    for (trail = *head; trail != NULL; trail = next) {
        next = trail->next;
        if (trail->timer != 0) {
            trail->timer -= D_800BE9E4;
        }
        if (trail->timer < 0) {
            trail->timer = 0;
        }
        D_8008D580[trail->type](trail);

        alpha = trail->alpha;
        if (trail->timer != 0) {
            if (alpha != 255) {
                alpha += D_800BE9E4 * 16;
                if (alpha >= 256) {
                    alpha = 255;
                }
                trail->alpha = alpha;
            }
            prev = trail;
        } else if (alpha != 0) {
            alpha -= D_800BE9E4 * 8;
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha == 0) {
                if (prev == NULL) {
                    *head = next;
                } else {
                    prev->next = next;
                }
                D_8008D588[trail->type](trail);
                func_10004074(trail);
            } else {
                trail->alpha = alpha;
                prev = trail;
            }
        }
    }
}

extern Mtx D_80089470;
extern Gfx *(*D_8008D590[])(Gfx *, Trail *, s16); /* per-type draw */

/* Draws every trail in list `listIndex`. */
Gfx *func_15188D00(Gfx *gfx, s32 listIndex, s16 player) {
    Trail *trail;

    trail = sTrailLists[listIndex];
    if (trail == NULL) {
        return gfx;
    }

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPPipeSync(gfx++);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    gDPSetConvert(gfx++, 0, 0, 0, 0, 0, 255);
    for (; trail != NULL; trail = trail->next) {
        gfx = D_8008D590[trail->type](gfx, trail, player);
    }
    return gfx;
}

/*
 * Type-0 update: shifts the history one frame. Frame i of the current half
 * (D_800BE9C0) takes frame i + 1 of the other half, and the last frame takes
 * the owner's current joint matrices.
 */
void func_15188E48(Trail *trail) {
    s32 i;
    s32 numFrames;
    s32 numJoints;
    s32 curBase;
    s32 prevBase;
    MtxS16 *src;
    MtxS16 *dst;
    s32 size;

    numFrames = trail->numFrames;
    numJoints = func_1502DB20(trail->owner.object->id);
    curBase = D_800BE9C0 * (numJoints * numFrames);
    prevBase = (D_800BE9C0 ^ 1) * (numJoints * numFrames);
    for (i = 0; i < numFrames; i++) {
        size = numJoints * sizeof(Mtx);
        if (i == numFrames - 1) {
            dst = (MtxS16 *)trail->owner.object->unk1D4;
            if (dst == NULL) {
                trail->numFilled = 0;
                return;
            }
            src = dst; // matching: sharing dst with the NULL-checked pointer steers regalloc
        } else {
            src = &trail->frames[(i + 1) * numJoints + prevBase];
        }
        dst = &trail->frames[i * numJoints + curBase];
        bcopy(src, dst, size);
    }
    if (trail->numFilled != trail->numFrames) {
        trail->numFilled++;
    }
}

/* Per-view list of the objects to draw this frame (see game_13F9D0.c). */
typedef struct DrawEntry {
    u16 index; // into D_800DBEF4
    u16 distance;
} DrawEntry;

extern DrawEntry **D_80089240[]; // per view
extern Mtx **D_80089250[];       // per frame buffer: one Mtx per object
extern u16 D_800DBEE8[];         // per view: entries in D_80089240

/*
 * Type-1 update: the one-matrix version of func_15188E48. The newest frame is
 * the owner's matrix from view 0's draw list; an owner that is not in the list
 * clears the history.
 */
void func_15188F84(Trail *arg) {
    Trail *trail;
    s32 i;
    s32 view;
    s32 numFrames;
    s32 curBase;
    s32 prevBase;
    MtxS16 *ownerMtx;
    MtxS16 *src;
    struct131 *objects;

    trail = arg; // matching: the copy puts trail in s2 and numFrames in s3
    view = 0;
    numFrames = trail->numFrames;
    curBase = D_800BE9C0 * numFrames;
    prevBase = (D_800BE9C0 ^ 1) * numFrames;
    for (i = 0; i < D_800DBEE8[view]; i++) {
        objects = D_800DBEF4;
        if (trail->owner.s131 == &objects[(*D_80089240[0])[i].index]) {
            break;
        }
    }
    if (i == D_800DBEE8[view]) {
        trail->numFilled = 0;
        return;
    }
    ownerMtx = (MtxS16 *)&(*D_80089250[D_800BE9C0])[(*D_80089240[0])[i].index];
    for (i = 0; i < numFrames; i++) {
        if (i == numFrames - 1) {
            src = ownerMtx;
        } else {
            src = &trail->frames[i + prevBase + 1];
        }
        bcopy(src, &trail->frames[curBase + i], sizeof(Mtx));
    }
    if (trail->numFilled != trail->numFrames) {
        trail->numFilled++;
    }
}

typedef struct Mtx15189118 {
    s16 i[4][4];
    s16 f[4][4];
} Mtx15189118;

typedef struct Sub15189118 {
    char pad0[4];
    u8 unk4;
    char pad5[0x1D4 - 5];
    Mtx15189118 *unk1D4;
} Sub15189118;

typedef struct Node15189118 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    char pad5[3];
    Mtx15189118 *unk8;
    struct Node15189118 *unkC;
    Sub15189118 *unk10;
} Node15189118;

void func_15189118(s32 idx) {
Node15189118 *node;
s32 count;
s32 m;
s32 base;
s32 i;
s32 j;
f32 maxdist;
Mtx15189118 *cur;
Mtx15189118 *from;
Mtx15189118 *q;
f32 dx;
f32 dy;
f32 dz;
f32 dist;
f32 ratio;
f32 cx;
f32 cy;
f32 cz;
f32 fx;
f32 fy;
f32 fz;

node = (Node15189118 *)D_800DF7C8[idx];
while (node != NULL) {
if (node->unk4 != 0) {
count = node->unk1;
m = func_1502DB20(node->unk10->unk4);
base = D_800BE9C0 * m * count;
maxdist = (u32)node->unk4;
for (i = node->unk2 - 1; i >= 0; i--) {
cur = node->unk8 + (i * m + base);
if (i + 1 == node->unk2) {
q = node->unk10->unk1D4;
if (q == NULL) {
node->unk2 = 0;
break;
}
from = q;
} else {
from = node->unk8 + ((i + 1) * m + base);
}
cx = ((cur->i[3][0] << 16) + cur->f[3][0]) * 1.52587890625e-05f;
cy = ((cur->i[3][1] << 16) + cur->f[3][1]) * 1.52587890625e-05f;
cz = ((cur->i[3][2] << 16) + cur->f[3][2]) * 1.52587890625e-05f;
fx = ((from->i[3][0] << 16) + from->f[3][0]) * 1.52587890625e-05f;
fy = ((from->i[3][1] << 16) + from->f[3][1]) * 1.52587890625e-05f;
fz = ((from->i[3][2] << 16) + from->f[3][2]) * 1.52587890625e-05f;
dist = sqrtf((cx - fx) * (cx - fx) + (cy - fy) * (cy - fy) + (cz - fz) * (cz - fz));
if (maxdist < dist && dist != 0.0f) {
ratio = maxdist / dist;
for (j = 0; j < m; j++) {
cx = ((cur->i[3][0] << 16) + cur->f[3][0]) * 1.52587890625e-05f;
cy = ((cur->i[3][1] << 16) + cur->f[3][1]) * 1.52587890625e-05f;
cz = ((cur->i[3][2] << 16) + cur->f[3][2]) * 1.52587890625e-05f;
fx = ((from->i[3][0] << 16) + from->f[3][0]) * 1.52587890625e-05f;
fy = ((from->i[3][1] << 16) + from->f[3][1]) * 1.52587890625e-05f;
fz = ((from->i[3][2] << 16) + from->f[3][2]) * 1.52587890625e-05f;
cx = (cx - fx) * ratio + fx;
cy = (cy - fy) * ratio + fy;
cz = (cz - fz) * ratio + fz;
cur->i[3][0] = (s32)cx;
cur->i[3][1] = (s32)cy;
cur->i[3][2] = (s32)cz;
cur->f[3][0] = 0;
cur->f[3][1] = 0;
cur->f[3][2] = 0;
cur++;
from++;
}
}
}
}
node = node->unkC;
}
}

void func_151895A4(s32 *arg0) {
    func_100043B4((s32 *)arg0[2], 2);
}

void func_151895CC(s32 *arg0) {
    func_100043B4((s32 *)arg0[2], 2);
}

typedef struct Node151895F4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    u8 *unk8;
    struct Node151895F4 *unkC;
    struct127 *unk10;
    s32 unk14;
} Node151895F4;

extern u8 func_150849CC(struct127 *, s32 *);
extern s32 func_1503DA9C(struct127 *, s32, s32, s32);
extern s32 func_1502C974(s32, s32, s32, s32, s32);
extern void *D_800DF7C0;
extern u8 D_800DF7C4;

s32 func_151895F4(s32 arg0, Node151895F4 *arg1, s16 arg2) {
    s32 i;
    s32 m;
    s32 count;
    s32 dst;
    s32 idx;
    s32 n;
    s32 shade;
    s32 active;
    void **model;
    s32 alpha;
    s32 lod;
    s32 p4;
    s32 p5;
    s32 saved;

    active = arg1->unk2;
    if (active == 0) {
        return arg0;
    }
    func_1503DA9C(arg1->unk10, func_150849CC(arg1->unk10, &lod), lod, 1);
    n = arg1->unk1;
    shade = arg1->unk3;
    m = func_1502DB20(arg1->unk10->id);
    count = arg1->unk2;
    dst = D_800BE9C0 * m * n;
    idx = arg1->unk10 - D_800CC2D0;
    if (idx < 0 || idx >= 25) {
        return arg0;
    }
    saved = *(s32 *)((u8 *)arg1->unk10 + 0x94);
    *(s32 *)((u8 *)arg1->unk10 + 0x94) = arg1->unk14;
    model = &D_800DF7C0;
    for (i = 0; i < count; i++) {
        alpha = (((i + 3) * 32) * shade) >> 8;
        *model = arg1->unk8 + ((dst + i * m) << 6);
        D_800DF7C4 = alpha;
        arg0 = func_1502C974(arg0, idx, arg2, 4, 1);
    }
    *(s32 *)((u8 *)arg1->unk10 + 0x94) = saved;
    return arg0;
}

typedef struct Model151897A4 {
    char pad0[0x8A];
    u8 unk8A;
} Model151897A4;

typedef struct Node151897A4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    u8 *unk8;
    struct Node151897A4 *unkC;
    Model151897A4 *unk10;
    s32 unk14;
} Node151897A4;

extern s32 func_151137D4(s32, Model151897A4 *, u8 *, s32, s32, s32);

s32 func_151897A4(s32 arg0, Node151897A4 *arg1, s16 arg2) {
    s32 i;
    s32 count;
    s32 shade;
    s32 n;
    s32 dst;
    s32 alpha;
    s32 pad1;
    s32 pad2;
    s32 saved;

    if (arg1->unk2 == 0) {
        return arg0;
    }
    count = arg1->unk2;
    n = arg1->unk1;
    shade = arg1->unk3;
    dst = D_800BE9C0 * n;
    saved = arg1->unk10->unk8A;
    for (i = 0; i < count; i++) {
        alpha = (((i + 3) * 32) * shade) >> 8;
        arg1->unk10->unk8A = alpha;
        arg0 = func_151137D4(arg0, arg1->unk10, arg1->unk8 + ((dst + i) << 6), arg2, 0, 0);
    }
    arg1->unk10->unk8A = saved;
    return arg0;
}

typedef struct Node151898C0 {
    char pad0[12];
    struct Node151898C0 *unkC;
    s32 unk10;
    s32 unk14;
} Node151898C0;

void func_151898C0(s32 arg0, s32 arg1) {
    Node151898C0 *node;

    node = (Node151898C0 *)D_800DF7C8[0];
    while (node != 0) {
        if (arg0 == node->unk10) {
            node->unk14 = arg1;
            return;
        }
        node = node->unkC;
    }
}
