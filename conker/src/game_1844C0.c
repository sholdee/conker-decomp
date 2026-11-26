#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Each view's orthographic projection (view_buildOrthoMatrices of game/View/ViewOrtho/viewortho builds them). */
Mtx gViewOrthoMtxs[4];

/* The model effect types, for effect_sendEventToModelEffects's event. */
const s32 gModelEffectTypes[3] = { 0x36, 0x4C, 0x5B };

/* The model effect type's render setup and teardown display lists (object type table). */
Gfx gModelEffectRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gModelEffectRenderTeardownDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Update callbacks (EffectParams.updateFunc, .updateFunc2, the third set): each returns 0 when the effect
   is done. Declared without a prototype where the entries take their own effect views. */
s32 (*gModelEffectUpdateFuncs[])() = { effect_moveModelEffect, func_151BECB8, particle_bobEffect, effect_updateFadeInEffect };
s32 (*gModelEffectUpdate2Funcs[])(ModelEffect *) = { effect_resetModelEffectRoot, effect_setModelEffectRootRotScale };
s32 (*D_8008ADA8[])() = { func_151BEB20, func_151D6E60, func_150FB188, effect_setModelEffectRootFromExtra, func_150C52CC };

/* Pre-draw and post-draw callbacks (EffectParams.preDrawFunc, .postDrawFunc). */
Gfx *(*gModelEffectPreDrawFuncs[])(Gfx *, ModelEffect *, s32, u8 *, u8 *) = {
    func_151BEC94, func_151D710C, func_150FB1E8, effect_preDrawModelEffectOrtho,
};
Gfx *(*gModelEffectPostDrawFuncs[])(Gfx *, ModelEffect *, s32, u8 *) = { effect_postDrawModelEffectProjection };

/*
 * Spawns a model effect from `params`, with `extraSize` bytes of extension at 0x120. The object type
 * is 0x5B or 0x4C when params->flags has 0x80 or 0x10 set, else 0x36.
 */
ModelEffect *effect_newModelEffect(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize,
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
    fx = effect_newObject(type, list, extraSize + 0x120, 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->params, params, sizeof(EffectParams));
    anim_createInstance(fx->params.model, fx->params.unkC, &fx->displayLists, &fx->displayListCount, &fx->segment1,
                  &fx->unk78, &fx->model);
    guMtxIdentF(fx->mtx[0]);
    guMtxIdentF(fx->mtx[1]);
    fx->model->matrices[0] = fx->mtx[0];
    fx->model->matrices[1] = fx->mtx[1];
    anim_play(fx->model, 1, arg1, arg2, 0.0f, 0);
    fx->attach.model = attachArg;
    fx->attach.unk1C = arg4;
    fx->attach.unk4 = 0;
    for (i = 0; i < 4; i++) {
        fx->attach.handles[i] = NULL;
    }
    fx->attach.sharedHandle = NULL;
    if (attachArg) {
        for (i = 0; i <= gScreenLayout; i++) {
            fx->attach.handles[i] = light_allocGfxLights(attachArg);
        }
        fx->attach.sharedHandle = func_1515D440();
    }
    return fx;
}

/* Frees the attachment's light buffers made by effect_newModelEffect. */
void effect_freeModelEffectLights(ModelEffect *fx) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= gScreenLayout; i++) {
        if (fx->attach.handles[i] != NULL) {
            heap_setTag((void *)(s32)fx->attach.handles[i], 4);
        }
    }
    if (fx->attach.sharedHandle != NULL) {
        heap_setTag((void *)(s32)fx->attach.sharedHandle, 4);
    }
}


/* Destroy handler: frees the attachments and model instance, then the object (effect_defaultDestroy). */
void effect_destroyModelEffect(ModelEffect *fx) {
    effect_freeModelEffectLights(fx);
    attach_releaseModel(fx->params.model);
    anim_freeInstance(fx->model);
    effect_defaultDestroy(&fx->header);
}

/* As effect_destroyModelEffect, but releases the object through effect_defaultFree. */
void effect_freeModelEffect(ModelEffect *fx) {
    effect_freeModelEffectLights(fx);
    attach_releaseModel(fx->params.model);
    anim_freeInstance(fx->model);
    effect_defaultFree(&fx->header);
}

/*
 * Update handler: counts the lifetime down (flag 1), runs both update callbacks and animates the
 * model. With flag 0x20 the alpha fades out over the last fadeStart ticks. The effect is destroyed
 * when its lifetime runs out or a callback returns 0.
 */
void effect_updateModelEffect(ModelEffect *fx) {
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
            if (gModelEffectUpdateFuncs[value](fx) == 0) {
                dead = TRUE;
            }
        }
        value = fx->params.updateFunc2;
        if (value != -1) {
            if (gModelEffectUpdate2Funcs[value](fx) == 0) {
                dead = TRUE;
            }
        }
    }
    if (!dead) {
        anim_updateInstance(fx->model);
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
        effect_destroyObject(&fx->header);
    }
}

extern RenderModePair gEffectRenderModes[];

/*
 * Draw handler for `view`: runs the pre-draw callback (which may cancel the draw), honours the view
 * mask, sets up geometry mode, prim/env colours and other modes, binds segments 3 (model matrices),
 * 1 and 8 (opaque or translucent variant), then calls the model's display lists.
 */
Gfx *effect_drawModelEffect(Gfx *gfx, ModelEffect *fx, s16 view) {
    s32 i;
    u8 needSync;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 proceed;

    needSync = TRUE;
    if (fx->params.preDrawFunc != -1) {
        gfx = gModelEffectPreDrawFuncs[fx->params.preDrawFunc](gfx, fx, view, &needSync, &proceed);
        if (!proceed) {
            return gfx;
        }
    }
    if ((fx->params.flags & 8) && (fx->params.mask != NULL) && !(fx->params.mask->views & (1 << view))) {
        return gfx;
    }
setup: // matching: a label here keeps as1 from filling the branch delay slots with the next load
    gfx = math_gfxUpdateGeometryMode(gfx, fx->params.render.setGeometryMode, fx->params.render.clearGeometryMode);
    math_pickPrimColour(&primR, &primG, &primB, &primA, fx->params.color0.red, fx->params.color0.green, fx->params.color0.blue,
                  fx->params.color0.alpha, fx->params.color1.red, fx->params.color1.green, fx->params.color1.blue,
                  fx->params.color1.alpha, 0xFF, fx->params.render.primMode);
    math_pickEnvColour(&envR, &envG, &envB, &envA, fx->params.color0.red, fx->params.color0.green, fx->params.color0.blue,
                  fx->params.color0.alpha, fx->params.color1.red, fx->params.color1.green, fx->params.color1.blue,
                  fx->params.color1.alpha, 0xFF, fx->params.render.envMode);
    gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &needSync);
    gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &needSync);
    gfx = particle_setCombineMode(gfx, ((u8 *)&fx->params.render.unk10)[3] /* matching: reads the low byte */, &needSync);
    gfx = math_gfxSetOtherMode(gfx,
                        fx->params.render.unk0 | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | fx->params.render.unk14 |
                            fx->params.render.unk18,
                        gEffectRenderModes[fx->params.render.renderMode].cycle1 | gEffectRenderModes[fx->params.render.renderMode].cycle2, &needSync);
    if (fx->params.flags & 2) {
        gfx = math_emitPointLights(gfx, &fx->attach, fx->params.unk41, fx->params.owner, fx->params.ownerId, view,
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
        gfx = gModelEffectPostDrawFuncs[fx->params.postDrawFunc](gfx, fx, view, &needSync);
    }
    return gfx;
}

/* Update callback (gModelEffectUpdate2Funcs[0]): resets this frame's root matrix to identity. */
s32 effect_resetModelEffectRoot(ModelEffect *fx) {
    guMtxIdentF(fx->mtx[D_800BE9C0]);
    return TRUE;
}

/* effect_newModelEffect plus a 0x38-byte extension copied from `extra`. */
ModelEffect *effect_newMovingModelEffect(EffectParams *params, EffectMotion *motion, s32 arg2, f32 arg3, s32 attachArg, s32 arg5,
                      s32 extraSize, u8 owner, s32 list) {
    ModelEffect *fx;

    fx = effect_newModelEffect(params, arg2, arg3, attachArg, arg5, extraSize + sizeof(EffectExtra), owner, list);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->extra, motion, sizeof(EffectExtra)); /* 4 bytes past an EffectMotion */
    return fx;
}

/* Update callback (gModelEffectUpdate2Funcs[1]): root matrix = rotation extra.rot, uniform scale, translation pos. */
s32 effect_setModelEffectRootRotScale(ModelEffect *fx) {
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
 * Update callback (gModelEffectUpdateFuncs[0]): integrates extra.motion. Drag is applied once per elapsed tick,
 * gravity accelerates vel.y, and pos advances by vel*t + a*t^2/2; rot advances by rotVel.
 */
s32 effect_moveModelEffect(ModelEffect *fx) {
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
            motion->vel.y += motion->gravity * gFrameDelta;
            accelY = motion->gravity;
        } else {
            accelY = 0.0f;
        }
        accelX = (motion->vel.x - oldVel.x) * gFrameDeltaInv;
        accelZ = (motion->vel.z - oldVel.z) * gFrameDeltaInv;
        fx->params.pos.x += (oldVel.x + 0.5f * accelX * gFrameDelta) * gFrameDelta;
        fx->params.pos.y += (oldVel.y + 0.5f * accelY * gFrameDelta) * gFrameDelta;
        fx->params.pos.z += (oldVel.z + 0.5f * accelZ * gFrameDelta) * gFrameDelta;
    }
    if (motion->flags & 2) {
        motion->rot.x += motion->rotVel.x * gFrameDelta;
        motion->rot.y += motion->rotVel.y * gFrameDelta;
        motion->rot.z += motion->rotVel.z * gFrameDelta;
    }
    return TRUE;
}

/* Event handler: lets effect_handleOwnerActorEvent track or retarget this effect's owner. */
void effect_modelEffectMessage(ModelEffect *fx, s32 event, u8 eventType) {
    effect_handleOwnerActorEvent(event, eventType, &fx->params.owner, &fx->params.ownerId, fx);
}

/* Update callback (D_8008ADA8[3]): root matrix from the extension, see effect_setModelEffectRootXform. */
s32 effect_setModelEffectRootFromExtra(ModelEffect *fx) {
    effect_setModelEffectRootXform(fx, fx->extra.raw);
    return TRUE;
}

/*
 * Sets this frame's root matrix to rotation xform[0..2], translated to pos, with the X and Z
 * axes scaled by xform[3] and the Y axis by xform[4].
 */
void effect_setModelEffectRootXform(ModelEffect *fx, f32 *xform) {
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


/* Pre-draw callback (gModelEffectPreDrawFuncs[3]): loads the identity modelview and this view's projection. */
Gfx *effect_preDrawModelEffectOrtho(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed) {
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &gViewOrthoMtxs[view], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    *proceed = TRUE;
    return gfx;
}

/* Post-draw callback (gModelEffectPostDrawFuncs[0]): reloads this view's projection matrices for the current frame. */
Gfx *effect_postDrawModelEffectProjection(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync) {
    gSPMatrix(gfx++, &gViews[view].projectionMtx[D_800BE9C0],
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, gViewMtxBuffers[D_800BE9C0] + view, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    return gfx;
}


/* Sends an event to every model effect. */
void effect_sendEventToModelEffects(s32 event, u8 eventType) {
    effect_sendEventToTypes(gModelEffectTypes, 3, event, eventType);
}
