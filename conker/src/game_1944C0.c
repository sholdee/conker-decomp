#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * Object type descriptors: D_8008B4A8[type], 0x34 bytes each (struct115 in structs.h only names unk0/unk18).
 * D_8008B4D0 / D_8008B4D4 in variables.h are this table's +0x28 / +0x2C fields seen as struct115.
 */
typedef struct {
    /* 0x00 */ void (*unk0)(struct102 *);
    /* 0x04 */ void (*unk4)(struct102 *);
    /* 0x08 */ Gfx *(*draw)(Gfx *gfx, struct102 *obj, s16 arg2);
    /* 0x0C */ void (*unkC)(struct102 *);
    /* 0x10 */ Gfx *drawBeginDL;
    /* 0x14 */ Gfx *(*drawBegin)(Gfx *gfx, s32 type, s32 list, s16 arg3);
    /* 0x18 */ void (*unk18)(void);
    /* 0x1C */ void (*handleEvent)(struct102 *obj, s32 event, u8 eventType);
    /* 0x20 */ Gfx *(*drawEnd)(Gfx *gfx, s32 type, s16 arg2);
    /* 0x24 */ Gfx *drawEndDL;
    /* 0x28 */ void (*destroy)(struct102 *);
    /* 0x2C */ void (*free)(struct102 *);
    /* 0x30 */ s8 drawMode;
    /* 0x31 */ u8 pad31[3];
} ObjType;

#define OBJ_TYPE_COUNT 104
#define OBJ_TYPES ((ObjType *)D_8008B4A8)

/* Object lists: D_800DCE50[list][type] is the head of a doubly linked list of struct102 nodes. */
extern struct102 *D_800DCE50[2][OBJ_TYPE_COUNT];

/*
 * While a list is being walked, the next node is saved in D_800DD198[depth] (depth D_800DD190) so that
 * callbacks may destroy the current node; func_151696DC repairs saved entries that point at a dying node.
 */
#define OBJ_ITER_NEXT ((struct102 **)D_800DD198)

/* Link fields shared by every object (struct102 has only u8 fields at 0x4). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

extern void (*D_8008CA20[])(struct102 *);
extern void (*D_8008C9C8[])(struct102 *);
extern void (*D_8008CB64[3])(void);

extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;

/* Template / payload of a type 0x10 object. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ f32 rotZ;
    /* 0x18 */ u8 pad18[0x18];
    /* 0x30 */ f32 scaleX;
    /* 0x34 */ f32 scaleY;
    /* 0x38 */ f32 scaleZ;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ Gfx *displayList;
    /* 0x44 */ u8 pad44[0x10];
    /* 0x54 */ u8 callback;     /* index into D_8008CA20, 0 = none */
    /* 0x55 */ u8 primA;
    /* 0x56 */ u8 tileUlt;
    /* 0x57 */ u8 pad57[5];
    /* 0x5C */ u8 renderMode;
    /* 0x5D */ u8 combineMode;
} ModelFxData;

/* Type 0x10 object (0xF0 bytes): a display list drawn with a per-frame matrix (created by func_15168BE4). */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ Mtx mtx[2];      /* double-buffered by D_800BE9C0 */
    /* 0x90 */ ModelFxData data;
} ModelFx;

Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);


/*
 * Sprite animation: `frames` is a single (segmented, < 0x10000000) texture address or a pointer to a table of
 * `frameCount` texture addresses.
 */
typedef struct {
    /* 0x0 */ s32 *frames;
    /* 0x4 */ u8 frameCount;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 unk6;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u8 unkA;
} SpriteAnim;

/* Type 3 object (0x28 bytes): a fading, animated billboard sprite. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ SpriteAnim *anim;
    /* 0x14 */ s16 frame;       /* 8.8 fixed point index into anim->frames */
    /* 0x16 */ s16 frameStep;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 z;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s8 fadeStep;     /* > 0: fade out by this per frame; < 0: fade on the last frame */
    /* 0x23 */ u8 alpha;
    /* 0x24 */ u8 callback;     /* index into D_8008CA20, 0 = none */
} SpriteFx;

/* Sprite placement passed to func_15095760. */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ u8 alpha;
    /* 0xB */ u8 unkB;
    /* 0xC */ u8 unkC;
    /* 0xD */ u8 flip;         /* bit 0: flip s, bit 1: flip t */
} SpriteParams;

/* Particle flags (Particle.flags). */
#define PARTICLE_ENV_COLOR      0x1    /* use envR/G/B instead of the per-texture default */
#define PARTICLE_PINGPONG       0x2    /* reverse the animation at either end instead of wrapping */
#define PARTICLE_FLIP_S         0x4
#define PARTICLE_FLIP_T         0x8
#define PARTICLE_DIE_AT_END     0x20
#define PARTICLE_DIE_AT_START   0x40
#define PARTICLE_LOOPED         0x80   /* set once the animation has passed its last frame */
#define PARTICLE_CUSTOM_MODE    0x100  /* render/combine modes come from unk14 */
#define PARTICLE_2CYCLE         0x200

/* Type 5 / 0x42 object: a moving, animated billboard particle (created by func_15167D84). */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ SpriteAnim *anim;
    /* 0x14 */ s32 unk14;       /* PARTICLE_CUSTOM_MODE: bits 0-7 render mode, bits 16-31 combine mode */
    /* 0x18 */ s32 unk18;       /* low byte: angle offset in 1/256ths of a turn */
    /* 0x1C */ s16 frame;       /* 8.8 fixed point index into anim->frames */
    /* 0x1E */ s16 frameStep;
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ u8 subX;         /* fractional (1/256) parts of x, z, y */
    /* 0x2D */ u8 subZ;
    /* 0x2E */ u8 subY;
    /* 0x2F */ s8 callback;     /* index into D_8008C9C8, -1 = none */
    /* 0x30 */ s16 velY;
    /* 0x32 */ s16 accelY;
    /* 0x34 */ s16 width;       /* < 0: scaled by the viewing angle */
    /* 0x36 */ s16 height;      /* < 0: scaled by the viewing angle */
    /* 0x38 */ s16 life;        /* 0x7FFF = forever */
    /* 0x3A */ s8 fadeStep;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ u8 primR;
    /* 0x3D */ u8 primG;
    /* 0x3E */ u8 primB;
    /* 0x3F */ u8 primA;
    /* 0x40 */ u8 envR;
    /* 0x41 */ u8 envG;
    /* 0x42 */ u8 envB;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ u16 flags;
    /* 0x46 */ u8 pad46[2];
    /* 0x48 */ s8 onDeath;      /* index into D_8008CA20, -1 = none */
} Particle;


typedef struct {
    char pad_0[0x1];
    u8 field_0x1;
    char pad_0x2[0xA];
    u8 field_0xC;
    char pad_0xD[0x3];
    u8 field_0x10;
    char pad_0x11[0x12];
    u8 field_0x23;
    char pad_0x24[0x4];
    char pad_0x28[0x20];
    u8 field_0x48;
} GameObjectHeader;

extern SpriteAnim *D_8008CA4C[];

void func_15169070(s32 first, s32 last, s32 event, u8 eventType);

/* Calls every object type's per-frame global hook (D_8008B4A8[i].unk18). */
void func_15167010(void) {
    struct115 *entry;
    struct115 *end;
    void (*func)(void);
    s32 unused;

    entry = D_8008B4A8;
    end = (struct115 *)((u32)entry + 101 * sizeof(struct115));
    unused = 0;
    // matching: a goto loop plus the dead `unused` (saved s1) give the target's base-relative end
    // and sltu exit; for/while/do forms fold `end` to a constant address
loop:
    func = (void (*)(void))entry->unk18;
    if (func != NULL) {
        func();
    }
    entry++;
    unused ^= 0;
    if ((u32)entry < (u32)end) {
        goto loop;
    }
}

/* Calls each of the three hooks in D_8008CB64 that is set. */
void func_1516706C(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8008CB64[i] != NULL) {
            D_8008CB64[i]();
        }
    }
}

/* Runs every type's unk0 callback on each object of both lists. */
void func_151670C0(void) {
    s32 list;
    s32 type;
    struct102 *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (OBJ_TYPES[type].unk0 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    OBJ_TYPES[type].unk0(node);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Runs every type's unk4 callback on each object of both lists. */
void func_151671E8(void) {
    s32 list;
    s32 type;
    struct102 *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (OBJ_TYPES[type].unk4 != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    OBJ_TYPES[type].unk4(node);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

extern f32 D_800DD1D0[];

/* Caches sin/cos of each player's unk3A0 angle, then runs every type's unkC callback on its objects. */
void func_15167310(void) {
    s32 k;
    f32 angle;

    for (k = 0; k < D_80082FA0 + 1; k++) {
        angle = D_800DBFF0[k].unk3A0;
        D_800DD1D8[k] = sinf(angle);
        D_800DD1E8[k] = cosf(angle);
    }
    angle = D_800DBFF0->unk3A0;
    D_800DD1D0[0] = sinf(angle);
    D_800DD1D0[1] = cosf(angle);
    {
        s32 list;
        s32 type;
        struct102 *node;

        for (type = 0; type != OBJ_TYPE_COUNT; type++) {
            for (list = 0; list != 2; list++) {
                if (D_800DCE50[list][type] != NULL && OBJ_TYPES[type].unkC != NULL) {
                    node = D_800DCE50[list][type];
                    D_800DD190++;
                    while (node != NULL) {
                        OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                        OBJ_TYPES[type].unkC(node);
                        node = OBJ_ITER_NEXT[D_800DD190];
                    }
                    D_800DD190--;
                }
            }
        }
    }
}

extern Mtx D_80089470;
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern u32 D_800DD1B0;
extern s32 D_800DD1B4;
extern s32 D_800DD1B8;
extern s16 D_800DD1BC;
extern s16 D_800DD1BE;
extern s16 D_800DD1C8;
extern s16 D_800DD1CA;
extern s16 D_800DD1CC;
extern s16 D_800DD1CE;
extern s16 D_800DD1F8;
extern s32 D_800DD1FC;
extern s32 D_800DD200;
extern s16 D_800DD204;
extern s16 D_800DD206;
extern s32 D_800DD208;
extern s32 D_800DD20C;
extern s32 D_800DD210;
extern s32 D_800DD214;
extern s32 D_800DD218;
extern s32 D_800DD21C;
extern s32 D_800DD220;
extern s32 D_800DD224;
extern s32 D_800DD228;
void func_15168F84(s32 group, s32 *first, s32 *last);

/*
 * Draws every object of list `list` whose type is in group `group` (see func_15168F84). Draw mode 1 types
 * drop any object that overflows the display list buffer; on overflow the pass is abandoned and the
 * display list is rewound to the start of the type that overflowed.
 */
Gfx *func_151674F8(Gfx *gfx, s32 group, s16 arg2, s32 list) {
    s32 type;
    s32 first;
    s32 last;
    struct102 **head;
    struct102 *node;
    s32 overflowed;
    Gfx *mode0Start;
    Gfx *prev;
    Gfx *mode1Start;
    s32 full;

    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    func_15168F84(group, &first, &last);
    for (type = first; type < last; type++) {
        if (OBJ_TYPES[type].draw != NULL) {
            head = &D_800DCE50[list][type];
            if (*head != NULL) {
                D_8003C8E0 = (type & 0xFFFFFF) | 0x7000000;
                overflowed = FALSE;
                if (OBJ_TYPES[type].drawMode == 0) {
                    mode0Start = gfx;
                }
                if (OBJ_TYPES[type].drawMode == 1) {
                    mode1Start = gfx;
                }
                D_800D2DAB = 0;
                D_800DD228 = 0;
                D_800DD220 = 0;
                D_800DD224 = 2;
                D_800DD1B0 = -1;
                D_800DD1B4 = -1;
                D_800DD1B8 = -1;
                D_800DD1BC = 0;
                D_800DD1BE = 0;
                D_800DD1C0 = D_800DD1C2 = D_800DD1C4 = D_800DD1C6 = -1;
                D_800DD1C8 = D_800DD1CA = D_800DD1CC = D_800DD1CE = -1;
                D_800DD1F8 = 0;
                D_800DD1FC = 0;
                D_800DD200 = 0;
                D_800DD204 = -1;
                D_800DD206 = -1;
                D_800DD208 = -1;
                D_800DD20C = -1;
                D_800DD210 = -1;
                D_800DD214 = 0;
                D_800DD218 = 0;
                D_800DD21C = 0;
                if (OBJ_TYPES[type].drawBeginDL != NULL) {
                    gSPDisplayList(gfx++, OBJ_TYPES[type].drawBeginDL);
                }
                if (OBJ_TYPES[type].drawBegin != NULL) {
                    gfx = OBJ_TYPES[type].drawBegin(gfx, type, list, arg2);
                }
                gSPClearGeometryMode(gfx++, G_LIGHTING);
                node = *head;
                if (OBJ_TYPES[type].drawMode == 1) {
                    D_800DD190++;
                    while (node != NULL) {
                        prev = gfx;
                        OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                        gfx = OBJ_TYPES[type].draw(gfx, node, arg2);
                        full = ((gfx - D_800BE9C8[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                        if (full) {
                            overflowed = TRUE;
                            gfx = prev;
                        }
                        node = OBJ_ITER_NEXT[D_800DD190];
                    }
                    D_800DD190--;
                } else {
                    D_800DD190++;
                    while (node != NULL) {
                        OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                        gfx = OBJ_TYPES[type].draw(gfx, node, arg2);
                        node = OBJ_ITER_NEXT[D_800DD190];
                    }
                    D_800DD190--;
                }
                if (OBJ_TYPES[type].drawEndDL != NULL) {
                    gSPDisplayList(gfx++, OBJ_TYPES[type].drawEndDL);
                }
                if (OBJ_TYPES[type].drawEnd != NULL) {
                    gfx = OBJ_TYPES[type].drawEnd(gfx, type, arg2);
                }
                if (overflowed) {
                    full = ((gfx - D_800BE9C8[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode1Start;
                    }
                }
                if (OBJ_TYPES[type].drawMode == 0) {
                    full = ((gfx - D_800BE9C8[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode0Start;
                    }
                }
                D_8003C8E0 = 0;
            }
        }
    }
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}

void func_15168A4C(ObjLink *obj, s32 type);

/* Allocates a `size`-byte object (func_10003C6C) and links it into D_800DCE50[list][type]. */
void *func_15167A68(type, list, size, arg3, owner, arg5)
    s32 type;
    s32 list;
    s32 size;
    s32 arg3;
    u8 owner;
    u8 arg5;
{
    ObjLink *obj;

    obj = (ObjLink *)func_10003C6C(size, 1, arg3, 0, arg5);
    if (obj != NULL) {
        obj->list = list;
        func_15168A4C(obj, type);
        obj->owner = owner;
    }
    return obj;
}
void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);

/* Spawns a type 3 SpriteFx from the 0x18-byte template at `src` (anim .. fadeStep), fully opaque. */
void func_15167AD8(SpriteFx *src, u8 owner, s32 list) {
    SpriteFx *fx;

    fx = func_15167A68(3, list, sizeof(SpriteFx), 0, owner, 1);
    if (fx != NULL) {
        bcopy(src, &fx->anim, 0x18);
        fx->alpha = 0xFF;
    }
}

/* Type 3 update: advances the animation and fade; destroys the sprite after its last frame. */
void func_15167B44(SpriteFx *fx) {
    s32 index;
    s32 fade;
    s32 rise;
    s32 frame;

    index = fx->callback;
    if (index != 0) {
        D_8008CA20[index]((struct102 *)fx);
    }

    fade = fx->fadeStep;
    if (fade > 0) {
        index = fx->alpha;
        if (fade < index) {
            fx->alpha = index - fade;
        } else {
            fx->frame = fx->anim->frameCount << 8;
        }
    } else if (fade < 0) {
        rise = -fade;
        frame = fx->frame;
        if (frame / 256 >= fx->anim->frameCount - 1) {
            index = fx->alpha;
            if (rise < index) {
                fx->alpha = index - rise;
                fx->frame = frame - fx->frameStep;
            }
        }
    }

    fx->frame += fx->frameStep;
    if (fx->frame / 256 >= fx->anim->frameCount) {
        func_1516972C((struct102 *)fx);
    }
}
typedef struct {
    s32 unk0;
    s32 unk4;
} Mode15168118;
extern Mode15168118 D_800A4AC8[];
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} StructC58b;
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
s32 func_15095760(Gfx *arg0, void *arg1);
/* Type 3 draw: the current animation frame as a billboard. */
Gfx *func_15167C58(Gfx *gdl, SpriteFx *fx, u8 arg2) {
    Gfx *gfx;
    SpriteParams params;
    StructC58b update; // matching: 12-byte stack object; only the first word (the update flag) is used
    SpriteAnim *anim;
    s32 renderMode;
    s16 frame;

    if (arg2) {} // matching: references the otherwise unused arg2
    update.unk0 = 1;
    frame = fx->frame;
    anim = fx->anim;
    gfx = func_15142E24(gdl, (s32)anim, frame << 8, 2, 0x100, 0x100,
                        anim->frames[frame >> 8], 5, (s32)&params, (u8 *)&update, 3);
    params.x = fx->x;
    params.y = fx->y;
    params.z = fx->z;
    params.unk6 = fx->unk1E;
    params.unk8 = fx->unk20;
    params.alpha = fx->alpha;
    params.flip = 0;
    // matching: fadeStep is an s8, so it never equals 0xFF; the target still tests both ways
    if (fx->fadeStep != 0xFF) {
        renderMode = 3;
    } else if (fx->fadeStep == 0xFF) {
        renderMode = 9;
    }
    gfx = func_15142FBC(gfx, 0x2C00, D_800A4AC8[renderMode].unk0 | D_800A4AC8[renderMode].unk4 | 4, (u8 *)&update);
    return (Gfx *)func_15095760(gfx, &params);
}
/* Spawns a particle (type 0x42 when `arg1` is set, else 5) from the 0x38-byte template at `src`. */
Particle *func_15167D84(src, arg1, extra, onDeath, owner, list)
    Particle *src;
    s32 arg1;
    s32 extra;
    s8 onDeath;
    u8 owner;
    s32 list;
{
    Particle *particle;

    particle = func_15167A68((arg1 == 0) ? 5 : 0x42, list, extra + 0x50, 0, owner, 1);
    if (particle == NULL) {
        return particle;
    }
    bcopy(src, &particle->anim, 0x38);
    particle->onDeath = onDeath;
    return particle;
}
/* Particle update: life, fade, motion and animation; runs onDeath and destroys it when finished. */
void func_15167E0C(Particle *p) {
    void (*func)(struct102 *);
    u8 finished;
    s32 limit;

    finished = 0;
    if (p->life != 0x7FFF) {
        p->life -= D_800BE9E4;
        if (p->life < 0) {
            p->life = 0;
            finished = 1;
        }
    }

    if (p->fadeStep > 0) {
        if (p->fadeStep < p->primA) {
            p->primA -= p->fadeStep;
        } else {
            p->life = 0;
            finished = 1;
        }
    }

    if (p->callback != -1) {
        func = D_8008C9C8[p->callback];
        if (func != NULL) {
            func((struct102 *)p);
        }
    }

    if (p->life != 0) {
        s32 posY;
        s32 posX;
        s32 posZ;
        s32 delta;

        p->velY += p->accelY;

        // matching: 24.8 fixed point moves written step by step (incl. the redundant (u8)) for IDO's temp registers
        posY = p->y;
        posY = (posY << 8) | p->subY;
        delta = p->velY * D_800BE9E4;
        posY += delta;
        p->y = posY >> 8;
        posY = (u8)posY;
        p->subY = posY;
        posX = p->x;
        posX = (posX << 8) | p->subX;
        delta = p->velX * D_800BE9E4;
        posX += delta;
        p->x = posX >> 8;
        posX = (u8)posX;
        p->subX = posX;
        posZ = p->z;
        posZ = (posZ << 8) | p->subZ;
        delta = p->velZ * D_800BE9E4;
        posZ += delta;
        p->z = posZ >> 8;
        posZ = (u8)posZ;
        p->subZ = posZ;
        if (p->frameStep != 0) {
            limit = (p->anim->frameCount << 8) - 1;
            p->frame += p->frameStep * D_800BE9E4;
            if (limit < p->frame) {
                if (p->flags & PARTICLE_DIE_AT_END) {
                    p->life = 0;
                    finished = 1;
                } else {
                    p->flags |= PARTICLE_LOOPED;
                    if (p->flags & PARTICLE_PINGPONG) {
                        p->frame = limit - (p->frame % limit);
                        p->frameStep = -p->frameStep;
                    } else {
                        do {
                            p->frame -= limit;
                        } while (limit < p->frame);
                    }
                }
            } else if (p->frame < 0) {
                if (p->flags & PARTICLE_DIE_AT_START) {
                    p->life = 0;
                    finished = 1;
                } else if (p->flags & PARTICLE_PINGPONG) {
                    p->frame = -p->frame % limit;
                    p->frameStep = -p->frameStep;
                } else {
                    do {
                        p->frame += limit;
                    } while (p->frame < 0);
                }
            }
        }
    } else {
        finished = 1;
    }

    if (finished != 0) {
        if (p->onDeath != -1) {
            D_8008CA20[p->onDeath]((struct102 *)p);
        }
        func_1516972C((struct102 *)p);
    }
}
extern SpriteAnim D_800907FC;
extern SpriteAnim D_8009053C;
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern s32 D_800D2C9C;
s32 func_15095760(Gfx *arg0, void *arg1);

/* Particle draw: sets render/combine modes and colours, then the current frame as a billboard. */
s32 func_15168118(Gfx *gfx, Particle *p, s16 viewIndex) {
    s32 view;
    SpriteParams params;
    s32 pad1[2]; // matching: stack layout of the target frame
    u8 update;
    s32 pad2[3]; // matching: stack layout of the target frame
    f32 angle;
    s32 size;
    s32 unused; // matching: an unused local the target allocates
    s32 c0, c1, c2; /* colour components, reused for prim and env */
    SpriteAnim *anim;
    SpriteAnim *white;

    params.flip = 0;
    if (p->flags & PARTICLE_FLIP_S) {
        params.flip = 1;
    }
    if (p->flags & PARTICLE_FLIP_T) {
        params.flip |= 2;
    }
    // matching: two empty tests of the same condition steer IDO's scheduling of the texture load
    if ((u32)p->anim->frames < 0x10000000) {
    }
    if ((u32)p->anim->frames < 0x10000000) {
    }
    update = 1;
    {
        s32 texture;

        anim = p->anim;
        if ((u32)anim->frames < 0x10000000) {
            texture = (s32)anim->frames;
        } else {
            texture = anim->frames[p->frame >> 8];
        }
        gfx = func_15142E24(gfx, (s32)anim, p->frame << 8, 2, 0x100, 0x100, texture, 5, (s32)&params, &update, 3);
    }
    view = viewIndex;
    params.x = p->x;
    params.y = p->y;
    params.z = p->z;
    size = p->width;
    if (size < 0) {
        /* 0.0111 = 1/90: fold the view angle into 0..90 degrees */
        angle = fabsf(D_800DBFF0[view].unk380 + (p->unk18 & 0xFF) * 1.40625f);
        while (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if (angle >= 180.0f) {
            angle -= 180.0f;
        }
        if (angle >= 90.0f) {
            angle = 180.0f - angle;
        }
        size = size * angle * -0.01111111138f;
        if (size < 0x400) {
            size = 0x400;
        }
    }
    params.unk6 = size;
    size = p->height;
    if (size < 0) {
        view = (p->width < 0) ? 0 : p->width;
        size = fabsf(D_800DBFF0[viewIndex].unk388) * (view + size) * 0.01111111138f - size;
    }
    params.unk8 = size;
    if (p->flags & PARTICLE_CUSTOM_MODE) {
        u8 renderMode;
        u8 combineMode;
        s32 pad3[2]; // matching: stack layout of the target frame

        renderMode = p->unk14 & 0xFF;
        combineMode = (p->unk14 & 0xFFFF0000) >> 16;
        gfx = func_15142B7C(gfx, 5, 0);
        gfx = func_1513F4E4(gfx, combineMode, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | ((p->flags & PARTICLE_2CYCLE) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[renderMode].unk0 | D_800A4AC8[renderMode].unk4 | 4, &update);
    } else if (p->anim->unkA != 5) {
        gfx = func_1513F4E4(gfx, 0xD, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (p->anim->unkA == 3) {
        gfx = func_1513F4E4(gfx, 0xE, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4 | 4, &update);
    } else if (p->flags & 0x10) {
        gfx = func_1513F4E4(gfx, 0xD, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x12].unk0 | D_800A4AC8[0x12].unk4 | 4, &update);
    } else {
        gfx = func_1513F4E4(gfx, 0xF, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_2CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[0x11].unk0 | D_800A4AC8[0x11].unk4 | 4, &update);
    }
    c0 = p->primG;
    c1 = p->primB;
    c2 = p->primA;
    gfx = func_15142CF0(gfx, 1, 0, p->primR, c0, c1, c2, &update);
    if (p->flags & PARTICLE_ENV_COLOR) {
        c0 = p->envR;
        c1 = p->envG;
        c2 = p->envB;
    } else {
        anim = p->anim;
        white = &D_8009053C;
        if (p->anim == &D_800907FC) {
            c0 = 0x80;
            c1 = 0x80;
            c2 = 0xFF;
        } else if (white == anim) {
            c0 = 0xFF;
            c1 = 0xFF;
            c2 = 0xFF;
        } else {
            c0 = 0;
            c1 = 0;
            c2 = 0;
        }
    }
    gfx = func_15142C10(gfx, c0, c1, c2, 0, &update);
    return func_15095760(gfx, &params);
}
/* Template / payload of a type 0xE object: a textured quad pair animated through a SpriteAnim. */
typedef struct {
    /* 0x00 */ Vtx vtx[8];
    /* 0x80 */ u8 pad80[8];
    /* 0x88 */ s16 frame;       /* 8.8 fixed point; -1 = finished */
    /* 0x8A */ s16 frameStep;
    /* 0x8C */ u8 pad8C[0xC];
    /* 0x98 */ u16 flags;       /* 0x4: finish at either end, 0x40: ping-pong, 0x80/0x100: flip s/t */
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ s16 unk9E;
    /* 0xA0 */ u8 animIndex;    /* index into D_8008CA4C */
    /* 0xA1 */ u8 padA1;
    /* 0xA2 */ s8 callback;     /* index into D_8008C9C8, -1 = none */
} QuadFxData;

/* Type 0xE object. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ QuadFxData data;
} QuadFx;

/* Fills a QuadFxData template's vertex colours and texture coordinates from its animation. */
void func_1516865C(QuadFxData *q, u8 r, u8 g, u8 b, u8 a) {
    s16 tB;
    s16 tA;
    s16 sB;
    s16 sA;
    s32 i;

    tA = 0x2000;
    tB = (D_8008CA4C[q->animIndex]->unk8 + 0x100) << 5;
    sA = 0x2000;
    sB = (D_8008CA4C[q->animIndex]->unk6 + 0x100) << 5;
    for (i = 0; i < 2; i++) {
        q->vtx[i * 4 + 1].v.flag = 0;
        q->vtx[i * 4 + 1].v.cn[0] = r;
        q->vtx[i * 4 + 1].v.cn[1] = g;
        q->vtx[i * 4 + 1].v.cn[2] = b;
        q->vtx[i * 4 + 1].v.cn[3] = a;
        q->vtx[i * 4 + 2].v.flag = 0;
        q->vtx[i * 4 + 2].v.cn[0] = r;
        q->vtx[i * 4 + 2].v.cn[1] = g;
        q->vtx[i * 4 + 2].v.cn[2] = b;
        q->vtx[i * 4 + 2].v.cn[3] = a;
        q->vtx[i * 4 + 3].v.flag = 0;
        q->vtx[i * 4 + 3].v.cn[0] = r;
        q->vtx[i * 4 + 3].v.cn[1] = g;
        q->vtx[i * 4 + 3].v.cn[2] = b;
        q->vtx[i * 4 + 3].v.cn[3] = a;
        q->vtx[i * 4 + 0].v.flag = 0;
        q->vtx[i * 4 + 0].v.cn[0] = r;
        q->vtx[i * 4 + 0].v.cn[1] = g;
        q->vtx[i * 4 + 0].v.cn[2] = b;
        q->vtx[i * 4 + 0].v.cn[3] = a;
    }
    if (q->flags & 0x80) {
        tB = 0x2000;
        tA = (D_8008CA4C[q->animIndex]->unk8 + 0x100) << 5;
    }
    if (q->flags & 0x100) {
        sB = 0x2000;
        sA = (D_8008CA4C[q->animIndex]->unk6 + 0x100) << 5;
    }
    q->vtx[1].v.tc[0] = q->vtx[5].v.tc[0] = q->vtx[2].v.tc[0] = q->vtx[6].v.tc[0] = sA;
    q->vtx[2].v.tc[1] = q->vtx[6].v.tc[1] = q->vtx[3].v.tc[1] = q->vtx[7].v.tc[1] = tA;
    q->vtx[0].v.tc[0] = q->vtx[4].v.tc[0] = q->vtx[3].v.tc[0] = q->vtx[7].v.tc[0] = sB;
    q->vtx[0].v.tc[1] = q->vtx[4].v.tc[1] = q->vtx[1].v.tc[1] = q->vtx[5].v.tc[1] = tB;
    q->frame = 0;
    q->unk9C = 0;
    q->unk9E = 0;
}

/* Spawns a type 0xE QuadFx from a template prepared with func_1516865C. */
QuadFx *func_15168800(src, owner, list)
    QuadFxData *src;
    u8 owner;
    s32 list;
{
    QuadFx *fx;

    fx = func_15167A68(0xE, list, sizeof(QuadFx), 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    bcopy(src, &fx->data, sizeof(QuadFxData));
    return fx;
}

/* Type 0xE update: advances the animation (wrapping, ping-ponging or finishing) and runs its callback. */
void func_15168870(QuadFx *fx) {
    s32 frame;
    s32 limit;
    u8 frameCount;
    s16 step;
    void (*func)(struct102 *);

    step = fx->data.frameStep;
    if (step != 0) {
        frameCount = D_8008CA4C[fx->data.animIndex]->frameCount;
        limit = (frameCount << 8) - 1;
        fx->data.frame += step * D_800BE9E4;
        frame = fx->data.frame;
        if (limit < frame) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = limit - (frame % limit);
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame - limit;
                    frame = fx->data.frame;
                } while (limit < frame);
            }
        } else if (frame < 0) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = -frame % limit;
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame + limit;
                    frame = fx->data.frame;
                } while (frame < 0);
            }
        }
    }

    frame = fx->data.callback;
    if (frame != -1) {
        func = D_8008C9C8[frame];
        if (func != NULL) {
            func((struct102 *)fx);
        }
    }

    if (fx->data.frame == -1) {
        func_1516972C((struct102 *)fx);
    }
}
void func_15168A2C(s32 arg0) {
    func_15168B10(arg0, 0);
}
/* Pushes obj onto the front of list D_800DCE50[obj->list][type] and sets its type. */
void func_15168A4C(ObjLink *obj, s32 type) {
    s32 list;
    ObjLink **head;

    list = obj->list;
    head = (ObjLink **)&D_800DCE50[list][type];
    obj->next = *head;
    if (obj->next != NULL) {
        obj->next->prev = obj;
    }
    obj->type = type;
    obj->prev = NULL;
    *head = obj;
}

/* Unlinks obj from its current list. */
void func_15168A9C(ObjLink *obj) {
    s32 list = obj->list;
    s32 type = obj->type;
    ObjLink **head = (ObjLink **)&D_800DCE50[list][type];

    if (obj == *head) {
        *head = obj->next;
    }
    if (obj->next != NULL) {
        obj->next->prev = obj->prev;
    }
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
}

/* Moves obj to the list for another type. */
void func_15168B10(s32 obj, s32 type) {
    func_15168A9C((ObjLink *)obj);
    func_15168A4C((ObjLink *)obj, type);
}

/*
 * Particle callback: unk14 holds a fade rate (high half) and a delay (low half). Keeps the particle alive
 * (life = 30) while the delay counts down, then fades primA by the rate each call and lets it die.
 * Readable equivalent:
 *     word = p->unk14;
 *     if (word & 0xFFFF) {
 *         p->unk14 &= 0xFFFF0000; p->life = 30; p->unk14 |= (u16)((word & 0xFFFF) - 1); return;
 *     }
 *     if ((u16)(word >> 16) < p->primA) { p->primA -= (u16)(word >> 16); p->life = 30; } else { p->life = 0; }
 */
// matching: permuter output; every readable form tried allocates `word` and the delay in other registers
typedef struct Local15168B44 { u8 pad0[0x14]; s32 unk14; u8 pad18[0x20]; s16 unk38; u8 pad3A[5]; u8 unk3F; } Local15168B44;

void func_15168B44(void *arg0)
{
  Local15168B44 *a;
  s32 v1;
  s32 new_var2;
  u16 t6;
  u16 hi;
  u8 a2;
  s32 new_var;
  s32 c0;
  s32 z0;
  s32 z1;
  s32 z2;
  s32 t9;
  s32 z3;
  s32 z4;
  s32 z5;
  s32 keep;
  a = arg0;
  hi = 0x1E;
  v1 = a->unk14;
  new_var2 = v1;
  t6 = ((u16) new_var2) ^ 0;
  new_var = t6 - (1 & 0xFFFFFFFFu);
  if (t6 != 0)
  {
    c0 = v1;
    z0 = 0;
    z1 = 0;
    z2 = 0;
    z3 = 0;
    z4 = 0;
    z5 = 0;
    keep = new_var & 0xFFFF;
    new_var = keep * 0;
    if (a)
    {
    }
    hi++;
    hi--;
    t9 = (v1 & 0xFFFF0000) | new_var;
    v1 = 0;
    a->unk14 = t9;
    a->unk38 = hi;
    if (keep)
    {
      ;
    }
    *((s32 *) (((s8 *) arg0) + 0x14)) = ((t9 | keep) | (((((((c0 * 0) | (z0 * 0)) | (z1 * 0)) | (z2 * 0)) | (z3 * 0)) | (z4 * 0)) | (z4 = z5 * 0))) & 0xFFFFFFFFu;
    return;
  }
  hi = (u16) (v1 >> 16);
  v1 = a->unk3F;
  if (0)
  {
  }
  a2 = v1;
  if (hi < a2)
  {
    a->unk3F = a2 - hi;
    if (1)
    {
    }
    a->unk38 = 0x1E;
  }
  else
  {
    a2 = 0;
    a->unk38 = (a2, 0);
  }
}
/* Type 0x10 update: runs the object's callback, if any. */
void func_15168BAC(ModelFx *fx) {
    u8 index;

    index = fx->data.callback;
    if (index != 0) {
        D_8008CA20[index]((struct102 *)fx);
    }
}

/* Spawns a type 0x10 ModelFx from the template at `src`, if it has a display list. */
void func_15168BE4(src, owner, list)
    ModelFxData *src;
    u8 owner;
    s32 list;
{
    ModelFx *fx;

    if (src->displayList != NULL) {
        fx = func_15167A68(0x10, list, sizeof(ModelFx), 1, owner, 1);
        if (fx != NULL) {
            bcopy(src, &fx->data, sizeof(ModelFxData));
        }
    }
}

/* Type 0x10 draw: builds this frame's matrix, sets fade/tile/render state and calls the display list. */
Gfx *func_15168C4C(Gfx *gfx, ModelFx *fx, s32 arg2) {
    s32 pad; // matching: stack layout of the target frame
    s32 renderMode;
    u8 update;

    func_15043D90(&fx->mtx[D_800BE9C0], fx->data.rotX, fx->data.rotY, fx->data.rotZ,
                  fx->data.scaleX, fx->data.scaleY, fx->data.scaleZ, fx->data.x, fx->data.y, fx->data.z);

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, fx->data.primA);
    /* D_800DD1C0..C6: cached prim colour (reset to -1 per type by func_151674F8) */
    D_800DD1C6 = fx->data.primA;
    D_800DD1C0 = D_800DD1C2 = D_800DD1C4 = 0;
    gDPSetEnvColor(gfx++, 0, 0, 0, 0);
    gDPSetTileSize(gfx++, 0, 2, fx->data.tileUlt, 0x7E, 0xFE);

    switch (fx->data.renderMode) {
    case 1:
        renderMode = 0x552230;
        break;
    case 2:
        renderMode = 0x504A50;
        break;
    }

    update = 0;
    gfx = func_15142FBC(gfx, 0x8ACA0, renderMode, &update);

    if (fx->data.combineMode == 1) {
        gDPSetCombine(gfx++, 0x123824, 0xFF73FFFF);
    }

    gSPMatrix(gfx++, &fx->mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, fx->data.displayList);

    return gfx;
}

/* Adds `offset` to an address unless it is segmented. */
void func_15168E34(s32 *addr, s32 offset) {
    s32 value = *addr;

    if ((value & 0x0F000000) == 0) {
        *addr = value + offset;
    }
}

/* One display list command seen as bytes: opcode, two parameter bytes, a third (G_MOVEMEM index), w1. */
typedef struct {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ s8 unk1;
    /* 0x2 */ s8 unk2;
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
} DLCommand;

#define IS_RELOCATABLE(cmd) \
    ((cmd)->opcode == G_VTX || ((cmd)->opcode == (s8)G_MOVEMEM && (cmd)->index == G_MV_MATRIX))

/* Relocates the vertex and matrix addresses of a display list by `offset` (segmented ones are left alone). */
void func_15168E54(s8 *dl, s32 offset) {
    s32 i;
    DLCommand *cmd;

    // matching: byte-pointer indexing (not a DLCommand array) gives the target's saved-register use
    for (i = 0; dl[i << 3] != (s8)G_ENDDL; i++) {
        cmd = (DLCommand *)(dl + (i << 3));
        if (IS_RELOCATABLE(cmd)) {
            func_15168E34((s32 *)&cmd->addr, offset);
        }
    }
}

/* Rebases the vertex and matrix addresses of a display list: drops their segment and adds `base`. */
void func_15168F08(DLCommand *dl, s32 base) {
    s32 i;
    DLCommand *cmd;
    s8 opcode;

    // matching: the early-out, the dead `opcode` loads and the do-while(0) reproduce the target's
    // double opcode load and register use; plain for/while loops do not
    i = 0;
    cmd = dl;
    if (dl->opcode == (s8)G_ENDDL) {
        return;
    }
    cmd = &dl[i];
    opcode = cmd->opcode;
    do {
        i++;
        if (IS_RELOCATABLE(cmd)) {
            cmd->addr = cmd->addr & 0xFFFFFF;
            cmd->addr += base;
        }
        cmd = &dl[i];
        do {
            opcode = cmd->opcode;
        } while (0);
    } while (cmd->opcode != (s8)G_ENDDL);
}
/* Gives the range [*first, *last) of object types drawn together as draw group `group`. */
void func_15168F84(s32 group, s32 *first, s32 *last) {
    if (group == 0) {
        *first = 1;
        *last = 65;
    } else if (group == 1) {
        *first = 66;
        *last = 79;
    } else if (group == 2) {
        *first = 80;
        *last = 88;
    } else if (group == 3) {
        *first = 89;
        *last = 92;
    } else if (group == 5) {
        *first = 97;
        *last = 99;
    } else if (group == 6) {
        *first = 100;
        *last = 101;
    } else {
        *first = 93;
        *last = 96;
    }
}

/* Sends an event to every object of every type. */
void func_15169040(s32 event, u8 eventType) {
    func_15169070(0, OBJ_TYPE_COUNT, event, eventType);
}
void func_15143D18(s32 *, s32 *, s32, s32);
void func_1516968C(struct102 *arg0, u8 *arg1, u8 arg2);

/* Sends an event to every object of types [first, last) (clamped by func_15143D18). */
void func_15169070(s32 first, s32 last, s32 event, u8 eventType) {
    s32 type;
    struct102 *node;

    func_15143D18(&first, &last, 2, OBJ_TYPE_COUNT);
    for (type = first; type < last; type++) {
        s32 list;
        for (list = 0; list != 2; list++) {
            if (OBJ_TYPES[type].handleEvent != NULL) {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    OBJ_TYPES[type].handleEvent(node, event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
    }
}

/* Sends an event to every object of the `count` types listed in `types`. */
void func_15169260(s32 types, s32 count, s32 event, u8 eventType) {
    s32 *type;
    s32 i;
    s32 list;
    struct102 *node;

    for (i = 0, type = (s32 *)types; i < count; i++) {
        for (list = 0; list != 2; list++) {
            if (OBJ_TYPES[*type].handleEvent != NULL) {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    OBJ_TYPES[*type].handleEvent(node, event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            } else {
                node = D_800DCE50[list][*type];
                D_800DD190++;
                while (node != NULL) {
                    OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                    func_1516968C(node, (u8 *)event, eventType);
                    node = OBJ_ITER_NEXT[D_800DD190];
                }
                D_800DD190--;
            }
        }
        type++;
    }
}

/* Sends an event to every object of one type (clamped to the last type). */
void func_1516944C(s32 type, s32 event, u8 eventType) {
    s32 list;
    struct102 *node;

    if (type >= OBJ_TYPE_COUNT) {
        type = OBJ_TYPE_COUNT - 1;
    }
    for (list = 0; list != 2; list++) {
        if (OBJ_TYPES[type].handleEvent != NULL) {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                func_1516968C(node, (u8 *)event, eventType);
                OBJ_TYPES[type].handleEvent(node, event, eventType);
                node = OBJ_ITER_NEXT[D_800DD190];
            }
            D_800DD190--;
        } else {
            node = D_800DCE50[list][type];
            D_800DD190++;
            while (node != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = (struct102 *)node->unk8;
                func_1516968C(node, (u8 *)event, eventType);
                node = OBJ_ITER_NEXT[D_800DD190];
            }
            D_800DD190--;
        }
    }
}
void func_151695F0(struct127 *arg0, u8 arg1) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;

    sp18.unk0 = (s32)arg0;
    sp18.unk4 = arg0->unique_id;
    func_15169040((s32)&sp18, arg1);
}
void func_1516962C(s32 arg0, struct127 *arg1, u8 arg2) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;
    sp18.unk0 = (s32)arg1;
    sp18.unk4 = arg1->unique_id;
    func_1516944C(arg0, (s32)&sp18, arg2);
}
s32 func_15169668(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800D2DAB = 1;
    return arg0;
}
void func_1516968C(struct102 *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0xF || arg2 == 0x10) {
        if (arg1[0] == ((u8 *)arg0)[0xC]) {
            func_1516972C(arg0);
        }
    }
}

void func_151696DC(struct102 *arg0) {
    s8 i;

    for (i = 0; i < D_800DD190; i++) {
        if ((s32)arg0 == ((s32 *)D_800DD198)[i]) {
            ((s32 *)D_800DD198)[i] = arg0->unk8;
        }
    }
}

void func_1516972C(struct102 *arg0) {
    void (*func)(struct102 *arg0);
    func_151696DC(arg0);

    if (arg0->unk0 >= 2) {
        func = D_8008B4D0[arg0->unk0].unk0;
        if (func != NULL) {
            func(arg0);
            return;
        }
        func_15169804(arg0);
    }
}

void func_1516979C(struct102 *arg0) {
    void (*func)(struct102 *arg0);

    func_151696DC(arg0);
    func = D_8008B4D4[arg0->unk0].unk0;
    if (func != NULL) {
        func(arg0);
        return;
    }
    func_15169824(arg0);
}

void func_15169804(struct102 *arg0) {
    func_15168B10(arg0, 1);
}

void func_15169824(struct102 *arg0) {
    func_15168A9C(arg0);
    func_10004074(arg0);
}

void func_15169850(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (arg1 == 0) {
        if (*(s32 *)arg0 == *(s32 *)arg2 || *(u8 *)(arg0 + 4) == *(u8 *)arg3) {
            func_1516972C((struct102 *)arg4);
        }
    } else if (arg1 == 0x2D) {
        s32 v = *(s32 *)arg0;
        if (v == *(s32 *)arg2) {
            *(s32 *)arg2 = *(s32 *)(arg0 + 4);
            *(u8 *)arg3 = *(u8 *)(arg0 + 9);
        } else if (*(s32 *)(arg0 + 4) == *(s32 *)arg2) {
            *(s32 *)arg2 = v;
            *(u8 *)arg3 = *(u8 *)(arg0 + 8);
        }
    }
}
