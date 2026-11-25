#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* One on-screen view (a player's region of the framebuffer); D_800BE628 points at an array of these. */
typedef struct RenderView {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 onScreen;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ f32 width;
    /* 0x008 */ f32 height;
    /* 0x00C */ f32 halfWidth;
    /* 0x010 */ f32 halfHeight;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[8];
    /* 0x024 */ f32 top;
    /* 0x028 */ f32 bottom;
    /* 0x02C */ f32 left;
    /* 0x030 */ f32 right;
    /* 0x034 */ f32 centerX;
    /* 0x038 */ f32 centerY;
    /* 0x03C */ u8 pad3C[4];
    /* 0x040 */ Vp viewports[2]; // indexed by the framebuffer index D_800BE9C0
    /* 0x060 */ f32 aspect;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ f32 fovX; // degrees
    /* 0x070 */ f32 fovY; // degrees
    /* 0x074 */ f32 frustumFovX;
    /* 0x078 */ f32 frustumFovY;
    /* 0x07C */ f32 pixelTanX; // tan(fovX / 2) / halfWidth
    /* 0x080 */ f32 pixelTanY; // tan(fovY / 2) / halfHeight
    /* 0x084 */ f32 unk84;
    /* 0x088 */ f32 frustumPlanes[4][3]; // left, right, top, bottom side-plane normals
    /* 0x0B8 */ u8 padB8[0xC8];
} RenderView; // size 0x180

/* variables.h declares these as s32; they hold pointers to arrays of views and viewports. */
#define RENDER_VIEWS ((RenderView *)D_800BE628)
#define VIEWPORTS ((Vp *)D_800BE62C)
#define VIEW_MATRICES ((f32 (*)[4][4])D_800D9D10)

/* variables.h declares D_800BE638 as s32; it starts the per-layout view widths (D_800BE650 has the heights). */
#define VIEW_WIDTHS (&D_800BE638)

void func_1501A8C0(s32 view, s32 layout, s32 zRange, s16 arg3);
s32 func_1501AE94(s32 viewIndex);
void func_1501AF44(f32 *left, f32 *top, f32 *right, f32 *bottom);

void func_1501A220(s32 arg0, s32 arg1) {
    s32 phi_s0;
    s32 i;
    s32 var_s1;
    f32 temp_f0;
    f32 temp_f2;
    s32 temp_f16;

    D_80082FA0 = arg0;
    var_s1 = D_80082FA0;

    if (var_s1 == 0) {
        phi_s0 = 1;
    } else {
        phi_s0 = var_s1 + 2;
    }

    D_800BE628 = allocate_memory(phi_s0 * 0x180, 1, 1, 0);
    D_800BE62C = allocate_memory(phi_s0 * 16, 1, 1, 0);

    if (D_80082FA0 == 1) {
        temp_f0 = (f32)D_800BE63C[0] - 2.0f;
        temp_f2 = temp_f0;
        temp_f2 *= *(f32 *)&D_800968E0;
        temp_f16 = temp_f2;
        D_800BE6B8 = (f32)(temp_f16 - (temp_f16 % 4)) / temp_f0;
    }

    func_150006E0(D_800BE9F0);

    var_s1 = D_80082FA0;
    i = 0;
    if (var_s1 >= 0) {
        do {
            func_1501A8C0(i, var_s1, 1023, 0);
            var_s1 = D_80082FA0;
            i++;
        } while (i <= var_s1);
    }

    if (var_s1 != 0) {
        func_1501A8C0(i, 0, 1023, 0);
    }

    D_800BE617 = 1;
    D_800BE614 = 1;
    D_800BE635 = 0;
    func_1510B070(arg1);
}
// NON-MATCHING: 80% there
// void func_1501A220(s32 arg0, s32 arg1) {
//     s32 phi_s0;
//     s32 i;
//
//     f32 temp_f0;
//     s32 temp_f16;
//
//     D_80082FA0 = arg0;
//
//     if (arg0 == 0) {
//         phi_s0 = 1;
//     } else {
//         phi_s0 = arg0 + 2;
//     }
//     D_800BE628 = allocate_memory((phi_s0 * 3) << 7, 1, 1, 0);
//     D_800BE62C = allocate_memory(phi_s0 * 16, 1, 1, 0);
//     if (D_80082FA0 == 1) {
//         temp_f0 = D_800BE63C - 2.0f;
//         temp_f16 = temp_f0 * D_800968E0;
//         phi_s0 = temp_f16 & 3;
//         if ((temp_f16 < 0) && ((temp_f16 & 3) != 0)) {
//             phi_s0 -= 4;
//         }
//         D_800BE6B8 = (temp_f16 - phi_s0) / temp_f0;
//     }
//
//     func_150006E0(D_800BE9F0);
//
//     for(i = 0; i <= D_80082FA0; i++) {
//         func_1501A8C0(i, D_80082FA0, 1023, 0);
//     }
//     if (D_80082FA0 != 0) {
//         func_1501A8C0(i, 0, 1023, 0);
//     }
//
//     D_800BE617 = 1;
//     D_800BE614 = 1;
//     D_800BE635 = 0;
//     func_1510B070(arg1);
// }

typedef union Game476D0DisplayCommandA {
    struct {
        u32 word0;
        u32 word1;
    } words;
    u64 alignment;
} Game476D0DisplayCommandA;

typedef struct Game476D0RenderView {
    u8 unknown00[0x40];
    Vp viewports[2];
    u8 unknown60[0x120];
} Game476D0RenderView;

extern u8 D_8002C930;
extern Game476D0DisplayCommandA *D_800BE9C8[];

void func_1501A39C(void) {
    s32 i;
    Game476D0DisplayCommandA *gdl;

    for (i = 0; i < 2; i++) {
        gdl = D_800BE9C8[i];
        { Game476D0DisplayCommandA *command = gdl++; command->words.word0 = 0xDB060000; command->words.word1 = 0; }
        { Game476D0DisplayCommandA *command = gdl++; command->words.word0 = 0xDB060000; command->words.word1 = 0; }
        { Game476D0DisplayCommandA *command = gdl++; command->words.word0 = 0xDE000000; command->words.word1 = (u32)&D_8002C930; }
        { Game476D0DisplayCommandA *command = gdl++; command->words.word0 = 0xFE000000; command->words.word1 = D_800BE9C4; }
        { Game476D0DisplayCommandA *command = gdl++; command->words.word0 = 0xDC080008; command->words.word1 = (u32)&((Game476D0RenderView *)D_800BE628)[0].viewports[i]; }
        D_800BE9D8[i] = (s32)gdl;
    }
}
// NON-MATCHING: something along these lines
// extern Gfx* D_800BE9C8;
// extern s32 D_8002C930;
// Gfx *func_1501A39C(void) {
//     s32 i;
//     Gfx *tmp = D_800BE9C8;
//     for (i = 0; i < 3; i++) {
//         gSPSegment(tmp++, 0x00, 0x00000000);
//         gSPSegment(tmp++, 0x00, 0x00000000);
//         gSPDisplayList(tmp++, D_8002C930);
//         gDPSetDepthImage(tmp++, D_800BE9C4);
//         gSPViewport(tmp++, D_800BE628 + i + 0x40);
//     }
//     return tmp;
// }

/* Opens a frame's display list: pipe sync, then a scissor around view (or the whole drawable area for view
 * 0xFF), with its bottom edge clipped to maxBottom when that is non-zero. Returns the advanced display list. */
// matching: functions.h types the display list as s32 and IDO's -g3 frame only fits one Gfx * per command,
// so gDPPipeSync/gDPSetScissor are expanded by hand, each advancing gfx before its stores (the sync must stay
// on one line: -g3 line numbers steer the scheduler).
/* Emits a pipe sync and a scissor around view viewIndex (the whole drawable area for 0xFF), clipping its
 * bottom edge to maxBottom when that is non-zero. Returns the advanced display list. */
s32 func_1501A490(s32 gfx, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode) {
    { Gfx *cmd = (Gfx *)gfx; gfx += sizeof(Gfx); cmd->words.w0 = _SHIFTL(G_RDPPIPESYNC, 24, 8); cmd->words.w1 = 0; }
    if (viewIndex == 0xFF) {
        Gfx *cmd = (Gfx *)gfx;
        gfx += sizeof(Gfx);
        cmd->words.w0 = _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL((s32)(2.0f * 4.0f), 12, 12) | _SHIFTL((s32)(0.0f * 4.0f), 0, 12);
        cmd->words.w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((s32)((f32)(D_800BE620 - 2) * 4.0f), 12, 12) |
                        _SHIFTL((s32)((f32)D_800BE624 * 4.0f), 0, 12);
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
        {
            Gfx *cmd = (Gfx *)gfx;
            gfx += sizeof(Gfx);
            cmd->words.w0 = _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL((s32)(left * 4.0f), 12, 12) |
                            _SHIFTL((s32)(top * 4.0f), 0, 12);
            cmd->words.w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((s32)(right * 4.0f), 12, 12) |
                            _SHIFTL((s32)(bottom * 4.0f), 0, 12);
        }
    }
    return gfx;
}

Gfx *func_1501A680(Gfx *arg0) {
    gDPSetColorImage(arg0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_8002AAE8[D_800BE9C0]);
    return arg0;
}

Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d) {
    if (a < 3) {
        a = 2;
    }
    if (b <= 0) {
        b = 0;
    }
    if (c >= (D_800BE620 - 2)) {
        c = D_800BE620 - 2;
    }
    if (d >= D_800BE624) {
        d = D_800BE624;
    }

    gDPFillRectangle(arg0++, a, b, c, d);
    return arg0;
}

void func_150A7A00(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW);

/* Projects a world point through view's matrix to screen x, y and a depth value. */
void func_1501A764(s16 view, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *depth) {
    f32 projX;
    f32 projY;
    f32 projZ;
    f32 invW;

    func_150A7A00(VIEW_MATRICES[view], x, y, z, &projX, &projY, &projZ, &invW);
    invW = 1.0f / invW;
    *screenX = RENDER_VIEWS[view].centerX + (RENDER_VIEWS[view].halfWidth + 5.0f) * projX * invW;
    *screenY = RENDER_VIEWS[view].centerY - (RENDER_VIEWS[view].halfHeight + 5.0f) * projY * invW;
    *depth = (projZ * invW * RENDER_VIEWS[view].viewports[D_800BE9C0].vp.vscale[2] +
              RENDER_VIEWS[view].viewports[D_800BE9C0].vp.vtrans[2]) * 32.0f;
}

extern f32 D_80082FA8[];
extern s16 D_800BE668[][5][2];
extern u8 D_800BE630[];
void func_1501B0A0(s32 arg0, s32 arg1);

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

    RENDER_VIEWS[view].width = VIEW_WIDTHS[layout];
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
        fullWidth = VIEW_WIDTHS[0];
        fullAspect = fullWidth / (f32)D_800BE650[0];
        aspect = RENDER_VIEWS[view].width / RENDER_VIEWS[view].height;
        RENDER_VIEWS[view].unk14 = (RENDER_VIEWS[view].width / fullWidth) * (fullAspect / aspect);
        RENDER_VIEWS[view].unk18 = RENDER_VIEWS[view].height / (f32)D_800BE650[0];
    }
    halfFov = RENDER_VIEWS[view].fovX * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanX = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfWidth);
    halfFov = RENDER_VIEWS[view].fovY * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanY = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfHeight);
    vpWidth = RENDER_VIEWS[view].width + 10.0f;
    VIEWPORTS[view].vp.vscale[0] = vpWidth + vpWidth;
    vpHeight = RENDER_VIEWS[view].height + 10.0f;
    VIEWPORTS[view].vp.vscale[1] = vpHeight + vpHeight;
    VIEWPORTS[view].vp.vscale[2] = zRange >> 1;
    VIEWPORTS[view].vp.vscale[3] = 0;
    VIEWPORTS[view].vp.vtrans[0] = RENDER_VIEWS[view].centerX * 4.0f;
    VIEWPORTS[view].vp.vtrans[1] = RENDER_VIEWS[view].centerY * 4.0f;
    VIEWPORTS[view].vp.vtrans[2] = zRange >> 1;
    VIEWPORTS[view].vp.vtrans[3] = 0;
    do { // matching: the loop scope changes IDO's register allocation for the copies below
        RENDER_VIEWS[view].viewports[D_800BE9C0] = VIEWPORTS[view];
    } while (0);
    RENDER_VIEWS[view].viewports[!D_800BE9C0] = RENDER_VIEWS[view].viewports[D_800BE9C0];
    RENDER_VIEWS[view].unk84 = 1.0f;
    RENDER_VIEWS[view].onScreen = func_1501AE94(view);
    // matching: index + base (not RENDER_VIEWS[view]) sets IDO's addu operand order
    func_1501AF44(&(view + RENDER_VIEWS)->left, &(view + RENDER_VIEWS)->top, &(view + RENDER_VIEWS)->right,
                  &(view + RENDER_VIEWS)->bottom);
    func_1510B32C(view, 0.0f, 0.0f, RENDER_VIEWS[view].unk84);
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

void func_1510B128(s32, f32, f32, f32, f32);

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
    distX = (f32)VIEW_WIDTHS[layout] / (2.0f * tanX);
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
