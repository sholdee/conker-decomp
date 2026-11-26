#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include <libc/stddef.h>

/* User data of the model spawned by effect_spawnWarpModel. */
typedef struct {
    /* 0x00 */ f32 scrollS;
    /* 0x04 */ f32 scrollT;
    /* 0x08 */ f32 scrollTPhase;
    /* 0x0C */ f32 scalePhaseS;
    /* 0x10 */ f32 scalePhaseT;
} TexWarpState;

typedef struct {
    s32 start;
    s32 count;
} VtxRange;

/* Render setup and reset display lists of these models; their entry in the object type table points to them. */
Gfx gDynModelRenderSetupDl[] = {
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetColorDither(G_CD_NOISE),
    gsDPSetAlphaDither(G_AD_NOISE),
    gsSPMatrix(&gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gDynModelRenderResetDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Update callbacks (DynModelDesc.updateFunc): 0 frees the model. */
static s32 (*gDynModelUpdateFuncs[])(DynModel *) = { func_15108AB4, func_15108C38, effect_updateWarpModel, effect_updateModelSway };
/* Draw callbacks (DynModelDesc.drawFunc): 0 skips the draw. func_15108D24 and effect_buildSwayMatrix take the viewport as
 * an s32. */
static s32 (*gDynModelDrawFuncs[])(DynModel *, s16) = {
    effect_setDynModelIdentityMtx, func_15108D24, effect_skipDynModelDraw, effect_projectOwnerModelTexCoords, func_15109120, effect_warpModelTexCoords, effect_buildSwayMatrix,
};
/* Destroy and free handlers per model kind. */
static void (*gDynModelDestroyFuncs[])(DynModel *) = { effect_defaultDestroyDynModel, effect_defaultDestroyDynModel, effect_defaultDestroyDynModel, effect_defaultDestroyDynModel };
static void (*gDynModelFreeFuncs[])(DynModel *) = { effect_defaultFreeDynModel, effect_defaultFreeDynModel, effect_defaultFreeDynModel, effect_defaultFreeDynModel };

/* Centres of the nine vertex groups of the warping model (effect_warpModelTexCoords). */
static const Vec3f gWarpModelGroupCentres[9] = {
    { -2098.139f, 1487.8055f, -2924.709f },
    { -4264.652f, 1487.8055f, -1274.1654f },
    { -3964.0457f, 1487.8055f, 1661.5109f },
    { -1169.4988f, 1487.8055f, 3118.5642f },
    { 1248.1896f, 1487.8055f, 3113.1545f },
    { 3550.895f, 1487.8055f, 2111.7466f },
    { 3954.7395f, 1487.8055f, -1754.5244f },
    { 2220.1824f, 1487.8055f, -2979.1628f },
    { -26.41471f, 1261.8988f, -402.8878f },
};
/* The vertices of each group. */
static const VtxRange gWarpModelGroupVertices[9] = {
    { 0, 12 }, { 12, 16 }, { 28, 16 }, { 44, 16 }, { 60, 16 }, { 76, 16 }, { 92, 16 }, { 108, 16 }, { 124, 16 },
};

/* Loads the descriptor's model and allocates a DynModel holding copies of its vertices. */
DynModel *effect_newDynModel(DynModelDesc *desc, u8 arg1, s32 userDataSize, u8 arg3, s32 arg4) {
    DynModel *obj;
    ModelAsset *asset;
    s32 buffersSize;
    s32 vtxSize;
    u8 *buffer;
    u32 loaded;
    s32 count;
    s32 i;

    asset = rom_loadPathTable(&loaded, 0, &count, 2, desc->unk30, desc->unk34);
    {
        s32 type;

        vtxSize = (u8 *)asset->displayList - (u8 *)asset->vertices;
        buffersSize = vtxSize * 2 * (gScreenLayout + 1);
        if (arg1) {
            type = 0x54;
        } else {
            type = 0x38;
        }
        obj = effect_newObject(type, arg4, userDataSize + buffersSize + offsetof(DynModel, buffers), 2, arg3, 1);
        if (obj == NULL) {
            return NULL;
        }
        memcpy(&obj->desc, desc, sizeof(DynModelDesc));
        obj->asset = asset;
    }
    for (i = 0, buffer = obj->buffers; i < gScreenLayout + 1; i++) {
        obj->vertices[0][i] = (Vtx *)buffer;
        obj->vertices[1][i] = (Vtx *)(buffer + vtxSize);
        memcpy(buffer, asset->vertices, vtxSize);
        memcpy(obj->vertices[1][i], asset->vertices, vtxSize);
        // matching: the unsigned (size_t) step lets IDO reuse vtxSize * 2 from above
        buffer += vtxSize * 2 * sizeof(u8);
    }
    obj->displayList = asset->displayList;
    obj->buffersSize = buffersSize;
    texture_resolveDl(obj->displayList, 0, 1, 0x3E, 0);
    return obj;
}

/* Per-frame update: counts down the lifetime, runs the update callback, frees when done. */
void effect_updateDynModel(DynModel *obj) {
    s32 func;
    u8 done = FALSE;

    if (obj->desc.flags & DYNMODEL_FLAG_LIFETIME) {
        obj->desc.lifetime -= D_800BE9E4;
        if (obj->desc.lifetime < 0) {
            done = TRUE;
        }
    }
    if (!done) {
        func = obj->desc.updateFunc;
        if (func != -1) {
            if (gDynModelUpdateFuncs[func](obj) == 0) {
                done = TRUE;
            }
        }
    }
    if (done) {
        effect_destroyObject(&obj->header);
    }
}

/* Draws the model in one viewport, after the draw callback (if any) has updated it. */
Gfx *effect_drawDynModel(Gfx *gfx, DynModel *obj, s16 viewport) {
    s32 func;

    if ((obj->desc.flags & DYNMODEL_FLAG_VIEWPORT_MASK) && !(obj->desc.viewportMask & (1 << viewport))) {
        return gfx;
    }

    func = obj->desc.drawFunc;
    if (func != -1) {
        if (gDynModelDrawFuncs[func](obj, viewport) == 0) {
            return gfx;
        }
    }

    gSPMatrix(gfx++, &obj->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gfx++, 1, obj->vertices[D_800BE9C0][viewport]);
    gSPDisplayList(gfx++, obj->displayList);

    return gfx;
}

s32 effect_setDynModelIdentityMtx(DynModel *obj, s16 viewport) {
    math_mtxIdentity(&obj->mtx[D_800BE9C0]);
    return 1;
}

void effect_defaultDestroyDynModel(DynModel *obj) {
    heap_setTag(obj->asset, 4);
    effect_defaultDestroy(&obj->header);
}

void effect_defaultFreeDynModel(DynModel *obj) {
    heap_setTag(obj->asset, 4);
    effect_defaultFree(&obj->header);
}

void effect_destroyDynModel(DynModel *obj) {
    gDynModelDestroyFuncs[obj->desc.kind](obj);
}

void effect_freeDynModel(DynModel *obj) {
    gDynModelFreeFuncs[obj->desc.kind](obj);
}

void effect_dynModelMessage(DynModel *obj, s32 arg1, u8 arg2) {
    switch (obj->desc.kind) {
    case 1:
        light_handleLampMessage(obj, arg1, arg2);
        break;
    case 2:
        effect_ownerModelMessage(obj, arg1, arg2);
        break;
    }
}

s32 effect_skipDynModelDraw(DynModel *obj, s16 viewport) {
    return 0;
}

/* Spawns the scrolling, warping texture model (update effect_updateWarpModel, draw effect_warpModelTexCoords). */
DynModel *effect_spawnWarpModel(void) {
    DynModel *obj;
    DynModelDesc desc;
    TexWarpState state;

    desc.updateFunc = 2;
    desc.drawFunc = 5;
    desc.lifetime = 300;
    desc.unk30 = 9;
    state.scrollS = 0.0f;
    state.scrollT = 0.0f;
    state.scalePhaseS = 0.0f;
    state.scalePhaseT = 0.0f;
    state.scrollTPhase = 0.0f;
    desc.flags = 0;
    desc.unk34 = 0x1AE;
    desc.unk8 = 1;
    desc.unkC = 0x220205;
    desc.unk10 = 0x40600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.kind = 3;

    obj = effect_newDynModel(&desc, 1, sizeof(TexWarpState), 0xFF, 1);
    if (obj != NULL) {
        if (obj->buffersSize != 0x1180) {
            effect_destroyObject(&obj->header);
        } else {
            memcpy(obj->buffers + obj->buffersSize, &state, sizeof(TexWarpState));
        }
    }
    return obj;
}

/* Draw callback: recomputes each vertex group's texture coordinates relative to the viewport position. */
s32 effect_warpModelTexCoords(DynModel *obj, s16 viewport) {
    TexWarpState *state;
    f32 scaleS;
    f32 scaleT;
    f32 offsetS;
    f32 offsetT;
    s32 k;
    Vec3f *viewPos;
    f32 dirX;
    f32 dirZ;
    s32 j;

    effect_setDynModelIdentityMtx(obj, viewport);
    state = (TexWarpState *)(obj->buffers + obj->buffersSize);
    scaleS = sinf(state->scalePhaseS) * 1.210000038f + 2.854000092f;
    scaleT = sinf(state->scalePhaseT) * 1.994000077f + 8.328000069f;
    viewPos = math_getCameraPos(viewport);
    offsetS = state->scrollS - (s32)(state->scrollS * (1.0f / 1024.0f)) * 1024.0f;
    offsetT = state->scrollT - (s32)(state->scrollT * (1.0f / 1024.0f)) * 1024.0f;
    for (k = 0; k < 9; k++) {
        f32 dx;
        f32 dz;

        dx = gWarpModelGroupCentres[k].x - viewPos->x;
        dz = gWarpModelGroupCentres[k].z - viewPos->z;
        if (dx != 0.0f || dz != 0.0f) {
            f32 invLen;

            invLen = 1.0f / sqrtf(dx * dx + dz * dz);
            dirX = dz * invLen;
            dirZ = -dx * invLen;
        } else {
            dirX = 1.0f;
            dirZ = 0.0f;
        }
        for (j = gWarpModelGroupVertices[k].start; j < gWarpModelGroupVertices[k].start + gWarpModelGroupVertices[k].count; j++) {
            Vtx *vtx;
            f32 x;
            f32 y;
            f32 z;
            f32 ex;
            f32 ey;
            f32 ez;

            vtx = &obj->vertices[D_800BE9C0][viewport][j];
            x = vtx->v.ob[0];
            y = vtx->v.ob[1];
            z = vtx->v.ob[2];
            ex = x - gWarpModelGroupCentres[k].x;
            ey = y - gWarpModelGroupCentres[k].y;
            ez = z - gWarpModelGroupCentres[k].z;
            vtx->v.tc[0] = (ex * dirX + ez * dirZ) * scaleS - offsetS;
            vtx->v.tc[1] = ey * scaleT - offsetT;
        }
    }
    return 1;
}

/* Update callback: advances the texture scroll and warp phases, wrapping the scroll to [0, 4096). */
s32 effect_updateWarpModel(DynModel *obj) {
    TexWarpState *state;

    state = (TexWarpState *)(obj->buffers + obj->buffersSize);
    state->scrollTPhase += 0.03700000048f * gFrameDelta;
    state->scrollTPhase = math_wrapRadians(state->scrollTPhase);
    state->scrollS += 1.899999976f * gFrameDelta;
    state->scrollT = sinf(state->scrollTPhase) * 77.30000305f;
    state->scalePhaseS += 0.01900000125f * gFrameDelta;
    state->scalePhaseT += 0.01000000071f * gFrameDelta;
    state->scalePhaseS = math_wrapRadians(state->scalePhaseS);
    state->scalePhaseT = math_wrapRadians(state->scalePhaseT);

    while (state->scrollS > 4096.0f) {
        state->scrollS -= 4096.0f;
    }
    while (state->scrollS < 0.0f) {
        state->scrollS += 4096.0f;
    }
    while (state->scrollT > 4096.0f) {
        state->scrollT -= 4096.0f;
    }
    while (state->scrollT < 0.0f) {
        state->scrollT += 4096.0f;
    }
    return 1;
}
