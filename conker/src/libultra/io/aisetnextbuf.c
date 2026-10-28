#include <os_internal.h>
#include <rcp.h>
#include "../os/osint.h"

/*
 * hdwrBugFlag is a function-scope static u8 (= 0) in the original source, but this
 * file's .data is not linked from C yet. Accessed through one extern, IDO hoists its
 * address into a register; the original (static) form addresses it afresh at each use.
 * Spelling the two stores via neighbouring symbols (same address 0x8002AB40) reproduces
 * that. Restore the static once the file's .data is linked.
 */
extern u8 hdwrBugFlag;
extern u8 D_8002AB14[]; /* D_8002AB14 + 0x2C == &hdwrBugFlag */
extern u8 D_8002AB50[]; /* D_8002AB50 - 0x10 == &hdwrBugFlag */

s32 osAiSetNextBuffer(void *bufPtr, u32 size) {
    u8 *bptr;

    bptr = bufPtr;
    if (hdwrBugFlag != 0) {
        bptr = (u8 *)bufPtr - 0x2000;
    }
    if ((((u32)bufPtr + size) & 0x3FFF) == 0x2000) {
        D_8002AB14[0x2C] = 1;
    } else {
        D_8002AB50[-0x10] = 0;
    }
    if (__osAiDeviceBusy() != 0) {
        return -1;
    }
    *(volatile u32 *)0xA4500000 = osVirtualToPhysical(bptr);
    *(volatile u32 *)0xA4500004 = size;
    return 0;
}
// NON-MATCHING: matches won't OK until .bss is figured out (i think?)
// s32 osAiSetNextBuffer(void *bufPtr, u32 size)
// {
//     static u8 hdwrBugFlag = 0;
//   	char *bptr = bufPtr;
//   	if (hdwrBugFlag != 0)
//   		  bptr = (u32)bufPtr - 0x2000;
//
//   	if ((((s32)bufPtr + size) & 0x3fff) == 0x2000)
//   		  hdwrBugFlag = 1;
//   	else
//   		  hdwrBugFlag = 0;
//
//   	if (__osAiDeviceBusy())
//   		return -1;
//
//   	IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
//   	IO_WRITE(AI_LEN_REG, size);
//   	return 0;
// }
