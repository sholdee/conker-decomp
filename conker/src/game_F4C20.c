#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_150C7770(Camera *arg0) {
    if (script_query(3, 0x2000, 0xAC, 0x401B, 0x401C, 0x401D) != 0) {
        script_command(1, 0x2000, 0x3B, 2);
    }
    if (script_query(1, 0x4006, 6, 0x2000) != 0) {
        arg0->AvailableModes |= 0x400000;
    } else {
        arg0->AvailableModes &= 0xFFBFFFFF;
    }
    arg0->AvailableModes |= 0x400000;
    if (script_query(1, 0x4030, 6, 0x2000) != 0) {
        arg0->AvailableModes |= 0x1000000;
    } else {
        arg0->AvailableModes &= 0xFEFFFFFF;
    }
}
