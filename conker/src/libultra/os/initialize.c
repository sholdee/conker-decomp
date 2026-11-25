#include <ultra64.h>

#include "functions.h"

extern s32 D_800428E0;
extern s32 D_8000030C;
extern s32 D_8000031C[16];
#pragma weak D_8002BD10
u64 D_8002BD10;
extern s32 D_8002BD20;


typedef struct {
    u32 inst1;
    u32 inst2;
    u32 inst3;
    u32 inst4;
} __osExceptionVector;

extern s32 __osSiRawReadIo(u32, u32 *);
extern s32 __osSiRawWriteIo(u32, u32);
extern s32 osPiRawReadIo(u32, u32 *);
extern u32 __osGetSR(void);
extern void __osSetSR(u32);
extern u32 __osSetFpcCsr(u32);
extern void osMapTLBRdb(void);
extern void __osSetHWIntrRoutine(s32, s32 (*)(void));
extern s32 __osLeoInterrupt(void);
extern void func_100071D0(void);

void __osInitialize_common(void) {
    u32 pifdata;
    u32 clock = 0;
    u32 leostatus;
    u32 pistatus;

    D_800428E0 = 1;
    __osSetSR(__osGetSR() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    while (__osSiRawReadIo(0x1FC007FC, &pifdata)) {
    }
    while (__osSiRawWriteIo(0x1FC007FC, pifdata | 8)) {
    }
    *(__osExceptionVector *)0x80000000 = *(__osExceptionVector *)func_100071D0;
    *(__osExceptionVector *)0x80000080 = *(__osExceptionVector *)func_100071D0;
    *(__osExceptionVector *)0x80000100 = *(__osExceptionVector *)func_100071D0;
    *(__osExceptionVector *)0x80000180 = *(__osExceptionVector *)func_100071D0;
    osWritebackDCache((void *)0x80000000, 0x190);
    osInvalICache((void *)0x80000000, 0x190);
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        D_8002BD10 = clock;
    }
    D_8002BD10 = D_8002BD10 * 3 / 4;
    if (D_8000030C == 0) {
        bzero(D_8000031C, 0x40);
    }
    pistatus = *(vu32 *)0xA4600010;
    while (pistatus & 3) {
        pistatus = *(vu32 *)0xA4600010;
    }
    leostatus = *(vu32 *)0xA5000508;
    if (!(leostatus & 0xFFFF)) {
        D_8002BD20 = 1;
        __osSetHWIntrRoutine(1, __osLeoInterrupt);
    } else {
        D_8002BD20 = 0;
    }
}
