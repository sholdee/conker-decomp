#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// While the values queried through func_1509BE40 allow it, sets 0x400E to 1 and resets the
// actor's unk348/unk34C/unk374; otherwise sets 0x400E back to 0 once func_151239CC(actor, CAMERA_MODE_TRANSPORT) succeeds.
void func_150BE850(Camera *actor) {
    s32 flagValue;
    s32 flagSet;

    flagSet = func_1509BE40(0, func_1509BE40(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    flagValue = func_1509BE40(0, 0x2000, 0xBB);
    if (flagSet && flagValue != -1) {
        func_1509BFB0(0, 0x400E, 1);
        if (actor->Mode != 8) {
            func_1512868C(actor);
        }
        if (func_15123934(actor, 8, 0, actor->Tightness, CAMERA_MODE_TRANSPORT)) {
            func_151254F4(actor, movement_obj[0].attach_char - 1);
            actor->AvailableModes |= 0x1000200;
            actor->DoingAim = 0;
            actor->AvailableModes &= ~4;
            actor->CharTrackingSpeedTarget = 0.0f;
        }
        actor->IdealTripodMagnitude = 778.0f;
        actor->height = 575.0f;
        actor->unk34C = 575.0f;
        actor->YOffsetTget = 0.0f;
        func_15123070(actor);
    } else if (func_151239CC(actor, CAMERA_MODE_TRANSPORT)) {
        func_151254F4(actor, 0);
        actor->CharTrackingSpeedTarget = 0.0f;
        func_1509BFB0(0, 0x400E, 0);
    }
}
