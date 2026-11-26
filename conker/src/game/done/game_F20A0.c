#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_150C4BF0(Camera *arg0) {
    if (script_query(1, 0x403F, 6, 0x2000)) {
        arg0->AvailableModes |= 0x1000000;
    } else {
        arg0->AvailableModes &= 0xFEFFFFFF;
    }
    if (script_query(1, 0x4019, 6, 0x2000)) {
        script_command(1, 0x9000, 0x10, 0xA0);
        return;
    }
    if (script_query(1, 0x4044, 6, 0x2000)) {
        script_command(1, 0x9000, 0x10, 0);
        return;
    }
    if (script_query(4, 0x2000, 0xAC, 0x401B, 0x401C, 0x401D, 0x401E)) {
        script_command(1, 0x9000, 0x10, -0x96);
        return;
    }
    script_command(1, 0x9000, 0x10, 0);
}
