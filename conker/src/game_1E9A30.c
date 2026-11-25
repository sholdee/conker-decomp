#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

typedef struct {
    u8 pad0[0x40];
    ActorRef owner;
} OwnedEffect40;

/* Returns whether the effect's owner is still active. */
s32 func_151BC580(OwnedEffect40 *effect) {
    Actor *owner = effect->owner.actor;

    if (owner->interaction_state == 0) {
        return 0;
    }
    return 1;
}

/* Message handler: message 0 for its owner frees the effect; 0x2D makes it follow the owner through a swap. */
void func_151BC5A4(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &effect->owner;
    Actor *actor = owner->actor;

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
    Vec3f pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 texU; /* texture coordinate along the ribbon */
} TrailPoint;

/* Trail state that follows the owner actor. */
typedef struct {
    ActorRef owner;
    Vec3f lastPos;      /* 0x08: position of the newest point */
    f32 pending;           /* 0x14: points owed, emitted once it exceeds 1 */
    f32 unk18;
    f32 texU;              /* 0x1C: texture coordinate reached by the owner */
    f32 lastTexU;          /* 0x20: texture coordinate of the newest point */
} TrailData;

/* Ribbon trail effect: a ring buffer of points from head (oldest) to tail (next write). */
typedef struct {
    u8 pad0[0x10];
    Vec3f pos;          /* 0x10 */
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
    Vec3f headPos;      /* 0x54 */
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
        trail->headPos.x = 0.0f;
        trail->headPos.y = 0.0f;
        trail->headPos.z = 0.0f;
    }
    return 1;
}

extern f32 func_15143E64(Vec3f *);

/* Follows the owner and emits trail points evenly along the distance it moved. */
s32 func_151BC794(TrailEffect *trail) {
    TrailData *data;
    TrailPoint *points;
    Actor *owner;
    Vec3f delta;
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
    trail->pos.x = owner->x_position;
    trail->pos.y = owner->y_position;
    trail->pos.z = owner->z_position;
    delta.x = trail->pos.x - data->lastPos.x;
    delta.y = trail->pos.y - data->lastPos.y;
    delta.z = trail->pos.z - data->lastPos.z;
    dist = func_15143E64(&delta);
    data->pending += dist * 0.04870799929f * D_800BE9A4;
    data->texU += dist * 9.708835602f;
    if (data->pending > 1.0f) {
        Vec3f pos;
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
        dx = delta.x * inv;
        dy = delta.y * inv;
        dz = delta.z * inv;
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
            pos.x += dx;
            pos.y += dy;
            pos.z += dz;
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
extern Vec3f *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws the trail as a camera-facing ribbon, two vertices per point. */
Gfx *func_151BCA90(TrailEffect *trail, Gfx *gfx, s16 viewport) {
    Vtx *vtx;
    TrailPoint *points;
    Vec3f *camPos;
    u8 dirty;
    Vec3f prev;
    Vec3f cur;
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

            edgeX = cur.x - prev.x;
            edgeY = cur.y - prev.y;
            edgeZ = cur.z - prev.z;
            viewX = cur.x - camPos->x;
            viewY = cur.y - camPos->y;
            viewZ = cur.z - camPos->z;
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
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = curTexU;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
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
                edgeX = cur.x - prev.x;
                edgeY = cur.y - prev.y;
                edgeZ = cur.z - prev.z;
                viewX = prev.x - camPos->x;
                viewY = prev.y - camPos->y;
                viewZ = prev.z - camPos->z;
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
                vtx->v.ob[0] = prev.x + sideX;
                vtx->v.ob[1] = prev.y + sideY;
                vtx->v.ob[2] = prev.z + sideZ;
                vtx->v.tc[0] = prevTexU;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = prevAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = prev.x - sideX;
                vtx->v.ob[1] = prev.y - sideY;
                vtx->v.ob[2] = prev.z - sideZ;
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

/* Message handler: message 0 for its owner finishes the trail; 0x2D makes it follow the owner through a swap. */
/* Message handler: message 0 for its owner finishes the trail; 0x2D makes it follow the owner through a swap. */
void func_151BD21C(TrailEffect *trail, ActorMsg *msg, u8 msgId) {
    ActorRef *owner;
    Actor *actor;
    Actor *actorA;

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

typedef struct {
    EffectHeader header;
    u8 pad10[0x18];
    ActorRef owner;
} OwnedEffect28;

/* Returns whether the effect's owner is still the same active actor. */
s32 func_151BD2BC(OwnedEffect28 *effect) {
    Actor *owner = effect->owner.actor;

    if (owner->interaction_state == 0) {
        return 0;
    }
    if (owner->unique_id != effect->owner.uniqueId) {
        return 0;
    }
    return 1;
}

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
    Actor *owner;
    u8 bone;
    u8 pad21[3];
    Vec3f offsetA;
    Vec3f offsetB;
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
    Vec3f posA;
    Vec3f posB;
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
void func_151BD2F8(Actor *actor, s32 arg1, s32 arg2) {
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
    desc.offsetA.x = 31.0f;
    desc.offsetA.y = -34.0f;
    desc.offsetA.z = -117.0f;
    desc.offsetB.x = 35.0f;
    desc.offsetB.y = -24.0f;
    desc.offsetB.z = -182.0f;
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

    desc.offsetA.x = -desc.offsetA.x;
    desc.offsetB.x = -desc.offsetB.x;

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

/* Particle spawned by func_151BD43C, with its 0x10 bytes of per-particle data. */
typedef struct {
    u8 pad0[0x4C];
    Vec3f offset;   /* 0x4C: ParticleDesc68.offset, the point the particle moves with */
    u8 pad58[0x50];
    f32 drag;          /* 0xA8: read by func_1513170C */
    u8 padAC[4];
    ActorRef owner;    /* 0xB0 */
} BloodDroplet;

s32 func_1513170C(BloodDroplet *particle, Vec3f *prevPos);

BloodDroplet *func_15130374(ParticleDesc68 *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);

/* BloodStrip emit callback: spawns one blood droplet at pointA moving towards pointB. */
void func_151BD43C(Vec3f *pointA, Vec3f *pointB, Vec3f *velA, Vec3f *velB, f32 time,
                   BloodStrip *strip) {
    BloodDroplet *particle;
    Actor *owner;
    ParticleDesc68 desc;
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
        desc.vel.x = (pointB->x - pointA->x) * strip->power;
        desc.vel.y = (pointB->y - pointA->y) * strip->power;
        desc.vel.z = (pointB->z - pointA->z) * strip->power;
        desc.pos.x = pointA->x - strip->owner->x_position;
        desc.pos.y = pointA->y - strip->owner->y_position;
        desc.pos.z = pointA->z - strip->owner->z_position;
        desc.pos.x += desc.vel.x * time;
        desc.pos.y += desc.vel.y * time;
        desc.pos.z += desc.vel.z * time;
        ownerRef.actor = strip->owner;
        ownerRef.uniqueId = strip->owner->unique_id;
        desc.updateFunc = 0x13;
        desc.offset.x = strip->owner->x_position;
        desc.offset.y = strip->owner->y_position;
        desc.offset.z = strip->owner->z_position;
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
void func_151BD79C(BloodDroplet *particle, Vec3f *prevPos) {
    Actor *owner = particle->owner.actor;

    if (owner->interaction_state != 0 && owner->id != 0xFF) {
        particle->offset.x = owner->x_position;
        particle->offset.y = owner->y_position;
        particle->offset.z = owner->z_position;
    }
    func_1513170C(particle, prevPos);
}

/* Broadcasts message 0x3B for actor. */
void func_151BD7F4(Actor *actor) {
    ActorRef ref;

    ref.actor = actor;
    ref.uniqueId = actor->unique_id;
    func_151494E0((s32)&ref, 0x3B);
}

BurstGlow *func_151407D0(GlowShape *shape, u32 shapeSize, ParticleDef *desc, u8, u8, u8, u8, s8, u8, s32);

/* Spawns the burst effect on actor: two sparks and two glows that the effect keeps on the actor. */
void func_151BD828(Actor *actor, u8 arg1, s32 arg2) {
    BurstEffect *effect;
    BurstData burst;
    BurstData *data;

    burst.owner = actor;
    burst.ownerId = actor->unique_id;
    burst.sparks[0] = NULL;
    burst.sparks[1] = NULL;
    burst.glows[0] = NULL;
    burst.glows[1] = NULL;
    effect = (BurstEffect *)func_15149130(300, -1, 0x40, -1, 0, 0x33, sizeof(BurstData), arg1, arg2);
    if (effect != NULL) {
        data = &effect->data;
        memcpy(data, &burst, sizeof(burst));
        {
            BurstSparkData spark;
            ParticleDef sparkEffect;

            spark.flags = 0;
            spark.scale = (actor->xz_scale + actor->y_scale) * 0.5f;
            spark.owner = actor;
            spark.ownerId = actor->unique_id;
            spark.widthBase = 15.045272f;
            spark.widthRange = 5.8676558f;
            spark.widthTarget = 12.111444f;
            spark.heightBase = 0.396174f;
            spark.widthTimer = 0.0f;
            spark.heightTimer = 0.0f;
            spark.alphaTimer = 0.0f;
            sparkEffect.width = 0.0f;
            sparkEffect.height = 0.0f;
            spark.alpha = 0.0f;
            spark.beamWidthTarget = 1.0f;
            spark.beamWidth = 1.0f;
            spark.beamLengthTarget = 1.0f;
            spark.beamLength = 1.0f;
            spark.burst = effect;
            sparkEffect.texture = 0x5F;
            sparkEffect.type = 8;
            sparkEffect.renderModes = 0x2203;
            sparkEffect.lifetime = 300;
            sparkEffect.frame = 0;
            sparkEffect.frameSpeed = 0;
            sparkEffect.rgba[0] = 0xFF;
            sparkEffect.rgba[1] = 0xFF;
            sparkEffect.rgba[2] = 0xFF;
            sparkEffect.rgba[3] = 0xFF;
            spark.heightRange = 0.1042563f;
            spark.heightRange2 = 0.5197371f;
            spark.heightTarget = 0.34404588f;
            spark.alphaBase = 160.0f;
            spark.alphaRange = 95.0f;
            spark.alphaTarget = 207.5f;
            sparkEffect.pos = D_800A5480;
            sparkEffect.rot = D_800A5480;
            sparkEffect.scale.x = 1.0f;
            sparkEffect.scale.y = 1.0f;
            sparkEffect.scale.z = 1.0f;
            sparkEffect.flags = 0x40EC0000;
            sparkEffect.unk44 = 0;
            sparkEffect.unk45 = 0xFF;
            sparkEffect.unk46 = 0;
            sparkEffect.unk47 = 7;
            sparkEffect.unk48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                data->sparks[spark.index] = func_1513D2F0(&sparkEffect, &D_800A4AA0, 0, 0x25, 0, 0x1D,
                    ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, sizeof(spark), arg1, arg2);
                if (data->sparks[spark.index] != NULL) {
                    memcpy(&data->sparks[spark.index]->data, &spark, sizeof(spark));
                }
            }
        }
        {
            GlowShape shape;
            ParticleDef glowEffect;
            BurstGlowData glow;

            glow.textureTimer = 0;
            glow.targetScale = 1.0f;
            glow.owner = actor;
            glow.burst = effect;
            glow.ownerId = actor->unique_id;
            shape.width1 = 50.3f;
            shape.height1 = 50.3f;
            shape.width0 = 30.9f;
            shape.height0 = 30.9f;
            shape.offset1 = 0.1f;
            shape.offset0 = 0.1f;
            shape.alpha0 = 0.7f;
            shape.fadeRate = 0.000001f;
            shape.screenX[0] = -1;
            shape.screenY[0] = -1;
            shape.screenX[1] = -1;
            shape.screenY[1] = -1;
            shape.screenX[2] = -1;
            shape.screenY[2] = -1;
            shape.screenX[3] = -1;
            shape.screenY[3] = -1;
            shape.alpha1 = 1.0f;
            shape.attach = 0;
            shape.scale = 1.0f;
            shape.zbufSample[0] = 0;
            shape.zbufSample[1] = 0;
            shape.zbufSample[2] = 0;
            shape.zbufSample[3] = 0;
            shape.visibility[0] = 0;
            shape.visibility[1] = 0;
            shape.visibility[2] = 0;
            shape.visibility[3] = 0;
            shape.type = 0xF;
            shape.handler = -1;
            glowEffect.texture = 0x60;
            glowEffect.type = 3;
            glowEffect.renderModes = 0x2203;
            glowEffect.lifetime = 300;
            glowEffect.frame = 0;
            glowEffect.frameSpeed = 0;
            glowEffect.rgba[0] = 0xFF;
            glowEffect.rgba[1] = 0xFF;
            glowEffect.rgba[2] = 0xFF;
            glowEffect.rgba[3] = 0xFF;
            glowEffect.width = 100.0f;
            glowEffect.height = 100.0f;
            glowEffect.pos = D_800A5480;
            glowEffect.rot = D_800A5480;
            glowEffect.scale.x = 1.0f;
            glowEffect.scale.y = 1.0f;
            glowEffect.scale.z = 1.0f;
            glowEffect.flags = 0xCD2002;
            glowEffect.unk44 = 0xFF;
            glowEffect.unk45 = 0xFF;
            glowEffect.unk46 = 0;
            glowEffect.unk47 = 7;
            glowEffect.unk48 = 0;
            glowEffect.viewportMask = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                data->glows[glow.index] = func_151407D0(&shape, 0x78, &glowEffect, 0, 0x2A, 0, 0, -1, arg1, arg2);
                if (data->glows[glow.index] != NULL) {
                    memcpy(&data->glows[glow.index]->data, &glow, sizeof(glow));
                }
            }
        }
    }
}

extern Vec3f D_800AA778;
extern Vec3f D_800AA784;
extern Vec3f D_800AA790;
extern Vec3f D_800AA79C;
extern s32 func_15145EA4(Vec3f **src, Vec3f **dst, Mtx *matrix, s32 count);

/* Per-frame update of the burst: keeps the sparks and glows on bone 1 of the owner. */
void func_151BDD8C(BurstEffect *effect) {
    BurstData *data;
    Actor *owner;
    Vec3f starts[2];
    Vec3f ends[2];
    u8 i;
    u8 j;
    u8 k;
    Vec3f *srcs[4];
    Vec3f *dsts[4];
    s32 player;

    data = &effect->data;
    owner = data->owner;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
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
                spark->head.def.pos = starts[i];
                spark->head.def.rot = ends[i];
            }
            if (data->glows[i] != NULL) {
                glow = data->glows[i];
                glow->head.def.flags |= 2;
                glow->head.def.flags &= ~4;
                glow->head.def.pos = starts[i];
                glow->head.def.rot = ends[i];
            }
        }
    } else {
        for (j = 0; j < 2; j++) {
            if (data->sparks[j] != NULL) {
                BurstSparkData *sparkData = &data->sparks[j]->data;

                sparkData->flags &= ~1;
            }
            if (data->glows[j] != NULL) {
                data->glows[j]->head.def.flags &= ~2;
            }
        }
    }
    player = owner - D_800CC2D0;
    if (player < 4) {
        for (k = 0; k < 2; k++) {
            if (data->sparks[k] != NULL) {
                BurstSparkData *sparkData = &data->sparks[k]->data;

                if (D_800BE728[(u8)player]->button & A_BUTTON) {
                    sparkData->beamWidthTarget = 6.0f;
                    sparkData->beamLengthTarget = 21.0f;
                } else {
                    sparkData->beamWidthTarget = 3.0f;
                    sparkData->beamLengthTarget = 8.0f;
                }
            }
            if (data->glows[k] != NULL) {
                BurstGlowData *glowData = &data->glows[k]->data;

                if (D_800BE728[(u8)player]->button & A_BUTTON) {
                    glowData->targetScale = 1.5f;
                } else {
                    glowData->targetScale = 1.0f;
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
