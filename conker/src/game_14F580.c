#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Unless flag 4 is set, recomputes the heading (unk37C, degrees; unk39C in radians)
 * from the vector between unk2F8 and the position unk2BC, then calls camera_followYaw.
 */
void camera_updateHeading(Camera *arg0) {
    Vec3f vector;
    f32 heading;

    if (!(arg0->PadPtr->button & 4)) {
        func_15048F90(&arg0->eye, &arg0->target, &vector);
        vector.x += arg0->prevTarget.x - arg0->target.x;
        vector.z += arg0->prevTarget.z - arg0->target.z;
        heading = func_15048FC8(&vector);
        arg0->CameraAngle = heading;
        arg0->CameraAngleRad = heading * 0.01745329238f;
    }
    camera_followYaw(arg0);
}
