#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/**
 * Eases the camera yaw toward the yaw behind its actor (or straight to the saved target yaw when the camera
 * is instant), then runs the three follow-up camera updates.
 */
void camera_easeYawBehind(Camera *camera, f32 yawOffset) {
    f32 angle;
    f32 speed;
    f32 limit;
    s32 mode;
    s32 active;
    Vec3f position;

    active = (camera->SpecialFlags & 0x10) != 0;
    if (active) {
        active = (camera->PadPtr->button & 0x10) != 0;
    }
    if (!active && (gIsMultiplayer || camera->CharContext == 9 || camera->CharContext == 59) && camera->mv->unk1B3) {
        if (gIsMultiplayer) {
            camera->targetSpeed = 5.0f;
            camera->targetLimit = 8.0f;
            func_150495B0(&camera->speed, 5.0f, &camera->speedVel, 1.0f, 2.0f, camera->SpringTime);
            func_150495B0(&camera->limit, camera->targetLimit, &camera->limitVel, 1.0f, 2.0f, camera->SpringTime);
            func_15049688(&camera->CameraAngle, camera->CharPtr->theta - 180.0f, &camera->AutoBehindAngleVel, camera->speed, camera->limit, camera->SpringTime);
        } else {
            func_15049688(&camera->CameraAngle, camera->CharPtr->theta - 180.0f, &camera->AutoBehindAngleVel, 5.0f, 8.0f, camera->SpringTime);
        }
        camera->CameraAngleRad = camera->CameraAngle * (3.1415927f / 180.0f);
    } else if (!camera->unk698) {
        angle = ((camera->CharPtr->theta + camera->CharPtr->mod_angle * (360.0f / 65536)) - 180.0f) - yawOffset;
        if (active && camera->CharContext != 28) {
            angle -= (camera->CharPtr->taskint[0] >> 16) * 0.02f;
        }
        if (camera->CharContext == 9 || camera->CharContext == 56 || camera->CharContext == 57 || camera->CharContext == 55 ||
            camera->CharContext == 59 || camera->CharContext == 18) {
            angle -= camera->mv->pssd_waist_tilt * (360.0f / 65536);
        }
        func_15048758(&angle);
        if (camera->Snap) {
            if (camera->mv->unk18C != 999.0f) { // 999.0f: "no saved yaw" sentinel
                camera->CameraAngle = camera->mv->unk18C;
                position = camera->mv->unk160;
                camera->eye = position;
                camera->prevEye = position;
                camera->mv->unk18C = 999.0f;
            } else {
                camera->CameraAngle = angle;
            }
            camera->AutoBehindAngleVel = 0.0f;
        } else {
            if (active || camera->blocked) {
                speed = 4.0f;
                limit = 6.0f;
            } else {
                mode = camera->CharContext;
                if ((mode == 9 && gIsMultiplayer) || mode == 56 || mode == 57 || mode == 55 || mode == 59 ||
                    mode == 18) {
                    speed = 2.0f;
                    limit = 6.0f;
                } else if (camera->AvailableModes & 0x200000) {
                    speed = 1.0f;
                    limit = 2.8f;
                } else {
                    speed = 0.75f;
                    limit = 2.2f;
                }
            }
            if (!gFrameRateDivider) {
                func_15049688(&camera->CameraAngle, angle, &camera->AutoBehindAngleVel, speed, limit, camera->SpringTime);
            }
        }
        camera->CameraAngleRad = camera->CameraAngle * (3.1415927f / 180.0f);
    }
    camera_updateFloorHeights(camera);
    camera_updateFollow(camera);
    camera_easeHeight(camera);
}
