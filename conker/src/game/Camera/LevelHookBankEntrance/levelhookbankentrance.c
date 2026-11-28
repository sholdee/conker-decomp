#include <ultra64.h>
#include "conker/camera.h"
#include "conker/prop.h"
#include "conker/script.h"

/* Camera hook: sets bit 31 of the camera's AvailableModes while query 0x401C holds, clears it otherwise. */
void camera_levelHook34(Camera *camera) {
    if (script_query(1, 0x401C, 6, 0x9000) != 0) {
        camera->AvailableModes |= 0x80000000;
    } else {
        camera->AvailableModes &= ~0x80000000;
    }
}

void camera_levelUpdateWaveChains(s32 unused) {
    prop_updateWaveChains();
}
