#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Writes to *out the world position of a fixed offset from one of the actor's bones, chosen by
 * point (3..6). */
s32 attach_getBonePoint(Vec3f *out, Actor *actor, u8 point) {
    s32 bone;
    f32 x;
    f32 y;
    f32 z;

    switch (point) {
        case 3:
            bone = 18;
            x = -7.0f;
            z = 30.0f;
            break;
        case 4:
            bone = 21;
            x = -4.0f;
            z = 35.0f;
            break;
        case 5:
            bone = 15;
            x = 5.0f;
            z = 20.0f;
            break;
        case 6:
            bone = 24;
            x = 5.0f;
            z = 31.0f;
            break;
    }
    y = 0.0f;
    func_150A7960(actor->ob_matrix_ptr[bone], x, y, z, &x, &y, &z);
    out->x = x;
    out->y = y;
    out->z = z;
    return 1;
}

s32 func_150D8A20(s32 arg0, s32 arg1) {
    return 8;
}

/* Spawns two effects at bone point `point` of the actor (see attach_getBonePoint), if it has bones. */
void attach_spawnBonePointEffects(Actor *actor, s32 point, s32 arg2) {
    Vec3f pos;

    if (actor->ob_matrix_ptr != NULL) {
        attach_getBonePoint(&pos, actor, point);
        func_151875E0(pos.x, pos.y, pos.z, 30, 15, 7, 0.1000000015f, 0.02999999933f);
        func_15165F80(-1, pos.x, actor->floorY + 4.0f, pos.z, 4, 50, 0, 255, 0);
    }
}
