#include <ultra64.h>
#define func_1513D524 func_1513D524_void_proto
#define func_15169850 func_15169850_s32
#include "functions.h"
#undef func_1513D524
#undef func_15169850
#include "variables.h"
#include "effects.h"

void *memcpy(void *, const void *, u32);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15169850(struct ActorRef *ref, u8 msg, struct127 **actor, u8 *actorId, struct102 *self);
void func_151D5D60(Vtx **buffers, s16 viewport, s32 size, Vtx **out, u8 *fresh);
void *func_1513D524(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize,
                    u8 owner, s32 list);
void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
u8 func_151D8E20(void);

typedef struct {
    f32 width, height;
} Size2f;

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
    /* 0x00 */ vertex vel;
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
typedef struct ActorRef {
    struct127 *actor;
    u8 id;
} ActorRef;

/* An emitter's attachment to its actor. */
typedef struct {
    /* 0x0 */ struct127 *actor;
    /* 0x4 */ u8 actorId;           /* actor->unique_id when attached */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ s16 timer;
    /* 0x8 */ u8 flags;             /* bit 0: the timer counts down */
    /* 0x9 */ u8 pad9;
    /* 0xA */ u8 kind;              /* D_8008FA70 index */
} ActorLink; /* size 0xC */

typedef struct DripSlot {
    /* 0x0 */ Particle *drop;
    /* 0x4 */ s32 timer;            /* ticks until the drop falls */
    /* 0x8 */ struct DripSlot *next;
} DripSlot;

#define DRIP_SLOT_COUNT 60

/* Leading fields of a func_151491F4 task. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 flags;            /* bit 0: the timer counts down */
    /* 0x0E */ s16 timer;
} TaskHeader;

/* A func_151491F4 task that keeps the link of an emitter that could not be created yet. */
typedef struct {
    /* 0x00 */ TaskHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ ActorLink link;
} DripRetryTask;

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

void func_15191B8C(ActorRef *ref, u8 msg);

/* Destroys the emitter already on link->actor, then creates one with extraSize bytes of extra data. */
DripEmitter *func_15190770(ActorLink *link, s32 extraSize, u8 owner, s32 list) {
    DripEmitter *emitter;
    ActorRef ref;

    ref.actor = link->actor;
    ref.id = link->actor->unique_id;
    func_15191B8C(&ref, 0xE);
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

/* A point on an actor's bone where a drop hangs. */
typedef struct {
    /* 0x0 */ vertex offset;
    /* 0xC */ u8 bone;
} BoneMount; /* size 0x10 */

/* D_8008FA70: one entry per emitter kind (ActorLink.kind). */
typedef struct {
    /* 0x00 */ u8 dropKind;         /* D_800A9D40 index */
    /* 0x04 */ s32 numMounts;
    /* 0x08 */ BoneMount *mounts;
    /* 0x0C */ f32 rate;            /* new drops per unit of D_800BE9A4 */
    /* 0x10 */ s32 maxDrops;
    /* 0x14 */ s16 holdMin;         /* D_800BE9E4 ticks a drop hangs before it falls */
    /* 0x16 */ s16 holdRange;
} EmitterKind; /* size 0x18 */

extern EmitterKind D_8008FA70[];
extern u8 D_800AB310[];
extern u8 D_800AB404[];
void func_1504715C(Ground *ground, struct127 *actor);
void func_15143134(vertex *offset, vertex *out, Mtx *mtx);
void func_1513F680(Particle *particle, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn);

/*
 * Update of the emitter: keeps its hanging drops on their bones and lets them fall when their time is up,
 * then hangs new drops on random free mounts at the kind's rate. Destroys itself when its actor is gone or
 * its timer runs out; when the actor has no bone matrices it hands its link to a retry task instead.
 */
void func_15190898(DripEmitter *emitter) {
    DripEmitter *self; // matching: stack copy of emitter, read back by the spawn loop test
    volatile u8 done; // matching: done is re-read from its stack slot after every store
    EmitterKind *kind;
    DripSlot *slot;
    DripSlot *head;
    DripSlot *tail;
    s32 freeCount;
    s32 i;
    s32 isFree;
    DropKind *dropKind;
    f32 dx;
    f32 dz;
    s32 skip;
    s32 index;
    Particle *drop;
    ParticleDef def;
    DripMotion motion;
    s16 timer;
    u8 hasTimer;

    self = emitter;
    done = 0;
    if (emitter->link.flags & 1) {
        emitter->link.timer -= D_800BE9E4;
        if (emitter->link.timer < 0) {
            done = 1;
        }
    }
    if (!done && (emitter->link.actor->interaction_state == 0 || emitter->link.actor->unique_id != emitter->link.actorId)) {
        done = 1;
    }
    if (!done) {
        if (emitter->link.actor->unk1D4 != NULL) {
            kind = &D_8008FA70[emitter->link.kind];
            head = NULL;
            tail = NULL;
            freeCount = 0;
            for (i = 0, slot = emitter->slots; i < kind->numMounts; i++, slot++) {
                if (slot->drop != NULL) {
                    slot->drop->def.rot = slot->drop->def.pos;
                    func_15143134(&kind->mounts[i].offset, &slot->drop->def.pos, (Mtx *)emitter->link.actor->unk1D4 + kind->mounts[i].bone);
                    slot->timer -= D_800BE9E4;
                    if (slot->timer < 0) {
                        Particle *falling = slot->drop;
                        DripMotion *fall = &slot->drop->motion;

                        dropKind = &D_800A9D40[fall->dropKind];
                        dx = (falling->def.pos.x - falling->def.rot.x) * D_800BE9A8;
                        dz = (falling->def.pos.z - falling->def.rot.z) * D_800BE9A8;
                        fall->vel.y = 0.0f;
                        fall->vel.x = dx;
                        fall->vel.z = dz;
                        func_1513F680(slot->drop, 13, 18, 0, 13);
                        isFree = 1;
                        slot->drop->def.lifetime = func_150ADA20() % (u32)(dropKind->lifeRange + 1) + dropKind->lifeMin;
                        slot->drop->def.flags |= 1;
                        slot->drop = NULL;
                        emitter->dropCount--;
                    } else {
                        isFree = 0;
                    }
                } else {
                    isFree = 1;
                }
                if (isFree) {
                    freeCount++;
                    if (tail == NULL) {
                        tail = slot;
                    }
                    slot->next = head;
                    head = slot;
                }
            }
            if (freeCount != 0) {
                emitter->spawnAccum += kind->rate * D_800BE9A4;
                if (emitter->spawnAccum > 1.0f) {
                    DropKind *newKind;

                    newKind = &D_800A9D40[kind->dropKind];
                    tail->next = head;
                    motion.vel.x = 0.0f;
                    motion.vel.y = 0.0f;
                    motion.vel.z = 0.0f;
                    motion.drag = 0.2f;
                    motion.fadeStart = 15;
                    motion.fadeRate = 17;
                    motion.trail = 0.0f;
                    motion.stretch = 0.2f;
                    motion.gravity = -1.0f;
                    motion.dropKind = kind->dropKind;
                    func_1504715C(&motion.ground, emitter->link.actor);
                    motion.kind = emitter->kind;
                    def.texture = D_800AB310[emitter->kind];
                    def.renderModes = 0x1001;
                    def.lifetime = 300;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0;
                    def.rgba[1] = 0;
                    def.rgba[2] = 0;
                    def.rgba[3] = 0xFF;
                    def.rot.x = 0.0f;
                    def.rot.y = 0.0f;
                    def.rot.z = 0.0f;
                    def.flags = 0;
                    def.unk45 = 0xFF;
                    def.scale.x = 1.0f;
                    def.scale.y = 1.0f;
                    def.scale.z = 1.0f;
                    motion.phase[0] = 0;
                    motion.phase[1] = 0;
                    do {
                        skip = func_150ADA20() % (u32)freeCount;
                        while (skip--) {
                            tail = head;
                            head = head->next;
                        }
                        index = head - emitter->slots;
                        def.unk44 = func_150ADA20() % 101U + 100;
                        def.width = 0.0f;
                        def.height = 0.0f;
                        motion.baseSize = newKind->baseSize;
                        motion.wobble = newKind->wobble;
                        motion.phaseSpeed[0] = func_150ADA20() % 5U + 4;
                        motion.phaseSpeed[1] = func_150ADA20() % 5U + 4;
                        func_15143134(&kind->mounts[index].offset, &def.pos, (Mtx *)emitter->link.actor->unk1D4 + kind->mounts[index].bone);
                        def.rot = def.pos;
                        if (emitter->link.actor->unk118 < def.pos.y - motion.baseSize.height - motion.wobble.height) {
                            /* the random flip bit is always 0 (% 1), but still advances the RNG */
                            drop = func_1513D524(&def, 12, 0, 0, 13,
                                                 ((func_150ADA20() % 1) ? 1 : 0) | (D_800AB404[emitter->kind] ? 0 : 2),
                                                 sizeof(DripMotion), 0xFF, 0);
                            head->drop = drop;
                            if (drop != NULL) {
                                memcpy(&drop->motion, &motion, sizeof(motion));
                                head->timer = func_150ADA20() % (u32)(kind->holdRange + 1) + kind->holdMin;
                                freeCount--;
                                head = head->next;
                                tail->next = head;
                                emitter->dropCount++;
                            }
                        }
                        emitter->spawnAccum -= 1.0f;
                    } while (emitter->spawnAccum > 1.0f && freeCount != 0 && self->dropCount < kind->maxDrops);
                }
            }
        } else {
            DripRetryTask *task;

            done = 1;
            if (emitter->link.flags & 1) {
                hasTimer = 1;
                timer = emitter->link.timer;
            } else {
                hasTimer = 0;
                timer = 300;
            }
            task = (DripRetryTask *)func_151491F4(timer, -1, 13, hasTimer, 8, sizeof(ActorLink), 0xFF, 0);
            if (task != NULL) {
                memcpy(&task->link, &emitter->link, sizeof(ActorLink));
            }
        }
    }
    if (done) {
        func_1516972C((struct102 *)emitter);
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

/*
 * Vertex function: builds the drop's quad in its vertex buffer for viewport vp, tilted by how fast it
 * moves sideways on screen (D_800DD1E8 / D_800DD1D8 are the viewport's view direction).
 */
Vtx *func_1519108C(Particle *drop, s16 vp) {
    Vtx *vtx;
    Vtx *ret;
    f32 halfWidthCos;
    f32 sinAngle;
    f32 halfWidthSin;
    f32 heightCos;
    f32 heightSin;
    f32 speed;
    f32 sign;
    f32 *viewX;
    f32 *viewZ;
    f32 angle;
    u8 fresh;

    func_151D5D60(drop->vtxBuffers, vp, 0x40, &vtx, &fresh);
    ret = vtx;
    if (vtx) {
        if (fresh) {
            memcpy(drop->vtxBuffers[vp], drop->vtxTemplate, sizeof(drop->vtxTemplate));
            memcpy(drop->vtxBuffers[vp] + 4, drop->vtxTemplate, sizeof(drop->vtxTemplate));
        }
    } else {
        return NULL;
    }

    viewX = &D_800DD1E8[vp];
    viewZ = &D_800DD1D8[vp];
    sign = -1.0f;
    speed = ((*viewX * (drop->def.rot.x - drop->def.pos.x)) - (*viewZ * (drop->def.rot.z - drop->def.pos.z))) * D_800BE9A8;
    if (speed < 0.0f) {
        sign = 1.0f;
        speed = -speed;
    }

    angle = sqrtf(speed) * drop->motion.stretch;
    if (angle > 1.570796371f) {
        angle = 1.570796371f;
    }

    angle = angle * sign;
    sinAngle = sinf(angle);
    angle = cosf(angle);
    halfWidthCos = drop->def.width * angle;
    halfWidthSin = drop->def.width * sinAngle;
    heightCos = drop->def.height * angle;
    heightSin = drop->def.height * sinAngle;

    vtx[0].v.ob[0] = (*viewX * -halfWidthCos) + drop->def.pos.x;
    vtx[0].v.ob[1] = drop->def.pos.y + halfWidthSin;
    vtx[0].v.ob[2] = drop->def.pos.z - (*viewZ * -halfWidthCos);
    vtx[0].v.flag = 0;

    vtx[1].v.ob[0] = (*viewX * halfWidthCos) + drop->def.pos.x;
    vtx[1].v.ob[1] = drop->def.pos.y - halfWidthSin;
    vtx[1].v.ob[2] = drop->def.pos.z - (*viewZ * halfWidthCos);
    vtx[1].v.flag = 0;

    angle = halfWidthCos - heightSin;
    vtx[2].v.ob[0] = (*viewX * angle) + drop->def.pos.x;
    vtx[2].v.ob[1] = (drop->def.pos.y - halfWidthSin) - heightCos;
    vtx[2].v.ob[2] = drop->def.pos.z - (*viewZ * angle);
    vtx[2].v.flag = 0;

    angle = -(halfWidthCos + heightSin);
    vtx[3].v.ob[0] = (*viewX * angle) + drop->def.pos.x;
    vtx[3].v.ob[1] = (drop->def.pos.y + halfWidthSin) - heightCos;
    vtx[3].v.ob[2] = drop->def.pos.z - (*viewZ * angle);
    vtx[3].v.flag = 0;

    return ret;
}

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

s32 func_1514672C(vertex *pos);
s32 func_15046C80(vertex *pos, s32 arg1, f32 minY, Ground *ground);
void func_151DAB58(u8, f32, u8, vertex *, s32, u8, u8);

/* Update of a falling drop: moves it under drag and gravity; on reaching the ground it splashes and dies. */
s8 func_15191520(Particle *drop) {
    f32 dx;
    f32 dz;
    f32 dy;
    DripMotion *motion;
    s8 alive;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;

    oldY = drop->def.pos.y;
    motion = &drop->motion;
    alive = 1;
    oldVx = motion->vel.x;
    oldVz = motion->vel.z;
    motion->vel.x *= motion->drag * D_800BE9A4;
    motion->vel.z *= motion->drag * D_800BE9A4;
    dx = (motion->vel.x + oldVx) * 0.5f * D_800BE9A4;
    dy = motion->vel.y * D_800BE9A4 + motion->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (motion->vel.z + oldVz) * 0.5f * D_800BE9A4;
    motion->vel.y += motion->gravity * D_800BE9A4;
    drop->def.pos.x += dx;
    drop->def.pos.y += dy;
    drop->def.pos.z += dz;
    drop->def.rot.x += dx;
    drop->def.rot.y += dy;
    drop->def.rot.z += dz;
    drop->def.rot.x = drop->def.pos.x + (drop->def.rot.x - drop->def.pos.x) * motion->trail;
    drop->def.rot.y = drop->def.pos.y + (drop->def.rot.y - drop->def.pos.y) * motion->trail;
    drop->def.rot.z = drop->def.pos.z + (drop->def.rot.z - drop->def.pos.z) * motion->trail;
    {
        vertex probe;

        probe.x = drop->def.pos.x;
        probe.y = oldY - drop->def.height;
        probe.z = drop->def.pos.z;
        if (func_1514672C(&probe) == 0) {
            return 0;
        }
        if (func_15046C80(&probe, 0, drop->def.pos.y - drop->def.height, &motion->ground) != 0) {
            probe.y = motion->ground.y;
            if (motion->ground.unk1D == 3) {
                func_151DBCBC(motion->kind, drop->def.width * drop->def.height * 0.25f, drop->def.unk44,
                              (s32)motion->ground.unk4, &probe, drop->owner, drop->list);
            } else if (func_150ADA20() & 1) {
                func_151D9B8C(motion->kind, drop->def.width * drop->def.height * 0.150000006f, drop->def.unk44,
                              (s32)motion->ground.unk4, (struct17 *)&probe, func_150ADA20() % 41U + 80, 1, 1, 0,
                              drop->owner, drop->list);
            } else {
                func_151DAB58(motion->kind, drop->def.width * drop->def.height * 0.05000000075f, drop->def.unk44,
                              &probe, 1, drop->owner, drop->list);
            }
            alive = 0;
        }
    }
    return alive;
}

/* Fades a drop's alpha out over the last fadeStart frames of its lifetime. */
s32 func_1519187C(Particle *drop) {
    s16 lifetime;
    s32 alpha;

    lifetime = drop->def.lifetime;
    if (lifetime < drop->motion.fadeStart) {
        alpha = lifetime * drop->motion.fadeRate;
        if (alpha < drop->def.unk44) {
            drop->def.unk44 = alpha;
        }
    }
    return 1;
}

/* Retries creating the emitter until its actor has bone matrices; gives up when the actor is gone. */
void func_151918BC(DripRetryTask *task) {
    struct127 *actor;
    ActorLink *link;
    u8 done;

    link = &task->link;
    actor = link->actor;
    done = 0;
    if (actor->interaction_state == 0) {
        done = 1;
    } else if (actor->unique_id != link->actorId) {
        done = 1;
    }
    if (!done && actor->unk1D4 != NULL) {
        if (task->header.flags & 1) {
            link->timer = task->header.timer;
            link->flags |= 1;
        } else {
            link->timer = 300;
        }
        done = 1;
        func_15190770(link, 0, task->header.owner, task->header.list);
    }
    if (done) {
        func_1516972C((struct102 *)task);
    }
}

/* Message handler of the emitter: 0xD starts its timer, 0xC / 0xE / 0x50 / 0x4E destroy it. */
/* Message handler of the emitter: 0xD starts its timer, 0xC / 0xE / 0x50 / 0x4E destroy it. */
void func_15191980(DripEmitter *emitter, ActorRef *ref, u8 msg) {
    if (msg == 0xD) {
        // matching: each test is its own if so IDO reloads ref for the second one (|| loads it once)
        if (ref->actor == emitter->link.actor) {
            goto hit2;
        }
        if (ref->id != emitter->link.actorId) {
            goto skip;
        }
    hit2:
        emitter->link.flags |= 1;
        emitter->link.timer = 100;
    skip:;
    } else if (msg == 0xC || msg == 0x50 || msg == 0xE) {
        if (ref->actor == emitter->link.actor) {
            goto hit;
        }
        if (ref->id == emitter->link.actorId) {
        hit:
            func_1516972C((struct102 *)emitter);
        }
    } else if (msg == 0x4E) {
        func_1516972C((struct102 *)emitter);
    } else {
        func_15169850(ref, msg, &emitter->link.actor, &emitter->link.actorId, (struct102 *)emitter);
    }
}

/* Message handler of the retry task, as func_15191980. */
void func_15191A84(DripRetryTask *task, ActorRef *ref, u8 msg) {
    ActorLink *link;

    link = &task->link;
    if (msg == 0xC || msg == 0xE || msg == 0x50) {
        // matching: separate ifs, as in func_15191980
        if (ref->actor == link->actor) {
            goto hit;
        }
        if (ref->id == link->actorId) {
        hit:
            func_1516972C((struct102 *)task);
        }
    } else if (msg == 0x4E) {
        func_1516972C((struct102 *)task);
    } else if (msg == 0xD) {
        if (ref->actor == link->actor) {
            goto hit2;
        }
        if (ref->id != link->actorId) {
            goto skip;
        }
    hit2:
        task->header.flags |= 1;
        task->header.timer = 100;
    skip:;
    } else {
        func_15169850(ref, msg, &link->actor, &link->actorId, (struct102 *)task);
    }
}


/* Sends msg about ref's actor through func_151494E0 and to every object of type 0x20. */
void func_15191B8C(ActorRef *ref, u8 msg) {
    s32 types[1] = { 0x20 };

    func_151494E0((s32)ref, msg);
    func_15169260(types, 1, (s32)ref, msg);
}
