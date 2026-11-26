#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Resets four distance parameters to their defaults, then calls camera_placeAtObject when the target
// is moving slowly and the value at 0x370 exceeds 1.5 * unk374.
void camera_resetDistances(Camera *camera) {
    Actor *target;

    if (camera->cmd.type != 0 || camera->cmdRunning == 0) {
        camera->height = 117.0f;
        camera->unk34C = 117.0f;
        camera->IdealTripodMagnitude = 370.0f;
        camera->YOffsetTget = 77.0f;
    }
    target = camera->CharPtr;
    if (target->speed < 50.0f && camera->TonyHitsClear == 0) {
        // matching: the no-op (f32 *) cast changes how IDO schedules the distance load
        if (0.5f * camera->IdealTripodMagnitude < *(f32 *)&camera->TripodMagnitude - camera->IdealTripodMagnitude) {
            camera_placeAtObject(camera, target);
        }
    }
}
