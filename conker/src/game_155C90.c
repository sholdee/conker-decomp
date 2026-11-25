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

/* Player extension block (struct127.unk31C, cached in Camera.playerExt); see game_14FF90.c. */
typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ u8 unk120;
} CamPlayerExt;

/* File-local view of the camera (struct108); include/structs.h lacks 0x65C/0x660. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 mode;
    /* 0x030 */ u8 pad30[0x104];
    /* 0x134 */ s32 presetIndex;
    /* 0x138 */ u8 pad138[0x104];
    /* 0x23C */ u8 snap; /* nonzero: jump to targets instead of easing */
    /* 0x23D */ u8 pad23D[0x153];
    /* 0x390 */ f32 angle; /* degrees */
    /* 0x394 */ u8 pad394[0x14];
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x24];
    /* 0x3D0 */ struct127 *player;
    /* 0x3D4 */ CamPlayerExt *playerExt;
    /* 0x3D8 */ u8 pad3D8[0x210];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ u8 pad5EC[0x70];
    /* 0x65C */ f32 unk3A8Vel;
    /* 0x660 */ f32 unk5E8Vel;
    /* 0x664 */ u8 pad664[0x64];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ u8 pad6CC[0x30];
    /* 0x6FC */ s32 unk6FC;
    /* 0x700 */ u8 pad700[0x3C];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 dt; /* frame time scale */
} Camera;

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

    savedPresetIndex = camera->presetIndex;
    if (camera->mode & 0x40) {
        camera->presetIndex = 0;
    }
    if ((D_800D2DB4 != 0) || ((camera->unk73C != 0) && (camera->unk73C != 3)) || (camera->playerExt->unk120 != 0)) {
        func_150495B0(&camera->unk5E8, 0.0f, &camera->unk5E8Vel, 6.0f, 9.0f, camera->dt);
        func_150495B0(&camera->unk3A8, 0.0f, &camera->unk3A8Vel, 1.5f, 2.5f, camera->dt);
        return;
    }
    unk3A8Scale = D_800894F0[camera->presetIndex].unk3A8Scale;
    unk5E8Scale = D_800894F0[camera->presetIndex].unk5E8Scale;
    unk3A8SpeedScale = D_800894F0[camera->presetIndex].unk3A8SpeedScale;
    unk5E8SpeedScale = D_800894F0[camera->presetIndex].unk5E8SpeedScale;
    unk3A8Bias = D_800894F0[camera->presetIndex].unk3A8Bias;
    snap = camera->snap;
    if ((snap != 0) || (unk3A8Scale == unk3A8SpeedScale)) {
        speed = 0.0f;
    } else {
        speed = camera->player->xz_velocity;
    }
    if (speed > 30.0f) {
        speed = 30.0f;
    }
    angle = camera->angle;
    if ((camera->unk6C8 != 0) && ((camera->unk6FC == 10) || (camera->unk6FC == 14))) {
        angle = fabsf(camera->angle);
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
        camera->unk3A8Vel = 0.0f;
    } else {
        func_150495B0(&camera->unk3A8, target, &camera->unk3A8Vel, 1.5f, 2.5f, camera->dt);
    }
    foldedAngle = camera->angle;
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
        camera->unk5E8Vel = 0.0f;
    } else if (camera->mode & 0x100) {
        func_150495B0(&camera->unk5E8, target, &camera->unk5E8Vel, 6.0f, 9.0f, camera->dt);
    } else {
        func_150495B0(&camera->unk5E8, target, &camera->unk5E8Vel, 1.5f, 2.5f, camera->dt);
    }
    camera->presetIndex = savedPresetIndex;
}
