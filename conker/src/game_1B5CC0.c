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

typedef struct Node15188B74 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[2];
    s16 unk6;
    char pad8[4];
    struct Node15188B74 *unkC;
} Node15188B74;

extern void (*D_8008D580[])(Node15188B74 *);
extern void (*D_8008D588[])(Node15188B74 *);

void func_15188B74(s32 idx) {
    Node15188B74 *node;
    Node15188B74 *prev;
    Node15188B74 *next;
    volatile s32 *delta;
    s32 *head;
    void (**funcs0)(Node15188B74 *);
    void (**funcs1)(Node15188B74 *);
    s32 countdown;
    s32 fade;
    s32 maxFade;

    head = &D_800DF7C8[idx];
    node = (Node15188B74 *)*head;
    funcs1 = D_8008D588;
    prev = 0;
    if (node != 0) {
        funcs0 = D_8008D580;
        delta = &D_800BE9E4;
        maxFade = 0xFF;
        do {
            countdown = node->unk6;
            next = node->unkC;
            if (countdown != 0) {
                node->unk6 = countdown - *delta;
                countdown = node->unk6;
            }
            if (countdown < 0) {
                node->unk6 = 0;
            }
            funcs0[node->unk0](node);

            fade = node->unk3;
            if (node->unk6 != 0) {
                if (fade != maxFade) {
                    fade += *delta << 4;
                    if (fade >= 0x100) {
                        fade = maxFade;
                    }
                    node->unk3 = fade;
                }
                prev = node;
            } else if (fade != 0) {
                fade -= *delta << 3;
                if (fade < 0) {
                    fade = 0;
                }
                if (fade == 0) {
                    if (prev == 0) {
                        *head = (s32)next;
                    } else {
                        prev->unkC = next;
                    }
                    funcs1[node->unk0](node);
                    func_10004074(node);
                } else {
                    node->unk3 = fade;
                    prev = node;
                }
            }
            node = next;
        } while (node != 0);
    }
}

typedef struct Node15188D00 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    s32 unk8;
    struct Node15188D00 *unkC;
    s32 unk10;
    s32 unk14;
} Node15188D00;

extern Mtx D_80089470;
extern Gfx *(*D_8008D590[])(Gfx *, Node15188D00 *, s16);

#define WGFX15188D00(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *func_15188D00(Gfx *gfx, s32 idx, s16 arg2) {
    Node15188D00 *node;
    Gfx *(**callbacks)(Gfx *, Node15188D00 *, s16);
    s16 tmp;

    tmp = arg2;
    node = (Node15188D00 *)D_800DF7C8[idx];
    if (node == 0) {
        return gfx;
    }

    WGFX15188D00(gfx++, 0xDA380003, &D_80089470);
    WGFX15188D00(gfx++, 0xD9FFFFFF, 0x00200004);
    WGFX15188D00(gfx++, 0xD9EEFFFF, 0x00000000);
    WGFX15188D00(gfx++, 0xE7000000, 0x00000000);
    WGFX15188D00(gfx++, 0xE2001E01, 0x00000000);
    WGFX15188D00(gfx++, 0xEC000000, 0x000000FF);
    callbacks = D_8008D590;
    while (node != 0) {
        gfx = callbacks[node->unk0](gfx, node, tmp);
        node = node->unkC;
    }
    return gfx;
}

typedef struct Sub15188E48 {
    char pad0[4];
    u8 unk4;
    char pad5[0x1D4 - 5];
    u8 *unk1D4;
} Sub15188E48;

typedef struct Node15188E48 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    u8 *unk8;
    struct Node15188E48 *unkC;
    Sub15188E48 *unk10;
} Node15188E48;

void func_15188E48(Node15188E48 *arg0) {
    s32 i;
    s32 count;
    s32 m;
    s32 dst;
    s32 off;
    u8 *from;
    u8 *q;
    s32 len;

    count = arg0->unk1;
    m = func_1502DB20(arg0->unk10->unk4);
    dst = D_800BE9C0 * (m * count);
    off = (D_800BE9C0 ^ 1) * (m * count);
    for (i = 0; i < count; i++) {
        len = m << 6;
        if (i == count - 1) {
            q = arg0->unk10->unk1D4;
            if (q == NULL) {
                arg0->unk2 = 0;
                return;
            }
            from = q;
        } else {
            from = arg0->unk8 + ((((i + 1) * m) + off) << 6);
        }
        q = arg0->unk8 + ((i * m + dst) << 6);
        bcopy(from, q, len);
    }
    if (arg0->unk2 != arg0->unk1) {
        arg0->unk2++;
    }
}

typedef struct Entry15188F84 {
    u16 index;
    u16 value;
} Entry15188F84;

typedef struct Node15188F84 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    char pad4[4];
    u8 *unk8;
    struct Node15188F84 *unkC;
    u8 *unk10;
} Node15188F84;

extern Entry15188F84 **D_80089240;
extern u8 **D_80089250[];
extern u16 D_800DBEE8[];

void func_15188F84(Node15188F84 *node) {
    s32 i;
    s32 count;
    s32 dst;
    s32 off;
    s32 view;
    u8 *src;
    u8 *from;
    s32 next;
    u8 *p;
    Node15188F84 *arg0;

    arg0 = node;
    view = 0;
    count = arg0->unk1;
    dst = D_800BE9C0 * count;
    off = (D_800BE9C0 ^ 1) * count;
    for (i = 0; i < D_800DBEE8[view]; i++) {
        p = (u8 *)D_800DBEF4;
        if (arg0->unk10 == p + (*D_80089240)[i].index * 0xA0) {
            break;
        }
    }
    if (i == D_800DBEE8[view]) {
        arg0->unk2 = 0;
        return;
    }
    src = *D_80089250[D_800BE9C0] + ((*D_80089240)[i].index << 6);
    for (i = 0; i < count; i++) {
        if (i == count - 1) {
            from = src;
        } else {
            from = arg0->unk8 + ((i + off + 1) << 6);
        }
        bcopy(from, arg0->unk8 + ((dst + i) << 6), 0x40);
    }
    if (arg0->unk2 != arg0->unk1) {
        arg0->unk2++;
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
