#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150EC4B0(Camera *arg0) {
    if (func_1509BE40(1, 0x4023, 6, 0x2000) != 0) {
        arg0->flags |= 0x1010;
    } else {
        arg0->flags &= ~0x1010;
    }
    arg0->flags &= ~8;
}
