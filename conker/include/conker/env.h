#ifndef CONKER_ENV_H
#define CONKER_ENV_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "env": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_3FC60.c
 *   game_FECC0.c
 *   game_13D350.c
 *   game_1A0790.c
 *   game_1A0E60.c
 */

/* A queued sky tile (game_13D350.c): its loaded texture and its format (the index into the per-format load tables). */
typedef struct SkyTile {
    s32 texture;
    s32 format;
} SkyTile;

/* game_3FC60.c */
void env_initVtxColours(void);
void env_recordVtxColours(Vtx *arg0, u32 arg1);
void env_recordEnvColours(Gfx *dl);
void env_clearAttachModelRefs(void);
extern u8 (*gVtxBaseColours[4])[3]; // [0]: base RGB of every level vertex, then the props'; [i]: level model i's first vertex
extern u8 (***gVtxGroupBaseColours)[3]; // [set][group][i]: base RGB of each color-group vertex
extern u16 *gPropVtxColourStarts;       // first gVtxBaseColours[0] entry of each prop

/* game_FECC0.c */
Gfx *env_drawSkyGradient(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);

/* game_13D350.c */
extern void *gSkyData;
extern Vtx *gSkyQuadVtx[4][2];
extern u8 gBackgroundColour[3];
extern u8 D_800DBEAB;
extern u8 D_800DBEAC;
extern Mtx *gSkyMatrices;
extern s32 *gSkyTextureSets[];
Gfx *env_drawSky(Gfx *gfx, s32 arg1);
void env_buildCameraRotation(MtxF mtx, f32 pitch, f32 yaw, f32 roll);
void env_buildViewProj(s32 viewIndex, MtxF mtx, f32 pitch, f32 yaw, f32 roll);
Gfx *env_drawBackgroundFill(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);
Gfx *env_fillRect(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, u8 r, u8 g, u8 b);
Gfx *env_drawMapBackgroundFill(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);
Gfx *env_clearDepth(Gfx *gfx);
s32 env_getSkyExtent(f32 x, f32 y, f32 z, f32 pitch, f32 yaw);
Gfx *env_drawSkyFace(Gfx *gfx, Vtx **vtxBufPtr, s32 firstTex, Vtx *quad, s32 arg4, u32 width, u32 height, u32 tileW,
                   u32 tileH, f32 x, f32 y, f32 z, u8 axis, MtxF mtx, s32 limit, s32 *vtxCountPtr, s32 *vtxIdxPtr,
                   SkyTile *queue, s32 *lastFormatPtr, u8 *sizes, u8 *shifts, s16 *palOffsets, u8 *tlutCounts,
                   u8 *tiles, s32 texSet);
void env_setSky(s32 sky);
void env_initStars(void);
Gfx *env_drawSkybox(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);

/* game_1A0790.c */
extern s8 gHasLevelFogPreset;     /* 1 when the level has its own fog preset */
extern u8 gFogColour[4];  /* the active fog: colour */
extern u16 D_800DD2E8;
extern u16 D_800DD2EA;
extern u16 D_800DD2EC;
extern u16 D_800DD2EE;
extern s16 gFogBlend;    /* blend of the last env_blendFog call */
extern s16 gFogNear;    /* near plane */
extern s16 gFogFar;    /* far plane */
void env_initFog(s32 arg0);
Gfx *func_151733D8(Gfx *gfx, s32 arg1);
void env_blendFog(s32 from, s32 to, s32 arg2, s32 blend, s32 dest);
void env_reapplyFog(s32 arg0);
s8 env_getLevelFogPreset(s32 arg0);

/* game_1A0E60.c */
void env_fadeVtxColours(s32 colorSet, s32 isProp, s32 blend, s32 group, s32 propIndex);
void env_fadeLevelVtxColours(s32 blend, s32 group);
void env_fadePropVtxColours(s32 blend, s32 group, s32 propId);

#endif
