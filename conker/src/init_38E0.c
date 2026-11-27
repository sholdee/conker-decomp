#include <ultra64.h>

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
