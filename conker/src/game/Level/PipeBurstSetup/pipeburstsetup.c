#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "libc/string.h"

void level_initPipeBurstPoints(void) {
    s32 row;
    s16 yaw;
    s16 pitch;
    f32 yawCos;
    f32 yawSin;
    TimedEffect *result;
    Vec3f hit;
    f32 pitchCos;
    f32 radius;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    s16 timer[3];

    for (row = 0; row < 3; row++) {
        s32 i;

        for (i = 0; i < 10; i++) {
            yaw = gPipeSpots[row].yaw + math_random() % 13U - 0x86;
            pitch = (math_random() & 0xF) - 10;
            yawCos = math_cosBinAngleMasked(yaw - 0x40);
            yawSin = math_cosBinAngleMasked(yaw);
            pitchCos = math_cosBinAngleMasked(pitch - 0x40);
            radius = math_cosBinAngleMasked(pitch);
            radius = 10.0f * radius;
            dirX = radius * yawCos;
            dirY = -10.0f * pitchCos;
            dirZ = radius * yawSin;
            if (collision_castRayWorld(0.0f, 300.0f, 0.0f, dirX, dirY, dirZ, NULL, NULL,
                              &hit.x, &hit.y, &hit.z, NULL, NULL, NULL, 0.0f) == 0) {
                gPipeBurstPoints[row][i].pos.x = 0.0f;
                gPipeBurstPoints[row][i].pos.y = 0.0f;
                gPipeBurstPoints[row][i].pos.z = 0.0f;
            } else {
                gPipeBurstPoints[row][i].pos.x = hit.x * 0.97f;
                gPipeBurstPoints[row][i].pos.y = (hit.y - 300.0f) * 0.97f + 300.0f;
                gPipeBurstPoints[row][i].pos.z = hit.z * 0.97f;
            }
        }
    }
    timer[0] = 0x14;
    timer[1] = 0x32;
    timer[2] = 0;
    result = effect_newTimedEffectNoDraw(0x12C, -1, 7, 0, 0, 6, 0xFF, 0);
    if (result != NULL) {
        memcpy(result + 1, timer, sizeof(timer));
    }
}
