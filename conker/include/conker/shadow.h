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
extern Gfx D_800890C0[]; /* render setup (object type table D_8008B4A8) */
Shadow *func_1510D970(s32 ownerType, void *owner, s32 texture, s32 createFlags, s32 flags);
void func_1510DA84(Shadow *shadow);
Gfx *func_1510E120(Gfx *gfx, Shadow *shadow, s16 view);
s32 func_1510E388(Vec3s **triPtrs, Vec3s *tri, f32 *slopeX, f32 *slopeZ);
Gfx *func_1510E634(Gfx *gfx, s32 arg1, s32 arg2);

/* game_1B1600.c */
extern u8 D_800DF0E0;
extern u8 D_800DF0E1; /* nonzero: halve the light distance term of the shadow near plane */
s32 func_15184150(s32 objIndex, f32 *halfX, f32 *halfY, f32 *halfZ);
f32 func_15184368(s32 objIndex, s32 slot, f32 halfX, f32 halfY, f32 halfZ, f32 minDist);
s32 func_15184DF0(s32 objIndex, s32 view, f32 *halfX, f32 *halfY, f32 *halfZ);
Gfx *func_15184FA4(Gfx *gfx, s32 objIndex, s32 slot, s32 view);
ClipVtx *func_15185454(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_15185554(ClipVtx *v);
ClipVtx *func_1518557C(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_1518567C(ClipVtx *v);
ClipVtx *func_151856A8(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_151857B0(ClipVtx *v);
ClipVtx *func_151857DC(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_151858D4(ClipVtx *v);
ClipVtx *func_151858FC(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_151859FC(ClipVtx *v);
ClipVtx *func_15185A28(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_15185B28(ClipVtx *v);
ClipVtx *func_15185B54(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_15185C44(ClipVtx *v);
ClipVtx *func_15185C6C(ClipVtx *a, ClipVtx *b, ClipVtx *out);
s32 func_15185D5C(ClipVtx *v);
s32 func_15185D84(ClipVtx *v);
s32 func_15185DAC(ClipVtx *v);
ClipVtx *func_15185DD4(ClipVtx *in, s32 count, s32 plane, ClipVtx *out);
s32 func_15185F24(ShadowVtx *in, s32 count, s32 maxVerts, Vtx *out, s32 *quadEnd, s32 split);
s32 func_1518652C(s32 x, s32 z, s32 radius, ShadowVtx *out, f32 *mtx, ShadowVtx *end);
Gfx *func_15186794(Gfx *gfx, s32 view);
void func_151872B0(s32 levelId);
s32 func_151873E4(s32 x, s32 z, s32 range, ShadowVtx *out, f32 *mtx, ShadowVtx *end, s32 count);

#endif
