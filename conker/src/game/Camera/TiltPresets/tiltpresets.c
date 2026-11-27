#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/math.h"
#include "conker/script.h"



/* Per-preset coefficients for camera_updateAngleOffsets, indexed by Camera.Tightness. */
typedef struct {
    /* 0x00 */ f32 unk3A8Scale;
    /* 0x04 */ f32 unk5E8Scale;
    /* 0x08 */ f32 unk3A8SpeedScale;
    /* 0x0C */ f32 unk5E8SpeedScale;
    /* 0x10 */ f32 unk3A8Bias;
} CamTiltPreset; // size 0x14

static CamTiltPreset gCameraAngleOffsetPresets[] = {
    { 3.8f, 4.0f, 0.0f, 0.0f, 4.0f },
    { 5.8f, 6.0f, 0.0f, 8.0f, 0.0f },
};

/*
 * Eases the camera's APitchAngle/YawAngle towards targets derived from its angle and the player's speed,
 * using the coefficients of the current preset. If outYawAngle/outPitchAngle are given, the targets are
 * written there instead of being applied.
 */
void camera_updateAngleOffsets(Camera *camera, f32 *outYawAngle, f32 *outPitchAngle) {
    s32 pad; // matching: unused stack slot keeps the frame layout
    s32 prevTightness;
    f32 pitchScale;
    f32 yawScale;
    f32 pitchSpeedScale;
    f32 yawSpeedScale;
    f32 pitchBias;
    f32 angle;
    f32 sign;
    f32 speed;
    f32 target;
    f32 foldedAngle;
    s32 snap;

    prevTightness = camera->Tightness;
    if (camera->Mode & 0x40) {
        camera->Tightness = 0;
    }
    if ((gPoseCommandRunning != 0) || ((camera->DoingAim != 0) && (camera->DoingAim != 3)) || (camera->mv->death_State != 0)) {
        math_springTo(&camera->YawAngle, 0.0f, &camera->CentYawAngleVel, 6.0f, 9.0f, camera->SpringTime);
        math_springTo(&camera->APitchAngle, 0.0f, &camera->CentPitchAngleVel, 1.5f, 2.5f, camera->SpringTime);
        return;
    }
    pitchScale = gCameraAngleOffsetPresets[camera->Tightness].unk3A8Scale;
    yawScale = gCameraAngleOffsetPresets[camera->Tightness].unk5E8Scale;
    pitchSpeedScale = gCameraAngleOffsetPresets[camera->Tightness].unk3A8SpeedScale;
    yawSpeedScale = gCameraAngleOffsetPresets[camera->Tightness].unk5E8SpeedScale;
    pitchBias = gCameraAngleOffsetPresets[camera->Tightness].unk3A8Bias;
    snap = camera->Snap;
    if ((snap != 0) || (pitchScale == pitchSpeedScale)) {
        speed = 0.0f;
    } else {
        speed = camera->CharPtr->speed;
    }
    if (speed > 30.0f) {
        speed = 30.0f;
    }
    angle = camera->CharRelAngle;
    if ((camera->cmdRunning != 0) && ((camera->cmd.type == 10) || (camera->cmd.type == 14))) {
        angle = fabsf(camera->CharRelAngle);
    }
    while (angle > 180.0f) {
        angle = 360.0f - angle;
    }
    if (angle < 90.0f) {
        angle = -180.0f + angle;
    }
    if (angle < 0.0f) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    angle -= 90.0f * sign;
    target = angle * (1.0f / 90.0f) * pitchScale - angle * speed * (1.0f / 2700.0f) * pitchSpeedScale + pitchBias;
    if (camera->Mode & 0x100) {
        target = 0.0f;
    }
    if (outPitchAngle != NULL) {
        *outPitchAngle = target;
    } else if (snap != 0) {
        camera->APitchAngle = target;
        camera->CentPitchAngleVel = 0.0f;
    } else {
        math_springTo(&camera->APitchAngle, target, &camera->CentPitchAngleVel, 1.5f, 2.5f, camera->SpringTime);
    }
    foldedAngle = camera->CharRelAngle;
    while (foldedAngle > 180.0f) {
        foldedAngle -= 360.0f;
    }
    if (foldedAngle > 90.0f) {
        foldedAngle = 180.0f - foldedAngle;
    } else if (foldedAngle < -90.0f) {
        foldedAngle = -180.0f - foldedAngle;
    }
    target = -(foldedAngle * (1.0f / 90.0f) * yawScale) - foldedAngle * speed * (1.0f / 2700.0f) * yawSpeedScale;
    if (outYawAngle != NULL) {
        *outYawAngle = target;
    } else if (camera->Snap != 0) { // matching: reload, not the snap local
        camera->YawAngle = target;
        camera->CentYawAngleVel = 0.0f;
    } else if (camera->Mode & 0x100) {
        math_springTo(&camera->YawAngle, target, &camera->CentYawAngleVel, 6.0f, 9.0f, camera->SpringTime);
    } else {
        math_springTo(&camera->YawAngle, target, &camera->CentYawAngleVel, 1.5f, 2.5f, camera->SpringTime);
    }
    camera->Tightness = prevTightness;
}
