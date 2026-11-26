#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: pi read from a const struct; as a plain literal (or a one-element array) IDO gives the
// function a larger frame (or another schedule)
static const struct {
    f32 pi;
} D_800A3730 = { 3.141592741f };

/*
 * Aims `camera` at object 0 (object 1 when camera->CNumber == 3): unk2BC..unk2C4 become the object's
 * position raised by 150, and unk2F8..unk300 a point 60 units from there on the same height, along
 * the heading (tenths of a degree) unk40 * 10 less the s16 angle in the upper half of taskint[0].
 */
void camera_aimAtObject(Camera *camera) {
    s32 index;
    Actor *obj;
    s16 heading;
    f32 radians;

    index = 0;
    if (camera->CNumber == 3) {
        index = 1;
    }

    obj = &movement_obj[index];
    camera->target.x = obj->xpos;
    camera->target.y = obj->ypos + 150.0f;
    camera->target.z = obj->zpos;

    heading = -*(s16 *)&obj->taskint[0]; // matching: the high half of taskint[0]
    heading += obj->theta * 10.0f;
    radians = (heading * D_800A3730.pi) / 1800.0f;

    camera->eye.x = camera->target.x - sinf(radians) * 60.0f;
    camera->eye.y = camera->target.y;
    camera->eye.z = camera->target.z - cosf(radians) * 60.0f;
}
