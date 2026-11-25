#include <ultra64.h>
/* functions.h declares func_1513D2F0's second parameter as s32; this file passes a pointer
   (the (s32) cast changes register allocation in func_151BD828). */
#define func_1513D2F0 func_1513D2F0_hdr
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9,
                    u8 argA, s32 argB);

/* Weak reference to an actor: valid while actor->unique_id still equals uniqueId. */
typedef struct {
    struct127 *actor;
    u8 uniqueId;
} ActorRef;

/* Message 0x2D payload: two actors exchange identities. */
typedef struct {
    struct127 *actorA;
    struct127 *actorB;
    u8 idA;
    u8 idB;
} ActorSwapMsg;

/* Payload of the actor messages handled below (0: actor gone, 0x2D: actor swap). */
typedef union {
    ActorRef ref;
    ActorSwapMsg swap;
} ActorMsg;

typedef struct {
    u8 pad0[0x40];
    ActorRef owner;
} OwnedEffect40;

/* Returns whether the effect's owner is still active. */
s32 func_151BC580(OwnedEffect40 *effect) {
    struct127 *owner = effect->owner.actor;

    if (owner->interaction_state == 0) {
        return 0;
    }
    return 1;
}

/* Message handler: removes the effect when its owner goes away, follows the owner through a swap. */
void func_151BC5A4(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &effect->owner;
    struct127 *actor = owner->actor;

    if (msgId == 0) {
        if (actor == msg->ref.actor || owner->uniqueId == msg->ref.uniqueId) {
            func_1516972C((struct102 *)effect);
        }
    } else if (msgId == 0x2D) {
        if (owner->actor == msg->swap.actorA) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
        } else if (owner->actor == msg->swap.actorB) {
            owner->actor = msg->swap.actorA;
            owner->uniqueId = msg->swap.idA;
        }
    }
}

/* One point of a ribbon trail. */
typedef struct {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 texU; /* texture coordinate along the ribbon */
} TrailPoint;

/* Trail state that follows the owner actor. */
typedef struct {
    ActorRef owner;
    struct17 lastPos;      /* 0x08: position of the newest point */
    f32 pending;           /* 0x14: points owed, emitted once it exceeds 1 */
    f32 unk18;
    f32 texU;              /* 0x1C: texture coordinate reached by the owner */
    f32 lastTexU;          /* 0x20: texture coordinate of the newest point */
} TrailData;

/* Ribbon trail effect: a ring buffer of points from head (oldest) to tail (next write). */
typedef struct {
    u8 pad0[0x10];
    struct17 pos;          /* 0x10 */
    u8 pad1C[2];
    u16 flags;             /* 0x1E */
    u8 pad20[5];
    u8 capacity;           /* 0x25 */
    u8 pad26[6];
    s8 count;              /* 0x2C */
    s8 head;               /* 0x2D */
    s8 tail;               /* 0x2E */
    u8 pad2F;
    s8 unk30;
    u8 pad31[0x23];
    struct17 headPos;      /* 0x54 */
    u8 pad60[0x24];
    u8 buffers[0x10];      /* 0x84 */
    TrailPoint *points;    /* 0x94 */
    TrailData *data;       /* 0x98 */
} TrailEffect;

/* Ages the trail: drops expired points and fades the last ones. Returns 0 once a finished trail is empty. */
s32 func_151BC64C(TrailEffect *trail) {
    TrailPoint *points;
    TrailPoint *point;
    s32 i;
    s16 lifetime;
    s32 alpha;

    points = trail->points;
    if (trail->count < 2 && (trail->flags & 8)) {
        return 0;
    }
    i = trail->tail;
    while (i != trail->head) {
        i--;
        if (i < 0) {
            i = trail->capacity - 1;
        }
        point = &points[i];
        point->lifetime -= D_800BE9E4;
        lifetime = point->lifetime;
        if (lifetime < 0) {
            /* this point and every older one expire */
            while (i != trail->head) {
                trail->head++;
                if (trail->head == trail->capacity) {
                    trail->head = 0;
                }
                trail->count--;
            }
        } else if (lifetime < 10) {
            alpha = lifetime * 25;
            if (alpha < point->alpha) {
                point->alpha = alpha;
            }
        }
    }
    if (trail->count > 0) {
        trail->headPos = points[trail->head].pos;
    } else {
        trail->headPos.unk0 = 0.0f;
        trail->headPos.unk4 = 0.0f;
        trail->headPos.unk8 = 0.0f;
    }
    return 1;
}

extern f32 func_15143E64(struct17 *);

/* Follows the owner and emits trail points evenly along the distance it moved. */
s32 func_151BC794(TrailEffect *trail) {
    TrailData *data;
    TrailPoint *points;
    struct127 *owner;
    struct17 delta;
    TrailPoint *point;
    f32 dist;
    f32 inv;
    f32 startU;

    data = trail->data;
    points = trail->points;
    owner = data->owner.actor;
    if (owner->interaction_state == 0 || owner->unique_id != data->owner.uniqueId) {
        trail->flags |= 8;
        return 1;
    }
    trail->pos.unk0 = owner->x_position;
    trail->pos.unk4 = owner->y_position;
    trail->pos.unk8 = owner->z_position;
    delta.unk0 = trail->pos.unk0 - data->lastPos.unk0;
    delta.unk4 = trail->pos.unk4 - data->lastPos.unk4;
    delta.unk8 = trail->pos.unk8 - data->lastPos.unk8;
    dist = func_15143E64(&delta);
    data->pending += dist * 0.04870799929f * D_800BE9A4;
    data->texU += dist * 9.708835602f;
    if (data->pending > 1.0f) {
        struct17 pos;
        f32 u;
        f32 y;
        f32 yStep;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 uSpan;

        inv = 1.0f / data->pending;
        y = data->unk18 + D_800BE9A4;
        pos = data->lastPos;
        startU = data->lastTexU;
        u = startU;
        yStep = -(y * inv);
        dx = delta.unk0 * inv;
        dy = delta.unk4 * inv;
        dz = delta.unk8 * inv;
        uSpan = data->texU - startU;
        do {
            point = &points[trail->tail];
            point->pos = pos;
            point->lifetime = 12;
            point->alpha = 100;
            point->texU = u;
            while (point->texU > 16384.0f) {
                point->texU -= 32768.0f;
            }
            trail->tail++;
            if (trail->tail == trail->capacity) {
                trail->tail = 0;
            }
            trail->count++;
            if (trail->tail == trail->head) {
                /* ring full: drop the oldest point */
                trail->head++;
                if (trail->head == trail->capacity) {
                    trail->head = 0;
                }
                trail->count--;
            }
            u += uSpan * inv;
            pos.unk0 += dx;
            pos.unk4 += dy;
            pos.unk8 += dz;
            y += yStep;
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
        data->lastPos = pos;
        data->lastTexU = u;
        data->unk18 = y;
    }
    return 1;
}

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws the trail as a camera-facing ribbon, two vertices per point. */
Gfx *func_151BCA90(TrailEffect *trail, Gfx *gfx, s16 viewport) {
    Vtx *vtx;
    TrailPoint *points;
    struct17 *camPos;
    u8 dirty;
    struct17 prev;
    struct17 cur;
    f32 viewX;
    f32 viewY;
    f32 viewZ;
    f32 edgeX;
    f32 edgeY;
    f32 edgeZ;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    s32 i;
    s32 j;
    f32 lenSq;
    u8 curAlpha;
    f32 prevTexU;
    f32 curTexU;
    u8 prevAlpha;
    f32 scale;

    if (trail->count >= 2) {
        func_151D5D60(trail->buffers, viewport, (trail->capacity + 5) * 2 * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            points = trail->points;
            camPos = func_15144B34(viewport);
            dirty = 1;
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &dirty, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &dirty);
            gfx = func_1513F4E4(gfx, 0x4E, &dirty);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &dirty);
            if (trail->flags & 2) {
                /* ribbon starts at the live position */
                i = trail->tail - 1;
                if (i < 0) {
                    i = trail->capacity - 1;
                }
                cur = trail->pos;
                curAlpha = 0x80;
                curTexU = 0.0f;
            } else {
                j = trail->tail - 1;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                i = j - 1;
                if (i < 0) {
                    i = trail->capacity - 1;
                }
                cur = points[j].pos;
                curAlpha = points[j].alpha;
                curTexU = points[j].texU;
            }
            prev = points[i].pos;
            prevAlpha = points[i].alpha;
            prevTexU = points[i].texU;

            edgeX = cur.unk0 - prev.unk0;
            edgeY = cur.unk4 - prev.unk4;
            edgeZ = cur.unk8 - prev.unk8;
            viewX = cur.unk0 - camPos->unk0;
            viewY = cur.unk4 - camPos->unk4;
            viewZ = cur.unk8 - camPos->unk8;
            sideX = (edgeY * viewZ) - (viewY * edgeZ);
            sideY = (edgeZ * viewX) - (viewZ * edgeX);
            sideZ = (edgeX * viewY) - (viewX * edgeY);
            lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
            if (lenSq == 0.0f) {
                sideZ = sideY = sideX = 0.0f;
            } else {
                scale = 35.0f / sqrtf(lenSq);
                sideX = sideX * scale;
                sideY = sideY * scale;
                sideZ = sideZ * scale;
            }
            vtx->v.ob[0] = cur.unk0 + sideX;
            vtx->v.ob[1] = cur.unk4 + sideY;
            vtx->v.ob[2] = cur.unk8 + sideZ;
            vtx->v.tc[0] = curTexU;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.unk0 - sideX;
            vtx->v.ob[1] = cur.unk4 - sideY;
            vtx->v.ob[2] = cur.unk8 - sideZ;
            vtx->v.tc[0] = curTexU;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                j = i;
                edgeX = cur.unk0 - prev.unk0;
                edgeY = cur.unk4 - prev.unk4;
                edgeZ = cur.unk8 - prev.unk8;
                viewX = prev.unk0 - camPos->unk0;
                viewY = prev.unk4 - camPos->unk4;
                viewZ = prev.unk8 - camPos->unk8;
                sideX = (edgeY * viewZ) - (viewY * edgeZ);
                sideY = (edgeZ * viewX) - (viewZ * edgeX);
                sideZ = (edgeX * viewY) - (viewX * edgeY);
                lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                if (lenSq == 0.0f) {
                    sideZ = sideY = sideX = 0.0f;
                } else {
                    scale = 35.0f / sqrtf(lenSq);
                    sideX = sideX * scale;
                    sideY = sideY * scale;
                    sideZ = sideZ * scale;
                }
                vtx->v.ob[0] = prev.unk0 + sideX;
                vtx->v.ob[1] = prev.unk4 + sideY;
                vtx->v.ob[2] = prev.unk8 + sideZ;
                vtx->v.tc[0] = prevTexU;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = prevAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = prev.unk0 - sideX;
                vtx->v.ob[1] = prev.unk4 - sideY;
                vtx->v.ob[2] = prev.unk8 - sideZ;
                vtx->v.tc[0] = prevTexU;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = prevAlpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i--;
                if (i < 0) {
                    i = trail->capacity - 1;
                }
                if (j != trail->head) {
                    cur = prev;
                    prev = points[i].pos;
                    prevAlpha = points[i].alpha;
                    prevTexU = points[i].texU;
                }
            } while (j != trail->head);
        }
    }
    return gfx;
}

/* Message handler: finishes the trail when its owner goes away, follows the owner through a swap. */
/* Message handler: finishes the trail when its owner goes away, follows the owner through a swap. */
void func_151BD21C(TrailEffect *trail, ActorMsg *msg, u8 msgId) {
    ActorRef *owner;
    struct127 *actor;
    struct127 *actorA;

    owner = &trail->data->owner;
    actor = owner->actor;
    if (msgId == 0) {
        if (actor == msg->ref.actor || msg->ref.uniqueId == owner->uniqueId) {
            u16 *flags = &trail->flags; // matching: keeps the flags store out of the jr delay slot

            trail->unk30 = 0;
            *flags |= 8;
        }
    } else if (msgId == 0x2D) {
        actorA = msg->swap.actorA;
        if (actorA == owner->actor) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
            return;
        }
        if (owner->actor == msg->swap.actorB) {
            owner->actor = actorA;
            owner->uniqueId = msg->swap.idA;
        }
    }
}

/* Common head of effect objects; each effect's own data starts at 0x28. */
typedef struct {
    u8 pad0;
    u8 kind;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 state;             /* 0x0E: -1 frees the effect */
    u8 pad10[0x18];
} EffectHeader;

typedef struct {
    EffectHeader header;
    ActorRef owner;
} OwnedEffect28;

/* Returns whether the effect's owner is still the same active actor. */
s32 func_151BD2BC(OwnedEffect28 *effect) {
    struct127 *owner = effect->owner.actor;

    if (owner->interaction_state == 0) {
        return 0;
    }
    if (owner->unique_id != effect->owner.uniqueId) {
        return 0;
    }
    return 1;
}

/* Spawn parameters of a BloodStrip (same layout as game_161520.c's BloodStripDesc). */
typedef struct {
    u8 ownerId;
    u8 pad1[3];
    struct127 *owner;
    u8 bone;
    u8 pad9[3];
    struct17 offsetA;
    struct17 offsetB;
    u8 flags;
    u8 pad25;
    s16 duration;
    s16 pause;
    s16 lifetime;
    u16 sound;
    s8 sampleCallback;
    s8 emitCallback;
    s8 rateCallback;
    u8 pad31[3];
    f32 rate;
    u8 mode;
    s8 updateCallback;
    u8 pad3A[2];
} BloodStripDesc;

/* Blood swept between two points on an actor's bone (game_161520.c's BloodStrip). */
typedef struct {
    u8 pad0;
    u8 kind;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    s32 active;
    s32 unk14;
    u8 ownerId;
    u8 pad19[3];
    struct127 *owner;
    u8 bone;
    u8 pad21[3];
    struct17 offsetA;
    struct17 offsetB;
    u8 flags;
    u8 pad3D;
    s16 duration;
    s16 pause;
    s16 lifetime;
    u16 sound;
    s8 sampleCallback;
    s8 emitCallback;
    s8 rateCallback;
    u8 pad49[3];
    f32 rate;
    u8 mode;
    s8 updateCallback;
    u8 pad52[2];
    s16 timer;
    u8 pad56[2];
    struct17 posA;
    struct17 posB;
    f32 accumulator;
    f32 power;
    f32 time;
} BloodStrip;

/* BloodStrip spawned by func_151BD2F8, with two bytes of extra data. */
typedef struct {
    BloodStrip strip;
    u8 pad7C[4];
    s16 sampleAge; /* 0x80: frames since the last sample */
} AgedBloodStrip;

AgedBloodStrip *func_15134DAC(BloodStripDesc *desc, s32 extraSize);

/* Spawns two mirrored blood strips on bone 1 of actor. */
void func_151BD2F8(struct127 *actor, s32 arg1, s32 arg2) {
    BloodStripDesc desc;
    struct {
        s16 value;
        u8 pad[2]; // matching: IDO gives the initial value a word-aligned slot
    } sampleAge;
    AgedBloodStrip *strip;

    if (actor == NULL) {
        return;
    }

    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offsetA.unk0 = 31.0f;
    desc.offsetA.unk4 = -34.0f;
    desc.offsetA.unk8 = -117.0f;
    desc.offsetB.unk0 = 35.0f;
    desc.offsetB.unk4 = -24.0f;
    desc.offsetB.unk8 = -182.0f;
    desc.flags = 0;
    desc.duration = 50;
    desc.pause = 100;
    desc.lifetime = 300;
    desc.sampleCallback = 4; /* func_151BD42C */
    desc.emitCallback = 4;   /* func_151BD43C */
    desc.rateCallback = 3;   /* func_151BD750 */
    desc.rate = 0.3006100059f;
    desc.mode = 5;
    desc.updateCallback = -1;

    sampleAge.value = 0;

    strip = func_15134DAC(&desc, sizeof(sampleAge.value));
    if (strip != NULL) {
        memcpy(&strip->sampleAge, &sampleAge.value, sizeof(sampleAge.value));
    }

    desc.offsetA.unk0 = -desc.offsetA.unk0;
    desc.offsetB.unk0 = -desc.offsetB.unk0;

    strip = func_15134DAC(&desc, sizeof(sampleAge.value));
    if (strip != NULL) {
        memcpy(&strip->sampleAge, &sampleAge.value, sizeof(sampleAge.value));
    }
}

/* BloodStrip sample callback: restarts the sample age. */
s32 func_151BD42C(AgedBloodStrip *strip) {
    strip->sampleAge = 0;
    return 1;
}

/* ParticleDesc.flags (see game_15D730.c) */
#define PARTICLE_LIFETIME   0x1
#define PARTICLE_GRAVITY    0x2
#define PARTICLE_MOVE       0x4
#define PARTICLE_FLIP_S     0x40
#define PARTICLE_FLIP_T     0x80
#define PARTICLE_FADE       0x200
#define PARTICLE_SCALE      0x400
#define PARTICLE_SCALE_MUL  0x800
#define PARTICLE_USE_OFFSET 0x1000
#define PARTICLE_HANDLERS   0x4000
#define PARTICLE_DRAW_FUNC  0x8000

/* Spawn parameters of a particle: the first 0x66 bytes of game_15D730.c's ParticleDesc
   (the full 0x70-byte struct moves this file's stack slots). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode;
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 offset;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ s32 flags;
    /* 0x5C */ void *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;
    /* 0x63 */ s8 deathFunc;
    /* 0x64 */ s8 drawFunc;
    /* 0x65 */ u8 handlerSet;
} ParticleDesc;

/* Particle spawned by func_151BD43C, with its 0x10 bytes of per-particle data. */
typedef struct {
    u8 pad0[0x4C];
    struct17 offset;   /* 0x4C: ParticleDesc.offset, the point the particle moves with */
    u8 pad58[0x50];
    f32 drag;          /* 0xA8: read by func_1513170C */
    u8 padAC[4];
    ActorRef owner;    /* 0xB0 */
} BloodDroplet;

s32 func_1513170C(BloodDroplet *particle, struct17 *prevPos);

BloodDroplet *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);

/* BloodStrip emit callback: spawns one blood droplet at pointA moving towards pointB. */
void func_151BD43C(struct17 *pointA, struct17 *pointB, struct17 *velA, struct17 *velB, f32 time,
                   BloodStrip *strip) {
    BloodDroplet *particle;
    struct127 *owner;
    ParticleDesc desc;
    f32 drag;
    s32 pad;  // matching: stack slot of the original frame
    ActorRef ownerRef;
    s32 pad2; // matching: stack slot of the original frame

    owner = strip->owner;
    if ((owner->unk74 & 0xF) != 0xF) {
        desc.texture = 0x29;
        desc.renderMode = 0xE03;
        desc.unk0 = 0x200005;
        desc.unk4 = 0;
        desc.frame = 0;
        desc.frameRate = 0;
        desc.gravity = ((func_150ADA68() * 251.0f) + 44.0f) * 0.001000000047f;
        desc.fadeStart = 20;
        desc.fadeRate = 12;
        desc.flags = (func_150ADA20() & 1 ? PARTICLE_FLIP_S : 0) |
                     (PARTICLE_LIFETIME | PARTICLE_GRAVITY | PARTICLE_MOVE | (func_150ADA20() & 1 ? PARTICLE_FLIP_T : 0)) |
                     PARTICLE_FADE | PARTICLE_SCALE | PARTICLE_SCALE_MUL | PARTICLE_USE_OFFSET | PARTICLE_HANDLERS |
                     PARTICLE_DRAW_FUNC;
        desc.primColorMode = 3;
        desc.envColorMode = 3;
        desc.deathFunc = -1;
        desc.drawFunc = -1;
        desc.handlerSet = 7;
        desc.scaleStart = 34;
        desc.r = 221;
        desc.g = 211;
        desc.b = 205;
        desc.a = 255;
        desc.r2 = 87;
        desc.g2 = 85;
        desc.b2 = 90;
        drag = 0.9347190261f;
        desc.scaleRate = 1.034358025f;
        desc.alpha = (func_150ADA20() % 156U) + 100;
        desc.brightness = 255;
        desc.timer = (func_150ADA20() % 18U) + 25;
        desc.width = desc.height = (func_150ADA68() * 53.0f) + 49.0f;
        desc.vel.unk0 = (pointB->unk0 - pointA->unk0) * strip->power;
        desc.vel.unk4 = (pointB->unk4 - pointA->unk4) * strip->power;
        desc.vel.unk8 = (pointB->unk8 - pointA->unk8) * strip->power;
        desc.pos.unk0 = pointA->unk0 - strip->owner->x_position;
        desc.pos.unk4 = pointA->unk4 - strip->owner->y_position;
        desc.pos.unk8 = pointA->unk8 - strip->owner->z_position;
        desc.pos.unk0 += desc.vel.unk0 * time;
        desc.pos.unk4 += desc.vel.unk4 * time;
        desc.pos.unk8 += desc.vel.unk8 * time;
        ownerRef.actor = strip->owner;
        ownerRef.uniqueId = strip->owner->unique_id;
        desc.updateFunc = 0x13;
        desc.offset.unk0 = strip->owner->x_position;
        desc.offset.unk4 = strip->owner->y_position;
        desc.offset.unk8 = strip->owner->z_position;
        particle = func_15130374(&desc, 1, 0x10, strip->unkC, strip->kind);
        if (particle != NULL) {
            memcpy(&particle->drag, &drag, sizeof(drag));
            memcpy(&particle->owner, &ownerRef, sizeof(ownerRef));
        }
    }
}

extern f32 D_800AA860; /* 0.001f */
extern f32 D_800AA864; /* 0.04f */

/* BloodStrip rate callback: the rate grows with the time since the last sample. */
f32 func_151BD750(AgedBloodStrip *strip) {
    s16 age;
    f32 rate;

    age = strip->sampleAge;
    rate = (1.0f + 1.0f) * (f32)age * D_800AA860 + D_800AA864; // matching: literals change FP register allocation
    strip->sampleAge = age + D_800BE9E4;
    return rate;
}

/* Particle update: keeps the droplet moving with its owner, then runs the standard update. */
void func_151BD79C(BloodDroplet *particle, struct17 *prevPos) {
    struct127 *owner = particle->owner.actor;

    if (owner->interaction_state != 0 && owner->id != 0xFF) {
        particle->offset.unk0 = owner->x_position;
        particle->offset.unk4 = owner->y_position;
        particle->offset.unk8 = owner->z_position;
    }
    func_1513170C(particle, prevPos);
}

/* Broadcasts message 0x3B for actor. */
void func_151BD7F4(struct127 *actor) {
    ActorRef ref;

    ref.actor = actor;
    ref.uniqueId = actor->unique_id;
    func_151494E0((s32)&ref, 0x3B);
}

typedef struct BurstSpark BurstSpark;
typedef struct BurstGlow BurstGlow;

/* Data of the burst effect object (at 0x28). */
typedef struct {
    ActorRef owner;
    BurstSpark *sparks[2];
    BurstGlow *glows[2];
} BurstData;

typedef struct {
    EffectHeader header;
    BurstData data;
} BurstEffect;

/* Per-object data func_151BD828 copies into each spark (at spark+0x110). */
typedef struct {
    struct127 *owner;
    u8 ownerId;
    u8 index;
    u8 pad06[2];
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    BurstEffect *burst;
    f32 field54;
    u8 flags;              /* bit 0: owner visible */
    u8 pad59[3];
} BurstSparkData;

/* Spawn parameters func_151BD828 passes to func_1513D2F0 and func_151407D0. */
typedef struct {
    u8 kind;
    u8 mode;
    s16 flags;
    s16 lifetime;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 color[4];
    f32 field14;
    f32 field18;
    struct17 field1C;
    struct17 field28;
    f32 field34;
    f32 field38;
    f32 field3C;
    s32 field40;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    s32 field48;
    u8 field4C;
    u8 pad4D[0xB];
} BurstObjectDesc;

/* Shape parameters of a glow (copied to glow+0x110 by func_151407D0). */
typedef struct {
    f32 field00;
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3C;
    s32 field40;
    s32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    u8 field54;
    u8 field55;
    u8 field56;
    u8 field57;
    u8 field58;
    s8 field59;
    u8 pad5A[2];
} BurstGlowShape;

/* Per-object data func_151BD828 copies into each glow (at glow+0x170). */
typedef struct {
    s16 field00;
    u8 pad02[2];
    f32 field04;
    BurstEffect *burst;
    u8 index;
    u8 pad0D[3];
    struct127 *owner;
    u8 ownerId;
    u8 pad15[3];
} BurstGlowData;

struct BurstSpark {
    u8 pad0[0x34];
    struct17 start;        /* 0x34 */
    struct17 end;          /* 0x40 */
    u8 pad4C[0x110 - 0x4C];
    BurstSparkData data;   /* 0x110 */
};

struct BurstGlow {
    u8 pad0[0x34];
    struct17 start;        /* 0x34 */
    struct17 end;          /* 0x40 */
    u8 pad4C[0xC];
    s32 flags;             /* 0x58: bit 1 visible */
    u8 pad5C[0x170 - 0x5C];
    BurstGlowData data;    /* 0x170 */
};

BurstGlow *func_151407D0(BurstGlowShape *shape, u32 shapeSize, BurstObjectDesc *desc, u8, u8, u8, u8, s8, u8, s32);

/* Spawns the burst effect on actor: two sparks and two glows that the effect keeps on the actor. */
void func_151BD828(struct127 *actor, u8 arg1, s32 arg2) {
    BurstEffect *effect;
    BurstData burst;
    BurstData *data;

    burst.owner.actor = actor;
    burst.owner.uniqueId = actor->unique_id;
    burst.sparks[0] = NULL;
    burst.sparks[1] = NULL;
    burst.glows[0] = NULL;
    burst.glows[1] = NULL;
    effect = (BurstEffect *)func_15149130(300, -1, 0x40, -1, 0, 0x33, (struct37 *)sizeof(BurstData), arg1, arg2);
    if (effect != NULL) {
        data = &effect->data;
        memcpy(data, &burst, sizeof(burst));
        {
            BurstSparkData spark;
            BurstObjectDesc sparkEffect;

            spark.flags = 0;
            spark.field54 = (actor->xz_scale + actor->y_scale) * 0.5f;
            spark.owner = actor;
            spark.ownerId = actor->unique_id;
            spark.field08 = 15.045272f;
            spark.field10 = 5.8676558f;
            spark.field18 = 12.111444f;
            spark.field0C = 0.396174f;
            spark.field24 = 0.0f;
            spark.field28 = 0.0f;
            spark.field38 = 0.0f;
            sparkEffect.field14 = 0.0f;
            sparkEffect.field18 = 0.0f;
            spark.field3C = 0.0f;
            spark.field48 = 1.0f;
            spark.field40 = 1.0f;
            spark.field4C = 1.0f;
            spark.field44 = 1.0f;
            spark.burst = effect;
            sparkEffect.kind = 0x5F;
            sparkEffect.mode = 8;
            sparkEffect.flags = 0x2203;
            sparkEffect.lifetime = 300;
            sparkEffect.field08 = 0;
            sparkEffect.field0C = 0;
            sparkEffect.color[0] = 0xFF;
            sparkEffect.color[1] = 0xFF;
            sparkEffect.color[2] = 0xFF;
            sparkEffect.color[3] = 0xFF;
            spark.field14 = 0.1042563f;
            spark.field20 = 0.5197371f;
            spark.field1C = 0.34404588f;
            spark.field2C = 160.0f;
            spark.field30 = 95.0f;
            spark.field34 = 207.5f;
            sparkEffect.field1C = *(struct17 *)&D_800A5480;
            sparkEffect.field28 = *(struct17 *)&D_800A5480;
            sparkEffect.field34 = 1.0f;
            sparkEffect.field38 = 1.0f;
            sparkEffect.field3C = 1.0f;
            sparkEffect.field40 = 0x40EC0000;
            sparkEffect.field44 = 0;
            sparkEffect.field45 = 0xFF;
            sparkEffect.field46 = 0;
            sparkEffect.field47 = 7;
            sparkEffect.field48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                data->sparks[spark.index] = func_1513D2F0(&sparkEffect, &D_800A4AA0, 0, 0x25, 0, 0x1D,
                    ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, 0x5C, arg1, arg2);
                if (data->sparks[spark.index] != NULL) {
                    memcpy(&data->sparks[spark.index]->data, &spark, sizeof(spark));
                }
            }
        }
        {
            BurstGlowShape shape;
            BurstObjectDesc glowEffect;
            BurstGlowData glow;

            glow.field00 = 0;
            glow.field04 = 1.0f;
            glow.owner = actor;
            glow.burst = effect;
            glow.ownerId = actor->unique_id;
            shape.field00 = 50.3f;
            shape.field04 = 50.3f;
            shape.field08 = 30.9f;
            shape.field0C = 30.9f;
            shape.field10 = 0.1f;
            shape.field14 = 0.1f;
            shape.field1C = 0.7f;
            shape.field20 = 0.000001f;
            shape.field24 = -1;
            shape.field34 = -1;
            shape.field28 = -1;
            shape.field38 = -1;
            shape.field2C = -1;
            shape.field3C = -1;
            shape.field30 = -1;
            shape.field40 = -1;
            shape.field18 = 1.0f;
            shape.field44 = 0;
            shape.field48 = 1.0f;
            shape.field4C = 0;
            shape.field4E = 0;
            shape.field50 = 0;
            shape.field52 = 0;
            shape.field54 = 0;
            shape.field55 = 0;
            shape.field56 = 0;
            shape.field57 = 0;
            shape.field58 = 0xF;
            shape.field59 = -1;
            glowEffect.kind = 0x60;
            glowEffect.mode = 3;
            glowEffect.flags = 0x2203;
            glowEffect.lifetime = 300;
            glowEffect.field08 = 0;
            glowEffect.field0C = 0;
            glowEffect.color[0] = 0xFF;
            glowEffect.color[1] = 0xFF;
            glowEffect.color[2] = 0xFF;
            glowEffect.color[3] = 0xFF;
            glowEffect.field14 = 100.0f;
            glowEffect.field18 = 100.0f;
            glowEffect.field1C = *(struct17 *)&D_800A5480;
            glowEffect.field28 = *(struct17 *)&D_800A5480;
            glowEffect.field34 = 1.0f;
            glowEffect.field38 = 1.0f;
            glowEffect.field3C = 1.0f;
            glowEffect.field40 = 0xCD2002;
            glowEffect.field44 = 0xFF;
            glowEffect.field45 = 0xFF;
            glowEffect.field46 = 0;
            glowEffect.field47 = 7;
            glowEffect.field48 = 0;
            glowEffect.field4C = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                data->glows[glow.index] = func_151407D0(&shape, 0x78, &glowEffect, 0, 0x2A, 0, 0, -1, arg1, arg2);
                if (data->glows[glow.index] != NULL) {
                    memcpy(&data->glows[glow.index]->data, &glow, sizeof(glow));
                }
            }
        }
    }
}

extern struct17 D_800AA778;
extern struct17 D_800AA784;
extern struct17 D_800AA790;
extern struct17 D_800AA79C;
extern s32 func_15145EA4(struct17 **src, struct17 **dst, Mtx *matrix, s32 count);

/* Per-frame update of the burst: keeps the sparks and glows on bone 1 of the owner. */
void func_151BDD8C(BurstEffect *effect) {
    BurstData *data;
    struct127 *owner;
    struct17 starts[2];
    struct17 ends[2];
    u8 i;
    u8 j;
    u8 k;
    struct17 *srcs[4];
    struct17 *dsts[4];
    s32 player;

    data = &effect->data;
    owner = data->owner.actor;
    if (owner->interaction_state == 0 || owner->unique_id != data->owner.uniqueId) {
        effect->header.state = -1;
        return;
    }
    if (owner->unk1D4 != NULL && (owner->unk74 & 0xF) != 0xF) {
        BurstSpark *spark;
        BurstGlow *glow;
        s32 pad1; // matching: stack slots of the original frame
        s32 pad2;

        srcs[0] = &D_800AA778;
        srcs[1] = &D_800AA784;
        srcs[2] = &D_800AA790;
        srcs[3] = &D_800AA79C;
        dsts[0] = &starts[0];
        dsts[1] = &starts[1];
        dsts[2] = &ends[0];
        dsts[3] = &ends[1];
        func_15145EA4(srcs, dsts, (Mtx *)owner->unk1D4 + 1, 4);
        for (i = 0; i < 2; i++) {
            if (data->sparks[i] != NULL) {
                spark = data->sparks[i];
                spark->data.flags |= 1;
                spark->start = starts[i];
                spark->end = ends[i];
            }
            if (data->glows[i] != NULL) {
                glow = data->glows[i];
                glow->flags |= 2;
                glow->flags &= ~4;
                glow->start = starts[i];
                glow->end = ends[i];
            }
        }
    } else {
        for (j = 0; j < 2; j++) {
            if (data->sparks[j] != NULL) {
                BurstSparkData *sparkData = &data->sparks[j]->data;

                sparkData->flags &= ~1;
            }
            if (data->glows[j] != NULL) {
                data->glows[j]->flags &= ~2;
            }
        }
    }
    player = owner - D_800CC2D0;
    if (player < 4) {
        for (k = 0; k < 2; k++) {
            if (data->sparks[k] != NULL) {
                BurstSparkData *sparkData = &data->sparks[k]->data;

                if (*((u16 **)D_800BE728[0])[(u8)player] & 0x8000) {
                    sparkData->field48 = 6.0f;
                    sparkData->field4C = 21.0f;
                } else {
                    sparkData->field48 = 3.0f;
                    sparkData->field4C = 8.0f;
                }
            }
            if (data->glows[k] != NULL) {
                BurstGlowData *glowData = &data->glows[k]->data;

                if (*((u16 **)D_800BE728[0])[(u8)player] & 0x8000) {
                    glowData->field04 = 1.5f;
                } else {
                    glowData->field04 = 1.0f;
                }
            }
        }
    }
}

/* Message handler of the burst: message 0x3B (func_151BD7F4) for its owner frees it; the rest go to func_15169850. */
/* Message handler of the burst: message 0x3B (func_151BD7F4) for its owner frees it; the rest go to func_15169850. */
void func_151BE0AC(s32 effect, s32 msg, u8 msgId) {
    // matching: the original reads its parameters through this pointer; typed parameters
    // with the same body let IDO merge the two loads of msg
    s32 *args = &effect;
    s32 owner;

    if (msgId == 0x3B) {
        owner = args[0] + 0x28; /* &((BurstEffect *)effect)->data.owner */
        if (((ActorRef *)msg)->actor == ((ActorRef *)owner)->actor ||
            ((ActorRef *)owner)->uniqueId == ((ActorRef *)args[1])->uniqueId) {
            func_1516972C((struct102 *)args[0]);
        }
    } else {
        owner = args[0] + 0x28;
        func_15169850(msg, msgId, owner, owner + 4, args[0]);
    }
}

/* Frees the burst's sparks and glows. */
void func_151BE138(BurstEffect *effect) {
    BurstData *data = &effect->data;

    if (data->sparks[0] != NULL) {
        func_1516972C((struct102 *)data->sparks[0]);
    }
    if (data->sparks[1] != NULL) {
        func_1516972C((struct102 *)data->sparks[1]);
    }
    if (data->glows[0] != NULL) {
        func_1516972C((struct102 *)data->glows[0]);
    }
    if (data->glows[1] != NULL) {
        func_1516972C((struct102 *)data->glows[1]);
    }
}

void func_151BE1B8(BurstEffect *effect) {
    func_151BE138(effect);
    func_1514933C((struct260 *)effect);
}

void func_151BE1E4(BurstEffect *effect) {
    func_151BE138(effect);
    func_15149368((struct260 *)effect);
}
