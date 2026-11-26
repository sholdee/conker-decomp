#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void camera_levelHook10(Camera *arg0) {
    arg0->AvailableModes |= 0x80004000;

    if (script_query(3, 0x2000, 0xAC, 0x4002, 0x4003, 0x4004) != 0) {
        arg0->AvailableModes |= 0x400000;
    } else {
        arg0->AvailableModes &= 0xFFBFFFFF;
    }
}
