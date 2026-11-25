#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005B10(Camera *arg0) {
    arg0->presetMask = 8;
    arg0->preset = 3;
    arg0->flags = arg0->flags & -5;
    func_15124B18(arg0);
}
