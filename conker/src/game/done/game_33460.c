#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005FB0(Camera *arg0) {
    arg0->mode = 8;
    arg0->flags |= 0x300000;
    arg0->flags &= -5;
    arg0->preset = (u16)1;
    arg0->presetMask = (u16)3;
    func_15124B18(arg0);
    arg0->unk134 = 0;
}
