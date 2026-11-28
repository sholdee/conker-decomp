#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"



void camera_levelHookBarnOutside(Camera *camera) {
    if (script_query(1, 0x4047, 6, 0x2000) != 0) {
        camera->AvailableModes |= 0x1000;
    } else {
        camera->AvailableModes &= ~0x1000;
    }
}
