#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHookHorrorIntro(Camera *camera) {
    if (camera->CharContext == 0x3B) {
        script_command(0, 0x405C, 1);
        if (camera->Mode != 0x100 && camera->cmdRunning == 0) {
            if (camera_pushMode(camera, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
                camera->AvailableModes |= 0x300000;
                camera->AvailableModes &= ~4;
                camera->DistIndex = 1;
                camera->DistMask = 3;
                camera_loadDistancePreset(camera);
            }
            camera->Tightness = 0;
            camera->height = 125.0f;
            camera->idealHeight = 125.0f;
            camera->IdealTripodMagnitude = 220.0f;
            camera->YOffsetTget = 30.0f;
        } else {
            camera->YOffsetTget = 0.0f;
        }
    } else if (camera->Mode == 8) {
        if (camera->cmdRunning == 0) {
            camera_popMode(camera, CAMERA_MODE_NORMAL);
            script_command(0, 0x405C, 0);
        }
    }

    if (script_query(1, 0x4054, 6, 0x9000) != 0 && script_query(1, 0x405E, 6, 0x9000) == 0) {
        camera->AvailableModes |= 0x80000000;
    } else {
        camera->AvailableModes &= ~0x80000000;
    }
}
