#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct Vec3i {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;
typedef struct Game14F130Actor {
    u8 pad00[0x40];
    f32 yaw;
    u8 pad44[0x38];
    u16 angle;
    u8 pad7E[0x266];
    s32 rotation;
} Game14F130Actor;
typedef struct Game14F130Target {
    u8 pad00[0x12];
    s16 angle;
    u8 pad14[0x14C];
    Vec3i position;
    u8 pad16C[0x20];
    f32 yaw;
    u8 pad190[0x23];
    u8 enabled;
} Game14F130Target;
typedef struct Game14F130Camera {
    u8 pad00[0x84];
    s32 flags84;
    u8 pad88[0x1B4];
    u8 instant;
    u8 pad23D;
    u8 mode;
    u8 pad23F[0xB9];
    Vec3i position, previousPosition;
    u8 pad310[0x5C];
    u16 *controls;
    u8 pad370[0xC];
    f32 yaw;
    u8 pad380[0x1C];
    f32 yawRadians;
    u8 pad3A0[0x30];
    Game14F130Actor *actor;
    Game14F130Target *target;
    u8 pad3D8[0x218];
    s32 flags5F0;
    u8 pad5F4[0xC];
    u8 fast;
    u8 pad601[0x97];
    s32 locked;
    u8 pad69C[0x118];
    f32 delta;
    u8 pad7B8[0x10];
    f32 yawVelocity;
    u8 pad7CC[0x1A0];
    f32 speed, limit;
    u8 pad974[8];
    f32 targetSpeed, targetLimit;
    u8 pad984[8];
    f32 speedVelocity, limitVelocity;
} Game14F130Camera;

void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
void func_1512E140(void *);
void func_15123A54(void *);
void func_1512A390(void *);
extern f32 D_800A3448; // matching: a 999.0f literal becomes lui+mtc1, the original loads it from rodata
extern u8 D_800BEA0C;

void func_15121C80(Game14F130Camera *camera, f32 yawOffset) {
    f32 angle;
    f32 speed;
    f32 limit;
    s32 mode;
    s32 active;
    Vec3i position;

    active = (camera->flags5F0 & 0x10) != 0;
    if (active != 0) {
        active = (*camera->controls & 0x10) != 0;
    }
    if (active == 0 && (D_800BE616 != 0 || (mode = camera->mode) == 9 || mode == 0x3B) && camera->target->enabled != 0) {
        if (D_800BE616 != 0) {
            camera->targetSpeed = 5.0f;
            camera->targetLimit = 8.0f;
            func_150495B0(&camera->speed, 5.0f, &camera->speedVelocity, 1.0f, 2.0f, camera->delta);
            func_150495B0(&camera->limit, camera->targetLimit, &camera->limitVelocity, 1.0f, 2.0f, camera->delta);
            func_15049688(&camera->yaw, camera->actor->yaw - 180.0f, &camera->yawVelocity, camera->speed, camera->limit, camera->delta);
        } else {
            func_15049688(&camera->yaw, camera->actor->yaw - 180.0f, &camera->yawVelocity, 5.0f, 8.0f, camera->delta);
        }
        camera->yawRadians = camera->yaw * 0.017453292f;
    } else if (camera->locked == 0) {
        angle = ((camera->actor->yaw + (f32) camera->actor->angle * 0.0054931640625f) - 180.0f) - yawOffset;
        if (active != 0 && camera->mode != 0x1C) {
            angle -= (f32) (camera->actor->rotation >> 16) * 0.02f;
        }
        mode = camera->mode;
        if (mode == 9 || mode == 0x38 || mode == 0x39 || mode == 0x37 || mode == 0x3B || mode == 0x12) {
            angle -= (f32) camera->target->angle * 0.0054931640625f;
        }
        func_15048758(&angle);
        if (camera->instant != 0) {
            speed = camera->target->yaw;
            limit = D_800A3448;
            if (limit != speed) {
                camera->yaw = speed;
                position = camera->target->position;
                camera->position = position;
                camera->previousPosition = position;
                camera->target->yaw = limit;
            } else {
                camera->yaw = angle;
            }
            camera->yawVelocity = 0.0f;
        } else {
            if (active != 0 || camera->fast != 0) {
                speed = 4.0f;
                limit = 6.0f;
            } else {
                mode = camera->mode;
                if ((mode == 9 && D_800BE616 != 0) || mode == 0x38 || mode == 0x39 || mode == 0x37 || mode == 0x3B || mode == 0x12) {
                    speed = 2.0f;
                    limit = 6.0f;
                } else if (camera->flags84 & 0x200000) {
                    speed = 1.0f;
                    limit = 2.8f;
                } else {
                    speed = 0.75f;
                    limit = 2.2f;
                }
            }
            if (D_800BEA0C == 0) {
                func_15049688(&camera->yaw, angle, &camera->yawVelocity, speed, limit, camera->delta);
            }
        }
        camera->yawRadians = camera->yaw * 0.017453292f;
    }
    func_1512A390(camera);
    func_15123A54(camera);
    func_1512E140(camera);
}
