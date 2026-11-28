#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHookEndArena(Camera *camera) {
    if (script_query(1, 0x4025, 6, 0x2000)) {
        camera->AvailableModes |= 0x80;
        camera->AvailableModes &= ~8;
        camera->YOffsetTget = 85.0f;
    } else {
        camera->AvailableModes &= ~0x80;
        camera->AvailableModes |= 8;
        camera->YOffsetTget = 0.0f;
    }
}
