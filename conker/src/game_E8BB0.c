#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_150BB700(s32 *arg0) {
    if (script_query(1, 0x4047, 6, 0x2000) != 0) {
        arg0[0x84 / 4] |= 0x1000;
    } else {
        arg0[0x84 / 4] &= ~0x1000;
    }
}
