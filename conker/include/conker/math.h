#ifndef CONKER_MATH_H
#define CONKER_MATH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "math": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Math/MtxBuild/mtxbuild.c
 *   game/Math/MtxTranslate/mtxtranslate.c
 *   game/Math/AcosLookup/acoslookup.c
 *   game/Math/Atan2/atan2.c
 *   game/Math/AcosTable/acostable.c
 *   game/Math/AngleLerp/anglelerp.c
 *   game/Math/AsinTable/asintable.c
 *   game/Math/BinAngle/binangle.c
 *   game/Math/VectorOps/vectorops.c
 *   game/Math/TriBox/tribox.c
 *   game/Math/BoxInView/boxinview.c
 *   game/Math/Spring/spring.c
 *   game/Math/CatmullRom/catmullrom.c
 *   game/Math/MtxInverse/mtxinverse.c
 *   game/Math/Quaternion/quaternion.c
 *   game/Math/Expf/expf.c
 *   game/Math/Powf/powf.c
 *   game/Math/Tanf/tanf.c
 *   game/Math/Logf/logf.c
 *   game/Math/RotateAroundY/rotatearoundy.c
 *   game/Math/MathUtil/mathutil.c
 *   game/Math/Random/random.c
 */

/* game/Math/MtxBuild/mtxbuild.c */
void math_mtxRotScaleTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void math_mtxRotTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void math_mtxSetScaleTrans(MtxF mtx, f32 x, f32 y, f32 z, f32 transX, f32 transY, f32 transZ);
void math_mtxFRotScaleTrans(MtxF mtx, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void math_mtxGetTranslation(Vec3f *out, Mtx *mtx);
void math_mtxLookAt(MtxF mtx, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);

/* game/Math/MtxTranslate/mtxtranslate.c */
void math_mtxTranslateLocal(MtxF arg0, f32 x, f32 y, f32 z);

/* game/Math/AcosLookup/acoslookup.c */
f32 math_acosLookup(f32 arg0);
f32 math_asinLookup(f32 arg0);

/* game/Math/Atan2/atan2.c */
f32 math_atan2(f32 arg0, f32 arg1); /* libultra files see the declaration in src/libultra/audio/n_synthInternals.h */

/* game/Math/AcosTable/acostable.c */
s32 math_acosFixedAbs(s32 x);
u16 math_acosFixed(s16 x);
s16 math_asinFixed(s16 x);

/* game/Math/AngleLerp/anglelerp.c */
f32 math_lerpAngle(f32 arg0, f32 arg1, f32 arg2);
void math_normalizeDegrees(f32 *arg0);

/* game/Math/AsinTable/asintable.c */
f32 math_asinTable(f32 x);
f32 math_asinTableBin(f32 x);
f32 math_asinTableLerp(f32 x);

/* game/Math/BinAngle/binangle.c */
f32 math_cosBinAngle(u8 angle);
f32 math_sinBinAngle(u8 angle);
f32 math_angleDiff(f32 current, f32 target);
s32 math_angleDiffInt(s32 current, s32 target);

/* game/Math/VectorOps/vectorops.c */
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

/* game/Math/TriBox/tribox.c */
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

/* game/Math/BoxInView/boxinview.c */
s32 math_isBoxInView(MtxF mf, f32 x, f32 y, f32 z, f32 xExtent, f32 yExtent, f32 nearBound, f32 farBound, f32 xBound,
                  f32 yBound);

/* game/Math/Spring/spring.c */
void math_springTo(f32 *value, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);
void math_springAngleTo(f32 *angle, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);

/* game/Math/CatmullRom/catmullrom.c */
f32 math_catmullRom(f32 *p, s32 i, f32 t);
f32 math_catmullRomDeriv(f32 *p, s32 i, f32 t, f32 *deriv);

/* game/Math/MtxInverse/mtxinverse.c */
void math_mtxAffineInverse(MtxF m, MtxF out);

/* game/Math/Quaternion/quaternion.c */
void math_quatAlign(f32 *arg0, f32 *arg1);
void math_mtxToQuat(MtxF m, f32 *q);
void math_quatSlerp(f32 *a, f32 *b, f32 t, f32 *out);
void math_quatToMtx(f32 *q, MtxF m);

/* game/Math/Expf/expf.c */
f32 func_1504A2B0(f32 arg0);

/* game/Math/Powf/powf.c */
f32 func_1504A400(f32 x, f32 y);

/* game/Math/Tanf/tanf.c */
f32 tanf(f32 arg0);

/* game/Math/Logf/logf.c */
f32 func_1504A620(f32 x);

/* game/Math/RotateAroundY/rotatearoundy.c */
void math_rotateAroundY(Vec3f *arg0, Vec3f *arg1, f32 arg2, Vec3f *arg3);
extern u8 D_80087358[40];     /* read by func_150ADAF0 (game_DAFA0.s) */
extern u32 gLinkGroupCount;        /* number of groups in gLinkGroups */
extern ObjGroup *gLinkGroups;  /* the level's link groups (func_15017578) */

/* game/Math/Random/random.c */
s32 math_rand(void);

#endif
