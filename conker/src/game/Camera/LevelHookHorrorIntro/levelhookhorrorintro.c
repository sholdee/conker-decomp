#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHookHorrorIntro(Camera *actor) {
    if (actor->CharContext == 0x3B) {
        script_command(0, 0x405C, 1);
        if (actor->Mode != 0x100 && actor->cmdRunning == 0) {
            if (camera_pushMode(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
                actor->AvailableModes |= 0x300000;
                actor->AvailableModes &= ~4;
                actor->DistIndex = 1;
                actor->DistMask = 3;
                camera_loadDistancePreset(actor);
            }
            actor->Tightness = 0;
            actor->height = 125.0f;
            actor->idealHeight = 125.0f;
            actor->IdealTripodMagnitude = 220.0f;
            actor->YOffsetTget = 30.0f;
        } else {
            actor->YOffsetTget = 0.0f;
        }
    } else if (actor->Mode == 8) {
        if (actor->cmdRunning == 0) {
            camera_popMode(actor, CAMERA_MODE_NORMAL);
            script_command(0, 0x405C, 0);
        }
    }

    if (script_query(1, 0x4054, 6, 0x9000) != 0 && script_query(1, 0x405E, 6, 0x9000) == 0) {
        actor->AvailableModes |= 0x80000000;
    } else {
        actor->AvailableModes &= ~0x80000000;
    }
}
