#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define DEG2RAD 0.01745329238f

/* Eases yawFollow toward a gain picked from the camera state and turns yaw by it toward the target's
   motion, then runs the per-frame camera updates. */
void camera_followYaw(Camera *camera) {
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

    camera_updateFollow(camera);
    camera_easeHeight(camera);
    camera_updateFloorHeights(camera);
}
