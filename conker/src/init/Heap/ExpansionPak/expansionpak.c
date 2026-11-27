#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
u8 gExpansionPak; // an Expansion Pak is present (8MB of RDRAM)

#include "conker/heap.h"


void heap_clearExpansionPak(void) {
    gExpansionPak = 0;
}
