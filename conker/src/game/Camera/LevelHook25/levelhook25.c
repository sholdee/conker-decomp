#include <ultra64.h>
#include "conker/camera.h"
#include "conker/player.h"
#include "conker/script.h"

void camera_levelHook25(Camera *camera) {
    if (player_getStat(camera->CharPtr->playernumber, 1) == 9) {
        camera_popMode(camera, CAMERA_MODE_ZONES);
        if (camera_pushMode(camera, 8, 0, camera->Tightness, CAMERA_MODE_TRANSPORT) != 0) {
            camera->DoingAim = 0;
            camera->AvailableModes |= 0x1300000;
            camera->AvailableModes &= -5;
            camera_updatePlayerAngle(camera);
        }
        camera->height = 150.0f;
        camera->unk34C = 150.0f;
        camera->IdealTripodMagnitude = 340.0f;
        camera->SpecialFlags |= 0x800;
        camera->YOffsetTget = -6.0f;
    } else {
        camera->SpecialFlags &= ~0x800;
    }
    if (script_query(1, 0x401A, 6, 0x9000) || script_query(1, 0x401B, 6, 0x9000)) {
        camera->AvailableModes |= 0x10000;
    } else {
        camera->AvailableModes &= 0xFFFEFFFF;
    }
}
