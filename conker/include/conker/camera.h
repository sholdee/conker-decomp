#ifndef CONKER_CAMERA_H
#define CONKER_CAMERA_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "camera": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Camera/CameraViews/cameraviews.c
 *   game/Camera/LevelCamSetup/levelcamsetup.c
 *   game/done/game_32F60.c
 *   game/done/game_32FB0.c
 *   game/done/game_32FC0.c
 *   game/done/game_33000.c
 *   game/done/game_33010.c
 *   game/done/game_33020.c
 *   game/done/game_33080.c
 *   game/done/game_330E0.c
 *   game/done/game_33130.c
 *   game/done/game_331B0.c
 *   game/done/game_33210.c
 *   game/done/game_33260.c
 *   game/done/game_332E0.c
 *   game/done/game_33320.c
 *   game/done/game_33390.c
 *   game/done/game_333D0.c
 *   game/done/game_33410.c
 *   game/done/game_33450.c
 *   game/done/game_33460.c
 *   game/done/game_334C0.c
 *   game/done/game_33620.c
 *   game_E3C90.c
 *   game_E8BB0.c
 *   game_EBD00.c
 *   game/done/game_F20A0.c
 *   game_F4800.c
 *   game_F4C20.c
 *   game_F52B0.c
 *   game/done/game_FC3C0.c
 *   game_FDB00.c
 *   game/Camera/SpeedDistance/speeddistance.c
 *   game_10CCD0.c
 *   game/done/game_10ECB0.c
 *   game_10ED10.c
 *   game_118670.c
 *   game/done/game_119960.c
 *   game_122760.c
 *   game/Camera/SegmentProgress/segmentprogress.c
 *   game_126ED0.c
 *   game/done/game_129EB0.c
 *   game_1308E0.c
 *   game/Camera/DistanceReset/distancereset.c
 *   game_135490.c
 *   game/Camera/CameraData/cameradata.c (data only, linked after game/Prop/PropLinks/proplinks)
 *   game/Camera/AimCamera/aimcamera.c
 *   game/Camera/GroupFraming/groupframing.c
 *   game/Camera/YawSpring/yawspring.c
 *   game/Camera/YawBehind/yawbehind.c
 *   game/Camera/Heading/heading.c
 *   game/Camera/HeadingToPoint/headingtopoint.c
 *   game/Camera/OrbitEye/orbiteye.c
 *   game/Camera/YawFollow/yawfollow.c
 *   game/Camera/Camera/camera.c
 *   game/Camera/TiltPresets/tiltpresets.c
 *   game/Camera/CameraMessages/cameramessages.c
 *   game/Camera/CameraPoses/cameraposes.c
 *   game/Camera/FreeFlyCamera/freeflycamera.c
 *   game/Camera/CameraOcclusion/cameraocclusion.c
 *   game/Camera/ViewMatrix/viewmatrix.c
 *   game/Camera/CameraStub/camerastub.c
 *   game/Camera/ManualOrbit/manualorbit.c
 *   game/Camera/MessageQueue/messagequeue.c
 *   game/Camera/ViewEnvelope/viewenvelope.c
 *   game/Camera/RailCamera/railcamera.c
 *   game/Camera/HeightEasing/heighteasing.c
 *   game/Camera/AimAtObject/aimatobject.c
 *   game/Camera/ZRowCopy/zrowcopy.c
 *   game/Camera/CameraCommands/cameracommands.c
 *   game/Camera/CameraWobble/camerawobble.c
 */

/* Sound emitter handed to the aim sound callbacks by sound_addDynamicSound. */
typedef struct CamSound {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Camera *camera;
    /* 0x1C */ u32 data;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u16 voice;
} CamSound;

/* State of one z-buffer occlusion scan (see camera_testZPixel). */
typedef struct ZScan {
    u8 state;        // 0 none, 1 occluder outside mid, 2 inside mid, 4 (camera_checkOcclusion)
    u8 movingIn;     // the last hit was not farther out than the one before it
    u8 prevMovingIn; // movingIn before the last hit
    u8 unk3;
    s32 offset; // distance from the scan centre of the last occluder
    s32 gap;    // its depth in front of the target
} ZScan;

/* One flag per axis: 1 while that wave's phase is below pi (its sine is positive). */
typedef struct {
    u8 v[3];
} CamWobbleHalves;

/* A camera wobble (game/Camera/CameraWobble/camerawobble.c): six sine waves summed into a player's camera offset. */
typedef struct CamWobble {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ CamWobbleDef def;
    /* 0x48 */ f32 phase[6];
    /* 0x60 */ CamWobbleHalves posHalf;
    /* 0x63 */ CamWobbleHalves rotHalf;
} CamWobble; // size 0x68

/* The amplitudes and speeds of CamWobbleDef seen as four vectors. */
typedef struct {
    /* 0x00 */ Vec3f ampPos;
    /* 0x0C */ Vec3f ampRot;
    /* 0x18 */ Vec3f speedPos;
    /* 0x24 */ Vec3f speedRot;
} CamWobbleShape;

/* A wobble that ramps up to peak over riseTime, holds it for holdTime and ramps down over fallTime. */
typedef struct CamWobblePreset {
    /* 0x00 */ CamWobbleShape peak;
    /* 0x30 */ f32 riseTime;
    /* 0x34 */ f32 holdTime;
    /* 0x38 */ f32 fallTime;
} CamWobblePreset;

typedef struct {
    /* 0x00 */ CamWobbleShape riseRate; // peak / riseTime
    /* 0x30 */ CamWobbleShape fallRate; // peak / fallTime, with the pos and rot vectors swapped
    /* 0x60 */ CamWobbleShape peak;
    /* 0x90 */ f32 riseTime;
    /* 0x94 */ f32 holdTime;
    /* 0x98 */ f32 fallTime;
    /* 0x9C */ f32 time;
} CamWobbleEnvelope; // size 0xA0

/* Distance falloff used by scaleFn 1 (camera_getWobbleFalloff). */
typedef struct CamWobbleFalloff {
    /* 0x00 */ Vec3f origin;
    /* 0x0C */ f32 radius;       // full strength within this distance of origin
    /* 0x10 */ f32 fadeRange;    // then fades to nothing over this distance
    /* 0x14 */ f32 invFadeRange; // 1 / fadeRange
} CamWobbleFalloff; // size 0x18

/* A CamWobble made by camera_startEnvelopeWobble, with its envelope and room for a falloff after it. */
typedef struct CamWobbleEnv {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x018 */ CamWobbleShape shape; // CamWobble.def.amplitude and .speed
    /* 0x048 */ u8 pad48[0x20];       // rest of the CamWobble
    /* 0x068 */ CamWobbleEnvelope envelope;
    /* 0x108 */ CamWobbleFalloff falloff; // only with extraSize 24
} CamWobbleEnv;

/* game/Camera/CameraViews/cameraviews.c */
void camera_initViews(s32 resource, s32 mode);
void camera_resetView(Camera *view, s32 index, s16 resource, s32 mode);
void camera_placeAt(Camera *view, Actor *actor, Vec3f *eye);

/* game/Camera/LevelCamSetup/levelcamsetup.c */
void camera_runLevelSetup(Camera *arg0, s32 arg1);

/* game/done/game_32F60.c */
void func_15005AB0(Camera *arg0);
void func_15005AF0(Camera *arg0);

/* game/done/game_32FB0.c */
void func_15005B00(Camera *arg0);

/* game/done/game_32FC0.c */
void func_15005B10(Camera *arg0);

/* game/done/game_33000.c */
void func_15005B50(Camera *arg0);

/* game/done/game_33010.c */
void func_15005B60(Camera *arg0);

/* game/done/game_33020.c */
void func_15005B70(Camera *arg0);

/* game/done/game_33080.c */
void func_15005BD0(Camera *arg0);

/* game/done/game_330E0.c */
void func_15005C30(Camera *arg0);

/* game/done/game_33130.c */
void func_15005C80(Camera *arg0);
void func_15005CF0(Camera *arg0);

/* game/done/game_331B0.c */
void func_15005D00(Camera *arg0);

/* game/done/game_33210.c */
void func_15005D60(Camera *arg0);
void func_15005DA0(Camera *arg0);

/* game/done/game_33260.c */
void func_15005DB0(Camera *arg0);

/* game/done/game_332E0.c */
void func_15005E30(Camera *arg0);

/* game/done/game_33320.c */
void func_15005E70(Camera *arg0);
void func_15005EA0(Camera *arg0);

/* game/done/game_33390.c */
void func_15005EE0(Camera *arg0);

/* game/done/game_333D0.c */
void func_15005F20(Camera *arg0);

/* game/done/game_33410.c */
void func_15005F60(Camera *arg0);

/* game/done/game_33450.c */
void func_15005FA0(Camera *arg0);

/* game/done/game_33460.c */
void func_15005FB0(Camera *arg0);

/* game/done/game_334C0.c */
void func_15006010(Camera *actor);
void func_15006140(Camera *actor);

/* game/done/game_33620.c */
void func_15006170(Camera *arg0);

/* game_E3C90.c */
void func_150B67E0(Camera *camera);

/* game_E8BB0.c */
void func_150BB700(s32 *arg0);

/* game_EBD00.c */
void func_150BE850(Camera *actor);

/* game/done/game_F20A0.c */
void func_150C4BF0(Camera *arg0);

/* game_F4800.c */
void func_150C7350(Camera *arg0);

/* game_F4C20.c */
void func_150C7770(Camera *arg0);

/* game_F52B0.c */
void func_150C7E00(Camera *camera);

/* game/done/game_FC3C0.c */
void func_150CEF10(Camera *arg0);

/* game_FDB00.c */
void func_150D0650(Camera *arg0);

/* game/Camera/SpeedDistance/speeddistance.c */
void camera_scaleBySpeed(Camera *camera);

/* game_10CCD0.c */
void func_150DF820(Camera *arg0);

/* game/done/game_10ECB0.c */
void func_150E1800(Camera *arg0);

/* game_10ED10.c */
void func_150E1860(Camera *arg0);

/* game_118670.c */
void func_150EB1C0(Camera *actor);

/* game/done/game_119960.c */
void func_150EC4B0(Camera *arg0);

/* game_122760.c */
void func_150F52B0(Camera *camera);
void func_150F5310(s32 arg0);

/* game/Camera/SegmentProgress/segmentprogress.c */
void func_150F6B00(Camera *actor);

/* game_126ED0.c */
void func_150F9A20(Camera *arg0);

/* game/done/game_129EB0.c */
void func_150FCA00(Camera *arg0);

/* game_1308E0.c */
void func_15103430(Camera *actor);

/* game/Camera/DistanceReset/distancereset.c */
void camera_resetDistances(Camera *camera);

/* game_135490.c */
void func_15107FE0(Camera *arg0);

/* game/Camera/CameraData/cameradata.c (data only, linked after game/Prop/PropLinks/proplinks) */
extern Camera *camera_global_Cameras; // original name (Oct 2000 debugger); one Camera per player
extern u8 D_800DBFF4[4]; // per-player camera hold timers; *(s32 *)D_800DBFF4 tests all four at once
extern Mtx gIdentityMtx; // the identity, loaded as a fixed modelview matrix
extern s32 gCameraFrames; // camera frames

/* game/Camera/AimCamera/aimcamera.c */
void camera_enterAim(Camera *camera);
void camera_updateAim(Camera *camera);
void camera_publishAimRay(Actor *player, OSContPad *pad);
void camera_setAimDistance(Camera *camera);
void camera_leaveAim(Camera *camera);

/* game/Camera/GroupFraming/groupframing.c */
void camera_frameGroup(Camera *camera);

/* game/Camera/YawSpring/yawspring.c */
void camera_springYaw(Camera *camera, f32 targetYaw, s32 unused, f32 speedGain, f32 accelGain);
void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game/Camera/YawBehind/yawbehind.c */
void camera_easeYawBehind(Camera *camera, f32 yawOffset);

/* game/Camera/Heading/heading.c */
void camera_updateHeading(Camera *arg0);

/* game/Camera/HeadingToPoint/headingtopoint.c */
void camera_turnHeadingToPoint(Camera *arg0);

/* game/Camera/OrbitEye/orbiteye.c */
void camera_placeOrbitEye(Camera *camera);

/* game/Camera/YawFollow/yawfollow.c */
void camera_followYaw(Camera *camera);

/* game/Camera/Camera/camera.c */
void camera_updateAll(void);
void camera_update(Camera *camera);
void camera_updatePlayerAngle(Camera *camera);
void camera_updateAimTrigger(Camera *camera);
void camera_readOrbitButtons(Camera *camera);
void camera_stepDistancePreset(Camera *camera);
void camera_updateStatus(Camera *camera);
s32 camera_pushMode(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot);
s32 camera_popMode(Camera *camera, CameraMode slot);
void camera_updateFollow(Camera *camera);
void camera_setPairObject(Camera *camera, s32 index);
void camera_updateFocus(Camera *camera);
void camera_updateViewDir(Camera *camera);
void camera_loadDistancePreset(Camera *camera);
void camera_getTargetHeight(Camera *camera, f32 *outY);
void camera_updatePitch(Camera *camera);
void camera_updateYaw(Camera *camera);
void camera_updateModeIndex(Camera *camera);
s32 camera_updateFixed(Camera *camera);
s32 camera_isInDeepWater(Camera *camera);
void camera_attachToObject(Camera *camera, s32 index);
void camera_updateAngles(Camera *camera);
void func_15125608(Camera *camera);
void func_15125628(void);
void func_15125690(Camera *camera, s32 frames);
void camera_updateSway(Camera *camera);
void camera_updateDeepWater(Camera *camera);
void camera_startZoneShot(Camera *camera);
void func_15125C40(Camera *camera);
void camera_filterButtons(Camera *camera);
void camera_moveFocus(Camera *camera);
void camera_getAimEye(Actor *obj, ActorCtl *ext, s32 zone, f32 *outX, f32 *outY, f32 *outZ, s32 lowView);
s32 camera_updateFirstPersonAim(Camera *camera);
s32 camera_onZoomSound(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);
s32 camera_onLookSound(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId);
s32 camera_updateObjectAim(Actor *obj, OSContPad *contPad);
void camera_leaveFirstPersonAim(Camera *camera);
void camera_startTransition(Camera *camera, s32 arg1, s32 arg2);
s32 camera_updateTransition(Camera *camera);
void camera_runModeHandler(Camera *camera);
void camera_endUpdate(Camera *camera);
s32 camera_runSpecialModes(Camera *camera);
void func_15128680(Camera *camera);
void camera_reset(Camera *camera);
void camera_placeAtObject(Camera *camera, Actor *obj);
extern const f32 D_800A34B0[4][2];
extern f32 gCameraTransitionHeight; // camera transition height (see camera_updateTransition)

/* game/Camera/TiltPresets/tiltpresets.c */
void camera_updateAngleOffsets(Camera *camera, f32 *outUnk5E8, f32 *outUnk3A8);

/* game/Camera/CameraMessages/cameramessages.c */
void camera_processMessages(Camera *cam);
extern s32 D_80089520;
extern s32 gSavedSniperStates[4];
extern s32 gSavedAimStates[4];
extern Camera *gSavedCameras;

/* game/Camera/CameraPoses/cameraposes.c */
/* (Camera *cam, s32 arg1); declared without a prototype: game/Camera/CameraViews/cameraviews.c also passes the actor */
void func_151298C0();
void func_15129934(Camera *cam);
extern s16 gNumCameraPoses;
extern CamPose *gCameraPoses;
extern CamPose *gTargetCameraPose;

/* game/Camera/FreeFlyCamera/freeflycamera.c */
void camera_updateFreeFly(Camera *cam);
void func_1512A360(Camera *cam);

/* game/Camera/CameraOcclusion/cameraocclusion.c */
void camera_updateFloorHeights(Camera *arg0);
void camera_resetZScans(void);
void camera_checkOcclusion(Camera *arg0);
void camera_updateOcclusion(Camera *arg0);
void camera_scanZRow(Camera *arg0, s32 screenX, s32 screenY, s32 depth);
s32 camera_clearZRow(Camera *arg0);
void camera_recordZHit(ZScan *scan, s32 offset, s32 gap);
s32 camera_testZPixel(Camera *arg0, ZScan *scan, s32 gap, s32 offset, s32 mid, s32 useAltThreshold);
s32 camera_getOcclusionOffset(Camera *arg0, f32 *offset, f32 *unused);
void camera_collide(Camera *arg0);
void func_1512C068(Camera *arg0);
void camera_blendYaw(Camera *arg0);
void func_1512C200(Camera *camera);
void camera_keepOutsideCircle(Camera *arg0);
s32 func_1512C47C(s32 arg0);
extern f32 D_800895B0;

/* game/Camera/ViewMatrix/viewmatrix.c */
void camera_buildViewMatrix(Camera *camera);
void camera_applyBreathing(Camera *camera);
void camera_loadShakeTables(void);
void camera_startShake(Camera *camera, s32 index);
void camera_updateShake(Camera *camera);
void func_1512D368(Camera *camera);
extern Mtx *gViewMtxBuffers[2];

/* game/Camera/CameraStub/camerastub.c */
void func_1512D380(Camera *camera);

/* game/Camera/ManualOrbit/manualorbit.c */
void camera_updateManualOrbit(Camera *camera);

/* game/Camera/MessageQueue/messagequeue.c */
void camera_queueMessage(Camera *camera, s32 type, void *arg);
CamMsg *camera_popMessage(); /* (Camera *camera): the definition has a second, scratch parameter */
void camera_clearMessages(Camera *camera);
s32 camera_isQueueEmpty(Camera *camera);
extern CamMsgQueue *gCameraMsgQueues;

/* game/Camera/ViewEnvelope/viewenvelope.c */
void camera_stopEnvelope(Camera *view);
void camera_startEnvelope(Camera *view, s32 presetIndex, s32 mode);
void camera_updateEnvelope(Camera *view);
extern ViewEnvelope gViewEnvelopes[4];

/* game/Camera/RailCamera/railcamera.c */
void camera_buildRail(Camera *camera);
void camera_updateRail(Camera *camera);

/* game/Camera/HeightEasing/heighteasing.c */
void camera_easeHeight(Camera *camera);

/* game/Camera/AimAtObject/aimatobject.c */
void camera_aimAtObject(Camera *camera);

/* game/Camera/ZRowCopy/zrowcopy.c */
Gfx *camera_copyZRow(Gfx *gfx, Camera *camera);
extern ZDecode D_80089630[8];

/* game/Camera/CameraCommands/cameracommands.c */
void camera_runCommand(Camera *camera);
void camera_startCommandMode(Camera *camera, s32 action);

/* game/Camera/CameraWobble/camerawobble.c */
extern CamOffset gCameraOffsets[4]; // per-player camera offset (position, rotation), summed from the wobbles
CamWobble *camera_createWobble(CamWobbleDef *def, s32 extraSize, u8 arg2, s32 arg3);
void camera_stopWobble(CamWobble *wobble);
void camera_updateWobble(CamWobble *wobble);
void camera_clearOffsets(void);
CamWobbleEnv *camera_startWobblePreset(u8 presetIndex, u8 playerIndex, CamWobbleFalloff *falloff, u8 arg3, s32 arg4);
CamWobbleEnv *camera_startEnvelopeWobble(CamWobblePreset *preset, u8 playerIndex, s8 scaleFn, s32 extraSize, u8 arg4, s32 arg5);
s32 camera_updateWobbleEnvelope(CamWobbleEnv *wobble);
f32 camera_getWobbleFalloff(CamWobbleEnv *wobble);
f32 camera_getFalloffScale(CamWobble *wobble, Vec3f *origin, f32 radius, f32 fadeRange, f32 invFadeRange);

#endif
