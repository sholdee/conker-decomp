#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHook22(Camera *camera) {
    if ((gTaskDoneBits[0x0] & 0x80) == 0) {
        if (script_query(0, 0x2000, 0xBB) != -1) {
            if (camera_pushMode(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_PICKED_UP) != 0) {
                camera->AvailableModes |= 0x1000000;
                camera_attachToObject(camera, movement_obj[0].attach_char - 1);
                camera->YOffsetTget = 100.0f;
            }
        } else if (camera_popMode(camera, CAMERA_MODE_PICKED_UP) != 0) {
            camera_attachToObject(camera, 0);
            camera->YOffsetTget = 0.0f;
        }
    }
}
