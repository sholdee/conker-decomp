#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A0F60;
extern f32 D_800A0F64;

void func_150DF820(Camera *arg0) {
    arg0->AvailableModes &= ~0x4000;
    arg0->AvailableModes |= 0x4;
    arg0->AvailableModes &= ~0x1010;
    if (arg0->CharPtr->inwater != 0) {
        arg0->AvailableModes |= 0x1010;
        arg0->AvailableModes &= ~0x4;
        arg0->IdealTripodMagnitude = D_800A0F60;
    } else {
        if (D_800A0F64 == arg0->IdealTripodMagnitude) {
            arg0->DistIndex = 3;
            func_15124B18(arg0);
        }
    }
}
