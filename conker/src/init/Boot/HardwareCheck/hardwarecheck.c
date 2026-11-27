#include <ultra64.h>

/*
 * The boot code's check of the hardware on the cartridge port. In the October 2000 debug build boot_entry calls
 * func_100038E0, and boot_runGame calls the function after it, which compares the id at 0xBC000000 with a list
 * of ids and returns nonzero when none fits; the game then stops in a loop. That function starts on an address
 * that is not 16-aligned, so it is part of this file. In the US build it only returns 0 (func_1000390C), and
 * nothing calls either function.
 */

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
static u32 D_80038070;
static u16 D_80038074;

#include "conker/boot.h"


// permuter MATCHED (was best 140): forcing v addressable (p = &v; return *p) keeps v
// pinned in v0 so the const store-base is reused and the const load stays lui/ori v0.
s32 func_100038E0(void) {
    s32 v = 0xBC000C02 ^ 0;
    s32 *p;
    if (1) {
        D_80038070 = v;
        D_80038074 = 0x4040;
        *(s16 *)v = 0x4040;
    }
    p = &v;
    return *p;
}

s32 func_1000390C(void) {
    return 0;
}
