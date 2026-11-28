#include <ultra64.h>
#include "conker/camera.h"
#include "conker/script.h"



void camera_levelHookDogFishNurseryPart1(Camera *camera) {
    camera->AvailableModes |= 0x80004000;

    if (script_query(3, 0x2000, 0xAC, 0x4002, 0x4003, 0x4004) != 0) {
        camera->AvailableModes |= 0x400000;
    } else {
        camera->AvailableModes &= 0xFFBFFFFF;
    }
}
