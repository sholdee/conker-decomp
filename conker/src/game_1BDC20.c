#include <ultra64.h>
#define func_15169260 func_15169260_s32
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_15169260
#undef func_1513D524
#include "variables.h"

void *memcpy(void *, const void *, u32);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15169260(void *, s32, s32, u8);
extern f32 D_800A8018;
extern void func_151D5D60();
extern void func_1513F680();
extern void func_15143134(void *, void *, void *);
extern void *func_1513D524(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_15191B8C(s32, u8);
extern u8 func_151D8E20(void);

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 width, height;
} Size2f;

/* Spawn template of a particle (as in game_169510.c), copied to Particle.def. */
typedef struct {
    u8 texture;
    u8 type;
    u16 renderModes;
    s16 lifetime;
    u8 pad6[2];
    s32 frame;
    s32 frameSpeed;
    u8 rgba[4];
    f32 width;
    f32 height;
    Vec3f pos;
    Vec3f rot;
    Vec3f scale;
    s32 flags;
    u8 unk44;           /* alpha, faded out with the lifetime */
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s16 *unk48;
    u8 viewportMask;
    u8 pad4D[3];
    s32 unk50;
    s16 fadeStart;
    s16 fadeRate;
} ParticleDef; /* size 0x58 */

/* Ground probe state filled by func_1504715C and updated by func_15046C80. */
typedef struct {
    /* 0x00 */ f32 y;
    /* 0x04 */ u8 unk4[0x18];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
} Ground; /* size 0x24 */

/* Motion state of a drop that hangs on an actor's bone, then falls and splashes. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 drag;
    /* 0x14 */ s16 fadeStart;       /* lifetime below which the alpha fades */
    /* 0x16 */ s16 fadeRate;        /* alpha per frame of lifetime left */
    /* 0x18 */ f32 trail;           /* how far def.rot (the tail) lags behind def.pos */
    /* 0x1C */ f32 stretch;         /* tilt per sqrt of sideways speed (func_1519108C) */
    /* 0x20 */ u8 dropKind;         /* D_800A9D40 index */
    /* 0x21 */ u8 phase[2];         /* width / height wobble angles */
    /* 0x23 */ s8 phaseSpeed[2];
    /* 0x28 */ Size2f baseSize;
    /* 0x30 */ Size2f wobble;
    /* 0x38 */ Ground ground;
    /* 0x5C */ u8 kind;             /* emitter kind, passed to the splash spawners */
    /* 0x5D */ u8 pad5D[3];
} DripMotion; /* size 0x60 */

typedef struct Particle {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 list;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ struct Particle *prev;
    /* 0x008 */ struct Particle *next;
    /* 0x00C */ u8 owner;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ s32 active;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0x50];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuffers[4];
    /* 0x110 */ DripMotion motion;
} Particle;

/* D_800A9D40: lifetime and wobble of each kind of falling drop. */
typedef struct {
    /* 0x00 */ s16 lifeMin;
    /* 0x02 */ s16 lifeRange;
    /* 0x04 */ Size2f baseSize;
    /* 0x0C */ Size2f wobble;
} DropKind; /* size 0x14 */

extern DropKind D_800A9D40[];

/* Actor reference sent with object messages (func_15191B8C). */
typedef struct {
    struct127 *actor;
    u8 id;
} ActorRef;

/* An emitter's attachment to its actor. */
typedef struct {
    /* 0x0 */ struct127 *actor;
    /* 0x4 */ u8 actorId;           /* actor->unique_id when attached */
    /* 0x6 */ s16 timer;
    /* 0x8 */ u8 flags;             /* bit 0: the timer counts down */
    /* 0xA */ u8 kind;              /* D_8008FA70 index */
} ActorLink; /* size 0xC */

typedef struct DripSlot {
    /* 0x0 */ Particle *drop;
    /* 0x4 */ s32 timer;            /* frames until the drop falls */
    /* 0x8 */ struct DripSlot *next;
} DripSlot;

#define DRIP_SLOT_COUNT 60

/* Object type 0x20: hangs drops on the bones of an actor. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ActorLink link;
    /* 0x1C */ f32 spawnAccum;
    /* 0x20 */ s32 dropCount;
    /* 0x24 */ DripSlot *slots;
    /* 0x28 */ void *extra;         /* caller-sized data after the slots */
    /* 0x2C */ u8 kind;             /* D_800AB310 / D_800AB404 index */
    /* 0x30 */ DripSlot slotStorage[DRIP_SLOT_COUNT];
} DripEmitter; /* size 0x300 */

/* Destroys the emitter already on link->actor, then creates one with extraSize bytes of extra data. */
DripEmitter *func_15190770(ActorLink *link, s32 extraSize, u8 owner, s32 list) {
    DripEmitter *emitter;
    ActorRef ref;

    ref.actor = link->actor;
    ref.id = link->actor->unique_id;
    func_15191B8C((s32)&ref, 0xE);
    emitter = func_15167A68(0x20, list, extraSize + sizeof(DripEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->link, link, sizeof(ActorLink));
    emitter->slots = emitter->slotStorage;
    emitter->extra = emitter + 1;
    emitter->dropCount = 0;
    emitter->spawnAccum = 0.0f;
    bzero(emitter->slotStorage, sizeof(emitter->slotStorage));
    emitter->kind = func_151D8E20();
    return emitter;
}

void func_15191400(DripEmitter *emitter);

/* Type 0x20 handler: releases the drops, then func_15169804. */
void func_15190840(DripEmitter *emitter) {
    func_15191400(emitter);
    func_15169804((struct102 *)emitter);
}

/* Type 0x20 handler: releases the drops, then func_15169824. */
void func_1519086C(DripEmitter *emitter) {
    func_15191400(emitter);
    func_15169824((struct102 *)emitter);
}

typedef struct F898Owner {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
    u8 pad11C[0xB8];
    u8 (*unk1D4)[0x40];
} F898Owner;

typedef struct F898V2 {
    s32 a, b;
} F898V2;

typedef struct F898V3 {
    s32 a, b, c;
} F898V3;

typedef struct F898Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 pad25[3];
    s32 unk28[2];
    s32 unk30[2];
    u8 unk38[0x24];
    u8 unk5C;
    u8 pad5D[3];
} F898Sub;

typedef struct F898Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x16];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xC];
    u32 unk58;
    u8 pad5C[0xB4];
    F898Sub sub;
} F898Obj;

typedef struct F898Slot {
    F898Obj *obj;
    s32 timer;
    struct F898Slot *next;
} F898Slot;

typedef struct F898Sk {
    u8 pad0[0xC];
    u8 unkC;
    u8 padD[3];
} F898Sk;

typedef struct F898Entry {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    F898Sk *unk8;
    f32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
} F898Entry;

typedef DropKind F898Range;

typedef struct F898Arg {
    u8 pad0[0x10];
    F898Owner *unk10;
    u8 unk14;
    u8 pad15;
    s16 unk16;
    u8 unk18;
    u8 pad19;
    u8 unk1A;
    u8 pad1B;
    f32 unk1C;
    s32 unk20;
    F898Slot *unk24;
    u8 pad28[4];
    u8 unk2C;
} F898Arg;

typedef struct F898Spawn {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C[3];
    f32 unk28[3];
    f32 unk34[3];
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 pad46[2];
} F898Spawn;

extern F898Entry D_8008FA70[];
extern u8 D_800AB310[];
extern u8 D_800AB404[];
extern f32 D_800A8014;
extern void func_1504715C(void *, void *);

void func_15190898(F898Arg *arg0) {
    F898Arg *self;
    volatile u8 done;
    F898Entry *ent;
    F898Slot *slot;
    F898Slot *head;
    F898Slot *tail;
    s32 count;
    s32 i;
    s32 keep;
    F898Obj *o;
    F898Sub *sub;
    F898Range *rng;
    f32 dx;
    f32 dz;
    s32 idx;
    s32 n;
    F898Obj *res;
    s32 kz;
    s32 flag;
    F898Spawn b;
    F898Sub a;
    s16 va0;
    u8 va3;
    s32 pad5;

    self = arg0;
    done = 0;
    if (arg0->unk18 & 1) {
        arg0->unk16 = arg0->unk16 - D_800BE9E4;
        if (arg0->unk16 < 0) {
            done = 1;
        }
    }
    if (done == 0 && (arg0->unk10->unk0 == 0 || arg0->unk10->unk3B != arg0->unk14)) {
        done = 1;
    }
    if (done == 0) {
        if (arg0->unk10->unk1D4 != NULL) {
            ent = &D_8008FA70[arg0->unk1A];
            head = NULL;
            tail = NULL;
            count = 0;
            i = 0;
            slot = arg0->unk24;
            for (; i < ent->unk4; i++, slot++) {
                if (slot->obj != NULL) {
                    *(F898V3 *)&slot->obj->unk40 = *(F898V3 *)&slot->obj->unk34;
                    func_15143134(ent->unk8 + i, &slot->obj->unk34, arg0->unk10->unk1D4 + ent->unk8[i].unkC);
                    slot->timer = slot->timer - D_800BE9E4;
                    if (slot->timer < 0) {
                        F898Obj *o2 = slot->obj;
                        F898Sub *sub2 = &slot->obj->sub;

                        rng = &D_800A9D40[sub2->unk20];
                        dx = (o2->unk34 - o2->unk40) * D_800BE9A8;
                        dz = (o2->unk3C - o2->unk48) * D_800BE9A8;
                        sub2->unk4 = 0.0f;
                        sub2->unk0 = dx;
                        sub2->unk8 = dz;
                        func_1513F680(slot->obj, 0xD, 0x12, 0, 0xD);
                        keep = 1;
                        slot->obj->unk1C = func_150ADA20() % (u32)(rng->lifeRange + 1) + rng->lifeMin;
                        slot->obj->unk58 |= 1;
                        slot->obj = NULL;
                        arg0->unk20 = arg0->unk20 - 1;
                    } else {
                        keep = 0;
                    }
                } else {
                    keep = 1;
                }
                if (keep != 0) {
                    count += 1;
                    if (tail == NULL) {
                        tail = slot;
                    }
                    slot->next = head;
                    head = slot;
                }
            }
            if (count != 0) {
                arg0->unk1C = arg0->unk1C + ent->unkC * D_800BE9A4;
                if (arg0->unk1C > 1.0f) {
                    F898Range *rng2;

                    rng2 = &D_800A9D40[ent->unk0];
                    tail->next = head;
                    a.unk0 = 0.0f;
                    a.unk4 = 0.0f;
                    a.unk8 = 0.0f;
                    a.unk10 = 0.2f;
                    a.unk14 = 0xF;
                    a.unk16 = 0x11;
                    a.unk18 = 0.0f;
                    a.unk1C = 0.2f;
                    a.unkC = -1.0f;
                    a.unk20 = ent->unk0;
                    func_1504715C(a.unk38, arg0->unk10);
                    a.unk5C = arg0->unk2C;
                    b.unk0 = D_800AB310[arg0->unk2C];
                    b.unk2 = 0x1001;
                    b.unk4 = 0x12C;
                    b.unk8 = 0;
                    b.unkC = 0;
                    b.unk10 = 0;
                    b.unk11 = 0;
                    b.unk12 = 0;
                    b.unk13 = 0xFF;
                    b.unk28[0] = 0.0f;
                    b.unk28[1] = 0.0f;
                    b.unk28[2] = 0.0f;
                    b.unk40 = 0;
                    b.unk45 = 0xFF;
                    b.unk34[0] = 1.0f;
                    b.unk34[1] = 1.0f;
                    b.unk34[2] = 1.0f;
                    a.unk21 = 0;
                    a.unk22 = 0;
                    do {
                        idx = func_150ADA20() % (u32)count;
                        while (idx--) {
                            tail = head;
                            head = head->next;
                        }
                        n = head - arg0->unk24;
                        b.unk44 = func_150ADA20() % 101U + 100;
                        b.unk14 = 0.0f;
                        b.unk18 = 0.0f;
                        *(F898V2 *)a.unk28 = *(F898V2 *)&rng2->baseSize;
                        *(F898V2 *)a.unk30 = *(F898V2 *)&rng2->wobble;
                        a.unk23 = func_150ADA20() % 5U + 4;
                        a.unk24 = func_150ADA20() % 5U + 4;
                        func_15143134(ent->unk8 + n, b.unk1C, arg0->unk10->unk1D4 + ent->unk8[n].unkC);
                        *(F898V3 *)b.unk28 = *(F898V3 *)b.unk1C;
                        if (arg0->unk10->unk118 < b.unk1C[1] - *(f32 *)&a.unk28[1] - *(f32 *)&a.unk30[1]) {
                            res = func_1513D524(&b, 12, 0, 0, 0xD, ((func_150ADA20() % 1) ? 1 : 0) | (D_800AB404[arg0->unk2C] ? 0 : 2), 0x60, 0xFF, 0);
                            head->obj = res;
                            if (res != NULL) {
                                memcpy(&res->sub, &a, sizeof(a));
                                head->timer = func_150ADA20() % (u32)(ent->unk16 + 1) + ent->unk14;
                                count -= 1;
                                head = head->next;
                                tail->next = head;
                                arg0->unk20 = arg0->unk20 + 1;
                            }
                        }
                        arg0->unk1C = arg0->unk1C - 1.0f;
                    } while (arg0->unk1C > 1.0f && count != 0 && self->unk20 < ent->unk10);
                }
            }
        } else {
            done = 1;
            if (arg0->unk18 & 1) {
                va3 = 1;
                va0 = arg0->unk16;
            } else {
                va3 = 0;
                va0 = 0x12C;
            }
            res = (F898Obj *)func_151491F4(va0, -1, 0xD, va3, 8, 0xC, 0xFF, 0);
            if (res != NULL) {
                memcpy((u8 *)res + 0x28, &arg0->unk10, 0xC);
            }
        }
    }
    if (done != 0) {
        func_1516972C((void *)arg0);
    }
}

/* Wobbles a drop's width and height around its base size. */
s32 func_15190F9C(Particle *drop) {
    Particle *p = drop; // matching: the param copy steers register allocation
    DripMotion *motion = &drop->motion;
    f32 widthWave;
    f32 heightWave;

    motion->phase[0] += motion->phaseSpeed[0] * D_800BE9E4;
    motion->phase[1] += motion->phaseSpeed[1] * D_800BE9E4;
    widthWave = func_151423D8((u8)(motion->phase[0] - 0x40));
    heightWave = func_151423D8((u8)(motion->phase[1] - 0x40));
    p->def.width += (motion->baseSize.width + motion->wobble.width * widthWave - p->def.width) * 0.5f;
    p->def.height += (motion->baseSize.height + motion->wobble.height * heightWave - p->def.height) * 0.5f;
    return 1;
}

struct Struct1519108C {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
    f32 unk12C;
};

s16 *func_1519108C(struct Struct1519108C *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f2;
    f32 sinval;
    f32 temp_f18;
    f32 temp_y_cos;
    f32 temp_y_sin;
    f32 var_f2;
    f32 var_f14;
    f32 *temp_v1;
    f32 *temp_a0;
    f32 temp_f0;
    u8 sp37;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp37);
    ret = var_v0;
    if (var_v0) {
        if (sp37) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_v1 = &D_800DD1E8[arg1];
    temp_a0 = &D_800DD1D8[arg1];
    var_f14 = -1.0f;
    var_f2 = ((*temp_v1 * (arg0->unk40 - arg0->unk34)) - (*temp_a0 * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f2 < 0.0f) {
        var_f14 = 1.0f;
        var_f2 = -var_f2;
    }

    temp_f0 = sqrtf(var_f2) * arg0->unk12C;
    if (D_800A8018 < temp_f0) {
        temp_f0 = D_800A8018;
    }

    temp_f0 = temp_f0 * var_f14;
    sinval = sinf(temp_f0);
    temp_f0 = cosf(temp_f0);
    temp_f2 = arg0->unk2C * temp_f0;
    temp_f18 = arg0->unk2C * sinval;
    temp_y_cos = arg0->unk30 * temp_f0;
    temp_y_sin = arg0->unk30 * sinval;

    var_v0[0] = (*temp_v1 * -temp_f2) + arg0->unk34;
    var_v0[1] = arg0->unk38 + temp_f18;
    var_v0[2] = arg0->unk3C - (*temp_a0 * -temp_f2);
    var_v0[3] = 0;

    var_v0[8] = (*temp_v1 * temp_f2) + arg0->unk34;
    var_v0[9] = arg0->unk38 - temp_f18;
    var_v0[10] = arg0->unk3C - (*temp_a0 * temp_f2);
    var_v0[11] = 0;

    temp_f0 = temp_f2 - temp_y_sin;
    var_v0[16] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[17] = (arg0->unk38 - temp_f18) - temp_y_cos;
    var_v0[18] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[19] = 0;

    temp_f0 = -(temp_f2 + temp_y_sin);
    var_v0[24] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[25] = (arg0->unk38 + temp_f18) - temp_y_cos;
    var_v0[26] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[27] = 0;

    return ret;
}

extern void func_1513F680();

/* Lets go of every drop the emitter still holds and clears its slots. */
void func_15191400(DripEmitter *emitter) {
    s32 i;
    DripSlot *slot;
    DripMotion *motion;
    DropKind *dropKind;

    emitter->spawnAccum = 0.0f;
    emitter->dropCount = 0;
    slot = emitter->slots;
    for (i = 0; i < DRIP_SLOT_COUNT; i++, slot++) {
        if (slot->drop != NULL) {
            motion = &slot->drop->motion;
            dropKind = &D_800A9D40[motion->dropKind];
            motion->vel.x = 0.0f;
            motion->vel.y = 0.0f;
            motion->vel.z = 0.0f;
            func_1513F680(slot->drop, 13, 18, 0, 13);
            slot->drop->def.lifetime = func_150ADA20() % (u32)(dropKind->lifeRange + 1) + dropKind->lifeMin;
            slot->drop->def.flags |= 1;
        }
    }
    bzero(emitter->slots, DRIP_SLOT_COUNT * sizeof(DripSlot));
}

struct Sub15191520 {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 grav;
    f32 drag;
    u8 pad_14[4];
    f32 damp;
    u8 pad_1C[0x1C];
    f32 floor;
    u8 unk3C[0x19];
    u8 type;
    u8 pad_56[6];
    u8 color;
};

struct Obj15191520 {
    u8 pad_0;
    u8 unk1;
    u8 pad_2[0xA];
    u8 unkC;
    u8 pad_D[0x1F];
    f32 unk2C;
    f32 unk30;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    u8 pad_4C[0x10];
    u8 unk5C;
    u8 pad_5D[0xB3];
    struct Sub15191520 sub;
};

s32 func_1514672C(struct17 *arg0);
s32 func_15046C80(f32 *arg0, s32 arg1, f32 arg2, void *arg3);
void func_151DAB58(u8, f32, u8, struct17 *, s32, u8, u8);

s8 func_15191520(struct Obj15191520 *arg0) {
    f32 dx;
    f32 dz;
    f32 dy;
    struct Sub15191520 *sub;
    s8 ret;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;

    oldY = arg0->y;
    sub = &arg0->sub;
    ret = 1;
    oldVx = sub->vx;
    oldVz = sub->vz;
    sub->vx *= sub->drag * D_800BE9A4;
    sub->vz *= sub->drag * D_800BE9A4;
    dx = (sub->vx + oldVx) * 0.5f * D_800BE9A4;
    dy = sub->vy * D_800BE9A4 + sub->grav * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (sub->vz + oldVz) * 0.5f * D_800BE9A4;
    sub->vy += sub->grav * D_800BE9A4;
    arg0->x += dx;
    arg0->y += dy;
    arg0->z += dz;
    arg0->x2 += dx;
    arg0->y2 += dy;
    arg0->z2 += dz;
    arg0->x2 = arg0->x + (arg0->x2 - arg0->x) * sub->damp;
    arg0->y2 = arg0->y + (arg0->y2 - arg0->y) * sub->damp;
    arg0->z2 = arg0->z + (arg0->z2 - arg0->z) * sub->damp;
    {
    struct17 pos;
    pos.unk0 = arg0->x;
    pos.unk4 = oldY - arg0->unk30;
    pos.unk8 = arg0->z;

    if (func_1514672C(&pos) == 0) {
        return 0;
    }
    if (func_15046C80((f32 *)&pos, 0, arg0->y - arg0->unk30, &sub->floor) != 0) {
        pos.unk4 = sub->floor;
        if (sub->type == 3) {
            func_151DBCBC(sub->color, arg0->unk2C * arg0->unk30 * 0.25f, arg0->unk5C, (s32)&sub->unk3C, &pos, arg0->unkC, arg0->unk1);
        } else if (func_150ADA20() & 1) {
            func_151D9B8C(sub->color, arg0->unk2C * arg0->unk30 * 0.150000006f, arg0->unk5C, (s32)&sub->unk3C, &pos, (func_150ADA20() % 41U) + 0x50, 1, 1, 0, arg0->unkC, arg0->unk1);
        } else {
            func_151DAB58(sub->color, arg0->unk2C * arg0->unk30 * 0.05000000075f, arg0->unk5C, &pos, 1, arg0->unkC, arg0->unk1);
        }
        ret = 0;
    }
    }
    return ret;
}

s32 func_1519187C(void *arg0)
{
  s16 v0;
  s32 v1;
  v0 = *((s16 *) (((u8 *) arg0) + 0x1C));
  if (v0 < (*((s16 *) (((0, (u8 *) arg0)) + 0x124))))
  {
    v1 = v0;
    v1 *= *((s16 *) (((u8 *) arg0) + 0x126));
    if (v1 < (*((u8 *) (((u8 *) arg0) + 0x5C))))
    {
      *((u8 *) (((u8 *) arg0) + 0x5C)) = v1;
    }
  }
  return 1;
}


void func_151918BC(void *arg0) {
    u8 *temp_v1;
    u8 *temp_a0;
    u8 var_v0;

    temp_a0 = (u8 *)arg0 + 0x28;
    temp_v1 = *(u8 **)temp_a0;
    var_v0 = 0;
    if (*(s32 *)temp_v1 == 0) {
        var_v0 = 1;
    } else if (temp_a0[4] != temp_v1[0x3B]) {
        var_v0 = 1;
    }
    if (!var_v0 && (*(s32 *)(temp_v1 + 0x1D4) != 0)) {
        if ((*(u8 *)((u8 *)arg0 + 0xD) & 1) != 0) {
            *(s16 *)(temp_a0 + 6) = *(s16 *)((u8 *)arg0 + 0xE);
            temp_a0[8] |= 1;
        } else {
            *(s16 *)(temp_a0 + 6) = 0x12C;
        }
        var_v0 = 1;
        func_15190770(temp_a0, 0, *(u8 *)((u8 *)arg0 + 0xC), *(u8 *)((u8 *)arg0 + 1));
    }
    if (var_v0) {
        func_1516972C(arg0);
    }
}

void func_15191980(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;

  arg1p = (s32 *) &arg1;
  if (arg2 == 0xD)
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      u8 *base;
      u8 old;
      u8 *flagsPtr;

      old = *((u8 *) (((u8 *) arg0) + 0x18));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0x18);
      base = (u8 *) arg0;
      *flagsPtr = *((u8 *) (((u8 *) arg0) + 0x18)) | 1;
      *((s16 *) (base + 0x16)) = 0x64;
      ;
    }
  }
  else
    if (((arg2 == 0xC) || (arg2 == 0x50)) || (arg2 == 0xE))
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
  {
    func_15169850(arg1, arg2, (s32) arg0 + 0x10, (s32) arg0 + 0x14, (s32) arg0);
  }
}

struct Struct1518F49C {
    s32 unk0;
    u8 unk4;
};

void func_1516972C(struct102 *arg0);
void func_15169850(s32, u8, s32, s32, s32);
void func_15191A84(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;
  struct Struct1518F49C *p;
  u8 *base;
  u8 old;
  u8 *flagsPtr;
  p = (struct Struct1518F49C *) (((u8 *) arg0) + 0x28);
  arg1p = (s32 *) (&arg1);
  if (((arg2 == 0xC) || (arg2 == 0xE)) || (arg2 == 0x50))
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*arg1p) + 4))) == p->unk4))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
    if (arg2 == 0xD)
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*((s32 *) (&arg1))) + 4))) == p->unk4))
    {
      old = *((u8 *) (((u8 *) arg0) + 0xD));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0xD);
      base = (u8 *) arg0;
      *flagsPtr = (*((u8 *) (((u8 *) arg0) + 0xD))) | 1;
      *((s16 *) (base + 0xE)) = 0x64;
      dummy_label_15191A84_1:
      ;

      ;
      ;
    }
  }
  else
  {
    func_15169850(arg1, arg2, (s32) p, (s32) (&p->unk4), (s32) arg0);
  }
}


extern s32 D_800A8010;

void func_15191B8C(s32 arg0, u8 arg1) {
    s32 sp1C;

    sp1C = D_800A8010;
    func_151494E0(arg0, arg1);
    func_15169260(&sp1C, 1, arg0, arg1);
}
