#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"

static s32 gLevelHook04TransportOn = 0; /* 1 while the camera is in the transport mode this hook set */

/*
 * When both script_query queries succeed, switch flags 0x405D-0x4061 on and (unless Mode is 0x100
 * or cmdRunning is set) put the camera into its transport mode and set gLevelHook04TransportOn. Otherwise leave that
 * state, switch the flags off again and mirror query 0x4040 into bit 0x80 of AvailableModes.
 */
void camera_levelHook04(Camera *camera)
{
    s32 firstResult;
    s32 secondResult;

    firstResult = script_query(0, script_query(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    secondResult = script_query(0, 0x2000, 0xBB);
    if (firstResult != 0 && secondResult != 0) {
        script_command(0, 0x405D, 1);
        script_command(0, 0x405E, 1);
        script_command(0, 0x405F, 1);
        script_command(0, 0x4060, 1);
        script_command(0, 0x4061, 1);
        if (camera->Mode != 0x100 && camera->cmdRunning == 0) {
            if (camera_pushMode(camera, 8, 0, 0, CAMERA_MODE_TRANSPORT)) {
                camera->AvailableModes |= 0x01100004;
                camera->AvailableModes &= ~2;
                camera->CharTrackingSpeedTarget = 0.0f;
                camera_attachToObject(camera, camera->CharPtr->attach_char - 1);
                camera->SpecialFlags |= 0x10;
            }
            camera->IdealTripodMagnitude = 550.0f;
            camera->Tightness = 0;
            camera->height = camera->unk34C = 320.0f;
            camera->YOffsetTget = 180.0f;
            gLevelHook04TransportOn = 1;
            script_command(0, 0x405C, 0);
        }
    } else {
        if (camera_popMode(camera, CAMERA_MODE_TRANSPORT)) {
            camera->YOffsetTget = 0.0f;
            camera_attachToObject(camera, camera->CNumber);
            camera->SpecialFlags &= ~0x10;
            camera->CharTrackingSpeedTarget = 0.0f;
            gLevelHook04TransportOn = 0;
        }
        script_command(0, 0x405C, 1);
        script_command(0, 0x405D, 0);
        script_command(0, 0x405E, 0);
        script_command(0, 0x405F, 0);
        script_command(0, 0x4060, 0);
        script_command(0, 0x4061, 0);
        if (script_query(1, 0x4040, 6, 0x9000)) {
            camera->AvailableModes |= 0x80;
        } else {
            camera->AvailableModes &= ~0x80;
        }
    }
}
