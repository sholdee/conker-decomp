#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Runs one frame of the camera's current command; cmd.type selects the behaviour.
void func_1512E8E0(Camera *camera) {
    CamCmd *cmd;
    CamCmdState *cam;

    if (camera->Mode & 0x100) {
        return;
    }
    if (camera->DoingAim != 0) {
        return;
    }
    func_151239CC(camera, CAMERA_MODE_SFLY);
    cmd = &camera->cmd;
    cam = &camera->cmdState;
    if (camera->Mode & 0x40000) {
        return;
    }
    if (cam->cmdId != cmd->id) {
        camera->DoingRail = 0;
        cam->cmdId = cmd->id;
        cam->unk6 = 0;
        func_151239CC(camera, CAMERA_MODE_FIXED);
        func_151239CC(camera, CAMERA_MODE_SFLY);
        func_151239CC(camera, CAMERA_MODE_ZONES);
        func_151239CC(camera, CAMERA_MODE_ZONE_FLAGS);
    }
    if ((cmd->type != 4) && (camera->mv->death_State != 0) && (camera->DoingRail == 0)) {
        cam->unk6 = 1;
    }
    if (cam->unk6 != 0) {
        func_151239CC(camera, CAMERA_MODE_FIXED);
    }
    if ((camera->cmdState.unk0 != 0) && (cam->unk6 == 0) && (cam->blending == 0)) {
        cam->lastType = cmd->type;
        if (cmd->type != 10) {
            func_1512FE00(camera, cmd->unk8);
        }
        if (cmd->type == 4) {
            camera->FCameraIndex = -1;
            camera->cmdRunning = 1;
            camera->savedDistIndex[7] = camera->DistIndex;
            camera->savedDistMask[7] = camera->DistMask;
            return;
        }
        if (cmd->type == 7) {
            f32 dx;
            s32 i;
            f32 minDist;
            Vec3f toCamera;
            Vec3f dir;
            Vec3f offset;
            Vec3f nodePos;
            Vec3f linkPos;
            Vec3f best;
            Vec3f closest;
            f32 dy;
            PathNode *node;
            f32 dz;

            // Find the closest point to the camera on the path segments leaving its nearest node.
            node = &D_800D2350[func_15085DF8(camera->eye.x, camera->eye.y, camera->eye.z, 7, 0)];
            minDist = 1e14f;
            if (camera->Mode & 0x100) {
                return;
            }
            nodePos.x = node->x;
            nodePos.y = node->y;
            nodePos.z = node->z;
            func_15048F90(&nodePos, &camera->eye, &toCamera);
            for (i = 0; i < 5; i++) {
                // matching: s32 link read back through (u8) adds a live range uopt needs for the frame layout
                s32 link;

                link = node->links[i];
                if ((u8)link != 0xFF) {
                    linkPos.x = D_800D2350[(u8)link].x;
                    linkPos.y = D_800D2350[(u8)link].y;
                    linkPos.z = D_800D2350[(u8)link].z;
                    func_150491EC(&nodePos, &linkPos, &dir);
                    func_15049148(&dir, func_150AD900(&dir.x, &toCamera.x), &offset);
                    func_15048F20(&nodePos, &offset, &closest);
                    dx = camera->eye.x - closest.x;
                    dy = camera->eye.y - closest.y;
                    dz = camera->eye.z - closest.z;
                    // matching: dy/dz stay dead and the y/z terms are spelled out; the extra CSE temps size the frame
                    if (((dx * dx) + ((camera->eye.y - closest.y) * (camera->eye.y - closest.y)) + ((camera->eye.z - closest.z) * (camera->eye.z - closest.z))) < minDist) {
                        minDist = (dx * dx) + ((camera->eye.y - closest.y) * (camera->eye.y - closest.y)) + ((camera->eye.z - closest.z) * (camera->eye.z - closest.z));
                        best = closest;
                    }
                }
            }
            if (func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_FIXED) != 0) {
                camera->cmdRunning = 1;
                camera->AvailableModes &= ~0x80000006;
                camera->RailFlags = cmd->unk8;
                camera->RailMagXZ = cmd->unk1C;
                camera->RailMagY = cmd->unk20;
                if (camera->RailFlags & 1) {
                    camera->AvailableModes |= 4;
                }
                if (camera->RailFlags & 2) {
                    camera->AvailableModes |= 2;
                }
                if (camera->RailFlags & 7) {
                    camera->AvailableModes |= 0x20000;
                }
                camera->DoingRail = 1;
            }
            if (camera->RailFlags & 0x20) {
                camera->AvailableModes |= 8;
            } else {
                camera->AvailableModes &= ~8;
            }
            camera->RailAccelATarget = 1.5f;
            camera->RailAccelBTarget = 2.5f;
            if ((camera->RailFlags & 2) && (camera->PadPtr->button & 7)) {
                camera->AvailableModes |= 8;
                camera->RailAccelATarget *= 0.25f;
                camera->RailAccelBTarget *= 0.25f;
            } else if (camera->RailFlags & 0x40) {
                if (camera->CharPtr->speed > 30.0f) {
                    camera->RailAccelATarget *= 7.0f;
                    camera->RailAccelBTarget *= 7.0f;
                } else {
                    camera->RailAccelATarget *= 4.0f;
                    camera->RailAccelBTarget *= 4.0f;
                }
                camera->AvailableModes |= 0x20000;
            }
            if (camera->Snap != 0) {
                camera->RailAccelAVel = 0.0f;
                camera->RailAccelA = camera->RailAccelATarget;
                camera->RailAccelBVel = 0.0f;
                camera->RailAccelB = camera->RailAccelBTarget;
            } else {
                func_150495B0(&camera->RailAccelA, camera->RailAccelATarget, &camera->RailAccelAVel, 1.5f, 2.0f, camera->SpringTime);
                func_150495B0(&camera->RailAccelB, camera->RailAccelBTarget, &camera->RailAccelBVel, 1.5f, 2.0f, camera->SpringTime);
            }
            if (!(camera->RailFlags & 0x10)) {
                if (camera->Snap != 0) {
                    camera->RailWantedXVel = 0.0f;
                    camera->RailWantedZVel = 0.0f;
                    camera->eye.x = best.x;
                    camera->eye.z = best.z;
                } else {
                    func_150495B0(&camera->eye.x, best.x, &camera->RailWantedXVel, camera->RailAccelA, camera->RailAccelB, camera->SpringTime);
                    func_150495B0(&camera->eye.z, best.z, &camera->RailWantedZVel, camera->RailAccelA, camera->RailAccelB, camera->SpringTime);
                }
            }
            if (!(camera->RailFlags & 8)) {
                if (camera->Snap != 0) {
                    camera->eye.y = best.y;
                    camera->RailWantedYVel = 0.0f;
                } else {
                    func_150495B0(&camera->eye.y, best.y, &camera->RailWantedYVel, camera->RailAccelA, camera->RailAccelB, camera->SpringTime);
                }
            }
            func_15125594(camera);
            camera->RailMagXZ = cmd->unk1C;
            camera->RailMagY = cmd->unk20;
            camera->IdealTripodMagnitude = camera->RailMagXZ;
            camera->height = camera->RailMagY;
            if (camera->RailFlags & 4) {
                camera->LastMagsZone = cmd->id | 0x4000;
                camera->LastMagsZoneConsider = 1;
            }
            return;
        }
        if (cmd->type == 5) {
            if (func_15123934(camera, 0x40, 0, 0, CAMERA_MODE_FIXED) != 0) {
                camera->AvailableModes &= ~0xE;
                camera->AvailableModes |= 0x200;
                camera->unk27C = camera->eye;
            }
            if ((cmd->unk8 == 5) || (cmd->unk8 == 0x11)) {
                if (camera->FCameraIndex != cmd->unk1C) {
                    camera->FCameraIndex = cmd->unk1C;
                    D_800DC024 = D_800DC020;
                    D_800DC024 = &D_800DC024[camera->FCameraIndex];
                    camera->FTgetRadius = cmd->unk20;
                    camera->unk288.x = D_800DC024->x;
                    camera->unk288.y = D_800DC024->y;
                    camera->unk288.z = D_800DC024->z;
                    camera->SpecialFlags |= 4;
                    camera->eye.x = camera->unk288.x;
                    camera->unk27C.x = camera->unk288.x;
                    camera->eye.y = camera->unk288.y;
                    camera->unk27C.y = camera->unk288.y;
                    camera->eye.z = camera->unk288.z;
                    camera->unk27C.z = camera->unk288.z;
                    func_15128774(camera, camera->CharPtr);
                    camera->Snap = 1;
                }
            } else {
                camera->FCameraIndex = cmd->unk1C;
                D_800DC024 = D_800DC020;
                D_800DC024 = &D_800DC024[camera->FCameraIndex];
                camera->unk288.x = D_800DC024->x;
                camera->unk288.y = D_800DC024->y;
                camera->unk288.z = D_800DC024->z;
                camera->FTgetRadius = cmd->unk20;
            }
            camera->FToFromStatus = cmd->unk8;
            camera->cmdRunning = 1;
            return;
        }
        if (cmd->type == 11) {
            cam->minHeight = cmd->unk1C;
            cam->maxHeight = cmd->unk20;
            cam->maxDistance = cmd->unk8;
            return;
        }
        if ((cmd->type == 10) || (cmd->type == 14)) {
            Actor *obj;

            // matching: reusing obj for the id (instead of nesting the calls) drops a cfe temp
            obj = (Actor *)func_15083E0C(cmd->unk1C);
            obj = actor_findOnPath((s32)obj);
            if (obj != NULL) {
                f32 dist;

                if (func_15123934(camera, 0x400, 0, camera->Tightness, CAMERA_MODE_ZONES) != 0) {
                    if (cmd->type == 14) {
                        func_151254F4(camera, obj - movement_obj);
                    }
                    camera->AvailableModes &= ~8;
                    camera->unk628 = 0;
                    camera->PivotPtr = &camera->pivotPos;
                    camera->cmdRunning = 1;
                    camera->unk62C = 0.0f;
                    camera->PanButtonAddOn = 0.0f;
                    dist = sqrtf(((camera->eye.x - obj->xpos) * (camera->eye.x - obj->xpos)) + ((camera->eye.z - obj->zpos) * (camera->eye.z - obj->zpos)));
                    camera->pivotPos.x = -sinf(camera->CameraAngle * 0.01745329238f) * dist + camera->focus.x;
                    camera->pivotPos.y = camera->focus.y;
                    camera->pivotPos.z = -cosf(camera->CameraAngle * 0.01745329238f) * dist + camera->focus.z;
                    camera->LockedCameraAngle = camera->CameraAngle;
                }
                if (cmd->type == 14) {
                    camera->pivotPos.x = movement_obj[camera->CNumber].xpos;
                    camera->pivotPos.y = movement_obj[camera->CNumber].ypos;
                    camera->pivotPos.z = movement_obj[camera->CNumber].zpos;
                } else if (camera->Snap != 0) {
                    camera->pivotPos.x = obj->xpos;
                    camera->pivotPos.y = obj->ypos;
                    camera->pivotPos.z = obj->zpos;
                } else {
                    camera->pivotPos.x += (obj->xpos - camera->pivotPos.x) * 0.03999999911f;
                    camera->pivotPos.y += (obj->ypos - camera->pivotPos.y) * 0.03999999911f;
                    camera->pivotPos.z += (obj->zpos - camera->pivotPos.z) * 0.03999999911f;
                }
                // matching: two separate tests; merging them (or dropping h) changes scheduling
                if (cmd->unk8 != 0) {
                    camera->IdealTripodMagnitude = cmd->unk20;
                }
                if (cmd->unk8 != 0) {
                    f32 h;

                    h = cmd->unk8 - (camera->focus.y - camera->pivotPos.y);
                    camera->unk34C = h;
                    cmd->unk34 = cmd->unk8;
                }
            }
            return;
        }
        if (cmd->type == 15) {
            if (func_15123934(camera, 0x800000, 0, camera->Tightness, CAMERA_MODE_ZONES) != 0) {
                camera->Tightness = 0;
                camera->AvailableModes |= 0x200;
                camera->AvailableModes &= ~8;
                func_1512DD50(camera);
            }
            return;
        }
        if (cmd->type == 3) {
            if (func_15123934(camera, 0x400, 0, camera->Tightness, CAMERA_MODE_ZONES) != 0) {
                camera->unk628 = 0;
                camera->AvailableModes |= 8;
                camera->PivotPtr = &camera->pivotPos;
                camera->unk62C = 0.0f;
                camera->PanButtonAddOn = 0.0f;
                camera->IdealTripodMagnitude = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
                camera->cmdRunning = 1;
            }
            camera->pivotPos.x = cmd->x;
            camera->pivotPos.y = cmd->y;
            camera->pivotPos.z = cmd->z;
            camera->IdealTripodMagnitude = cmd->unk1C;
            return;
        }
        if (cmd->type == 2) {
            f32 shortfall;

            if (func_15123934(camera, 0x2000, 0, camera->Tightness, CAMERA_MODE_ZONES) != 0) {
                camera->IdealTripodMagnitude = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
            }
            camera->cmdRunning = 1;
            func_1512E140(camera);
            camera->TripodMagnitude = sqrtf(((camera->eye.x - camera->focus.x) * (camera->eye.x - camera->focus.x)) + ((camera->eye.z - camera->focus.z) * (camera->eye.z - camera->focus.z)));
            if (camera->TripodMagnitude < camera->IdealTripodMagnitude) {
                shortfall = camera->IdealTripodMagnitude - camera->TripodMagnitude;
                camera->eye.x += shortfall * cam->pushRate * func_150AD78C(camera->CameraAngleRad);
                camera->eye.z += shortfall * cam->pushRate * func_150AD780(camera->CameraAngleRad);
                cam->pushRate += 0.004999999888f;
                cam->pushRate = (cam->pushRate > 0.03999999911f) ? 0.03999999911f : cam->pushRate;
            }
            func_151287E0(camera, NULL, NULL);
            return;
        }
        if ((cmd->type == 1) || (cmd->type == 0)) {
            f32 dist;
            f32 shortfall;

            dist = sqrtf(((camera->eye.x - cmd->x) * (camera->eye.x - cmd->x)) + ((camera->eye.z - cmd->z) * (camera->eye.z - cmd->z)));
            if ((camera->cmdRunning == 0) || (cmd->type == 0)) {
                if (func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONES) != 0) {
                    camera->cmdRunning = 1;
                    camera->IdealTripodMagnitude = cmd->unk1C;
                    camera->unk34C = cmd->unk20;
                    cmd->unk34 = cmd->unk20;
                    camera->AvailableModes &= ~4;
                    if (cmd->type == 0) {
                        camera->LastMagsZone = cmd->id | 0x4000;
                        camera->LastMagsZoneConsider = 1;
                    }
                }
            }
            camera->IdealTripodMagnitude = cmd->unk1C;
            camera->unk34C = camera->height = cmd->unk20;
            cmd->unk34 = cmd->unk20;
            if ((dist < camera->IdealTripodMagnitude) && (cmd->type == 1)) {
                shortfall = camera->IdealTripodMagnitude - dist;
                camera->eye.x += shortfall * cam->pushRate * func_150AD78C(camera->CameraAngleRad);
                camera->eye.z += shortfall * cam->pushRate * func_150AD780(camera->CameraAngleRad);
                cam->pushRate += 0.002499999944f;
                cam->pushRate = (cam->pushRate > 0.03999999911f) ? 0.03999999911f : cam->pushRate;
            }
            if (cmd->type == 1) {
                cam->unk18 += (cmd->x - cam->unk18) * 0.03999999911f;
                cam->unk20 += (cmd->z - cam->unk20) * 0.03999999911f;
                camera->focus.x += (cam->unk18 - camera->focus.x) * 0.5f;
                camera->focus.z += (cam->unk20 - camera->focus.z) * 0.5f;
            }
            if (cmd->type != 0) {
                func_1512E140(camera);
                func_15124C38(camera, 0);
                cam->blend = 0.0f;
            }
            return;
        }
        if (cmd->type == 13) {
            func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONES);
            return;
        }
    }
    cam->minHeight = 0.0f;
    cam->maxHeight = 0.0f;
    cam->maxDistance = 0.0f;
    if (camera->cmdState.unk0 == 0) {
        cam->unk6 = 0;
    }
    if ((camera->cmdRunning == 1) && (cam->lastType == 1)) {
        cam->unk18 = camera->focus.x + (cam->unk18 - camera->focus.x) * 0.5f;
        cam->unk20 = camera->focus.z + (cam->unk20 - camera->focus.z) * 0.5f;
        cam->pushRate = 0.0f;
    }
    if (camera->Mode == 0x40) {
        camera->FCameraIndex = -1;
        func_151239CC(camera, CAMERA_MODE_FIXED);
    }
    if (camera->cmdRunning == 1) {
        if (cam->lastType == 0) {
            cam->unk18 = camera->focus.x;
            cam->unk20 = camera->focus.z;
        }
        if (cam->lastType == 14) {
            camera->CharTrackingSpeed = 0.009999999776f;
            func_151254F4(camera, camera->CNumber);
        }
    }
    if (camera->Snap != 0) {
        cam->unk18 = camera->focus.x;
        cam->unk20 = camera->focus.z;
        cam->blend = 1.0f;
    } else {
        cam->unk18 += (camera->focus.x - cam->unk18) * cam->blend;
        cam->unk20 += (camera->focus.z - cam->unk20) * cam->blend;
    }
    camera->focus.x = cam->unk18;
    camera->focus.z = cam->unk20;
    cam->blend += 0.01499999966f;
    cam->blend = (cam->blend > 1.0f) ? 1.0f : cam->blend;
    if ((cam->blend != 1.0f) && (cam->lastType == 1)) {
        cam->blending = 1;
    } else {
        cam->blending = 0;
    }
    if (camera->cmdRunning == 1) {
        camera->FCameraIndex = -1;
        camera->PivotPtr = NULL;
        camera->cmdRunning = 0;
        cmd->unk34 = 0;
        cmd->type = 13;
        camera->RailWantedX = 0.0f;
        camera->RailWantedXVel = 0.0f;
        camera->RailWantedY = 0.0f;
        camera->RailWantedYVel = 0.0f;
        camera->RailWantedZ = 0.0f;
        camera->RailWantedZVel = 0.0f;
        camera->RailAccelATarget = 0.0f;
        camera->RailAccelA = 0.0f;
        camera->RailAccelAVel = 0.0f;
        camera->RailAccelBTarget = 0.0f;
        camera->RailAccelB = 0.0f;
        camera->RailAccelBVel = 0.0f;
        camera->RailFlags = 0;
        camera->DoingRail = 0;
        camera->unk948 = 0;
        camera->LockedCameraAngle = camera->CameraAngle;
        // matching: double 0.0 literals; 0.0f changes the generated code
        camera->RailMagXZ = 0.0;
        camera->RailMagY = 0.0;
        func_151239CC(camera, CAMERA_MODE_AIM);
        func_151239CC(camera, CAMERA_MODE_FIXED);
        func_151239CC(camera, CAMERA_MODE_SFLY);
        func_151239CC(camera, CAMERA_MODE_ZONES);
        func_151239CC(camera, CAMERA_MODE_ZONE_FLAGS);
    }
}

// Starts the camera mode for a command's action (cmd.unk8) and sets its flags.
void func_1512FE00(Camera *camera, s32 action) {
    switch (action) {
    case 1:
        func_15123934(camera, camera->Mode, 0, 0, CAMERA_MODE_ZONE_FLAGS);
        break;
    case 2:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes &= ~8;
        break;
    case 3:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x10000000;
        break;
    case 8:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x80;
        break;
    case 9:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes &= ~8;
        camera->AvailableModes |= 0x80;
        break;
    case 11:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x4000;
        break;
    case 12:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x8000;
        break;
    case 13:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x20000;
        break;
    case 16:
        func_15123934(camera, 8, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        break;
    case 14:
        func_15123934(camera, 2, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        break;
    case 18:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x100000;
        break;
    case 19:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x400000;
        break;
    case 21:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->YOffsetTget = 0.0f;
        break;
    case 22:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x800;
        camera->YOffsetTget = -30.0f;
        break;
    case 25:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x40;
        break;
    case 23:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x1000000;
        break;
    case 24:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x80000000;
        break;
    case 26:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x10;
        break;
    case 27:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x20;
        break;
    case 28:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x10;
        camera->AvailableModes &= ~8;
        break;
    case 29:
        func_15123934(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_ZONE_FLAGS);
        camera->AvailableModes |= 0x1000010;
        camera->AvailableModes &= ~8;
        break;
    case 30:
        func_15128774(camera, camera->CharPtr);
        break;
    }
}
