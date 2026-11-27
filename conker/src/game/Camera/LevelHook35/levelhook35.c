#include <ultra64.h>
#include "conker/camera.h"
#include "conker/math.h"
#include "conker/script.h"

/* Four channels: the current and target values (255, or 192 while the gLevelHook35QueryIds query holds), the
 * math_springTo spring velocity, the command words queried with script_query and those that receive
 * the current value through script_command. */
static f32 gLevelHook35Values[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 gLevelHook35Targets[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
static f32 gLevelHook35Velocities[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static s32 gLevelHook35QueryIds[4] = { 0x4039, 0x403A, 0x403B, 0x403C };
static s32 gLevelHook35TargetIds[4] = { 0x30E4, 0x30E5, 0x30E6, 0x30E7 };

void camera_levelHook35(Camera *arg0) {
    s32 i;
    s32 flag;

    flag = 0;
    arg0->AvailableModes |= 0x10;
    for (i = 0; i < 4; i++) {
        if (script_query(1, gLevelHook35QueryIds[i], 6, 0x9000) != 0) {
            gLevelHook35Targets[i] = 192.0f;
            arg0->AvailableModes |= 0x80021010;
            flag = 1;
        } else {
            gLevelHook35Targets[i] = 255.0f;
        }
        math_springTo(&gLevelHook35Values[i], gLevelHook35Targets[i], &gLevelHook35Velocities[i], 4.0f, 6.0f, arg0->SpringTime);
        script_command(1, gLevelHook35TargetIds[i], 0x12, (u8)gLevelHook35Values[i]);
    }
    if (flag == 0) {
        arg0->AvailableModes &= ~0x80021000;
    }
    if (script_query(1, 0x403D, 6, 0x2000) != 0) {
        arg0->AvailableModes &= ~0x80000000;
    } else {
        arg0->AvailableModes |= 0x80000000;
    }
}
