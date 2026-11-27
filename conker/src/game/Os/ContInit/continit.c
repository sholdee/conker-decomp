#include <ultra64.h>
#include "conker/os.h"
#include "controller.h"
#include "siint.h"

/* game/Os/ContInit/continit -- the game's private copy of osContInit / __osContGetInitData /
 * __osPackRequestData.  Built at -O1, like the rest of the libultra code in this ROM
 * (see the OPT_FLAGS override in the Makefile); at the -O2 -g3 tree default these score
 * 198/132/134, at -O1 all three are byte-perfect.
 *
 * The three functions do NOT call each other: osContInit2's `jal`s resolve to the
 * BOOT-segment copies at 0x10025608 / 0x10025538 (checked against the raw encodings
 * 0x0C009582 / 0x0C00954E), so the external names below are deliberate.
 *
 * The two details that differ from the published SDK source -- the zero loop running to 16
 * rather than ARRLEN(ramarray)==15, and the type word being assembled as
 * `(typel << 8) | typeh` against this field order -- are taken from the already-matched
 * __osPfsRequestData2 / __osPfsGetInitData2 in src/libultra/io/pfsisplug2.c, which is the
 * same libultra code operating on a different PIF ram block.
 *
 * Local declaration order in osContInit2 is load-bearing: IDO gives each automatic a stack
 * home in REVERSE declaration order (first declared lands highest), and golden's frame is
 * dummy 0x7C, ret 0x78, time 0x70, t 0x50, timerMesgQueue 0x38.
 */

/* .bss, linked at 0x800E0C90..0x800E0D20: the globals of this copy of the SDK's controller.c, in the
 * SDK's layout (__osContPifRam, __osContLastCmd, __osMaxControllers, __osEepromTimer, __osEepromTimerQ,
 * __osEepromTimerMsg). Not referenced: the code below uses the BOOT-segment globals of those names. */
OSPifRam __osContPifRam2;
u8 __osContLastCmd2;
u8 __osMaxControllers2;
OSTimer __osEepromTimer2;
OSMesgQueue __osEepromTimerQ2;
OSMesg __osEepromTimerMsg2;

/* This ROM predates the OS_CPU_COUNTER form in include/2.0L/PR/os_convert.h: golden loads the
 * 64-bit osClockRate global at 0x8002BD10 and divides by 1000000. */
#define GAME_USEC_TO_CYCLES(n) (((u64)(n) * (u64)osClockRate) / 1000000)

s32 osContInit2(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *data) {
    OSMesg dummy;
    s32 ret = 0;
    u64 time;
    OSTimer t;
    OSMesgQueue timerMesgQueue;

    if (__osContinitialized) {
        return 0;
    }
    __osContinitialized = 1;

    time = osGetTime();
    if (time < GAME_USEC_TO_CYCLES(500000)) {
        osCreateMesgQueue(&timerMesgQueue, &dummy, 1);
        osSetTimer(&t, GAME_USEC_TO_CYCLES(500000) - time, 0, &timerMesgQueue, &dummy);
        osRecvMesg(&timerMesgQueue, &dummy, OS_MESG_BLOCK);
    }

    __osMaxControllers = 4;

    __osPackRequestData(0);
    ret = __osSiRawStartDma(OS_WRITE, &__osContPifRam);
    osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

    ret = __osSiRawStartDma(OS_READ, &__osContPifRam);
    osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

    __osContGetInitData(bitpattern, data);

    __osContLastCmd = 0;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&__osEepromTimerQ, &__osEepromTimerMsg, 1);

    return ret;
}

void __osContGetInitData2(u8 *pattern, OSContStatus *data) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 bits;

    bits = 0;
    ptr = (u8 *) __osContPifRam.ramarray;
    for (i = 0; i < __osMaxControllers; i++, ptr += sizeof(__OSContRequesFormat)) {
        requestformat = *(__OSContRequesFormat *) ptr;
        data->errno = (requestformat.rxsize & 0xC0) >> 4;
        if (data->errno == 0) {
            data->type = (requestformat.typel << 8) | (requestformat.typeh);
            data->status = requestformat.status;
            bits |= 1 << i;
        }
        data++;
    }
    *pattern = bits;
}

void __osPackRequestData2(u8 cmd) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;

    for (i = 0; i < 16; i++) {
        __osContPifRam.ramarray[i] = 0;
    }
    __osContPifRam.pifstatus = 1;
    ptr = (u8 *) __osContPifRam.ramarray;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    for (i = 0; i < __osMaxControllers; i++) {
        *(__OSContRequesFormat *) ptr = requestformat;
        ptr += sizeof(__OSContRequesFormat);
    }
    *ptr = 0xFE;
}
