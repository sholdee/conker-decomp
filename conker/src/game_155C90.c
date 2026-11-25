#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Per-preset coefficients for func_151287E0, indexed by Camera.presetIndex. */
typedef struct {
    /* 0x00 */ f32 unk3A8Scale;
    /* 0x04 */ f32 unk5E8Scale;
    /* 0x08 */ f32 unk3A8SpeedScale;
    /* 0x0C */ f32 unk5E8SpeedScale;
    /* 0x10 */ f32 unk3A8Bias;
} CamTiltPreset; // size 0x14

extern CamTiltPreset D_800894F0[];
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 arg3, f32 arg4, f32 dt);

/*
 * Eases the camera's unk3A8/unk5E8 towards targets derived from its angle and the player's speed,
 * using the coefficients of the current preset. If outUnk5E8/outUnk3A8 are given, the targets are
 * written there instead of being applied.
 */
void func_151287E0(Camera *camera, f32 *outUnk5E8, f32 *outUnk3A8) {
    s32 pad; // matching: unused stack slot keeps the frame layout
    s32 savedPresetIndex;
    f32 unk3A8Scale;
    f32 unk5E8Scale;
    f32 unk3A8SpeedScale;
    f32 unk5E8SpeedScale;
    f32 unk3A8Bias;
    f32 angle;
    f32 sign;
    f32 speed;
    f32 target;
    f32 foldedAngle;
    s32 snap;

    savedPresetIndex = camera->unk134;
    if (camera->mode & 0x40) {
        camera->unk134 = 0;
    }
    if ((D_800D2DB4 != 0) || ((camera->aimState != 0) && (camera->aimState != 3)) || (camera->playerExt->death_State != 0)) {
        func_150495B0(&camera->unk5E8, 0.0f, &camera->unk660, 6.0f, 9.0f, camera->dt);
        func_150495B0(&camera->unk3A8, 0.0f, &camera->unk65C, 1.5f, 2.5f, camera->dt);
        return;
    }
    unk3A8Scale = D_800894F0[camera->unk134].unk3A8Scale;
    unk5E8Scale = D_800894F0[camera->unk134].unk5E8Scale;
    unk3A8SpeedScale = D_800894F0[camera->unk134].unk3A8SpeedScale;
    unk5E8SpeedScale = D_800894F0[camera->unk134].unk5E8SpeedScale;
    unk3A8Bias = D_800894F0[camera->unk134].unk3A8Bias;
    snap = camera->snap;
    if ((snap != 0) || (unk3A8Scale == unk3A8SpeedScale)) {
        speed = 0.0f;
    } else {
        speed = camera->player->xz_velocity;
    }
    if (speed > 30.0f) {
        speed = 30.0f;
    }
    angle = camera->unk390;
    if ((camera->cmdRunning != 0) && ((camera->cmd.type == 10) || (camera->cmd.type == 14))) {
        angle = fabsf(camera->unk390);
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
    target = angle * (1.0f / 90.0f) * unk3A8Scale - angle * speed * (1.0f / 2700.0f) * unk3A8SpeedScale + unk3A8Bias;
    if (camera->mode & 0x100) {
        target = 0.0f;
    }
    if (outUnk3A8 != NULL) {
        *outUnk3A8 = target;
    } else if (snap != 0) {
        camera->unk3A8 = target;
        camera->unk65C = 0.0f;
    } else {
        func_150495B0(&camera->unk3A8, target, &camera->unk65C, 1.5f, 2.5f, camera->dt);
    }
    foldedAngle = camera->unk390;
    while (foldedAngle > 180.0f) {
        foldedAngle -= 360.0f;
    }
    if (foldedAngle > 90.0f) {
        foldedAngle = 180.0f - foldedAngle;
    } else if (foldedAngle < -90.0f) {
        foldedAngle = -180.0f - foldedAngle;
    }
    target = -(foldedAngle * (1.0f / 90.0f) * unk5E8Scale) - foldedAngle * speed * (1.0f / 2700.0f) * unk5E8SpeedScale;
    if (outUnk5E8 != NULL) {
        *outUnk5E8 = target;
    } else if (camera->snap != 0) { // matching: reload, not the snap local
        camera->unk5E8 = target;
        camera->unk660 = 0.0f;
    } else if (camera->mode & 0x100) {
        func_150495B0(&camera->unk5E8, target, &camera->unk660, 6.0f, 9.0f, camera->dt);
    } else {
        func_150495B0(&camera->unk5E8, target, &camera->unk660, 1.5f, 2.5f, camera->dt);
    }
    camera->unk134 = savedPresetIndex;
}
