#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* While a list is walked, the next node is saved in gObjectWalkNext[depth] (depth gObjectWalkDepth). */

#define OBJ_TYPE_TRAIL_EMITTER 0x1F

/* Returns the actor's trail emitter, creating one if its kind has emitter settings. */
TrailEmitter *trail_getActorEmitter(Actor *actor, u8 owner, s32 list) {
    EffectHeader **heads;
    TrailEmitterParams params;
    TrailEmitter *emitter;
    s32 i;

    if (actor == NULL) {
        return NULL;
    }
    for (i = 0; i < 2; i++) {
        heads = gObjectLists[i];
        for (emitter = (TrailEmitter *)heads[OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
             emitter = (TrailEmitter *)emitter->link.next) {
            if (emitter->params.owner == actor || emitter->params.ownerId == actor->unique_id) {
                return emitter;
            }
        }
    }

    switch (actor->character_type + 1) {
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
    return trail_spawnEmitter(&params, owner, list);
}

/* Destroys every trail emitter (after pointing each at `actor`). */
void trail_destroyAllEmitters(Actor *actor) {
    s32 i;

    if (actor != NULL) {
        for (i = 0; i < 2; i++) {
            TrailEmitter *emitter;

            gObjectWalkDepth++;
            for (emitter = (TrailEmitter *)gObjectLists[i][OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
                 emitter = gObjectWalkNext[gObjectWalkDepth]) {
                gObjectWalkNext[gObjectWalkDepth] = emitter->link.next;
                emitter->params.owner = actor;
                // the owner test always passes: every emitter is destroyed
                if (actor != NULL || emitter->params.ownerId == actor->unique_id) {
                    effect_destroyObject(&emitter->link);
                }
            }
            gObjectWalkDepth--;
        }
    }
}

/* Spawns a trail emitter object from `params` with random initial timers. */
TrailEmitter *trail_spawnEmitter(TrailEmitterParams *params, u8 owner, s32 list) {
    TrailEmitter *emitter;

    emitter = effect_newObject(OBJ_TYPE_TRAIL_EMITTER, list, sizeof(TrailEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(TrailEmitterParams));
    emitter->timers[0] = math_random() & 0x1F;
    emitter->timers[1] = math_random() & 0x1F;
    return emitter;
}

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

/* MotionTrail emitter update: while the owner is alive, spawns a trail from each point when its timer runs out. */
void trail_updateEmitter(TrailEmitter *emitter) {
    u8 i;
    s32 point;
    MotionTrail *trail;
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
        emitter->timers[i] -= gFrameTicks;
        owner = emitter->params.owner;
        if (owner != NULL && owner->type != 0 && owner->ob_matrix_ptr != NULL &&
            owner->unique_id == emitter->params.ownerId) {
            if (emitter->timers[i] < 0) {
                point = i;
                if (point) {} // matching: a second use keeps the copy of i in its own register
                angle = math_random() % (emitter->params.angleRange + 1);
                sinA = math_cosBinAngleMasked(angle);
                cosA = math_cosBinAngleMasked(angle - 0x40);
                speed = math_randomFloat() * emitter->params.speedRange + emitter->params.speedMin;
                emitter->timers[i] = math_random() % 23U + 5;
                render.unk0 = 0;
                render.setGeometryMode = 1;
                render.clearGeometryMode = 0x160600;
                render.renderMode = 3;
                render.unk10 = 0x10;
                render.unk14 = 0x80;
                render.unk18 = 0x20;
                render.primMode = 0;
                render.envMode = 9;
                spawn.itemCount = math_random() % (emitter->params.pointRange + 1) + 3;
                spawn.kind = 1;
                spawn.flags = 1;
                spawn.life = math_random() % 21U + 80;
                motion.texture = emitter->params.texture;
                motion.halfWidth = math_randomFloat() * emitter->params.halfWidthRange + emitter->params.halfWidthMin;
                motion.velocity.y = 0.0f;
                motion.gravity = 0.6539999843f;
                motion.velocity.x = speed * cosA;
                motion.velocity.z = speed * sinA;
                motion.flags = emitter->params.flags | 8;
                motion.shade = 0xFF - ((u8)(emitter->params.owner->floorMaterial >> 5) << 6);
                motion.alpha = 0xFF;
                trail = trail_createMotionTrail(&spawn, &motion, spawn.itemCount * sizeof(Vec3f) + 8, 5, 1, 0, 2, 1, 0, 0,
                                      0, &render, 0, emitter->link.owner, emitter->link.kind);
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
 * MotionTrail update for trails spawned by a TrailEmitter: moves each point by its velocity and gravity
 * relative to the owner's emit point (rotated by the owner's facing), then either grows the trail by
 * one point or, once full, hands the owner's motion to the trail and lets it go.
 * Returns 0 when the owner is gone.
 */
s32 trail_updateEmittedTrail(MotionTrail *trail) {
    EmitterTrailParams *params;
    MotionTrailPoint *points;
    s32 i;
    TrailEmitterLink *link;
    Vec3f *offsets;
    Vec3f pos;
    f32 dirX;
    f32 dirZ;
    f32 lenSq;
    f32 cosA;
    f32 sinA;

    points = trail->points;
    params = (EmitterTrailParams *)trail->params;
    link = &params->link;
    offsets = params->offsets;
    i = trail->head.start;
    if (link->emitter->params.owner == NULL || link->emitter->params.owner->type == 0 ||
        link->emitter->params.owner->ob_matrix_ptr == NULL ||
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
        f32 invLen;

        invLen = 1.0f / sqrtf(lenSq);
        cosA = dirX * invLen;
        sinA = dirZ * invLen;
    } else {
        sinA = cosA = 0.0f;
    }
    while (i != trail->head.end) {
        points[i].velocityY -= params->base.motion.gravity * gFrameDelta;
        offsets[i].x += params->base.motion.velocity.x * gFrameDelta;
        offsets[i].y += points[i].velocityY;
        offsets[i].z += params->base.motion.velocity.z * gFrameDelta;
        points[i].pos.x = (pos.x + offsets[i].x * sinA) + offsets[i].z * cosA;
        points[i].pos.y = offsets[i].y + pos.y;
        points[i].pos.z = (pos.z + offsets[i].z * sinA) - offsets[i].x * cosA;
        i++;
        if (i == trail->head.tmpl.itemCount) {
            i = 0;
        }
    }
    if (trail->head.count < trail->head.tmpl.itemCount - 1) {
        u16 phase;
        u16 phaseStep;

        phase = 0;
        phaseStep = 0;
        if (trail->head.count != 0) {
            phaseStep = 0x1000 / trail->head.count;
        }
        points[trail->head.end].pos = pos;
        points[trail->head.end].velocityY = 0.0f;
        offsets[trail->head.end].x = 0.0f;
        offsets[trail->head.end].y = 0.0f;
        offsets[trail->head.end].z = 0.0f;
        trail->head.count++;
        trail->head.end++;
        if (trail->head.end == trail->head.tmpl.itemCount) {
            trail->head.end = 0;
        }
        for (i = trail->head.start; i != trail->head.end;) {
            points[i].phase = phase;
            i++;
            phase += phaseStep;
            if (i == trail->head.tmpl.itemCount) {
                i = 0;
            }
        }
    } else {
        f32 ownerVelX;
        f32 ownerVelZ;
        f32 invDt;
        f32 velX;
        f32 velZ;

        invDt = 1.0f / gFrameDelta;
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

/* MotionTrail fade: alpha (motion.alpha) follows 8 * unk1C, capped at 0xFF. */
s32 trail_fadeEmittedTrail(MotionTrail *trail) {
    s32 alpha;
    TrailParams *params;

    params = trail->params;
    alpha = trail->head.tmpl.life << 3;
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    if ((alpha & 0xFF) < 0) { /* never true */
        return 0;
    }
    return 1;
}

/* MotionTrail hit handler: spawns a QuadFx puff where the trail's oldest point hit at height `hitY`. */
s32 trail_spawnHitPuff(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY) {
    QuadFxData fx;
    TrailParams *params;
    MotionTrailPoint *points;

    params = trail->params;
    points = trail->points;
    fx.dropFlags = 0;
    fx.frameStep = 0x100;
    fx.x = points[trail->head.start].pos.x;
    fx.y = hitY;
    fx.z = points[trail->head.start].pos.z;
    fx.unk94 = math_random() % 21U + 35;
    fx.unk92 = fx.unk94;
    fx.callback = -1;
    fx.animIndex = 0x12;
    fx.flags = 0xC;
    fx.speed = 0;
    fx.floorY = hitY;
    effect_initQuadFxTemplate(&fx, 0xFF, 0xFF, 0xFF, 0xFF);
    effect_spawnQuadFx(&fx, trail->head.header.owner, trail->head.header.kind);
    params->state = 4;
    return 1;
}

/* Same as trail_spawnHitPuff, plus a particle_getContextEffectKind() effect (arg5) at the hit point. */
s32 trail_spawnHitPuffAndEffect(MotionTrail *trail, f32 x, f32 y, f32 z, f32 hitY, CollisionTri *surface) {
    QuadFxData fx;
    TrailParams *params;
    MotionTrailPoint *points;

    params = trail->params;
    points = trail->points;
    fx.dropFlags = 0;
    fx.frameStep = 0x100;
    fx.x = points[trail->head.start].pos.x;
    fx.y = hitY;
    fx.z = points[trail->head.start].pos.z;
    fx.unk94 = math_random() % 21U + 35;
    fx.unk92 = fx.unk94;
    fx.callback = -1;
    fx.animIndex = 0x12;
    fx.flags = 0xC;
    fx.speed = 0;
    fx.floorY = hitY;
    effect_initQuadFxTemplate(&fx, 0xFF, 0xFF, 0xFF, 0xFF);
    effect_spawnQuadFx(&fx, trail->head.header.owner, trail->head.header.kind);
    params->state = 4;
    {
        u8 kind;
        f32 pos[3];

        pos[0] = points[trail->head.start].pos.x;
        pos[1] = hitY;
        pos[2] = points[trail->head.start].pos.z;
        kind = particle_getContextEffectKind();
        particle_spawnKindMark(kind, math_randomFloat() * 40.0f + 100.0f, 0xFF, surface, pos, trail->head.header.owner, trail->head.header.kind);
    }
    return 1;
}

/*
 * MotionTrail emitter message handler: follows its owner through an actor swap (type 0x2D) and is
 * destroyed when the owner goes away (type 0).
 */
void trail_onEmitterMessage(TrailEmitter *emitter, s32 data, u8 type) {
    ActorMsg *msg = (ActorMsg *)data;
    Actor *actor;

    if (type == 0x2D) {
        actor = msg->swap.actorA;
        if (actor == emitter->params.owner) {
            emitter->params.owner = msg->swap.actorB;
            emitter->params.ownerId = msg->swap.idB;
        } else {
            if (emitter->params.owner == msg->swap.actorB) {
                emitter->params.owner = actor;
                emitter->params.ownerId = msg->swap.idA;
            }
        // matching: the label ends the block, so IDO emits the else path's own branch to the epilogue
        swapped:;
        }
    } else if (type == 0) {
        actor = msg->ref.actor;
        if (actor == emitter->params.owner || msg->ref.uniqueId == emitter->params.ownerId) {
            effect_destroyObject(&emitter->link);
        }
    }
}
