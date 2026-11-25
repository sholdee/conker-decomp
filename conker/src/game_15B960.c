#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 D_800A3730; // pi; matching: the 3.141592741f literal changes the bytes

/*
 * Aims `camera` at object 0 (object 1 when camera->playerIndex == 3): unk2BC..unk2C4 become the object's
 * position raised by 150, and unk2F8..unk300 a point 60 units from there on the same height, along
 * the heading (tenths of a degree) unk40 * 10 less the s16 angle in the upper half of unk2E4.
 */
void func_1512E4B0(Camera *camera) {
    s32 index;
    Actor *obj;
    s16 heading;
    f32 radians;

    index = 0;
    if (camera->playerIndex == 3) {
        index = 1;
    }

    obj = &D_800CC2D0[index];
    camera->target.x = obj->x_position;
    camera->target.y = obj->y_position + 150.0f;
    camera->target.z = obj->z_position;

    heading = -*(s16 *)&obj->unk2E4; // matching: the high half of unk2E4
    heading += obj->unk40 * 10.0f;
    radians = (heading * D_800A3730) / 1800.0f;

    camera->eye.x = camera->target.x - sinf(radians) * 60.0f;
    camera->eye.y = camera->target.y;
    camera->eye.z = camera->target.z - cosf(radians) * 60.0f;
}
