#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150440A0(f32 mtx[4][4], f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);

/*
 * Rebuilds bone matrix `bone` of actor `actorIndex` as a look-at (func_150440A0) along the line
 * from the bone's position to D_800DBFF0's position (turned 90 degrees about Y when `quarterTurn`
 * is set), keeping the bone's translation, then reapplies the actor's XZ and Y scale.
 */
void func_15036310(s32 unused, s32 actorIndex, s32 bone, s32 quarterTurn) {
    f32 (*mtx)[4];
    f32 up[3];
    f32 position[3];
    f32 dir[3];
    f32 side[3];
    struct127 *actor;
    f32 swap;

    actor = &D_800CC2D0[actorIndex];
    if (actor->unk1D4 != NULL) {
        mtx = ((f32 (*)[4][4])actor->unk1D4)[bone];
        position[0] = mtx[3][0];
        position[1] = mtx[3][1];
        position[2] = mtx[3][2];
        dir[0] = D_800DBFF0->eye.x - position[0];
        dir[1] = D_800DBFF0->eye.y - position[1];
        dir[2] = D_800DBFF0->eye.z - position[2];
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
        if (actor->xz_scale != 1.0f) {
            mtx[0][0] *= actor->xz_scale;
            mtx[1][0] *= actor->xz_scale;
            mtx[2][0] *= actor->xz_scale;
            mtx[0][2] *= actor->xz_scale;
            mtx[1][2] *= actor->xz_scale;
            mtx[2][2] *= actor->xz_scale;
        }
        if (actor->y_scale != 1.0f) {
            mtx[0][1] *= actor->y_scale;
            mtx[1][1] *= actor->y_scale;
            mtx[2][1] *= actor->y_scale;
        }
    }
}
