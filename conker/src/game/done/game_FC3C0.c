#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_150CEF10(Camera *arg0) {
    arg0->flags |= 0x10;
    if (arg0->preset == 3) {
        arg0->presetMask = 5;
        arg0->preset = 2;
        func_15124B18(arg0);
    }
}
