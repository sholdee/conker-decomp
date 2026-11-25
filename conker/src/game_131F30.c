#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* A node of the object lists D_800DCE50[list][type] (see game_1944C0.c), as func_15105C24 reads it. */
typedef struct ListObj {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct ListObj *next;
    /* 0x0C */ u8 padC[7];
    /* 0x13 */ u8 type;
    /* 0x14 */ u8 pad14[0x14];
    /* 0x28 */ s32 owner;
} ListObj;



typedef struct {
    s32 count;
    const u8 *list;
    Actor *player;
} Msg35;

/* Arrival callback of a level prop (D_800DBEF4 entry): finds the first active player in the prop's
 * player mask and sends message 0x35 with a 9-entry list chosen by the prop's id (0xF7-0xF9),
 * or message 0x38 for 0xF6. */
void func_15104A80(WorldObject *prop) {
    Actor *player;

    player = NULL;
    {
        s32 mask;
        s32 i;
        Actor *candidate;

        i = prop - D_800DBEF4;
        mask = D_800DBF94[i];
        if (mask != 0) {
            for (i = 0; i < 25 && player == NULL; i++) {
                candidate = &D_800CC2D0[i];
                if (candidate->interaction_state != 0 && (mask & (1U << i))) {
                    player = candidate;
                }
            }
        }
    }
    if (prop->id == 0xF9 || prop->id == 0xF8 || prop->id == 0xF7) {
        Msg35 msg;

        msg.player = player;
        if (prop->id == 0xF9) {
            const u8 list[9] = { 0, 1, 2, 3, 4, 5, 15, 16, 17 };

            msg.count = 9;
            msg.list = list;
        } else if (prop->id == 0xF8) {
            const u8 list[9] = { 6, 7, 8, 24, 25, 26, 18, 19, 20 };

            msg.count = 9;
            msg.list = list;
        } else if (prop->id == 0xF7) {
            const u8 list[9] = { 9, 10, 11, 12, 13, 14, 21, 22, 23 };

            msg.count = 9;
            msg.list = list;
        }
        func_151494E0((s32)&msg, 0x35);
    } else if (prop->id == 0xF6) {
        Msg35 msg;

        msg.player = player;
        func_151494E0((s32)&msg.player, 0x38);
    }
}

/* Rain of lightning bolts falling around a point onto the ground (func_15104C44, func_15104FF8). */
typedef struct BoltRainState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 pending;      /* bolts owed, accumulated per frame */
    /* 0x08 */ s16 timer;        /* frames left; negative when off */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 height;       /* bolts start this far above pos */
    /* 0x20 */ CollisionHit ground; /* where the next bolt lands */
    /* 0x44 */ s16 *unk44;
} BoltRainState;

typedef struct BoltRainObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltRainState state;
} BoltRainObj;

s32 func_1000FD38();
extern s32 func_151464B8(s16 *);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern s32 func_15046C80(f32 *pos, s32, f32 y, CollisionHit *hit);
extern void func_15105CE0(BoltDesc *desc, s32 extraSize, s32, s32);

/* While the timer runs, drops lightning bolts from above pos to random ground points within radius. */
void func_15104C44(BoltRainObj *obj) {
    BoltRainState *state;

    state = &obj->state;
    if (state->timer >= 0) {
        state->timer -= D_800BE9E4;
        if (state->timer < 0) {
            func_1000FD38(func_1000EF40, state->region, 0);
        }
        if (func_151464B8(state->unk44) == 0) {
            state->pending += (0.1970000118f + func_150ADA68() * 0.3f) * D_800BE9A4;
            if (state->pending > 1.0f) {
                BoltDesc desc;
                f32 ground[3];
                f32 offZ1;
                f32 offX1;
                f32 offZ2;
                f32 offX2;

                ground[1] = state->pos.y + state->height;
                desc.flags = 1;
                desc.updateKind = -1;
                desc.rejitterBase = 3;
                desc.rejitterRange = 4;
                desc.kind = 0;
                desc.width = 45.0f;
                desc.jitter = 25.0f;
                desc.ctrl[0].x = state->pos.x;
                desc.ctrl[0].y = state->pos.y + state->height;
                desc.ctrl[0].z = state->pos.z;
                desc.ctrl[1].y = state->pos.y + state->height - 180.0f;
                desc.color.red = 0x61;
                desc.color.green = 0xF2;
                desc.color.blue = 0xFF;
                do {
                    func_15143874(func_150ADA20() & 0xFF, (func_150ADA68() * 0.5f + 0.5f) * state->radius,
                                  &ground[0], &ground[2]);
                    ground[0] += state->pos.x;
                    ground[2] += state->pos.z;
                    if (func_15046C80(ground, 0, state->pos.y, &state->ground) != 0) {
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX1, &offZ1);
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX2, &offZ2);
                        desc.ctrl[1].x = state->pos.x + offX1;
                        desc.ctrl[1].z = state->pos.z + offZ1;
                        desc.ctrl[3].x = ground[0];
                        desc.ctrl[3].y = state->ground.height;
                        desc.ctrl[3].z = ground[2];
                        desc.ctrl[2].x = offX2 + ground[0];
                        desc.ctrl[2].y = desc.ctrl[3].y + 180.0f;
                        desc.ctrl[2].z = offZ2 + ground[2];
                        desc.life = func_150ADA20() % 13U + 8;
                        desc.color.alpha = func_150ADA20() % 156U + 100;
                        desc.segments = func_150ADA20() % 3U + 5;
                        func_15105CE0(&desc, 0, obj->base.header.owner, obj->base.header.kind);
                    }
                    state->pending -= 1.0f;
                } while (state->pending > 1.0f);
            }
        }
    }
}

/* Message handler: on 0x38 while the region is idle, restarts the rain for 300 frames with its sound. */
void func_15104FF8(BoltRainObj *obj, s32 arg1, u8 msg) {
    BoltRainState *state = &obj->state;

    if (msg == 0x38 && state->region->disabled == 1) {
        state->timer = 300;
        // matching: the (s32) copy keeps IDO from sharing one register for region across the call
        func_1000FD38(func_1000EF40, (s32)state->region, 0);
        func_1000FA64(0x236, state->region->x, state->region->y, state->region->z, 0x4000, 1500, 1000,
                      func_1000EF40, state->region, 0, 8, 0);
    }
}

/* Lightning arcing between the four edges of a quad (func_151050B0, func_15105548). */
typedef struct BoltArcState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ s16 timer;          /* frames left; negative when off */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 pending;        /* bolts owed, accumulated per frame */
    /* 0x0C */ Vec3f edgeStart[4];
    /* 0x3C */ Vec3f edgeDelta[4]; /* edge i runs from edgeStart[i] to edgeStart[i] + edgeDelta[i] */
    /* 0x6C */ s16 *unk6C;
    /* 0x70 */ s32 unk70;
} BoltArcState;

struct BoltArcObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltArcState state;
};

void *func_15106F98(Vec3f *start, Vec3f *end, s32 depth, BoltChainConfig *config, f32 spread, u8 kind, u8, s32);

/* While the timer runs, chains lightning between random points on two different edges of the quad. */
void func_151050B0(BoltArcObj *obj) {
    BoltArcState *state;
    BoltChainConfig config;
    Vec3f start;
    Vec3f end;
    f32 t;
    u32 edgeA;
    u8 edgeB;

    state = &obj->state;
    if (state->timer < 0) {
        state->region->disabled = 1;
        return;
    }
    state->region->disabled = 0;
    state->timer -= D_800BE9E4;
    if (state->timer < 0) {
        func_151494E0(0, 0x39);
        func_151494E0(0, 0x4B);
    }
    if (func_151464B8(state->unk6C) != 0) {
        return;
    }
    t = func_150ADA68();
    state->pending += (0.04000000283f + t * 0.0700000003f) * D_800BE9A4;
    if (state->pending > 1.0f) {
        config.widthBase = 10.0f;
        config.widthRange = 15.0f;
        config.jitter = 20.0f;
        config.bendBase = 44.0f;
        config.bendRange = 37.0f;
        config.bendSpread = 100;
        config.rejitterBase = 0;
        config.rejitterRange = 10;
        config.lifeBase = 1;
        config.lifeRange = 10;
        config.segmentsBase = 4;
        config.segmentsRange = 2;
        config.r = 0x7E;
        config.g = 0xF9;
        config.b = 0xFF;
        config.alphaBase = 0x37;
        config.alphaRange = 0xC8;
        do {
            edgeA = (u8)(func_150ADA68() * 3.0f);
            edgeB = func_150ADA68() * 2 + 1 + edgeA;
            if (edgeB >= 4) {
                edgeB -= 4;
            }
            t = func_150ADA68();
            start.x = state->edgeStart[edgeA].x + state->edgeDelta[edgeA].x * t;
            start.y = state->edgeStart[edgeA].y + state->edgeDelta[edgeA].y * t;
            start.z = state->edgeStart[edgeA].z + state->edgeDelta[edgeA].z * t;
            t = func_150ADA68();
            end.x = state->edgeStart[edgeB].x + state->edgeDelta[edgeB].x * t;
            end.y = state->edgeStart[edgeB].y + state->edgeDelta[edgeB].y * t;
            end.z = state->edgeStart[edgeB].z + state->edgeDelta[edgeB].z * t;
            func_15106F98(&start, &end, 3, &config, 0.3250000179f, 2, obj->base.header.owner, obj->base.header.kind);
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
    }
}

/* Message handler: on 0x4B, calls func_1516972C on the object. */
void func_1510550C(EffectHeader *obj, s32 arg1, u8 msg) {
    if (msg == 0x4B) {
        func_1516972C(obj);
    }
}

/* Message handler: on 0x38 while the region is idle, restarts the arcs for 300 frames. */
void func_15105548(BoltArcObj *obj, s32 *arg1, u8 msg) {
    BoltArcState *state = &obj->state;

    if (msg == 0x38 && state->region->disabled == 1) {
        state->unk70 = *arg1;
        state->timer = 300;
    }
}

/* The surface hit an impact effect object was spawned for. */
typedef struct ImpactHit {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Vec3f position;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ Vec3f velocity;
    /* 0x6C */ CollisionTri triangle;
    /* 0x7E */ u8 pad7E[0xA];
    /* 0x88 */ s32 surfaceFlags;   /* bits 0-4: surface type */
} ImpactHit;

typedef struct ImpactObj {
    /* 0x00 */ TimedEffect base;
    /* 0x028 */ u8 pad28[0xE8];
    /* 0x110 */ ImpactHit hit;
} ImpactObj;

void *func_15102920(f32 size, u8 alpha, CollisionTri *triangle, Vec3f *pos, s32 lifetime, s32, s32, s32 surfaceFlags,
                   s32, s32);
void func_151C329C(Vec3f *pos, u8, s32);

/* One-entry variant lists for func_15151D6C, copied from rodata. */
typedef struct { s32 v[1]; } S32x1;
typedef struct { f32 v[1]; } F32x1;
extern S32x1 D_800A23A4; /* { 0x85 } */
extern F32x1 D_800A23A8; /* { 0.1f } */
void func_15151D6C(SurfaceDebrisBurst *desc, s32 *variantIds, f32 *variantScales, s32 variantCount, u8, s32);

/* Spawns the effects of an impact: a func_15102920 effect, func_151C329C by chance and, except on
 * surface type 9, sometimes debris flying back along the impact direction. */
void func_1510558C(ImpactObj *obj) {
    ImpactHit *hit;
    u32 surfaceFlags;
    // matching: one struct keeps IDO's stack order (scales, ids, desc) above the spilled locals
    struct {
        F32x1 variantScales;
        S32x1 variantIds;
        SurfaceDebrisBurst desc;
    } debris;
    u32 lifetime;
    u32 random;
    f32 fraction;

    hit = &obj->hit;
    fraction = func_150ADA68();
    random = func_150ADA20();
    lifetime = func_150ADA20() % 81U + 50;
    func_15102920(fraction * 25.0f + 15.0f, random % 56U + 200, &hit->triangle, &hit->position,
                  lifetime, 1, 1, hit->surfaceFlags, obj->base.header.owner, obj->base.header.kind);
    surfaceFlags = hit->surfaceFlags;
    if ((surfaceFlags & 0x1F) == 9) {
        if (func_150ADA68() < 0.400000006f) {
            func_151C329C(&hit->position, obj->base.header.owner, obj->base.header.kind);
        }
    } else {
        if (func_150ADA68() < 0.1000000015f) {
            func_151C329C(&hit->position, obj->base.header.owner, obj->base.header.kind);
        }
        if (func_150ADA68() < 0.400000006f) {
            debris.variantIds = D_800A23A4;
            debris.variantScales = D_800A23A8;
            debris.desc.countBase = 1;
            debris.desc.countRange = 3;
            debris.desc.pos = hit->position;
            debris.desc.source.normal.x = -hit->velocity.x;
            debris.desc.source.normal.y = -hit->velocity.y;
            debris.desc.source.normal.z = -hit->velocity.z;
            debris.desc.source.triangle = hit->triangle;
            debris.desc.source.spread = 614.0f;
            debris.desc.speedBase = 0.009000000544f;
            debris.desc.lifeBase = 15;
            debris.desc.lifeRange = 15;
            debris.desc.speedRange = 0.01000000071f;
            debris.desc.gravityBase = -0.9040000439f;
            debris.desc.gravityRange = 0.3950000107f;
            debris.desc.scaleBase = 0.06000000238f;
            debris.desc.scaleRange = 0.1420000046f;
            debris.desc.spin = 25.77700043f;
            func_15151D6C(&debris.desc, debris.variantIds.v, debris.variantScales.v, 1, obj->base.header.owner,
                          obj->base.header.kind);
        }
    }
}

/* Sparks thrown up from random points of a spawn region (func_151058B4, func_15105848, func_15105BC8). */
typedef struct SparkState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 rate;
    /* 0x08 */ s16 *unk8;
    /* 0x0C */ u8 flags;           /* bit 0: on (set by message 0x38, cleared by 0x39) */
} SparkState;

struct SparkObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ SparkState state;
};

extern void func_151432BC(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom);
extern void func_15143794(s16, s16, f32, f32 *);
extern void func_1515C2F0(EffectTemplate18 *, s32, TrailBurstParams *, s32, s32, s32);
void func_151058B4(SparkObj *obj);

/* Message handler: 0x38 bursts sparks and switches the effect on, 0x39 switches it off. */
void func_15105848(SparkObj *obj, s32 arg1, u8 msg) {
    SparkState *state = &obj->state;

    if (msg == 0x38) {
        func_151058B4(obj);
        state->flags |= 1;
    } else if (msg == 0x39) {
        state->flags &= ~1;
    }
}

/* Throws a rate-scaled random number of sparks from random points of the region. */
void func_151058B4(SparkObj *obj) {
    SparkState *state;
    f32 count;
    u32 angle;
    EffectTemplate18 spawn;
    TrailBurstParams part;
    f32 top;
    f32 bottom;

    state = &obj->state;
    if (func_151464B8(state->unk8) == 0) {
        count = (func_150ADA68() * 0.0013f + 0.001899999916f) * state->rate;
        if (count > 1.0f) {
            spawn.flags = 0x15;
            spawn.kind = 10;
            spawn.unk14 = -1;
            part.flags = 4;
            part.primMode = 2;
            part.envMode = 3;
            part.color.red = 0x61;
            part.color.green = 0xF2;
            part.color.blue = 0xFF;
            part.color2.red = 0xFF;
            part.color2.green = 0xFF;
            part.color2.blue = 0xFF;
            part.color2.alpha = 0xFF;
            part.colorLevel = 0xFF;
            part.renderMode = 3;
            part.unk21 = 0x24;
            part.setGeometryMode = 0x200005;
            part.clearGeometryMode = 0x60600;
            part.fadeStart = 20;
            part.fadeRate = 12;
            part.shrinkStart = 1;
            part.shrinkPaused = 0;
            part.shrinkFactor = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            spawn.pos.y = state->region->y;
            do {
                part.color.alpha = func_150ADA20() % 101U + 155;
                spawn.itemCount = (func_150ADA20() & 3) + 3;
                func_151432BC(state->region, &spawn.pos.x, &spawn.pos.z, &top, &bottom);
                spawn.life = func_150ADA20() % 31U + 30;
                part.width = func_150ADA68() * 10.13f + 7.399999619f;
                angle = func_150ADA20();
                func_15143794(angle & 0xFF, func_150ADA20() % 22U - 54, func_150ADA68() * 20.0f + 30.0f, &part.vel.x);
                part.gravity = func_150ADA68() * 1.486000061f + -3.048f;
                func_1515C2F0(&spawn, 0, &part, 0, obj->base.header.owner, obj->base.header.kind);
                count -= 1.0f;
            } while (count > 1.0f);
        }
    }
}

/* While the sparks are on, calls func_1508B20C at the region's position. */
void func_15105BC8(SparkObj *obj) {
    if (obj->state.flags & 1) {
        func_1508B20C(obj->state.region->x, obj->state.region->y, obj->state.region->z, 500.0f);
    }
}

/* Returns the first type-0x2E object owned by `owner` in either object list, searching the two
 * object types listed in D_800A5770, or NULL. */
ListObj *func_15105C24(s32 owner) {
    u8 i;
    u8 list;
    ListObj *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = (ListObj *)D_800DCE50[list][D_800A5770[i]]; obj != NULL; obj = obj->next) {
                if (obj->type == 0x2E && owner == obj->owner) {
                    return obj;
                }
            }
        // matching: the label starts a new basic block, which keeps IDO from hoisting list++
        next_list:;
        }
    }
    return NULL;
}
