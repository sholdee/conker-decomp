#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct Entry151298C0 {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 padC[0x18];
};

extern struct Entry151298C0 D_800DC028[];

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} Data15128CB0;

typedef struct {
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

typedef struct {
    s32 type;
    union {
        s32 index;          /* types 5, 6 */
        Cfg15128CB0 *cfg;   /* type 1 */
        Data15128CB0 *data; /* types 7, 8, 10 */
        f32 *value;         /* type 9 */
    } arg;
} Msg15128CB0;

s32 func_1509BE40(s32, s32, s32, s32);
void func_1512D560(Camera *, s32, TriggerZone *);
Msg15128CB0 *func_1512D604(Camera *);
s32 func_1512D6B0(Camera *);
void func_1512D6F0(Camera *);
void func_151C9ED4(Camera *);
void func_1512E8E0(Camera *);
void func_151C9BA0(Camera *, s32);
void func_150A09D0(s32);
void func_150A11C4(Actor *);
void func_151287E0(Camera *, s32, s32);
void func_151283B8(Camera *);
void func_1510B128(s32, f32, f32, f32, f32);
void func_1501B320(s32);
void func_1510E7A4(s32 *, s32 *, f32 *, f32 *, s32 *, s32, f32, f32, f32, f32, u16, s32, f32, f32);
void func_151236D0(Camera *);
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32, f32, f32);
s32 func_150A3FC4(f32, f32, s32, s32 *, f32 *);
void func_1512C150(Camera *);
extern Vec3f D_80089544;

/*
 * Per-frame camera message pump: handles a pending unk734 request, then drains the camera's
 * message queue (func_1512D604) and applies each message by type.
 * matching: in the braced cases, `break` follows the closing brace (as1 orders the case-end
 * branch by source line).
 */
void func_15128CB0(Camera *cam) {
    Msg15128CB0 *msg;

    cam->cmdState.unk0 = 0;
    if (cam->LastMagsZone != 0 && func_1512D6B0(cam)) {
        if (func_1509BE40(1, cam->LastMagsZone, 6, 0x9000) && cam->LastMagsZoneConsider) {
            func_1512D560(cam, 1, &D_800D3098[cam->LastMagsZone & 0xFFF]);
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
                func_151C9ED4(cam);
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
                            func_151C9BA0(cam, cam->CharContext);
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
                    func_150A09D0(D_800BE9E4);
                    func_150A11C4(&movement_obj[cam->CNumber]);
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
            Data15128CB0 *data = msg->arg.data;
            cam->CutbackAngle = data->unkC;
        }
            break;
        case 9:
            cam->zoneWaterY = *msg->arg.value;
            break;
        case 10:
        {
            Data15128CB0 *data;

            data = msg->arg.data;
            cam->transition.duration = data->unk10;
            if (data->unkC == 999.0f) {
                cam->transition.endYaw = cam->CharPtr->theta - 180.0f;
            } else {
                cam->transition.endYaw = data->unkC;
            }
        }
            break;
        case 7:
        {
            Data15128CB0 *data;
            f32 surface;
            Vec3f pos;
            f32 angle;
            f32 x; // matching: stack-slot pads that place pos and angle in the frame
            f32 y;

            data = msg->arg.data;
            cam->eye.x = data->unk14;
            cam->eye.y = data->unk18;
            cam->eye.z = data->unk1C;
            if (data->unk0 == 0 || data->unk0 == 2) {
                cam->focus.x = data->unk20;
                cam->focus.y = data->unk24;
                cam->focus.z = data->unk28;
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
                while (data->unk20 > 180.0f) {
                    data->unk20 -= 360.0f;
                }
                while (data->unk24 > 180.0f) {
                    data->unk24 -= 360.0f;
                }
                while (data->unk28 > 180.0f) {
                    data->unk28 -= 360.0f;
                }
                cam->Roll = data->unk20;
                angle = data->unk24 * 0.01745329238f;
                pos.y = sinf(angle) * -pos.z;
                pos.z = cosf(angle) * pos.z;
                if (D_800DBFF4[0]) {
                    cam->CameraAngle = data->unk28;
                    cam->TrueCameraAngle = data->unk28;
                    cam->CameraAngleRad = cam->CameraAngle * 0.01745329238f;
                    cam->TrueCameraAngleRad = cam->TrueCameraAngle * 0.01745329238f;
                } else {
                    x = cam->CameraAngle; // matching: dead store steers register allocation
                }
                angle = cam->CameraAngle;
                angle += func_15048A70(cam->CameraAngle, data->unk28) / 1.0f;
                angle *= 0.01745329238f;
                pos.x = sinf(angle) * pos.z;
                pos.z = cosf(angle) * pos.z;
                pos.x += data->unk14;
                pos.y += data->unk18;
                pos.z += data->unk1C;
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
                func_1510B128(cam->CNumber, cam->FOVX, cam->FOVY, 1.0f, 0.0f);
            }
            if (D_800DBFF4[0]) {
                func_1501B320(0);
            }
            if (!D_800BEAC0) {
                func_1510E7A4(&cam->TrianglePtr, &cam->unk648, &surface, &cam->waterY, &cam->Attributes, 0,
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

/* Resets this camera's D_800DC028 entry when D_80089550 is set. */
void func_151298C0(Camera *cam, s32 arg1) {
    if (D_80089550 != 0) {
        D_800DC028[cam->CNumber].unk2 = 0;
        D_800DC028[cam->CNumber].unk4 = -1.0f;
        D_800DC028[cam->CNumber].unk8 = 1.0f / 90.0f;
    }
}


/*
 * Eases the camera point unk2F8 toward the point unk278 short of unk2BC on the line from
 * unk288 (snapping when unk23C is set), then re-queries func_1510E7A4 for it when needed.
 */
void func_15129934(Camera *cam) {
    f32 x;
    Vec3f dir;
    f32 z;
    f32 y;
    f32 dist;
    f32 surface;
    f32 dx;
    f32 dy;
    f32 dz;

    if (D_80089550 != 0 && cam->DoingAim == 0) {
        cam->unk27C.x = cam->unk288.x;
        cam->unk27C.y = cam->unk288.y;
        cam->unk27C.z = cam->unk288.z;
        func_15048F90(&cam->unk27C, &cam->target, &dir);
        func_1504917C(&dir, &dir);
        dx = cam->target.x - cam->unk27C.x;
        dy = cam->target.y - cam->unk27C.y;
        dz = cam->target.z - cam->unk27C.z;
        dist = sqrtf(dx * dx + dy * dy + dz * dz) - cam->FTgetRadius;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        func_15049148(&dir, dist, &dir);
        x = cam->unk27C.x + dir.x;
        if (cam->cmd.unk8 & 0xF) {
            y = cam->unk27C.y;
        } else {
            y = cam->unk27C.y + dir.y * 1.2f;
        }
        z = cam->unk27C.z + dir.z;
        if (cam->Snap) {
            cam->eye.x = x;
            cam->prevEye.x = x;
            cam->CurrentVelX = 0; // matching: int 0, not 0.0f (a float zero gets promoted to a register)
        } else {
            func_150495B0(&cam->eye.x, x, &cam->CurrentVelX, 1.0f, 3.0f, cam->SpringTime);
        }
        if (cam->Snap) {
            cam->eye.y = y;
            cam->prevEye.y = y;
            cam->CurrentVelY = 0;
        } else {
            func_150495B0(&cam->eye.y, y, &cam->CurrentVelY, 1.0f, 2.0f, cam->SpringTime);
        }
        if (cam->Snap) {
            cam->eye.z = z;
            cam->prevEye.z = z;
            cam->CurrentVelZ = 0;
        } else {
            func_150495B0(&cam->eye.z, z, &cam->CurrentVelZ, 1.0f, 3.0f, cam->SpringTime);
        }
        func_1512C150(cam);
        if (cam->Snap || (cam->AvailableModes & 0x400000) ||
            !func_150A3FC4(cam->eye.x, cam->eye.z, cam->TrianglePtr, &cam->unk648, &cam->unk35C)) {
            func_1510E7A4(NULL, NULL, &surface, &cam->waterY, &cam->Attributes, 0, cam->eye.x, cam->eye.y,
                          cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
            if (cam->zoneWaterY != -10000.0f) {
                cam->waterY = cam->zoneWaterY;
            }
            func_151236D0(cam);
        }
        cam->unk974 = 1.0f;
        cam->unk964 = 1.0f;
        cam->targetSpeed = 1.0f;
        cam->speed = 1.0f;
        cam->unk978 = 2.0f;
        cam->unk968 = 2.0f;
        cam->targetLimit = 2.0f;
        cam->limit = 2.0f;
    }
}
