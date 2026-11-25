#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Link fields shared by every object (see game_1944C0.c). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

/* Object lists: D_800DCE50[list][type] is the head of a linked list of objects of that type. */
extern ObjLink *D_800DCE50[2][104];

/* While a list is walked, the next node is saved in D_800DD198[depth] (depth D_800DD190). */

#define OBJ_TYPE_TRAIL_EMITTER 0x1F

/* Emitter settings, copied into the object from a template by func_1518BCD0. */
typedef struct {
    /* 0x00 */ f32 speedMin;
    /* 0x04 */ f32 speedRange;
    /* 0x08 */ f32 halfWidthMin;
    /* 0x0C */ f32 halfWidthRange;
    /* 0x10 */ s16 pointRange; /* each trail gets 3 + rand % (pointRange + 1) points */
    /* 0x12 */ s16 angleRange;
    /* 0x14 */ Actor *owner;
    /* 0x18 */ u8 ownerId;     /* owner->unique_id when attached */
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 flags;
} TrailEmitterParams; /* size 0x1C */

/* Type 0x1F object: emits trails from two points of its owner actor. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ TrailEmitterParams params;
    /* 0x2C */ s32 timers[2]; /* time until the next trail from each point */
    /* 0x34 */ f32 prevX[2];  /* each point's position on the previous update */
    /* 0x3C */ f32 prevZ[2];
} TrailEmitter; /* size 0x44 */

void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list);

/* Returns the actor's trail emitter, creating one if its kind has emitter settings. */
TrailEmitter *func_1518BA90(Actor *actor, u8 owner, s32 list) {
    ObjLink **heads;
    TrailEmitterParams params;
    TrailEmitter *emitter;
    s32 i;

    if (actor == NULL) {
        return NULL;
    }
    for (i = 0; i < 2; i++) {
        heads = D_800DCE50[i];
        for (emitter = (TrailEmitter *)heads[OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
             emitter = (TrailEmitter *)emitter->link.next) {
            if (emitter->params.owner == actor || emitter->params.ownerId == actor->unique_id) {
                return emitter;
            }
        }
    }

    switch (actor->id + 1) {
    case 0: /* id 0xFF */
        return NULL;
    case 8:
        params.angleRange = 55;
        params.pointRange = 4;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 2.75999999f;
        params.speedRange = 5.960000038f;
        params.halfWidthMin = 3.210000038f;
        params.halfWidthRange = 5.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        params.angleRange = 0;
        params.pointRange = 5;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 10.0f;
        params.speedRange = 10.0f;
        params.halfWidthMin = 10.0f;
        params.halfWidthRange = 10.0f;
        break;
    default:
        return NULL;
    }
    params.owner = actor;
    params.ownerId = actor->unique_id;
    return func_1518BCD0(&params, owner, list);
}

/* Destroys every trail emitter (after pointing each at `actor`). */
void func_1518BBF4(Actor *actor) {
    s32 i;
    TrailEmitter *emitter;

    if (actor != NULL) {
        for (i = 0; i < 2; i++) {
            D_800DD190++;
            for (emitter = (TrailEmitter *)D_800DCE50[i][OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
                 emitter = (TrailEmitter *)D_800DD198[D_800DD190]) {
                D_800DD198[D_800DD190] = emitter->link.next;
                emitter->params.owner = actor;
                // the owner test always passes: every emitter is destroyed
                if (actor != NULL || emitter->params.ownerId == actor->unique_id) {
                    func_1516972C((struct102 *)emitter);
                }
            }
            {
                // matching: a slotless block-scope decl moves the -g3 line so the decrement fills the bnel slot
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
}

/* Spawns a trail emitter object from `params` with random initial timers. */
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list) {
    TrailEmitter *emitter;

    emitter = func_15167A68(OBJ_TYPE_TRAIL_EMITTER, list, sizeof(TrailEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(TrailEmitterParams));
    emitter->timers[0] = func_150ADA20() & 0x1F;
    emitter->timers[1] = func_150ADA20() & 0x1F;
    return emitter;
}

/* Trail types (see game_175250.c). */

/* One point of a trail; the points form a ring buffer. */
typedef struct {
    Vec3f pos;
    f32 velocityY;
    u16 phase; /* texture coordinate along the trail */
    u8 pad12[2];
} TrailPoint;

typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ Vec3f position;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[7];
    /* 0x25 */ u8 capacity; /* number of points in the ring buffer */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 start;    /* oldest point */
    /* 0x2E */ s8 end;      /* one past the newest point */
    /* 0x2F */ u8 pad2F[0x65];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailParams *params;
} Trail;

typedef struct {
    TrailEmitter *emitter;
    s8 point; /* which of the owner's two points the trail follows */
} TrailEmitterLink;

/* Params of a trail spawned by a TrailEmitter: the TrailParams, then emitter data. */
typedef struct {
    /* 0x00 */ TrailParams base;
    /* 0x48 */ TrailEmitterLink link;
    /* 0x50 */ Vec3f offsets[1]; /* per trail point, relative to the emit point */
} EmitterTrailParams;

extern Trail *func_15147DA0(EffectTemplate18 *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                            s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                            TrailRender *render, s32 arg12, s32 arg13, s32 arg14);

/* Trail emitter update: while the owner is alive, spawns a trail from each point when its timer runs out. */
void func_1518BD60(TrailEmitter *emitter) {
    u8 i;
    s32 point;
    Trail *trail;
    EffectTemplate18 spawn;
    TrailMotion motion;
    s32 pad; // matching: stack slot between motion and render
    TrailRender render;
    f32 speed;
    f32 sinA;
    f32 cosA;
    s32 angle;
    Actor *owner;
    TrailEmitterLink *link;

    for (i = 0; i < 2; i++) {
        emitter->timers[i] -= D_800BE9E4;
        owner = emitter->params.owner;
        if (owner != NULL && owner->interaction_state != 0 && owner->unk1D4 != NULL &&
            owner->unique_id == emitter->params.ownerId) {
            if (emitter->timers[i] < 0) {
                point = i;
                if (point) {} // matching: a second use keeps the copy of i in its own register
                angle = func_150ADA20() % (u32)(emitter->params.angleRange + 1);
                sinA = func_151423D8(angle);
                cosA = func_151423D8(angle - 0x40);
                speed = func_150ADA68() * emitter->params.speedRange + emitter->params.speedMin;
                emitter->timers[i] = func_150ADA20() % 23U + 5;
                render.unk0 = 0;
                render.setGeometryMode = 1;
                render.clearGeometryMode = 0x160600;
                render.renderMode = 3;
                render.unk10 = 0x10;
                render.unk14 = 0x80;
                render.unk18 = 0x20;
                render.primMode = 0;
                render.envMode = 9;
                spawn.itemCount = func_150ADA20() % (u32)(emitter->params.pointRange + 1) + 3;
                spawn.kind = 1;
                spawn.flags = 1;
                spawn.life = func_150ADA20() % 21U + 80;
                motion.texture = emitter->params.texture;
                motion.halfWidth = func_150ADA68() * emitter->params.halfWidthRange + emitter->params.halfWidthMin;
                motion.velocity.y = 0.0f;
                motion.gravity = 0.6539999843f;
                motion.velocity.x = speed * cosA;
                motion.velocity.z = speed * sinA;
                motion.flags = emitter->params.flags | 8;
                motion.shade = 0xFF - ((u8)((u32)emitter->params.owner->unk184 >> 5) << 6);
                motion.alpha = 0xFF;
                trail = func_15147DA0(&spawn, &motion, spawn.itemCount * sizeof(Vec3f) + 8, 5, 1, 0, 2, 1, 0, 0,
                                      0, &render, 0, emitter->link.owner, emitter->link.list);
                if (trail != NULL) {
                    link = &((EmitterTrailParams *)trail->params)->link;
                    link->emitter = emitter;
                    link->point = point;
                }
                owner = emitter->params.owner;
            }
            emitter->prevX[i] = owner->aim.side[i][0];
            emitter->prevZ[i] = emitter->params.owner->aim.side[i][2];
        }
    }
}

/*
 * Trail update for trails spawned by a TrailEmitter: moves each point by its velocity and gravity
 * relative to the owner's emit point (rotated by the owner's facing), then either grows the trail by
 * one point or, once full, hands the owner's motion to the trail and lets it go.
 * Returns 0 when the owner is gone.
 */
s32 func_1518C0B8(Trail *trail) {
    EmitterTrailParams *params;
    TrailPoint *points;
    s32 i;
    TrailEmitterLink *link;
    Vec3f *offsets;
    Vec3f pos;
    f32 dirX;
    f32 dirZ;
    f32 lenSq;
    f32 cosA;
    f32 sinA;
    f32 invLen;
    u16 phase;
    u16 phaseStep;
    f32 ownerVelX;
    f32 ownerVelZ;
    f32 invDt;
    f32 velX;
    f32 velZ;

    points = trail->points;
    params = (EmitterTrailParams *)trail->params;
    link = &params->link;
    offsets = params->offsets;
    i = trail->start;
    if (link->emitter->params.owner == NULL || link->emitter->params.owner->interaction_state == 0 ||
        link->emitter->params.owner->unk1D4 == NULL ||
        link->emitter->params.owner->unique_id != link->emitter->params.ownerId) {
        return 0;
    }
    pos.x = link->emitter->params.owner->aim.side[link->point][0];
    pos.y = link->emitter->params.owner->aim.side[link->point][1];
    pos.z = link->emitter->params.owner->aim.side[link->point][2];
    dirX = link->emitter->params.owner->aim.dir[0] * (1.0f / 256);
    dirZ = link->emitter->params.owner->aim.dir[2] * (1.0f / 256);
    lenSq = dirX * dirX + dirZ * dirZ;
    if (lenSq) {
        // matching: a slotless block-scope decl adds the -g3 line that gives the nop after the mul.s
        extern f32 sqrtf(f32);

        invLen = 1.0f / sqrtf(lenSq);
        cosA = dirX * invLen;
        sinA = dirZ * invLen;
    } else {
        sinA = cosA = 0.0f;
    }
    while (i != trail->end) {
        points[i].velocityY -= params->base.motion.gravity * D_800BE9A4;
        offsets[i].x += params->base.motion.velocity.x * D_800BE9A4;
        offsets[i].y += points[i].velocityY;
        offsets[i].z += params->base.motion.velocity.z * D_800BE9A4;
        points[i].pos.x = (pos.x + offsets[i].x * sinA) + offsets[i].z * cosA;
        points[i].pos.y = offsets[i].y + pos.y;
        points[i].pos.z = (pos.z + offsets[i].z * sinA) - offsets[i].x * cosA;
        i++;
        if (i == trail->capacity) {
            i = 0;
        }
    }
    if (trail->count < trail->capacity - 1) {
        phase = 0;
        phaseStep = 0;
        if (trail->count != 0) {
            phaseStep = 0x1000 / trail->count;
        }
        points[trail->end].pos = pos;
        points[trail->end].velocityY = 0.0f;
        offsets[trail->end].x = 0.0f;
        offsets[trail->end].y = 0.0f;
        offsets[trail->end].z = 0.0f;
        trail->count++;
        trail->end++;
        if (trail->end == trail->capacity) {
            trail->end = 0;
        }
        for (i = trail->start; i != trail->end;) {
            points[i].phase = phase;
            i++;
            phase += phaseStep;
            if (i == trail->capacity) {
                i = 0;
            }
        }
    } else {
        invDt = 1.0f / D_800BE9A4;
        velX = params->base.motion.velocity.x * sinA + params->base.motion.velocity.z * cosA;
        velZ = params->base.motion.velocity.z * sinA - params->base.motion.velocity.x * cosA;
        ownerVelX = (pos.x - link->emitter->prevX[link->point]) * invDt;
        ownerVelZ = (pos.z - link->emitter->prevZ[link->point]) * invDt;
        params->base.motion.velocity.x = velX + ownerVelX;
        params->base.motion.velocity.z = velZ + ownerVelZ;
        if (params->base.motion.flags & 0x17) {
            params->base.state = 3;
        } else {
            params->base.state = 2;
        }
    }
    return 1;
}

/* Trail fade: alpha (motion.alpha) follows 8 * unk1C, capped at 0xFF. */
s32 func_1518C540(Trail *trail) {
    s32 alpha;
    TrailParams *params;

    params = trail->params;
    alpha = trail->unk1C << 3;
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    if ((alpha & 0xFF) < 0) { /* never true */
        return 0;
    }
    return 1;
}

/* Template / payload of a type 0xE QuadFx object (see game_1944C0.c). */
typedef struct {
    /* 0x00 */ Vtx vtx[8];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ s16 frame;
    /* 0x8A */ s16 frameStep; /* 8.8 fixed point */
    /* 0x8C */ s16 x;
    /* 0x8E */ s16 y;
    /* 0x90 */ s16 z;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ u16 flags;
    /* 0x9A */ u8 pad9A[6];
    /* 0xA0 */ u8 animIndex;  /* index into D_8008CA4C */
    /* 0xA1 */ u8 unkA1;
    /* 0xA2 */ s8 callback;   /* -1 = none */
} QuadFxData;

void func_1516865C(QuadFxData *q, u8 r, u8 g, u8 b, u8 a);
void *func_15168800(QuadFxData *src, s32 owner, s32 list);
u8 func_151D8E20(void);

/* Trail hit handler: spawns a QuadFx puff where the trail's oldest point hit at height `hitY`. */
s32 func_1518C57C(Trail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY) {
    QuadFxData fx;
    TrailParams *params;
    TrailPoint *points;

    params = trail->params;
    points = trail->points;
    fx.unk80 = 0;
    fx.frameStep = 0x100;
    fx.x = points[trail->start].pos.x;
    fx.y = hitY;
    fx.z = points[trail->start].pos.z;
    fx.unk94 = func_150ADA20() % 21U + 35;
    fx.unk92 = fx.unk94;
    fx.callback = -1;
    fx.animIndex = 0x12;
    fx.flags = 0xC;
    fx.unkA1 = 0;
    fx.unk96 = hitY;
    func_1516865C(&fx, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800(&fx, trail->link.owner, trail->link.list);
    params->state = 4;
    return 1;
}

/* Same as func_1518C57C, plus a func_151D8E20() effect (arg5) at the hit point. */
s32 func_1518C69C(Trail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY, s32 arg5) {
    QuadFxData fx;
    TrailParams *params;
    TrailPoint *points;

    params = trail->params;
    points = trail->points;
    fx.unk80 = 0;
    fx.frameStep = 0x100;
    fx.x = points[trail->start].pos.x;
    fx.y = hitY;
    fx.z = points[trail->start].pos.z;
    fx.unk94 = func_150ADA20() % 21U + 35;
    fx.unk92 = fx.unk94;
    fx.callback = -1;
    fx.animIndex = 0x12;
    fx.flags = 0xC;
    fx.unkA1 = 0;
    fx.unk96 = hitY;
    func_1516865C(&fx, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800(&fx, trail->link.owner, trail->link.list);
    params->state = 4;
    {
        u8 kind;
        f32 pos[3];

        pos[0] = points[trail->start].pos.x;
        pos[1] = hitY;
        pos[2] = points[trail->start].pos.z;
        kind = func_151D8E20();
        func_151DBCBC(kind, func_150ADA68() * 40.0f + 100.0f, 0xFF, arg5, pos, trail->link.owner, trail->link.list);
    }
    return 1;
}

/* Message sent to objects about an actor (see game_105FC0.c's TrailMessage). */
typedef struct {
    Actor *actor;
    union {
        u8 actorId;        /* type 0 */
        Actor *other;  /* type 0x2D */
    } x4;
    u8 actorId;  /* type 0x2D */
    u8 otherId;  /* type 0x2D */
} ActorMessage;

/*
 * Trail emitter message handler: follows its owner through an actor swap (type 0x2D) and is
 * destroyed when the owner goes away (type 0).
 */
void func_1518C850(TrailEmitter *emitter, s32 data, u8 type) {
    ActorMessage *msg = (ActorMessage *)data;
    Actor *actor;

    if (type == 0x2D) {
        actor = msg->actor;
        if (actor == emitter->params.owner) {
            emitter->params.owner = msg->x4.other;
            emitter->params.ownerId = msg->otherId;
        } else {
            if (emitter->params.owner == msg->x4.other) {
                emitter->params.owner = actor;
                emitter->params.ownerId = msg->actorId;
            }
        // matching: the label ends the block, so IDO emits the else path's own branch to the epilogue
        swapped:;
        }
    } else if (type == 0) {
        actor = msg->actor;
        if (actor == emitter->params.owner || msg->x4.actorId == emitter->params.ownerId) {
            func_1516972C((struct102 *)emitter);
        }
    }
}
