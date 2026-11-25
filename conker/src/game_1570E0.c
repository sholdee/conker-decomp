#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Per-camera accumulated speeds for the free camera, indexed by Camera.playerIndex. */
extern f32 D_80089560[]; /* vertical (C-up / C-down) */
extern f32 D_80089570[]; /* strafe or roll (C-left / C-right) */
extern f32 D_80089580[]; /* pitch (A / B) */

void func_1510E7A4(s32 *, s32 *, f32 *, f32 *, s32 *, s32, f32, f32, f32, f32, u16, s32, f32, f32);
void func_151E6C1C(s32 index);

/*
 * Free-fly camera. D-pad down toggles it (flag 0x80000000 in flags means paused). While active, the
 * stick turns (x) and moves forward/back (y), C-up/C-down move vertically, A/B pitch, and
 * C-left/C-right strafe, or roll while Z is held. The camera looks 600 units ahead.
 */
void func_15129C30(Camera *cam) {
    f32 dirX;
    f32 lookY;
    f32 dirZ;
    f32 forward;
    f32 strafe;
    f32 stickX;
    f32 stickY;
    struct17 floor; // matching: frame size needs 12 bytes here although only .unk0 is written
    u8 index;
    u16 buttons;
    f32 f;

    strafe = 0.0f;
    index = cam->playerIndex;
    buttons = cam->contPad->button;
    if (cam->buttonsPressed & D_JPAD) {
        if (cam->status & 0x80000000) {
            cam->status &= ~0x80000000;
        } else {
            cam->status |= 0x80000000;
        }
    }
    if (!(cam->status & 0x80000000)) {
        // Stick dead zone of +-4.
        if (cam->contPad->stick_x < -4 || cam->contPad->stick_x >= 5) {
            stickX = cam->contPad->stick_x;
        } else {
            stickX = 0.0f;
        }
        if (cam->contPad->stick_y < -4 || cam->contPad->stick_y >= 5) {
            stickY = cam->contPad->stick_y;
        } else {
            stickY = 0.0f;
        }

        f = stickX / 80.0f;
        if (f > 0.0f) {
            cam->yaw -= f * f * D_800BEA08;
        } else {
            cam->yaw += f * f * D_800BEA08;
        }
        cam->yawRad = cam->yaw * 0.01745329238f;

        f = (stickY / 80.0f) * 7.0f;
        if (f > 0.0f) {
            forward = f * f * D_800BEA08;
        } else {
            forward = -f * f * D_800BEA08;
        }

        if (buttons & D_CBUTTONS) {
            D_80089560[index] += 0.200000003f;
            cam->eye.y -= D_80089560[index] * D_800BEA08;
        } else if (buttons & U_CBUTTONS) {
            D_80089560[index] += 0.200000003f;
            cam->eye.y += D_80089560[index] * D_800BEA08;
        } else {
            D_80089560[index] = 0.0f;
        }

        if (buttons & A_BUTTON) {
            D_80089580[index] += 0.01250000019f;
            cam->pitch -= D_80089580[index] * D_800BEA08;
        } else if (buttons & B_BUTTON) {
            D_80089580[index] += 0.01250000019f;
            cam->pitch += D_80089580[index] * D_800BEA08;
        } else {
            D_80089580[index] = 0.0f;
        }
        if (cam->pitch < -89.5f) {
            cam->pitch = -89.5f;
        } else {
            // matching: clamp through the temp f; a plain `else if` store or direct ternary mismatches
            f = (cam->pitch > 89.5f) ? 89.5f : cam->pitch;
            cam->pitch = f;
        }
        cam->pitchRad = cam->pitch * 0.01745329238f;

        if (buttons & Z_TRIG) {
            if ((buttons & L_CBUTTONS) && (buttons & Z_TRIG)) {
                D_80089570[index] += 0.200000003f;
                cam->swayAmount -= D_80089570[index] * 0.1000000015f;
            } else if ((buttons & R_CBUTTONS) && (buttons & Z_TRIG)) {
                D_80089570[index] += 0.200000003f;
                cam->swayAmount += D_80089570[index] * 0.1000000015f;
            } else {
                D_80089570[index] = 0.0f;
            }
        } else {
            if (buttons & L_CBUTTONS) {
                D_80089570[index] += 0.400000006f;
                strafe -= D_80089570[index];
            } else if (buttons & R_CBUTTONS) {
                D_80089570[index] += 0.400000006f;
                strafe += D_80089570[index];
            } else {
                D_80089570[index] = 0.0f;
            }
        }

        D_80089570[index] = (D_80089570[index] > 100.0f) ? 100.0f : D_80089570[index];
        D_80089580[index] = (D_80089580[index] > 8.0f) ? 8.0f : D_80089580[index];
        D_80089560[index] = (D_80089560[index] > 40.0f) ? 40.0f : D_80089560[index];
        if (cam->swayAmount < -45.0f) {
            cam->swayAmount = -45.0f;
        } else {
            f = (cam->swayAmount > 45.0f) ? 45.0f : cam->swayAmount;
            cam->swayAmount = f;
        }

        dirX = -sinf(cam->yawRad);
        dirZ = -cosf(cam->yawRad);
        cam->eye.x += dirX * forward;
        cam->eye.z += dirZ * forward;
        cam->eye.x += -dirZ * strafe;
        cam->eye.z += dirX * strafe;
        cam->unk344 = 0.0f;

        dirX *= 600.0f;
        dirZ *= 600.0f;
        f = sqrtf(dirX * dirX + dirZ * dirZ);
        lookY = sinf(cam->pitchRad) * f;
        dirX = cosf(cam->pitchRad) * dirX;
        dirZ = cosf(cam->pitchRad) * dirZ;
        // matching: the look point reuses stickX/stickY; separate locals change register allocation
        stickX = cam->eye.x + dirX;
        cam->focus.x = stickX;
        stickY = cam->eye.y + lookY;
        cam->target.x = stickX;
        f = cam->eye.z + dirZ;
        cam->unk390 = 0.0f;
        cam->focus.z = cam->target.z = f;
        cam->focus.y = cam->target.y = stickY;
        cam->focus = cam->target;
    } else {
        cam->unk394 = cam->yaw;
    }
    cam->unk380 = cam->yaw;
    cam->unk3A0 = cam->yawRad = cam->yaw * 0.01745329238f;
    func_1510E7A4(&cam->unk644, &cam->unk648, &floor.unk0, &cam->unk360, &cam->unk640, 0, cam->eye.x, cam->eye.y,
                  cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
}

/* Runs func_151E6C1C for this camera's slot. */
void func_1512A360(Camera *cam) {
    func_151E6C1C(cam->playerIndex);
}
