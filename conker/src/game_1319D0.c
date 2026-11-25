#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Resets four distance parameters to their defaults, then calls func_15128774 when the target
// is moving slowly and the value at 0x370 exceeds 1.5 * unk374.
void func_15104520(Camera *camera) {
    Actor *target;

    if (camera->cmd.type != 0 || camera->cmdRunning == 0) {
        camera->height = 117.0f;
        camera->unk34C = 117.0f;
        camera->targetDistance = 370.0f;
        camera->liftTarget = 77.0f;
    }
    target = camera->player;
    if (target->speed < 50.0f && camera->unk3E8 == 0) {
        // matching: the no-op (f32 *) cast changes how IDO schedules the distance load
        if (0.5f * camera->targetDistance < *(f32 *)&camera->distance - camera->targetDistance) {
            func_15128774(camera, target);
        }
    }
}
