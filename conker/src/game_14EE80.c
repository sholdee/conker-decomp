#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Frames the first D_8008FD8C objects of D_800CC2D0: the camera looks at their max X, mean Y (+150) and
 * mid Z, and backs off along +X/+Y by the largest Z spread from the middle (at least 300, plus 100).
 * unk2BC..unk2C4 is the look-at point, unk2F8..unk300 the eye position.
 */
void func_151219D0(struct108 *camera) {
    s32 i;
    f32 minX, maxX, minZ, maxZ, sumY;
    f32 dx, dz, maxDist;
    f32 midZ;
    f32 x, z;

    minZ = 10000.0f;
    maxZ = -10000.0f;
    minX = 10000.0f;
    maxX = -10000.0f;
    sumY = 0.0f;
    for (i = 0; i < D_8008FD8C; i++) {
        x = D_800CC2D0[i].x_position;
        if (x < minX) {
            minX = x;
        }
        if (maxX < x) {
            maxX = x;
        }
        z = D_800CC2D0[i].z_position;
        sumY += D_800CC2D0[i].y_position;
        if (z < minZ) {
            minZ = z;
        }
        if (maxZ < z) {
            maxZ = z;
        }
    }
    sumY /= D_8008FD8C;
    camera->unk2BC = maxX;
    camera->unk2C0 = sumY + 150.0f;
    maxDist = 0.0f;
    midZ = (minZ + maxZ) * 0.5f;
    camera->unk2C4 = midZ;
    for (i = 0; i < D_8008FD8C; i++) {
        dx = (D_800CC2D0[i].x_position - maxX) * 0.5f;
        if (dx < 0.0f) {
            dx = 0.0f;
        }
        dz = D_800CC2D0[i].z_position - midZ;
        if (dz < 0.0f) {
            dz = -dz;
        }
        if (dz < dx) {
            dz = dx;
        }
        if (maxDist < dz) {
            maxDist = dz;
        }
    }
    if (maxDist < 300.0f) {
        maxDist = 300; // matching: int literal stops IDO reusing the 300.0f compared above (target loads it twice)
    }
    maxDist += 100.0f;
    camera->unk2F8 = camera->unk2BC + (maxDist + maxDist);
    camera->unk2FC = camera->unk2C0 + (maxDist * 0.5f);
    camera->unk300 = camera->unk2C4;
}

/**
 * Springs the camera yaw (unk37C, degrees) towards targetYaw with func_15049688, using the f32 at 0x8C0
 * as its velocity and unk7B4 as the frame time, then stores the yaw in radians in unk39C.
 */
void func_15121C00(struct108 *camera, f32 targetYaw, s32 unused, f32 speedGain, f32 accelGain) {
    // pad8C0: the shared struct108 does not declare this f32 yaw velocity
    func_15049688(&camera->unk37C, targetYaw, (f32 *)&camera->pad8C0, speedGain, accelGain, camera->unk7B4);
    camera->unk39C = camera->unk37C * 0.01745329238f; // degrees to radians
}

/** Empty stub. */
void func_15121C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}
