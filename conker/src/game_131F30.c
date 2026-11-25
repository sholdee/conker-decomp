#include <ultra64.h>
/* functions.h narrows func_150ADA20's return to u8 (the code uses all 32 bits) and declares the
 * message handlers below with struct207, whose 0x28 is a pointer; here 0x28 holds the state inline. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_15105548 func_15105548_decl_in_functions_h
#define func_15105848 func_15105848_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_15105548
#undef func_15105848
u32 func_150ADA20(void);
#include "variables.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { u8 r, g, b, a; } ColorRGBA;

/* Same layout as SpawnRegion in game_16EE20.c. */
typedef struct SpawnRegion {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; /* degrees */
    /* 0x14 */ u8 unk14;     /* the effects here set it to 1 while off */
    /* 0x15 */ u8 flags;     /* bits 0-1: shape */
} SpawnRegion;

/* Same layout as BoltDesc in game_133190.c: a lightning bolt along a cubic Bezier. */
typedef struct BoltDesc {
    /* 0x00 */ s16 life;
    /* 0x02 */ u8 flags;          /* bit 0: life counts down */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ Vec3f ctrl[4];     /* Bezier control points; ctrl[0] and ctrl[3] are the end points */
    /* 0x34 */ s8 updateKind;     /* -1: none */
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 segments;
    /* 0x3C */ f32 jitter;        /* max sideways offset of each joint */
    /* 0x40 */ f32 width;         /* half width of the drawn strip */
    /* 0x44 */ s16 rejitterBase;  /* frames between re-jitters: base + rand % (range + 1) */
    /* 0x46 */ s16 rejitterRange;
    /* 0x48 */ ColorRGBA color;
    /* 0x4C */ u8 kind;
    /* 0x4D */ u8 pad4D[3];
} BoltDesc; /* size 0x50 */

extern s32 D_800DCE50[][0x68];

typedef struct Func15105C24Obj {
    u8 pad0[8];
    struct Func15105C24Obj *next;
    u8 padC[7];
    u8 type;
    u8 pad14[0x14];
    s32 owner;
} Func15105C24Obj;

extern s32 *D_800DBF94;

typedef struct {
    s32 count;
    const u8 *list;
    struct127 *player;
} Msg35;

/* Sends message 0x35 (selection list for kinds 0xF7-0xF9) or 0x38 (kinds 0xF6) about the
 * first active player in this object's player mask. */
void func_15104A80(struct131 *arg0) {
    struct127 *player;

    player = NULL;
    {
        s32 mask;
        s32 i;
        struct127 *candidate;

        i = arg0 - D_800DBEF4;
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
    if (((u8 *)arg0)[0x72] == 0xF9 || ((u8 *)arg0)[0x72] == 0xF8 || ((u8 *)arg0)[0x72] == 0xF7) {
        Msg35 msg;

        msg.player = player;
        if (((u8 *)arg0)[0x72] == 0xF9) {
            const u8 list[9] = { 0, 1, 2, 3, 4, 5, 15, 16, 17 };

            msg.count = 9;
            msg.list = list;
        } else if (((u8 *)arg0)[0x72] == 0xF8) {
            const u8 list[9] = { 6, 7, 8, 24, 25, 26, 18, 19, 20 };

            msg.count = 9;
            msg.list = list;
        } else if (((u8 *)arg0)[0x72] == 0xF7) {
            const u8 list[9] = { 9, 10, 11, 12, 13, 14, 21, 22, 23 };

            msg.count = 9;
            msg.list = list;
        }
        func_151494E0((s32)&msg, 0x35);
    } else if (((u8 *)arg0)[0x72] == 0xF6) {
        Msg35 msg;

        msg.player = player;
        func_151494E0((s32)&msg.player, 0x38);
    }
}

/* Header shared by the effect objects in this file; each kind keeps its own state at 0x28. */
typedef struct ObjHeader {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
} ObjHeader; /* size 0x28 */

/* Rain of lightning bolts falling around a point onto the ground (func_15104C44, func_15104FF8). */
typedef struct BoltRainState {
    /* 0x00 */ SpawnRegion *region;
    /* 0x04 */ f32 pending;      /* bolts owed, accumulated per frame */
    /* 0x08 */ s16 timer;        /* frames left; negative when off */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 height;       /* bolts start this far above pos */
    /* 0x20 */ f32 groundY;
    /* 0x24 */ u8 pad24[0x20];
    /* 0x44 */ s16 *unk44;
} BoltRainState;

typedef struct BoltRainObj {
    /* 0x00 */ ObjHeader header;
    /* 0x28 */ BoltRainState state;
} BoltRainObj;

s32 func_1000FD38();
extern s32 func_151464B8(s16 *);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern s32 func_15046C80(f32 *pos, s32, f32 y, f32 *groundY);
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
                desc.color.r = 0x61;
                desc.color.g = 0xF2;
                desc.color.b = 0xFF;
                do {
                    func_15143874(func_150ADA20() & 0xFF, (func_150ADA68() * 0.5f + 0.5f) * state->radius,
                                  &ground[0], &ground[2]);
                    ground[0] += state->pos.x;
                    ground[2] += state->pos.z;
                    if (func_15046C80(ground, 0, state->pos.y, &state->groundY) != 0) {
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX1, &offZ1);
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX2, &offZ2);
                        desc.ctrl[1].x = state->pos.x + offX1;
                        desc.ctrl[1].z = state->pos.z + offZ1;
                        desc.ctrl[3].x = ground[0];
                        desc.ctrl[3].y = state->groundY;
                        desc.ctrl[3].z = ground[2];
                        desc.ctrl[2].x = offX2 + ground[0];
                        desc.ctrl[2].y = desc.ctrl[3].y + 180.0f;
                        desc.ctrl[2].z = offZ2 + ground[2];
                        desc.life = func_150ADA20() % 13U + 8;
                        desc.color.a = func_150ADA20() % 156U + 100;
                        desc.segments = func_150ADA20() % 3U + 5;
                        func_15105CE0(&desc, 0, obj->header.unkC, obj->header.unk1);
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

    if (msg == 0x38 && state->region->unk14 == 1) {
        state->timer = 300;
        // matching: the (s32) copy keeps IDO from sharing one register for region across the call
        func_1000FD38(func_1000EF40, (s32)state->region, 0);
        func_1000FA64(0x236, state->region->x, state->region->y, state->region->z, 0x4000, 1500, 1000,
                      (s32)func_1000EF40, state->region, 0, 8, 0);
    }
}

/* Same layout as BoltChainConfig in game_133190.c: spawn parameters for each bolt of a chain. */
typedef struct BoltChainConfig {
    /* 0x00 */ f32 widthBase;
    /* 0x04 */ f32 widthRange;
    /* 0x08 */ f32 jitter;
    /* 0x0C */ f32 bendBase;       /* distance of the inner control points from the ends */
    /* 0x10 */ f32 bendRange;
    /* 0x14 */ s16 bendSpread;     /* pitch range of the inner control points */
    /* 0x16 */ s16 rejitterBase;
    /* 0x18 */ s16 rejitterRange;
    /* 0x1A */ s16 lifeBase;
    /* 0x1C */ s16 lifeRange;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 segmentsBase;
    /* 0x24 */ s32 segmentsRange;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 alphaBase;
    /* 0x2C */ u8 alphaRange;
} BoltChainConfig; /* size 0x30 */

/* Lightning arcing between the four edges of a quad (func_151050B0, func_15105548). */
typedef struct BoltArcState {
    /* 0x00 */ SpawnRegion *region;
    /* 0x04 */ s16 timer;          /* frames left; negative when off */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 pending;        /* bolts owed, accumulated per frame */
    /* 0x0C */ Vec3f edgeStart[4];
    /* 0x3C */ Vec3f edgeDelta[4]; /* edge i runs from edgeStart[i] to edgeStart[i] + edgeDelta[i] */
    /* 0x6C */ s16 *unk6C;
    /* 0x70 */ s32 unk70;
} BoltArcState;

typedef struct BoltArcObj {
    /* 0x00 */ ObjHeader header;
    /* 0x28 */ BoltArcState state;
} BoltArcObj;

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
        state->region->unk14 = 1;
        return;
    }
    state->region->unk14 = 0;
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
            func_15106F98(&start, &end, 3, &config, 0.3250000179f, 2, obj->header.unkC, obj->header.unk1);
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
    }
}

/* Message handler: 0x4B releases the object. */
void func_1510550C(struct102 *obj, s32 arg1, u8 msg) {
    if (msg == 0x4B) {
        func_1516972C(obj);
    }
}

/* Message handler: on 0x38 while the region is idle, restarts the arcs for 300 frames. */
void func_15105548(BoltArcObj *obj, s32 *arg1, u8 msg) {
    BoltArcState *state = &obj->state;

    if (msg == 0x38 && state->region->unk14 == 1) {
        state->unk70 = *arg1;
        state->timer = 300;
    }
}

typedef struct { f32 x, y, z; } Vec3f_1510558C;
typedef struct Game131F30Surface { s16 values[9]; } Game131F30Surface;
typedef struct Game131F30Color { s32 value; } Game131F30Color;
typedef struct Game131F30Factor { f32 value; } Game131F30Factor;
typedef struct Game131F30Descriptor {
    s32 count, spread;
    Vec3f_1510558C position, velocity;
    Game131F30Surface surface;
    u8 pad32[2];
    f32 field34, field38, field3C, field40, field44;
    s16 duration, durationRange;
    f32 field4C, field50, field54;
} Game131F30Descriptor;
typedef struct Game131F30Part {
    u8 pad0[0x30];
    Vec3f_1510558C position;
    u8 pad3C[0x24];
    Vec3f_1510558C velocity;
    Game131F30Surface surface;
    u8 pad7E[0xA];
    s32 flags;
} Game131F30Part;
typedef struct Game131F30Emitter {
    u8 pad0, group, pad2[0xA], mode, padD[0x103];
    Game131F30Part part;
} Game131F30Emitter;

s32 func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void func_151C329C(void *, u8, s32);
void func_15151D6C(Game131F30Descriptor *, Game131F30Color *, Game131F30Factor *, s32, u8, s32);
extern Game131F30Color D_800A23A4;
extern Game131F30Factor D_800A23A8;
extern f32 D_800A23C0, D_800A23C4, D_800A23C8, D_800A23CC, D_800A23D0;
extern f32 D_800A23D4, D_800A23D8, D_800A23DC, D_800A23E0, D_800A23E4, D_800A23E8;

void func_1510558C(Game131F30Emitter *arg0) {
    Game131F30Part *part;
    u32 flags;
    struct {
        Game131F30Factor factor;
        Game131F30Color color;
        Game131F30Descriptor descriptor;
    } work;
    u32 lifetime;
    u32 random;
    f32 fraction;

    part = (Game131F30Part *)((u8 *)arg0 + 0x110);
    fraction = func_150ADA68();
    random = func_150ADA20();
    lifetime = func_150ADA20() % 81U + 0x32;
    func_15102920(fraction * 25.0f + 15.0f, ((random % 56U) + 0xC8) & 0xFF,
                  &part->surface, &part->position, lifetime, 1, 1,
                  part->flags, arg0->mode, arg0->group);
    flags = part->flags;
    if ((flags & 0x1F) == 9) {
        if (func_150ADA68() < D_800A23C0) {
            func_151C329C(&part->position, arg0->mode, arg0->group);
        }
    } else {
        if (func_150ADA68() < D_800A23C4) {
            func_151C329C(&part->position, arg0->mode, arg0->group);
        }
        if (func_150ADA68() < D_800A23C8) {
            work.color = D_800A23A4;
            work.factor = D_800A23A8;
            work.descriptor.count = 1;
            work.descriptor.spread = 3;
            work.descriptor.position = part->position;
            work.descriptor.velocity.x = -part->velocity.x;
            work.descriptor.velocity.y = -part->velocity.y;
            work.descriptor.velocity.z = -part->velocity.z;
            work.descriptor.surface = part->surface;
            work.descriptor.field34 = D_800A23CC;
            work.descriptor.field38 = D_800A23D0;
            work.descriptor.duration = 15;
            work.descriptor.durationRange = 15;
            work.descriptor.field3C = D_800A23D4;
            work.descriptor.field40 = D_800A23D8;
            work.descriptor.field44 = D_800A23DC;
            work.descriptor.field4C = D_800A23E0;
            work.descriptor.field50 = D_800A23E4;
            work.descriptor.field54 = D_800A23E8;
            func_15151D6C(&work.descriptor, &work.color, &work.factor, 1, arg0->mode, arg0->group);
        }
    }
}

/* Sparks thrown up from random points of a spawn region (func_151058B4, func_15105848, func_15105BC8). */
typedef struct SparkState {
    /* 0x00 */ SpawnRegion *region;
    /* 0x04 */ f32 rate;
    /* 0x08 */ s16 *unk8;
    /* 0x0C */ u8 flags;           /* bit 0: on (set by message 0x38, cleared by 0x39) */
} SparkState;

typedef struct SparkObj {
    /* 0x00 */ ObjHeader header;
    /* 0x28 */ SparkState state;
} SparkObj;

typedef struct SparkSpawn {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 unk15;
} SparkSpawn;

typedef struct SparkParticle {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 vel[3];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 unk39;
} SparkParticle;

extern void func_151432BC(SpawnRegion *region, f32 *x, f32 *z, f32 *top, f32 *bottom);
extern void func_15143794(s16, s16, f32, f32 *);
extern void func_1515C2F0(SparkSpawn *, s32, SparkParticle *, s32, s32, s32);
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
    SparkSpawn spawn;
    SparkParticle part;
    f32 top;
    f32 bottom;

    state = &obj->state;
    if (func_151464B8(state->unk8) == 0) {
        count = (func_150ADA68() * 0.0013f + 0.001899999916f) * state->rate;
        if (count > 1.0f) {
            spawn.unkE = 0x15;
            spawn.unk10 = 10;
            spawn.unk14 = -1;
            part.unk14 = 4;
            part.unk15 = 2;
            part.unk16 = 3;
            part.unk17 = 0x61;
            part.unk18 = 0xF2;
            part.unk19 = 0xFF;
            part.unk1B = 0xFF;
            part.unk1C = 0xFF;
            part.unk1D = 0xFF;
            part.unk1E = 0xFF;
            part.unk1F = 0xFF;
            part.unk20 = 3;
            part.unk21 = 0x24;
            part.unk24 = 0x200005;
            part.unk28 = 0x60600;
            part.unk2C = 20;
            part.unk2E = 12;
            part.unk30 = 1;
            part.unk32 = 0;
            part.unk34 = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            spawn.y = state->region->y;
            do {
                part.unk1A = func_150ADA20() % 101U + 155;
                spawn.unk15 = (func_150ADA20() & 3) + 3;
                func_151432BC(state->region, &spawn.x, &spawn.z, &top, &bottom);
                spawn.unkC = func_150ADA20() % 31U + 30;
                part.unk0 = func_150ADA68() * 10.13f + 7.399999619f;
                angle = func_150ADA20();
                func_15143794(angle & 0xFF, func_150ADA20() % 22U - 54, func_150ADA68() * 20.0f + 30.0f, part.vel);
                part.unk10 = func_150ADA68() * 1.486000061f + -3.048f;
                func_1515C2F0(&spawn, 0, &part, 0, obj->header.unkC, obj->header.unk1);
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

Func15105C24Obj *func_15105C24(s32 arg0)
{
  u8 i;
  u8 j;
  Func15105C24Obj *obj;
 i = 0; do {
    j = 0;
    do
    {
      obj = (Func15105C24Obj *) D_800DCE50[j][(&D_800A5770)[i]];
      if (obj != 0)
      {
        do
        {
          if ((obj->type == 0x2E) && (arg0 == obj->owner))
          {
            return obj;
          }
          obj = obj->next;
        }
        while (obj != 0);
      }
      next_j:
      j++;

    }
    while (j < 2);
    next_i:
    i++;

  }
  while (i < 2);
  return 0;
}
// void *func_15105C24(s32 arg0) {
//     s32 temp_t4;
//     s32 temp_t5;
//     void *temp_a0;
//     void *temp_a0_2;
//     s32 phi_v1;
//     s32 phi_v0;
//     void *phi_a0;
//
//     phi_v0 = 0;
// loop_1:
//     phi_v1 = 0;
// loop_2:
//     temp_a0 = *((((((phi_v1 * 4) - phi_v1) * 4) + phi_v1) << 5) + 0x800DCE50 + (((phi_v0 * 4) + 0x800A0000)->unk5770 * 4));
//     phi_a0 = temp_a0;
//     if (temp_a0 != 0) {
// loop_3:
//         if ((phi_a0->unk13 == 0x2E) && (arg0 == phi_a0->unk28)) {
//             return phi_a0;
//         }
//         temp_a0_2 = phi_a0->unk8;
//         phi_a0 = temp_a0_2;
//         if (temp_a0_2 != 0) {
//             goto loop_3;
//         }
//     }
//     temp_t4 = (phi_v1 + 1) & 0xFF;
//     phi_v1 = temp_t4;
//     if (temp_t4 < 2) {
//         goto loop_2;
//     }
//     temp_t5 = (phi_v0 + 1) & 0xFF;
//     phi_v0 = temp_t5;
//     if (temp_t5 < 2) {
//         goto loop_1;
//     }
//     return NULL;
// }
