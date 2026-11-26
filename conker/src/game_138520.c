#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init_50A0.c). */
Vec3f D_800D9AC0[4];     // FOV changes func_1510B32C recorded per view: x = scale (-1.0f: none), y/z = deltas
u8 D_800D9AF0;           // set while a D_800D9AC0 record is pending
ViewClip gViewClips[4];
u8 D_800D9B18;           // set while a gViewClips record is pending
f32 D_800D9B1C;          // default far plane
f32 D_800D9B20;          // default near plane
static Mtx D_800D9B28;   // projection spanning view 0

#include "functions.h"
#include "variables.h"

/* The rest of this unit's .bss: conker/view.h declares these before the definitions, so they follow D_800D9B28. */
u8 D_800D9B68[4][3];     // per tint slot: prim colour (game_139FC0 adds the positive part of its wave)
u8 D_800D9B78[4][3];     // per tint slot: env colour (the negative part)
u8 D_800D9B84[3];
u8 D_800D9B88[3];
u8 D_800D9B8B;
u8 D_800D9B8C;
LookAt D_800D9B90[2];          // per framebuffer
Ambient D_800D9BD0[4][2];      // per view and framebuffer
MtxF D_800D9C10[4];            // per view: view matrix
MtxF D_800D9D10[4];            // per view: view matrix * projection
PointLightGfx *D_800D9E10[4];  // per view
u8 D_800D9E20;
u8 D_800D9E21;                 // light count byte func_1515D914 stored for player 0
PointLight *D_800D9E28[14];    // point lights func_1515D914 picked this frame

/* Loads the default near/far planes from D_800DD2F2/D_800DD2F4 and builds the matrix D_800D9B28 spanning
 * view 0 (+-halfWidth by +-halfHeight, depth 1..10000). */
void func_1510B070(s32 arg0) {
    D_800D9B20 = D_800DD2F2;
    D_800D9B1C = D_800DD2F4;
    D_800D3670 = 100.0f - D_800D9B1C;
    func_151EF954(&D_800D9B28, -D_800BE628->halfWidth, D_800BE628->halfWidth, -D_800BE628->halfHeight,
                  D_800BE628->halfHeight, 1.0f, 10000.0f, 1.0f);
}

/* Widens a view's frustum FOVs by (fovXDelta, fovYDelta) plus the global D_800C3648, updates unk1C/unk20 and
 * rebuilds its projection. The near plane is D_800C364C, else `near`, else the default D_800D9B20. */
void func_1510B128(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale, f32 near) {
    f32 nearPlane;
    f32 farPlane;
    f32 pad; // matching: keeps the stack frame size

    D_800BE628[viewIndex].frustumFovX = D_800BE628[viewIndex].fovX + fovXDelta + D_800C3648;
    D_800BE628[viewIndex].frustumFovY = D_800BE628[viewIndex].fovY + fovYDelta + D_800C3648;
    {
        f32 dist;
        f32 cosine;
        f32 angle;

        /* half the FOV in radians; dist = halfWidth / tan(angle) */
        angle = D_800BE628[viewIndex].frustumFovX * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * D_800BE628[viewIndex].halfWidth) / sinf(angle);
        D_800BE628[viewIndex].unk1C =
            (D_800BE628[viewIndex].scaleX * dist) * D_800BE628[viewIndex].pixelTanX * scale;
        angle = D_800BE628[viewIndex].frustumFovY * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * D_800BE628[viewIndex].halfHeight) / sinf(angle);
        D_800BE628[viewIndex].unk20 =
            (D_800BE628[viewIndex].scaleY * dist) * D_800BE628[viewIndex].pixelTanY * scale;
    }
    if (D_800C364C != 0.0f) {
        nearPlane = D_800C364C;
    } else if (near != 0.0f) {
        nearPlane = near;
    } else {
        nearPlane = D_800D9B20;
    }
    if (D_800C3650 != 0.0f) {
        farPlane = D_800C3650;
    } else {
        farPlane = D_800D9B1C;
    }
    func_1510B5F8(viewIndex, &D_800BE628[viewIndex].perspNorm, D_800BE628[viewIndex].frustumFovY,
                  D_800BE628[viewIndex].frustumFovX, nearPlane, farPlane, scale);
}

/* func_1510B128 with the default near plane, also remembered in D_800D9AC0 (unk0 = scale, unk4/unk8 = FOV
 * deltas) so func_1510B3B0 can re-apply it. */
void func_1510B32C(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale) {
    func_1510B128(viewIndex, fovXDelta, fovYDelta, scale, 0.0f);
    D_800D9AC0[viewIndex].x = scale;
    D_800D9AC0[viewIndex].y = fovXDelta;
    D_800D9AC0[viewIndex].z = fovYDelta;
    D_800D9AF0 = TRUE;
}

/* Re-applies every FOV change func_1510B32C recorded (scale -1.0f marks none) and clears the records. */
void func_1510B3B0(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800D9AC0[i].x != -1.0f) {
            func_1510B128(i, D_800D9AC0[i].y, D_800D9AC0[i].z, D_800D9AC0[i].x, 0.0f);
            D_800D9AC0[i].x = -1.0f;
        }
    }
    D_800D9AF0 = FALSE;
}

/* Rebuilds a view's projection with new near/far planes (0.0f picks the defaults D_800D9B20/D_800D9B1C) and
 * remembers them in gViewClips for func_1510B51C. */
void func_1510B458(s32 viewIndex, f32 near, f32 far) {
    if (near == 0.0f) {
        near = D_800D9B20;
    }
    if (far == 0.0f) {
        far = D_800D9B1C;
    }
    func_1510B5F8(viewIndex, &D_800BE628[viewIndex].perspNorm, D_800BE628[viewIndex].frustumFovY,
                  D_800BE628[viewIndex].frustumFovX, near, far, D_800BE628[viewIndex].perspScale);
    gViewClips[viewIndex].near = near;
    gViewClips[viewIndex].far = far;
    D_800D9B18 = TRUE;
}

/* Re-applies every near/far change func_1510B458 recorded (near -1.0f marks none) and clears the records. */
void func_1510B51C(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (gViewClips[i].near != -1.0f) {
            func_1510B5F8(i, &D_800BE628[i].perspNorm, D_800BE628[i].frustumFovY, D_800BE628[i].frustumFovX,
                          gViewClips[i].near, gViewClips[i].far, D_800BE628[i].perspScale);
            gViewClips[i].near = -1.0f;
        }
    }
    D_800D9B18 = FALSE;
}

/* Builds a view's floating-point projection matrix and converts it into the current framebuffer's Mtx. */
void func_1510B5F8(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    guPerspectiveF(D_800BE628[viewIndex].projection, perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L(D_800BE628[viewIndex].projection, &D_800BE628[viewIndex].projectionMtx[D_800BE9C0]);
}

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define gRenderViews ((RenderView *)D_800BE628)

// matching: 0x800D35E4 has no symbol; addressing it as D_800D35E0[1] lets IDO share one %hi, the original doesn't

#define D_800D35E4 ((&D_800D35E8)[-1])

/* Rebuilds every view's float view matrix and its view * projection product for this frame. */
void func_1510B690(void) {
    s16 view;
    MtxF projection;

    for (view = 0; view <= D_80082FA0; view++) {
        guMtxL2F(projection, &gRenderViews[view].projectionMtx[D_800BE9C0]);
        guMtxL2F(D_800D9C10[view], &D_800DC2A0[D_800BE9C0][view]);
        func_150A7A48(D_800D9C10[view], projection, D_800D9D10[view]);
        func_1501B22C(view);
    }
}

/* Emits the per-view RSP setup: matrices, perspective normalisation, clip ratio and render mode. */
Gfx *func_1510B7B4(Gfx *gfx, s32 view) {
    gDPPipeSync(gfx++);
    gDPSetBlendColor(gfx++, 0, 0, 0, 1);
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPPerspNormalize(gfx++, gRenderViews[view].perspNorm);
    gSPClipRatio(gfx++, FRUSTRATIO_3);
    gSPClearGeometryMode(gfx++, G_LOD);
    gSPMatrix(gfx++, &gRenderViews[view].projectionMtx[D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_800DC2A0[D_800BE9C0][view], G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
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
Gfx *func_1510B9D0(Gfx *gfx, s16 view) {
    Camera *camera;
    s32 skipWorld;

    camera = &camera_global_Cameras[view];
    D_800BEBA0 = 1;
    skipWorld = FALSE;
    if (D_800C35EA[0] == 1 && D_800C3662 != 0) {
        skipWorld = TRUE;
    }
    if (D_800D18A0 & (1 << view)) {
        skipWorld = TRUE;
    }
    gfx = func_1510B7B4(gfx, view);
    gSPLookAtX(gfx++, &D_800D9B90[D_800BE9C0].l[0]);
    gSPLight(gfx++, &D_800D9B90[D_800BE9C0].l[1], 1);
    func_1510CB10(view);
    gfx = func_1510CDB8(gfx, 0xFF, 0xFF, view);
    gfx = func_151733D8(gfx, 1);
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    if (!skipWorld) {
        switch (level) {
        case 0x14:
            gfx = func_150C8600(gfx);
            break;
        case 0x1A:
            gfx = func_15100464(gfx);
            break;
        case 0x13:
            gfx = func_150DFBD0(gfx);
            break;
        case 0x35:
            gfx = func_150CF5E8(gfx);
            break;
        case 0x33:
            gfx = func_150D765C(gfx);
            break;
        }
    }
    func_1510F800(0);
    if (D_800DCD7C == 0) {
        gDma2p(gfx++, G_MOVEMEM, &D_800D9BD0[view][D_800BE9C0], 48, G_MV_LIGHT, G_MVO_L2);
    }
    gfx = func_1515D914(gfx, view, camera_global_Cameras[view].eye.x, camera_global_Cameras[view].eye.y, camera_global_Cameras[view].eye.z, 0,
                        D_800D9E10[view], D_800D9E20, &D_800D9BD0[view][D_800BE9C0], &D_800D9E21, 1, 0, 0x25,
                        D_800D9E28);
    if (!skipWorld) {
        if (level != 5) {
            if (D_800BEAAA) {
                gDPPipeSync(gfx++);
                if (D_800DBE62) {
                    if (level != 0x14 && level != 0x33 && level != 0x32) {
                        gfx = func_150A5378(D_800B0E00[0], gfx, D_800D9C10[view], 0, 0, 1 << view);
                    } else {
                        gSPDisplayList(gfx++, D_800B0E00[0]);
                    }
                    if (D_800B0E00[1] != NULL) {
                        gDPPipeSync(gfx++);
                        gfx = func_1510CDB8(gfx, 0xFF, 0xFF, view);
                        gfx = func_151733D8(gfx, 1);
                        if (level == 0x35) {
                            gfx = func_150CF5E8(gfx);
                        }
                        gSPDisplayList(gfx++, D_800B0E00[1]);
                    }
                    D_800BEBA0 = 0;
                    if (camera->HitsZBufferReadFlag && (camera->AvailableModes & 8)) {
                        gfx = func_1512E5F0(gfx, camera);
                        gDPPipeSync(gfx++);
                    }
                } else {
                    gSPDisplayList(gfx++, D_800B0E00[0]);
                }
            } else if (D_800BEAAA) {
                gfx = func_150A50C0(D_800B0E00[0], gfx, D_800D9C10[view], 0, 0, 0);
            } else {
                gSPDisplayList(gfx++, D_800B0E00[0]);
            }
        }
        gfx = func_1515E544(gfx, D_800D9E10[view], D_800D9E20, D_800D9E21, &D_800D9BD0[view][D_800BE9C0]);
        if (D_800B0E00[2] != NULL) {
            gDPPipeSync(gfx++);
            gfx = func_1510CDB8(gfx, 0xFF, 0xFF, view);
            gSPDisplayList(gfx++, D_800B0E00[2]);
        }
    }
    gfx = func_151742EC(gfx, view);
    gDPPipeSync(gfx++);
    return gfx;
}
