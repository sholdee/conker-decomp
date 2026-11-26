#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150F9A20(Camera *arg0) {
    if (script_query(1, 0x4025, 6, 0x2000)) {
        arg0->AvailableModes |= 0x80;
        arg0->AvailableModes &= ~8;
        arg0->YOffsetTget = 85.0f;
    } else {
        arg0->AvailableModes &= ~0x80;
        arg0->AvailableModes |= 8;
        arg0->YOffsetTget = 0.0f;
    }
}
