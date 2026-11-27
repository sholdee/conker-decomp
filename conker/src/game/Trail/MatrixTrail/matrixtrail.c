#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/trail.h"

/*
 * Ribbon trails attached to a point of an object (a game object's matrix plus an offset).
 * A trail keeps its points in a ring buffer, emits new points at the attachment point at a
 * fixed rate, ages them and draws them as a textured strip. Trails are started either
 * directly (trail_startMatrixTrail) or by a task that waits until the owner has its matrices.
 *
 * Frame timing globals: gFrameDelta is the frame time step, gFrameDeltaInv its reciprocal and
 * gFrameTicks the number of frames elapsed.
 */

/* Trail parameters, one entry per trail kind (gMatrixTrailSpecs). */
typedef struct TrailSpec {
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 spawnRate;
    /* 0x08 */ u8 brightness;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 matrixIndex;
    /* 0x14 */ Vec3f offset;
    /* 0x20 */ s16 life;
    /* 0x22 */ s16 delay;
    /* 0x24 */ s16 lifeDecay;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 widthGrowth;
    /* 0x30 */ f32 drag;
    /* 0x34 */ f32 follow;
    /* 0x38 */ s16 texStep;
    /* 0x3A */ s16 texStepRange;
    /* 0x3C */ u8 headFadeLen;
    /* 0x3D */ u8 headFadeStep;
    /* 0x3E */ u8 tailFadeLen;
    /* 0x3F */ u8 tailFadeStep;
} TrailSpec;

/* The ribbon trail object currently started by trail_queueTrailPair's task, or NULL (game/Game/LevelStart/levelstart clears it). */
TrailTask *gPendingTrailPairTask;

static const TrailSpec gMatrixTrailSpecs[5] = {
    { 0.6f, 0.37f, 255, 11, 22, { 0.0f, 45.0f, 0.0f }, 255, 6, 18, 4.0f, 0.4f, 0.3f, 1.0f, 0, 7, 2, 127, 2, 127 },
    { 0.6f, 0.37f, 255, 11, 6, { 35.0f, 27.0f, 59.0f }, 100, 6, 4, 7.0f, 0.6f, 0.3f, 0.4f, 0, 32, 2, 127, 2, 127 },
    { 0.14999999f, 0.38f, 255, 10, 15, { -21.0f, -12.0f, 32.0f }, 150, 16, 6, 2.8f, 0.099999994f, 0.3f, 1.0f, 0, 5, 2, 127, 2, 127 },
    { 0.14999999f, 0.38f, 127, 15, 4, { -5.3f, -93.0f, 39.0f }, 150, 45, 7, 6.2000003f, 0.17f, 0.3f, 0.5f, 0, 7, 2, 127, 2, 127 },
    { 0.14999999f, 0.38f, 127, 15, 9, { 5.3f, -93.0f, 39.0f }, 150, 45, 7, 6.2000003f, 0.17f, 0.3f, 0.5f, 0, 7, 2, 127, 2, 127 },
};

/* Stops every trail of kind specIndex (event 6). */
void trail_stopMatrixTrailsOfKind(u8 specIndex) {
    u8 event[4];

    event[0] = specIndex;
    effect_sendEventToEffectObjs((s32)event, 6);
}

/* Stops a trail: it no longer emits and its tail fades out. */
void trail_stopMatrixTrail(MatrixTrail *trail) {
    MatrixTrailData *data;

    trail->head.updateKind2 = 0;
    data = trail->data;
    trail->head.tmpl.flags &= ~2;
    data->flags |= 1;
    data->flags |= 4;
}

/* Trail teardown (two variants): detaches it from its owner's attachments first. */
void trail_destroyMatrixTrail(MatrixTrail *trail) {
    MatrixTrailData *data;

    data = trail->data;
    effect_detachHandle(trail, data->owner);
    effect_defaultDestroyEffectObj(trail);
}

void trail_freeMatrixTrail(MatrixTrail *trail) {
    MatrixTrailData *data;

    data = trail->data;
    effect_detachHandle(trail, data->owner);
    effect_defaultFreeEffectObj(trail);
}

/* Starts a trail of kind specIndex on owner. Returns it, or NULL. */
MatrixTrail *trail_startMatrixTrail(Actor *owner, s32 specIndex, s16 lifetime, u8 arg3, u8 arg4, s32 arg5) {
    EffectTemplate spawn;
    MatrixTrailData data;
    MatrixTrail *trail;
    const TrailSpec *spec;

    spec = &gMatrixTrailSpecs[specIndex];

    if (owner == NULL) {
        return NULL;
    }

    data.time = 0.0f;
    data.ownerPos.x = owner->xpos;
    data.ownerPos.y = owner->ypos;
    data.ownerPos.z = owner->zpos;
    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.flags = 2;
    data.texS = 0;
    data.specIndex = specIndex;
    spawn.life = lifetime;
    data.spawnCount = 0.0f;

    spawn.flags = (arg3 ? 1 : 0) | 2;

    data.gravity = spec->gravity;
    data.spawnRate = spec->spawnRate;
    data.brightness = spec->brightness;
    spawn.itemCount = spec->unkC;
    data.matrixIndex = spec->matrixIndex;
    data.offset = spec->offset;
    data.life = spec->life;
    data.delay = spec->delay;
    data.lifeDecay = spec->lifeDecay;
    data.width = spec->width;
    data.widthGrowth = spec->widthGrowth;
    data.drag = spec->drag;
    data.follow = spec->follow;
    data.texStep = spec->texStep;
    data.texStepRange = spec->texStepRange;
    data.headFadeLen = spec->headFadeLen;
    data.headFadeStep = spec->headFadeStep;
    data.tailFadeLen = spec->tailFadeLen;
    data.tailFadeStep = spec->tailFadeStep;

    if (trail_getAttachPoint(&data, &spawn.pos, 0.0f, 0.0f, 0.0f, 1.0f) != 0) {
        data.lastPos = spawn.pos;
        spawn.flags |= 4;
    }

    spawn.kind = 4;
    trail = effect_newEffectObj(&spawn, sizeof(MatrixTrailData), sizeof(MatrixTrailPoint), 3, 3, 3, 0, 0, 0, arg4, arg5);
    if (trail != NULL) {
        memcpy(trail->data, &data, sizeof(MatrixTrailData));
    }
    return trail;
}

/* Ages a trail's points (newest to oldest) and drops the dead ones. Returns 0 once a stopped trail is gone. */
s32 trail_ageMatrixTrail(MatrixTrail *trail) {
    MatrixTrailData *data;
    MatrixTrailPoint *points;
    s32 i;
    s32 offset;
    s16 delay;
    MatrixTrailPoint *point;
    MatrixTrailPoint *same;

    data = trail->data;
    points = trail->points;
    if ((trail->head.count < 2) && (data->flags & 1)) {
        return 0;
    }
    i = trail->head.end;
    while (i != trail->head.start) {
        i--;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        offset = i * sizeof(MatrixTrailPoint);
        point = &points[i];
        point->alpha = 0xFF;
        // matching: a second, commuted address of the same point (points + offset vs offset + points) is not CSE'd
        same = (MatrixTrailPoint *)((u8 *)points + offset);
        delay = point->delay;
        point->vel.x *= data->drag;
        point->vel.y += data->gravity * gFrameDelta;
        point->vel.z *= data->drag;
        point->pos.x += same->vel.x * gFrameDelta;
        point->pos.y += same->vel.y * gFrameDelta;
        point->pos.z += same->vel.z * gFrameDelta;
        if (delay > 0) {
            point->delay = delay - gFrameTicks;
        } else {
            point->life -= gFrameTicks * data->lifeDecay;
        }
        point->width += gFrameDelta * data->widthGrowth;
        if (point->life < 0) {
            /* Everything from the head up to this point is dead too. */
            data->flags &= ~2;
            while (i != trail->head.start) {
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            points[trail->head.start].life = 0;
        }
    }
    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Moves a trail's emitter towards its attachment point and emits the points it owes along the
 * way, then moves all points along with the owner. A trail whose attachment point is lost stops
 * and hands over to a task (trail_updateStartTask family). Returns 0 when the owner is gone.
 */
s32 trail_followMatrixTrail(MatrixTrail *trail) {
    MatrixTrailData *data;
    MatrixTrailPoint *points;
    s32 started;
    f32 prevX;
    f32 prevZ;
    TrailTaskData taskData;
    TrailTask *task;
    f32 invCount;
    f32 time;
    f32 timeStep;
    f32 dz;
    f32 dy;
    f32 dx;
    f32 dw;
    Vec3f pos;
    f32 width;
    s32 i;
    f32 moveX;
    f32 moveY;
    f32 moveZ;
    Vec3f vel;
    f32 stepY;
    f32 stepZ;
    f32 stepWidth;

    data = trail->data;
    points = trail->points;
    if (data->owner->type == 0) {
        return 0;
    }
    if (data->owner->unique_id != data->ownerId) {
        return 0;
    }
    started = 0;
    if (!(trail->head.tmpl.flags & 4)) {
        if (trail_getAttachPoint(data, &trail->head.tmpl.pos, 0.0f, 0.0f, 0.0f, 1.0f) != 0) {
            started = 1;
            data->lastPos = trail->head.tmpl.pos;
            trail->head.tmpl.flags |= 4;
        } else {
            return 1;
        }
    }
    prevX = trail->head.tmpl.pos.x;
    prevZ = trail->head.tmpl.pos.z;
    if (!started && trail_getAttachPoint(data, &trail->head.tmpl.pos, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, data->follow) == 0) {
        trail_stopMatrixTrail(trail);
        taskData.owner = data->owner;
        taskData.ownerId = data->owner->unique_id;
        taskData.unk8 = data->specIndex;
        task = effect_newTimedEffectNoDraw(trail->head.tmpl.life, -1, 8, (trail->head.tmpl.flags & 1) ? 1 : 0, 2, 0xC, 0xFF, 0);
        if (task != NULL) {
            memcpy(&task->data, &taskData, sizeof(TrailTaskData));
            if (effect_findAttachmentByHandle(data->owner->attachments, trail, NULL) != 0) {
                effect_attach(task, data->owner, 0x11);
            }
        }
        return 1;
    }
    data->spawnCount += data->spawnRate * gFrameDelta;
    if (data->spawnCount > 1.0f) {
        /* Emit the owed points evenly between the last point and the emitter, oldest first. */
        time = data->time + gFrameDelta;
        invCount = 1.0f / data->spawnCount;
        pos = data->lastPos;
        timeStep = time * invCount;
        width = data->width + data->widthGrowth * time;
        dx = trail->head.tmpl.pos.x - data->lastPos.x;
        dy = trail->head.tmpl.pos.y - data->lastPos.y;
        dz = trail->head.tmpl.pos.z - data->lastPos.z;
        dw = data->width - width;
        if (0) {} // matching: splits uopt basic blocks for the original register allocation
        vel.x = (trail->head.tmpl.pos.x - prevX) * gFrameDeltaInv;
        vel.y = 0.0f;
        vel.z = (trail->head.tmpl.pos.z - prevZ) * gFrameDeltaInv;
        moveX = dx * invCount;
        stepY = dy * invCount;
        stepZ = dz * invCount;
        stepWidth = dw * invCount;
        // matching: dead expressions shift IDO's TU-wide state so trail_drawMatrixTrail compiles as in the ROM
        dx * time;
        dy * time;
        dz * time;
        do {
            MatrixTrailPoint *point;

            point = &points[trail->head.end];
            point->pos.x = pos.x;
            point->pos.y = pos.y;
            point->pos.z = pos.z;
            point->vel = vel;
            point->texS = data->texS;
            point->width = width;
            point->life = data->life;
            point->delay = data->delay;
            data->texS += data->texStep + ((u32)math_randomU8() % (data->texStepRange + 1));
            point->alpha = 0xFF;
            point->vel.y += data->gravity * time;
            point->pos.x += point->vel.x * time;
            point->pos.y += point->vel.y * time;
            point->pos.z += point->vel.z * time;
            time -= timeStep;
            trail->head.end++;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                /* Full: drop the oldest point. */
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            pos.x += moveX;
            pos.y += stepY;
            pos.z += stepZ;
            width += stepWidth;
            data->spawnCount -= 1.0f;
        } while (data->spawnCount > 1.0f);
        data->lastPos = pos;
        data->time = time;
    }
    /* Move all points along with the owner. */
    {
        MatrixTrailPoint *point;

        i = trail->head.end;
        moveX = data->owner->xpos - data->ownerPos.x;
        moveY = data->owner->ypos - data->ownerPos.y;
        moveZ = data->owner->zpos - data->ownerPos.z;
        while (i != trail->head.start) {
            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            point = &points[i];
            point->pos.x += moveX;
            point->pos.y += moveY;
            point->pos.z += moveZ;
        }
        data->ownerPos.x = data->owner->xpos;
        data->ownerPos.y = data->owner->ypos;
        data->ownerPos.z = data->owner->zpos;
    }
    return 1;
}

/*
 * Draws a trail for view `view` as a strip of quads from the newest point to the oldest, each
 * point spread sideways by its width. Fades the head in and the tail out first if asked to.
 */
Gfx *trail_drawMatrixTrail(MatrixTrail *trail, Gfx *gfx, s16 view) {
    Vtx *vtx;
    MatrixTrailData *data;
    MatrixTrailPoint *points;
    Vec3f next;
    Vec3f cur;
    s32 j;
    s32 i;
    u8 texS;
    u8 curTexS;
    s16 alpha;
    s16 curAlpha;
    u8 update;
    f32 offZ;
    f32 offX;
    f32 curOffZ;
    f32 curOffX;
    s16 count;
    u8 fade;
    s32 k;

    if (trail->head.count >= 2) {
        particle_getViewBuffer(trail->vtxBuffers, view, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            data = trail->data;
            points = trail->points;
            update = 1;
            if (data->flags & 2) {
                u8 headFade;
                s16 headCount;

                headFade = 0;
                headCount = data->headFadeLen;
                k = trail->head.start;
                do {
                    points[k].alpha = headFade;
                    k++;
                    headFade += data->headFadeStep;
                    if (k == trail->head.tmpl.itemCount) {
                        k = 0;
                    }
                    headCount--;
                } while (headCount != 0 && k != trail->head.end);
            }
            if (data->flags & 4) {
                fade = 0;
                count = data->tailFadeLen;
                k = trail->head.end - 1;
                if (k < 0) {
                    k = trail->head.tmpl.itemCount - 1;
                }
                do {
                    points[k].alpha = (points[k].alpha * fade) >> 8;
                    k--;
                    fade += data->tailFadeStep;
                    count--;
                    if (k < 0) {
                        k = trail->head.tmpl.itemCount - 1;
                    }
                } while (count != 0 && k != trail->head.end);
            }
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[31], 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = math_gfxSetEnvColor(gfx, data->brightness, data->brightness, data->brightness, 0xFF, &update);
            gfx = particle_setCombineMode(gfx, 6, &update);
            gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
            gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, G_RM_AA_ZB_XLU_SURF | G_RM_AA_ZB_XLU_SURF2, &update);
            if (trail->head.tmpl.flags & 2) {
                /* Attached: the strip starts at the emitter. */
                j = trail->head.end - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = trail->head.tmpl.pos;
                curTexS = points[j].texS;
                curOffX = points[j].width * gCameraAngleCosines[view];
                curOffZ = points[j].width * gCameraAngleSines[view];
                curAlpha = (points[j].life * points[j].alpha) >> 8;
            } else {
                i = trail->head.end - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = points[i].pos;
                curOffX = points[i].width * gCameraAngleCosines[view];
                curTexS = points[i].texS;
                curOffZ = points[i].width * gCameraAngleSines[view];
                curAlpha = (points[i].life * points[i].alpha) >> 8;
            }
            next = points[j].pos;
            offX = points[j].width * gCameraAngleCosines[view];
            texS = points[j].texS;
            offZ = points[j].width * gCameraAngleSines[view];
            alpha = (points[j].life * points[j].alpha) >> 8;
            vtx->v.ob[0] = cur.x + curOffX;
            vtx->v.ob[1] = cur.y;
            vtx->v.ob[2] = cur.z - curOffZ;
            vtx->v.tc[0] = curTexS << 6;
            vtx->v.tc[1] = 31 << 6;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - curOffX;
            vtx->v.ob[1] = cur.y;
            vtx->v.ob[2] = cur.z + curOffZ;
            vtx->v.tc[0] = curTexS << 6;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                vtx->v.ob[0] = next.x + offX;
                vtx->v.ob[1] = next.y;
                vtx->v.ob[2] = next.z - offZ;
                vtx->v.tc[0] = texS << 6;
                vtx->v.tc[1] = 31 << 6;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - offX;
                vtx->v.ob[1] = next.y;
                vtx->v.ob[2] = next.z + offZ;
                vtx->v.tc[0] = texS << 6;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (curTexS < texS) {
                    /* The texture S wrapped: repeat the pair 256 texels back so the next quad continues it. */
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 256 << 6;
                    vtx++;
                    vtx[-2].v.tc[0] -= 256 << 6;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                if (i != trail->head.start) {
                    curTexS = texS;
                    cur = points[i].pos;
                    next = points[j].pos;
                    offX = points[j].width * gCameraAngleCosines[view];
                    texS = points[j].texS;
                    offZ = points[j].width * gCameraAngleSines[view];
                    alpha = (points[j].life * points[j].alpha) >> 8;
                }
            } while (i != trail->head.start);
        }
    }
    return gfx;
}

/* Trail event handler: stops the trail when its owner or kind is named, follows an owner swap. */
void trail_onMatrixTrailEvent(MatrixTrail *trail, TrailEvent *event, u8 eventId) {
    MatrixTrailData *data = trail->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            trail_stopMatrixTrail(trail);
        }
    } else if (eventId == 6) {
        if (data->specIndex == event->specIndex) {
            trail_stopMatrixTrail(trail);
        }
    } else if (eventId == 7) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            trail_stopMatrixTrail(trail);
        }
    } else if (eventId == 0x2D) {
        if (data->owner == event->swap.actorA) {
            data->owner = event->swap.actorB;
            data->ownerId = event->swap.idB;
        } else if (data->owner == event->swap.actorB) {
            data->owner = event->swap.actorA;
            data->ownerId = event->swap.idA;
        }
    }
}

/*
 * Transforms data->offset by the owner's matrix into out, lerped from (x, y, z) by t.
 * Returns 0 (out untouched) while the owner has no matrices.
 */
s32 trail_getAttachPoint(MatrixTrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t) {
    Vec3f pos;

    if (data->owner->ob_matrix_ptr == NULL) {
        return 0;
    }
    /* 64-byte matrices */
    math_transformOffset(&data->offset, &pos, &data->owner->ob_matrix_ptr[data->matrixIndex]);
    out->x = (pos.x - x) * t + x;
    out->y = (pos.y - y) * t + y;
    out->z = (pos.z - z) * t + z;
    return 1;
}

/* Restarts the kind 3 and 4 trails on owner and spawns a effect_newTimedEffectNoDraw object. */
void trail_restartTrailPair(Actor *owner, s16 lifetime, u8 arg2, s32 arg3) {
    trail_cancelTrailPair();
    trail_startMatrixTrail(owner, 3, 300, 0, arg2, arg3);
    trail_startMatrixTrail(owner, 4, 300, 0, arg2, arg3);
    effect_newTimedEffectNoDraw(lifetime, 6, -1, 1, 3, 0, arg2, arg3);
}

/* Trail task: once the owner has its matrices, starts the trail and finishes. */
void trail_updateStartTask(TrailTask *task) {
    TrailTaskData *data;
    u8 finished;
    MatrixTrail *trail;

    finished = FALSE;
    data = &task->data;
    if (task->data.owner->type == 0) {
        finished = TRUE;
    }
    if (data->owner->unique_id != data->ownerId) {
        finished = TRUE;
    }
    if (!finished) {
        if (data->owner->ob_matrix_ptr != NULL) {
            finished = TRUE;
            trail = trail_startMatrixTrail(data->owner, data->unk8, task->base.header.state, task->base.header.flags & 1, task->base.header.owner, task->base.header.kind);
            if (trail != NULL) {
                if (effect_findAttachmentByHandle(data->owner->attachments, task, NULL) != 0) {
                    effect_attach(trail, data->owner, 0x10);
                }
            }
        }
    }
    if (finished) {
        task->base.header.state = -1;
        task->base.header.flags |= 1;
    }
}

/* Trail task event handler: ends the task when its owner is named, follows an owner swap. */
void trail_onTaskEvent(TrailTask *task, TrailEvent *event, u8 eventId) {
    TrailTaskData *data = &task->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            effect_destroyObject(&task->base.header);
        }
    } else if (eventId == 0x2D) {
        if (data->owner == event->swap.actorA) {
            data->owner = event->swap.actorB;
            data->ownerId = event->swap.idB;
        } else if (data->owner == event->swap.actorB) {
            data->owner = event->swap.actorA;
            data->ownerId = event->swap.idA;
        }
    }
}

void trail_destroyOnEndEvent(EffectHeader *task, TrailEvent *event, u8 eventId) {
    if (eventId == 0 || eventId == 9) {
        effect_destroyObject(task);
    }
}

void trail_stopTrailPair(s32 unused) {
    trail_stopMatrixTrailsOfKind(3);
    trail_stopMatrixTrailsOfKind(4);
}

/* Stops the trails of kinds 3 and 4 and ends the pending trail tasks (event 9). */
void trail_cancelTrailPair(void) {
    trail_stopMatrixTrailsOfKind(3);
    trail_stopMatrixTrailsOfKind(4);
    effect_sendEventToEffectObjs(NULL, 9);
}

/* Stops the kind 3 and 4 trails and, unless one is pending, starts the trail_updateTrailPairTask task for owner. */
void trail_queueTrailPair(Actor *owner) {
    TrailTaskData taskData;

    trail_cancelTrailPair();
    if (gPendingTrailPairTask == NULL) {
        taskData.unk8 = 0;
        taskData.owner = owner;
        taskData.ownerId = owner->unique_id;
        gPendingTrailPairTask = effect_newTimedEffectNoDraw(300, -1, 9, 0, 4, 0xC, 0xFF, 0);
        if (gPendingTrailPairTask != NULL) {
            memcpy(&gPendingTrailPairTask->data, &taskData, sizeof(TrailTaskData));
        }
    }
}

/* Task of trail_queueTrailPair: ends when the owner leaves state 0x7C, starting trail_restartTrailPair if that took over 200 frames. */
void trail_updateTrailPairTask(TrailTask *task) {
    TrailTaskData *data;
    u8 finished;

    finished = FALSE;
    data = &task->data;
    if (task->data.owner->type == 0) {
        finished = TRUE;
    }
    if (data->owner->unique_id != data->ownerId) {
        finished = TRUE;
    }
    if (!finished) {
        data->unk8 += gFrameTicks;
        if (data->owner->animseq.uh != 0x7C) {
            finished = TRUE;
            if (data->unk8 > 200) {
                trail_restartTrailPair(data->owner, 300, task->base.header.owner, task->base.header.kind);
            }
        }
    }
    if (finished) {
        gPendingTrailPairTask = NULL;
        task->base.header.state = -1;
        task->base.header.flags |= 1;
    }
}

/* trail_onTaskEvent for the trail_queueTrailPair task, which also clears gPendingTrailPairTask. */
void trail_onTrailPairTaskEvent(TrailTask *task, TrailEvent *event, u8 eventId) {
    TrailTaskData *data = &task->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            effect_destroyObject(&task->base.header);
            gPendingTrailPairTask = NULL;
        // matching: an unused label ends the basic block here, as in the ROM
        skip:;
        }
    } else if (eventId == 0x2D) {
        if (data->owner == event->swap.actorA) {
            data->owner = event->swap.actorB;
            data->ownerId = event->swap.idB;
        } else if (data->owner == event->swap.actorB) {
            data->owner = event->swap.actorA;
            data->ownerId = event->swap.idA;
        }
    }
}

/* Trail task teardown (two variants): detaches it from its owner's attachments first. */
void trail_destroyTask(TrailTask *task) {
    effect_detachHandle(task, task->data.owner);
    effect_defaultDestroyTimedEffect(&task->base);
}

void trail_freeTask(TrailTask *task) {
    effect_detachHandle(task, task->data.owner);
    effect_defaultFreeTimedEffect(&task->base);
}

void trail_destroyTrailPairTask(TimedEffect *task) {
    gPendingTrailPairTask = NULL;
    effect_defaultDestroyTimedEffect(task);
}

void trail_freeTrailPairTask(TimedEffect *task) {
    gPendingTrailPairTask = NULL;
    effect_defaultFreeTimedEffect(task);
}
