#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "macros.h"

extern f32 D_800894C0;
extern Vec3f D_800894C8;

void func_150495B0(f32 *value, f32 target, f32 *vel, f32 accel, f32 maxSpeed, f32 dt);
void func_15123070(Camera *camera);
void func_15123A54(Camera *camera);
void func_1512E140(Camera *camera);
void func_1512A390(Camera *camera);

#define SQ(x) ((x) * (x))
#define DEG2RAD 0.01745329238f

/* Places the eye on an orbit around the target: yaw from the player's facing (or the camera yaw),
   pitch from the player or pitchOverride, then eases it there and keeps it out of the target. */
void func_15122440(Camera *camera) {
    Vec3f offset = D_800894C8;
    f32 horiz;
    f32 yaw;
    f32 pitch;
    f32 sinYaw;
    f32 goalX;
    f32 goalZ;
    f32 goalY;

    yaw = (camera->CharPtr->theta + 90.0f) * DEG2RAD;
    func_1512A390(camera);
    if (!(camera->SpecialFlags & 1) && (camera->CharPtr->unk102 == 0)) {
        yaw = camera->CameraAngleRad - 1.570796371f;
    }
    if (camera->ForceSwimAngle != 999.0f) {
        pitch = camera->ForceSwimAngle;
    } else if (camera->CharPtr->energy == 0) {
        pitch = 0.0f;
    } else {
        pitch = (camera->CharPtr->unk102 ? 10.0f : 0.0f) + camera->CharPtr->vtilt;
    }
    if (camera->CharPtr->unk102) {
        pitch *= 0.5f;
    }
    pitch = CLAMP(pitch, -88.0f, 88.0f);
    if (fabsf(D_800894C0 - pitch) < 10.0f) {
        pitch = D_800894C0;
    } else {
        D_800894C0 = pitch;
    }
    pitch *= DEG2RAD;
    func_15123070(camera);
    if (camera->PadPtr->button & R_TRIG) {
        offset.x = 150.0f;
    } else {
        offset.x = camera->IdealTripodMagnitude;
    }
    if ((camera->CharPtr->unk102 != 0) && (camera->TonyHitsClear == 0)) {
        if (camera->TripodMagnitude < camera->IdealTripodMagnitude) {
            offset.x = camera->TripodMagnitude;
        }
    }
    horiz = cosf(pitch) * offset.x;
    offset.y = sinf(pitch) * offset.x;
    offset.x = horiz;
lbl: // matching: the unused label starts a new basic block before the orbit rotation
    {
        f32 cosYaw;
        sinYaw = sinf(yaw);
        cosYaw = cosf(yaw);
        camera->SwimLockedRy = yaw;
        do { // matching: block split after the unk3DC store keeps offset's fields in registers
            offset.z = -offset.x * sinYaw;
            offset.x = offset.x * cosYaw;
            offset.y = CLAMP(offset.y, -200.0f, 200.0f);
        } while (0);
        offset.x += camera->target.x;
        offset.y += camera->target.y;
        offset.z += camera->target.z;
    }
    {
        f32 accel = 1.0f;
        f32 maxSpeed = 2.200000048f;
        goalX = offset.x;
        goalY = offset.y;
        goalZ = offset.z;
        if (camera->Snap) {
            camera->eye.x = goalX;
            camera->eye.y = goalY;
            camera->eye.z = goalZ;
            camera->CurrentVelX = 0.0f;
            camera->CurrentVelY = 0.0f;
            camera->CurrentVelZ = 0.0f;
        } else {
            accel *= 2.0f;
            maxSpeed *= 2.0f;
            if (camera->CharPtr->unk102) {
                accel *= 2.0f;
                maxSpeed *= 2.0f;
            }
            func_150495B0(&camera->eye.x, goalX, &camera->CurrentVelX, accel, maxSpeed, camera->SpringTime);
            func_150495B0(&camera->eye.y, goalY, &camera->CurrentVelY, accel, maxSpeed, camera->SpringTime);
            func_150495B0(&camera->eye.z, goalZ, &camera->CurrentVelZ, accel, maxSpeed, camera->SpringTime);
        }
    }
    if ((camera->CharPtr->unk102 == 0) && !(camera->CameraButtons & R_TRIG)) {
        if (sqrtf(SQ(camera->eye.x - camera->target.x) + SQ(camera->eye.y - camera->target.y) + SQ(camera->eye.z - camera->target.z)) < 120.0f) {
            Vec3f dir;
            func_150491EC(&camera->target, &camera->eye, &dir);
            camera->eye.x = camera->target.x + 120.0f * dir.x;
            camera->eye.y = camera->target.y + 120.0f * dir.y;
            camera->eye.z = camera->target.z + 120.0f * dir.z;
        }
    }
    if (camera->eye.y < camera->unk35C + 30.0f) {
        camera->eye.y = camera->unk35C + 30.0f;
    }
    camera->YawAngle += (camera->Roll - camera->YawAngle) * 0.150000006f;
    camera->TripodMagnitude = sqrtf(SQ(camera->eye.x - camera->target.x) + SQ(camera->eye.z - camera->target.z));
    camera->height = camera->unk344 = camera->eye.y - camera->unk354;
}

/* Eases yawFollow toward a gain picked from the camera state and turns yaw by it toward the target's
   motion, then runs the per-frame camera updates. */
void func_15122980(Camera *camera) {
    Vec3f toPrevTarget;
    Vec3f toTarget;
    f32 prevAngle;
    f32 angle;

    if (camera->Motion != 4) {
        func_15048F90(&camera->eye, &camera->prevTarget, &toPrevTarget);
        func_15048F90(&camera->eye, &camera->target, &toTarget);
        prevAngle = func_15048FC8(&toPrevTarget);
        angle = func_15048FC8(&toTarget);
        if (camera->CharMoved == 0) {
            camera->PredictSpeedTarget = 0.0f;
        } else if ((camera->AvailableModes & 0x100000) || ((camera->CharRelAngle > 180.0f) && (camera->CharRelAngle < 270.0f))) {
            camera->PredictSpeedTarget = 0.75f;
        } else {
            camera->PredictSpeedTarget = 0.5f;
        }
        func_150495B0(&camera->PredictSpeed, camera->PredictSpeedTarget, &camera->PredictSpeedVel, 1.0f, 2.0f, camera->SpringTime);
        camera->CameraAngle += func_15048A70(prevAngle, angle) * camera->PredictSpeed;
        camera->CameraAngleRad = camera->CameraAngle * DEG2RAD;
    }

    func_15123A54(camera);
    func_1512E140(camera);
    func_1512A390(camera);
}
