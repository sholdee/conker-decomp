#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Copies one row of the z-buffer (D_800BE9C4, D_800BE620 pixels wide; row index at
 * camera+0x8BA) into the camera's one-row buffer unk8BC by drawing a 1-pixel-high
 * copy-mode texture rectangle, then restores a 1-cycle z-buffered render mode.
 */
Gfx *func_1512E5F0(Gfx *gfx, Camera *camera) {
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE | 0xF, // 0xF: blend mask bits
                    G_AC_NONE | G_ZS_PIXEL);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
                       D_800BE9C4 + (u16)camera->HitsZBufferRowIndex * D_800BE620);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, D_800BE620 - 1, CALC_DXT(D_800BE620, G_IM_SIZ_16b_BYTES));
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, ((D_800BE620 * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, (D_800BE620 - 1) << 2, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, camera->HitsZBufferHoriDataPtr);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, D_800BE620, 1);
    gSPTextureRectangle(gfx++, 0, 0, D_800BE620 << 2, 1 << 2, G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
    gDPPipeSync(gfx++);

    gfx = func_1501A490(func_1501A680(gfx), D_80082FA6, 0, 0, 0, 0);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE | 0xF, // 0xF: blend mask bits
                    G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | CVG_DST_FULL | ZMODE_OPA | ALPHA_CVG_SEL |
                        GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM) |
                        GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM));
    return gfx;
}
