#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/script.h"


/* Updates flag bits in unk84 (0x4, 0x4000, 0x10000, 0x1000000) from script_query queries; the first two
   checks are skipped when gTaskDoneBits[0x4] bit 0x80 / unk1 bit 4 are set. */
void effect_levelHookTheWindmill(Camera *camera) {
    if (!(gTaskDoneBits[0x4] & 0x80)) {
        if (script_query(0, 0x2000, 0xBB) != -1) {
            if (camera_pushMode(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_PICKED_UP) != 0) {
                camera->AvailableModes |= 0x1000000;
                camera_attachToObject(camera, movement_obj[0].attach_char - 1);
            }
        } else if (camera_popMode(camera, CAMERA_MODE_PICKED_UP) != 0) {
            camera_attachToObject(camera, 0);
        }
    }
    camera->AvailableModes &= ~0x4000;
    if (!(gTaskDoneBits[0x1] & 4)) {
        if (script_query(1, 0x2000, 0x95, script_query(0, 0x2014, 0xB7) | 0x2000) != 0) {
            camera->AvailableModes |= 0x1000000;
            if ((camera->Mode & 1) && camera_pushMode(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_NORMAL) != 0) {
                camera->DistIndex = 3;
                camera->AvailableModes &= ~4;
                camera_loadDistancePreset(camera);
            }
        } else if (camera_popMode(camera, CAMERA_MODE_NORMAL) != 0) {
            camera_loadDistancePreset(camera);
            camera->AvailableModes &= ~0x1000000;
        }
    }
    if (script_query(1, 0x4082, 6, 0x9000) != 0) {
        camera->AvailableModes |= 0x10000;
    } else {
        camera->AvailableModes &= ~0x10000;
    }
}
