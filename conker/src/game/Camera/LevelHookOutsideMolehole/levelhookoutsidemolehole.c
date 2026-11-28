#include <ultra64.h>
#include "conker/camera.h"
#include "conker/math.h"
#include "conker/script.h"

/* Four channels: the current and target values (255, or 192 while the gOutsideMoleholeQueryIds query holds), the
 * math_springTo spring velocity, the command words queried with script_query and those that receive
 * the current value through script_command. */
static f32 gOutsideMoleholeValues[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 gOutsideMoleholeTargets[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 gOutsideMoleholeVelocities[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static s32 gOutsideMoleholeQueryIds[4] = { 0x4039, 0x403A, 0x403B, 0x403C };
static s32 gOutsideMoleholeTargetIds[4] = { 0x30E4, 0x30E5, 0x30E6, 0x30E7 };

void camera_levelHookOutsideMolehole(Camera *camera) {
    s32 i;
    s32 flag;

    flag = 0;
    camera->AvailableModes |= 0x10;
    for (i = 0; i < 4; i++) {
        if (script_query(1, gOutsideMoleholeQueryIds[i], 6, 0x9000) != 0) {
            gOutsideMoleholeTargets[i] = 192.0f;
            camera->AvailableModes |= 0x80021010;
            flag = 1;
        } else {
            gOutsideMoleholeTargets[i] = 255.0f;
        }
        math_springTo(&gOutsideMoleholeValues[i], gOutsideMoleholeTargets[i], &gOutsideMoleholeVelocities[i], 4.0f, 6.0f, camera->SpringTime);
        script_command(1, gOutsideMoleholeTargetIds[i], 0x12, (u8)gOutsideMoleholeValues[i]);
    }
    if (flag == 0) {
        camera->AvailableModes &= ~0x80021000;
    }
    if (script_query(1, 0x403D, 6, 0x2000) != 0) {
        camera->AvailableModes &= ~0x80000000;
    } else {
        camera->AvailableModes |= 0x80000000;
    }
}
