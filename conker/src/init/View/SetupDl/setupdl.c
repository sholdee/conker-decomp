#include <ultra64.h>
#include "conker/view.h"

/*
 * A data-only unit, linked after libultra. Its .rodata (0x2C930) follows that of libultra/gu/libm_vals, the
 * last libultra object, and ends with unit padding before the audio microcode data (0x2C960). No init unit
 * uses it.
 *
 * The setup display list at the start of each framebuffer's display list (view_initFrameDisplayLists,
 * game/View/Views/views.c).
 */
const Gfx gFrameSetupDL[] = {
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsDPSetOtherMode(0x00000F, 0),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
