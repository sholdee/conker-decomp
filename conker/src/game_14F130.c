#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_150495B0(f32 *value, f32 goal, f32 *velocity, f32 arg3, f32 arg4, f32 dt);
void func_1512E140(Camera *camera);
void func_15123A54(Camera *camera);
void func_1512A390(Camera *camera);
extern f32 D_800A3448; // 999.0f, "no saved yaw" sentinel; matching: a literal becomes lui+mtc1 instead of this load
extern u8 D_800BEA0C;

/**
 * Eases the camera yaw toward the yaw behind its actor (or straight to the saved target yaw when the camera
 * is instant), then runs the three follow-up camera updates.
 */
void func_15121C80(Camera *camera, f32 yawOffset) {
    f32 angle;
    f32 speed;
    f32 limit;
    s32 mode;
    s32 active;
    vertex position;

    active = (camera->status & 0x10) != 0;
    if (active) {
        active = (camera->contPad->button & 0x10) != 0;
    }
    if (!active && (D_800BE616 || camera->zone == 9 || camera->zone == 59) && camera->playerExt->unk1B3) {
        if (D_800BE616) {
            camera->targetSpeed = 5.0f;
            camera->targetLimit = 8.0f;
            func_150495B0(&camera->speed, 5.0f, &camera->speedVel, 1.0f, 2.0f, camera->dt);
            func_150495B0(&camera->limit, camera->targetLimit, &camera->limitVel, 1.0f, 2.0f, camera->dt);
            func_15049688(&camera->yaw, camera->player->unk40 - 180.0f, &camera->yawEaseVel, camera->speed, camera->limit, camera->dt);
        } else {
            func_15049688(&camera->yaw, camera->player->unk40 - 180.0f, &camera->yawEaseVel, 5.0f, 8.0f, camera->dt);
        }
        camera->yawRad = camera->yaw * (3.1415927f / 180.0f);
    } else if (!camera->unk698) {
        angle = ((camera->player->unk40 + (f32) camera->player->unk7C * (360.0f / 65536)) - 180.0f) - yawOffset;
        if (active && camera->zone != 28) {
            angle -= (f32) (camera->player->unk2E4 >> 16) * 0.02f;
        }
        if (camera->zone == 9 || camera->zone == 56 || camera->zone == 57 || camera->zone == 55 ||
            camera->zone == 59 || camera->zone == 18) {
            angle -= (f32) camera->playerExt->unk12 * (360.0f / 65536);
        }
        func_15048758(&angle);
        if (camera->snap) {
            // matching: reusing speed/limit as temporaries keeps the original stack frame
            speed = camera->playerExt->unk18C;
            limit = D_800A3448;
            if (limit != speed) {
                camera->yaw = speed;
                position = camera->playerExt->unk160;
                camera->eye = position;
                camera->prevEye = position;
                camera->playerExt->unk18C = limit;
            } else {
                camera->yaw = angle;
            }
            camera->yawEaseVel = 0.0f;
        } else {
            if (active || camera->blocked) {
                speed = 4.0f;
                limit = 6.0f;
            } else {
                mode = camera->zone;
                if ((mode == 9 && D_800BE616) || mode == 56 || mode == 57 || mode == 55 || mode == 59 ||
                    mode == 18) {
                    speed = 2.0f;
                    limit = 6.0f;
                } else if (camera->flags & 0x200000) {
                    speed = 1.0f;
                    limit = 2.8f;
                } else {
                    speed = 0.75f;
                    limit = 2.2f;
                }
            }
            if (!D_800BEA0C) {
                func_15049688(&camera->yaw, angle, &camera->yawEaseVel, speed, limit, camera->dt);
            }
        }
        camera->yawRad = camera->yaw * (3.1415927f / 180.0f);
    }
    func_1512A390(camera);
    func_15123A54(camera);
    func_1512E140(camera);
}
