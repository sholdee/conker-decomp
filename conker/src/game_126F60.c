#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define RENDER_VIEWS ((RenderView *)D_800BE628) // matching: the no-op cast changes the code; plain D_800BE628 mismatches

/* Fills the current view with black, sets a primitive-colour combiner, and passes the view-projection
 * matrix built from (pitch, yaw, roll) on to func_150FB4C0. A sky draw routine (D_800891D8). */
Gfx *func_150F9AB0(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent) {
    MtxF viewMtx;
    RenderView *view;

    func_15110360(D_80082FA4, viewMtx, pitch, yaw, roll);
    view = &RENDER_VIEWS[D_80082FA4];
    gfx = func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom, 0, 0, 0);
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x2C0F, 0x0F0A4004);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, PRIMITIVE_ALPHA, 0, 0, 0, 0, 0, PRIMITIVE, 0, PRIMITIVE_ALPHA, 0, 0, 0, 0, 0);
    return func_150FB4C0(gfx, viewMtx);
}
