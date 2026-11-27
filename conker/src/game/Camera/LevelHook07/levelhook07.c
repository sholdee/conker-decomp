#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"

static s32 D_80088710 = 999; /* queued flag query (0x900x), 999 for none */

/*
 * Per-frame camera setup driven by script_query queries. Saves the camera mode in slot 0 (Camera is the
 * camera: unk2C mode, unk1B4 preset, unk374 target distance, unk348 height, unk5F0 status) or restores it,
 * then saves/restores slot 3 depending on the 0xBC/0xBB queries; unk190 follows the C-down button.
 */
void camera_levelHook07(Camera *camera) {
    s32 resultBC;
    s32 resultBB;
    s32 i;

    if (!script_query(0, 0x503C, 0x1A)) {
        if (camera_pushMode(camera, camera->Mode, 0, 0, CAMERA_MODE_NORMAL)) {
            camera->AvailableModes |= 0x20000;
            camera->AvailableModes &= ~4;
            camera->DistIndex = 3;
            camera_loadDistancePreset(camera);
            camera->Snap = 1;
        }
        camera->IdealTripodMagnitude = 650.0f;
        camera->height = 450.0f;
        camera->idealHeight = 450.0f;
    } else {
        script_command(0, 0x4000, 0);
        if (camera->Mode == 1 && camera_popMode(camera, CAMERA_MODE_NORMAL)) {
            camera->DistIndex = 2;
            camera_loadDistancePreset(camera);
        }
        if (script_query(1, 0x4024, 6, 0x2000)) {
            D_80088710 = 0x9003;
        } else if (script_query(1, 0x4025, 6, 0x2000)) {
            D_80088710 = 0x9009;
        } else if (script_query(1, 0x4026, 6, 0x2000)) {
            D_80088710 = 0x900A;
        } else if (script_query(1, 0x4027, 6, 0x2000)) {
            D_80088710 = 0x900B;
        }
        {
            // matching: loading through a pointer makes ugen (not uopt) hold the value, in $t8
            s32 *p = &D_80088710;

            if (*p != 999) {
                script_command(5, 0x4000, 4, 2, 0, *p & 0xFFF, 0, 0);
            }
        }
        for (i = 0; i < 3; i++) {
            if (script_query(1, i + 0x400C, 6, 0x2000) || script_query(1, 0x4014, 6, 0x2000)) {
                script_command(1, 0x2000, 0x3B, 2);
            }
        }
        if (script_query(1, 0x4000, 6, 0x2000)) {
            script_command(2, 0x9000, 6, 1, 0x80000);
            script_command(1, 0x9000, 0x10, 0x55);
        } else {
            script_command(2, 0x9000, 6, 0, 0x80000);
            script_command(1, 0x9000, 0x10, 0);
        }
    }
    resultBC = script_query(0, script_query(0, 0x200A, 0xB7) | 0x2000, 0xBC);
    resultBB = script_query(0, 0x2000, 0xBB);
    if (resultBC != 0 && resultBB != -1) {
        camera->SpecialFlags |= 0x100;
    } else {
        camera->SpecialFlags &= ~0x100;
    }
    if (resultBC != 0 && resultBB != -1) {
        if (camera_pushMode(camera, camera->Mode, 0, 0, CAMERA_MODE_TRANSPORT)) {
            camera->AvailableModes |= 0x100002;
            camera->CharTrackingSpeedTarget = 0.0f;
            camera_attachToObject(camera, camera->CharPtr->attach_char - 1);
            camera->Tightness = 0;
        }
        camera->IdealTripodMagnitude = 650.0f;
        camera->idealHeight = 450.0f;
        camera->height = 450.0f;
        if (camera->PadPtr->button & D_CBUTTONS) {
            camera->YOffsetTget = 0.0f;
        } else {
            camera->YOffsetTget = 135.0f;
        }
    } else if (camera_popMode(camera, CAMERA_MODE_TRANSPORT)) {
        camera_attachToObject(camera, camera->CNumber);
        camera->CharTrackingSpeedTarget = 0.0f;
        camera->YOffsetTget = 0.0f;
    }
}
