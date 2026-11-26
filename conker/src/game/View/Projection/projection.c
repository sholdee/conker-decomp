#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
Vec3f gFovChanges[4];     // FOV changes view_recordFovChange recorded per view: x = scale (-1.0f: none), y/z = deltas
u8 gFovChangePending;           // set while a gFovChanges record is pending
ViewClip gViewClips[4];
u8 gClipChangePending;           // set while a gViewClips record is pending
f32 gDefaultFar;          // default far plane
f32 gDefaultNear;          // default near plane
static Mtx gView0Projection;   // projection spanning view 0

#include "functions.h"
#include "variables.h"

/* The rest of this unit's .bss: conker/view.h declares these before the definitions, so they follow gView0Projection. */
u8 gTintPrimColors[4][3];     // per tint slot: prim colour (game/Texture/TextureLoad/textureload adds the positive part of its wave)
u8 gTintEnvColors[4][3];     // per tint slot: env colour (the negative part)
u8 D_800D9B84[3];
u8 D_800D9B88[3];
u8 D_800D9B8B;
u8 D_800D9B8C;
LookAt gLookAts[2];          // per framebuffer
Ambient gViewAmbients[4][2];      // per view and framebuffer
MtxF gViewMatrices[4];            // per view: view matrix
MtxF gViewProjMatrices[4];            // per view: view matrix * projection
PointLightGfx *gViewGfxLights[4];  // per view
u8 gViewGfxLightMax;
u8 D_800D9E21;                 // light count byte light_pickLights stored for player 0
PointLight *gPickedLights[14];    // point lights light_pickLights picked this frame

/* Loads the default near/far planes from gFogNear/gFogFar and builds the matrix gView0Projection spanning
 * view 0 (+-halfWidth by +-halfHeight, depth 1..10000). */
void view_initDefaultProjection(s32 arg0) {
    gDefaultNear = gFogNear;
    gDefaultFar = gFogFar;
    D_800D3670 = 100.0f - gDefaultFar;
    func_151EF954(&gView0Projection, -gViews->halfWidth, gViews->halfWidth, -gViews->halfHeight,
                  gViews->halfHeight, 1.0f, 10000.0f, 1.0f);
}

/* Widens a view's frustum FOVs by (fovXDelta, fovYDelta) plus the global gViewFovOffset, updates unk1C/unk20 and
 * rebuilds its projection. The near plane is gNearPlaneOverride, else `near`, else the default gDefaultNear. */
void view_widenFov(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale, f32 near) {
    f32 nearPlane;
    f32 farPlane;
    f32 pad; // matching: keeps the stack frame size

    gViews[viewIndex].frustumFovX = gViews[viewIndex].fovX + fovXDelta + gViewFovOffset;
    gViews[viewIndex].frustumFovY = gViews[viewIndex].fovY + fovYDelta + gViewFovOffset;
    {
        f32 dist;
        f32 cosine;
        f32 angle;

        /* half the FOV in radians; dist = halfWidth / tan(angle) */
        angle = gViews[viewIndex].frustumFovX * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * gViews[viewIndex].halfWidth) / sinf(angle);
        gViews[viewIndex].unk1C =
            (gViews[viewIndex].scaleX * dist) * gViews[viewIndex].pixelTanX * scale;
        angle = gViews[viewIndex].frustumFovY * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * gViews[viewIndex].halfHeight) / sinf(angle);
        gViews[viewIndex].unk20 =
            (gViews[viewIndex].scaleY * dist) * gViews[viewIndex].pixelTanY * scale;
    }
    if (gNearPlaneOverride != 0.0f) {
        nearPlane = gNearPlaneOverride;
    } else if (near != 0.0f) {
        nearPlane = near;
    } else {
        nearPlane = gDefaultNear;
    }
    if (gFarPlaneOverride != 0.0f) {
        farPlane = gFarPlaneOverride;
    } else {
        farPlane = gDefaultFar;
    }
    view_buildProjection(viewIndex, &gViews[viewIndex].perspNorm, gViews[viewIndex].frustumFovY,
                  gViews[viewIndex].frustumFovX, nearPlane, farPlane, scale);
}

/* view_widenFov with the default near plane, also remembered in gFovChanges (unk0 = scale, unk4/unk8 = FOV
 * deltas) so view_reapplyFovChanges can re-apply it. */
void view_recordFovChange(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale) {
    view_widenFov(viewIndex, fovXDelta, fovYDelta, scale, 0.0f);
    gFovChanges[viewIndex].x = scale;
    gFovChanges[viewIndex].y = fovXDelta;
    gFovChanges[viewIndex].z = fovYDelta;
    gFovChangePending = TRUE;
}

/* Re-applies every FOV change view_recordFovChange recorded (scale -1.0f marks none) and clears the records. */
void view_reapplyFovChanges(void) {
    s32 i;

    for (i = 0; i <= gScreenLayout; i++) {
        if (gFovChanges[i].x != -1.0f) {
            view_widenFov(i, gFovChanges[i].y, gFovChanges[i].z, gFovChanges[i].x, 0.0f);
            gFovChanges[i].x = -1.0f;
        }
    }
    gFovChangePending = FALSE;
}

/* Rebuilds a view's projection with new near/far planes (0.0f picks the defaults gDefaultNear/gDefaultFar) and
 * remembers them in gViewClips for view_reapplyClipPlanes. */
void view_setClipPlanes(s32 viewIndex, f32 near, f32 far) {
    if (near == 0.0f) {
        near = gDefaultNear;
    }
    if (far == 0.0f) {
        far = gDefaultFar;
    }
    view_buildProjection(viewIndex, &gViews[viewIndex].perspNorm, gViews[viewIndex].frustumFovY,
                  gViews[viewIndex].frustumFovX, near, far, gViews[viewIndex].perspScale);
    gViewClips[viewIndex].near = near;
    gViewClips[viewIndex].far = far;
    gClipChangePending = TRUE;
}

/* Re-applies every near/far change view_setClipPlanes recorded (near -1.0f marks none) and clears the records. */
void view_reapplyClipPlanes(void) {
    s32 i;

    for (i = 0; i <= gScreenLayout; i++) {
        if (gViewClips[i].near != -1.0f) {
            view_buildProjection(i, &gViews[i].perspNorm, gViews[i].frustumFovY, gViews[i].frustumFovX,
                          gViewClips[i].near, gViewClips[i].far, gViews[i].perspScale);
            gViewClips[i].near = -1.0f;
        }
    }
    gClipChangePending = FALSE;
}

/* Builds a view's floating-point projection matrix and converts it into the current framebuffer's Mtx. */
void view_buildProjection(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    guPerspectiveF(gViews[viewIndex].projection, perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L(gViews[viewIndex].projection, &gViews[viewIndex].projectionMtx[gDrawBufferIndex]);
}

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define gRenderViews ((RenderView *)gViews)

// matching: 0x800D35E4 has no symbol; addressing it as D_800D35E0[1] lets IDO share one %hi, the original doesn't

#define D_800D35E4 ((&gCullVtxFlags)[-1])

/* Rebuilds every view's float view matrix and its view * projection product for this frame. */
void view_updateMatrices(void) {
    s16 view;
    MtxF projection;

    for (view = 0; view <= gScreenLayout; view++) {
        guMtxL2F(projection, &gRenderViews[view].projectionMtx[gDrawBufferIndex]);
        guMtxL2F(gViewMatrices[view], &gViewMtxBuffers[gDrawBufferIndex][view]);
        math_mtxMulF(gViewMatrices[view], projection, gViewProjMatrices[view]);
        view_buildFrustumPlanes(view);
    }
}

/* Emits the per-view RSP setup: matrices, perspective normalisation, clip ratio and render mode. */
Gfx *view_emitRspSetup(Gfx *gfx, s32 view) {
    gDPPipeSync(gfx++);
    gDPSetBlendColor(gfx++, 0, 0, 0, 1);
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPPerspNormalize(gfx++, gRenderViews[view].perspNorm);
    gSPClipRatio(gfx++, FRUSTRATIO_3);
    gSPClearGeometryMode(gfx++, G_LOD);
    gSPMatrix(gfx++, &gRenderViews[view].projectionMtx[gDrawBufferIndex], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &gViewMtxBuffers[gDrawBufferIndex][view], G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE | 0xF, // 0xF: blend mask bits
                    G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL |
                        GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM) |
                        GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM));
    return gfx;
}

/* Sets D_800D35E0/E4 to unk64/unk68 offset by how far the frustum FOV is below the view FOV (1 - frustum/fov). */
void func_1510B958(s32 view) {
    RenderView *rv;

    rv = &gRenderViews[view];
    D_800D35E0 = (rv->frustumFovX / rv->fovX - 1.0f) * -1.0f + rv->unk64;
    D_800D35E4 = (rv->frustumFovY / rv->fovY - 1.0f) * -1.0f + rv->unk68;
}

/* Builds the world display list for one view. */
Gfx *view_buildWorldDisplayList(Gfx *gfx, s16 view) {
    Camera *camera;
    s32 skipWorld;

    camera = &camera_global_Cameras[view];
    D_800BEBA0 = 1;
    skipWorld = FALSE;
    if (gCutsceneStates[0] == 1 && gCutsceneHideWorld != 0) {
        skipWorld = TRUE;
    }
    if (gEliminatedPlayers & (1 << view)) {
        skipWorld = TRUE;
    }
    gfx = view_emitRspSetup(gfx, view);
    gSPLookAtX(gfx++, &gLookAts[gDrawBufferIndex].l[0]);
    gSPLight(gfx++, &gLookAts[gDrawBufferIndex].l[1], 1);
    texture_updateTint(view);
    gfx = texture_setTintColors(gfx, 0xFF, 0xFF, view);
    gfx = func_151733D8(gfx, 1);
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    if (!skipWorld) {
        switch (level) {
        case 0x14:
            gfx = effect_bindBounceRecordFrames(gfx);
            break;
        case 0x1A:
            gfx = effect_bindFlagLightTextures(gfx);
            break;
        case 0x13:
            gfx = effect_bindMaterialTextures(gfx);
            break;
        case 0x35:
            gfx = effect_setScrolledTileSizes(gfx);
            break;
        case 0x33:
            gfx = effect_bindAnimatedTextures(gfx);
            break;
        }
    }
    collision_selectMap(0);
    if (gLightingEnabled == 0) {
        gDma2p(gfx++, G_MOVEMEM, &gViewAmbients[view][gDrawBufferIndex], 48, G_MV_LIGHT, G_MVO_L2);
    }
    gfx = light_pickLights(gfx, view, camera_global_Cameras[view].eye.x, camera_global_Cameras[view].eye.y, camera_global_Cameras[view].eye.z, 0,
                        gViewGfxLights[view], gViewGfxLightMax, &gViewAmbients[view][gDrawBufferIndex], &D_800D9E21, 1, 0, 0x25,
                        gPickedLights);
    if (!skipWorld) {
        if (level != 5) {
            if (D_800BEAAA) {
                gDPPipeSync(gfx++);
                if (gMeshBuilt) {
                    if (level != 0x14 && level != 0x33 && level != 0x32) {
                        gfx = gfx_copyLevelDlCulled(gLevelDisplayLists[0], gfx, gViewMatrices[view], 0, 0, 1 << view);
                    } else {
                        gSPDisplayList(gfx++, gLevelDisplayLists[0]);
                    }
                    if (gLevelDisplayLists[1] != NULL) {
                        gDPPipeSync(gfx++);
                        gfx = texture_setTintColors(gfx, 0xFF, 0xFF, view);
                        gfx = func_151733D8(gfx, 1);
                        if (level == 0x35) {
                            gfx = effect_setScrolledTileSizes(gfx);
                        }
                        gSPDisplayList(gfx++, gLevelDisplayLists[1]);
                    }
                    D_800BEBA0 = 0;
                    if (camera->HitsZBufferReadFlag && (camera->AvailableModes & 8)) {
                        gfx = camera_copyZRow(gfx, camera);
                        gDPPipeSync(gfx++);
                    }
                } else {
                    gSPDisplayList(gfx++, gLevelDisplayLists[0]);
                }
            } else if (D_800BEAAA) {
                gfx = gfx_copyDlCulled(gLevelDisplayLists[0], gfx, gViewMatrices[view], 0, 0, 0);
            } else {
                gSPDisplayList(gfx++, gLevelDisplayLists[0]);
            }
        }
        gfx = light_reemitLights(gfx, gViewGfxLights[view], gViewGfxLightMax, D_800D9E21, &gViewAmbients[view][gDrawBufferIndex]);
        if (gLevelDisplayLists[2] != NULL) {
            gDPPipeSync(gfx++);
            gfx = texture_setTintColors(gfx, 0xFF, 0xFF, view);
            gSPDisplayList(gfx++, gLevelDisplayLists[2]);
        }
    }
    gfx = flare_runProbes(gfx, view);
    gDPPipeSync(gfx++);
    return gfx;
}
