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
void func_15043D90(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void func_15043E68(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void func_15043EC8(MtxF mtx, f32 x, f32 y, f32 z, f32 transX, f32 transY, f32 transZ);
void func_15043F6C(MtxF mtx, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z);
void math_mtxGetTranslation(Vec3f *out, Mtx *mtx);
void func_150440A0(MtxF mtx, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);

/* game_71770.c */
void func_150442C0(MtxF arg0, f32 x, f32 y, f32 z);

/* game/done/game_75810.c */
f32 func_15048360(f32 arg0);
f32 func_15048408(f32 arg0);

/* game/done/game_75950.c */
f32 func_150484A0(f32 arg0, f32 arg1); /* libultra files see the declaration in src/libultra/audio/n_synthInternals.h */

/* game/done/game_75A90.c */
s32 func_150485E0(s32 x);
u16 func_15048664(s16 x);
s16 func_150486B8(s16 x);

/* game/done/game_75BD0.c */
f32 func_15048720(f32 arg0, f32 arg1, f32 arg2);
void func_15048758(f32 *arg0);

/* game_75C90.c */
f32 func_150487E0(f32 x);
f32 func_15048864(f32 x);
f32 func_150488C8(f32 x);

/* game_75E60.c */
f32 func_150489B0(u8 angle);
f32 func_15048A40(u8 angle);
f32 func_15048A70(f32 current, f32 target);
s32 func_15048AD0(s32 current, s32 target);

/* game_75FC0.c */
void func_15048B10(s32 mtx, s32 out);
f32 func_15048C30(f32 x);
void func_15048F20(Vec3f *a, Vec3f *b, Vec3f *out);
void func_15048F58(Vec3f *a, Vec3f *b, Vec3f *out);
void func_15048F90(Vec3f *from, Vec3f *to, Vec3f *out);
f32 func_15048FC8(Vec3f *vec);
s32 func_150490A8(Vec3f *vec);
void func_15049148(Vec3f *vec, f32 scale, Vec3f *out);
void func_1504917C(Vec3f *vec, Vec3f *out);
void func_150491EC(Vec3f *from, Vec3f *to, Vec3f *out);

/* game_76710.c */
s32 func_15049260(Tri3f tri);
void func_150492CC(f32 width, f32 height, f32 depth);
void func_15049350(Tri3f tri);

/* game/done/game_768F0.c */
s32 func_15049440(MtxF mf, f32 x, f32 y, f32 z, f32 xExtent, f32 yExtent, f32 nearBound, f32 farBound, f32 xBound,
                  f32 yBound);

/* game_76A60.c */
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);
void func_15049688(f32 *angle, f32 target, f32 *velocity, f32 speedGain, f32 accelGain, f32 dt);

/* game_76C90.c */
f32 func_150497E0(f32 *p, s32 i, f32 t);
f32 func_150498A4(f32 *p, s32 i, f32 t, f32 *deriv);

/* game_76E50.c */
void func_150499A0(MtxF m, MtxF out);

/* game_770F0.c */
void func_15049C40(f32 *arg0, f32 *arg1);
void func_15049CB8(MtxF m, f32 *q);
void func_15049EDC(f32 *a, f32 *b, f32 t, f32 *out);
void func_1504A140(f32 *q, MtxF m);

/* game_77760.c */
f32 func_1504A2B0(f32 arg0);

/* game_778B0.c */
f32 func_1504A400(f32 x, f32 y);

/* game/done/game_77A90.c */
f32 tanf(f32 arg0);

/* game_77AD0.c */
f32 func_1504A620(f32 x);

/* game_BC430.c */
void func_1508EF80(Vec3f *arg0, Vec3f *arg1, f32 arg2, Vec3f *arg3);

/* game_21CAC0.c */
s32 func_151EF610(void);

#endif
