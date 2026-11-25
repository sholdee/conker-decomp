#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Rebuilds bone matrix `bone` of actor `actorIndex` as a look-at (func_150440A0) along the line
 * from the bone's position to camera_global_Cameras's position (turned 90 degrees about Y when `quarterTurn`
 * is set), keeping the bone's translation, then reapplies the actor's XZ and Y scale.
 */
void func_15036310(s32 unused, s32 actorIndex, s32 bone, s32 quarterTurn) {
    f32 (*mtx)[4];
    f32 up[3];
    f32 position[3];
    f32 dir[3];
    f32 side[3];
    Actor *actor;
    f32 swap;

    actor = &movement_obj[actorIndex];
    if (actor->ob_matrix_ptr != NULL) {
        mtx = actor->ob_matrix_ptr[bone];
        position[0] = mtx[3][0];
        position[1] = mtx[3][1];
        position[2] = mtx[3][2];
        dir[0] = camera_global_Cameras->eye.x - position[0];
        dir[1] = camera_global_Cameras->eye.y - position[1];
        dir[2] = camera_global_Cameras->eye.z - position[2];
        if (quarterTurn) {
            swap = dir[0];
            dir[0] = dir[2];
            dir[2] = -swap;
        }
        up[0] = 0.0f;
        up[1] = 1.0f;
        up[2] = 0.0f;
        side[0] = (dir[1] * up[2]) - (dir[2] * up[1]);
        side[1] = (dir[2] * up[0]) - (dir[0] * up[2]);
        side[2] = (dir[0] * up[1]) - (dir[1] * up[0]);
        up[0] = (dir[1] * side[2]) - (dir[2] * side[1]);
        up[1] = (dir[2] * side[0]) - (dir[0] * side[2]);
        up[2] = (dir[0] * side[1]) - (dir[1] * side[0]);

        func_150440A0(mtx, position[0], position[1], position[2],
                      position[0] - dir[0], position[1] - dir[1], position[2] - dir[2],
                      up[0], up[1], up[2]);
        if (actor->scale_xz != 1.0f) {
            mtx[0][0] *= actor->scale_xz;
            mtx[1][0] *= actor->scale_xz;
            mtx[2][0] *= actor->scale_xz;
            mtx[0][2] *= actor->scale_xz;
            mtx[1][2] *= actor->scale_xz;
            mtx[2][2] *= actor->scale_xz;
        }
        if (actor->scale_y != 1.0f) {
            mtx[0][1] *= actor->scale_y;
            mtx[1][1] *= actor->scale_y;
            mtx[2][1] *= actor->scale_y;
        }
    }
}
