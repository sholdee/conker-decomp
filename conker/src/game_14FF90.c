#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#define CAM_C_BUTTONS (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

#define DEG2RAD 0.01745329238474369f

extern f32 D_800DC000;
extern f32 D_800C3614;
extern f32 D_800C3618;
extern f32 D_800C361C;
/* Camera distance presets, indexed by Camera.DistIndex: { tripod magnitude, height }. */
const f32 D_800A34B0[4][2] = {
    { 267.0f, 100.0f },
    { 247.0f, 100.0f },
    { 370.0f, 185.0f },
    { 530.0f, 400.0f },
};
extern u16 D_800894E0[2][3];

/* Per-frame update of every active player camera. */
void func_15122AE0(void) {
    Camera *camera;
    s16 i;
    f32 dt;

    dt = D_800BEA08 * (1.0f / 60.0f);

    for (i = 0; i <= D_80082FA0; i++) {
        camera = &camera_global_Cameras[i];
        if (func_150859AC(i, 0) || (i == 0)) {
            camera->SpringTime = dt;
            if ((D_800BEAC0 == 0) || (D_800C35EA[0] != 0) || (D_800D2DB4 != 0)) {
                func_151239CC(camera, CAMERA_MODE_PAUSE);
                func_15122C5C(camera);
            } else {
                func_15123934(camera, 0x2000, 0, camera->Tightness, CAMERA_MODE_PAUSE);
                func_1512C490(camera);
            }
        }
    }
    D_800894B0 += 1;
}

/* Per-frame update of one player camera: input, zone state, then either a special mode or the
   follow camera (focus, distance, angles, collision). */
void func_15122C5C(Camera *camera) {
    Vec3f *target;
    Vec3f *eye;
    u8 index;
    f32 fov;

    if (camera->Snap) {
        camera->savedTarget = camera->target;
        camera->savedEye = camera->eye;
    }
    target = &camera->target;
    eye = &camera->eye;
    if ((camera->mv != NULL) && (camera->DoingAim != 3) && ((camera->Mode != 0x100) || (camera->DoingAim != 0))) {
        camera->CharContext = camera->mv->rt_context_b;
    }
    if (D_800DBFF4[camera->CNumber] != 4) {
        camera->SpecialFlags &= ~4;
    }
    if (!(camera->Mode & 0x400000)) {
        *target = camera->savedTarget;
        *eye = camera->savedEye;
    }
    index = camera->CNumber;
    camera->PadPtr = D_800BE728[index];
    camera->CameraButtons = D_800BE710[index];
    camera->prevEye = *eye;
    camera->unk358 = camera->unk35C;
    camera->prevTarget = *target;
    if (camera->Snap != 0) {
        camera->FOVX = camera->TgetFOVX;
        camera->FOVY = camera->TgetFOVY;
    } else {
        camera->FOVX += (camera->TgetFOVX - camera->FOVX) * 0.1f;
        camera->FOVY += (camera->TgetFOVY - camera->FOVY) * 0.1f;
    }
    if (camera->Mode & 0x100) {
        fov = 16.0f;
    } else {
        fov = 0.0f;
    }
    // matching: the playerIndex reload is assigned inside the call (a separate statement reorders it)
    func_1510B128(index = camera->CNumber, camera->FOVX, camera->FOVY, D_800BE628[index].perspScale, fov);
    func_15097798(camera->CNumber);
    func_15125A6C(camera);
    func_15128CB0(camera);
    func_15125DB4(camera);
    if ((camera->mv->death_State != 0) && (camera->mv->SniperState != 0)) {
        func_15127EB8(camera);
    }
    if (func_15128540(camera) == 0) {
        if ((camera->AvailableModes & 0x08000000) && !(camera->AvailableModes & 0x400)) {
            func_1512E8E0(camera);
        }
        func_15130230(camera, level);
        func_15126138(camera);
        func_15125C40(camera);
        func_15128680(camera);
        func_1512317C(camera);
        func_15125394(camera);
        if (!(camera->AvailableModes & 0x08000000) && !(camera->AvailableModes & 0x400)) {
            func_1512E8E0(camera);
        }
        func_15123568(camera);
        func_1512C200(camera);
        func_15125924(camera);
        func_151283B8(camera);
        if (camera->Snap != 0) {
            func_15125594(camera);
            func_15123070(camera);
            func_151283B8(camera);
            func_151283B8(camera);
        }
        func_15123508(camera);
        if ((camera->AvailableModes & 2) && (camera->mv->death_State == 0)) {
            func_1512D390(camera);
        }
        func_15125594(camera);
        func_151256BC(camera);
        func_1512D980(camera);
        func_1512C068(camera);
        func_1512B100(camera);
        func_1512C20C(camera);
        func_1512BB10(camera);
        if (camera->Snap != 0) {
            func_1512C068(camera);
            func_1512B100(camera);
            func_1512C20C(camera);
            func_1512BB10(camera);
        }
        func_15125594(camera);
        func_15123070(camera);
        if (camera->transition.state == 0) {
            func_151287E0(camera, NULL, NULL);
        }
        func_1512C150(camera);
        func_151284C4(camera);
    }
}

/* Computes unk390, the angle (degrees, wrapped above -360) between the camera yaw and the player. */
void func_15123070(Camera *camera) {
    f32 angle;

    if ((camera->cmdRunning != 0) && ((camera->cmd.type == 10) || (camera->cmd.type == 14))) {
        Vec3f delta;

        func_15048F90(&camera->pivotPos, &camera->focus, &delta);
        camera->CharRelAngle = camera->CameraAngle - func_15048FC8(&delta);
    } else {
        angle = camera->CharPtr->theta - camera->CameraAngle - 180.0f;

        while (angle < 0.0f) {
            angle += 360.0f;
        }

        camera->CharRelAngle = angle;
    }

    while (camera->CharRelAngle < -360.0f) {
        camera->CharRelAngle += 360.0f;
    }
}

/* Whether the R-trigger camera is allowed this frame; pressing R then pushes mode 0x100 (slot 1). */
void func_1512317C(Camera *camera) {
    s32 stunned;
    s32 status100;
    s32 status200;
    s32 blocked;

    status100 = camera->SpecialFlags & 0x100;
    status200 = camera->SpecialFlags & 0x200;
    stunned = (camera->CharPtr->stunned != 0) && (camera->CharContext != 9) && (camera->CharContext != 0x38) &&
              (camera->CharContext != 0x39) && (camera->CharContext != 0x3B) && (camera->CharContext != 0x37) &&
              (camera->CharContext != 0x15) && (camera->CharContext != 0x26) && (camera->CharContext != 0x3A);

    blocked = ((camera->Mode & 0x80) != 0) ||
              ((camera->AvailableModes & 0x1000000) != 0) ||
              ((camera->CharPtr->shad != 0.0f) && (camera->CharPtr->inwater == 0) &&
               (camera->mv->ropePtr == NULL) && (status100 == 0) && (status200 == 0) &&
               (camera->CharContext != 3)) ||
              ((camera->CharPtr->attach_char != 0) && (status100 == 0) && (status200 == 0)) ||
              (stunned != 0) ||
              (func_15125490(camera) != 0) ||
              (camera->CharPtr->energy == 0) ||
              (((camera->mv->transport & 0xF) == 1) && (camera->CharPtr->speed > 15.0f)) ||
              (camera->mv->on_fire != 0);

    if ((camera->PadPtr->button & R_TRIG) && !blocked) {
        if (!(camera->CameraButtons & R_TRIG)) {
            return;
        }
        if (func_15123934(camera, 0x100, 4, camera->Tightness, CAMERA_MODE_AIM) != 0) {
            func_1511FC60(camera);
        }
        return;
    }

    if ((camera->Mode == 0x100) && (camera->DoingAim == 0)) {
        func_151239CC(camera, CAMERA_MODE_AIM);
        camera->mv->AimState = 0;
        camera->mv->SniperState = 0;
        camera->SideHitsClear = 1;
        camera->SHitsOffLastFrame = 2;
        camera->CameraButtons &= ~R_TRIG;
        camera->PadPtr->button &= ~R_TRIG;
    }

    if ((camera->Mode == 0x100) || (camera->DoingAim != 0)) {
        camera->CameraButtons &= ~R_TRIG;
        camera->PadPtr->button &= ~R_TRIG;
    }
    camera->mv->AimState = camera->DoingAim;

    if ((camera->DoingAim != 0) || (camera->DoingAim == 3)) {
        func_150495B0(&camera->aim.offset.x, 0.0f, &camera->aim.offsetXVel, 4.0f, 9.0f, camera->SpringTime);
        func_150495B0(&camera->aim.offset.z, 0.0f, &camera->aim.offsetZVel, 4.0f, 9.0f, camera->SpringTime);
    }

    if ((camera->Mode != 0x100) && (camera->DoingAim != 0)) {
        camera->DoingAim = 0;
    }
}

/* With the C-left/C-right buttons, sets orbitDir to -1/+1 (when AvailableModes bit 2 allows it). */
void func_15123508(Camera *camera) {
    if (camera->AvailableModes & 2) {
        if ((camera->CameraButtons & L_CBUTTONS) && (camera->unk698 == 0)) {
            camera->orbitDir = -1;
        }
        if ((camera->CameraButtons & R_CBUTTONS) && (camera->unk698 == 0)) {
            camera->orbitDir = 1;
        }
    }
}

/* C-up steps to the previous allowed distance preset (with a 20-frame cooldown). */
void func_15123568(Camera *camera) {
    s32 cycle;

    if ((camera->mv->death_State == 4) && (camera->Mode != 0x40) && (camera->DoingRail == 0)) {
        camera->CentreBiasTarget = 999.0f;
        camera->DistIndex = 1;
        func_15124B18(camera);
        camera->XZOffsetTget = 0.0f;
        camera->YOffsetTget = 0.0f;
        return;
    }
    if (camera->ZoomCounter > 0) {
        camera->ZoomCounter -= D_800BE9E4;
        if (camera->ZoomCounter < 0) {
            camera->ZoomCounter = 0;
        }
    } else if ((camera->AvailableModes & 4) && (camera->CameraButtons & U_CBUTTONS) && (camera->Motion != 4) &&
               ((camera->cmdRunning == 0) || (camera->cmd.type == 4)) &&
               ((camera->CharPtr->inwater != 1) || (camera->DistIndex != 1))) {
        // matching: the loop flag (not an if around for (;;)) keeps the constant 1 in a1
        cycle = camera->DistIndex != 1;
        while (cycle) {
            if (--camera->DistIndex <= 0) {
                camera->DistIndex = 3;
            }
            if ((1 << camera->DistIndex) & camera->DistMask) {
                func_15124B18(camera);
                func_15125608(camera);
                camera->ZoomCounter = 20;
                return;
            }
            cycle = TRUE;
        }
    }
}

/* Updates the status bits taken from the player state, and bit 1 (eye at or below unk360), starting
   or stopping the matching per-player effects (func_1515BAxx) when bit 1 changes. */
void func_151236D0(Camera *camera) {
    s32 wasBelow;

    wasBelow = camera->SpecialFlags & 1;
    if (camera->mv != NULL) {
        if ((camera->mv->ropePtr != NULL) ||
            ((camera->mv->ladder != 0) && (camera->CharPtr->unk137 == 0))) {
            camera->SpecialFlags |= 0x40;
        } else {
            camera->SpecialFlags &= ~0x40;
        }
        if (camera->CharPtr->shad == 0.0f) {
            camera->SpecialFlags &= ~0x400;
        }
        if ((camera->mv->pissed_up != 0) && (camera->mv->pissed_hangover == 0)) {
            camera->SpecialFlags |= 8;
        } else {
            camera->SpecialFlags &= ~8;
        }
        if (camera->mv->transport == 2) {
            camera->SpecialFlags |= 0x80;
        } else {
            camera->SpecialFlags &= ~0x80;
        }
    }
    if ((camera->eye.y <= camera->waterY) && (camera->Mode != 0x100)) {
        camera->SpecialFlags |= 1;
    } else {
        camera->SpecialFlags &= ~1;
    }
    if (camera->Snap != 0) {
        if (camera->SpecialFlags & 1) {
            func_1515BA80(camera->CNumber);
        } else {
            func_1515BA48(camera->CNumber);
            camera->WaterDx = 0.0f;
        }
    } else if (!wasBelow && (camera->SpecialFlags & 1)) {
        if ((camera->SpecialFlags & 4) || (camera->Mode & 0x40000)) {
            func_1515BA80(camera->CNumber);
        } else {
            func_1515BAAC(camera->CNumber);
        }
    } else if (wasBelow && !(camera->SpecialFlags & 1)) {
        if ((camera->SpecialFlags & 4) || (camera->Mode & 0x40000)) {
            func_1515BA48(camera->CNumber);
            camera->WaterDx = 0.0f;
        } else {
            func_1515BA54(camera->CNumber);
            func_15124B18(camera);
        }
    } else if (camera->SpecialFlags & 1) {
        func_1515BA1C(camera->CNumber);
    } else {
        func_1515BA10(camera->CNumber);
    }
}
/* Saves the mode state in cache slot `slot` and switches to newMode. Returns 0 if the slot is taken. */
s32 func_15123934(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot) {
    if (camera->FCache[slot] == 0) {
        camera->savedModeIndex[slot] = camera->ModeIndex;
        camera->MCache[slot] = camera->Mode;
        camera->savedAvailableModes[slot] = camera->AvailableModes;
        camera->savedMotion[slot] = camera->Motion;
        camera->savedTightness[slot] = camera->Tightness;
        camera->savedDistIndex[slot] = camera->DistIndex;
        camera->savedDistMask[slot] = camera->DistMask;
        camera->Mode = newMode;
        camera->Motion = newMotion;
        camera->Tightness = newTightness;
        camera->FCache[slot] = 1;
        func_15125394(camera);
        return 1;
    }
    return 0;
}

/* Restores the mode state saved in `slot` by func_15123934. Returns 0 if the slot is empty. */
s32 func_151239CC(Camera *camera, CameraMode slot) {
    if (camera->FCache[slot] != 0) {
        camera->ModeIndex = camera->savedModeIndex[slot];
        camera->Mode = camera->MCache[slot];
        camera->Motion = camera->savedMotion[slot];
        camera->AvailableModes = camera->savedAvailableModes[slot];
        camera->Tightness = camera->savedTightness[slot];
        camera->DistIndex = camera->savedDistIndex[slot];
        camera->DistMask = camera->savedDistMask[slot];
        func_15124B18(camera);
        camera->FCache[slot] = 0;
        return 1;
    }
    return 0;
}

/* Follow-camera distance and yaw: look-behind (C-down), wall collision between focus and eye, and
   easing of the current distance; then places the eye behind the target at that distance. */
void func_15123A54(Camera *camera) {
    f32 angle;
    f32 savedDistance;

    savedDistance = camera->IdealTripodMagnitude;
    camera->blocked = 0;
    camera->blockedDistance = camera->IdealTripodMagnitude;
    if (D_800C3671 == 0) {
        if (camera->cmdState.maxDistance != 0.0f) {
            camera->IdealTripodMagnitude = (camera->cmdState.maxDistance < camera->IdealTripodMagnitude) ? camera->cmdState.maxDistance : camera->IdealTripodMagnitude;
        }
        if (camera->Snap == 0) {
            if ((camera->SpecialFlags & 0x10) && (camera->PadPtr->button & R_TRIG)) {
                camera->IdealTripodMagnitude = 130.0f;
            }
            if ((camera->DoingAim == 0) && (camera->SpecialFlags & 0x40)) {
                camera->IdealTripodMagnitude = 420.0f;
            }
            if (camera->Mode & 0x403) {
                if (camera->CameraButtons & D_CBUTTONS) {
                    camera->DoingAlignBehind = 1;
                    camera->AlignBehindTimeOut = 180;
                    camera->AlignBehindAngleVel = 0.0f;
                } else if ((camera->PadPtr->button & D_CBUTTONS) || camera->DoingAlignBehind) {
                    if (camera->DoingAlignBehind && (camera->PadPtr->button & D_CBUTTONS)) {
                        camera->DoingAlignBehind += D_800BEA08;
                        camera->DoingAlignBehind = (camera->DoingAlignBehind >= 16) ? 15 : camera->DoingAlignBehind;
                    }
                    if (camera->DoingAlignBehind == 15) {
                        camera->LockedBehind = 1;
                        camera->ZoomCounter = 20;
                        camera->IdealTripodMagnitude = 300.0f;
                        camera->height = 150.0f;
                        func_15049688(&camera->CameraAngle, camera->CharPtr->theta - 180.0f, &camera->AlignBehindAngleVel, 1.5f, 2.0f,
                                      camera->SpringTime);
                        if (!(camera->PadPtr->button & D_CBUTTONS)) {
                            camera->DoingAlignBehind = 0;
                        }
                    } else {
                        func_15049688(&camera->CameraAngle, camera->CharPtr->theta - 180.0f, &camera->AlignBehindAngleVel, 2.0f, 4.0f,
                                      camera->SpringTime);
                    }
                    camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
                    if ((camera->DoingAlignBehind < 15) && !(camera->PadPtr->button & D_CBUTTONS)) {
                        if (camera->CharMoved == 0) {
                            camera->AlignBehindTimeOut -= D_800BEA08;
                            if (camera->AlignBehindTimeOut <= 0) {
                                camera->DoingAlignBehind = 0;
                            } else if (fabsf(camera->CameraAngle - (camera->CharPtr->theta - 180.0f)) <= 2.0f) {
                                camera->DoingAlignBehind = 0;
                            }
                        } else {
                            camera->DoingAlignBehind = 0;
                        }
                    }
                } else if (camera->LockedBehind != 0) {
                    func_15124B18(camera);
                    camera->LockedBehind = 0;
                    camera->DoingAlignBehind = 0;
                }
            }
        } else {
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
        }
        angle = camera->CameraAngleRad;
        if (D_800BEA0C != 0) {
            // matching: only the (f32)1 factor matches (dropping it or writing 1.0f changes the code)
            camera->CameraAngle += (f32)1 * camera->MatrixEffectDirection * D_800BEA08;
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
            angle = camera->CameraAngleRad;
        } else if (func_150ADA20() & 1) {
            camera->MatrixEffectDirection = -1.0f;
        } else {
            camera->MatrixEffectDirection = 1.0f;
        }
        if ((camera->mv->ladder != 0) && (camera->SpecialFlags & 0x40)) {
            angle = camera->CharPtr->theta + 180.0f;
            func_15049688(&camera->CameraAngle, angle, &camera->LadderAngleVel, 2.0f, 4.0f, camera->SpringTime);
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
            angle = camera->CameraAngleRad;
            camera->IdealTripodMagnitude = 530.0f;
        }
        if (camera->Mode != 0x2000) {
            camera->TripodMagnitude = sqrtf((camera->eye.x - camera->target.x) * (camera->eye.x - camera->target.x) +
                                  (camera->eye.z - camera->target.z) * (camera->eye.z - camera->target.z));
            if ((camera->DoingAim != 0) || (camera->Snap != 0) || (camera->CharPtr->unk102 != 0) ||
                (!(camera->AvailableModes & 0x1000) && (camera->DoingRail == 0) && !(camera->AvailableModes & 0x40000) && !(camera->SpecialFlags & 0x80))) {
                Vec3f dir;
                s16 tri[10];
                f32 hit[3];
                f32 dist;
                f32 pad1; // matching: unused stack slots (pad1, pad2) in the frame
                s16 tri2[10];
                f32 backZ;
                f32 pad2;
                f32 backX;
                Vec3f edge1;
                Vec3f edge2;
                Vec3f normal;
                f32 hit2[3];
                f32 dist2;
                f32 prevDistance;
                Vec3f v0;
                Vec3f v1;
                Vec3f v2;

                dir.x = sinf(camera->CameraAngleRad);
                dir.y = 0.0f;
                dir.z = cosf(camera->CameraAngleRad);
                if ((func_150AC9C0(camera->focus.x, camera->focus.y + 140.0f, camera->focus.z, dir.x, dir.y, dir.z, 0,
                                   tri, &hit[0], &hit[1], &hit[2], &dist, 0, 0, camera->IdealTripodMagnitude + 30.0f) != 0) &&
                    (dist < camera->IdealTripodMagnitude)) {
                    backX = sinf(camera->CameraAngleRad + 3.1415927410125732f) * camera->IdealTripodMagnitude;
                    backZ = cosf(camera->CameraAngleRad + 3.1415927410125732f) * camera->IdealTripodMagnitude;
                    v0.x = tri[0];
                    v0.y = tri[1];
                    v0.z = tri[2];
                    v1.x = tri[3];
                    v1.y = tri[4];
                    v1.z = tri[5];
                    v2.x = tri[6];
                    v2.y = tri[7];
                    v2.z = tri[8];
                    func_15048F90(&v0, &v1, &edge1);
                    func_15048F90(&v0, &v2, &edge2);
                    func_150AD8B0(&edge2.x, &edge1.x, &normal.x);
                    func_1504917C(&normal, &normal);
                    if (0.3253220021724701f < func_150AD900(&dir.x, &normal.x)) {
                        if (func_150AC9C0(camera->focus.x - backX, camera->focus.y + 140.0f, camera->focus.z - backZ, backX,
                                          0.0f, backZ, 0, tri2, &hit2[0], &hit2[1], &hit2[2], &dist2, 0, 0,
                                          camera->IdealTripodMagnitude + 30.0f) != 0) {
                            f32 dx;
                            f32 dz;

                            dx = hit2[0] - hit[0];
                            dz = hit2[2] - hit[2];
                            if ((sqrtf(dx * dx + dz * dz) < 1.0f) || (camera->SpecialFlags & 0x10) || (camera->CharContext == 0xC) ||
                                (camera->CharContext == 9) || (camera->CharContext == 0x38) || (camera->CharContext == 0x39) ||
                                (camera->CharContext == 0x37) || (camera->CharContext == 0x3B) || (camera->CharContext == 0x12) ||
                                (camera->CharContext == 0x15) || (camera->CharContext == 0x26) || (camera->CharContext == 0x3A) ||
                                (camera->Snap != 0)) {
                                prevDistance = camera->IdealTripodMagnitude;
                                if (camera->DoingAim != 0) {
                                    dist -= 16.0f;
                                } else {
                                    dist -= 12.0f;
                                }
                                dist = (dist < 50.0f) ? 50.0f : dist;
                                camera->IdealTripodMagnitude = (dist < camera->IdealTripodMagnitude) ? dist : camera->IdealTripodMagnitude;
                                if (camera->CharContext == 3) {
                                    camera->height *= camera->IdealTripodMagnitude / prevDistance;
                                    if (camera->SpecialFlags & 0x10) {
                                        camera->height = (camera->height < camera->target.y - camera->unk354)
                                                          ? camera->target.y - camera->unk354
                                                          : camera->height;
                                    }
                                }
                                camera->blockedDistance = dist;
                                camera->blocked = 1;
                            }
                        }
                    }
                }
            }
            if (camera->transition.state != 0) {
                camera->IdealTripodMagnitude = D_800DC000;
            }
            if (camera->Snap != 0) {
                camera->TripodMagnitude = camera->IdealTripodMagnitude;
                camera->XZVel = 0.0f;
                if (camera->AvailableModes & 0x200000) {
                    if (camera->Mode & 0x40) {
                        camera->unk974 = 1.0f;
                        camera->unk964 = 1.0f;
                        camera->unk978 = 2.0f;
                        camera->unk968 = 2.0f;
                    } else {
                        camera->unk978 = 8.0f;
                        camera->unk968 = 8.0f;
                        camera->unk974 = 6.0f;
                        camera->unk964 = 6.0f;
                    }
                    camera->unk984 = 0.0f;
                    camera->unk988 = 0.0f;
                }
            } else {
                if ((camera->ZoomCounter != 0) || (camera->Mode & 0x100) ||
                    ((camera->Mode & 0x80) && (camera->PadPtr->button & R_TRIG))) {
                    camera->AccelA = 6.0f;
                    camera->AccelB = 10.0f;
                } else if ((D_800BE616 != 0) || (camera->AvailableModes & 0x200000)) {
                    if (camera->Mode & 0x40) {
                        camera->unk974 = 1.0f;
                        camera->unk978 = 2.0f;
                    } else {
                        camera->unk974 = 6.0f;
                        camera->unk978 = 8.0f;
                    }
                    func_150495B0(&camera->unk964, camera->unk974, &camera->unk984, 1.0f, 2.0f, camera->SpringTime);
                    func_150495B0(&camera->unk968, camera->unk978, &camera->unk988, 1.0f, 2.0f, camera->SpringTime);
                    camera->AccelA = camera->unk964;
                    camera->AccelB = camera->unk968;
                } else if ((camera->CharPtr->unk102 != 0) || (camera->LockedBehind != 0) ||
                           ((camera->DoingRail != 0) && (camera->RailFlags & 0x40))) {
                    camera->AccelA = 6.0f;
                    camera->AccelB = 8.0f;
                } else if ((camera->CameraButtons & (L_CBUTTONS | R_CBUTTONS)) || (camera->Mode & 0x40)) {
                    camera->AccelA = 1.0f;
                    camera->AccelB = 2.0f;
                } else {
                    camera->AccelA = 2.0f;
                    camera->AccelB = 3.0f;
                }
                if (D_800BEA0C == 0) {
                    if (camera->blocked != 0) {
                        if ((camera->DoingAim != 0) || (D_800BE616 != 0)) {
                            func_150495B0(&camera->TripodMagnitude, camera->IdealTripodMagnitude, &camera->XZVel, 8.0f, 10.0f, camera->SpringTime);
                        } else {
                            func_150495B0(&camera->TripodMagnitude, camera->IdealTripodMagnitude, &camera->XZVel, 6.0f, 8.0f, camera->SpringTime);
                        }
                    } else {
                        func_150495B0(&camera->TripodMagnitude, camera->IdealTripodMagnitude, &camera->XZVel, camera->AccelA,
                                      camera->AccelB, camera->SpringTime);
                    }
                } else if ((D_800895B0 == 0.0f) && ((camera->TonyHitsClear != 0) || (camera->CharPtr->unk102 != 0))) {
                    func_150495B0(&camera->TripodMagnitude, 200.0f, &camera->XZVel, 4.0f, 8.0f, camera->SpringTime);
                } else {
                    func_150495B0(&camera->TripodMagnitude, 200.0f, &camera->XZVel, 0.5f, 1.0f, camera->SpringTime);
                }
            }
            camera->eye.x = camera->TripodMagnitude * sinf(angle) + camera->target.x;
            camera->eye.z = camera->TripodMagnitude * cosf(angle) + camera->target.z;
            camera->TripodMagnitude = sqrtf((camera->eye.x - camera->target.x) * (camera->eye.x - camera->target.x) +
                                  (camera->eye.z - camera->target.z) * (camera->eye.z - camera->target.z));
            if (camera->IdealTripodMagnitude + camera->IdealTripodMagnitude < camera->TripodMagnitude) {
                if ((camera->PadPtr->button & (L_CBUTTONS | R_CBUTTONS)) && (D_800BE616 == 0)) {
                    camera->PadPtr->button &= ~(L_CBUTTONS | R_CBUTTONS);
                    camera->CameraButtons &= ~(L_CBUTTONS | R_CBUTTONS);
                }
            }
            camera->IdealTripodMagnitude = savedDistance;
        }
    }
}

/* Keeps object `index` of movement_obj in frame together with the player (0 clears it). */
void func_15124770(Camera *camera, s32 index) {
    if (index != 0) {
        camera->InterestCharIndex = index;
        camera->InterestCharPtr = &movement_obj[index];
    } else {
        camera->InterestCharIndex = 0;
        camera->InterestCharPtr = NULL;
    }
}

/* Moves the focus point toward the player (or toward the midpoint with pairObj) at followRate, then
   adds the sideways offset. */
void func_151247C0(Camera *camera) {
    f32 pad[3]; // matching: three unused stack words in the frame
    f32 sideX;
    Actor *obj;
    Actor *pair;
    ActorCtl *ext;
    u8 slow;
    f32 sideZ;

    if (camera->transition.state == 0) {
        ext = camera->mv;
        slow = (ext != NULL) ? ((s32)ext->ropePtr | ext->ladder) : 0;
        if (slow) {
            camera->CharTrackingSpeedTarget = 0.25f;
        } else {
            camera->CharTrackingSpeedTarget = 1.0f;
        }
        if (camera->Snap) {
            camera->CharTrackingSpeed = camera->CharTrackingSpeedTarget;
            camera->CharTrackingSpeedVel = 0;
        } else {
            func_150495B0(&camera->CharTrackingSpeed, camera->CharTrackingSpeedTarget, &camera->CharTrackingSpeedVel,
                          camera->AccelA * 0.25f, camera->AccelB * 0.25f, camera->SpringTime);
        }
        if (camera->InterestCharIndex) {
            if (camera->Snap) {
                obj = camera->CharPtr;
                pair = camera->InterestCharPtr;
                camera->focus.x = (obj->xpos + pair->xpos) * 0.5f;
                camera->focus.y = (obj->ypos + pair->ypos) * 0.5f;
                camera->focus.z = (obj->zpos + pair->zpos) * 0.5f;
            } else {
                obj = camera->CharPtr;
                pair = camera->InterestCharPtr;
                camera->focus.x += (((obj->xpos + pair->xpos) * 0.5f) - camera->focus.x) * camera->CharTrackingSpeed;
                camera->focus.y = (obj->ypos + pair->ypos) * 0.5f;
                camera->focus.z += (((obj->zpos + pair->zpos) * 0.5f) - camera->focus.z) * camera->CharTrackingSpeed;
            }
        } else {
            if (camera->Snap) {
                obj = camera->CharPtr;
                camera->focus.x = obj->xpos;
                camera->focus.z = obj->zpos;
            } else {
                obj = camera->CharPtr;
                camera->focus.x += (obj->xpos - camera->focus.x) * camera->CharTrackingSpeed;
                camera->focus.z += (obj->zpos - camera->focus.z) * camera->CharTrackingSpeed;
            }
            camera->focus.y = obj->ypos;
        }
        if (camera->sideOffset != 0.0f || camera->sideOffsetTarget != 0.0f) {
            if (camera->Snap) {
                camera->sideOffset = camera->sideOffsetTarget;
                camera->sideOffsetVel = 0;
            } else {
                func_150495B0(&camera->sideOffset, camera->sideOffsetTarget, &camera->sideOffsetVel, 4.0f, 6.0f, camera->SpringTime);
            }
            sideX = camera->sideOffset * sinf(camera->CameraAngleRad - 1.5707963705062866f);
            sideZ = camera->sideOffset * cosf(camera->CameraAngleRad - 1.5707963705062866f);
            camera->focus.x += sideX;
            camera->focus.z += sideZ;
        }
        camera->target.x = camera->focus.x;
        camera->target.z = camera->focus.z;
        func_15125594(camera);
    }
}

/* Unit view direction from the yaw and pitch angles. */
void func_15124AB4(Camera *camera) {
    camera->forward.y = -sinf(camera->CameraPitchRad);
    camera->forward.z = cosf(camera->CameraPitchRad);
    camera->forward.x = sinf(camera->CameraAngleRad) * camera->forward.z;
    camera->forward.z = cosf(camera->CameraAngleRad) * camera->forward.z;
}

/* Loads the follow distance and height of the current preset (or of a mode/player state override). */
void func_15124B18(Camera *camera) {
    s32 mode80;
    f32 *preset;

    if (camera->DistIndex == 0) {
        camera->DistIndex = 4;
    }
    mode80 = camera->Mode & 0x80;
    if (mode80 && (camera->PadPtr->button & R_TRIG)) {
        camera->height = 40.0f;
        camera->unk34C = 40.0f;
        camera->IdealTripodMagnitude = 150.0f;
    } else if (mode80) {
        camera->height = camera->eye.y - camera->unk354;
    } else if (mode80 && (camera->CharPtr->unk102 == 0) && (level != 0x17)) {
        camera->height = 40.0f;
        camera->unk34C = 40.0f;
        camera->IdealTripodMagnitude = 150.0f;
    } else if (camera->CharPtr->unk102 != 0) {
        camera->height = 40.0f;
        camera->unk34C = 40.0f;
        camera->IdealTripodMagnitude = 194.0f;
    } else {
        preset = D_800A34B0[camera->DistIndex];
        camera->IdealTripodMagnitude = preset[0];
        camera->height = preset[1];
        camera->unk34C = camera->height;
    }
}

/* Height the camera target follows (player height scaled per state, plus status offsets). Stores it
   in *outY when outY is given; otherwise eases target.y toward it. */
void func_15124C38(Camera *camera, f32 *outY) {
    f32 goalY;
    f32 factor;
    f32 height;

    if ((camera->transition.state == 0) || (outY != 0)) {
        if (camera->mv != NULL) {
            if ((camera->mv->transport & 0xF) == 1) {
                if (camera->Mode & 0x100) {
                    height = camera->mv->height * 0.65f;
                } else {
                    height = camera->mv->height * 0.55f;
                }
            } else if (camera->Mode & 0x100) {
                if (camera->CharContext == 0) {
                    if (camera->aim.pitch < -15.0f) {
                        height = camera->CharPtr->unkCC + camera->mv->height * 0.9f;
                    } else {
                        height = camera->CharPtr->unkCC + camera->mv->height * 0.75f;
                    }
                } else {
                    height = camera->CharPtr->unkCC + camera->mv->height * 0.85f;
                }
            } else if ((camera->Snap == 0) && (camera->blocked != 0)) {
                height = camera->CharPtr->unkCC + (f32)camera->mv->height;
            } else {
                height = camera->CharPtr->unkCC + camera->mv->height * 0.75f;
            }
            if (camera->SpecialFlags & 0x100) {
                height += 50.0f;
            } else if (camera->SpecialFlags & 0x200) {
                height += 150.0f;
            } else if (camera->SpecialFlags & 0x800) {
                height = 135.0f;
            }
        } else {
            s16 top;
            s16 discard1;
            s16 discard2;

            func_1507C3E0(camera->CharPtr, &top, &discard1, &discard2);
            height = top;
        }
        if (camera->mv->death_State != 0) {
            f32 z;
            f32 y;
            f32 x;

            func_1507E1D0(camera->CharPtr, &x, &y, &z);
            func_150495B0(&camera->target.y, y, &camera->LOSYVel, 4.0f, 8.0f, camera->SpringTime);
            return;
        }
        {
            f32 accel;
            f32 maxSpeed;

            if (((camera->CharContext == 0xC) || (camera->CharContext == 9) || (camera->CharContext == 0x38) || (camera->CharContext == 0x39) ||
                 (camera->CharContext == 0x37) || (camera->CharContext == 0x3B) || (camera->CharContext == 0x12) || (camera->CharContext == 0x15) ||
                 (camera->CharContext == 0x26) || (camera->CharContext == 0x3A)) &&
                (camera->Mode & 0x100)) {
                height += 30.0f;
            }
            if ((camera->Mode & 0x80) || (func_15125490(camera) != 0)) {
                goalY = camera->CharPtr->ypos + height * 0.25f;
            } else if (camera->AvailableModes & 0x20000000) {
                goalY = camera->CharPtr->floorY + height;
            } else if ((D_800C3671 != 0) || (camera->CharPtr->unk137 != 0) ||
                       (camera->CharPtr->animseq.uh == 0x194) || (camera->CharPtr->inwater == 1)) {
                goalY = camera->CharPtr->ypos + height;
            } else if ((-10000.0f == camera->waterY) && !(camera->AvailableModes & 0x20000000) && !(camera->AvailableModes & 0x200000) &&
                       (camera->CharPtr->shad < 170.0f) && (300.0f < camera->TripodMagnitude)) {
                goalY = camera->CharPtr->floorY + height;
            } else {
                factor = camera->CharPtr->shad * (1.0f / 300.0f);
                factor = (factor < 0.0f) ? 0.0f : ((1.0f < factor) ? 1.0f : factor);
                goalY = camera->CharPtr->floorY + camera->CharPtr->shad * factor + height;
            }
            if (camera->CharPtr->unk102 != 0) {
                if (camera->TripodMagnitude < 100.0f) {
                    if (camera->eye.y < camera->CharPtr->ypos) {
                        goalY = camera->CharPtr->ypos - 10.0f;
                    } else {
                        goalY = camera->CharPtr->ypos + 10.0f;
                    }
                } else {
                    goalY = camera->CharPtr->ypos;
                }
            }
            if (outY != 0) {
                *outY = goalY;
                return;
            }
            if (camera->Snap != 0) {
                camera->target.y = goalY;
                camera->LOSYVel = 0.0f;
                return;
            }
            if (camera->SpecialFlags & 0x80) {
                accel = 5.0f;
                maxSpeed = 8.0f;
            } else if (camera->Mode & 0x100) {
                accel = 5.0f;
                maxSpeed = 9.0f;
            } else if (camera->CharPtr->unk102 != 0) {
                accel = 7.0f;
                maxSpeed = 9.0f;
            } else {
                accel = 4.0f;
                maxSpeed = 8.0f;
            }
            if ((camera->AvailableModes & 0x200000) && (goalY < camera->prevTarget.y)) {
                accel += accel;
                maxSpeed += maxSpeed;
            }
            func_150495B0(&camera->target.y, goalY, &camera->LOSYVel, accel, maxSpeed, camera->SpringTime);
        }
    }
}

/* Pitch of the eye->target line (degrees and radians), plus the pitch offsets unk38C / unk3A8. */
void func_1512523C(Camera *camera) {
    f32 dz;
    f32 dx;
    f32 dist;

    dx = camera->target.x - camera->eye.x;
    dz = camera->target.z - camera->eye.z;
    dz = sqrtf((dx * dx) + (dz * dz));
    if (dz < 0.0f) {
        dist = -dz;
    } else {
        dist = dz;
    }
    dx = func_150484A0(camera->eye.y - camera->target.y, dist) * 57.2957763671875f;
    camera->CameraPitch = dx;
    if (dx > 180.0f) {
        camera->CameraPitch -= 360.0f;
    }
    camera->CameraPitch = -camera->CameraPitch;
    camera->CameraPitch -= camera->APitchAngle;
    camera->CameraPitch += camera->AddOnPitch;
    camera->CameraPitchRad = camera->CameraPitch * DEG2RAD;
}

/* Yaw (degrees and radians) of the eye->target line. */
void func_15125330(Camera *camera) {
    Vec3f delta;

    delta.x = camera->target.x - camera->eye.x;
    delta.y = 0.0f;
    delta.z = camera->target.z - camera->eye.z;

    camera->CameraAngle = func_15048FC8(&delta);
    camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
}

/* modeIndex = bit number of the (one-hot) mode. */
void func_15125394(Camera *camera) {
    s32 i = 0;

    while (!(camera->Mode & (1 << i))) {
        i++;
    }
    camera->ModeIndex = i;
}

/* Mode 0x40000 (fixed camera): keeps eye and target, refreshes the angles. Returns 1 in that mode. */
s32 func_151253CC(Camera *camera) {
    if (camera->Mode & 0x40000) {
        camera->AvailableModes &= ~0x4F;
        camera->AvailableModes |= 0x2680;
        camera->APitchAngle = 0.0f;
        camera->YawAngle = 0.0f;
        camera->AddOnPitch = 0.0f;
        func_15125330(camera);
        func_1512523C(camera);
        camera->TrueCameraAngle = camera->CameraAngle;
        camera->TrueCameraAngleRad = camera->CameraAngle * DEG2RAD;
        camera->CameraPitchRad = camera->CameraPitch * DEG2RAD;
        func_15124AB4(camera);
        if (camera->mv->SniperState != 0) {
            func_15127EB8(camera);
        }
        func_1512C490(camera);
        return 1;
    }
    return 0;
}

/* Whether the player is in water far (over 300 units) from unk118; 0 when closer than 100. */
s32 func_15125490(Camera *camera) {
    Actor *player = camera->CharPtr;
    s32 depth;

    if (player->inwater == 1) {
        depth = fabsf(player->ypos - player->unk118);
        if (depth < 100) {
            return 0;
        }
        if (depth > 300) {
            return 1;
        }
    } else {
        return 0;
    }
    // matching: no return for 100..300 (callers see the leftover v0), as in the original
}

/* Attaches the camera to object `index` and leaves the aim state. */
void func_151254F4(Camera *camera, s32 index) {
    camera->TrueCameraAngleRad = camera->TrueCameraAngle * DEG2RAD;
    camera->CameraPitchRad = camera->CameraPitch * DEG2RAD;
    func_15124AB4(camera);
    func_151239CC(camera, CAMERA_MODE_AIM);
    camera->mv->AimState = 0;
    camera->DoingAim = 0;
    camera->CharPtr = &movement_obj[index];
    camera->CharTrackingSpeed = 0.0f;
}

/* Recomputes pitch, yaw, unk380 (yaw minus unk5E8, wrapped) and the view direction. */
void func_15125594(Camera *camera) {
    func_1512523C(camera);
    func_15125330(camera);

    camera->TrueCameraAngle = camera->CameraAngle - camera->YawAngle;
    if (camera->CameraAngle < camera->YawAngle) {
        camera->TrueCameraAngle += 360.0f;
    }
    camera->TrueCameraAngleRad = camera->TrueCameraAngle * DEG2RAD;
    func_15124AB4(camera);
}

/* Sets unk24C / unk250 (called after a preset change). */
void func_15125608(Camera *camera) {
    camera->DistSpeedXZ = 3.0f;
    camera->DistSpeedY = 2.5f;
}

/* Counts down the four per-player camera hold timers (D_800DBFF4[0..3]). */
void func_15125628(void) {
    // matching: a pointer per timer keeps IDO from sharing one base address (target re-lui's each access)
    u8 *timer0 = &D_800DBFF4[0];
    u8 *timer1 = &D_800DBFF4[1];
    u8 *timer2 = &D_800DBFF4[2];
    u8 *timer3 = &D_800DBFF4[3];
    u8 value;

    value = *timer0;
    if (value != 0) {
        *timer0 = value - 1;
    }
    value = *timer1;
    if (value != 0) {
        *timer1 = value - 1;
    }
    value = *timer2;
    if (value != 0) {
        *timer2 = value - 1;
    }
    value = *timer3;
    if (value != 0) {
        *timer3 = value - 1;
    }
}

/* Raises this player's hold timer to at least `frames`. */
void func_15125690(Camera *camera, s32 frames) {
    u8 *timer = &D_800DBFF4[camera->CNumber];

    if (*timer < frames) {
        *timer = frames;
    }
}

/* Camera sway: while armed (mode 0x80000, or status bit 8 outside mode 0x40000) swayPhase advances by
   a random 0/2/4 degrees per frame, wrapping at 2 pi; otherwise it decays. The resulting swayAmount
   orbits eye and target around each other (func_1508EF80). */
void func_151256BC(Camera *camera) {
    f32 twoPi;
    f32 amount;
    f32 eyeScale;
    f32 targetScale;

    if ((camera->Mode & 0x80000) || ((camera->SpecialFlags & 8) && !(camera->Mode & 0x40000))) {
        amount = (f32)(func_150ADA20() % 3);
        twoPi = 6.2831854820251465f;
        camera->DrunkRoll += ((amount + amount) * DEG2RAD) * D_800BE9A4;
        while (twoPi < camera->DrunkRoll) {
            camera->DrunkRoll -= twoPi;
        }
        camera->Roll += ((sinf(camera->DrunkRoll) * 4.0f) - camera->Roll) * 0.04f;
        amount = camera->Roll * 0.05f;
        if (camera->Mode & 0x80000) {
            eyeScale = 4.0f;
            targetScale = 4.0f;
        } else {
            eyeScale = 1.0f;
            targetScale = 20.0f;
        }
        func_1508EF80(&camera->eye, &camera->target, amount * eyeScale, &camera->eye);
        func_1508EF80(&camera->target, &camera->eye, amount * targetScale,
                      &camera->target);
    } else if (camera->DrunkRoll != 0.0f) {
        camera->DrunkRoll -= camera->DrunkRoll * 0.08f;
        camera->Roll += ((sinf(camera->DrunkRoll) * 4.0f) - camera->Roll) * 0.04f;
        amount = camera->Roll * 0.05f;
        func_1508EF80(&camera->eye, &camera->target, amount, &camera->eye);
        func_1508EF80(&camera->target, &camera->eye, amount * 20.0f, &camera->target);
    }
}

/* Pushes the deep-water mode 0x80 (slot 13) while func_15125490 says so, and pops it otherwise. */
void func_15125924(Camera *camera) {
    s32 extUnk4E;
    s32 extUnk1C;

    if (camera->mv != NULL) {
        extUnk1C = camera->mv->water_up;
        extUnk4E = camera->mv->transport;
    } else {
        extUnk1C = 0;
        extUnk4E = 0;
    }

    if (!(camera->Mode & 0x40)) {
        if ((camera->AvailableModes & 0x4000) || (camera->CharPtr->unk102 != 0)) {
            if (func_15125490(camera) != 0) {
                if ((extUnk4E == 0) && (extUnk1C == 0)) {
                    if (func_15123934(camera, 0x80, 1, 1, CAMERA_MODE_SFLY) != 0) {
                        if (camera->cmdRunning == 0) {
                            func_15124B18(camera);
                            camera->SpecialFlags |= 0x1000;
                        }
                    }
                    return;
                }
            }
            if (camera->Mode & 0x80) {
                func_151239CC(camera, CAMERA_MODE_SFLY);
                camera->YOffsetTget = 0.0f;
            }
            if (camera->SpecialFlags & 0x1000) {
                camera->SpecialFlags &= ~0x1000;
                camera->height = camera->unk344 = camera->eye.y - camera->unk354;
            }
        }
    }
}

/* Zone 0x2A: hands the camera to a scripted shot (gCamShot) behind and beside the player. */
void func_15125A6C(Camera *camera) {
    u8 zone;
    f32 radius;
    f32 sideRadius;
    f32 yaw;
    f32 angle;
    f32 sideAngle;

    zone = camera->CharContext;
    if (zone == 0x2A) {
        yaw = camera->CharPtr->theta;
        if (camera->Mode != 0x40000) {
            func_1512D560(camera, 5, 0);
            camera->DoingSpecialContextCase = 1;
        }
        D_800DBFF4[0] = 3;
        gCamShot.mode = 1;
        gCamShot.unk4 = 0;
        radius = 138.0f;
        sideRadius = 46.0f;
        angle = yaw * DEG2RAD;
        sideAngle = (yaw - 90.0f) * DEG2RAD;
        // matching: the position fields go through their own symbols (gCamShot.eye.x.. moves code)
        D_800C3614 = (camera->CharPtr->xpos + (radius * sinf(angle))) + (sinf(sideAngle) * sideRadius);
        D_800C3618 = camera->CharPtr->ypos + 96.0f;
        D_800C361C = (camera->CharPtr->zpos + (radius * cosf(angle))) + (cosf(sideAngle) * sideRadius);
        gCamShot.at.angles.sway = 0.0f;
        D_800C3620.y = -180.0f;
        gCamShot.unk8 = 0.0f;
        gCamShot.at.angles.yaw = yaw - 180.0f;
        D_800C365C[0] = 0;
        func_1512D560(camera, 7, &gCamShot);
        return;
    }
    if (camera->DoingSpecialContextCase != 0) {
        func_1512D560(camera, 6, 0);
        camera->DoingSpecialContextCase = 0;
    }
}

/* Zones 3 and 0x1A (and 0x1A in level 0x42): plays the zone effect when unk7CC runs out and forces
   the R-trigger camera on; leaving the zone stops the effect. */
void func_15125C40(Camera *camera) {
    s32 level42;
    s32 active;
    s32 zone;

    zone = camera->CharContext;
    level42 = (D_800D1940 == 0x42) && (zone == 0x1A);
    active = (zone == 3) || (zone == 0x1A) || level42;
    camera->DelayOneFrame--;
    if (active) {
        if (camera->DelayOneFrame == 0) {
            if (level42) {
                func_1509BFB0(3, 0x9000, 0x18, (s32)camera->CharPtr->theta, 0, 250);
            } else if (zone == 0x1A) {
                func_1509BFB0(3, 0x9000, 0x18, 0, 0, 250);
            }
            camera->SpecialFlags |= 2;
            camera->PadPtr->button |= R_TRIG;
            camera->CameraButtons |= R_TRIG;
            camera->DelayOneFrame = 1;
        }
    } else {
        if ((camera->SpecialFlags & 2) && !active) {
            func_1509BFB0(1, 0x9000, 0x10, 0);
            func_1509BFB0(1, 0x9000, 0xF, 0);
            camera->SpecialFlags &= ~2;
        }
        camera->DelayOneFrame = 2;
    }
}

/* Filters this frame's camera buttons by zone and player state (C buttons, R trigger). */
void func_15125DB4(Camera *camera) {
    s32 zone;

    zone = camera->CharContext;
    if ((camera->mv->death_State != 0) || (D_80084480 != 0)) {
        camera->CameraButtons = 0;
        camera->PadPtr->button &= ~(R_TRIG | CAM_C_BUTTONS);
        return;
    }

    if (zone != 0) {
        if ((zone == 2) || (zone == 0x12) || (zone == 0x13) || (zone == 0xA) ||
            (zone == 0x34) || (zone == 0x10) || (zone == 0xF)) {
            if (zone == 0xF) {
                func_1509BFB0(3, 0x9000, 0x18, 0x25, 0x13, 480);
            }
            camera->PadPtr->button |= R_TRIG;
            camera->CameraButtons |= R_TRIG;
            camera->PadPtr->button &= ~CAM_C_BUTTONS;
        } else {
            camera->CameraButtons = D_800BE710[camera->PadNumber];
        }
        if ((zone == 3) || (zone == 0xD) || (zone == 0x1A) || (zone == 0x2A)) {
            camera->CameraButtons &= ~CAM_C_BUTTONS;
            camera->PadPtr->button &= ~CAM_C_BUTTONS;
        }
        if (zone == 0x29) {
            camera->PadPtr->button |= R_TRIG;
            camera->CameraButtons |= R_TRIG;
        }
    } else {
        if (camera->Mode & 0x100) {
            camera->CameraButtons &= ~CAM_C_BUTTONS;
            camera->PadPtr->button &= ~CAM_C_BUTTONS;
        }
    }

    if ((camera->CharPtr->inwater == 1) && (camera->Mode & 0x80)) {
        camera->CameraButtons &= ~(R_TRIG | U_CBUTTONS | D_CBUTTONS);
        camera->PadPtr->button &= ~(U_CBUTTONS | D_CBUTTONS);
    }

    if (camera->AvailableModes & 0x200000) {
        camera->CameraButtons &= ~CAM_C_BUTTONS;
    }

    if ((D_800C3671 != 0) || (camera->SpecialFlags & 0x80)) {
        camera->CameraButtons &= ~(R_TRIG | CAM_C_BUTTONS);
        camera->PadPtr->button &= ~(R_TRIG | CAM_C_BUTTONS);
    }

    if ((camera->mv->matrix_physics != 0) || (camera->mv->on_fire != 0) || (camera->mv->jump_on_char != 0)) {
        camera->CameraButtons &= ~R_TRIG;
        camera->PadPtr->button &= ~R_TRIG;
    }

    if ((camera->mv->ladder != 0) || ((camera->SHitsOffLastFrame != 0) && (camera->mv->unk1B3 != 0))) {
        camera->CameraButtons &= ~(R_TRIG | CAM_C_BUTTONS);
        camera->PadPtr->button &= ~(R_TRIG | CAM_C_BUTTONS);
    }

    if (((camera->Mode & 0x100) && (camera->DoingAim == 0)) || (camera->DoingAim == 3)) {
        camera->CameraButtons &= ~R_TRIG;
        camera->PadPtr->button &= ~R_TRIG;
    }

    if ((camera->Mode & 0x40) && (camera->AimTimeOuts != 0)) {
        camera->AimTimeOuts -= D_800BE9E4;
        camera->CameraButtons &= ~R_TRIG;
        camera->PadPtr->button &= ~R_TRIG;
        if (camera->AimTimeOuts <= 0) {
            camera->AimTimeOuts = 0;
        }
    }
}

/* Moves focus and target height, and flags whether the player moved since the last frame. */
void func_15126138(Camera *camera) {
    Actor *player;

    func_151247C0(camera);

    player = camera->CharPtr;

    if (((player->xpos != camera->lastPlayerPos.x) ||
         (player->ypos != camera->lastPlayerPos.y) ||
         (player->zpos != camera->lastPlayerPos.z)) && ((camera->Mode & ~0x100) != 0)) {
        camera->CharMoved = 1;
        camera->AlignBehindCounter = 60;
        camera->DontMoveTilCharacterDoes = 0;
    } else {
        camera->CharMoved = 0;
    }

    if (camera->AvailableModes & 0x200000) {
        player = camera->CharPtr;
        if (player->type != 0x1E) {
            if (player->unk102 == 0) {
                if (camera->PadPtr->button & CAM_C_BUTTONS) {
                    if (!(camera->Mode & 0x40)) {
                        func_151220D0(camera);
                    }
                }
            }
        }
    }
    func_15124C38(camera, NULL);
}

/* Aim eye position for `obj`: a point fixed to one of its bone matrices for the zone values that have one,
   else the object position raised to 3/4 of its height. */
void func_1512623C(Actor *obj, ActorCtl *ext, s32 zone, f32 *outX, f32 *outY, f32 *outZ, s32 lowView) {
    s32 useObjectPos;
    Vec3f localPos;
    Vec3f worldPos;
    s32 bone;
    Mtx *boneMtx;
    f32 offsetY;

    useObjectPos = 1;
    if (obj->ob_matrix_ptr != NULL) {
        useObjectPos = 0;
        if (lowView != 0) {
            bone = 3;
            offsetY = 20.0f;
            localPos.x = 0.0f;
            localPos.y = offsetY;
            localPos.z = 0.0f;
        } else if (zone == 0x1B) {
            bone = 4;
            offsetY = 116.0f;
            localPos.y = offsetY;
            localPos.x = 0.0f;
            localPos.z = 130.0f;
        } else {
            useObjectPos = 1;
        }

        if (useObjectPos == 0) {
            boneMtx = (Mtx *)obj->ob_matrix_ptr;
            boneMtx += bone;
            func_15143134(&localPos, &worldPos, boneMtx);
            *outX = worldPos.x;
            *outY = worldPos.y;
            *outZ = worldPos.z;
        }
    }

    if (useObjectPos != 0) {
        *outX = obj->xpos;
        *outY = obj->ypos + (ext->height * 0.75f);
        *outZ = obj->zpos;
    }
}

/* First-person aiming camera (R trigger in the aim areas): sets it up on the first frame, then steers
   yaw/pitch with the stick and zoom with C-up/C-down. Returns 1 while aiming. */
s32 func_15126378(Camera *camera) {
    ActorCtl *ext;
    Actor *viewer;
    f32 zoomRatio;
    f32 aspect;
    f32 maxZoom;
    f32 zoomStep;
    s32 zone;
    f32 scale;
    f32 angle;
    f32 t;
    f32 yawRad;
    f32 minRange;
    f32 aimKey;
    s32 buttons;
    Vec3f dir;
    f32 rotated;
    f32 maxRange;
    f32 yawDelta;
    f32 pitchDelta;
    s32 valid;
    s32 state;

    scale = 0.02f;
    ext = camera->mv;
    aspect = D_800BE628[camera->CNumber].fovY / D_800BE628[camera->CNumber].fovX;
    maxZoom = -(D_800BE628[camera->CNumber].fovX - 5.0f);
    zoomStep = D_800BE628[camera->CNumber].fovX * scale;
    zone = camera->CharContext;
    if (((zone == 0xC) && (camera->CharPtr->animseq.uh != 0x76)) || (zone == 0x1B)) {
        camera->CameraButtons |= R_TRIG;
        camera->PadPtr->button |= R_TRIG;
    }
    if ((ext->SniperState == 0) && ((camera->PadPtr->stick_x != 0) || (camera->PadPtr->stick_y != 0))) {
        ext->aimReset = 1;
    }
    // matching: [linked] - 1 rather than [linked - 1]
    viewer = (camera->CharPtr->attach_char != 0) ? &movement_obj[camera->CharPtr->attach_char] - 1 : camera->CharPtr;
    valid = (ext->weapon_status == 2) || (zone == 0x1B) || (zone == 0x1F) || (zone == 0x29);
    if (((zone == 0x23) || (zone == 0x24) || (zone == 0x16) || (zone == 0x18) || (zone == 0x41) ||
         (zone == 0xC) || (zone == 0x14) || (zone == 0x3F) || (zone == 0x1B) || (zone == 0x1F) ||
         (zone == 0x29)) &&
        (camera->PadPtr->button & R_TRIG) && valid &&
        (((s32)camera->CharPtr->shad == 0) || ((s32)viewer->shad == 0)) &&
        (camera->CharPtr->stunned == 0) && (ext->SniperState == 0)) {
        camera->mv->AimState = 0;
        camera->DoingAim = 0;
        if (((zone == 0x1F) || (zone == 0x29)) && !(camera->SpecialFlags & 4)) {
            t = *(s16 *)&viewer->taskint[0] * scale;
            angle = (viewer->theta - 90.0f) - t;
            ext->aimReset = 1;
        } else {
            angle = camera->CharPtr->theta - 90.0f;
        }
        func_15048758(&angle);
        yawRad = angle * DEG2RAD;
        func_151239CC(camera, CAMERA_MODE_AIM);
        ext->SniperState = 2;
        func_151C9BA0(camera, zone);
        func_1512623C(viewer, camera->mv, zone, &camera->eye.x, &camera->eye.y, &camera->eye.z, camera->SpecialFlags & 0x10);
        camera->target.x = (sinf(yawRad) * 100.0f) + camera->eye.x;
        camera->target.y = camera->CharPtr->ypos + (camera->mv->height * 0.75f);
        camera->target.z = (cosf(yawRad) * 100.0f) + camera->eye.z;
        ext->aimEye = camera->eye;
        ext->unk18C = camera->CameraAngle;
        ext->unk160 = camera->eye;
        if (zone != ext->aimZone) {
            if (zone == 0x1B) {
                t = 1.0f;
            } else if (zone == 0x16) {
                t = 0.75f;
            } else {
                t = 0.5f;
            }
            if ((zone == 0x18) || (zone == 0x41) || (zone == 0x3F) || (zone == 0x14) || (zone == 0x12)) {
                t = (t < 0.0f) ? 0.0f : ((t > 0.6f) ? 0.6f : t);
            }
            ext->aimZoom = ((0.0f - maxZoom) * t) + maxZoom;
            ext->aimReset = 1;
            ext->aimZone = zone;
        }
        if (ext->aimReset != 0) {
            ext->aimYawTarget = angle;
            ext->aimPitchTarget = 0.0f;
            ext->aimPitch = 0.0f;
            ext->aimYawVel = 0.0f;
            ext->aimPitchVel = 0.0f;
            ext->aimReset = 0;
            ext->aimYaw = ext->aimYawTarget;
        }
        ext->aimZoomY = ext->aimZoom * aspect;
        camera->FOVX = ext->aimZoom;
        camera->FOVY = ext->aimZoomY;
        camera->TgetFOVX = ext->aimZoom;
        camera->TgetFOVY = ext->aimZoomY;
        func_1510B32C(camera->CNumber, camera->FOVX, camera->FOVY, 1.0f);
        D_800BE628[camera->CNumber].perspScale = 1.0f;
        func_150627D4(camera->CharPtr);
        D_800DBFF4[camera->CNumber] = 2;
        if ((zone == 0x24) || (zone == 0x23)) {
            func_1000FA64(0x605, 0, 0, 0, 0x6590, 300, 299, (SoundRecordCallback)func_15127520, camera,
                          (u32)(-camera->TgetFOVX * 10000.0f), 0, 0);
        } else if ((zone == 0x1B) || (zone == 0x1F) || (zone == 0x29)) {
            aimKey = ext->aimYawTarget * ext->aimPitchTarget;
            func_1000FA64(0x622, 0, 0, 0, 0x6590, 300, 299, (SoundRecordCallback)func_15127520, camera,
                          (u32)(-camera->TgetFOVX * 10000.0f) | 0x80000000, 0x100, 0);
            func_1000FA64(0x622, 0, 0, 0, 0x3E80, 300, 299, (SoundRecordCallback)func_151277B0, camera, *(s32 *)&aimKey, 0x100, 0);
        }
        viewer->unk2FC |= 1 << camera->CNumber;
        viewer->unk74 |= 1 << camera->CNumber;
        { typedef s32 LineMark; } // matching: -g3 line marker; as1 then schedules the SniperState load after the a2 reload
    }
    state = ext->SniperState;
    if (state == 0) {
        return 0;
    }
    if (state == 2) {
        buttons = camera->PadPtr->button;
        func_1512623C(viewer, camera->mv, zone, &camera->eye.x, &camera->eye.y, &camera->eye.z, camera->SpecialFlags & 0x10);
        zoomRatio = camera->FOVX / maxZoom;
        zoomRatio = (zoomRatio < 0.0f) ? 0.0f : ((zoomRatio > 1.0f) ? 1.0f : zoomRatio);
        if (!(buttons & R_TRIG) || (zone == 0) || (ext->on_fire != 0) ||
            ((camera->CharPtr->energy == 0) && (zone != 0x29)) ||
            ((camera->CharPtr->stunned != 0) && (camera->CharPtr->attach_char == 0) && ((s32)camera->CharPtr->shad != 0) &&
             (zone != 0x1F) && (zone != 0x29))) {
            func_15127EB8(camera);
            return 0;
        }
        if (zone == 0x1B) {
            minRange = 19.0f;
            maxRange = 31.0f;
        } else {
            minRange = 25.0f;
            maxRange = 90.0f;
        }
        if (camera->PadPtr->stick_x < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        t = 1.0f / (((maxRange - minRange) * zoomRatio) + minRange);
        yawDelta = valid * ((f32)camera->PadPtr->stick_x * (f32)camera->PadPtr->stick_x * (t * t));
        if (camera->PadPtr->stick_y < 0) {
            valid = -1;
        } else {
            valid = 1;
        }
        pitchDelta = valid * ((f32)camera->PadPtr->stick_y * (f32)camera->PadPtr->stick_y * (t * t));
        if (zone == 0x1B) {
            yawDelta *= 0.25f;
            pitchDelta *= 0.5f;
            yawDelta = (yawDelta < -3.0f) ? -3.0f : ((yawDelta > 3.0f) ? 3.0f : yawDelta);
            pitchDelta = (pitchDelta < -3.0f) ? -3.0f : ((pitchDelta > 3.0f) ? 3.0f : pitchDelta);
        } else if (zone == 0xC) {
            yawDelta *= 1.5f;
            pitchDelta *= 1.5f;
        }
        ext->aimYawTarget -= yawDelta;
        ext->aimPitchTarget += pitchDelta;
        func_15048758(&ext->aimYawTarget);
        if ((zone == 0x1F) || (zone == 0x29)) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 30.0f) ? 30.0f : ext->aimPitchTarget);
        } else if (zone == 0x1B) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -60.0f) ? -60.0f
                                : ((ext->aimPitchTarget > 50.0f) ? 50.0f : ext->aimPitchTarget);
        } else {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        }
        func_15049688(&ext->aimYaw, ext->aimYawTarget, &ext->aimYawVel, 6.0f, 8.0f, camera->SpringTime);
        func_15049688(&ext->aimPitch, ext->aimPitchTarget, &ext->aimPitchVel, 6.0f, 8.0f, camera->SpringTime);
        if ((zone != 0x16) && (zone != 0xC) && ((zone != 0x29) || (zone != 0x1F) || (D_800BE616 != 0)) &&
            ((buttons & (U_CBUTTONS | D_CBUTTONS)) != (U_CBUTTONS | D_CBUTTONS))) {
            if ((buttons & U_CBUTTONS) && (maxZoom < camera->TgetFOVX) && (zone != 0xC)) {
                camera->TgetFOVX -= zoomStep;
                camera->TgetFOVY -= zoomStep * aspect;
            } else if ((buttons & D_CBUTTONS) && (camera->TgetFOVX < 0.0f)) {
                camera->TgetFOVX += zoomStep;
                camera->TgetFOVY += zoomStep * aspect;
            }
        }
        if (zone == 0x24) {
            camera->TgetFOVX = (camera->TgetFOVX < maxZoom) ? maxZoom : ((camera->TgetFOVX > 0.0f) ? 0.0f : camera->TgetFOVX);
            camera->TgetFOVY = (camera->TgetFOVY < maxZoom * aspect) ? maxZoom * aspect
                        : ((camera->TgetFOVY > 0.0f) ? 0.0f : camera->TgetFOVY);
        } else if (zone == 0x1B) {
            t = maxZoom * 0.5f;
            camera->TgetFOVX = (camera->TgetFOVX < t) ? t : ((camera->TgetFOVX > 0.0f) ? 0.0f : camera->TgetFOVX);
            camera->TgetFOVY = (camera->TgetFOVY < t * aspect) ? t * aspect : ((camera->TgetFOVY > 0.0f) ? 0.0f : camera->TgetFOVY);
        } else {
            t = maxZoom * 0.75f;
            camera->TgetFOVX = (camera->TgetFOVX < t) ? t : ((camera->TgetFOVX > 0.0f) ? 0.0f : camera->TgetFOVX);
            camera->TgetFOVY = (camera->TgetFOVY < t * aspect) ? t * aspect : ((camera->TgetFOVY > 0.0f) ? 0.0f : camera->TgetFOVY);
        }
        camera->FOVX = camera->TgetFOVX;
        camera->FOVY = camera->TgetFOVY;
        ext->aimZoom = camera->TgetFOVX;
        ext->aimZoomY = camera->TgetFOVY;
        dir.x = 1.0f;
        rotated = cosf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.y = sinf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.x = rotated;
        rotated = cosf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.z = sinf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.x = rotated;
        camera->target.x = camera->eye.x + dir.x;
        camera->target.y = camera->eye.y + dir.y;
        camera->target.z = camera->eye.z + dir.z;
        camera->focus.x = camera->CharPtr->xpos;
        camera->focus.y = camera->CharPtr->ypos;
        camera->focus.z = camera->CharPtr->zpos;
        ext->aimTarget = camera->target;
        ext->aimEye = camera->eye;
        ext->aimDir = dir;
        func_15048F90(&ext->aimEye, &ext->aimTarget, &ext->aimDir);
        D_800BE628[camera->CNumber].perspScale = (-0.94f * zoomRatio) + 1.0f;
        func_1510B128(camera->CNumber, camera->FOVX, camera->FOVY, D_800BE628[camera->CNumber].perspScale,
                      (71.0f * zoomRatio) + 54.0f);
        return 1;
    }
    // matching: no return for other aim states, as in the original
}

/* Sound callback while aiming (zoom): follows the player and switches between the D_800894E0[index]
   sounds when the zoom changes or stops. Returns 1 when the aim ended. */
s32 func_15127520(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId) {
    Camera *camera;
    u32 index;
    u32 oldZoom;
    u32 newZoom;
    f32 zoom;
    s32 changed;

    camera = sound->camera;
    index = sound->data >> 31;
    oldZoom = sound->data & 0xFFFFFF;
    zoom = camera->TgetFOVX * -10000.0f;
    changed = 0;

    if (*soundId == D_800894E0[index][2]) {
        *soundId = 0;
    }

    sound->x = camera->CharPtr->xpos;
    sound->y = camera->CharPtr->ypos;
    sound->z = camera->CharPtr->zpos;

    if (camera->mv->SniperState == 2) {
        newZoom = (u32)zoom;
        if (oldZoom != newZoom) {
            changed = 1;
            sound->data = (index << 31) | newZoom;
        }

        if (changed) {
            if (*soundId != D_800894E0[index][0]) {
                if (sound->voice != 0) {
                    func_100111C8(sound->voice);
                }
                sound->voice = 0;
                sound->flags &= ~0x80;
                *soundId = D_800894E0[index][0];
            }
        } else if (*soundId == D_800894E0[index][0]) {
            if (sound->voice != 0) {
                func_100111C8(sound->voice);
            }
            sound->voice = 0;
            *soundId = D_800894E0[index][1];
            func_10010F30(D_800894E0[index][1], 20000, 0x40, 0, 0);
            sound->flags &= ~0x80;
        } else if (sound->flags & 0x80) {
            func_151CC290((s32)camera);
            func_10010F30(D_800894E0[index][2], 20000, 0x40, 0, 0);
        }
    } else {
        return 1;
    }

    sound->flags &= ~0x80;
    return 0;
}
/* Sound callback while aiming (look): plays sound 0x61C while the aim angles keep changing, 0x61D
   once they settle. Returns 1 when the aim ended. */
s32 func_151277B0(CamSound *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *soundId) {
    Camera *camera;
    u32 count;
    u32 oldKey;
    u32 newKey;
    s32 changed;
    f32 key;

    camera = sound->camera;
    oldKey = sound->data & 0xFFFFFF;
    count = sound->data >> 24;
    key = camera->mv->aimYawTarget * camera->mv->aimPitchTarget;
    newKey = *(u32 *)&key & 0xFFFFFF;
    changed = 0;

    if (*soundId == 0x622) {
        *soundId = 0;
    }

    sound->x = camera->CharPtr->xpos;
    sound->y = camera->CharPtr->ypos;
    sound->z = camera->CharPtr->zpos;

    if (camera->mv->SniperState == 2) {
        if (oldKey != newKey) {
            if (count < 10) {
                if ((((oldKey - newKey) < 0) ? -(oldKey - newKey) : (oldKey - newKey)) > 100) {
                    count += D_800BE9E4;
                }
            } else {
                changed = 1;
            }
        } else {
            count = 0;
        }
        sound->data = (count << 24) | newKey;

        if (changed) {
            if (*soundId != 0x61C) {
                if (sound->voice != 0) {
                    func_100111C8(sound->voice);
                }
                sound->voice = 0;
                sound->flags &= ~0x80;
                *soundId = 0x61C;
            }
        } else if (*soundId == 0x61C) {
            if (sound->voice != 0) {
                func_100111C8(sound->voice);
            }
            sound->voice = 0;
            *soundId = 0x61D;
            sound->flags &= ~0x80;
        }
    } else {
        return 1;
    }

    if (sound->voice == 0 && *soundId == 0) {
        sound->flags &= ~0x80;
    }
    return 0;
}

/* Aiming view for an object without a player camera (stick-steered yaw/pitch around its head).
   Returns 1 while aiming. */
s32 func_151279A0(Actor *obj, OSContPad *contPad) {
    ActorCtl *ext;
    s32 zone;
    Vec3f pos;
    Vec3f base;
    f32 dt;
    Vec3f dir;
    f32 rotated;
    f32 pad; // matching: unused stack slot in the frame
    s32 buttons;
    f32 angle;

    ext = obj->mv;
    zone = ext->rt_context_b;
    dt = D_800BE9E4 * (1.0f / 60.0f);
    if ((zone == 0xC) || (zone == 0x1B)) {
        contPad->button |= R_TRIG;
    }
    if (((zone == 0x23) || (zone == 0x24) || (zone == 0x16) || (zone == 0x12) || (zone == 0x18) ||
         (zone == 0x41) || (zone == 0xC) || (zone == 0x14) || (zone == 0x3F) || (zone == 0x1B) ||
         (zone == 0x1F) || (zone == 0x29)) &&
        (contPad->button & R_TRIG) && (ext->SniperState == 0)) {
        ext->SniperState = 2;
        ext->aimEye = base;
        ext->aimYawTarget = obj->theta - 90.0f;
        ext->aimPitchTarget = 0.0f;
        ext->aimPitch = 0.0f;
        ext->aimYawVel = 0.0f;
        ext->aimPitchVel = 0.0f;
        ext->aimYaw = ext->aimYawTarget;
        func_150627D4(obj);
    }
    if (ext->SniperState == 0) {
        return 0;
    }
    if (ext->SniperState == 2) {
        buttons = contPad->button;
        angle = (obj->theta - 90.0f) * DEG2RAD;
        pos.x = obj->xpos;
        pos.y = obj->ypos + (ext->height * 0.75f);
        pos.z = obj->zpos;
        base = pos;
        pos.x = (sinf(angle) * 100.0f) + base.x;
        pos.z = (cosf(angle) * 100.0f) + base.z;
        base.y += obj->ypos - obj->old_ypos;
        if (!(buttons & R_TRIG) || (zone == 0)) {
            ext->SniperState = 0;
            func_150627D4(obj);
            return 0;
        }
        ext->aimYawTarget -= contPad->stick_x * 0.025f;
        ext->aimPitchTarget += contPad->stick_y * 0.025f;
        func_15048758(&ext->aimYawTarget);
        if ((zone == 0x1F) || (zone == 0x29)) {
            ext->aimPitchTarget = (ext->aimPitchTarget < -20.0f) ? -20.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        } else {
            ext->aimPitchTarget = (ext->aimPitchTarget < -80.0f) ? -80.0f
                                : ((ext->aimPitchTarget > 80.0f) ? 80.0f : ext->aimPitchTarget);
        }
        func_15049688(&ext->aimYaw, ext->aimYawTarget, &ext->aimYawVel, 6.0f, 8.0f, dt);
        func_15049688(&ext->aimPitch, ext->aimPitchTarget, &ext->aimPitchVel, 6.0f, 8.0f, dt);
        dir.x = 1.0f;
        rotated = cosf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.y = sinf(ext->aimPitch * -DEG2RAD) * dir.x;
        dir.x = rotated;
        rotated = cosf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.z = sinf(ext->aimYaw * -DEG2RAD) * dir.x;
        dir.x = rotated;
        pos.x = base.x + dir.x;
        pos.y = base.y + dir.y;
        pos.z = base.z + dir.z;
        ext->aimTarget = pos;
        ext->aimEye = base;
        ext->aimDir = dir;
        return 1;
    }
    // matching: no return for other aim states, as in the original
}

/* Leaves the aiming camera: restores slot 1, resets the zoom and clears this player's bit on the target. */
void func_15127EB8(Camera *camera) {
    Actor *player;
    Actor *target;
    u8 linked;

    func_151239CC(camera, CAMERA_MODE_AIM);
    camera->mv->SniperState = 0;
    func_151C9ED4(camera);
    camera->FOVX = 0.0f;
    camera->FOVY = 0.0f;
    camera->TgetFOVX = 0.0f;
    camera->TgetFOVY = 0.0f;
    func_1510B32C(camera->CNumber, 0.0f, 0.0f, 1.0f);
    D_800BE628[camera->CNumber].perspScale = 1.0f;
    func_150627D4(camera->CharPtr);
    D_800DBFF4[camera->CNumber] = 2;

    player = camera->CharPtr;
    linked = player->attach_char;
    // matching: [linked] - 1 rather than [linked - 1] (the latter changes the code)
    target = (linked != 0) ? &movement_obj[linked] - 1 : player;

    target->unk2FC &= ~(1 << camera->CNumber);
    target->unk74 &= ~(1 << camera->CNumber);
    camera->Snap = 1;
}

/* Starts a transition (func_15128030) from the current focus point. */
void func_15127FEC(Camera *camera, s32 arg1, s32 arg2) {
    camera->transition.state = 1;
    camera->transition.startTarget.x = camera->focus.x;
    camera->transition.startTarget.y = camera->focus.y;
    camera->transition.startTarget.z = camera->focus.z;
    func_1512A390(camera);
}

/* Runs the camera transition started by func_15127FEC: eases target, yaw, distance (D_800DC000) and
   height (D_800DC004) from their start values to the follow camera's over `duration` frames.
   Returns 1 while it runs. */
s32 func_15128030(Camera *camera) {
    CamTransition *tr;

    tr = &camera->transition;
    if (camera->Snap != 0) {
        camera->transition.endYaw = 999.0f;
        camera->transition.duration = 0;
        camera->transition.state = 0;
        return 0;
    }
    switch (tr->state) {
    case 0:
        return 0;
    case 1:
        tr->startDistance = sqrtf((camera->eye.x - camera->CharPtr->xpos) * (camera->eye.x - camera->CharPtr->xpos) +
                                  (camera->eye.z - camera->CharPtr->zpos) * (camera->eye.z - camera->CharPtr->zpos));
        camera->YawAngle = 0.0f;
        camera->CentYawAngleVel = 0.0f;
        camera->APitchAngle = 0.0f;
        camera->CentPitchAngleVel = 0.0f;
        camera->transition.LerpHeightStarting = camera->eye.y - camera->unk35C;
        func_15125594(camera);
        tr->startYaw = camera->CameraAngle;
        func_151287E0(camera, &tr->endYawAngle, &tr->endAPitchAngle);
        camera->transition.LerpCurrentFrames = tr->duration;
        tr->progress = 0.0f;
        tr->state = 2;
        // fallthrough
    case 2: {
        s32 done;
        f32 angle;

        done = camera->transition.LerpCurrentFrames == 0;
        if (camera->Snap != 0) {
            done = 1;
            tr->progress = 1.0f;
            camera->transition.LerpCurrentFrames = 0;
        } else {
            tr->progress = 1.0f - (f32)camera->transition.LerpCurrentFrames / (f32)tr->duration;
        }
        func_15123568(camera);
        tr->endDistance = camera->IdealTripodMagnitude;
        camera->transition.LerpHeightTarget = camera->height;
        tr->endTarget.x = camera->CharPtr->xpos;
        func_15124C38(camera, &tr->endTarget.y);
        angle = tr->endYaw;
        tr->endTarget.z = camera->CharPtr->zpos;
        while (angle < 0.0f) {
            angle += 360.0f;
        }
        camera->focus.x = camera->CharPtr->xpos;
        camera->focus.y = camera->CharPtr->ypos;
        camera->focus.z = camera->CharPtr->zpos;
        camera->CharRelAngle = angle;
        while (camera->CharRelAngle < -360.0f) {
            camera->CharRelAngle += 360.0f;
        }
        camera->target.x = tr->startTarget.x + (tr->endTarget.x - tr->startTarget.x) * tr->progress;
        camera->target.y = tr->startTarget.y + (tr->endTarget.y - tr->startTarget.y) * tr->progress;
        camera->target.z = tr->startTarget.z + (tr->endTarget.z - tr->startTarget.z) * tr->progress;
        if (camera->Mode != 0x40) {
            camera->CameraAngle = func_15048720(tr->progress, tr->startYaw, tr->endYaw);
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
        }
        D_800DC000 = tr->startDistance + (tr->endDistance - tr->startDistance) * tr->progress;
        D_800DC004 = camera->transition.LerpHeightStarting +
                     (camera->transition.LerpHeightTarget - camera->transition.LerpHeightStarting) * tr->progress;
        camera->YawAngle = func_15048720(tr->progress, 0.0f, tr->endYawAngle);
        camera->APitchAngle = func_15048720(tr->progress, 0.0f, tr->endAPitchAngle);
        if (done) {
            camera->transition.endYaw = 999.0f;
            camera->transition.duration = 0;
            camera->transition.state = 0;
            camera->AvailableModes |= 8;
            break;
        }
        camera->transition.LerpCurrentFrames -= D_800BE9E4;
        if (camera->transition.LerpCurrentFrames < 0) {
            camera->transition.LerpCurrentFrames = 0;
        }
        return 1;
    }
    }
    // matching: no return after the transition ends (callers see the leftover v0), as in the original
}
/* Runs the handler of the current mode. */
void func_151283B8(Camera *camera) {
    switch (camera->Mode) {
    case 0x80:
        func_15122440(camera);
        break;
    case 0x40:
        func_15129934(camera);
        break;
    case 1:
        func_15122980(camera);
        break;
    case 8:
        func_15121C80(camera, 0.0f);
        break;
    case 2:
        func_151220D0(camera);
        break;
    case 0x400:
        func_15122170(camera);
        break;
    case 0x800:
        func_15122980(camera);
        break;
    case 0x100:
        func_15120158(camera);
        break;
    case 0x800000:
        func_1512DEA4(camera);
        break;
    case 0x2000:
        break;
    }
}

/* End of the camera update: remembers the player position and counts down the snap frames. */
void func_151284C4(Camera *camera) {
    Actor *player;

    func_1512C490(camera);
    player = camera->CharPtr;
    camera->lastPlayerPos.x = player->xpos;
    camera->lastPlayerPos.y = player->ypos;
    camera->lastPlayerPos.z = player->zpos;
    if (camera->Snap != 0) {
        camera->Snap--;
    }
    if (camera->AvailableModes & 8) {
        if (camera->unk5FE <= 0) {
            camera->unk5FE = 60;
            func_15128774(camera, camera->CharPtr);
        }
    }
}
/* Modes that replace the normal follow logic. Returns 1 if one of them ran the whole update. */
s32 func_15128540(Camera *camera) {
    s32 mode;

    if (func_151253CC(camera) != 0) {
        return 1;
    }

    if (func_15128030(camera) != 0) {
        return 0;
    }

    mode = camera->Mode;
    if (mode & 0x80000) {
        func_1512A360(camera);
        camera->target = camera->focus;
        func_151256BC(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x100000) {
        func_1512E4B0(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x200000) {
        func_151219D0(camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (mode & 0x400000) {
        func_1512D380((s32)camera);
        func_151236D0(camera);
        func_151284C4(camera);
        return 1;
    }

    if (func_15126378(camera) != 0) {
        func_151284C4(camera);
        return 1;
    }
    return 0;
}

void func_15128680(Camera *camera) {
}

/* Resets the camera to its defaults and attaches it to player playerIndex. */
void func_1512868C(Camera *camera) {
    Actor *player;
    u8 index;
    s32 i;

    for (i = 0; i < 21; i++) {
        camera->FCache[i] = 0;
    }

    camera->Mode = 1;
    camera->Motion = 0;
    camera->Tightness = 1;
    camera->AvailableModes = 14;
    camera->DistIndex = 3;
    func_15124B18(camera);
    index = camera->CNumber;
    camera->CharPtr = player = &movement_obj[index];
    camera->PadNumber = camera->CharIndex = index;
    camera->mv = player->mv;
    camera->YOffsetTget = 0.0f;
    camera->XZOffsetTget = 0.0f;
    camera->YOffset = 0.0f;
    camera->XZOffset = 0.0f;
    camera->CharTrackingSpeedTarget = 1.0f;
    camera->mv->AimState = 0;
    camera->DoingAim = 0;
}

/* Places the eye at obj (current and previous position) and looks the way the player faces. */
void func_15128774(Camera *camera, Actor *obj) {
    camera->unk35C = obj->floorY;
    camera->prevEye.x = camera->eye.x = obj->xpos;
    camera->prevEye.y = camera->eye.y = obj->ypos;
    camera->prevEye.z = camera->eye.z = obj->zpos;
    camera->CameraAngle = camera->CharPtr->theta - 180.0f;
    camera->mv->unk18C = 999.0f;
    camera->Snap = 1;
}
