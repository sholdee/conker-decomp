#include <ultra64.h>

#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define RENDER_VIEWS ((RenderView *)D_800BE628)

// matching: a one-element const table; with a 0.8f literal IDO picks other FP registers in func_1501A220
static const f32 D_800968E0[] = { 0.8f };

/* Allocates the views for numPlayers players (numPlayers + 2 of them, or 1 when it is 0) and lays out views
 * 0..numPlayers with layout numPlayers, plus a layout-0 (full-screen) view after them. */
void func_1501A220(s32 numPlayers, s32 arg1) {
    s32 numViews;
    s32 i;
    s32 layout;
    f32 width;
    f32 scaled;
    s32 scaledWidth;

    D_80082FA0 = numPlayers;
    layout = D_80082FA0;

    if (layout == 0) {
        numViews = 1;
    } else {
        numViews = layout + 2;
    }

    D_800BE628 = allocate_memory(numViews * sizeof(RenderView), 1, 1, 0);
    D_800BE62C = allocate_memory(numViews * sizeof(Vp), 1, 1, 0);

    if (D_80082FA0 == 1) {
        width = (f32)D_800BE638[1] - 2.0f;
        scaled = width;
        scaled *= D_800968E0[0];
        scaledWidth = scaled;
        D_800BE6B8 = (f32)(scaledWidth - (scaledWidth % 4)) / width;
    }

    func_150006E0(level);

    for (i = 0; i <= (layout = D_80082FA0); i++) {
        func_1501A8C0(i, layout, 1023, 0);
    }

    if (layout != 0) {
        func_1501A8C0(i, 0, 1023, 0);
    }

    D_800BE617 = 1;
    D_800BE614 = 1;
    D_800BE635 = 0;
    func_1510B070(arg1);
}

extern Gfx D_8002C930[];

/* Writes the start of each framebuffer's display list (segment 0, the setup list D_8002C930, the depth image
 * and view 0's viewport for that framebuffer) and stores where each one ends in D_800BE9D8. */
void func_1501A39C(void) {
    s32 i;
    Gfx *gfx;

    for (i = 0; i < 2; i++) {
        gfx = D_800BE9C8[i];
        gSPSegment(gfx++, 0, 0);
        gSPSegment(gfx++, 0, 0);
        gSPDisplayList(gfx++, D_8002C930);
        gDPSetDepthImage(gfx++, D_800BE9C4);
        gSPViewport(gfx++, &RENDER_VIEWS[0].viewports[i]);
        D_800BE9D8[i] = gfx;
    }
}

/* Emits a pipe sync and a scissor around view viewIndex (the whole drawable area for 0xFF), clipping its
 * bottom edge to maxBottom when that is non-zero. Returns the advanced display list. */
Gfx *func_1501A490(Gfx *gfx, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode) {
    gDPPipeSync(gfx++);
    if (viewIndex == 0xFF) {
        gDPSetScissor(gfx++, mode, 2, 0, D_800BE620 - 2, D_800BE624);
    } else {
        f32 left;
        f32 right;
        f32 top;
        f32 bottom;
        RenderView *view;

        view = &RENDER_VIEWS[viewIndex];
        left = view->left;
        right = view->right;
        top = view->top;
        if (maxBottom != 0) {
            if ((f32)maxBottom < view->bottom) {
                bottom = (f32)maxBottom;
            } else {
                bottom = view->bottom;
            }
        } else {
            bottom = view->bottom;
        }
        func_1501AF44(&left, &top, &right, &bottom);
        gDPSetScissor(gfx++, mode, left, top, right, bottom);
    }
    return gfx;
}

/* Sets the color image to the current framebuffer. */
Gfx *func_1501A680(Gfx *gfx) {
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_8002AAE8[D_800BE9C0]);
    return gfx;
}

/* Fills a rectangle clamped to the drawable framebuffer area. */
Gfx *func_1501A6CC(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    if (ulx < 3) {
        ulx = 2;
    }
    if (uly <= 0) {
        uly = 0;
    }
    if (lrx >= D_800BE620 - 2) {
        lrx = D_800BE620 - 2;
    }
    if (lry >= D_800BE624) {
        lry = D_800BE624;
    }

    gDPFillRectangle(gfx++, ulx, uly, lrx, lry);
    return gfx;
}

/* Projects a world point through view's matrix to screen x, y and a depth value. */
void func_1501A764(s16 view, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *depth) {
    f32 projX;
    f32 projY;
    f32 projZ;
    f32 invW;

    func_150A7A00(D_800D9D10[view], x, y, z, &projX, &projY, &projZ, &invW);
    invW = 1.0f / invW;
    *screenX = RENDER_VIEWS[view].centerX + (RENDER_VIEWS[view].halfWidth + 5.0f) * projX * invW;
    *screenY = RENDER_VIEWS[view].centerY - (RENDER_VIEWS[view].halfHeight + 5.0f) * projY * invW;
    *depth = (projZ * invW * RENDER_VIEWS[view].viewports[D_800BE9C0].vp.vscale[2] +
              RENDER_VIEWS[view].viewports[D_800BE9C0].vp.vtrans[2]) * 32.0f;
}

extern f32 D_80082FA8[];

/* Lays out view `view` for screen layout `layout` (D_800BE668 holds each view's centre) and sets up its
 * projection and viewport, with depth range zRange. */
void func_1501A8C0(s32 view, s32 layout, s32 zRange, s16 arg3) {
    f32 pad[3]; // matching: unused locals that size the -g3 stack frame
    f32 halfFov;
    f32 sine;
    f32 fullWidth;
    f32 fullAspect;
    f32 aspect;
    f32 vpWidth;
    f32 vpHeight;
    s16 *center;

    RENDER_VIEWS[view].width = D_800BE638[layout];
    RENDER_VIEWS[view].height = D_800BE650[layout];
    if (layout != 0) {
        RENDER_VIEWS[view].width -= 2.0f;
        RENDER_VIEWS[view].height -= 2.0f;
    }
    RENDER_VIEWS[view].unk64 = D_80082FA8[layout];
    RENDER_VIEWS[view].unk68 = 2.0f;
    if (layout == 1) {
        RENDER_VIEWS[view].width *= D_800BE6B8;
        RENDER_VIEWS[view].fovX = 103.02f * D_800BE6B8;
        RENDER_VIEWS[view].fovY = 50.0f;
    } else {
        RENDER_VIEWS[view].fovX = 60.6f;
        RENDER_VIEWS[view].fovY = 50.0f;
    }
    RENDER_VIEWS[view].frustumFovX = RENDER_VIEWS[view].fovX;
    RENDER_VIEWS[view].frustumFovY = RENDER_VIEWS[view].fovY;
    center = D_800BE668[layout][view];
    RENDER_VIEWS[view].aspect = RENDER_VIEWS[view].width / RENDER_VIEWS[view].height;
    RENDER_VIEWS[view].halfWidth = RENDER_VIEWS[view].width * 0.5f;
    RENDER_VIEWS[view].halfHeight = RENDER_VIEWS[view].height * 0.5f;
    RENDER_VIEWS[view].centerX = center[0];
    RENDER_VIEWS[view].centerY = center[1];
    RENDER_VIEWS[view].left = RENDER_VIEWS[view].centerX - RENDER_VIEWS[view].halfWidth;
    RENDER_VIEWS[view].top = RENDER_VIEWS[view].centerY - RENDER_VIEWS[view].halfHeight;
    RENDER_VIEWS[view].right = RENDER_VIEWS[view].centerX + RENDER_VIEWS[view].halfWidth;
    RENDER_VIEWS[view].bottom = RENDER_VIEWS[view].centerY + RENDER_VIEWS[view].halfHeight;
    if (arg3 != 0) {
        func_1501B0A0(view, layout);
    } else {
        fullWidth = D_800BE638[0];
        fullAspect = fullWidth / (f32)D_800BE650[0];
        aspect = RENDER_VIEWS[view].width / RENDER_VIEWS[view].height;
        RENDER_VIEWS[view].scaleX = (RENDER_VIEWS[view].width / fullWidth) * (fullAspect / aspect);
        RENDER_VIEWS[view].scaleY = RENDER_VIEWS[view].height / (f32)D_800BE650[0];
    }
    halfFov = RENDER_VIEWS[view].fovX * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanX = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfWidth);
    halfFov = RENDER_VIEWS[view].fovY * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanY = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfHeight);
    vpWidth = RENDER_VIEWS[view].width + 10.0f;
    D_800BE62C[view].vp.vscale[0] = vpWidth + vpWidth;
    vpHeight = RENDER_VIEWS[view].height + 10.0f;
    D_800BE62C[view].vp.vscale[1] = vpHeight + vpHeight;
    D_800BE62C[view].vp.vscale[2] = zRange >> 1;
    D_800BE62C[view].vp.vscale[3] = 0;
    D_800BE62C[view].vp.vtrans[0] = RENDER_VIEWS[view].centerX * 4.0f;
    D_800BE62C[view].vp.vtrans[1] = RENDER_VIEWS[view].centerY * 4.0f;
    D_800BE62C[view].vp.vtrans[2] = zRange >> 1;
    D_800BE62C[view].vp.vtrans[3] = 0;
    do { // matching: the loop scope changes IDO's register allocation for the copies below
        RENDER_VIEWS[view].viewports[D_800BE9C0] = D_800BE62C[view];
    } while (0);
    RENDER_VIEWS[view].viewports[!D_800BE9C0] = RENDER_VIEWS[view].viewports[D_800BE9C0];
    RENDER_VIEWS[view].perspScale = 1.0f;
    RENDER_VIEWS[view].onScreen = func_1501AE94(view);
    // matching: index + base (not RENDER_VIEWS[view]) sets IDO's addu operand order
    func_1501AF44(&(view + RENDER_VIEWS)->left, &(view + RENDER_VIEWS)->top, &(view + RENDER_VIEWS)->right,
                  &(view + RENDER_VIEWS)->bottom);
    func_1510B32C(view, 0.0f, 0.0f, RENDER_VIEWS[view].perspScale);
    D_800BE630[view] = view;
}

/* Returns whether view's rectangle lies entirely inside the drawable framebuffer area. */
s32 func_1501AE94(s32 viewIndex) {
    f32 maxX = (f32)D_800BE620 - 2.0f;
    f32 maxY = (f32)D_800BE624;
    RenderView *view = &RENDER_VIEWS[viewIndex];

    if (view->left < 2.0f || maxX < view->right) {
        return FALSE;
    }
    if (view->top < 0.0f || maxY < view->bottom) {
        return FALSE;
    }
    return TRUE;
}

/* Clamps a rectangle to the drawable framebuffer area (x in [2, width - 2], y in [0, height]). */
void func_1501AF44(f32 *left, f32 *top, f32 *right, f32 *bottom) {
    f32 clamped;

    if (*left < 2.0f) {
        *left = 2.0f;
    } else {
        if ((f32)D_800BE620 - 2.0f < *left) {
            clamped = (f32)D_800BE620 - 2.0f;
        } else {
            clamped = *left;
        }
        *left = clamped;
    }

    if (*right < 2.0f) {
        *right = 2.0f;
    } else {
        if ((f32)D_800BE620 - 2.0f < *right) {
            clamped = (f32)D_800BE620 - 2.0f;
        } else {
            clamped = *right;
        }
        *right = clamped;
    }

    if (*top < 0.0f) {
        *top = 0.0f;
    } else {
        if ((f32)D_800BE624 < *top) {
            clamped = (f32)D_800BE624;
        } else {
            clamped = *top;
        }
        *top = clamped;
    }

    if (*bottom < 0.0f) {
        *bottom = 0.0f;
    } else {
        if ((f32)D_800BE624 < *bottom) {
            clamped = (f32)D_800BE624;
        } else {
            clamped = *bottom;
        }
        *bottom = clamped;
    }
}

/* Passes func_1510B128 the angles (degrees) that view's half-size subtends at the distance where the layout's
 * full width/height fills view's fov, less the fov of view `layout`. */
void func_1501B0A0(s32 view, s32 layout) {
    f32 halfFovX;
    f32 halfFovY;
    f32 tanX;
    f32 tanY;
    f32 distX;
    f32 distY;
    f32 x;
    f32 y;

    halfFovX = RENDER_VIEWS[view].fovX * 0.008726646f;
    halfFovY = RENDER_VIEWS[view].fovY * 0.008726646f;
    tanX = sinf(halfFovX) / cosf(halfFovX);
    tanY = sinf(halfFovY) / cosf(halfFovY);
    distX = (f32)D_800BE638[layout] / (2.0f * tanX);
    distY = (f32)D_800BE650[layout] / (2.0f * tanY);
    x = func_150484A0(RENDER_VIEWS[view].halfWidth, distX) * 114.59155f - RENDER_VIEWS[layout].fovX;
    y = func_150484A0(RENDER_VIEWS[view].halfHeight, distY) * 114.59155f - RENDER_VIEWS[layout].fovY;
    func_1510B128(view, x, y, 1.0f, 0.0f);
}

/* Builds the four side-plane normals of view's frustum from its frustum fov. */
void func_1501B22C(s32 viewIndex) {
    f32 s;
    f32 n;
    f32 c;
    f32 halfFovX;
    RenderView *view;
    f32 halfFovY;

    view = viewIndex + RENDER_VIEWS; // matching: index + base sets IDO's register order
    halfFovX = view->frustumFovX * 0.5f;
    halfFovY = view->frustumFovY * 0.5f;
    c = cosf(-halfFovX * 0.017453292f);
    s = sinf(-halfFovX * 0.017453292f);
    n = -c;
    view->frustumPlanes[0][0] = -n;
    view->frustumPlanes[1][0] = n;
    view->frustumPlanes[1][2] = s;
    view->frustumPlanes[0][2] = s;
    view->frustumPlanes[1][1] = 0.0f;
    view->frustumPlanes[0][1] = 0.0f;
    c = cosf(halfFovY * 0.017453292f);
    s = -sinf(halfFovY * 0.017453292f);
    n = -c;
    view->frustumPlanes[2][0] = 0.0f;
    view->frustumPlanes[2][1] = n;
    view->frustumPlanes[2][2] = s;
    view->frustumPlanes[3][0] = 0.0f;
    view->frustumPlanes[3][1] = -n;
    view->frustumPlanes[3][2] = s;
}
