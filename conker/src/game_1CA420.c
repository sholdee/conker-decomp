#include <ultra64.h>
// matching: func_150ADA20 returns a u32 random number, but this file was built against a u8-returning
// prototype (callers use the low byte without masking it), so it keeps that view.
#define func_150ADA20 func_150ADA20_functions_h
#include "functions.h"
#undef func_150ADA20
u8 func_150ADA20(void);
#include "variables.h"
#include "effects.h"

/*
 * Ribbon trails attached to a point of an object (a game object's matrix plus an offset).
 * A trail keeps its points in a ring buffer, emits new points at the attachment point at a
 * fixed rate, ages them and draws them as a textured strip. Trails are started either
 * directly (func_1519D030) or by a task that waits until the owner has its matrices.
 *
 * Frame timing globals: D_800BE9A4 is the frame time step, D_800BE9A8 its reciprocal and
 * D_800BE9E4 the number of frames elapsed.
 */

/* Trail parameters, one entry per trail kind (D_800A8B80). */
typedef struct TrailSpec {
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 spawnRate;
    /* 0x08 */ u8 brightness;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 matrixIndex;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ Vec3f offset;
    /* 0x20 */ s16 life;
    /* 0x22 */ s16 delay;
    /* 0x24 */ s16 lifeDecay;
    /* 0x26 */ u8 pad26[2];
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

extern TrailSpec D_800A8B80[];

/* A trail's own data (0x60 bytes), at Trail::data. */
typedef struct TrailData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;      /* owner's unique_id when the trail started */
    /* 0x05 */ u8 matrixIndex;  /* which of the owner's matrices the trail follows */
    /* 0x06 */ u8 flags;        /* bit 0: stopped, bit 1: fade in the head, bit 2: fade out the tail */
    /* 0x07 */ u8 texS;         /* texture S of the next point */
    /* 0x08 */ Vec3f offset;    /* attachment point, in the matrix's space */
    /* 0x14 */ Vec3f lastPos;   /* where the last point was emitted */
    /* 0x20 */ f32 time;        /* age of the last emitted point */
    /* 0x24 */ f32 gravity;
    /* 0x28 */ f32 spawnRate;
    /* 0x2C */ f32 spawnCount;  /* points owed, accumulated from spawnRate */
    /* 0x30 */ f32 drag;
    /* 0x34 */ f32 widthGrowth;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 follow;      /* how far the emitter moves towards the attachment point per frame */
    /* 0x40 */ u8 brightness;
    /* 0x41 */ u8 pad41;
    /* 0x42 */ s16 life;
    /* 0x44 */ s16 delay;
    /* 0x46 */ s16 lifeDecay;
    /* 0x48 */ s16 texStep;
    /* 0x4A */ s16 texStepRange;
    /* 0x4C */ u8 specIndex;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ Vec3f ownerPos;  /* owner position the points were last moved with */
    /* 0x5C */ u8 headFadeLen;
    /* 0x5D */ u8 headFadeStep;
    /* 0x5E */ u8 tailFadeLen;
    /* 0x5F */ u8 tailFadeStep;
} TrailData;

/* One point of a trail (0x24 bytes). */
typedef struct TrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 width;
    /* 0x1C */ s16 life;   /* counts down by lifeDecay once delay ran out; dead below 0 */
    /* 0x1E */ s16 delay;
    /* 0x20 */ u8 alpha;
    /* 0x21 */ u8 texS;
    /* 0x22 */ u8 pad22[2];
} TrailPoint;

/* A trail object made by func_15147A80. */
typedef struct Trail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuffers[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailData *data;
} Trail;

/* Data of a trail task (func_151491F4), at TrailTask::data. */
typedef struct TrailTaskData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ s32 unk8;  /* spec index for func_1519E464, a frame timer for func_1519E754 */
} TrailTaskData;

typedef struct TrailTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailTaskData data;
} TrailTask;

/* Event payloads the handlers below get (see func_15147D64). */
typedef union TrailEvent {
    ActorRef object;    /* events 0 and 7: trails of this owner stop */
    u8 specIndex;       /* event 6: trails of this kind stop (func_1519CF70) */
    ActorSwapMsg swap;  /* event 0x2D: owners actorA and actorB trade places */
} TrailEvent;

extern void func_15147D64(void *event, u8 eventId);
extern /* Restarts the kind 3 and 4 trails on owner and spawns a func_151491F4 object. */
void func_1519E3BC(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
extern Trail *func_1519D030(Actor *owner, s32 specIndex, s16 lifetime, u8 arg3, u8 arg4, s32 arg5);
extern void func_1519E688(void);
extern Trail *func_15147A80(EffectTemplate *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_1519E304(TrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t);

/* Stops every trail of kind specIndex (event 6). */
void func_1519CF70(u8 specIndex) {
    u8 event[4];

    event[0] = specIndex;
    func_15147D64(event, 6);
}

/* Stops a trail: it no longer emits and its tail fades out. */
void func_1519CFA0(Trail *trail) {
    TrailData *data;

    trail->head.updateKind2 = 0;
    data = trail->data;
    trail->head.tmpl.flags &= ~2;
    data->flags |= 1;
    data->flags |= 4;
}

extern void func_151478F4(Trail *);

/* Trail teardown (two variants): detaches it from its owner's attachments first. */
void func_1519CFD0(Trail *trail) {
    TrailData *data;

    data = trail->data;
    func_1514EDF0(trail, data->owner);
    func_151478F4(trail);
}

extern void func_15147928(Trail *);

void func_1519D000(Trail *trail) {
    TrailData *data;

    data = trail->data;
    func_1514EDF0(trail, data->owner);
    func_15147928(trail);
}

/* Starts a trail of kind specIndex on owner. Returns it, or NULL. */
Trail *func_1519D030(Actor *owner, s32 specIndex, s16 lifetime, u8 arg3, u8 arg4, s32 arg5) {
    EffectTemplate spawn;
    TrailData data;
    Trail *trail;
    TrailSpec *spec;

    spec = &D_800A8B80[specIndex];

    if (owner == NULL) {
        return NULL;
    }

    data.time = 0.0f;
    data.ownerPos.x = owner->x_position;
    data.ownerPos.y = owner->y_position;
    data.ownerPos.z = owner->z_position;
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

    if (func_1519E304(&data, &spawn.pos, 0.0f, 0.0f, 0.0f, 1.0f) != 0) {
        data.lastPos = spawn.pos;
        spawn.flags |= 4;
    }

    spawn.kind = 4;
    trail = func_15147A80(&spawn, sizeof(TrailData), sizeof(TrailPoint), 3, 3, 3, 0, 0, 0, arg4, arg5);
    if (trail != NULL) {
        memcpy(trail->data, &data, sizeof(TrailData));
    }
    return trail;
}


/* Ages a trail's points (newest to oldest) and drops the dead ones. Returns 0 once a stopped trail is gone. */
s32 func_1519D240(Trail *trail) {
    TrailData *data;
    TrailPoint *points;
    s32 i;
    s32 offset;
    s16 delay;
    TrailPoint *point;
    TrailPoint *same;

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
        offset = i * sizeof(TrailPoint);
        point = &points[i];
        point->alpha = 0xFF;
        // matching: a second, commuted address of the same point (points + offset vs offset + points) is not CSE'd
        same = (TrailPoint *)((u8 *)points + offset);
        delay = point->delay;
        point->vel.x *= data->drag;
        point->vel.y += data->gravity * D_800BE9A4;
        point->vel.z *= data->drag;
        point->pos.x += same->vel.x * D_800BE9A4;
        point->pos.y += same->vel.y * D_800BE9A4;
        point->pos.z += same->vel.z * D_800BE9A4;
        if (delay > 0) {
            point->delay = delay - D_800BE9E4;
        } else {
            point->life -= D_800BE9E4 * data->lifeDecay;
        }
        point->width += D_800BE9A4 * data->widthGrowth;
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
 * and hands over to a task (func_1519E464 family). Returns 0 when the owner is gone.
 */
s32 func_1519D454(Trail *trail) {
    TrailData *data;
    TrailPoint *points;
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
    if (data->owner->interaction_state == 0) {
        return 0;
    }
    if (data->owner->unique_id != data->ownerId) {
        return 0;
    }
    started = 0;
    if (!(trail->head.tmpl.flags & 4)) {
        if (func_1519E304(data, &trail->head.tmpl.pos, 0.0f, 0.0f, 0.0f, 1.0f) != 0) {
            started = 1;
            data->lastPos = trail->head.tmpl.pos;
            trail->head.tmpl.flags |= 4;
        } else {
            return 1;
        }
    }
    prevX = trail->head.tmpl.pos.x;
    prevZ = trail->head.tmpl.pos.z;
    if (!started && func_1519E304(data, &trail->head.tmpl.pos, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, data->follow) == 0) {
        func_1519CFA0(trail);
        taskData.owner = data->owner;
        taskData.ownerId = data->owner->unique_id;
        taskData.unk8 = data->specIndex;
        task = (TrailTask *)func_151491F4(trail->head.tmpl.life, -1, 8, (trail->head.tmpl.flags & 1) ? 1 : 0, 2, 0xC, 0xFF, 0);
        if (task != NULL) {
            memcpy(&task->data, &taskData, sizeof(TrailTaskData));
            if (func_1514ED3C(data->owner->attachments, trail, NULL) != 0) {
                func_1514EC1C(task, data->owner, 0x11);
            }
        }
        return 1;
    }
    data->spawnCount += data->spawnRate * D_800BE9A4;
    if (data->spawnCount > 1.0f) {
        /* Emit the owed points evenly between the last point and the emitter, oldest first. */
        time = data->time + D_800BE9A4;
        invCount = 1.0f / data->spawnCount;
        pos = data->lastPos;
        timeStep = time * invCount;
        width = data->width + data->widthGrowth * time;
        dx = trail->head.tmpl.pos.x - data->lastPos.x;
        dy = trail->head.tmpl.pos.y - data->lastPos.y;
        dz = trail->head.tmpl.pos.z - data->lastPos.z;
        dw = data->width - width;
        if (0) {} // matching: splits uopt basic blocks for the original register allocation
        vel.x = (trail->head.tmpl.pos.x - prevX) * D_800BE9A8;
        vel.y = 0.0f;
        vel.z = (trail->head.tmpl.pos.z - prevZ) * D_800BE9A8;
        moveX = dx * invCount;
        stepY = dy * invCount;
        stepZ = dz * invCount;
        stepWidth = dw * invCount;
        // matching: dead expressions shift IDO's TU-wide state so func_1519D9F4 compiles as in the ROM
        dx * time;
        dy * time;
        dz * time;
        do {
            TrailPoint *point;

            point = &points[trail->head.end];
            point->pos.x = pos.x;
            point->pos.y = pos.y;
            point->pos.z = pos.z;
            point->vel = vel;
            point->texS = data->texS;
            point->width = width;
            point->life = data->life;
            point->delay = data->delay;
            data->texS += data->texStep + ((u32)func_150ADA20() % (data->texStepRange + 1));
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
        TrailPoint *point;

        i = trail->head.end;
        moveX = data->owner->x_position - data->ownerPos.x;
        moveY = data->owner->y_position - data->ownerPos.y;
        moveZ = data->owner->z_position - data->ownerPos.z;
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
        data->ownerPos.x = data->owner->x_position;
        data->ownerPos.y = data->owner->y_position;
        data->ownerPos.z = data->owner->z_position;
    }
    return 1;
}

void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

/*
 * Draws a trail for view `view` as a strip of quads from the newest point to the oldest, each
 * point spread sideways by its width. Fades the head in and the tail out first if asked to.
 */
Gfx *func_1519D9F4(Trail *trail, Gfx *gfx, s16 view) {
    Vtx *vtx;
    TrailData *data;
    TrailPoint *points;
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
        func_151D5D60(trail->vtxBuffers, view, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
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
            gfx = func_15142E24(gfx, D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, data->brightness, data->brightness, data->brightness, 0xFF, &update);
            gfx = func_1513F4E4(gfx, 6, &update);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, G_RM_AA_ZB_XLU_SURF | G_RM_AA_ZB_XLU_SURF2, &update);
            if (trail->head.tmpl.flags & 2) {
                /* Attached: the strip starts at the emitter. */
                j = trail->head.end - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = trail->head.tmpl.pos;
                curTexS = points[j].texS;
                curOffX = points[j].width * D_800DD1E8[view];
                curOffZ = points[j].width * D_800DD1D8[view];
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
                curOffX = points[i].width * D_800DD1E8[view];
                curTexS = points[i].texS;
                curOffZ = points[i].width * D_800DD1D8[view];
                curAlpha = (points[i].life * points[i].alpha) >> 8;
            }
            next = points[j].pos;
            offX = points[j].width * D_800DD1E8[view];
            texS = points[j].texS;
            offZ = points[j].width * D_800DD1D8[view];
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
                    offX = points[j].width * D_800DD1E8[view];
                    texS = points[j].texS;
                    offZ = points[j].width * D_800DD1D8[view];
                    alpha = (points[j].life * points[j].alpha) >> 8;
                }
            } while (i != trail->head.start);
        }
    }
    return gfx;
}

/* Trail event handler: stops the trail when its owner or kind is named, follows an owner swap. */
void func_1519E1F4(Trail *trail, TrailEvent *event, u8 eventId) {
    TrailData *data = trail->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            func_1519CFA0(trail);
        }
    } else if (eventId == 6) {
        if (data->specIndex == event->specIndex) {
            func_1519CFA0(trail);
        }
    } else if (eventId == 7) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            func_1519CFA0(trail);
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

extern void func_15143134(Vec3f *in, Vec3f *out, void *mtx);

/*
 * Transforms data->offset by the owner's matrix into out, lerped from (x, y, z) by t.
 * Returns 0 (out untouched) while the owner has no matrices.
 */
s32 func_1519E304(TrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t) {
    Vec3f pos;

    if (data->owner->unk1D4 == NULL) {
        return 0;
    }
    /* 64-byte matrices */
    func_15143134(&data->offset, &pos, (u8 *)data->owner->unk1D4 + (data->matrixIndex << 6));
    out->x = (pos.x - x) * t + x;
    out->y = (pos.y - y) * t + y;
    out->z = (pos.z - z) * t + z;
    return 1;
}

/* Restarts the kind 3 and 4 trails on owner and spawns a func_151491F4 object. */
void func_1519E3BC(Actor *owner, s16 lifetime, u8 arg2, s32 arg3) {
    func_1519E688();
    func_1519D030(owner, 3, 300, 0, arg2, arg3);
    func_1519D030(owner, 4, 300, 0, arg2, arg3);
    func_151491F4(lifetime, 6, -1, 1, 3, 0, arg2, arg3);
}

/* Trail task: once the owner has its matrices, starts the trail and finishes. */
void func_1519E464(TrailTask *task) {
    TrailTaskData *data;
    u8 finished;
    Trail *trail;

    finished = FALSE;
    data = &task->data;
    if (task->data.owner->interaction_state == 0) {
        finished = TRUE;
    }
    if (data->owner->unique_id != data->ownerId) {
        finished = TRUE;
    }
    if (!finished) {
        if (data->owner->unk1D4 != NULL) {
            finished = TRUE;
            trail = func_1519D030(data->owner, data->unk8, task->base.header.state, task->base.header.flags & 1, task->base.header.owner, task->base.header.kind);
            if (trail != NULL) {
                if (func_1514ED3C(data->owner->attachments, task, NULL) != 0) {
                    func_1514EC1C(trail, data->owner, 0x10);
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
void func_1519E570(TrailTask *task, TrailEvent *event, u8 eventId) {
    TrailTaskData *data = &task->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            func_1516972C(&task->base.header);
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

void func_1519E61C(EffectHeader *task, TrailEvent *event, u8 eventId) {
    if (eventId == 0 || eventId == 9) {
        func_1516972C(task);
    }
}

void func_1519E65C(s32 unused) {
    func_1519CF70(3);
    func_1519CF70(4);
}

/* Stops the trails of kinds 3 and 4 and ends the pending trail tasks (event 9). */
void func_1519E688(void) {
    func_1519CF70(3);
    func_1519CF70(4);
    func_15147D64(NULL, 9);
}

/* Stops the kind 3 and 4 trails and, unless one is pending, starts the func_1519E754 task for owner. */
void func_1519E6BC(Actor *owner) {
    TrailTaskData taskData;

    func_1519E688();
    if (D_800E0920 == NULL) {
        taskData.unk8 = 0;
        taskData.owner = owner;
        taskData.ownerId = owner->unique_id;
        D_800E0920 = (TrailTask *)func_151491F4(300, -1, 9, 0, 4, 0xC, 0xFF, 0);
        if (D_800E0920 != NULL) {
            memcpy(&D_800E0920->data, &taskData, sizeof(TrailTaskData));
        }
    }
}

/* Task of func_1519E6BC: ends when the owner leaves state 0x7C, starting func_1519E3BC if that took over 200 frames. */
void func_1519E754(TrailTask *task) {
    TrailTaskData *data;
    u8 finished;

    finished = FALSE;
    data = &task->data;
    if (task->data.owner->interaction_state == 0) {
        finished = TRUE;
    }
    if (data->owner->unique_id != data->ownerId) {
        finished = TRUE;
    }
    if (!finished) {
        data->unk8 += D_800BE9E4;
        if (data->owner->unk84.uh != 0x7C) {
            finished = TRUE;
            if (data->unk8 > 200) {
                func_1519E3BC(data->owner, 300, task->base.header.owner, task->base.header.kind);
            }
        }
    }
    if (finished) {
        D_800E0920 = NULL;
        task->base.header.state = -1;
        task->base.header.flags |= 1;
    }
}

/* func_1519E570 for the func_1519E6BC task, which also clears D_800E0920. */
void func_1519E818(TrailTask *task, TrailEvent *event, u8 eventId) {
    TrailTaskData *data = &task->data;

    if (eventId == 0) {
        if ((event->object.actor == data->owner) || (event->object.uniqueId == data->ownerId)) {
            func_1516972C(&task->base.header);
            D_800E0920 = NULL;
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
void func_1519E8CC(TrailTask *task) {
    func_1514EDF0(task, task->data.owner);
    func_1514933C(&task->base);
}

void func_1519E8F8(TrailTask *task) {
    func_1514EDF0(task, task->data.owner);
    func_15149368(&task->base);
}

void func_1519E924(TimedEffect *task) {
    D_800E0920 = NULL;
    func_1514933C(task);
}

void func_1519E948(TimedEffect *task) {
    D_800E0920 = NULL;
    func_15149368(task);
}
