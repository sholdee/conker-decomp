#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Creates a trail object from a spawn description, motion block and render settings. Returns the MotionTrail
 * as void *: callers whose params extend TrailParams (extraSize bytes) keep it in their own view. */
void *trail_createMotionTrail(EffectTemplate *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4, s32 finishFn,
                    s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10, TrailRender *render,
                    CollisionHit *hit, u8 arg13, s32 arg14) {
    MotionTrail *trail;
    TrailParams *params;

    spawn->kind = 1;
    trail = effect_newEffectObj(spawn, extraSize + sizeof(TrailParams), sizeof(MotionTrailPoint), 1, 0, 1, arg9, arg10,
                          hit, arg13, arg14);
    if (trail == NULL) {
        return NULL;
    }
    params = trail->params;
    memcpy(&params->motion, motion, sizeof(TrailMotion));
    params->state = state;
    params->unk21 = arg4;
    params->finishFn = finishFn;
    params->hitFn = hitFn;
    params->hitKind3Fn = hitKind3Fn;
    params->unk25 = arg8;
    params->render = *render;
    return trail;
}

/*
 * This file's .data: the handler tables of the trails, by the indices in TrailParams. The update and finish
 * handlers are declared without a prototype: each trail kind keeps its own view of the object.
 */
typedef s32 (*TrailFunc)();
/* Update handlers (TrailParams.state, then .unk21); 0 ends the trail. */
static TrailFunc gMotionTrailStateProcs[6] = {
    NULL, trail_moveAndAppendPoint, trail_movePoints, trail_movePointsGroundTest, trail_dropNewestPoint, trail_updateEmittedTrail,
};
static TrailFunc gMotionTrailUpdateProcs[13] = {
    NULL,          trail_fadeEmittedTrail, func_150AED9C, trail_fadeMotionTrail, particle_fadeMotionTrailByLife, trail_fadeKind2Trail, trail_fadeIn,
    func_150B378C, func_151AC3CC, effect_fadeTrailByLife, trail_landSprayTrail, func_150BDB3C, func_1514B844,
};
/* Finish handlers (TrailParams.finishFn); there are none. */
static TrailFunc gMotionTrailFinishProcs[1] = { NULL };
/*
 * Ground hit handlers (TrailParams.hitFn, and .hitKind3Fn for a hit of source 3), given the trail, the oldest
 * point's last position, the ground height and the surface: 0 ends the trail. They take floats, so they keep a
 * prototype.
 */
typedef s32 (*TrailHitFunc)(MotionTrail *, f32, f32, f32, f32, CollisionTri *);
static TrailHitFunc D_8008A430[8] = {
    NULL,          func_15148EF8, trail_spawnHitPuff, func_150DDFAC, func_150DE12C, func_150B37C8, func_151AC408,
    particle_splashBurst,
};
static TrailHitFunc D_8008A450[5] = {
    NULL, trail_spawnHitPuffAndEffect, func_151AC550, func_15189FD0, particle_splashSpray,
};

/* Runs the trail's update handlers and fades; returns whether it is still alive. */
s8 trail_updateMotionTrail(MotionTrail *trail) {
    TrailParams *params;
    s8 done;
    MotionTrailPoint *points;
    s16 unk1C;
    s32 fade;
    s32 ret;

    params = trail->params;
    done = 0;
    /* The two update handlers are called without arguments. */
    if (params->state != 0) {
        if (gMotionTrailStateProcs[params->state]() == 0) {
            done = 1;
        }
    }
    if (params->unk21 != 0 && !done) {
        if (gMotionTrailUpdateProcs[params->unk21]() == 0) {
            done = 1;
        }
    }
    if ((params->motion.flags & 0x40) && !done) {
        unk1C = trail->head.tmpl.life;
        if (unk1C < params->motion.unk1C) {
            fade = unk1C * params->motion.unk1E;
            if (fade < params->motion.alpha) {
                params->motion.alpha = fade;
            }
        }
    }
    ret = !done;
    if (done) {
        if (params->finishFn != 0) {
            done = gMotionTrailFinishProcs[params->finishFn](trail); // matching: dead store of the result
        }
    }
    points = trail->points;
    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    return ret;
}

extern struct {
    s32 unk0;
    s32 unk4;
} gEffectRenderModes[];

/* Draws the trail as a camera-facing ribbon of quads, newest point first. */
Gfx *trail_drawMotionTrail(MotionTrail *trail, Gfx *gfx, s16 arg2) {
    TrailParams *params;
    Vtx *vtx;
    u8 update;
    Vec3f next;
    Vec3f cur;
    s32 i;
    s32 j;
    Vec3f *cam;
    MotionTrailPoint *points;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    f32 segX;
    f32 segY;
    f32 segZ;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 len;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s32 tex;

    params = trail->params;
    if (trail->head.count >= 2) {
        particle_getViewBuffer(trail->buffers, arg2, (trail->head.tmpl.itemCount - 1) * 2 * sizeof(Vtx), &vtx, 0);
        if (vtx == NULL) {
            return gfx;
        }
        if (vtx != NULL) { // matching: redundant re-test of vtx
            if (D_800BE9C0 != 0) {
                vtx += (trail->head.tmpl.itemCount - 1) * 2;
            }
            update = 1;
            cam = math_getCameraPos(arg2);
            points = trail->points;
            tex = math_getTextureFrameId(0, params->motion.texture, 0, 2);
            if (tex != D_800DD1B0) {
                gTrailTexture.addr = D_80091564[params->motion.texture];
                gfx = sprite_loadTexture(gfx, &gTrailTexture, 0, 0, 0, 0, 2, 0x100, 0x100, (params->motion.flags & 0x80) ? 0x3E : 3);
                D_800DD1B0 = tex;
            }
            math_pickPrimColour(&envR, &envG, &envB, &envA, 0, 0, 0, 0, 0, 0, 0, params->motion.alpha, params->motion.shade, params->render.primMode);
            math_pickEnvColour(&primR, &primG, &primB, &primA, 0, 0, 0, 0, 0, 0, 0, params->motion.alpha, params->motion.shade, params->render.envMode);
            gfx = math_gfxUpdateGeometryMode(gfx, params->render.setGeometryMode, params->render.clearGeometryMode);
            gfx = math_gfxSetEnvColor(gfx, primR, primG, primB, primA, &update);
            gfx = math_gfxSetPrimColor(gfx, 0, 0, envR, envG, envB, envA, &update);
            gfx = particle_setCombineMode(gfx, ((u8 *)&params->render.unk10)[3] /* matching: reads the low byte */, &update);
            gfx = math_gfxSetOtherMode(gfx, params->render.unk0 | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | params->render.unk14 | params->render.unk18, gEffectRenderModes[params->render.renderMode].unk0 | gEffectRenderModes[params->render.renderMode].unk4, &update);
            i = trail->head.end - 1;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            j = i - 1;
            if (j < 0) {
                j = trail->head.tmpl.itemCount - 1;
            }
            cur = points[i].pos;
            next = points[j].pos;
            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            toCamX = cur.x - cam->x;
            toCamY = cur.y - cam->y;
            toCamZ = cur.z - cam->z;
            sideX = (segY * toCamZ) - (toCamY * segZ);
            sideY = (segZ * toCamX) - (toCamZ * segX);
            sideZ = (segX * toCamY) - (toCamX * segY);
            len = sqrtf((sideX * sideX) + (sideY * sideY) + (sideZ * sideZ));
            if (len == 0.0f) {
                sideX = 0.0f;
                sideY = 0.0f;
                sideZ = 0.0f;
            } else {
                f32 scale;
                scale = params->motion.halfWidth / len;
                sideX *= scale;
                sideY *= scale;
                sideZ *= scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[1] = points[i].phase + 0x4000;
            vtx->v.tc[0] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[1] = points[i].phase + 0x4000;
            vtx->v.tc[0] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            do {
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                toCamX = next.x - cam->x;
                toCamY = next.y - cam->y;
                toCamZ = next.z - cam->z;
                sideX = (segY * toCamZ) - (toCamY * segZ);
                sideY = (segZ * toCamX) - (toCamZ * segX);
                sideZ = (segX * toCamY) - (toCamX * segY);
                len = sqrtf((sideX * sideX) + (sideY * sideY) + (sideZ * sideZ));
                if (len == 0.0f) {
                    sideX = 0.0f;
                    sideY = 0.0f;
                    sideZ = 0.0f;
                } else {
                    f32 scale;
                    scale = params->motion.halfWidth / len;
                    sideX *= scale;
                    sideY *= scale;
                    sideZ *= scale;
                }
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[1] = points[j].phase + 0x4000;
                vtx->v.tc[0] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0xFF;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[1] = points[j].phase + 0x4000;
                vtx->v.tc[0] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0xFF;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i = j;
                j--;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = points[i].pos;
                next = points[j].pos;
            } while (i != trail->head.start);
        }
    }
    return gfx;
}

/* Moves every point by the trail's velocity and gravity, then appends the current position. */
s32 trail_moveAndAppendPoint(MotionTrail *trail) {
    MotionTrailPoint *points;
    TrailParams *params;
    s32 phase;
    struct {
        u16 step;
    } interval; // matching: a struct puts step at offset 0 of its stack slot (a bare u16 sits at 2)
    s32 index;
    s32 count;
    s32 phaseIndex;

    points = trail->points;
    params = trail->params;
    index = trail->head.start;
    while (index != trail->head.end) {
        points[index].velocityY -= params->motion.gravity * gFrameDelta;
        points[index].pos.x += params->motion.velocity.x * gFrameDelta;
        points[index].pos.y += points[index].velocityY * gFrameDelta;
        points[index].pos.z += params->motion.velocity.z * gFrameDelta;
        index++;
        if (index == trail->head.tmpl.itemCount) {
            index = 0;
        }
    }
    count = trail->head.count;
    if (count < trail->head.tmpl.itemCount - 1) {
        phase = (params->motion.flags & 0x20) ? 0x1000 : 0;
        if (count != 0) {
            interval.step = 0x1000 / count;
        }
        trail->head.count = count + 1;
        points[trail->head.end].pos = trail->head.tmpl.pos;
        points[trail->head.end].velocityY = params->motion.velocity.y;
        trail->head.end++;
        if (trail->head.end == trail->head.tmpl.itemCount) {
            trail->head.end = 0;
        }
        phaseIndex = trail->head.start;
        while (phaseIndex != trail->head.end) {
            points[phaseIndex].phase = phase;
            if (params->motion.flags & 0x20) {
                phase = (phase - interval.step) & 0xFFFF;
            } else {
                phase += interval.step;
                phase &= 0xFFFF;
            }
            phaseIndex++;
            if (phaseIndex == trail->head.tmpl.itemCount) {
                phaseIndex = 0;
            }
        }
    } else if (params->motion.flags & 0x17) {
        params->state = 3;
    } else {
        params->state = 2;
    }
    return 1;
}

/* Moves every point by the trail's velocity and gravity, newest to oldest. */
s32 trail_movePoints(MotionTrail *trail) {
    s32 idx;
    MotionTrailPoint *points;
    TrailParams *params;

    points = trail->points;
    params = trail->params;
    idx = trail->head.end;
    do {
        idx--;
        if (idx < 0) {
            idx = trail->head.tmpl.itemCount - 1;
        }
        points[idx].velocityY -= params->motion.gravity * gFrameDelta;
        points[idx].pos.x += params->motion.velocity.x * gFrameDelta;
        points[idx].pos.y += points[idx].velocityY * gFrameDelta;
        points[idx].pos.z += params->motion.velocity.z * gFrameDelta;
    } while (idx != trail->head.start);
    return 1;
}

/* Like trail_movePoints, then tests the oldest point's fall against the ground and runs a hit handler. */
s32 trail_movePointsGroundTest(MotionTrail *trail) {
    s32 index;
    TrailParams *params;
    MotionTrailPoint *points;
    Vec3f oldPosition;
    Vec3f position;
    u8 callback;

    params = trail->params;
    points = trail->points;
    index = trail->head.end;
    if (params->motion.flags & 7) {
        oldPosition = points[trail->head.start].pos;
    }
    do {
        index--;
        if (index < 0) {
            index = trail->head.tmpl.itemCount - 1;
        }
        points[index].velocityY -= params->motion.gravity * gFrameDelta;
        points[index].pos.x += params->motion.velocity.x * gFrameDelta;
        points[index].pos.y += points[index].velocityY * gFrameDelta;
        points[index].pos.z += params->motion.velocity.z * gFrameDelta;
    } while (index != trail->head.start);
    if (params->motion.flags & 0x17) {
        if (points[trail->head.start].pos.y < oldPosition.y) {
            position.x = points[trail->head.start].pos.x;
            position.y = oldPosition.y;
            position.z = points[trail->head.start].pos.z;
            if (collision_probeFloorSurfABSet0Cached(&position, 0, points[trail->head.start].pos.y,
                              &trail->collision) != 0) {
                if (trail->collision.source == 3) {
                    callback = params->hitKind3Fn;
                    if (callback != 0 &&
                        D_8008A450[callback](trail, oldPosition.x, oldPosition.y,
                                             oldPosition.z, trail->collision.height,
                                             &trail->collision.tri) == 0) {
                        return 0;
                    }
                } else {
                    callback = params->hitFn;
                    if (callback != 0 &&
                        D_8008A430[callback](trail, oldPosition.x, oldPosition.y,
                                             oldPosition.z, trail->collision.height,
                                             &trail->collision.tri) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

/* Drops the newest point and respaces the remaining points' phases; fails below three points. */
s32 trail_dropNewestPoint(MotionTrail *trail) {
    s32 idx;
    u16 step;
    u16 phase;
    TrailParams *params;
    MotionTrailPoint *points;

    if (trail->head.count >= 3) {
        trail->head.count--;
        points = trail->points;
        params = trail->params;
        idx = trail->head.start;
        step = 0x1000 / trail->head.count;
        phase = (params->motion.flags & 0x20) ? 0x1000 : 0;
        trail->head.end--;
        if (trail->head.end < 0) {
            trail->head.end = trail->head.tmpl.itemCount - 1;
        }
        while (idx != trail->head.end) {
            points[idx].phase = phase;
            if (params->motion.flags & 0x20) {
                phase -= step;
            } else {
                phase += step;
            }
            idx++;
            if (idx == trail->head.tmpl.itemCount) {
                idx = 0;
            }
        }
        return 1;
    }
    return 0;
}

/* Handler: switches the trail to state 4. */
s32 func_15148EF8(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3) {
    TrailParams *params = trail->params;

    params->state = 4;
    return 1;
}

/* Spawns a trail at (x, y, z), launched at speed along the direction given by the yaw and pitch angles. */
void trail_launchMotionTrail(u8 texture, f32 x, f32 y, f32 z, s32 arg4, s32 yaw, s32 pitch, f32 speed, s16 arg8,
                   f32 halfWidth, f32 gravity, u8 arg11) {
    f32 yawWaveQ;
    EffectTemplate18 spawn;
    f32 horizontal;
    TrailMotion1C motion;
    f32 pitchWave;
    f32 pitchWaveQ;
    f32 yawWave;
    u8 tex;
    TrailRender render;

    /* math_cosBinAngleMasked is a 256-step periodic lookup; "Q" values are a quarter turn (0x40) back. */
    pitchWave = math_cosBinAngleMasked(pitch);
    pitchWaveQ = math_cosBinAngleMasked(pitch - 0x40);
    yawWave = math_cosBinAngleMasked(yaw);
    yawWaveQ = math_cosBinAngleMasked(yaw - 0x40);

    spawn.kind = 1;
    spawn.flags = 1;
    motion.shade = 0xFF;
    motion.flags = 8;
    tex = texture; // matching: the u8 copy owns the stack slot above render
    motion.texture = tex;
    if (tex == 10) {
        motion.flags = 0x28;
    }

    horizontal = speed * pitchWave;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    motion.halfWidth = halfWidth;
    motion.gravity = gravity;
    motion.velocity.x = horizontal * yawWaveQ;
    motion.velocity.y = -speed * pitchWaveQ;
    motion.velocity.z = horizontal * yawWave;
    motion.alpha = 0xFF;
    spawn.itemCount = arg4 + 3;
    spawn.life = arg8;

    render.unk0 = 0;
    render.setGeometryMode = 1;
    render.clearGeometryMode = 0x160600;
    render.renderMode = 3;
    render.unk10 = 0x10;
    render.unk14 = 0x80;
    render.unk18 = 0x20;
    render.primMode = 0;
    render.envMode = 9;
    trail_createMotionTrail(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &render, 0, arg11, 1);
}

/* Fades the trail in: alpha = 8 per unit of unk1C, capped at 255. */
s32 trail_fadeIn(MotionTrail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->head.tmpl.life * 8;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    if ((u8)alpha < 0) { // matching: never true, but IDO still emits the test
        return 0;
    }
    return 1;
}

/* Hands the trail to effect_defaultDestroyEffectObj. */
void trail_destroyEffectObj(EffectObj *obj) {
    effect_defaultDestroyEffectObj(obj);
}
