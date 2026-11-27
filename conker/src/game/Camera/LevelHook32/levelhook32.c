#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"


void camera_levelHook32(Camera *arg0) {
    if (script_query(1, 0x4023, 6, 0x2000) != 0) {
        arg0->AvailableModes |= 0x1010;
    } else {
        arg0->AvailableModes &= ~0x1010;
    }
    arg0->AvailableModes &= ~8;
}
