#ifndef CONKER_SHADOW_H
#define CONKER_SHADOW_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "shadow": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_13AE20.c
 *   game_1B1600.c
 */

/* A shadow-polygon vertex while it is clipped in light space. */
typedef struct ClipVtx {
    f32 x;       /* light-space position, clipped against the shadow frustum */
    f32 y;
    f32 z;
    s16 modelX;  /* model-space position, interpolated along */
    s16 modelY;
    s16 modelZ;
} ClipVtx;       /* size 0x14 */

/* A shadow-receiving vertex: light-space position and a pointer to its model-space position. */
typedef struct ShadowVtx {
    f32 x;
    f32 y;
    f32 z;
    s16 *model;
    s32 unk10;
} ShadowVtx; /* size 0x14 */

/* game_13AE20.c */
extern Gfx gBlobShadowSetupDl[]; /* render setup (object type table D_8008B4A8) */
Shadow *shadow_createBlob(s32 ownerType, void *owner, s32 texture, s32 createFlags, s32 flags);
void shadow_updateBlob(Shadow *shadow);
Gfx *shadow_drawBlob(Gfx *gfx, Shadow *shadow, s16 view);
s32 shadow_getTriSlopes(Vec3s **triPtrs, Vec3s *tri, f32 *slopeX, f32 *slopeZ);
Gfx *func_1510E634(Gfx *gfx, s32 arg1, s32 arg2);

/* game_1B1600.c */
extern u8 D_800DF0E0;
extern u8 D_800DF0E1; /* nonzero: halve the light distance term of the shadow near plane */
s32 shadow_getObjectHalfExtents(s32 objIndex, f32 *halfX, f32 *halfY, f32 *halfZ);
f32 shadow_setupLight(s32 objIndex, s32 slot, f32 halfX, f32 halfY, f32 halfZ, f32 minDist);
s32 shadow_getObjectBounds(s32 objIndex, s32 view, f32 *halfX, f32 *halfY, f32 *halfZ);
Gfx *shadow_renderSilhouette(Gfx *gfx, s32 objIndex, s32 slot, s32 view);
ClipVtx *shadow_clipEdgeYZ(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideYZ(ClipVtx *v);
ClipVtx *shadow_clipEdgeXNegZ(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideXNegZ(ClipVtx *v);
ClipVtx *shadow_clipEdgeYNegZ(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideYNegZ(ClipVtx *v);
ClipVtx *shadow_clipEdgeXZ(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideXZ(ClipVtx *v);
ClipVtx *shadow_clipEdgeNear(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideNear(ClipVtx *v);
ClipVtx *shadow_clipEdgeFar(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isInsideFar(ClipVtx *v);
ClipVtx *shadow_clipEdgeX0(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isXNonPositive(ClipVtx *v);
ClipVtx *shadow_clipEdgeY0(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 shadow_isYNonPositive(ClipVtx *v);
s32 shadow_isXPositive(ClipVtx *v);
s32 shadow_isYPositive(ClipVtx *v);
ClipVtx *shadow_clipPolygonToPlane(ClipVtx *in, s32 count, s32 plane, ClipVtx *out);
s32 shadow_emitClippedPolygon(ShadowVtx *in, s32 count, s32 maxVerts, Vtx *out, s32 *quadEnd, s32 split);
s32 shadow_gatherLevelFaces(s32 x, s32 z, s32 radius, ShadowVtx *out, f32 *mtx, ShadowVtx *end);
Gfx *shadow_drawRealTime(Gfx *gfx, s32 view);
void shadow_initLevelSettings(s32 levelId);
s32 shadow_gatherObjectFaces(s32 x, s32 z, s32 range, ShadowVtx *out, f32 *mtx, ShadowVtx *end, s32 count);

#endif
