#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Frames the first gPlayerCount objects of movement_obj: the camera looks at their max X, mean Y (+150) and
 * mid Z, and backs off along +X/+Y by the largest Z spread from the middle (at least 300, plus 100).
 * unk2BC..unk2C4 is the look-at point, unk2F8..unk300 the eye position.
 */
void camera_frameGroup(Camera *camera) {
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
    for (i = 0; i < gPlayerCount; i++) {
        x = movement_obj[i].xpos;
        if (x < minX) {
            minX = x;
        }
        if (maxX < x) {
            maxX = x;
        }
        z = movement_obj[i].zpos;
        sumY += movement_obj[i].ypos;
        if (z < minZ) {
            minZ = z;
        }
        if (maxZ < z) {
            maxZ = z;
        }
    }
    sumY /= gPlayerCount;
    camera->target.x = maxX;
    camera->target.y = sumY + 150.0f;
    maxDist = 0.0f;
    midZ = (minZ + maxZ) * 0.5f;
    camera->target.z = midZ;
    for (i = 0; i < gPlayerCount; i++) {
        dx = (movement_obj[i].xpos - maxX) * 0.5f;
        if (dx < 0.0f) {
            dx = 0.0f;
        }
        dz = movement_obj[i].zpos - midZ;
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
    camera->eye.x = camera->target.x + (maxDist + maxDist);
    camera->eye.y = camera->target.y + (maxDist * 0.5f);
    camera->eye.z = camera->target.z;
}
