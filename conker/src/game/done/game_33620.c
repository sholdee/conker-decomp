#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15006170(Camera *arg0) {
    arg0->presetMask = (u16)5;
    arg0->preset = (u16)2;
    func_15124B18(arg0);
    arg0->flags = (s32) (arg0->flags | 0x4000);
}
