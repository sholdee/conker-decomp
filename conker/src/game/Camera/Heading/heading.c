#include <ultra64.h>
#include "conker/camera.h"
#include "conker/math.h"

/**
 * Unless flag 4 is set, recomputes the heading (unk37C, degrees; unk39C in radians)
 * from the vector between unk2F8 and the position unk2BC, then calls camera_followYaw.
 */
void camera_updateHeading(Camera *camera) {
    Vec3f vector;
    f32 heading;

    if (!(camera->PadPtr->button & 4)) {
        math_vec3Diff(&camera->eye, &camera->target, &vector);
        vector.x += camera->prevTarget.x - camera->target.x;
        vector.z += camera->prevTarget.z - camera->target.z;
        heading = math_vec3Heading(&vector);
        camera->CameraAngle = heading;
        camera->CameraAngleRad = heading * 0.01745329238f;
    }
    camera_followYaw(camera);
}
