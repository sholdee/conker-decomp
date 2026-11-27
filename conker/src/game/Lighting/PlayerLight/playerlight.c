#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/math.h"

static f32 gPlayerLightPitch[4]; // per-player smoothed light pitch (degrees, -180..180)
static f32 gPlayerLightYaw[4]; // per-player smoothed light yaw (degrees, 0..360)

/*
 * Eases player's directional light towards the camera's pitch (unk388) and yaw (unk380),
 * keeping the light within 90 degrees of the camera yaw, and stores it as an s8 unit vector.
 */
void light_easePlayerLight(s32 player) {
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
        } else if (*(s32 *)gCameraSnapTimers != 0) { // any of the four per-player bytes set
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
        diff = pitch - gPlayerLightPitch[player];
        gPlayerLightPitch[player] += diff * rate;
        diff = math_angleDiff(gPlayerLightYaw[player], yaw);
        gPlayerLightYaw[player] += rate * diff;
        if (gPlayerLightYaw[player] < 0.0f) {
            gPlayerLightYaw[player] += 360.0f;
        } else if (gPlayerLightYaw[player] >= 360.0f) {
            gPlayerLightYaw[player] -= 360.0f;
        }
        diff = math_angleDiff(gPlayerLightYaw[player], yaw);
        if (fabsf(diff) > 90.0f) {
            gPlayerLightYaw[player] = yaw - ((diff < 0) ? -1 : 1) * 90.0f;
        }
        if (gPlayerLightYaw[player] < 0.0f) {
            gPlayerLightYaw[player] += 360.0f;
        } else if (gPlayerLightYaw[player] >= 360.0f) {
            gPlayerLightYaw[player] -= 360.0f;
        }
        angle = gPlayerLightPitch[player] * 0.01745329238f;
        y = -sinf(angle);
        horiz = cosf(angle);
        angle = gPlayerLightYaw[player] * 0.01745329238f;
        x = sinf(angle) * horiz;
        horiz = cosf(angle) * horiz; // matching: z reuses horiz; a separate local changes regalloc
        gPlayerLightDirs[player][0] = x * 127.0f;
        gPlayerLightDirs[player][1] = y * 127.0f;
        gPlayerLightDirs[player][2] = horiz * 127.0f;
    }
}

void light_updatePlayerLight(s32 player) {
    light_easePlayerLight(player);
}
