#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHook32(Camera *camera) {
    if (script_query(1, 0x4023, 6, 0x2000) != 0) {
        camera->AvailableModes |= 0x1010;
    } else {
        camera->AvailableModes &= ~0x1010;
    }
    camera->AvailableModes &= ~8;
}
