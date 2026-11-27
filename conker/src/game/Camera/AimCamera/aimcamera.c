#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/sight.h"
#include "conker/text.h"

/*
 * Aim camera: the camera mode that swings behind the player (camera_enterAim enters it, camera_updateAim
 * runs it each frame, camera_leaveAim leaves it). Camera.DoingAim is the state machine:
 *   1 = swinging round to the aim view, 2 = aiming with the stick, 3 = swinging back, 0 = off.
 */

#define DEG2RAD 0.01745329238f
#define BAM2DEG 0.005493164f /* 360 / 65536: 16-bit binary angle to degrees */

/* Zones whose aim yaw follows the player's facing angle; sight_startPlayerEffects runs while in one of them. */
#define IS_FACING_ZONE(z)                                                                                  \
    (((z) == 9) || ((z) == 0x38) || ((z) == 0x39) || ((z) == 0x37) || ((z) == 0x3B) || ((z) == 0x15) || \
     ((z) == 0x26) || ((z) == 0x3A))

/* Enters aim mode: saves the current view and picks the aim yaw, pitch and distance for the zone. */
void camera_enterAim(Camera *camera) {
    CamAim *aim;
    s32 zone;

    aim = &camera->aim;
    zone = camera->CharContext;
    camera->AimTimeOuts = 60;
    if ((camera->SpecialFlags & 2) || (camera->CharContext == 9) || (camera->CharContext == 0x38) || (camera->CharContext == 0x39) ||
        (camera->CharContext == 0x37) || (camera->CharContext == 0x3B)) {
        // matching: the original stores 0 then 1; IDO drops the dead store unless it is volatile
        *(volatile u8 *)&aim->unk4D = 0;
        aim->unk4D = 1;
    } else {
        aim->unk4D = 1;
    }
    camera->AimTargetOn = FALSE;
    if (camera->MCache[CAMERA_MODE_AIM] == 0x40) {
        aim->targetYaw = camera->CameraAngle;
    } else if (aim->unk4D) {
        if (IS_FACING_ZONE(camera->CharContext)) {
            aim->targetYaw = ((camera->CharPtr->langle - camera->mv->pssd_waist_tilt) + 0xC000) * BAM2DEG;
        } else if ((gContextTextId == 0x42) && (zone == 0x1A)) {
            aim->targetYaw = camera->CharPtr->theta;
        } else {
            aim->targetYaw = camera->CharPtr->theta - 180.0f;
        }
    } else {
        aim->targetYaw = camera->CameraAngle;
        camera->AvailableModes &= ~0x280;
    }
    if (IS_FACING_ZONE(zone)) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
        sight_startPlayerEffects(camera, zone);
        camera->AimTargetOn = TRUE;
    } else if ((zone == 2) || (zone == 0x13)) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    } else if (zone == 0x1C) { /* keeps the current pitch */
    } else if (zone == 0x34) {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    } else if (zone == 0xF) {
        aim->targetPitch = 27.0f;
        aim->pitch = 27.0f;
        aim->targetYaw = camera->CharPtr->theta + 225.0f;
    } else {
        aim->targetPitch = 0.0f;
        aim->pitch = 0.0f;
    }
    camera_setAimDistance(camera);
    math_normalizeDegrees(&aim->targetYaw);
    if (aim->useOverride) {
        aim->targetPitch = aim->overridePitch;
        aim->pitch = aim->overridePitch;
        aim->distance = aim->overrideDistance;
        aim->targetYaw += aim->overrideYawOffset;
    }
    aim->savedYaw = camera->CameraAngle;
    aim->savedTightness = camera->Tightness;
    if (camera->DoingRail) {
        aim->savedTargetDistance = sqrtf((camera->target.x - camera->eye.x) * (camera->target.x - camera->eye.x) +
                                          (camera->target.z - camera->eye.z) * (camera->target.z - camera->eye.z));
        aim->savedHeight = camera->unk344;
        aim->savedUnk354 = camera->unk354;
    } else {
        aim->savedTargetDistance = camera->IdealTripodMagnitude;
        aim->savedHeight = camera->height;
        aim->savedUnk354 = camera->unk354;
    }
    camera->floorY = camera->target.y;
    camera->unk354 = camera->target.y;
    aim->distance = 110.0f;
    camera->height = 30.0f;
    camera->mv->aimYaw = aim->targetYaw;
    if ((zone == 0) || (zone == 0xC) || IS_FACING_ZONE(zone)) {
        camera->height += 30.0f;
    }
    aim->stickLock = 0;
    camera->DoingAim = 1;
    if (camera->Snap) {
        aim->targetYaw = camera->CharPtr->theta - 180.0f;
        camera->CameraAngle = aim->targetYaw;
        camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
        camera->XZOffset = camera->XZOffsetTget;
        camera->YOffset = camera->YOffsetTget;
    }
    if (aim->distance < camera->TripodMagnitude) {
        aim->turnDistance = aim->distance * 1.2f;
        aim->approachDir = -1;
    } else {
        aim->turnDistance = aim->distance * 0.6666659713f;
        aim->approachDir = 1;
    }
    if ((zone == 0xA) || (zone == 2) || (zone == 0x12) || (zone == 3) || (zone == 0x34)) {
        aim->turnDistance = camera->IdealTripodMagnitude;
        aim->approachDir = -1;
    }
    camera->AvailableModes &= ~0x280;
    camera->IdealTripodMagnitude = aim->distance;
}

/* Per-frame update of the aim camera (see the state machine at the top of the file). */
void camera_updateAim(Camera *camera) {
    CamAim *aim;
    f32 probeY;
    f32 dist;
    s32 zone;
    s32 inMap42Zone1A;
    f32 yawDelta;

    aim = &camera->aim;
    zone = camera->CharContext;
    inMap42Zone1A = (gContextTextId == 0x42) && (zone == 0x1A);
    collision_findFloorInRange(0, 0, &probeY, &camera->waterY, 0, 0,
                 camera->eye.x, camera->eye.y, camera->eye.z, camera->eye.y, 0, 0, -10000.0f, camera->eye.y);
    camera->mv->AimState = camera->DoingAim;
    camera->TripodMagnitude = sqrtf((camera->eye.x - camera->target.x) * (camera->eye.x - camera->target.x) +
                             (camera->eye.z - camera->target.z) * (camera->eye.z - camera->target.z));
    if (camera->Snap) {
        aim->savedUnk354 = probeY;
    }
    if ((zone != 0x2A) && (camera->DoingAim != 3)) {
        if (zone == 0xD) {
            camera->YOffsetTget = 100.0f;
        } else if (zone == 3) {
            aim->targetPitch = 15.0f;
            camera->XZOffsetTget = 0.0f;
            aim->distance = 350.0f;
            if (camera->blocked) {
                aim->pitch *= camera->blockedDistance * 0.002857142827f;
                aim->pitch = (aim->pitch < 0.0f) ? 0.0f : aim->pitch;
            }
        } else if (inMap42Zone1A) {
            camera->YOffsetTget = 18.0f;
            camera->XZOffsetTget = 60.0f;
            aim->distance = 250.0f;
            aim->targetYaw = camera->CharPtr->theta;
        } else if (zone == 0x1A) {
            camera->YOffsetTget = 35.0f;
            camera->XZOffsetTget = 100.0f;
            aim->distance = 250.0f;
        } else {
            camera->YOffsetTget = 20.0f;
            camera->XZOffsetTget = 0.0f;
            camera_setAimDistance(camera);
        }
    }
    camera->floorY = camera->unk354 = camera->target.y;
    actor_fadeCommand(camera->CharPtr, 2, 0xFF, 0x20, camera->CNumber);
    dist = aim->distance;
    if (camera->DoingAim == 1) {
        s32 horizontal;
        f32 sinPitch;
        s32 passed;

        if (camera->AimTimeOuts != 0) {
            camera->AimTimeOuts -= gFrameTicks;
            if (camera->AimTimeOuts <= 0) {
                camera->mv->AimState = 1;
                camera->DoingAim = 2;
                aim->targetYaw = camera->CameraAngle;
                return;
            }
        }
        camera->Tightness = 1;
        horizontal = cosf(aim->pitch * DEG2RAD) * dist;
        sinPitch = sinf(aim->pitch * DEG2RAD);
        camera->IdealTripodMagnitude = horizontal;
        camera->height = (s32)(sinPitch * dist);
        if (camera->Snap) {
            if (inMap42Zone1A) {
                aim->targetYaw = camera->CharPtr->theta;
            } else {
                aim->targetYaw = camera->CharPtr->theta - 180.0f;
            }
            camera->CameraAngle = aim->targetYaw;
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
            camera->XZOffset = camera->XZOffsetTget;
            camera->YOffset = camera->YOffsetTget;
        }
        // matching: a goto to the end of this branch makes IDO duplicate the yawRad store; break does not
        do {
            if (aim->unk4D) {
                // matching: the original compares the yaw error and does nothing with the result
                if (fabsf(math_angleDiff(camera->CameraAngle, aim->targetYaw)) < 0.009999999776f) {
                }
            } else {
                camera->mv->AimState = 1;
                camera->DoingAim = 2;
                break;
            }
            if (aim->approachDir == -1) {
                passed = camera->TripodMagnitude < aim->turnDistance;
            } else {
                passed = aim->turnDistance < camera->TripodMagnitude;
            }
            // matching: the redundant & 0xFF uses up one ugen temp, which fixes the t-register rotation
            if (passed || (aim->useOverride & 0xFF)) {
                if (camera->blocked) {
                    math_springAngleTo(&camera->CameraAngle, aim->targetYaw, &aim->yawVel, 10.0f, 18.0f, camera->SpringTime);
                } else {
                    math_springAngleTo(&camera->CameraAngle, aim->targetYaw, &aim->yawVel, 5.0f, 9.0f, camera->SpringTime);
                }
                math_springTo(&camera->XZOffset, camera->XZOffsetTget, &camera->OffsetXZVel, 5.0f, 9.0f, camera->SpringTime);
                math_springTo(&camera->YOffset, camera->YOffsetTget, &camera->OffsetYVel, 5.0f, 9.0f, camera->SpringTime);
                camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
            }
        } while (0);
    } else if (camera->DoingAim == 2) {
        f32 horizontal;
        f32 sinPitch;
        f32 rate;
        s32 notUnk3E8;
        s32 stickX;
        OSContPad *pad;

        if (zone) {
            rate = 0.01279999968f;
        } else {
            rate = 0.02500000037f;
        }
        if (camera->Snap) {
            if (inMap42Zone1A) {
                aim->targetYaw = camera->CharPtr->theta;
            } else {
                aim->targetYaw = camera->CharPtr->theta - 180.0f;
            }
            camera->CameraAngle = aim->targetYaw;
            camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
            camera->XZOffset = camera->XZOffsetTget;
            camera->YOffset = camera->YOffsetTget;
        }
        if (IS_FACING_ZONE(zone)) {
            if (!camera->AimTargetOn) {
                sight_startPlayerEffects(camera, zone);
                camera->AimTargetOn = TRUE;
            }
        } else if (camera->AimTargetOn) {
            sight_resetPlayerEffects(camera);
            camera->AimTargetOn = FALSE;
        }
        if (zone != 0xD) {
            // matching: the negated copy in a local fixes the register allocation of this block
            notUnk3E8 = !camera->TonyHitsClear;
            if (!aim->unk4D) {
                if (!notUnk3E8) {
                    aim->stickLock = 0;
                } else {
                    if (aim->stickLock == 0) {
                        stickX = camera->PadPtr->stick_x;
                        aim->stickLock |= (stickX > 0) ? 1 : ((stickX < 0) ? 2 : 0);
                    }
                    camera->eye = camera->prevEye;
                    if (aim->stickLock & 1) {
                        pad = camera->PadPtr;
                        pad->stick_x = (pad->stick_x > 0) ? 0 : pad->stick_x;
                    } else if (aim->stickLock & 2) {
                        pad = camera->PadPtr;
                        pad->stick_x = (pad->stick_x < 0) ? 0 : pad->stick_x;
                    }
                }
            }
            if (IS_FACING_ZONE(zone)) {
                aim->targetYaw = ((camera->CharPtr->langle - camera->mv->pssd_waist_tilt) + 0xC000) * BAM2DEG;
            } else if (inMap42Zone1A) {
                aim->targetYaw = camera->CharPtr->theta;
            } else if (zone != 0xF) {
                if (camera->SpecialFlags & 2) {
                    aim->targetYaw = ((camera->CharPtr->langle - camera->mv->pssd_waist_tilt * 0.349999994f) + 49152.0f) * BAM2DEG;
                } else {
                    if (zone == 0x34) {
                        aim->targetYaw -= camera->PadPtr->stick_x * rate * gFrameTicks;
                    }
                    aim->targetYaw -= camera->PadPtr->stick_x * rate * gFrameTicks;
                }
            }
            if ((zone != 3) && (zone != 0x1A) && !inMap42Zone1A && (zone != 0xF)) {
                aim->targetPitch += camera->PadPtr->stick_y * rate * gFrameTicks;
            }
            if (zone == 0x38) {
                // matching: sinPitch doubles as the pitch adjustment; a separate local changes the frame
                if (camera->PadPtr->button & Z_TRIG) {
                    sinPitch = camera->LastGunRecoil - (camera->CharPtr->mv->gunRecoil >> 8) * 1.098632812f;
                } else {
                    sinPitch = 0.0f;
                }
                aim->targetPitch += sinPitch;
                camera->LastGunRecoil = (camera->CharPtr->mv->gunRecoil >> 8) * 1.098632812f;
            }
            math_normalizeDegrees(&aim->targetYaw);
            if (zone == 0x34) {
                aim->targetPitch = (aim->targetPitch < -5.0f) ? -5.0f : ((aim->targetPitch > 15.0f) ? 15.0f : aim->targetPitch);
            } else if (zone == 0x12) {
                aim->targetPitch = (aim->targetPitch < -40.0f) ? -40.0f : ((aim->targetPitch > 15.0f) ? 15.0f : aim->targetPitch);
            } else if ((zone == 2) || (zone == 0xA) || (zone == 0x13)) {
                aim->targetPitch = (aim->targetPitch < -37.0f) ? -37.0f : ((aim->targetPitch > 30.0f) ? 30.0f : aim->targetPitch);
            } else {
                aim->targetPitch = (aim->targetPitch < -45.0f) ? -45.0f : ((aim->targetPitch > 80.0f) ? 80.0f : aim->targetPitch);
            }
            if ((gIsMultiplayer == 0) && IS_FACING_ZONE(zone)) {
                aim->targetPitch = (aim->targetPitch < -20.0f) ? -20.0f : aim->targetPitch;
            }
            if (camera->Snap) {
                camera->CameraAngle = aim->targetYaw;
                aim->pitch = aim->targetPitch;
                camera->XZOffset = camera->XZOffsetTget;
                camera->YOffset = camera->YOffsetTget;
            } else if (zone || inMap42Zone1A) {
                math_springAngleTo(&camera->CameraAngle, aim->targetYaw, &aim->yawVel, 3.91f, 8.210000038f, camera->SpringTime);
                math_springAngleTo(&aim->pitch, aim->targetPitch, &aim->pitchVel, 3.91f, 8.210000038f, camera->SpringTime);
                math_springTo(&camera->XZOffset, camera->XZOffsetTget, &camera->OffsetXZVel, 3.91f, 8.210000038f, camera->SpringTime);
                math_springTo(&camera->YOffset, camera->YOffsetTget, &camera->OffsetYVel, 3.91f, 8.210000038f, camera->SpringTime);
            } else {
                math_springAngleTo(&camera->CameraAngle, aim->targetYaw, &aim->yawVel, 4.3f, 7.300000191f, camera->SpringTime);
                math_springAngleTo(&aim->pitch, aim->targetPitch, &aim->pitchVel, 4.3f, 7.300000191f, camera->SpringTime);
            }
        }
        camera->mv->aimYaw = camera->CameraAngle;
        camera->mv->aimPitch = aim->pitch;
        camera->mv->aimTarget = camera->target;
        camera->mv->aimEye = camera->eye;
        while (camera->mv->aimPitch > 180.0f) {
            camera->mv->aimPitch -= 360.0f;
        }
        while (camera->mv->aimPitch < -180.0f) {
            camera->mv->aimPitch += 360.0f;
        }
        horizontal = cosf(aim->pitch * DEG2RAD) * dist;
        sinPitch = sinf(aim->pitch * DEG2RAD);
        camera->IdealTripodMagnitude = horizontal;
        camera->height = sinPitch * dist;
        aim->orbitPos.x = sinf(camera->CameraAngle * DEG2RAD + 3.141592741f) * dist + camera->target.x;
        aim->orbitPos.y = sinf(aim->pitch * DEG2RAD + 3.141592741f) * dist + camera->target.y;
        aim->orbitPos.z = cosf(camera->CameraAngle * DEG2RAD + 3.141592741f) * dist + camera->target.z;
        if (aim->pitch >= 180.0f) {
            aim->signedPitch = aim->pitch - 360.0f;
        } else {
            aim->signedPitch = aim->pitch;
        }
        camera_leaveAim(camera);
    } else if (camera->DoingAim == 3) {
        yawDelta = fabsf(math_angleDiff(camera->CameraAngle, aim->savedYaw));
        camera->Tightness = camera->savedTightness[1];
        if (camera->AimTimeOuts != 0) {
            camera->AimTimeOuts -= gFrameTicks;
            if (camera->AimTimeOuts <= 0) {
                camera->mv->AimState = 0;
                camera->DoingAim = 0;
                return;
            }
        }
        if ((camera->AvailableModes & 0x200000) || IS_FACING_ZONE(camera->CharContext)) {
            aim->unk4D = 0;
        }
        if (!aim->unk4D || (yawDelta < 45.0f)) {
            camera->IdealTripodMagnitude = aim->savedTargetDistance;
            camera->height = aim->savedHeight;
            if ((camera->SpecialFlags & 0x40) || (camera->waterY != 0.0f)) {
                camera->floorY = aim->savedUnk354;
                camera->unk354 = aim->savedUnk354;
            } else {
                camera->floorY = camera->CharPtr->floorY;
                camera->unk354 = camera->CharPtr->floorY;
            }
        }
        if (!aim->unk4D || (yawDelta < 0.05000000075f)) {
            camera->mv->AimState = 0;
            camera->DoingAim = 0;
        } else {
            math_springAngleTo(&camera->CameraAngle, aim->savedYaw, &aim->yawVel, 5.0f, 9.0f, camera->SpringTime);
        }
    }
    camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
    if (((zone == 2) || (zone == 0x13) || (zone == 0x12) || (zone == 0xA) || (zone == 0x34)) && (zone != 0xD)) {
        f32 offsetZ;
        f32 pad4; // matching: unused stack slot
        f32 offsetX;
        f32 shift;

        if (camera->SpecialFlags & 2) {
            shift = aim->targetYaw * -3.0f;
        } else {
            shift = -35.0f;
        }
        offsetX = sinf(camera->CameraAngleRad - 1.570796371f) * shift;
        offsetZ = cosf(camera->CameraAngleRad - 1.570796371f) * shift;
        if (camera->Snap) {
            aim->offset.x = offsetX;
            aim->offset.z = offsetZ;
            aim->offsetXVel = 0.0f;
        } else {
            math_springTo(&aim->offset.x, offsetX, &aim->offsetXVel, 4.0f, 9.0f, camera->SpringTime);
            math_springTo(&aim->offset.z, offsetZ, &aim->offsetZVel, 4.0f, 9.0f, camera->SpringTime);
        }
    } else {
        math_springTo(&aim->offset.x, 0.0f, &aim->offsetXVel, 4.0f, 9.0f, camera->SpringTime);
        math_springTo(&aim->offset.z, 0.0f, &aim->offsetZVel, 4.0f, 9.0f, camera->SpringTime);
    }
    camera->mv->aimTarget.x += aim->offset.x;
    camera->mv->aimTarget.z += aim->offset.z;
    camera->mv->aimEye.x += aim->offset.x;
    camera->mv->aimEye.z += aim->offset.z;
    math_dirTowards(&camera->mv->aimEye, &camera->mv->aimTarget,
                  &camera->mv->aimDir);
    camera_updateFollow(camera);
    camera_easeHeight(camera);
    camera_updateAngles(camera);
}

/* While R is held in a facing zone, publishes the player's aim ray (eye, target, direction) to the
   player extension block. */
void camera_publishAimRay(Actor *player, OSContPad *pad) {
    ActorCtl *ext;
    s32 zone;
    Vec3f target;
    Vec3f eye;
    f32 angle;
    Vec3f dir;
    f32 temp;

    player->mv->AimState = 0;
    if (pad->button & R_TRIG) {
        zone = player->mv->rt_context_b;
        if (IS_FACING_ZONE(zone)) {
            ext = player->mv;
            angle = (player->theta - 90.0f) * DEG2RAD;
            target.x = player->xpos;
            target.y = player->ypos + (ext->height * 0.75f);
            target.z = player->zpos;
            eye = target;
            target.x = (sinf(angle) * 100.0f) + eye.x;
            target.y = eye.y;
            target.z = (cosf(angle) * 100.0f) + eye.z;
            eye.y += player->ypos - player->old_ypos;
            ext->aimEye = eye;
            ext->aimTarget = target;
            ext->AimState = 1;
            dir.x = 1.0f;
            temp = dir.x * cosf(ext->aimPitch * -DEG2RAD);
            dir.y = dir.x * sinf(ext->aimPitch * -DEG2RAD);
            dir.x = temp;
            temp = dir.x * cosf(ext->aimYaw * -DEG2RAD);
            dir.z = dir.x * sinf(ext->aimYaw * -DEG2RAD);
            dir.x = temp;
            ext->aimDir = dir;
        }
    }
}

/* Sets the aim distance, unk190 and sideOffsetTarget for the camera's zone. */
void camera_setAimDistance(Camera *camera) {
    s32 zone;

    zone = camera->CharContext;
    if (zone) {
        if (zone == 0x34) {
            camera->YOffsetTget = 80.0f;
            camera->aim.distance = 350.0f;
            return;
        }
        if ((zone == 0x3B) && ((level == LEVEL_W3_HORROR_INTRO) || (level == LEVEL_W3_HAUNTED_HOUSE))) {
            camera->YOffsetTget = -12.0f;
            camera->sideOffsetTarget = -28.0f;
            camera->aim.distance = 106.0f;
            return;
        }
        if ((zone == 2) || (zone == 0xA) || (zone == 0x13)) {
            camera->YOffsetTget = 10.0f;
            camera->AvailableModes |= 0x80;
            camera->aim.distance = 135.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if ((zone == 0x12) && (level == LEVEL_W3_HAUNTED_HOUSE)) {
            camera->YOffsetTget = 10.0f;
            camera->aim.distance = 180.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0xF) {
            camera->YOffsetTget = 10.0f;
            camera->aim.distance = 480.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0x38) {
            camera->aim.distance = 110.0f;
            camera->YOffsetTget = 5.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        if (zone == 0x15) {
            camera->aim.distance = 110.0f;
            camera->YOffsetTget = 25.0f;
            camera->sideOffsetTarget = 0.0f;
            return;
        }
        camera->YOffsetTget = 5.0f;
        camera->aim.distance = 75.0f;
        camera->sideOffsetTarget = 0.0f;
        return;
    }
    camera->aim.distance = 90.0f;
    camera->YOffsetTget = 25.0f;
    camera->sideOffsetTarget = 0.0f;
}

/* Leaves aim mode once R is released: state 3 swings back to the saved view (or off at once when the
   previous mode was 0x40). */
void camera_leaveAim(Camera *camera) {
    CamAim *aim = &camera->aim;

    if (camera->PadPtr->button & R_TRIG) {
        return;
    }
    if (camera->MCache[CAMERA_MODE_AIM] == 0x40) {
        if (camera->cmd.type == 0xF) {
            camera->rail.eyeEaseA = 0.0f;
            camera->rail.eyeEaseB = 0.0f;
        }
        camera->mv->AimState = 0;
        camera->DoingAim = 0;
        camera->AimTimeOuts = 60;
    } else {
        camera->AimTimeOuts = 60;
        camera->DoingAim = 3;
    }
    camera->IdealTripodMagnitude = 200.0f;
    camera->height = 100.0f;
    camera->Tightness = aim->savedTightness;
    aim->useOverride = FALSE;
    camera->YOffsetTget = 0.0f;
    camera->sideOffsetTarget = 0.0f;
    camera->XZOffsetTget = 0.0f;
    actor_fadeCommand(camera->CharPtr, 2, 0xFF, 0x20, camera->CNumber);
    if (camera->AimTargetOn) {
        sight_resetPlayerEffects(camera);
        camera->AimTargetOn = FALSE;
    }
}
