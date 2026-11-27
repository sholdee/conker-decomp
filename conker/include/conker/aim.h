#ifndef CONKER_AIM_H
#define CONKER_AIM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "aim": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Aim/AimPoints/aimpoints.c
 *   game/Aim/AimUpdate/aimupdate.c
 *   game/Aim/AimRemove/aimremove.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct AimLimits AimLimits;

/* game/Aim/AimPoints/aimpoints.c */
s32 aim_loadPoints(s32 assetId);

/* game/Aim/AimUpdate/aimupdate.c */
void aim_initState(Actor *obj);
void func_15036CE8(void);
void aim_updateByType(void);
s32 aim_isPointInCone(s32 objIndex, s32 refBone, s32 bone, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 noCheck,
                  s32 axisMode);
s32 aim_getFixedTarget(s32 objIndex, f32 *out);
void aim_findTarget(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, s32 axisMode);
s32 aim_findNearestObject(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist, s32 noCheck,
                  s32 skipUnk127, s32 anyRange, s32 axisMode);
void aim_directionToAngles(f32 *yaw, f32 *pitch, f32 x, f32 y, f32 z, s32 axisMode);
void aim_turnBone(s32 bone, s32 refBone, f32 *target, s32 objIndex, s32 limitIdx, s32 rateIdx, s32 slot, s32 axes,
                   s32 noLimit, s32 relative, s32 yawDeadzone, s32 argB, s32 axisMode);
const AimLimits *aim_getLimits(s32 objIndex, s32 limitIdx);
void aim_clampAngles(f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex);
void aim_updateLock(Actor *obj);
void aim_clampToLimits(s32 limitIdx, f32 *yaw, f32 *pitch, f32 *prevYaw, f32 *prevPitch, f32 fallbackYaw, f32 fallbackPitch,
                   s32 objIndex, s32 slot);
void aim_updatePoints(Actor *obj);
void func_1503A60C(void);
void func_1503A678(void);
void func_1503A7F0(void);
void aim_turnHead(Actor *obj);
void aim_findTargetObject(f32 *out, s32 refBone, s32 bone, s32 objIndex, f32 range, s32 preferId, f32 *bestDist, s32 noCheck,
                   s32 skipUnk127, s32 anyRange, s32 axisMode);
extern f32 gAimLookPoint[3];
extern u8 gAimHasOverride;
extern f32 gAimOverrideYaw;
extern f32 gAimOverridePitch;
extern s16 gAimFixedPoint[3];
extern u8 gAimPass; /* aim pass / slot counter */

/* game/Aim/AimRemove/aimremove.c */
extern AimPoint *gAimPoints; // aim points (gAimPointCount of them)
extern s32 gAimPointCount;
void aim_removePoint(s32 index);

#endif
