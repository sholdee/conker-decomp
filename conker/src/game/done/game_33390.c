#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005EE0(Camera *arg0) {
    arg0->preset = (u16)2;
    arg0->presetMask = (u16)0xF;
    func_15124B18(arg0);
    arg0->unk134 = 1;
}
