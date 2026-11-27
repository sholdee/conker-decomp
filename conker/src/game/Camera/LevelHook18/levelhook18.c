#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/script.h"

// While the values queried through script_query allow it, sets 0x400E to 1 and resets the
// actor's unk348/unk34C/unk374; otherwise sets 0x400E back to 0 once camera_popMode(actor, CAMERA_MODE_TRANSPORT) succeeds.
void camera_levelHook18(Camera *actor) {
    s32 flagValue;
    s32 flagSet;

    flagSet = script_query(0, script_query(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    flagValue = script_query(0, 0x2000, 0xBB);
    if (flagSet && flagValue != -1) {
        script_command(0, 0x400E, 1);
        if (actor->Mode != 8) {
            camera_reset(actor);
        }
        if (camera_pushMode(actor, 8, 0, actor->Tightness, CAMERA_MODE_TRANSPORT)) {
            camera_attachToObject(actor, movement_obj[0].attach_char - 1);
            actor->AvailableModes |= 0x1000200;
            actor->DoingAim = 0;
            actor->AvailableModes &= ~4;
            actor->CharTrackingSpeedTarget = 0.0f;
        }
        actor->IdealTripodMagnitude = 778.0f;
        actor->height = 575.0f;
        actor->unk34C = 575.0f;
        actor->YOffsetTget = 0.0f;
        camera_updatePlayerAngle(actor);
    } else if (camera_popMode(actor, CAMERA_MODE_TRANSPORT)) {
        camera_attachToObject(actor, 0);
        actor->CharTrackingSpeedTarget = 0.0f;
        script_command(0, 0x400E, 0);
    }
}
