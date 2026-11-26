#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static f32 D_800BE6C0[4]; // per-player smoothed light pitch (degrees, -180..180)
static f32 D_800BE6D0[4]; // per-player smoothed light yaw (degrees, 0..360)

/*
 * Eases player's directional light towards the camera's pitch (unk388) and yaw (unk380),
 * keeping the light within 90 degrees of the camera yaw, and stores it as an s8 unit vector.
 */
void func_1501B320(s32 player) {
    f32 x;
    f32 z; // matching: unused, but IDO allocates differently without it
    f32 angle;
    f32 rate;
    f32 pitch;
    f32 y;
    f32 horiz;
    f32 diff;
    f32 yaw;

    if (gPaused == 0) {
        if (camera_global_Cameras[player].unk698 != 0) {
            rate = 0.1000000015f;
        } else if (*(s32 *)D_800DBFF4 != 0) { // any of the four per-player bytes set
            rate = 1.0f;
        } else {
            rate = 0.02999999933f;
        }
        pitch = camera_global_Cameras[player].CameraPitch;
        yaw = camera_global_Cameras[player].TrueCameraAngle;
        if (pitch < 0.0f) {
            pitch += 360.0f;
        } else if (pitch >= 360.0f) {
            pitch -= 360.0f;
        }
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        diff = pitch - D_800BE6C0[player];
        D_800BE6C0[player] += diff * rate;
        diff = func_15048A70(D_800BE6D0[player], yaw);
        D_800BE6D0[player] += rate * diff;
        if (D_800BE6D0[player] < 0.0f) {
            D_800BE6D0[player] += 360.0f;
        } else if (D_800BE6D0[player] >= 360.0f) {
            D_800BE6D0[player] -= 360.0f;
        }
        diff = func_15048A70(D_800BE6D0[player], yaw);
        if (fabsf(diff) > 90.0f) {
            D_800BE6D0[player] = yaw - ((diff < 0) ? -1 : 1) * 90.0f;
        }
        if (D_800BE6D0[player] < 0.0f) {
            D_800BE6D0[player] += 360.0f;
        } else if (D_800BE6D0[player] >= 360.0f) {
            D_800BE6D0[player] -= 360.0f;
        }
        angle = D_800BE6C0[player] * 0.01745329238f;
        y = -sinf(angle);
        horiz = cosf(angle);
        angle = D_800BE6D0[player] * 0.01745329238f;
        x = sinf(angle) * horiz;
        horiz = cosf(angle) * horiz; // matching: z reuses horiz; a separate local changes regalloc
        D_800DCD30[player][0] = x * 127.0f;
        D_800DCD30[player][1] = y * 127.0f;
        D_800DCD30[player][2] = horiz * 127.0f;
    }
}

void func_1501B640(s32 player) {
    func_1501B320(player);
}
