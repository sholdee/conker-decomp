#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 (*D_8008AD90[])(ModelEffect *);
extern s32 (*D_8008ADA0[])(ModelEffect *);

/*
 * Spawns a model effect from `params`, with `extraSize` bytes of extension at 0x120. The object type
 * is 0x5B or 0x4C when params->flags has 0x80 or 0x10 set, else 0x36.
 */
ModelEffect *func_15157010(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize,
                      u8 owner, s32 list) {
    ModelEffect *fx;
    s32 type;
    s32 i;

    type = 0x36;
    if (params->flags & 0x80) {
        type = 0x5B;
    } else if (params->flags & 0x10) {
        type = 0x4C;
    }
    fx = func_15167A68(type, list, extraSize + 0x120, 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->params, params, sizeof(EffectParams));
    func_1503F62C(fx->params.model, fx->params.unkC, &fx->displayLists, &fx->displayListCount, &fx->segment1,
                  &fx->unk78, &fx->model);
    guMtxIdentF(fx->mtx[0]);
    guMtxIdentF(fx->mtx[1]);
    fx->model->matrices[0] = fx->mtx[0];
    fx->model->matrices[1] = fx->mtx[1];
    func_1503F5B8(fx->model, 1, arg1, arg2, 0.0f, 0);
    fx->attach.unk0 = attachArg;
    fx->unk118 = arg4;
    fx->attach.unk4 = 0;
    for (i = 0; i < 4; i++) {
        fx->attach.unk8[i] = 0;
    }
    fx->attach.unk18 = 0;
    if (attachArg) {
        for (i = 0; i <= D_80082FA0; i++) {
            fx->attach.unk8[i] = func_1515D480(attachArg);
        }
        fx->attach.unk18 = func_1515D440();
    }
    return fx;
}

/* Frees the attachment allocations made by func_15157010. */
void func_151571C4(ModelEffect *fx) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (fx->attach.unk8[i] != 0) {
            func_100043B4((s32 *)fx->attach.unk8[i], 4);
        }
    }
    if (fx->attach.unk18 != 0) {
        func_100043B4((s32 *)fx->attach.unk18, 4);
    }
}


/* Destroy handler: frees the attachments and model instance, then the object (func_15169804). */
void func_15157248(ModelEffect *fx) {
    func_151571C4(fx);
    func_1518CA04(fx->params.model);
    func_1503F7B8(fx->model);
    func_15169804(&fx->header);
}

/* As func_15157248, but releases the object through func_15169824. */
void func_1515728C(ModelEffect *fx) {
    func_151571C4(fx);
    func_1518CA04(fx->params.model);
    func_1503F7B8(fx->model);
    func_15169824(&fx->header);
}

/*
 * Update handler: counts the lifetime down (flag 1), runs both update callbacks and animates the
 * model. With flag 0x20 the alpha fades out over the last fadeStart ticks. The effect is destroyed
 * when its lifetime runs out or a callback returns 0.
 */
void func_151572D0(ModelEffect *fx) {
    s32 value; // matching: one local for both callback indices and the faded alpha
    u8 dead;
    s16 lifetime;

    dead = FALSE;
    if (fx->params.flags & 1) {
        fx->params.lifetime -= D_800BE9E4;
        if (fx->params.lifetime < 0) {
            dead = TRUE;
        }
    }
    if (!dead) {
        value = fx->params.updateFunc;
        if (value != -1) {
            if (D_8008AD90[value](fx) == 0) {
                dead = TRUE;
            }
        }
        value = fx->params.updateFunc2;
        if (value != -1) {
            if (D_8008ADA0[value](fx) == 0) {
                dead = TRUE;
            }
        }
    }
    if (!dead) {
        func_1503F4B0(fx->model);
    }
    if (!dead && (fx->params.flags & 0x20)) {
        lifetime = fx->params.lifetime;
        if (lifetime < fx->params.fadeStart) {
            value = lifetime * fx->params.fadeRate;
            if (value < fx->params.color0.alpha) {
                fx->params.color0.alpha = value;
            }
        }
    }
    if (dead) {
        func_1516972C(&fx->header);
    }
}

extern Gfx *(*D_8008ADBC[])(Gfx *, ModelEffect *, s16, u8 *, u8 *);
extern Gfx *(*D_8008ADCC[])(Gfx *, ModelEffect *, s16, u8 *);
extern RenderModePair D_800A4AC8[];

/*
 * Draw handler for `view`: runs the pre-draw callback (which may cancel the draw), honours the view
 * mask, sets up geometry mode, prim/env colours and other modes, binds segments 3 (model matrices),
 * 1 and 8 (opaque or translucent variant), then calls the model's display lists.
 */
Gfx *func_15157420(Gfx *gfx, ModelEffect *fx, s16 view) {
    s32 i;
    u8 needSync;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 proceed;

    needSync = TRUE;
    if (fx->params.preDrawFunc != -1) {
        gfx = D_8008ADBC[fx->params.preDrawFunc](gfx, fx, view, &needSync, &proceed);
        if (!proceed) {
            return gfx;
        }
    }
    if ((fx->params.flags & 8) && (fx->params.mask != NULL) && !(fx->params.mask->viewBits & (1 << view))) {
        return gfx;
    }
setup: // matching: a label here keeps as1 from filling the branch delay slots with the next load
    gfx = func_15142B7C(gfx, fx->params.render.setGeometryMode, fx->params.render.clearGeometryMode);
    func_151441A4(&primR, &primG, &primB, &primA, fx->params.color0.red, fx->params.color0.green, fx->params.color0.blue,
                  fx->params.color0.alpha, fx->params.color1.red, fx->params.color1.green, fx->params.color1.blue,
                  fx->params.color1.alpha, 0xFF, fx->params.render.primMode);
    func_151442FC(&envR, &envG, &envB, &envA, fx->params.color0.red, fx->params.color0.green, fx->params.color0.blue,
                  fx->params.color0.alpha, fx->params.color1.red, fx->params.color1.green, fx->params.color1.blue,
                  fx->params.color1.alpha, 0xFF, fx->params.render.envMode);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
    gfx = func_1513F4E4(gfx, ((u8 *)&fx->params.render.unk10)[3] /* matching: reads the low byte */, &needSync);
    gfx = func_15142FBC(gfx,
                        fx->params.render.unk0 | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | fx->params.render.unk14 |
                            fx->params.render.unk18,
                        D_800A4AC8[fx->params.render.renderMode].cycle1 | D_800A4AC8[fx->params.render.renderMode].cycle2, &needSync);
    if (fx->params.flags & 2) {
        gfx = func_151462C8(gfx, &fx->attach, fx->params.unk41, fx->params.owner, fx->params.ownerId, view,
                            &fx->params.pos, (fx->params.flags & 4) ? 1 : 0, 0);
    }
    gDPPipeSync(gfx++);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, fx->model->partMtx[D_800BE9C0]);
    gSPSegment(gfx++, 1, fx->segment1);
    if (fx->params.color0.alpha < 0xFF || fx->params.color1.alpha < 0xFF) {
        gSPSegment(gfx++, 8, (fx->params.flags & 0x40) ? D_80083A40 : D_80083740);
    } else {
        gSPSegment(gfx++, 8, (fx->params.flags & 0x40) ? D_80083BC0 : D_800838C0);
    }
    for (i = 0; i < fx->displayListCount; i++) {
        gSPDisplayList(gfx++, fx->displayLists[i]);
    }
    if (fx->params.postDrawFunc != -1) {
        gfx = D_8008ADCC[fx->params.postDrawFunc](gfx, fx, view, &needSync);
    }
    return gfx;
}

/* Update callback (D_8008ADA0[0]): resets this frame's root matrix to identity. */
s32 func_15157860(ModelEffect *fx) {
    guMtxIdentF(fx->mtx[D_800BE9C0]);
    return TRUE;
}

/* func_15157010 plus a 0x38-byte extension copied from `extra`. */
ModelEffect *func_15157898(EffectParams *params, EffectMotion *motion, s32 arg2, f32 arg3, s32 attachArg, s32 arg5,
                      s32 extraSize, u8 owner, s32 list) {
    ModelEffect *fx;

    fx = func_15157010(params, arg2, arg3, attachArg, arg5, extraSize + sizeof(EffectExtra), owner, list);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->extra, motion, sizeof(EffectExtra)); /* 4 bytes past an EffectMotion */
    return fx;
}

/* Update callback (D_8008ADA0[1]): root matrix = rotation extra.rot, uniform scale, translation pos. */
s32 func_15157918(ModelEffect *fx) {
    EffectMotion *motion;

    motion = &fx->extra.motion;
    func_150A8050(fx->mtx[D_800BE9C0], motion->rot.x, motion->rot.y, motion->rot.z);
    fx->mtx[D_800BE9C0][3][0] = fx->params.pos.x;
    fx->mtx[D_800BE9C0][3][1] = fx->params.pos.y;
    fx->mtx[D_800BE9C0][3][2] = fx->params.pos.z;
    fx->mtx[D_800BE9C0][0][0] *= motion->scale;
    fx->mtx[D_800BE9C0][0][1] *= motion->scale;
    fx->mtx[D_800BE9C0][0][2] *= motion->scale;
    fx->mtx[D_800BE9C0][1][0] *= motion->scale;
    fx->mtx[D_800BE9C0][1][1] *= motion->scale;
    fx->mtx[D_800BE9C0][1][2] *= motion->scale;
    fx->mtx[D_800BE9C0][2][0] *= motion->scale;
    fx->mtx[D_800BE9C0][2][1] *= motion->scale;
    fx->mtx[D_800BE9C0][2][2] *= motion->scale;
    return TRUE;
}

/*
 * Update callback (D_8008AD90[0]): integrates extra.motion. Drag is applied once per elapsed tick,
 * gravity accelerates vel.y, and pos advances by vel*t + a*t^2/2; rot advances by rotVel.
 */
s32 func_15157AA8(ModelEffect *fx) {
    EffectMotion *motion;
    Vec3f oldVel;
    f32 accelX;
    f32 accelY;
    f32 accelZ;
    s32 i;

    motion = &fx->extra.motion;
    if (motion->flags & 1) {
        oldVel = motion->vel;
        if (motion->flags & 8) {
            for (i = D_800BE9E4; i != 0; i--) {
                motion->vel.x *= motion->drag;
                motion->vel.z *= motion->drag;
            }
        }
        if (motion->flags & 4) {
            motion->vel.y += motion->gravity * D_800BE9A4;
            accelY = motion->gravity;
        } else {
            accelY = 0.0f;
        }
        accelX = (motion->vel.x - oldVel.x) * D_800BE9A8;
        accelZ = (motion->vel.z - oldVel.z) * D_800BE9A8;
        fx->params.pos.x += (oldVel.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        fx->params.pos.y += (oldVel.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        fx->params.pos.z += (oldVel.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
    }
    if (motion->flags & 2) {
        motion->rot.x += motion->rotVel.x * D_800BE9A4;
        motion->rot.y += motion->rotVel.y * D_800BE9A4;
        motion->rot.z += motion->rotVel.z * D_800BE9A4;
    }
    return TRUE;
}

/* Event handler: lets func_15169850 track or retarget this effect's owner. */
void func_15157D88(ModelEffect *fx, s32 event, u8 eventType) {
    func_15169850(event, eventType, &fx->params.owner, &fx->params.ownerId, fx);
}

/* Update callback (D_8008ADA8[3]): root matrix from the extension, see func_15157DEC. */
s32 func_15157DC8(ModelEffect *fx) {
    func_15157DEC(fx, fx->extra.raw);
    return TRUE;
}

/*
 * Sets this frame's root matrix to rotation xform[0..2], translated to pos, with the X and Z
 * axes scaled by xform[3] and the Y axis by xform[4].
 */
void func_15157DEC(ModelEffect *fx, f32 *xform) {
    func_150A8050(fx->mtx[D_800BE9C0], xform[0], xform[1], xform[2]);
    fx->mtx[D_800BE9C0][3][0] = fx->params.pos.x;
    fx->mtx[D_800BE9C0][3][1] = fx->params.pos.y;
    fx->mtx[D_800BE9C0][3][2] = fx->params.pos.z;
    fx->mtx[D_800BE9C0][0][0] *= xform[3];
    fx->mtx[D_800BE9C0][0][1] *= xform[3];
    fx->mtx[D_800BE9C0][0][2] *= xform[3];
    fx->mtx[D_800BE9C0][1][0] *= xform[4];
    fx->mtx[D_800BE9C0][1][1] *= xform[4];
    fx->mtx[D_800BE9C0][1][2] *= xform[4];
    fx->mtx[D_800BE9C0][2][0] *= xform[3];
    fx->mtx[D_800BE9C0][2][1] *= xform[3];
    fx->mtx[D_800BE9C0][2][2] *= xform[3];
}


/* Pre-draw callback (D_8008ADBC[3]): loads the identity modelview and this view's projection. */
Gfx *func_15157F80(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed) {
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_800DCC10[view], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    *proceed = TRUE;
    return gfx;
}

/* Post-draw callback (D_8008ADCC[0]): reloads this view's projection matrices for the current frame. */
Gfx *func_15157FE8(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync) {
    gSPMatrix(gfx++, &D_800BE628[view].projectionMtx[D_800BE9C0],
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, D_800DC2A0[D_800BE9C0] + view, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    return gfx;
}


/* Sends an event to every model effect. */
void func_15158078(s32 event, u8 eventType) {
    func_15169260(D_800A6060, 3, event, eventType);
}
