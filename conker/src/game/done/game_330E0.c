#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005C30(Camera *arg0) {
    arg0->flags |= 0x100000;
    arg0->flags &= -5;
    arg0->preset = 2;
    arg0->presetMask = 5;
    func_15124B18(arg0);
}
