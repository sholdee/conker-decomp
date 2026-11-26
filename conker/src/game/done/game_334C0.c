#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelSetup24(Camera *actor) {
    if (actor->CharPtr->unk102 != 0) {
        if (camera_pushMode(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            actor->AvailableModes |= 0x100000;
            actor->AvailableModes &= ~4;
            actor->DistIndex = 1;
            actor->DistMask = 3;
            camera_loadDistancePreset(actor);
        }
        actor->height = 143.0f;
        actor->unk34C = 143.0f;
        actor->IdealTripodMagnitude = 150.0f;
        actor->YOffsetTget = -38.0f;
        return;
    }

    if (actor->CharContext == 59) {
        if (camera_pushMode(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
            actor->AvailableModes |= 0x100000;
            actor->AvailableModes &= ~4;
            actor->height = 155.0f;
            actor->unk34C = 155.0f;
            actor->YOffsetTget = 30.0f;
            actor->YOffset = 30.0f;
            actor->IdealTripodMagnitude = 178.0f;
        }
    } else {
        camera_popMode(actor, CAMERA_MODE_NORMAL);
    }
}

void camera_levelSetup25(Camera *actor) {
    actor->DistMask = 5;
    actor->DistIndex = 2;
    camera_loadDistancePreset(actor);
}
