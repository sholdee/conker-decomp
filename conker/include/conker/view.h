#ifndef CONKER_VIEW_H
#define CONKER_VIEW_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "view": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/View/ViewOrtho/viewortho.c
 *   game/Level/PipeBurstSetup/pipeburstsetup.c
 *   game/View/ViewLayout/viewlayout.c
 *   game/View/Views/views.c
 *   game/View/Projection/projection.c
 *   init/View/SetupDl/setupdl.c (init segment)
 */

/* game/View/ViewOrtho/viewortho.c */
void view_buildOrthoMatrices(void);
void view_initGroundDecalQueue(void);

/* game/Level/PipeBurstSetup/pipeburstsetup.c */
void level_initPipeBurstPoints(void);

/* game/View/ViewLayout/viewlayout.c */
void view_setPlayerCount(s16 count, s32 map);
void view_initLayouts(s32 width, s32 height);

/* game/View/Views/views.c */
void view_allocViews(s32 numPlayers, s32 arg1);
void view_initFrameDisplayLists(void);
Gfx *view_setScissor(Gfx *gfx, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode);
Gfx *view_setColorImage(Gfx *gfx);
Gfx *view_fillRect(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry);
void view_projectPoint(s16 view, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *depth);
void view_setupView(s32 view, s32 layout, s32 zRange, s16 arg3);
s32 view_isOnScreen(s32 viewIndex);
void view_clampRect(f32 *left, f32 *top, f32 *right, f32 *bottom);
void func_1501B0A0(s32 view, s32 layout);
void view_buildFrustumPlanes(s32 viewIndex);
extern s32 gScreenLayout; // the screen layout: the number of players
extern s32 gCurrentView; // the view being drawn
extern s32 gScreenWidth; // the screen width
extern s32 gScreenHeight; // the screen height
extern RenderView *gViews; // the views
extern Vp *gViewports; // one viewport per view
extern u8 gViewPlayers[5]; // the player of each view
extern u8 D_800BE635;
extern s32 gLayoutViewWidths[6]; // the view width of each screen layout
extern s32 gLayoutViewHeights[6]; // the view height of each screen layout
extern s16 gLayoutViewCentres[4][5][2]; // the centre (x, y) of each view, by screen layout
extern f32 gTwoPlayerWidthScale; // the width scale of the two-player layout
extern Gfx gModelXluRenderModes[24][2]; // render mode tables for segment 8
extern Gfx gModelOpaRenderModes[24][2];
extern Gfx D_800832C0[24][2];
extern Gfx D_80083440[24][2];
extern Gfx D_800835C0[24][2];
extern Gfx gEffectXluRenderModes[24][2];
extern Gfx gEffectOpaRenderModes[24][2];
extern Gfx D_80083A40[24][2];
extern Gfx D_80083BC0[24][2];
extern Gfx gWorldObjectXluRenderModes[24][2];
extern Gfx gWorldObjectOpaRenderModes[24][2];

/* init/View/SetupDl/setupdl.c (a data-only unit in the init segment) */
extern const Gfx D_8002C930[]; // the setup display list of each frame (view_initFrameDisplayLists)

/* game/View/Projection/projection.c */
extern Vec3f gFovChanges[4];
extern u8 gFovChangePending;
extern ViewClip gViewClips[4];
extern u8 gClipChangePending;
extern f32 gDefaultFar;
extern f32 gDefaultNear;
extern u8 gTintPrimColors[4][3];
extern u8 gTintEnvColors[4][3];
extern u8 D_800D9B84[3];
extern u8 D_800D9B88[3];
extern u8 D_800D9B8B;
extern u8 D_800D9B8C;
extern LookAt gLookAts[2];
extern Ambient gViewAmbients[4][2];
extern MtxF gViewMatrices[4];
extern MtxF gViewProjMatrices[4];
extern PointLightGfx *gViewGfxLights[4];
extern u8 gViewGfxLightMax;
extern u8 gViewGfxLightCount;
extern PointLight *gPickedLights[14];
void view_initDefaultProjection(s32 arg0);
void view_widenFov(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale, f32 near);
void view_recordFovChange(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale);
void view_reapplyFovChanges(void);
void view_setClipPlanes(s32 viewIndex, f32 near, f32 far);
void view_reapplyClipPlanes(void);
void view_buildProjection(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void view_updateMatrices(void);
Gfx *view_emitRspSetup(Gfx *gfx, s32 view);
void func_1510B958(s32 view);
Gfx *view_buildWorldDisplayList(Gfx *gfx, s16 view);

#endif
