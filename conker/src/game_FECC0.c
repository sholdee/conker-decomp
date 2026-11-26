#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A sky draw routine (D_800891D8): draws the current view's sky quad (D_800DBE88) with the 4x256 RGBA32 gradient
 * texture D_800DBE80, rolled by `roll` and with its t window (rows top..bottom, chosen by level) scrolled by the
 * camera pitch. */
Gfx *func_150D1810(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent) {
    MtxF rotation;
    MtxF viewProj;
    s32 top;
    s32 bottom;
    s32 tOffset;
    Vtx *vtx;
    Texture tex;

    tOffset = pitch * 81.0f;
    if ((level == 0x33) || (level == 0x32) || (level == 0x3F)) {
        top = -98;
        bottom = 39;
    } else {
        top = -19;
        bottom = 101;
    }
    tex.addr = (u32)D_800DBE80;
    tex.width = 4;
    tex.height = 256;
    tex.fmt = G_IM_FMT_RGBA;
    tex.siz = G_IM_SIZ_32b;
    tex.frames = 0;
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    // 0xF: the low nibble below G_MDSFT_ALPHADITHER is set in the original word
    gDPSetOtherMode(gfx++, G_TP_PERSP | G_TF_BILERP | G_TC_FILT | 0xF,
                    G_RM_OPA_SURF | G_RM_OPA_SURF2 | G_ZS_PRIM);
    gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gfx = func_1501A490(gfx, D_80082FA4, 0, 0, 0, 0); // takes the view index as s16
    gfx = func_1501A680(gfx);
    gfx = func_150950D4(gfx, &tex, 0, 0, 0, 0, G_TX_CLAMP, 256, 256, 3);
    func_151102CC(rotation, 0.0f, 0.0f, roll);
    func_150A7A48(rotation, D_800BE628[D_80082FA4].projection, viewProj);
    guMtxF2L(viewProj, &D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0]);
    gSPMatrix(gfx++, &D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gfx++, &D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPPerspNormalize(gfx++, D_800BE628[D_80082FA4].perspNorm);
    vtx = D_800DBE88[D_80082FA4][D_800BE9C0];
    vtx[0].v.tc[1] = vtx[1].v.tc[1] = ((top + 256) << 5) + tOffset;
    vtx[2].v.tc[1] = vtx[3].v.tc[1] = ((bottom + 511) << 5) + tOffset;
    gSPVertex(gfx++, vtx, 4, 0);
    gSP2Triangles(gfx++, 0, 1, 2, 0, 0, 2, 3, 0);
    return gfx;
}
