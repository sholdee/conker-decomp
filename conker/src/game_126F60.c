#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-player view record (0x180 bytes) in the array D_800BE628 points to; see game_13D350.c. */
typedef struct RenderView {
    /* 0x000 */ u8 pad0[0x24];
    /* 0x024 */ f32 top;
    /* 0x028 */ f32 bottom;
    /* 0x02C */ f32 left;
    /* 0x030 */ f32 right;
    /* 0x034 */ u8 pad34[0x14C];
} RenderView; // size 0x180

#define RENDER_VIEWS ((RenderView *)D_800BE628)

void func_15110360(s32 viewIndex, f32 mtx[4][4], f32 pitch, f32 yaw, f32 roll);
Gfx *func_15110544(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, u8 r, u8 g, u8 b);
void func_150FB4C0(Gfx *gfx, f32 mtx[4][4]);

/* Fills the current view with black, sets a primitive-colour combiner, and passes the view-projection
 * matrix built from (pitch, yaw, roll) on to func_150FB4C0. */
void func_150F9AB0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 pitch, f32 yaw, f32 roll) {
    f32 viewMtx[4][4];
    RenderView *view;

    func_15110360(D_80082FA4, viewMtx, pitch, yaw, roll);
    view = &RENDER_VIEWS[D_80082FA4];
    gfx = func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom, 0, 0, 0);
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x2C0F, 0x0F0A4004);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, PRIMITIVE_ALPHA, 0, 0, 0, 0, 0, PRIMITIVE, 0, PRIMITIVE_ALPHA, 0, 0, 0, 0, 0);
    func_150FB4C0(gfx, viewMtx);
}
