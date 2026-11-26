#include <ultra64.h>
#include "structs.h"

/* This unit's .bss (game_138B40.c's follows it), defined before the headers declare it (see init_50A0.c). */
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
