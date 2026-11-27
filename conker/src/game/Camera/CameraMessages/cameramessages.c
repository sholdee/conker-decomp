#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
Camera *gSavedCameras; // saved cameras (one per view)

#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/math.h"
#include "conker/script.h"
#include "conker/sight.h"
#include "conker/view.h"
#include "conker/zone.h"

typedef struct Cfg15128CB0 {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 padC[0xB];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Cfg15128CB0;


s32 D_80089520 = 0; // frames left before a pending yaw override applies (set by game/Script/CameraCmd/cameracmd.c)
s32 gSavedSniperStates[4] = { 0 }; // per view: SniperState saved over a camera switch
s32 gSavedAimStates[4] = { 0 }; // per view: aim state saved over a camera switch
static Vec3f gCamShotFocusOffset = { 0.0f, 0.0f, -400.0f };

/*
 * Per-frame camera message pump: handles a pending unk734 request, then drains the camera's
 * message queue (camera_popMessage) and applies each message by type.
 * matching: in the braced cases, `break` follows the closing brace (as1 orders the case-end
 * branch by source line).
 */
void camera_processMessages(Camera *cam) {
    CamMsg *msg;

    cam->cmdState.unk0 = 0;
    if (cam->LastMagsZone != 0 && camera_isQueueEmpty(cam)) {
        if (script_query(1, cam->LastMagsZone, 6, 0x9000) && cam->LastMagsZoneConsider) {
            camera_queueMessage(cam, 1, &zones[cam->LastMagsZone & 0xFFF]);
            cam->cmd.id = cam->LastMagsZone & 0xFFF;
        } else {
            cam->LastMagsZone = 0;
            cam->LastMagsZoneConsider = 0;
        }
    }
    if (camera_isQueueEmpty(cam)) {
        if (cam->Mode & 1) {
            camera_popMode(cam, CAMERA_MODE_FIXED);
        }
        return;
    }
    while (!camera_isQueueEmpty(cam)) {
        msg = camera_popMessage(cam);
        switch (msg->type) {
        case 5:
            cam->CharContext = cam->mv->rt_context_b;
            if (cam->CharContext == 0) {
                camera_popMode(cam, CAMERA_MODE_AIM);
                cam->mv->AimState = 0;
                cam->mv->SniperState = 0;
                cam->SideHitsClear = 1;
                cam->SHitsOffLastFrame = 2;
                cam->mv->AimState = 0;
                cam->DoingAim = 0;
                camera_leaveFirstPersonAim(cam);
                gSavedSniperStates[cam->CNumber] = 0;
                gSavedAimStates[cam->CNumber] = 0;
            } else {
                gSavedSniperStates[cam->CNumber] = cam->mv->SniperState;
                gSavedAimStates[cam->CNumber] = cam->mv->AimState;
            }
            camera_stopEnvelope(cam);
            if (cam->AimTargetOn) {
                sight_resetPlayerEffects(cam);
                cam->AimTargetOn = 0;
            }
            gSavedCameras[msg->arg.index] = camera_global_Cameras[cam->CNumber];
            camera_pushMode(cam, 0x40000, 0, 0, CAMERA_MODE_CUTSCENE);
            cam->SpecialFlags |= 4;
            cam->CutbackAngle = -1.0f;
            break;
        case 6:
        {
            f32 angle;

            cam->CharContext = cam->mv->rt_context_b;
            if (D_80089520 != 0) {
                D_80089520--;
            } else if (cam->Mode == 0x40000) {
                angle = cam->CutbackAngle;
                if (gPendingCutbackAngle != 999.0f) {
                    angle = gPendingCutbackAngle;
                    gPendingCutbackAngle = 999.0f;
                }
                camera_popMode(cam, CAMERA_MODE_CUTSCENE);
                if (gCutsceneSkipped) {
                    cam->transition.endYaw = 999.0f;
                    cam->transition.duration = 0;
                    cam->transition.state = 0;
                }
                if (cam->transition.duration == 0) {
                    camera_global_Cameras[cam->CNumber] = gSavedCameras[msg->arg.index];
                    cam->PadPtr = gPadTable[cam->CNumber];
                    cam->CameraButtons = gButtonsPressed[cam->CNumber];
                    cam->CharContext = cam->mv->rt_context_b;
                    cam->Snap = 1;
                    camera_runCommand(cam);
                    if (gSavedAimStates[cam->CNumber] != 0) {
                        cam->CameraButtons |= 0x10;
                        cam->PadPtr->button |= 0x10;
                        angle = cam->aim.targetYaw;
                    } else if (gSavedSniperStates[cam->CNumber] != 0) {
                        if ((cam->CharContext == 12 && cam->CharPtr->animseq.uh != 0x76) || cam->CharContext == 27 || (cam->PadPtr->button & 0x10)) {
                            cam->CameraButtons |= 0x10;
                            cam->PadPtr->button |= 0x10;
                            cam->mv->SniperState = gSavedSniperStates[cam->CNumber];
                            sight_startPlayerEffects(cam, cam->CharContext);
                            angle = cam->aim.targetYaw;
                        } else {
                            cam->mv->AimState = 0;
                            cam->DoingAim = 0;
                            gSavedSniperStates[cam->CNumber] = 0;
                            gSavedAimStates[cam->CNumber] = 0;
                            camera_leaveFirstPersonAim(cam);
                        }
                    } else {
                        cam->mv->AimState = 0;
                        cam->DoingAim = 0;
                        camera_leaveFirstPersonAim(cam);
                    }
                    camera_moveFocus(cam);
                    if (cam->SpecialFlags & 2) {
                        angle = cam->CharPtr->theta - 180.0f;
                    }
                    cam->CameraAngle = angle;
                    cam->CameraAngleRad = angle * 0.01745329238f;
                    cam->savedTarget = cam->target;
                    cam->savedEye = cam->eye;
                    cam->prevTarget = cam->target;
                    cam->prevEye = cam->eye;
                    zone_updateGlobal(gFrameTicks);
                    zone_updateActor(&movement_obj[cam->CNumber]);
                    camera_runCommand(cam);
                    camera_updateAngleOffsets(cam, 0, 0);
                    camera_runModeHandler(cam);
                    camera_moveFocus(cam);
                    cam->SpecialFlags |= 4;
                    cam->CharTrackingSpeed = 0.0f;
                    cam->CharTrackingSpeedVel = 0.0f;
                    cam->DontMoveTilCharacterDoes = 1;
                    cam->Snap = 1;
                    cam->CharTrackingSpeedTarget = 1.0f;
                } else {
                    cam->PadPtr = gPadTable[cam->CNumber];
                    cam->CameraButtons = gButtonsPressed[cam->CNumber];
                    camera_startTransition(cam, gCutsceneEndTransitionTime, *(s32 *)&gCutsceneEndTransitionYaw); // matching: the f32 is passed as raw bits (callee ignores it)
                    cam->Snap = 0;
                }
            }
        }
            break;
        case 8:
        {
            CamShot *data = msg->arg.data;
            cam->CutbackAngle = data->yaw;
        }
            break;
        case 9:
            cam->zoneWaterY = *msg->arg.value;
            break;
        case 10:
        {
            CamShot *data;

            data = msg->arg.data;
            cam->transition.duration = data->duration;
            if (data->yaw == 999.0f) {
                cam->transition.endYaw = cam->CharPtr->theta - 180.0f;
            } else {
                cam->transition.endYaw = data->yaw;
            }
        }
            break;
        case 7:
        {
            CamShot *data;
            f32 surface;
            Vec3f pos;
            f32 angle;
            f32 x; // matching: stack-slot pads that place pos and angle in the frame
            f32 y;

            data = msg->arg.data;
            cam->eye.x = data->eye.x;
            cam->eye.y = data->eye.y;
            cam->eye.z = data->eye.z;
            if (data->mode == 0 || data->mode == 2) {
                cam->focus.x = data->at.look.focus.x;
                cam->focus.y = data->at.look.focus.y;
                cam->focus.z = data->at.look.focus.z;
                cam->FOVX = data->at.look.fovX;
                cam->FOVY = data->at.look.fovY;
                cam->TgetFOVX = data->at.look.fovX;
                cam->TgetFOVY = data->at.look.fovY;
                cam->target = cam->focus;
                cam->Roll = data->at.look.roll;
                cam->APitchAngle = 0.0f;
                cam->YawAngle = 0.0f;
                cam->AddOnPitch = 0.0f;
            } else {
                pos = gCamShotFocusOffset;
                while (data->at.aim.roll > 180.0f) {
                    data->at.aim.roll -= 360.0f;
                }
                while (data->at.aim.pitch > 180.0f) {
                    data->at.aim.pitch -= 360.0f;
                }
                while (data->at.aim.yaw > 180.0f) {
                    data->at.aim.yaw -= 360.0f;
                }
                cam->Roll = data->at.aim.roll;
                angle = data->at.aim.pitch * 0.01745329238f;
                pos.y = sinf(angle) * -pos.z;
                pos.z = cosf(angle) * pos.z;
                if (gCameraSnapTimers[0]) {
                    cam->CameraAngle = data->at.aim.yaw;
                    cam->TrueCameraAngle = data->at.aim.yaw;
                    cam->CameraAngleRad = cam->CameraAngle * 0.01745329238f;
                    cam->TrueCameraAngleRad = cam->TrueCameraAngle * 0.01745329238f;
                } else {
                    x = cam->CameraAngle; // matching: dead store steers register allocation
                }
                angle = cam->CameraAngle;
                angle += math_angleDiff(cam->CameraAngle, data->at.aim.yaw) / 1.0f;
                angle *= 0.01745329238f;
                pos.x = sinf(angle) * pos.z;
                pos.z = cosf(angle) * pos.z;
                pos.x += data->eye.x;
                pos.y += data->eye.y;
                pos.z += data->eye.z;
                cam->FOVX = data->at.aim.fovX;
                cam->FOVY = data->at.aim.fovY;
                cam->TgetFOVX = data->at.aim.fovX;
                cam->TgetFOVY = data->at.aim.fovY;
                cam->focus.x = pos.x;
                cam->focus.y = pos.y;
                cam->focus.z = pos.z;
                cam->target = cam->focus;
                cam->APitchAngle = 0.0f;
                cam->YawAngle = 0.0f;
                cam->AddOnPitch = 0.0f;
            }
            if (gCutsceneStates[data->scene] == 1) {
                gViewFovOffset = data->fovOffset;
                view_widenFov(cam->CNumber, cam->FOVX, cam->FOVY, 1.0f, 0.0f);
            }
            if (gCameraSnapTimers[0]) {
                light_easePlayerLight(0);
            }
            if (!gPaused) {
                collision_findFloorInRange(&cam->TrianglePtr, cam->floorTri.v, &surface, &cam->waterY, &cam->Attributes, 0,
                              cam->eye.x, cam->eye.y, cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
                camera_updateStatus(cam);
            }
        }
            break;
        case 1:
            cam->cmd.type = msg->arg.cfg->unk17;
            cam->cmd.unk1C = msg->arg.cfg->unk18;
            cam->cmd.unk20 = msg->arg.cfg->unk1C;
            cam->cmd.unk8 = msg->arg.cfg->unk20;
            cam->cmd.x = msg->arg.cfg->unk0;
            cam->cmd.y = msg->arg.cfg->unk2;
            cam->cmd.z = msg->arg.cfg->unk4;
            cam->cmd.halfLength = msg->arg.cfg->unk6;
            cam->cmd.unk18 = msg->arg.cfg->unk8;
            cam->cmd.unk1A = msg->arg.cfg->unkA;
            cam->cmd.unk24 = msg->arg.cfg->unk28;
            cam->cmd.unk28 = msg->arg.cfg->unk24;
            cam->cmd.unk2C = msg->arg.cfg->unk30;
            cam->cmd.unk30 = msg->arg.cfg->unk2C;
            cam->cmdState.unk0 = 1;
            break;
        }
    }
}
