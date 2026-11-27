#include <ultra64.h>
#include "conker/camera.h"
#include "conker/math.h"

/**
 * Springs the camera yaw (degrees) towards targetYaw with math_springAngleTo, using yawSpringVel as its
 * velocity, then stores the yaw in radians in yawRad.
 */
void camera_springYaw(Camera *camera, f32 targetYaw, s32 unused, f32 speedGain, f32 accelGain) {
    math_springAngleTo(&camera->CameraAngle, targetYaw, &camera->AutoAlignTargetYawVel, speedGain, accelGain, camera->SpringTime);
    camera->CameraAngleRad = camera->CameraAngle * 0.01745329238f; // degrees to radians
}

/** Empty stub. */
void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}
