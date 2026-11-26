#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
u32 D_80038090; // RDP output buffer address (u64 *); matching: stored as an integer like D_80038098/9C so IDO shares the constants
u32 D_80038094; // its end address (same as D_80038090)
u32 D_80038098;
u32 D_8003809C;

#include "functions.h"
#include "variables.h"

void func_10003930(void) {
    if (D_80038080) {
        D_80038090 = 0x807F5000;
        D_80038094 = 0x807FE000;
        D_8003809C = 0x807FE000;
        D_80038098 = 0x807F5000;
    } else {
        D_80038090 = 0x803F5000;
        D_80038094 = 0x803FE000;
        D_8003809C = 0x803FE000;
        D_80038098 = 0x803F5000;
    }
}
