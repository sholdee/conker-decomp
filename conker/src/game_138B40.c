#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define gRenderViews ((RenderView *)D_800BE628)

extern Gfx *D_800B0E08;
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
                    if (D_800B0E04 != NULL) {
                        gDPPipeSync(gfx++);
                        gfx = func_1510CDB8(gfx, 0xFF, 0xFF, view);
                        gfx = func_151733D8(gfx, 1);
                        if (level == 0x35) {
                            gfx = func_150CF5E8(gfx);
                        }
                        gSPDisplayList(gfx++, D_800B0E04);
                    }
                    D_800BEBA0 = 0;
                    // byte 0x8B8 of Camera is not yet a named field in structs.h
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
        if (D_800B0E08 != NULL) {
            gDPPipeSync(gfx++);
            gfx = func_1510CDB8(gfx, 0xFF, 0xFF, view);
            gSPDisplayList(gfx++, D_800B0E08);
        }
    }
    gfx = func_151742EC(gfx, view);
    gDPPipeSync(gfx++);
    return gfx;
}
