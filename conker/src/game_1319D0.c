#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Resets four distance parameters to their defaults, then calls func_15128774 when the target
// is moving slowly and the value at 0x370 exceeds 1.5 * unk374.
void func_15104520(struct108 *camera) {
    struct127 *target;

    if (camera->unk6FC != 0 || camera->unk6C8 == 0) {
        camera->unk348 = 117.0f;
        camera->unk34C = 117.0f;
        camera->unk374 = 370.0f;
        camera->unk190 = 77.0f;
    }
    target = camera->unk3D0;
    if (target->xz_velocity < 50.0f && camera->unk3E8 == 0) {
        // pad370 is an f32 the shared struct108 does not name yet
        if (0.5f * camera->unk374 < *(f32 *)camera->pad370 - camera->unk374) {
            func_15128774(camera, target);
        }
    }
}
