#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
u8 D_80038080; // an Expansion Pak is present (8MB of RDRAM)

#include "functions.h"
#include "variables.h"


void func_10003920(void) {
    D_80038080 = 0;
}
