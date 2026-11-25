#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    /* 0x0 */ u16 buttons;
    /* 0x2 */ s8 stickX;
    /* 0x3 */ s8 stickY;
} ControllerInput;

/*
 * Camera state: the same 0x9A0-byte object as struct108 (include/structs.h). Only the fields this
 * file uses are named.
 */
typedef struct {
    /* 0x000 */ u8 pad0[0x23D];
    /* 0x23D */ u8 index; /* this camera's slot in the per-camera tables */
    /* 0x23E */ u8 pad23E[0x66];
    /* 0x2A4 */ struct17 unk2A4;
    /* 0x2B0 */ u8 pad2B0[0xC];
    /* 0x2BC */ struct17 unk2BC;
    /* 0x2C8 */ u8 pad2C8[0x30];
    /* 0x2F8 */ f32 posX;
    /* 0x2FC */ f32 posY;
    /* 0x300 */ f32 posZ;
    /* 0x304 */ u8 pad304[0x40];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ u8 pad348[0x18];
    /* 0x360 */ f32 unk360;
    /* 0x364 */ u8 pad364[6];
    /* 0x36A */ u16 buttonsPressed;
    /* 0x36C */ ControllerInput *input;
    /* 0x370 */ u8 pad370[0xC];
    /* 0x37C */ f32 yaw; /* degrees */
    /* 0x380 */ f32 unk380;
    /* 0x384 */ u8 pad384[4];
    /* 0x388 */ f32 pitch; /* degrees */
    /* 0x38C */ u8 pad38C[4];
    /* 0x390 */ f32 unk390;
    /* 0x394 */ f32 unk394;
    /* 0x398 */ f32 pitchRad;
    /* 0x39C */ f32 yawRad;
    /* 0x3A0 */ f32 unk3A0;
    /* 0x3A4 */ u8 pad3A4[0x248];
    /* 0x5EC */ f32 roll; /* degrees */
    /* 0x5F0 */ s32 flags;
    /* 0x5F4 */ u8 pad5F4[0x4C];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
} Camera;

/* Per-camera accumulated speeds for the free camera, indexed by Camera.index. */
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
    struct17 floor;
    u8 index;
    u16 buttons;
    f32 f;

    strafe = 0.0f;
    index = cam->index;
    buttons = cam->input->buttons;
    if (cam->buttonsPressed & D_JPAD) {
        if (cam->flags & 0x80000000) {
            cam->flags &= ~0x80000000;
        } else {
            cam->flags |= 0x80000000;
        }
    }
    if (!(cam->flags & 0x80000000)) {
        // Stick dead zone of +-4.
        if (cam->input->stickX < -4 || cam->input->stickX >= 5) {
            stickX = cam->input->stickX;
        } else {
            stickX = 0.0f;
        }
        if (cam->input->stickY < -4 || cam->input->stickY >= 5) {
            stickY = cam->input->stickY;
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
            cam->posY -= D_80089560[index] * D_800BEA08;
        } else if (buttons & U_CBUTTONS) {
            D_80089560[index] += 0.200000003f;
            cam->posY += D_80089560[index] * D_800BEA08;
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
            f = (cam->pitch > 89.5f) ? 89.5f : cam->pitch;
            cam->pitch = f;
        }
        cam->pitchRad = cam->pitch * 0.01745329238f;

        if (buttons & Z_TRIG) {
            if ((buttons & L_CBUTTONS) && (buttons & Z_TRIG)) {
                D_80089570[index] += 0.200000003f;
                cam->roll -= D_80089570[index] * 0.1000000015f;
            } else if ((buttons & R_CBUTTONS) && (buttons & Z_TRIG)) {
                D_80089570[index] += 0.200000003f;
                cam->roll += D_80089570[index] * 0.1000000015f;
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
        if (cam->roll < -45.0f) {
            cam->roll = -45.0f;
        } else {
            f = (cam->roll > 45.0f) ? 45.0f : cam->roll;
            cam->roll = f;
        }

        dirX = -sinf(cam->yawRad);
        dirZ = -cosf(cam->yawRad);
        cam->posX += dirX * forward;
        cam->posZ += dirZ * forward;
        cam->posX += -dirZ * strafe;
        cam->posZ += dirX * strafe;
        cam->unk344 = 0.0f;

        dirX *= 600.0f;
        dirZ *= 600.0f;
        f = sqrtf(dirX * dirX + dirZ * dirZ);
        lookY = sinf(cam->pitchRad) * f;
        dirX = cosf(cam->pitchRad) * dirX;
        dirZ = cosf(cam->pitchRad) * dirZ;
        stickX = cam->posX + dirX;
        cam->unk2A4.unk0 = stickX;
        stickY = cam->posY + lookY;
        cam->unk2BC.unk0 = stickX;
        f = cam->posZ + dirZ;
        cam->unk390 = 0.0f;
        cam->unk2A4.unk8 = cam->unk2BC.unk8 = f;
        cam->unk2A4.unk4 = cam->unk2BC.unk4 = stickY;
        cam->unk2A4 = cam->unk2BC;
    } else {
        cam->unk394 = cam->yaw;
    }
    cam->unk380 = cam->yaw;
    cam->unk3A0 = cam->yawRad = cam->yaw * 0.01745329238f;
    func_1510E7A4(&cam->unk644, &cam->unk648, &floor.unk0, &cam->unk360, &cam->unk640, 0, cam->posX, cam->posY,
                  cam->posZ, cam->posY, 0, 0, -10000.0f, cam->posY);
}

/* Runs func_151E6C1C for this camera's slot. */
void func_1512A360(Camera *cam) {
    func_151E6C1C(cam->index);
}
