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
 *   game/Camera/LevelCamSetup01/levelcamsetup01.c
 *   game/Camera/LevelCamSetup03/levelcamsetup03.c
 *   game/Camera/LevelCamSetup04/levelcamsetup04.c
 *   game/Camera/LevelCamSetup05/levelcamsetup05.c
 *   game/Camera/LevelCamSetup06/levelcamsetup06.c
 *   game/Camera/LevelCamSetup07/levelcamsetup07.c
 *   game/Camera/LevelCamSetup08/levelcamsetup08.c
 *   game/Camera/LevelCamSetup09/levelcamsetup09.c
 *   game/Camera/LevelCamSetup10/levelcamsetup10.c
 *   game/Camera/LevelCamSetup12/levelcamsetup12.c
 *   game/Camera/LevelCamSetup13/levelcamsetup13.c
 *   game/Camera/LevelCamSetup15/levelcamsetup15.c
 *   game/Camera/LevelCamSetup16/levelcamsetup16.c
 *   game/Camera/LevelCamSetup17/levelcamsetup17.c
 *   game/Camera/LevelCamSetup19/levelcamsetup19.c
 *   game/Camera/LevelCamSetup20/levelcamsetup20.c
 *   game/Camera/LevelCamSetup21/levelcamsetup21.c
 *   game/Camera/LevelCamSetup22/levelcamsetup22.c
 *   game/Camera/LevelCamSetup23/levelcamsetup23.c
 *   game/Camera/LevelCamSetup24/levelcamsetup24.c
 *   game/Camera/LevelCamSetup26/levelcamsetup26.c
 *   game/Camera/LevelHook07/levelhook07.c
 *   game/Camera/LevelHook05/levelhook05.c
 *   game/Camera/LevelHook18/levelhook18.c
 *   game/Camera/LevelHook03/levelhook03.c
 *   game/Camera/LevelHook10/levelhook10.c
 *   game/Camera/LevelHook01/levelhook01.c
 *   game/Camera/LevelHook06/levelhook06.c
 *   game/Camera/LevelHook13/levelhook13.c
 *   game/Camera/LevelHook22/levelhook22.c
 *   game/Camera/SpeedDistance/speeddistance.c
 *   game/Camera/LevelHook09/levelhook09.c
 *   game/Camera/LevelHook33/levelhook33.c
 *   game/Camera/LevelHook35/levelhook35.c
 *   game/Camera/LevelHook04/levelhook04.c
 *   game/Camera/LevelHook32/levelhook32.c
 *   game/Camera/LevelHook34/levelhook34.c
 *   game/Camera/SegmentProgress/segmentprogress.c
 *   game/Camera/LevelHook28/levelhook28.c
 *   game/Camera/LevelHook19/levelhook19.c
 *   game/Camera/LevelHook15/levelhook15.c
 *   game/Camera/DistanceReset/distancereset.c
 *   game/Camera/LevelHook25/levelhook25.c
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

/* game/Camera/LevelCamSetup01/levelcamsetup01.c */
void camera_levelSetup01(Camera *arg0);
void camera_emptyLevelSetup02(Camera *arg0);

/* game/Camera/LevelCamSetup03/levelcamsetup03.c */
void camera_emptyLevelSetup03(Camera *arg0);

/* game/Camera/LevelCamSetup04/levelcamsetup04.c */
void camera_levelSetup04(Camera *arg0);

/* game/Camera/LevelCamSetup05/levelcamsetup05.c */
void camera_emptyLevelSetup05(Camera *arg0);

/* game/Camera/LevelCamSetup06/levelcamsetup06.c */
void camera_emptyLevelSetup06(Camera *arg0);

/* game/Camera/LevelCamSetup07/levelcamsetup07.c */
void camera_levelSetup07(Camera *arg0);

/* game/Camera/LevelCamSetup08/levelcamsetup08.c */
void camera_levelSetup08(Camera *arg0);

/* game/Camera/LevelCamSetup09/levelcamsetup09.c */
void camera_levelSetup09(Camera *arg0);

/* game/Camera/LevelCamSetup10/levelcamsetup10.c */
void camera_levelSetup10(Camera *arg0);
void camera_emptyLevelSetup11(Camera *arg0);

/* game/Camera/LevelCamSetup12/levelcamsetup12.c */
void camera_levelSetup12(Camera *arg0);

/* game/Camera/LevelCamSetup13/levelcamsetup13.c */
void camera_levelSetup13(Camera *arg0);
void camera_emptyLevelSetup14(Camera *arg0);

/* game/Camera/LevelCamSetup15/levelcamsetup15.c */
void camera_levelSetup15(Camera *arg0);

/* game/Camera/LevelCamSetup16/levelcamsetup16.c */
void camera_levelSetup16(Camera *arg0);

/* game/Camera/LevelCamSetup17/levelcamsetup17.c */
void camera_levelSetup17(Camera *arg0);
void camera_levelSetup18(Camera *arg0);

/* game/Camera/LevelCamSetup19/levelcamsetup19.c */
void camera_levelSetup19(Camera *arg0);

/* game/Camera/LevelCamSetup20/levelcamsetup20.c */
void camera_levelSetup20(Camera *arg0);

/* game/Camera/LevelCamSetup21/levelcamsetup21.c */
void camera_levelSetup21(Camera *arg0);

/* game/Camera/LevelCamSetup22/levelcamsetup22.c */
void camera_emptyLevelSetup22(Camera *arg0);

/* game/Camera/LevelCamSetup23/levelcamsetup23.c */
void camera_levelSetup23(Camera *arg0);

/* game/Camera/LevelCamSetup24/levelcamsetup24.c */
void camera_levelSetup24(Camera *actor);
void camera_levelSetup25(Camera *actor);

/* game/Camera/LevelCamSetup26/levelcamsetup26.c */
void camera_levelSetup26(Camera *arg0);

/* game/Camera/LevelHook07/levelhook07.c */
void camera_levelHook07(Camera *camera);

/* game/Camera/LevelHook05/levelhook05.c */
void camera_levelHook05(s32 *arg0);

/* game/Camera/LevelHook18/levelhook18.c */
void camera_levelHook18(Camera *actor);

/* game/Camera/LevelHook03/levelhook03.c */
void camera_levelHook03(Camera *arg0);

/* game/Camera/LevelHook10/levelhook10.c */
void camera_levelHook10(Camera *arg0);

/* game/Camera/LevelHook01/levelhook01.c */
void camera_levelHook01(Camera *arg0);

/* game/Camera/LevelHook06/levelhook06.c */
void camera_levelHook06(Camera *camera);

/* game/Camera/LevelHook13/levelhook13.c */
void camera_levelHook13(Camera *arg0);

/* game/Camera/LevelHook22/levelhook22.c */
void camera_levelHook22(Camera *arg0);

/* game/Camera/SpeedDistance/speeddistance.c */
void camera_scaleBySpeed(Camera *camera);

/* game/Camera/LevelHook09/levelhook09.c */
void camera_levelHook09(Camera *arg0);

/* game/Camera/LevelHook33/levelhook33.c */
void camera_levelHook33(Camera *arg0);

/* game/Camera/LevelHook35/levelhook35.c */
void camera_levelHook35(Camera *arg0);

/* game/Camera/LevelHook04/levelhook04.c */
void camera_levelHook04(Camera *actor);

/* game/Camera/LevelHook32/levelhook32.c */
void camera_levelHook32(Camera *arg0);

/* game/Camera/LevelHook34/levelhook34.c */
void camera_levelHook34(Camera *camera);
void camera_levelUpdateWaveChains(s32 arg0);

/* game/Camera/SegmentProgress/segmentprogress.c */
void camera_levelHook23(Camera *actor);

/* game/Camera/LevelHook28/levelhook28.c */
void camera_levelHook28(Camera *arg0);

/* game/Camera/LevelHook19/levelhook19.c */
void camera_levelHook19(Camera *arg0);

/* game/Camera/LevelHook15/levelhook15.c */
void camera_levelHook15(Camera *actor);

/* game/Camera/DistanceReset/distancereset.c */
void camera_resetDistances(Camera *camera);

/* game/Camera/LevelHook25/levelhook25.c */
void camera_levelHook25(Camera *arg0);

/* game/Camera/CameraData/cameradata.c (data only, linked after game/Prop/PropLinks/proplinks) */
extern Camera *camera_global_Cameras; // original name (Oct 2000 debugger); one Camera per player
extern u8 gCameraSnapTimers[4]; // per-player camera hold timers; *(s32 *)gCameraSnapTimers tests all four at once
extern Mtx gIdentityMtx; // the identity, loaded as a fixed modelview matrix
extern Mtx D_89470; // gIdentityMtx's physical address (undefined_syms.us.txt): static display lists load it through segment 0
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
void camera_setDistSpeeds(Camera *camera);
void camera_tickSnapTimers(void);
void camera_raiseSnapTimer(Camera *camera, s32 frames);
void camera_updateSway(Camera *camera);
void camera_updateDeepWater(Camera *camera);
void camera_startZoneShot(Camera *camera);
void camera_forceZoneRTrigger(Camera *camera);
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
void camera_emptyUpdateHook(Camera *camera);
void camera_reset(Camera *camera);
void camera_placeAtObject(Camera *camera, Actor *obj);
extern const f32 gCameraDistancePresets[4][2];
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
void camera_swingIfTooClose(Camera *arg0);
void camera_blendYaw(Camera *arg0);
void func_1512C200(Camera *camera);
void camera_keepOutsideCircle(Camera *arg0);
s32 func_1512C47C(s32 arg0);
extern f32 gCameraAvoidTargetDist;

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
extern ZDecode gZDecodeTable[8];

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
