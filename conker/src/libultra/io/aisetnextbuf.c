#include <os_internal.h>
#include <rcp.h>
#include "../os/osint.h"

s32 osAiSetNextBuffer(void *bufPtr, u32 size) {
    static u8 hdwrBugFlag = 0;
    u8 *bptr = bufPtr;

    if (hdwrBugFlag != 0) {
        bptr = (u8 *)bufPtr - 0x2000;
    }
    if ((((u32)bufPtr + size) & 0x3FFF) == 0x2000) {
        hdwrBugFlag = 1;
    } else {
        hdwrBugFlag = 0;
    }
    if (__osAiDeviceBusy() != 0) {
        return -1;
    }
    *(volatile u32 *)0xA4500000 = osVirtualToPhysical(bptr);
    *(volatile u32 *)0xA4500004 = size;
    return 0;
}
