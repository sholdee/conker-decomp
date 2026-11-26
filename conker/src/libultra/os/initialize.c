#include <ultra64.h>
#include <os_internal.h>
#include "osint.h"
#include "piint.h"

u64 osClockRate = 62500000;      /* OS_CLOCK_RATE */
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
s32 osDiskExist = 0;
u32 __osFinalrom;


void __osInitialize_common(void) {
    u32 pifdata;
    u32 clock = 0;
    u32 leostatus;
    u32 pistatus;

    __osFinalrom = 1;
    __osSetSR(__osGetSR() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    while (__osSiRawReadIo(0x1FC007FC, &pifdata)) {
    }
    while (__osSiRawWriteIo(0x1FC007FC, pifdata | 8)) {
    }
    *(__osExceptionVector *)0x80000000 = *func_100071D0;
    *(__osExceptionVector *)0x80000080 = *func_100071D0;
    *(__osExceptionVector *)0x80000100 = *func_100071D0;
    *(__osExceptionVector *)0x80000180 = *func_100071D0;
    osWritebackDCache((void *)0x80000000, 0x190);
    osInvalICache((void *)0x80000000, 0x190);
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;
    if (D_8000030C == 0) {
        bzero(D_8000031C, 0x40);
    }
    pistatus = *(vu32 *)0xA4600010;
    while (pistatus & 3) {
        pistatus = *(vu32 *)0xA4600010;
    }
    leostatus = *(vu32 *)0xA5000508;
    if (!(leostatus & 0xFFFF)) {
        osDiskExist = 1;
        __osSetHWIntrRoutine(1, __osLeoInterrupt);
    } else {
        osDiskExist = 0;
    }
}
