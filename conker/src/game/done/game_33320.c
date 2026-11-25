#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005E70(Camera *arg0) {
    arg0->preset = 2;
    func_15124B18(arg0);
    arg0->snap = 1;
}

void func_15005EA0(Camera *arg0) {
    arg0->preset = 1;
    arg0->presetMask = 15;
    func_15124B18(arg0);
    arg0->unk134 = 1;
}
