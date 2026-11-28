#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/script.h"

// While the values queried through script_query allow it, sets 0x400E to 1 and resets the
// camera's height/idealHeight/IdealTripodMagnitude; otherwise sets 0x400E back to 0 once camera_popMode(camera, CAMERA_MODE_TRANSPORT) succeeds.
void camera_levelHook18(Camera *camera) {
    s32 flagValue;
    s32 flagSet;

    flagSet = script_query(0, script_query(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    flagValue = script_query(0, 0x2000, 0xBB);
    if (flagSet && flagValue != -1) {
        script_command(0, 0x400E, 1);
        if (camera->Mode != 8) {
            camera_reset(camera);
        }
        if (camera_pushMode(camera, 8, 0, camera->Tightness, CAMERA_MODE_TRANSPORT)) {
            camera_attachToObject(camera, movement_obj[0].attach_char - 1);
            camera->AvailableModes |= 0x1000200;
            camera->DoingAim = 0;
            camera->AvailableModes &= ~4;
            camera->CharTrackingSpeedTarget = 0.0f;
        }
        camera->IdealTripodMagnitude = 778.0f;
        camera->height = 575.0f;
        camera->idealHeight = 575.0f;
        camera->YOffsetTget = 0.0f;
        camera_updatePlayerAngle(camera);
    } else if (camera_popMode(camera, CAMERA_MODE_TRANSPORT)) {
        camera_attachToObject(camera, 0);
        camera->CharTrackingSpeedTarget = 0.0f;
        script_command(0, 0x400E, 0);
    }
}
