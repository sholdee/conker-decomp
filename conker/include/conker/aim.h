#ifndef CONKER_AIM_H
#define CONKER_AIM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "aim": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_32490.c
 *   game_64120.c
 *   game_6B280.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct AimLimits AimLimits;

/* game_32490.c */
s32 aim_loadPoints(s32 assetId);

/* game_64120.c */
void func_15036C70(Actor *obj);
void func_15036CE8(void);
void func_15036F34(void);
s32 func_15037698(s32 objIndex, s32 refBone, s32 bone, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 noCheck,
                  s32 axisMode);
s32 func_15037880(s32 objIndex, f32 *out);
void func_150379DC(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, s32 axisMode);
s32 func_150380C0(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist, s32 noCheck,
                  s32 skipUnk127, s32 anyRange, s32 axisMode);
void func_15038468(f32 *yaw, f32 *pitch, f32 x, f32 y, f32 z, s32 axisMode);
void func_15038620(s32 bone, s32 refBone, f32 *target, s32 objIndex, s32 limitIdx, s32 rateIdx, s32 slot, s32 axes,
                   s32 noLimit, s32 relative, s32 yawDeadzone, s32 argB, s32 axisMode);
AimLimits *func_15039A54(s32 objIndex, s32 limitIdx);
void func_15039A78(f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex);
void func_15039CC8(Actor *obj);
void func_15039ED0(s32 limitIdx, f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex, s32 slot);
void func_1503A08C(Actor *obj);
void func_1503A60C(void);
void func_1503A678(void);
void func_1503A7F0(void);
void func_1503A830(Actor *obj);
void func_1503B708(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist, s32 noCheck,
                   s32 skipUnk127, s32 anyRange, s32 axisMode);
extern f32 D_800C3FD0[3];
extern u8 D_800C3FDC;
extern f32 D_800C3FE0;
extern f32 D_800C3FE4;
extern s16 D_800C3FF4[3];
extern u8 D_800C3FFA; /* aim pass / slot counter */

/* game_6B280.c */
void aim_removePoint(s32 arg0);

#endif
