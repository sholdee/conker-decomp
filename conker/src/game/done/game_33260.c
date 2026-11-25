#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005DB0(Camera *arg0) {
    func_15123934(arg0, 8, 0, arg0->unk134, 3);
    arg0->flags |= 0x1000000;
    arg0->flags &= -5;
    arg0->height = 500.0f;
    arg0->unk34C = 500.0f;
    arg0->targetDistance = 800.0f;
    arg0->aimState = (u16)0;
}
