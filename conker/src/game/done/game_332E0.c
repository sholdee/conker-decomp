#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005E30(Camera *arg0) {
    if (D_800BE9F0 == 50) {
        arg0->preset = 2;
        arg0->presetMask = 4;
        func_15124B18(arg0);
    }
}
