#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
CamOffset *gCameraShakeTables[4]; // camera shake tables: one CamOffset delta per frame
s32 gCameraShakeLengths[4];        // their lengths in frames
Mtx *gViewMtxBuffers[2];       // per framebuffer: one view matrix per view

#include "functions.h"
#include "variables.h"

#define VIEW_MTX(player) (&gViewMtxBuffers[D_800BE9C0][player])

/*
 * Builds the player's view matrix for this frame: eases the side/lift offsets, places eye and target,
 * then stacks the shake, sway roll, per-player offsets (gCameraOffsets) and the breathing wobble on top.
 */
void camera_buildViewMatrix(Camera *camera) {
    Mtx rotMtx;
    Mtx transMtx;
    s32 pad114; // matching: unused stack slot in the original frame
    f32 side;
    f32 lift;
    f32 shiftX;
    f32 shiftZ;
    s32 pad100; // matching: unused stack slot in the original frame
    f32 sideZ;
    f32 sideX;
    Vec3f eye;
    Vec3f at;
    Vec3f prevEye;
    Vec3f prevTarget;
    f32 shakeScale;
    Vec3f dir;
    f32 stiffness;
    f32 damping;
    f32 dirZ;
    f32 dirY;
    f32 dirX;
    MtxF dirMtx;
    f32 yaw;

    shakeScale = gViews[camera->CNumber].perspScale;
    shakeScale = (1.0f - shakeScale) * 0.5f + shakeScale;
    if (camera->mv->death_State != 0) {
        camera->XZOffsetTget = 0.0f;
        camera->YOffsetTget = 0.0f;
        camera->aim.offset.x = 0.0f;
        camera->aim.offset.z = 0.0f;
    }
    if (camera->Snap != 0) {
        camera->OffsetXZVel = 0.0f;
        camera->OffsetYVel = 0.0f;
        camera->XZOffset = camera->XZOffsetTget;
        camera->YOffset = camera->YOffsetTget;
    } else if (camera->Mode == 0x100 || camera->CharContext != 0) {
        func_150495B0(&camera->XZOffset, camera->XZOffsetTget, &camera->OffsetXZVel, 4.0f, 6.0f, camera->SpringTime);
        func_150495B0(&camera->YOffset, camera->YOffsetTget, &camera->OffsetYVel, 4.0f, 6.0f, camera->SpringTime);
    } else {
        func_150495B0(&camera->XZOffset, camera->XZOffsetTget, &camera->OffsetXZVel, 0.75f, 1.1f, camera->SpringTime);
        func_150495B0(&camera->YOffset, camera->YOffsetTget, &camera->OffsetYVel, 0.75f, 1.1f, camera->SpringTime);
    }
    camera->savedTarget = camera->target;
    camera->savedEye = camera->eye;
    if ((camera->Mode & 0x40000) || (camera->Mode & 0x20)) {
        eye = camera->eye;
        at = camera->focus;
    } else if (camera->mv->SniperState != 0) {
        eye = camera->eye;
        at = camera->target;
        camera_updatePitch(camera);
        camera_updateAngles(camera);
    } else {
        side = camera->XZOffset;
        lift = camera->YOffset;
        shiftX = camera->aim.offset.x;
        shiftZ = camera->aim.offset.z;
        sideX = sinf(camera->TrueCameraAngleRad - 1.57079637f) * side; // yaw - pi/2: perpendicular to the view
        sideZ = cosf(camera->TrueCameraAngleRad - 1.57079637f) * side;
        eye.x = camera->eye.x + shiftX + sideX;
        eye.y = camera->eye.y + lift;
        eye.z = camera->eye.z + shiftZ + sideZ;
        at.x = camera->target.x + shiftX + sideX;
        at.y = camera->target.y + lift;
        at.z = camera->target.z + shiftZ + sideZ;
        // matching: these copies are never read, but the original makes them
        prevEye = camera->eye;
        prevTarget = camera->target;
        camera->eye = eye;
        camera->target = at;
        camera_updatePitch(camera);
        camera_updateAngles(camera);
    }
    func_150491EC(&eye, &at, &dir);
    camera->mv->aimTarget = at;
    camera->mv->aimEye = eye;
    camera->mv->aimDir = dir;
    {
        f32 swayTarget;

        func_15047B80(VIEW_MTX(camera->CNumber), &gLookAts[D_800BE9C0], eye.x, eye.y, eye.z, at.x, at.y, at.z,
                      0.0f, 1.0f, 0.0f);
        if (camera->Mode & 0x80000) {
            camera_applyBreathing(camera);
            func_1512D368(camera);
            return;
        }
        if (camera->Mode == 0x40000) {
            func_150A7DF0(&rotMtx, 0.0f, 0.0f, camera->Roll);
            guMtxCatL(VIEW_MTX(camera->CNumber), &rotMtx, VIEW_MTX(camera->CNumber));
            camera_applyBreathing(camera);
            return;
        }
        if (camera->mv->SniperState != 0) {
            if (camera->DoingDeltas == 2) {
                CamOffset *tbl;

                tbl = gCameraShakeTables[camera->DeltaPlaying];
                // matching: index-first addition orders the addu operands as the original (tbl[i] swaps them)
                camera->DeltaRoll += (camera->DeltaCurrentIndex + tbl)->rot.x;
                camera->DeltaPitch += (camera->DeltaCurrentIndex + tbl)->rot.y;
                camera->DeltaYaw += (camera->DeltaCurrentIndex + tbl)->rot.z;
                camera->DeltaX += (camera->DeltaCurrentIndex + tbl)->pos.x;
                camera->DeltaY += (camera->DeltaCurrentIndex + tbl)->pos.y;
                camera->DeltaZ += (camera->DeltaCurrentIndex + tbl)->pos.z;
            } else {
                camera->DeltaRoll -= camera->DeltaRoll * 0.08f;
                camera->DeltaPitch -= camera->DeltaPitch * 0.08f;
                camera->DeltaYaw -= camera->DeltaYaw * 0.08f;
                camera->DeltaX -= camera->DeltaX * 0.08f;
                camera->DeltaY -= camera->DeltaY * 0.08f;
                camera->DeltaZ -= camera->DeltaZ * 0.08f;
            }
            camera_updateShake(camera);
        } else {
            camera->DeltaRoll -= camera->DeltaRoll * 0.08f;
            camera->DeltaPitch -= camera->DeltaPitch * 0.08f;
            camera->DeltaYaw -= camera->DeltaYaw * 0.08f;
            camera->DeltaX -= camera->DeltaX * 0.08f;
            camera->DeltaY -= camera->DeltaY * 0.08f;
            camera->DeltaZ -= camera->DeltaZ * 0.08f;
        }
        camera->Roll += camera->DeltaRoll * shakeScale;
        func_150A7DF0(&rotMtx, -camera->DeltaPitch * shakeScale, camera->DeltaYaw * shakeScale, camera->DeltaRoll * shakeScale);
        guMtxCatL(VIEW_MTX(camera->CNumber), &rotMtx, VIEW_MTX(camera->CNumber));
        if (camera->mv != NULL) {
            yaw = func_15048FC8(&camera->mv->aimDir) - 180.0f;
            func_150A8050(dirMtx,
                          -func_150484A0(camera->mv->aimDir.y,
                                         sqrtf(camera->mv->aimDir.x * camera->mv->aimDir.x +
                                               camera->mv->aimDir.z * camera->mv->aimDir.z)) *
                                  57.2957764f - // 180/pi
                              camera->DeltaPitch * shakeScale,
                          yaw - camera->DeltaYaw * shakeScale, 0.0f);
            guMtxXFMF(dirMtx, 0.0f, 0.0f, 1.0f, &dirX, &dirY, &dirZ);
            camera->mv->aimDir.x = dirX;
            camera->mv->aimDir.y = dirY;
            camera->mv->aimDir.z = dirZ;
        }
        if (camera->mv->SniperState != 0) {
            func_150A7DF0(&rotMtx, gCameraOffsets[camera->CNumber].rot.x, gCameraOffsets[camera->CNumber].rot.y,
                          gCameraOffsets[camera->CNumber].rot.z);
            func_150A7D00(&transMtx, gCameraOffsets[camera->CNumber].pos.x, gCameraOffsets[camera->CNumber].pos.y,
                          gCameraOffsets[camera->CNumber].pos.z);
            guMtxCatL(&rotMtx, &transMtx, &rotMtx);
            guMtxCatL(VIEW_MTX(camera->CNumber), &rotMtx, VIEW_MTX(camera->CNumber));
            return;
        }
        if (((camera->Mode & 0x80) || camera->CharPtr->unk102 != 0) && camera->CharMoved != 0) {
            swayTarget = camera->CharPtr->lean_angle * 0.25f;
        } else if ((camera->SpecialFlags & 0x80) && camera->Mode != 0x40) {
            swayTarget = camera->CharPtr->lean_angle * 0.5f;
        } else {
            swayTarget = 0.0f;
        }
        if (!(camera->Mode & 0x40000)) {
            if ((camera->SpecialFlags & 0x80) || camera->CharPtr->unk102 != 0) {
                stiffness = 2.0f;
                damping = 4.0f;
            } else {
                stiffness = 4.0f;
                damping = 8.0f;
            }
            func_150495B0(&camera->Roll, swayTarget, &camera->RollVel, stiffness, damping, camera->SpringTime);
        }
        func_1512D368(camera);
        if (camera->Mode != 0x8000) {
            if (camera->Mode == 1) {
                camera->CameraPitch -= camera->ClampYAngle;
                camera->CameraPitchRad = camera->CameraPitch * 0.0174532924f; // pi/180
            }
            func_150A7DF0(&rotMtx,
                          gCameraOffsets[camera->CNumber].rot.x + (camera->ClampYAngle + camera->APitchAngle + camera->AddOnPitch),
                          gCameraOffsets[camera->CNumber].rot.y + camera->YawAngle,
                          camera->Roll + gViewEnvelopes[camera->CNumber].output[3] * 0.5f +
                              gCameraOffsets[camera->CNumber].rot.z);
            if (camera->Mode == 0x40000) {
                func_150A7DF0(&rotMtx, 0.0f, 0.0f, camera->Roll);
            }
        }
        guMtxCatL(VIEW_MTX(camera->CNumber), &rotMtx, VIEW_MTX(camera->CNumber));
        camera_applyBreathing(camera);
        if (!(camera->Mode & 0x40000)) {
            func_150A7D00(&transMtx, gViewEnvelopes[camera->CNumber].output[0] + gCameraOffsets[camera->CNumber].pos.x,
                          gViewEnvelopes[camera->CNumber].output[1] + gCameraOffsets[camera->CNumber].pos.y,
                          gViewEnvelopes[camera->CNumber].output[2] + gCameraOffsets[camera->CNumber].pos.z);
            guMtxCatL(VIEW_MTX(camera->CNumber), &transMtx, VIEW_MTX(camera->CNumber));
        }
    }
}

/* Applies the camera's breathing wobble: a scale on x and y that oscillates with wobblePhase. */
void camera_applyBreathing(Camera *camera) {
    Mtx mtx;
    MtxF mf;
    f32 amount;

    if (camera->Mode & 0x80000) {
        amount = 0.003000000026f;
    } else if (camera->CharMoved != 0) {
        amount = 0.03999999911f;
    } else {
        amount = 0.02999999933f;
    }

    if ((camera->Mode & 0x80000) || (camera->SpecialFlags & 1) || (camera->Mode != 0x40000 && (camera->SpecialFlags & 8))) {
        camera->WaterDx += 0.04363323003f * D_800BE9E4; // 2.5 degrees per frame
        if (camera->WaterDx > 3.141592741f) {
            camera->WaterDx -= 6.283185482f;
        }
    } else {
        amount = 0.0f;
    }

    if (camera->Snap != 0 || (camera->SpecialFlags & 4)) {
        camera->WaterEffectTargetAmp = amount;
    } else {
        func_150495B0(&camera->WaterEffectTargetAmp, amount, &camera->WaterEffectVel, 0.2f, 0.400000006f, camera->SpringTime);
    }

    guMtxIdentF(mf);
    mf[0][0] = func_150AD78C(camera->WaterDx) * camera->WaterEffectTargetAmp * 2.0f + (camera->WaterEffectTargetAmp + camera->WaterEffectTargetAmp) + 1.0f;
    mf[1][1] = func_150AD780(camera->WaterDx) * camera->WaterEffectTargetAmp + camera->WaterEffectTargetAmp + 1.0f;
    guMtxF2L(mf, &mtx);
    guMtxCatL(VIEW_MTX(camera->CNumber), &mtx, VIEW_MTX(camera->CNumber));
}

/* Loads the four camera shake tables. */
void camera_loadShakeTables(void) {
    u32 size;
    s32 i;

    for (i = 0; i < 4; i++) {
        gCameraShakeTables[i] = rom_loadPath(&size, 2, 0x1B, i);
        gCameraShakeLengths[i] = size / sizeof(CamOffset);
    }
}

/* Starts camera shake number `index`. */
void camera_startShake(Camera *camera, s32 index) {
    camera->DeltaPlaying = index;
    camera->DoingDeltas = 1;
}

/* Advances the camera shake by the frames elapsed, stopping at the end of its table. */
void camera_updateShake(Camera *camera) {
    u8 frame;

    switch (camera->DoingDeltas) {
    case 1:
        camera->DeltaCurrentIndex = 0;
        camera->DoingDeltas = 2;
        break;
    case 2:
        frame = camera->DeltaCurrentIndex + D_800BE9E4;
        camera->DeltaCurrentIndex = frame;
        // matching: the explicit mask makes IDO store the unmasked sum and re-mask a copy
        if ((frame & 0xFF) >= gCameraShakeLengths[camera->DeltaPlaying]) {
            camera->DoingDeltas = 0;
        }
        break;
    }
}

void func_1512D368(Camera *camera) {
}
