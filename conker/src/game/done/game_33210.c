#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005D60(Camera *arg0) {
    arg0->preset = 2;
    arg0->presetMask = 5;
    func_15124B18(arg0);
    arg0->unk134 = 0;
    arg0->flags |= 0x4000;
}

void func_15005DA0(s32 arg0) {
}
