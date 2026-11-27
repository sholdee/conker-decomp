#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"

void camera_levelHook33(Camera *camera) {
    if (script_query(1, 0x403E, 6, 0x2000) != 0) {
        camera->AvailableModes |= 0x20;
        return;
    }
    camera->AvailableModes &= ~0x20;
}
