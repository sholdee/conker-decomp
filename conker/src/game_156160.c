#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
Camera *D_800DC010; // saved cameras (one per view)

#include "functions.h"
#include "variables.h"

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
s32 D_80089524[4] = { 0 }; // per view: SniperState saved over a camera switch
s32 D_80089534[4] = { 0 }; // per view: aim state saved over a camera switch
static Vec3f D_80089544 = { 0.0f, 0.0f, -400.0f };

/*
 * Per-frame camera message pump: handles a pending unk734 request, then drains the camera's
 * message queue (func_1512D604) and applies each message by type.
 * matching: in the braced cases, `break` follows the closing brace (as1 orders the case-end
 * branch by source line).
 */
void func_15128CB0(Camera *cam) {
    CamMsg *msg;

    cam->cmdState.unk0 = 0;
    if (cam->LastMagsZone != 0 && func_1512D6B0(cam)) {
        if (script_query(1, cam->LastMagsZone, 6, 0x9000) && cam->LastMagsZoneConsider) {
            func_1512D560(cam, 1, &zones[cam->LastMagsZone & 0xFFF]);
            cam->cmd.id = cam->LastMagsZone & 0xFFF;
        } else {
            cam->LastMagsZone = 0;
            cam->LastMagsZoneConsider = 0;
        }
    }
    if (func_1512D6B0(cam)) {
        if (cam->Mode & 1) {
            func_151239CC(cam, CAMERA_MODE_FIXED);
        }
        return;
    }
    while (!func_1512D6B0(cam)) {
        msg = func_1512D604(cam);
        switch (msg->type) {
        case 5:
            cam->CharContext = cam->mv->rt_context_b;
            if (cam->CharContext == 0) {
                func_151239CC(cam, CAMERA_MODE_AIM);
                cam->mv->AimState = 0;
                cam->mv->SniperState = 0;
                cam->SideHitsClear = 1;
                cam->SHitsOffLastFrame = 2;
                cam->mv->AimState = 0;
                cam->DoingAim = 0;
                func_15127EB8(cam);
                D_80089524[cam->CNumber] = 0;
                D_80089534[cam->CNumber] = 0;
            } else {
                D_80089524[cam->CNumber] = cam->mv->SniperState;
                D_80089534[cam->CNumber] = cam->mv->AimState;
            }
            func_1512D6F0(cam);
            if (cam->AimTargetOn) {
                sight_resetPlayerEffects(cam);
                cam->AimTargetOn = 0;
            }
            D_800DC010[msg->arg.index] = camera_global_Cameras[cam->CNumber];
            func_15123934(cam, 0x40000, 0, 0, CAMERA_MODE_CUTSCENE);
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
                if (D_800D2DB0 != 999.0f) {
                    angle = D_800D2DB0;
                    D_800D2DB0 = 999.0f;
                }
                func_151239CC(cam, CAMERA_MODE_CUTSCENE);
                if (D_800C3672) {
                    cam->transition.endYaw = 999.0f;
                    cam->transition.duration = 0;
                    cam->transition.state = 0;
                }
                if (cam->transition.duration == 0) {
                    camera_global_Cameras[cam->CNumber] = D_800DC010[msg->arg.index];
                    cam->PadPtr = D_800BE728[cam->CNumber];
                    cam->CameraButtons = D_800BE710[cam->CNumber];
                    cam->CharContext = cam->mv->rt_context_b;
                    cam->Snap = 1;
                    func_1512E8E0(cam);
                    if (D_80089534[cam->CNumber] != 0) {
                        cam->CameraButtons |= 0x10;
                        cam->PadPtr->button |= 0x10;
                        angle = cam->aim.targetYaw;
                    } else if (D_80089524[cam->CNumber] != 0) {
                        if ((cam->CharContext == 12 && cam->CharPtr->animseq.uh != 0x76) || cam->CharContext == 27 || (cam->PadPtr->button & 0x10)) {
                            cam->CameraButtons |= 0x10;
                            cam->PadPtr->button |= 0x10;
                            cam->mv->SniperState = D_80089524[cam->CNumber];
                            sight_startPlayerEffects(cam, cam->CharContext);
                            angle = cam->aim.targetYaw;
                        } else {
                            cam->mv->AimState = 0;
                            cam->DoingAim = 0;
                            D_80089524[cam->CNumber] = 0;
                            D_80089534[cam->CNumber] = 0;
                            func_15127EB8(cam);
                        }
                    } else {
                        cam->mv->AimState = 0;
                        cam->DoingAim = 0;
                        func_15127EB8(cam);
                    }
                    func_15126138(cam);
                    if (cam->SpecialFlags & 2) {
                        angle = cam->CharPtr->theta - 180.0f;
                    }
                    cam->CameraAngle = angle;
                    cam->CameraAngleRad = angle * 0.01745329238f;
                    cam->savedTarget = cam->target;
                    cam->savedEye = cam->eye;
                    cam->prevTarget = cam->target;
                    cam->prevEye = cam->eye;
                    zone_updateGlobal(D_800BE9E4);
                    zone_updateActor(&movement_obj[cam->CNumber]);
                    func_1512E8E0(cam);
                    func_151287E0(cam, 0, 0);
                    func_151283B8(cam);
                    func_15126138(cam);
                    cam->SpecialFlags |= 4;
                    cam->CharTrackingSpeed = 0.0f;
                    cam->CharTrackingSpeedVel = 0.0f;
                    cam->DontMoveTilCharacterDoes = 1;
                    cam->Snap = 1;
                    cam->CharTrackingSpeedTarget = 1.0f;
                } else {
                    cam->PadPtr = D_800BE728[cam->CNumber];
                    cam->CameraButtons = D_800BE710[cam->CNumber];
                    func_15127FEC(cam, D_800C367C, *(s32 *)&D_800C3678); // matching: the f32 is passed as raw bits (callee ignores it)
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
                cam->focus.x = data->at.focus.x;
                cam->focus.y = data->at.focus.y;
                cam->focus.z = data->at.focus.z;
                cam->FOVX = data->unk30;
                cam->FOVY = data->unk34;
                cam->TgetFOVX = data->unk30;
                cam->TgetFOVY = data->unk34;
                cam->target = cam->focus;
                cam->Roll = data->unk2C;
                cam->APitchAngle = 0.0f;
                cam->YawAngle = 0.0f;
                cam->AddOnPitch = 0.0f;
            } else {
                pos = D_80089544;
                while (data->at.angles.sway > 180.0f) {
                    data->at.angles.sway -= 360.0f;
                }
                while (data->at.angles.pitch > 180.0f) {
                    data->at.angles.pitch -= 360.0f;
                }
                while (data->at.angles.yaw > 180.0f) {
                    data->at.angles.yaw -= 360.0f;
                }
                cam->Roll = data->at.angles.sway;
                angle = data->at.angles.pitch * 0.01745329238f;
                pos.y = sinf(angle) * -pos.z;
                pos.z = cosf(angle) * pos.z;
                if (D_800DBFF4[0]) {
                    cam->CameraAngle = data->at.angles.yaw;
                    cam->TrueCameraAngle = data->at.angles.yaw;
                    cam->CameraAngleRad = cam->CameraAngle * 0.01745329238f;
                    cam->TrueCameraAngleRad = cam->TrueCameraAngle * 0.01745329238f;
                } else {
                    x = cam->CameraAngle; // matching: dead store steers register allocation
                }
                angle = cam->CameraAngle;
                angle += func_15048A70(cam->CameraAngle, data->at.angles.yaw) / 1.0f;
                angle *= 0.01745329238f;
                pos.x = sinf(angle) * pos.z;
                pos.z = cosf(angle) * pos.z;
                pos.x += data->eye.x;
                pos.y += data->eye.y;
                pos.z += data->eye.z;
                cam->FOVX = data->unk2C;
                cam->FOVY = data->unk30;
                cam->TgetFOVX = data->unk2C;
                cam->TgetFOVY = data->unk30;
                cam->focus.x = pos.x;
                cam->focus.y = pos.y;
                cam->focus.z = pos.z;
                cam->target = cam->focus;
                cam->APitchAngle = 0.0f;
                cam->YawAngle = 0.0f;
                cam->AddOnPitch = 0.0f;
            }
            if (D_800C35EA[data->unk4] == 1) {
                D_800C3648 = data->unk8;
                view_widenFov(cam->CNumber, cam->FOVX, cam->FOVY, 1.0f, 0.0f);
            }
            if (D_800DBFF4[0]) {
                light_easePlayerLight(0);
            }
            if (!gPaused) {
                collision_findFloorInRange(&cam->TrianglePtr, cam->floorTri.v, &surface, &cam->waterY, &cam->Attributes, 0,
                              cam->eye.x, cam->eye.y, cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
                func_151236D0(cam);
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
