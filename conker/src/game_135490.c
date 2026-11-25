#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15107FE0(Camera *arg0) {
    if (func_150859AC(arg0->CharPtr->playernumber, 1) == 9) {
        func_151239CC(arg0, CAMERA_MODE_ZONES);
        if (func_15123934(arg0, 8, 0, arg0->Tightness, CAMERA_MODE_TRANSPORT) != 0) {
            arg0->DoingAim = 0;
            arg0->AvailableModes |= 0x1300000;
            arg0->AvailableModes &= -5;
            func_15123070(arg0);
        }
        arg0->height = 150.0f;
        arg0->unk34C = 150.0f;
        arg0->IdealTripodMagnitude = 340.0f;
        arg0->SpecialFlags |= 0x800;
        arg0->YOffsetTget = -6.0f;
    } else {
        arg0->SpecialFlags &= ~0x800;
    }
    if (func_1509BE40(1, 0x401A, 6, 0x9000) || func_1509BE40(1, 0x401B, 6, 0x9000)) {
        arg0->AvailableModes |= 0x10000;
    } else {
        arg0->AvailableModes &= 0xFFFEFFFF;
    }
}
