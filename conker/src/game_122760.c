#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Camera hook: sets bit 31 of the camera's AvailableModes while query 0x401C holds, clears it otherwise. */
void func_150F52B0(Camera *camera) {
    if (func_1509BE40(1, 0x401C, 6, 0x9000) != 0) {
        camera->AvailableModes |= 0x80000000;
    } else {
        camera->AvailableModes &= ~0x80000000;
    }
}

void func_150F5310(s32 arg0) {
    func_151827D0();
}
