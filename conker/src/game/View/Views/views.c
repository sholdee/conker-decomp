#include <ultra64.h>
#include "structs.h"

/* This file's .bss, in address order, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
s32 gScreenWidth;              // the screen width
s32 gScreenHeight;              // the screen height
RenderView *gViews;      // the views
Vp *gViewports;              // one viewport per view
u8 gViewPlayers[5];            // the player of each view
u8 D_800BE635;
s32 gLayoutViewWidths[6];           // the view width of each screen layout
s32 gLayoutViewHeights[6];           // the view height of each screen layout
s16 gLayoutViewCentres[4][5][2];     // the centre (x, y) of each view, by screen layout
f32 gTwoPlayerWidthScale;              // the width scale of the two-player layout

#include <math.h>
#include "conker/boot.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/math.h"
#include "conker/music.h"
#include "conker/view.h"


// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define RENDER_VIEWS ((RenderView *)gViews)

/* The screen layout: the number of players (0 for one full-screen view). */
s32 gScreenLayout = 0;
/* The view being drawn. Callers that take an s16 view read its low half. */
s32 gCurrentView = 0;
/* The unk64 value of each screen layout's views. */
static f32 D_80082FA8[6] = { 1.659999967f, 0.8299999833f, 1.679999948f, 1.679999948f, 1.659999967f, 0.0f };

/* A one-command display list that sets the other modes (high and low words). */
#define RENDER_MODE(hi, lo) { gsDPSetOtherMode(hi, lo), gsSPEndDisplayList() }

/*
 * Render mode tables. Code binds one of them to segment 8. A model's display lists then call entry n
 * (at segment offset n * 16) to set the render mode of a part.
 */
Gfx gModelXluRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x04D049D8), RENDER_MODE(0x182C3F, 0x04D049D8),
    RENDER_MODE(0x182C3F, 0x04D04DD8), RENDER_MODE(0x182C3F, 0x04D049D8),
    RENDER_MODE(0x18AC3F, 0x04D049D8), RENDER_MODE(0x18AC3F, 0x04D049D8),
    RENDER_MODE(0x18AC3F, 0x04D04DD8), RENDER_MODE(0x18AC3F, 0x04D049D8),
    RENDER_MODE(0x18EC3F, 0x04D049D8), RENDER_MODE(0x18EC3F, 0x04D049D8),
    RENDER_MODE(0x18EC3F, 0x04D04DD8), RENDER_MODE(0x18EC3F, 0x04D049D8),
    RENDER_MODE(0x192C3F, 0x04D049D8), RENDER_MODE(0x192C3F, 0x04D049D8),
    RENDER_MODE(0x192C3F, 0x04D04DD8), RENDER_MODE(0x192C3F, 0x04D049D8),
    RENDER_MODE(0x19AC3F, 0x04D049D8), RENDER_MODE(0x19AC3F, 0x04D049D8),
    RENDER_MODE(0x19AC3F, 0x04D04DD8), RENDER_MODE(0x19AC3F, 0x04D049D8),
    RENDER_MODE(0x19EC3F, 0x04D049D8), RENDER_MODE(0x19EC3F, 0x04D049D8),
    RENDER_MODE(0x19EC3F, 0x04D04DD8), RENDER_MODE(0x19EC3F, 0x04D049D8),
};

Gfx gModelOpaRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x04D12078), RENDER_MODE(0x182C3F, 0x04D13078),
    RENDER_MODE(0x182C3F, 0x04D12D58), RENDER_MODE(0x182C3F, 0x04D12278),
    RENDER_MODE(0x18AC3F, 0x04D12078), RENDER_MODE(0x18AC3F, 0x04D13078),
    RENDER_MODE(0x18AC3F, 0x04D12D58), RENDER_MODE(0x18AC3F, 0x04D12278),
    RENDER_MODE(0x18EC3F, 0x04D12078), RENDER_MODE(0x18EC3F, 0x04D13078),
    RENDER_MODE(0x18EC3F, 0x04D12D58), RENDER_MODE(0x18EC3F, 0x04D12278),
    RENDER_MODE(0x192C3F, 0x04D12078), RENDER_MODE(0x192C3F, 0x04D13078),
    RENDER_MODE(0x192C3F, 0x04D12D58), RENDER_MODE(0x192C3F, 0x04D12278),
    RENDER_MODE(0x19AC3F, 0x04D12078), RENDER_MODE(0x19AC3F, 0x04D13078),
    RENDER_MODE(0x19AC3F, 0x04D12D58), RENDER_MODE(0x19AC3F, 0x04D12278),
    RENDER_MODE(0x19EC3F, 0x04D12078), RENDER_MODE(0x19EC3F, 0x04D13078),
    RENDER_MODE(0x19EC3F, 0x04D12D58), RENDER_MODE(0x19EC3F, 0x04D12278),
};

Gfx D_800832C0[24][2] = {
    RENDER_MODE(0x182C3F, 0x07C24000), RENDER_MODE(0x182C3F, 0x07C27008),
    RENDER_MODE(0x182C3F, 0x07C24000), RENDER_MODE(0x182C3F, 0x07C24000),
    RENDER_MODE(0x18AC3F, 0x07C24000), RENDER_MODE(0x18AC3F, 0x07C27008),
    RENDER_MODE(0x18AC3F, 0x07C24000), RENDER_MODE(0x18AC3F, 0x07C24000),
    RENDER_MODE(0x18EC3F, 0x07C24000), RENDER_MODE(0x18EC3F, 0x07C27008),
    RENDER_MODE(0x18EC3F, 0x07C24000), RENDER_MODE(0x18EC3F, 0x07C24000),
    RENDER_MODE(0x192C3F, 0x07C24000), RENDER_MODE(0x192C3F, 0x07C27008),
    RENDER_MODE(0x192C3F, 0x07C24000), RENDER_MODE(0x192C3F, 0x07C24000),
    RENDER_MODE(0x19AC3F, 0x07C24000), RENDER_MODE(0x19AC3F, 0x07C27008),
    RENDER_MODE(0x19AC3F, 0x07C24000), RENDER_MODE(0x19AC3F, 0x07C24000),
    RENDER_MODE(0x19EC3F, 0x07C24000), RENDER_MODE(0x19EC3F, 0x07C27008),
    RENDER_MODE(0x19EC3F, 0x07C24000), RENDER_MODE(0x19EC3F, 0x07C24000),
};

Gfx gModelXluNoZRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x04D041C8), RENDER_MODE(0x182C3F, 0x04D041C8),
    RENDER_MODE(0x182C3F, 0x04D041C8), RENDER_MODE(0x182C3F, 0x04D041C8),
    RENDER_MODE(0x18AC3F, 0x04D041C8), RENDER_MODE(0x18AC3F, 0x04D041C8),
    RENDER_MODE(0x18AC3F, 0x04D041C8), RENDER_MODE(0x18AC3F, 0x04D041C8),
    RENDER_MODE(0x18EC3F, 0x04D041C8), RENDER_MODE(0x18EC3F, 0x04D041C8),
    RENDER_MODE(0x18EC3F, 0x04D041C8), RENDER_MODE(0x18EC3F, 0x04D041C8),
    RENDER_MODE(0x192C3F, 0x04D041C8), RENDER_MODE(0x192C3F, 0x04D041C8),
    RENDER_MODE(0x192C3F, 0x04D041C8), RENDER_MODE(0x192C3F, 0x04D041C8),
    RENDER_MODE(0x19AC3F, 0x04D041C8), RENDER_MODE(0x19AC3F, 0x04D041C8),
    RENDER_MODE(0x19AC3F, 0x04D041C8), RENDER_MODE(0x19AC3F, 0x04D041C8),
    RENDER_MODE(0x19EC3F, 0x04D041C8), RENDER_MODE(0x19EC3F, 0x04D041C8),
    RENDER_MODE(0x19EC3F, 0x04D041C8), RENDER_MODE(0x19EC3F, 0x04D041C8),
};

Gfx gModelOpaNoZRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x04D12048), RENDER_MODE(0x182C3F, 0x04D13048),
    RENDER_MODE(0x182C3F, 0x04D12048), RENDER_MODE(0x182C3F, 0x04D12248),
    RENDER_MODE(0x18AC3F, 0x04D12048), RENDER_MODE(0x18AC3F, 0x04D13048),
    RENDER_MODE(0x18AC3F, 0x04D12048), RENDER_MODE(0x18AC3F, 0x04D12248),
    RENDER_MODE(0x18EC3F, 0x04D12048), RENDER_MODE(0x18EC3F, 0x04D13048),
    RENDER_MODE(0x18EC3F, 0x04D12048), RENDER_MODE(0x18EC3F, 0x04D12248),
    RENDER_MODE(0x192C3F, 0x04D12048), RENDER_MODE(0x192C3F, 0x04D13048),
    RENDER_MODE(0x192C3F, 0x04D12048), RENDER_MODE(0x192C3F, 0x04D12248),
    RENDER_MODE(0x19AC3F, 0x04D12048), RENDER_MODE(0x19AC3F, 0x04D13048),
    RENDER_MODE(0x19AC3F, 0x04D12048), RENDER_MODE(0x19AC3F, 0x04D12248),
    RENDER_MODE(0x19EC3F, 0x04D12048), RENDER_MODE(0x19EC3F, 0x04D13048),
    RENDER_MODE(0x19EC3F, 0x04D12048), RENDER_MODE(0x19EC3F, 0x04D12248),
};

Gfx gEffectXluRenderModes[24][2] = {
    RENDER_MODE(0x182CAF, 0x0C1849D8), RENDER_MODE(0x182CAF, 0x0C1849D8),
    RENDER_MODE(0x182CAF, 0x0C184DD8), RENDER_MODE(0x182CAF, 0x0C1849D8),
    RENDER_MODE(0x18ACAF, 0x0C1849D8), RENDER_MODE(0x18ACAF, 0x0C1849D8),
    RENDER_MODE(0x18ACAF, 0x0C184DD8), RENDER_MODE(0x18ACAF, 0x0C1849D8),
    RENDER_MODE(0x18ECAF, 0x0C1849D8), RENDER_MODE(0x18ECAF, 0x0C1849D8),
    RENDER_MODE(0x18ECAF, 0x0C184DD8), RENDER_MODE(0x18ECAF, 0x0C1849D8),
    RENDER_MODE(0x192CAF, 0x0C1849D8), RENDER_MODE(0x192CAF, 0x0C1849D8),
    RENDER_MODE(0x192CAF, 0x0C184DD8), RENDER_MODE(0x192CAF, 0x0C1849D8),
    RENDER_MODE(0x19ACAF, 0x0C1849D8), RENDER_MODE(0x19ACAF, 0x0C1849D8),
    RENDER_MODE(0x19ACAF, 0x0C184DD8), RENDER_MODE(0x19ACAF, 0x0C1849D8),
    RENDER_MODE(0x19ECAF, 0x0C1849D8), RENDER_MODE(0x19ECAF, 0x0C1849D8),
    RENDER_MODE(0x19ECAF, 0x0C184DD8), RENDER_MODE(0x19ECAF, 0x0C1849D8),
};

Gfx gEffectOpaRenderModes[24][2] = {
    RENDER_MODE(0x182CAF, 0x0C192078), RENDER_MODE(0x182CAF, 0x0C193078),
    RENDER_MODE(0x182CAF, 0x0C192D58), RENDER_MODE(0x182CAF, 0x0C192278),
    RENDER_MODE(0x18ACAF, 0x0C192078), RENDER_MODE(0x18ACAF, 0x0C193078),
    RENDER_MODE(0x18ACAF, 0x0C192D58), RENDER_MODE(0x18ACAF, 0x0C192278),
    RENDER_MODE(0x18ECAF, 0x0C192078), RENDER_MODE(0x18ECAF, 0x0C193078),
    RENDER_MODE(0x18ECAF, 0x0C192D58), RENDER_MODE(0x18ECAF, 0x0C192278),
    RENDER_MODE(0x192CAF, 0x0C192078), RENDER_MODE(0x192CAF, 0x0C193078),
    RENDER_MODE(0x192CAF, 0x0C192D58), RENDER_MODE(0x192CAF, 0x0C192278),
    RENDER_MODE(0x19ACAF, 0x0C192078), RENDER_MODE(0x19ACAF, 0x0C193078),
    RENDER_MODE(0x19ACAF, 0x0C192D58), RENDER_MODE(0x19ACAF, 0x0C192278),
    RENDER_MODE(0x19ECAF, 0x0C192078), RENDER_MODE(0x19ECAF, 0x0C193078),
    RENDER_MODE(0x19ECAF, 0x0C192D58), RENDER_MODE(0x19ECAF, 0x0C192278),
};

Gfx gEffectXluNoZRenderModes[24][2] = {
    RENDER_MODE(0x182CAF, 0x0C1841C8), RENDER_MODE(0x182CAF, 0x0C1841C8),
    RENDER_MODE(0x182CAF, 0x0C184DD8), RENDER_MODE(0x182CAF, 0x0C1841C8),
    RENDER_MODE(0x18ACAF, 0x0C1841C8), RENDER_MODE(0x18ACAF, 0x0C1841C8),
    RENDER_MODE(0x18ACAF, 0x0C184DD8), RENDER_MODE(0x18ACAF, 0x0C1841C8),
    RENDER_MODE(0x18ECAF, 0x0C1841C8), RENDER_MODE(0x18ECAF, 0x0C1841C8),
    RENDER_MODE(0x18ECAF, 0x0C184DD8), RENDER_MODE(0x18ECAF, 0x0C1841C8),
    RENDER_MODE(0x192CAF, 0x0C1841C8), RENDER_MODE(0x192CAF, 0x0C1841C8),
    RENDER_MODE(0x192CAF, 0x0C184DD8), RENDER_MODE(0x192CAF, 0x0C1841C8),
    RENDER_MODE(0x19ACAF, 0x0C1841C8), RENDER_MODE(0x19ACAF, 0x0C1841C8),
    RENDER_MODE(0x19ACAF, 0x0C184DD8), RENDER_MODE(0x19ACAF, 0x0C1841C8),
    RENDER_MODE(0x19ECAF, 0x0C1841C8), RENDER_MODE(0x19ECAF, 0x0C1841C8),
    RENDER_MODE(0x19ECAF, 0x0C184DD8), RENDER_MODE(0x19ECAF, 0x0C1841C8),
};

Gfx gEffectOpaNoZRenderModes[24][2] = {
    RENDER_MODE(0x182CAF, 0x0C192048), RENDER_MODE(0x182CAF, 0x0C193048),
    RENDER_MODE(0x182CAF, 0x0C192D58), RENDER_MODE(0x182CAF, 0x0C192248),
    RENDER_MODE(0x18ACAF, 0x0C192048), RENDER_MODE(0x18ACAF, 0x0C193048),
    RENDER_MODE(0x18ACAF, 0x0C192D58), RENDER_MODE(0x18ACAF, 0x0C192248),
    RENDER_MODE(0x18ECAF, 0x0C192048), RENDER_MODE(0x18ECAF, 0x0C193048),
    RENDER_MODE(0x18ECAF, 0x0C192D58), RENDER_MODE(0x18ECAF, 0x0C192248),
    RENDER_MODE(0x192CAF, 0x0C192048), RENDER_MODE(0x192CAF, 0x0C193048),
    RENDER_MODE(0x192CAF, 0x0C192D58), RENDER_MODE(0x192CAF, 0x0C192248),
    RENDER_MODE(0x19ACAF, 0x0C192048), RENDER_MODE(0x19ACAF, 0x0C193048),
    RENDER_MODE(0x19ACAF, 0x0C192D58), RENDER_MODE(0x19ACAF, 0x0C192248),
    RENDER_MODE(0x19ECAF, 0x0C192048), RENDER_MODE(0x19ECAF, 0x0C193048),
    RENDER_MODE(0x19ECAF, 0x0C192D58), RENDER_MODE(0x19ECAF, 0x0C192248),
};

Gfx gWorldObjectXluRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x0C1849D8), RENDER_MODE(0x182C3F, 0x0C1849D8),
    RENDER_MODE(0x182C3F, 0x0C184DD8), RENDER_MODE(0x182C3F, 0x0C1849D8),
    RENDER_MODE(0x18AC3F, 0x0C1849D8), RENDER_MODE(0x18AC3F, 0x0C1849D8),
    RENDER_MODE(0x18AC3F, 0x0C184DD8), RENDER_MODE(0x18AC3F, 0x0C1849D8),
    RENDER_MODE(0x18EC3F, 0x0C1849D8), RENDER_MODE(0x18EC3F, 0x0C1849D8),
    RENDER_MODE(0x18EC3F, 0x0C184DD8), RENDER_MODE(0x18EC3F, 0x0C1849D8),
    RENDER_MODE(0x192C3F, 0x0C1849D8), RENDER_MODE(0x192C3F, 0x0C1849D8),
    RENDER_MODE(0x192C3F, 0x0C184DD8), RENDER_MODE(0x192C3F, 0x0C1849D8),
    RENDER_MODE(0x19AC3F, 0x0C1849D8), RENDER_MODE(0x19AC3F, 0x0C1849D8),
    RENDER_MODE(0x19AC3F, 0x0C184DD8), RENDER_MODE(0x19AC3F, 0x0C1849D8),
    RENDER_MODE(0x19EC3F, 0x0C1849D8), RENDER_MODE(0x19EC3F, 0x0C1849D8),
    RENDER_MODE(0x19EC3F, 0x0C184DD8), RENDER_MODE(0x19EC3F, 0x0C1849D8),
};

Gfx gWorldObjectOpaRenderModes[24][2] = {
    RENDER_MODE(0x182C3F, 0x0C192078), RENDER_MODE(0x182C3F, 0x0C193078),
    RENDER_MODE(0x182C3F, 0x0C192D58), RENDER_MODE(0x182C3F, 0x0C192278),
    RENDER_MODE(0x18AC3F, 0x0C192078), RENDER_MODE(0x18AC3F, 0x0C193078),
    RENDER_MODE(0x18AC3F, 0x0C192D58), RENDER_MODE(0x18AC3F, 0x0C192278),
    RENDER_MODE(0x18EC3F, 0x0C192078), RENDER_MODE(0x18EC3F, 0x0C193078),
    RENDER_MODE(0x18EC3F, 0x0C192D58), RENDER_MODE(0x18EC3F, 0x0C192278),
    RENDER_MODE(0x192C3F, 0x0C192078), RENDER_MODE(0x192C3F, 0x0C193078),
    RENDER_MODE(0x192C3F, 0x0C192D58), RENDER_MODE(0x192C3F, 0x0C192278),
    RENDER_MODE(0x19AC3F, 0x0C192078), RENDER_MODE(0x19AC3F, 0x0C193078),
    RENDER_MODE(0x19AC3F, 0x0C192D58), RENDER_MODE(0x19AC3F, 0x0C192278),
    RENDER_MODE(0x19EC3F, 0x0C192078), RENDER_MODE(0x19EC3F, 0x0C193078),
    RENDER_MODE(0x19EC3F, 0x0C192D58), RENDER_MODE(0x19EC3F, 0x0C192278),
};

#undef RENDER_MODE

// matching: a one-element const table; with a 0.8f literal IDO picks other FP registers in view_allocViews
static const f32 D_800968E0[] = { 0.8f };

/* Allocates the views for numPlayers players (numPlayers + 2 of them, or 1 when it is 0) and lays out views
 * 0..numPlayers with layout numPlayers, plus a layout-0 (full-screen) view after them. */
void view_allocViews(s32 numPlayers, s32 arg1) {
    s32 numViews;
    s32 i;
    s32 layout;
    f32 width;
    f32 scaled;
    s32 scaledWidth;

    gScreenLayout = numPlayers;
    layout = gScreenLayout;

    if (layout == 0) {
        numViews = 1;
    } else {
        numViews = layout + 2;
    }

    gViews = allocate_memory(numViews * sizeof(RenderView), 1, 1, 0);
    gViewports = allocate_memory(numViews * sizeof(Vp), 1, 1, 0);

    if (gScreenLayout == 1) {
        width = gLayoutViewWidths[1] - 2.0f;
        scaled = width;
        scaled *= D_800968E0[0];
        scaledWidth = scaled;
        gTwoPlayerWidthScale = (scaledWidth - (scaledWidth % 4)) / width;
    }

    music_initLevelViewState(level);

    for (i = 0; i <= (layout = gScreenLayout); i++) {
        view_setupView(i, layout, 1023, 0);
    }

    if (layout != 0) {
        view_setupView(i, 0, 1023, 0);
    }

    gViewLayoutChanged = 1;
    gRebuildFrameDisplayLists = 1;
    D_800BE635 = 0;
    view_initDefaultProjection(arg1);
}

/* Writes the start of each framebuffer's display list (segment 0, the setup list gFrameSetupDL, the depth image
 * and view 0's viewport for that framebuffer) and stores where each one ends in gDisplayListPrologueEnds. */
void view_initFrameDisplayLists(void) {
    s32 i;
    Gfx *gfx;

    for (i = 0; i < 2; i++) {
        gfx = gDisplayListBuffers[i];
        gSPSegment(gfx++, 0, 0);
        gSPSegment(gfx++, 0, 0);
        gSPDisplayList(gfx++, gFrameSetupDL);
        gDPSetDepthImage(gfx++, gZBuffer);
        gSPViewport(gfx++, &RENDER_VIEWS[0].viewports[i]);
        gDisplayListPrologueEnds[i] = gfx;
    }
}

/* Emits a pipe sync and a scissor around view viewIndex (the whole drawable area for 0xFF), clipping its
 * bottom edge to maxBottom when that is non-zero. Returns the advanced display list. */
Gfx *view_setScissor(Gfx *gfx, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode) {
    gDPPipeSync(gfx++);
    if (viewIndex == 0xFF) {
        gDPSetScissor(gfx++, mode, 2, 0, gScreenWidth - 2, gScreenHeight);
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
            if (maxBottom < view->bottom) {
                bottom = maxBottom;
            } else {
                bottom = view->bottom;
            }
        } else {
            bottom = view->bottom;
        }
        view_clampRect(&left, &top, &right, &bottom);
        gDPSetScissor(gfx++, mode, left, top, right, bottom);
    }
    return gfx;
}

/* Sets the color image to the current framebuffer. */
Gfx *view_setColorImage(Gfx *gfx) {
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gFramebuffers[gDrawBufferIndex]);
    return gfx;
}

/* Fills a rectangle clamped to the drawable framebuffer area. */
Gfx *view_fillRect(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    if (ulx < 3) {
        ulx = 2;
    }
    if (uly <= 0) {
        uly = 0;
    }
    if (lrx >= gScreenWidth - 2) {
        lrx = gScreenWidth - 2;
    }
    if (lry >= gScreenHeight) {
        lry = gScreenHeight;
    }

    gDPFillRectangle(gfx++, ulx, uly, lrx, lry);
    return gfx;
}

/* Projects a world point through view's matrix to screen x, y and a depth value. */
void view_projectPoint(s16 view, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *depth) {
    f32 projX;
    f32 projY;
    f32 projZ;
    f32 invW;

    math_transformPointW(gViewProjMatrices[view], x, y, z, &projX, &projY, &projZ, &invW);
    invW = 1.0f / invW;
    *screenX = RENDER_VIEWS[view].centerX + (RENDER_VIEWS[view].halfWidth + 5.0f) * projX * invW;
    *screenY = RENDER_VIEWS[view].centerY - (RENDER_VIEWS[view].halfHeight + 5.0f) * projY * invW;
    *depth = (projZ * invW * RENDER_VIEWS[view].viewports[gDrawBufferIndex].vp.vscale[2] +
              RENDER_VIEWS[view].viewports[gDrawBufferIndex].vp.vtrans[2]) * 32.0f;
}

/* Lays out view `view` for screen layout `layout` (gLayoutViewCentres holds each view's centre) and sets up its
 * projection and viewport, with depth range zRange. */
void view_setupView(s32 view, s32 layout, s32 zRange, s16 arg3) {
    f32 pad[3]; // matching: unused locals that size the -g3 stack frame
    f32 halfFov;
    f32 sine;
    f32 fullWidth;
    f32 fullAspect;
    f32 aspect;
    f32 vpWidth;
    f32 vpHeight;
    s16 *center;

    RENDER_VIEWS[view].width = gLayoutViewWidths[layout];
    RENDER_VIEWS[view].height = gLayoutViewHeights[layout];
    if (layout != 0) {
        RENDER_VIEWS[view].width -= 2.0f;
        RENDER_VIEWS[view].height -= 2.0f;
    }
    RENDER_VIEWS[view].unk64 = D_80082FA8[layout];
    RENDER_VIEWS[view].unk68 = 2.0f;
    if (layout == 1) {
        RENDER_VIEWS[view].width *= gTwoPlayerWidthScale;
        RENDER_VIEWS[view].fovX = 103.02f * gTwoPlayerWidthScale;
        RENDER_VIEWS[view].fovY = 50.0f;
    } else {
        RENDER_VIEWS[view].fovX = 60.6f;
        RENDER_VIEWS[view].fovY = 50.0f;
    }
    RENDER_VIEWS[view].frustumFovX = RENDER_VIEWS[view].fovX;
    RENDER_VIEWS[view].frustumFovY = RENDER_VIEWS[view].fovY;
    center = gLayoutViewCentres[layout][view];
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
        fullWidth = gLayoutViewWidths[0];
        fullAspect = fullWidth / gLayoutViewHeights[0];
        aspect = RENDER_VIEWS[view].width / RENDER_VIEWS[view].height;
        RENDER_VIEWS[view].scaleX = (RENDER_VIEWS[view].width / fullWidth) * (fullAspect / aspect);
        RENDER_VIEWS[view].scaleY = RENDER_VIEWS[view].height / gLayoutViewHeights[0];
    }
    halfFov = RENDER_VIEWS[view].fovX * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanX = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfWidth);
    halfFov = RENDER_VIEWS[view].fovY * 0.5f * 0.01745329238f;
    sine = sinf(halfFov);
    RENDER_VIEWS[view].pixelTanY = sine / (cosf(halfFov) * RENDER_VIEWS[view].halfHeight);
    vpWidth = RENDER_VIEWS[view].width + 10.0f;
    gViewports[view].vp.vscale[0] = vpWidth + vpWidth;
    vpHeight = RENDER_VIEWS[view].height + 10.0f;
    gViewports[view].vp.vscale[1] = vpHeight + vpHeight;
    gViewports[view].vp.vscale[2] = zRange >> 1;
    gViewports[view].vp.vscale[3] = 0;
    gViewports[view].vp.vtrans[0] = RENDER_VIEWS[view].centerX * 4.0f;
    gViewports[view].vp.vtrans[1] = RENDER_VIEWS[view].centerY * 4.0f;
    gViewports[view].vp.vtrans[2] = zRange >> 1;
    gViewports[view].vp.vtrans[3] = 0;
    do { // matching: the loop scope changes IDO's register allocation for the copies below
        RENDER_VIEWS[view].viewports[gDrawBufferIndex] = gViewports[view];
    } while (0);
    RENDER_VIEWS[view].viewports[!gDrawBufferIndex] = RENDER_VIEWS[view].viewports[gDrawBufferIndex];
    RENDER_VIEWS[view].perspScale = 1.0f;
    RENDER_VIEWS[view].onScreen = view_isOnScreen(view);
    // matching: index + base (not RENDER_VIEWS[view]) sets IDO's addu operand order
    view_clampRect(&(view + RENDER_VIEWS)->left, &(view + RENDER_VIEWS)->top, &(view + RENDER_VIEWS)->right,
                  &(view + RENDER_VIEWS)->bottom);
    view_recordFovChange(view, 0.0f, 0.0f, RENDER_VIEWS[view].perspScale);
    gViewPlayers[view] = view;
}

/* Returns whether view's rectangle lies entirely inside the drawable framebuffer area. */
s32 view_isOnScreen(s32 viewIndex) {
    f32 maxX = gScreenWidth - 2.0f;
    f32 maxY = gScreenHeight;
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
void view_clampRect(f32 *left, f32 *top, f32 *right, f32 *bottom) {
    f32 clamped;

    if (*left < 2.0f) {
        *left = 2.0f;
    } else {
        if (gScreenWidth - 2.0f < *left) {
            clamped = gScreenWidth - 2.0f;
        } else {
            clamped = *left;
        }
        *left = clamped;
    }

    if (*right < 2.0f) {
        *right = 2.0f;
    } else {
        if (gScreenWidth - 2.0f < *right) {
            clamped = gScreenWidth - 2.0f;
        } else {
            clamped = *right;
        }
        *right = clamped;
    }

    if (*top < 0.0f) {
        *top = 0.0f;
    } else {
        if (gScreenHeight < *top) {
            clamped = gScreenHeight;
        } else {
            clamped = *top;
        }
        *top = clamped;
    }

    if (*bottom < 0.0f) {
        *bottom = 0.0f;
    } else {
        if (gScreenHeight < *bottom) {
            clamped = gScreenHeight;
        } else {
            clamped = *bottom;
        }
        *bottom = clamped;
    }
}

/* Passes view_widenFov the angles (degrees) that view's half-size subtends at the distance where the layout's
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
    distX = gLayoutViewWidths[layout] / (2.0f * tanX);
    distY = gLayoutViewHeights[layout] / (2.0f * tanY);
    x = math_atan2(RENDER_VIEWS[view].halfWidth, distX) * 114.59155f - RENDER_VIEWS[layout].fovX;
    y = math_atan2(RENDER_VIEWS[view].halfHeight, distY) * 114.59155f - RENDER_VIEWS[layout].fovY;
    view_widenFov(view, x, y, 1.0f, 0.0f);
}

/* Builds the four side-plane normals of view's frustum from its frustum fov. */
void view_buildFrustumPlanes(s32 viewIndex) {
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
