#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Springs the camera yaw (degrees) towards targetYaw with func_15049688, using yawSpringVel as its
 * velocity, then stores the yaw in radians in yawRad.
 */
void camera_springYaw(Camera *camera, f32 targetYaw, s32 unused, f32 speedGain, f32 accelGain) {
    func_15049688(&camera->CameraAngle, targetYaw, &camera->AutoAlignTargetYawVel, speedGain, accelGain, camera->SpringTime);
    camera->CameraAngleRad = camera->CameraAngle * 0.01745329238f; // degrees to radians
}

/** Empty stub. */
void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}
