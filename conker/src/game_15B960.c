#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 D_800A3730; // pi; matching: the 3.141592741f literal changes the bytes

/*
 * Aims `camera` at object 0 (object 1 when camera->unk23D == 3): unk2BC..unk2C4 become the object's
 * position raised by 150, and unk2F8..unk300 a point 60 units from there on the same height, along
 * the heading (tenths of a degree) unk40 * 10 less the s16 angle in the upper half of unk2E4.
 */
void func_1512E4B0(struct108 *camera) {
    s32 index;
    struct127 *obj;
    s16 heading;
    f32 radians;

    index = 0;
    if (camera->unk23D == 3) {
        index = 1;
    }

    obj = &D_800CC2D0[index];
    camera->unk2BC = obj->x_position;
    camera->unk2C0 = obj->y_position + 150.0f;
    camera->unk2C4 = obj->z_position;

    heading = -*(s16 *)&obj->unk2E4;
    heading += obj->unk40 * 10.0f;
    radians = (heading * D_800A3730) / 1800.0f;

    camera->unk2F8 = camera->unk2BC - sinf(radians) * 60.0f;
    camera->unk2FC = camera->unk2C0;
    camera->unk300 = camera->unk2C4 - cosf(radians) * 60.0f;
}
