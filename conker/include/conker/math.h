#ifndef CONKER_MATH_H
#define CONKER_MATH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "math": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_71240.c
 *   game_71770.c
 *   game/done/game_75810.c
 *   game/done/game_75950.c
 *   game/done/game_75A90.c
 *   game/done/game_75BD0.c
 *   game_75C90.c
 *   game_75E60.c
 *   game_75FC0.c
 *   game_76710.c
 *   game/done/game_768F0.c
 *   game_76A60.c
 *   game_76C90.c
 *   game_76E50.c
 *   game_770F0.c
 *   game_77760.c
 *   game_778B0.c
 *   game/done/game_77A90.c
 *   game_77AD0.c
 *   game_BC430.c
 *   game_16F770.c
 *   game_21CAC0.c
 */

/* game_71240.c */
void math_mtxRotScaleTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void math_mtxRotTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void math_mtxSetScaleTrans(MtxF mtx, f32 x, f32 y, f32 z, f32 transX, f32 transY, f32 transZ);
void math_mtxFRotScaleTrans(MtxF mtx, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void math_mtxGetTranslation(Vec3f *out, Mtx *mtx);
void math_mtxLookAt(MtxF mtx, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);

/* game_71770.c */
void math_mtxTranslateLocal(MtxF arg0, f32 x, f32 y, f32 z);

/* game/done/game_75810.c */
f32 math_acosLookup(f32 arg0);
f32 math_asinLookup(f32 arg0);

/* game/done/game_75950.c */
f32 math_atan2(f32 arg0, f32 arg1); /* libultra files see the declaration in src/libultra/audio/n_synthInternals.h */

/* game/done/game_75A90.c */
s32 math_acosFixedAbs(s32 x);
u16 math_acosFixed(s16 x);
s16 math_asinFixed(s16 x);

/* game/done/game_75BD0.c */
f32 math_lerpAngle(f32 arg0, f32 arg1, f32 arg2);
void math_normalizeDegrees(f32 *arg0);

/* game_75C90.c */
f32 math_asinTable(f32 x);
f32 math_asinTableBin(f32 x);
f32 math_asinTableLerp(f32 x);

/* game_75E60.c */
f32 math_cosBinAngle(u8 angle);
f32 math_sinBinAngle(u8 angle);
f32 math_angleDiff(f32 current, f32 target);
s32 math_angleDiffInt(s32 current, s32 target);

/* game_75FC0.c */
void math_mtxRigidInverse(MtxF mtx, MtxF out);
f32 func_15048C30(f32 x);
void math_vec3Add(Vec3f *a, Vec3f *b, Vec3f *out);
void math_vec3Sub(Vec3f *a, Vec3f *b, Vec3f *out);
void math_vec3Diff(const Vec3f *from, const Vec3f *to, Vec3f *out);
f32 math_vec3Heading(Vec3f *vec);
s32 math_vec3HeadingBin(Vec3f *vec);
void math_vec3Scale(Vec3f *vec, f32 scale, Vec3f *out);
void math_vec3Unit(Vec3f *vec, Vec3f *out);
void math_dirTowards(Vec3f *from, Vec3f *to, Vec3f *out);

/* game_76710.c */
s32 math_testTriBox(Tri3f tri);
void math_setTestBox(f32 width, f32 height, f32 depth);
void math_computeTriPlane(Tri3f tri);
extern f32 gTriPlaneNormalX; // plane of the last triangle: normal x, y, z
extern f32 gTriPlaneNormalY;
extern f32 gTriPlaneNormalZ;
extern f32 gTriPlaneDist; // normal . first vertex
extern f32 gTestBoxWidth; // box: width, height, depth
extern f32 gTestBoxHeight;
extern f32 gTestBoxDepth;
extern f32 gTestBoxHalfWidth; // box half sizes
extern f32 gTestBoxHalfHeight;
extern f32 gTestBoxHalfDepth;
extern f32 gTestBoxHeightRatio; // height / width
extern f32 gTestBoxDepthRatio; // depth / width

/* game/done/game_768F0.c */
s32 math_isBoxInView(MtxF mf, f32 x, f32 y, f32 z, f32 xExtent, f32 yExtent, f32 nearBound, f32 farBound, f32 xBound,
                  f32 yBound);

/* game_76A60.c */
void math_springTo(f32 *value, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);
void math_springAngleTo(f32 *angle, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);

/* game_76C90.c */
f32 math_catmullRom(f32 *p, s32 i, f32 t);
f32 math_catmullRomDeriv(f32 *p, s32 i, f32 t, f32 *deriv);

/* game_76E50.c */
void math_mtxAffineInverse(MtxF m, MtxF out);

/* game_770F0.c */
void math_quatAlign(f32 *arg0, f32 *arg1);
void math_mtxToQuat(MtxF m, f32 *q);
void math_quatSlerp(f32 *a, f32 *b, f32 t, f32 *out);
void math_quatToMtx(f32 *q, MtxF m);

/* game_77760.c */
f32 func_1504A2B0(f32 arg0);

/* game_778B0.c */
f32 func_1504A400(f32 x, f32 y);

/* game/done/game_77A90.c */
f32 tanf(f32 arg0);

/* game_77AD0.c */
f32 func_1504A620(f32 x);

/* game_BC430.c */
void math_rotateAroundY(Vec3f *arg0, Vec3f *arg1, f32 arg2, Vec3f *arg3);
extern u8 D_80087358[40];     /* read by func_150ADAF0 (game_DAFA0.s) */
extern u32 gLinkGroupCount;        /* number of groups in gLinkGroups */
extern ObjGroup *gLinkGroups;  /* the level's link groups (func_15017578) */

/* game_21CAC0.c */
s32 math_rand(void);

#endif
