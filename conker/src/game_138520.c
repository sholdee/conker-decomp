#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One on-screen view (a player's region of the framebuffer); D_800BE628 points at an array of these.
 * Same layout as game_13D350.c's and game_476D0.c's RenderView, with the projection fields this file uses. */
typedef struct RenderView {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ f32 halfWidth;
    /* 0x010 */ f32 halfHeight;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ u8 pad24[0x48];
    /* 0x06C */ f32 fovX; // degrees
    /* 0x070 */ f32 fovY; // degrees
    /* 0x074 */ f32 frustumFovX;
    /* 0x078 */ f32 frustumFovY;
    /* 0x07C */ f32 pixelTanX;
    /* 0x080 */ f32 pixelTanY;
    /* 0x084 */ f32 perspScale;
    /* 0x088 */ u8 pad88[0x30];
    /* 0x0B8 */ u16 perspNorm;
    /* 0x0BA */ u8 padBA[2];
    /* 0x0BC */ f32 projection[4][4];
    /* 0x0FC */ u8 padFC[4];
    /* 0x100 */ Mtx projectionMtx[2]; // indexed by the framebuffer index D_800BE9C0
} RenderView; // size 0x180

/* variables.h types D_800BE628 as s32. */
#define RENDER_VIEWS ((RenderView *)D_800BE628)
// matching: IDO allocates registers differently for &RENDER_VIEWS[i].perspNorm than for this integer arithmetic
#define VIEW_PERSP_NORM(i) (&((RenderView *)(D_800BE628 + (i) * 0x180))->perspNorm)

extern f32 D_800D9B28[4][4];
extern s16 D_800DD2F2;
extern s16 D_800DD2F4;
extern f32 D_800C3648; // added to both FOVs of every view
extern f32 D_800C364C; // near-plane override when non-zero
extern f32 D_800C3650; // far-plane override when non-zero

f32 cosf(f32);
f32 sinf(f32);
void func_1510B5F8(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);

/* Loads the default near/far planes from D_800DD2F2/D_800DD2F4 and builds the matrix D_800D9B28 spanning
 * view 0 (+-halfWidth by +-halfHeight, depth 1..10000). */
void func_1510B070(s32 arg0) {
    D_800D9B20 = D_800DD2F2;
    D_800D9B1C = D_800DD2F4;
    D_800D3670 = 100.0f - D_800D9B1C;
    func_151EF954(D_800D9B28, -RENDER_VIEWS->halfWidth, RENDER_VIEWS->halfWidth, -RENDER_VIEWS->halfHeight,
                  RENDER_VIEWS->halfHeight, 1.0f, 10000.0f, 1.0f);
}

/* Widens a view's frustum FOVs by (fovXDelta, fovYDelta) plus the global D_800C3648, updates unk1C/unk20 and
 * rebuilds its projection. The near plane is D_800C364C, else `near`, else the default D_800D9B20. */
void func_1510B128(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale, f32 near) {
    f32 nearPlane;
    f32 farPlane;
    f32 pad; // matching: keeps the stack frame size

    RENDER_VIEWS[viewIndex].frustumFovX = RENDER_VIEWS[viewIndex].fovX + fovXDelta + D_800C3648;
    RENDER_VIEWS[viewIndex].frustumFovY = RENDER_VIEWS[viewIndex].fovY + fovYDelta + D_800C3648;
    {
        f32 dist;
        f32 cosine;
        f32 angle;

        /* half the FOV in radians; dist = halfWidth / tan(angle) */
        angle = RENDER_VIEWS[viewIndex].frustumFovX * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * RENDER_VIEWS[viewIndex].halfWidth) / sinf(angle);
        RENDER_VIEWS[viewIndex].unk1C =
            (RENDER_VIEWS[viewIndex].unk14 * dist) * RENDER_VIEWS[viewIndex].pixelTanX * scale;
        angle = RENDER_VIEWS[viewIndex].frustumFovY * 0.008726646192f;
        cosine = cosf(angle);
        dist = (cosine * RENDER_VIEWS[viewIndex].halfHeight) / sinf(angle);
        RENDER_VIEWS[viewIndex].unk20 =
            (RENDER_VIEWS[viewIndex].unk18 * dist) * RENDER_VIEWS[viewIndex].pixelTanY * scale;
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
    func_1510B5F8(viewIndex, VIEW_PERSP_NORM(viewIndex), RENDER_VIEWS[viewIndex].frustumFovY,
                  RENDER_VIEWS[viewIndex].frustumFovX, nearPlane, farPlane, scale);
}

/* func_1510B128 with the default near plane, also remembered in D_800D9AC0 (unk0 = scale, unk4/unk8 = FOV
 * deltas) so func_1510B3B0 can re-apply it. */
void func_1510B32C(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale) {
    func_1510B128(viewIndex, fovXDelta, fovYDelta, scale, 0.0f);
    D_800D9AC0[viewIndex].unk0 = scale;
    D_800D9AC0[viewIndex].unk4 = fovXDelta;
    D_800D9AC0[viewIndex].unk8 = fovYDelta;
    D_800D9AF0 = TRUE;
}

/* Re-applies every FOV change func_1510B32C recorded (scale -1.0f marks none) and clears the records. */
void func_1510B3B0(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800D9AC0[i].unk0 != -1.0f) {
            func_1510B128(i, D_800D9AC0[i].unk4, D_800D9AC0[i].unk8, D_800D9AC0[i].unk0, 0.0f);
            D_800D9AC0[i].unk0 = -1.0f;
        }
    }
    D_800D9AF0 = FALSE;
}

/* Rebuilds a view's projection with new near/far planes (0.0f picks the defaults D_800D9B20/D_800D9B1C) and
 * remembers them in D_800D9AF8 for func_1510B51C. */
void func_1510B458(s32 viewIndex, f32 near, f32 far) {
    if (near == 0.0f) {
        near = D_800D9B20;
    }
    if (far == 0.0f) {
        far = D_800D9B1C;
    }
    func_1510B5F8(viewIndex, VIEW_PERSP_NORM(viewIndex), RENDER_VIEWS[viewIndex].frustumFovY,
                  RENDER_VIEWS[viewIndex].frustumFovX, near, far, RENDER_VIEWS[viewIndex].perspScale);
    D_800D9AF8[viewIndex].unk0 = near;
    D_800D9AF8[viewIndex].unk4 = far;
    D_800D9B18 = TRUE;
}

/* Re-applies every near/far change func_1510B458 recorded (near -1.0f marks none) and clears the records. */
void func_1510B51C(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800D9AF8[i].unk0 != -1.0f) {
            func_1510B5F8(i, VIEW_PERSP_NORM(i), RENDER_VIEWS[i].frustumFovY, RENDER_VIEWS[i].frustumFovX,
                          D_800D9AF8[i].unk0, D_800D9AF8[i].unk4, RENDER_VIEWS[i].perspScale);
            D_800D9AF8[i].unk0 = -1.0f;
        }
    }
    D_800D9B18 = FALSE;
}

/* Builds a view's floating-point projection matrix and converts it into the current framebuffer's Mtx. */
void func_1510B5F8(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    guPerspectiveF(RENDER_VIEWS[viewIndex].projection, perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L(RENDER_VIEWS[viewIndex].projection, &RENDER_VIEWS[viewIndex].projectionMtx[D_800BE9C0]);
}
