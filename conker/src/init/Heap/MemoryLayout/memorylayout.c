#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
u32 gRdpOutputBuffer; // RDP output buffer address (u64 *); matching: stored as an integer like gHeapEnd/9C so IDO shares the constants
u32 gRdpOutputBufferEnd; // its end address (same as gRdpOutputBuffer)
u32 gHeapEnd;
u32 gDecompressWork;

#include "functions.h"
#include "variables.h"

void heap_initMemoryLayout(void) {
    if (gExpansionPak) {
        gRdpOutputBuffer = 0x807F5000;
        gRdpOutputBufferEnd = 0x807FE000;
        gDecompressWork = 0x807FE000;
        gHeapEnd = 0x807F5000;
    } else {
        gRdpOutputBuffer = 0x803F5000;
        gRdpOutputBufferEnd = 0x803FE000;
        gDecompressWork = 0x803FE000;
        gHeapEnd = 0x803F5000;
    }
}
