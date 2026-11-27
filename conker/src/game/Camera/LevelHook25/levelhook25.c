#include <ultra64.h>
#include "conker/camera.h"
#include "conker/player.h"
#include "conker/script.h"

void camera_levelHook25(Camera *arg0) {
    if (player_getStat(arg0->CharPtr->playernumber, 1) == 9) {
        camera_popMode(arg0, CAMERA_MODE_ZONES);
        if (camera_pushMode(arg0, 8, 0, arg0->Tightness, CAMERA_MODE_TRANSPORT) != 0) {
            arg0->DoingAim = 0;
            arg0->AvailableModes |= 0x1300000;
            arg0->AvailableModes &= -5;
            camera_updatePlayerAngle(arg0);
        }
        arg0->height = 150.0f;
        arg0->unk34C = 150.0f;
        arg0->IdealTripodMagnitude = 340.0f;
        arg0->SpecialFlags |= 0x800;
        arg0->YOffsetTget = -6.0f;
    } else {
        arg0->SpecialFlags &= ~0x800;
    }
    if (script_query(1, 0x401A, 6, 0x9000) || script_query(1, 0x401B, 6, 0x9000)) {
        arg0->AvailableModes |= 0x10000;
    } else {
        arg0->AvailableModes &= 0xFFFEFFFF;
    }
}
