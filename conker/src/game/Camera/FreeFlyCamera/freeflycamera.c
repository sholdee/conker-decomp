#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Per-camera accumulated speeds for the free camera, indexed by Camera.CNumber. */
static f32 gFreeFlyVertical[4] = { 0.0f }; /* vertical (C-up / C-down) */
static f32 gFreeFlyStrafe[4] = { 0.0f }; /* strafe or roll (C-left / C-right) */
static f32 gFreeFlyPitch[4] = { 0.0f }; /* pitch (A / B) */

/*
 * Free-fly camera. D-pad down toggles it (flag 0x80000000 in flags means paused). While active, the
 * stick turns (x) and moves forward/back (y), C-up/C-down move vertically, A/B pitch, and
 * C-left/C-right strafe, or roll while Z is held. The camera looks 600 units ahead.
 */
void camera_updateFreeFly(Camera *cam) {
    f32 dirX;
    f32 lookY;
    f32 dirZ;
    f32 forward;
    f32 strafe;
    f32 stickX;
    f32 stickY;
    Vec3f floor; // matching: frame size needs 12 bytes here although only .unk0 is written
    u8 index;
    u16 buttons;
    f32 f;

    strafe = 0.0f;
    index = cam->CNumber;
    buttons = cam->PadPtr->button;
    if (cam->CameraButtons & D_JPAD) {
        if (cam->SpecialFlags & 0x80000000) {
            cam->SpecialFlags &= ~0x80000000;
        } else {
            cam->SpecialFlags |= 0x80000000;
        }
    }
    if (!(cam->SpecialFlags & 0x80000000)) {
        // Stick dead zone of +-4.
        if (cam->PadPtr->stick_x < -4 || cam->PadPtr->stick_x >= 5) {
            stickX = cam->PadPtr->stick_x;
        } else {
            stickX = 0.0f;
        }
        if (cam->PadPtr->stick_y < -4 || cam->PadPtr->stick_y >= 5) {
            stickY = cam->PadPtr->stick_y;
        } else {
            stickY = 0.0f;
        }

        f = stickX / 80.0f;
        if (f > 0.0f) {
            cam->CameraAngle -= f * f * D_800BEA08;
        } else {
            cam->CameraAngle += f * f * D_800BEA08;
        }
        cam->CameraAngleRad = cam->CameraAngle * 0.01745329238f;

        f = (stickY / 80.0f) * 7.0f;
        if (f > 0.0f) {
            forward = f * f * D_800BEA08;
        } else {
            forward = -f * f * D_800BEA08;
        }

        if (buttons & D_CBUTTONS) {
            gFreeFlyVertical[index] += 0.200000003f;
            cam->eye.y -= gFreeFlyVertical[index] * D_800BEA08;
        } else if (buttons & U_CBUTTONS) {
            gFreeFlyVertical[index] += 0.200000003f;
            cam->eye.y += gFreeFlyVertical[index] * D_800BEA08;
        } else {
            gFreeFlyVertical[index] = 0.0f;
        }

        if (buttons & A_BUTTON) {
            gFreeFlyPitch[index] += 0.01250000019f;
            cam->CameraPitch -= gFreeFlyPitch[index] * D_800BEA08;
        } else if (buttons & B_BUTTON) {
            gFreeFlyPitch[index] += 0.01250000019f;
            cam->CameraPitch += gFreeFlyPitch[index] * D_800BEA08;
        } else {
            gFreeFlyPitch[index] = 0.0f;
        }
        if (cam->CameraPitch < -89.5f) {
            cam->CameraPitch = -89.5f;
        } else {
            // matching: clamp through the temp f; a plain `else if` store or direct ternary mismatches
            f = (cam->CameraPitch > 89.5f) ? 89.5f : cam->CameraPitch;
            cam->CameraPitch = f;
        }
        cam->CameraPitchRad = cam->CameraPitch * 0.01745329238f;

        if (buttons & Z_TRIG) {
            if ((buttons & L_CBUTTONS) && (buttons & Z_TRIG)) {
                gFreeFlyStrafe[index] += 0.200000003f;
                cam->Roll -= gFreeFlyStrafe[index] * 0.1000000015f;
            } else if ((buttons & R_CBUTTONS) && (buttons & Z_TRIG)) {
                gFreeFlyStrafe[index] += 0.200000003f;
                cam->Roll += gFreeFlyStrafe[index] * 0.1000000015f;
            } else {
                gFreeFlyStrafe[index] = 0.0f;
            }
        } else {
            if (buttons & L_CBUTTONS) {
                gFreeFlyStrafe[index] += 0.400000006f;
                strafe -= gFreeFlyStrafe[index];
            } else if (buttons & R_CBUTTONS) {
                gFreeFlyStrafe[index] += 0.400000006f;
                strafe += gFreeFlyStrafe[index];
            } else {
                gFreeFlyStrafe[index] = 0.0f;
            }
        }

        gFreeFlyStrafe[index] = (gFreeFlyStrafe[index] > 100.0f) ? 100.0f : gFreeFlyStrafe[index];
        gFreeFlyPitch[index] = (gFreeFlyPitch[index] > 8.0f) ? 8.0f : gFreeFlyPitch[index];
        gFreeFlyVertical[index] = (gFreeFlyVertical[index] > 40.0f) ? 40.0f : gFreeFlyVertical[index];
        if (cam->Roll < -45.0f) {
            cam->Roll = -45.0f;
        } else {
            f = (cam->Roll > 45.0f) ? 45.0f : cam->Roll;
            cam->Roll = f;
        }

        dirX = -sinf(cam->CameraAngleRad);
        dirZ = -cosf(cam->CameraAngleRad);
        cam->eye.x += dirX * forward;
        cam->eye.z += dirZ * forward;
        cam->eye.x += -dirZ * strafe;
        cam->eye.z += dirX * strafe;
        cam->unk344 = 0.0f;

        dirX *= 600.0f;
        dirZ *= 600.0f;
        f = sqrtf(dirX * dirX + dirZ * dirZ);
        lookY = sinf(cam->CameraPitchRad) * f;
        dirX = cosf(cam->CameraPitchRad) * dirX;
        dirZ = cosf(cam->CameraPitchRad) * dirZ;
        // matching: the look point reuses stickX/stickY; separate locals change register allocation
        stickX = cam->eye.x + dirX;
        cam->focus.x = stickX;
        stickY = cam->eye.y + lookY;
        cam->target.x = stickX;
        f = cam->eye.z + dirZ;
        cam->CharRelAngle = 0.0f;
        cam->focus.z = cam->target.z = f;
        cam->focus.y = cam->target.y = stickY;
        cam->focus = cam->target;
    } else {
        cam->FCameraAngle = cam->CameraAngle;
    }
    cam->TrueCameraAngle = cam->CameraAngle;
    cam->TrueCameraAngleRad = cam->CameraAngleRad = cam->CameraAngle * 0.01745329238f;
    collision_findFloorInRange(&cam->TrianglePtr, cam->floorTri.v, &floor.x, &cam->waterY, &cam->Attributes, 0, cam->eye.x, cam->eye.y,
                  cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
}

/* Runs func_151E6C1C for this camera's slot. */
void func_1512A360(Camera *cam) {
    func_151E6C1C(cam->CNumber);
}
