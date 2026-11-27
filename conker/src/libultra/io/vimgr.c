#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/rcp.h"
#include "PRinternal/viint.h"

// osint.h's declarations that this file uses. The whole header adds enough types to the -g3
// symbol table that IDO then allocates the function-local `retrace` first in .bss, not last.
extern void __osTimerServicesInit(void);
extern void __osTimerInterrupt(void);
extern OSTime __osCurrentTime;
extern u32 __osBaseCounter;
extern u32 __osViIntrCount;

OSDevMgr __osViDevMgr = { 0 };
#if BUILD_VERSION >= VERSION_J
u32 __additional_scanline = 0;
#endif
static OSThread viThread ALIGNED(0x8);
// Rare's thread object is 0x230 bytes: init/Boot/Exception/exceptasm.s saves all 32 FP registers (the SDK's
// OSThread ends at 0x1B0). C does not reference the other 0x80 bytes.
static u8 D_80036D50[0x80];
static STACK(viThreadStack, OS_VIM_STACKSIZE) ALIGNED(0x10);
static OSMesgQueue viEventQueue ALIGNED(0x8);
static OSMesg viEventBuf[5] ALIGNED(0x8);
static OSIoMesg viRetraceMsg ALIGNED(0x8);
static OSIoMesg viCounterMsg ALIGNED(0x8);

static void viMgrMain(void* arg);
void osCreateViManager(OSPri pri) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

#ifdef _DEBUG
    if ((pri < OS_PRIORITY_IDLE) || (pri > OS_PRIORITY_MAX)) {
        __osError(ERR_OSCREATEVIMANAGER, 1, pri);
        return;
    }
#endif

    if (__osViDevMgr.active) {
        return;
    }
    __osTimerServicesInit();
#if BUILD_VERSION >= VERSION_J
    __additional_scanline = 0;
#endif
    osCreateMesgQueue(&viEventQueue, viEventBuf, ARRLEN(viEventBuf));
    viRetraceMsg.hdr.type = OS_MESG_TYPE_VRETRACE;
    viRetraceMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    viRetraceMsg.hdr.retQueue = NULL;
    viCounterMsg.hdr.type = OS_MESG_TYPE_COUNTER;
    viCounterMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    viCounterMsg.hdr.retQueue = NULL;
    osSetEventMesg(OS_EVENT_VI, &viEventQueue, &viRetraceMsg);
    osSetEventMesg(OS_EVENT_COUNTER, &viEventQueue, &viCounterMsg);
    oldPri = -1;
    myPri = osGetThreadPri(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(NULL, pri);
    }

    savedMask = __osDisableInt();
    __osViDevMgr.active = TRUE;
    __osViDevMgr.thread = &viThread;
    __osViDevMgr.cmdQueue = &viEventQueue;
    __osViDevMgr.evtQueue = &viEventQueue;
    __osViDevMgr.acsQueue = NULL;
    __osViDevMgr.dma = NULL;
    __osViDevMgr.edma = NULL;
    osCreateThread(&viThread, 0, viMgrMain, &__osViDevMgr, STACK_START(viThreadStack), pri);
    __osViInit();
    osStartThread(&viThread);
    __osRestoreInt(savedMask);

    if (oldPri != -1) {
        osSetThreadPri(NULL, oldPri);
    }
}

static void viMgrMain(void* arg) {
    __OSViContext* vc;
    OSDevMgr* dm;
    OSIoMesg* mb;
    static u16 retrace;
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = __osViGetCurrentContext();
    retrace = vc->retraceCount;
    if (retrace == 0) {
        retrace = 1;
    }
    dm = (OSDevMgr*)arg;

    while (TRUE) {
        osRecvMesg(dm->evtQueue, (OSMesg)&mb, OS_MESG_BLOCK);
        switch (mb->hdr.type) {
            case OS_MESG_TYPE_VRETRACE:
                __osViSwapContext();
                retrace--;

                if (retrace == 0) {
                    vc = __osViGetCurrentContext();
                    if (vc->msgq != NULL) {
                        osSendMesg(vc->msgq, vc->msg, OS_MESG_NOBLOCK);
                    }
                    retrace = vc->retraceCount;
                }

                __osViIntrCount++;

                if (first) {
                    count = osGetCount();
                    __osCurrentTime = count;
                    first = 0;
                }

                count = __osBaseCounter;
                __osBaseCounter = osGetCount();
                count = __osBaseCounter - count;
                __osCurrentTime = __osCurrentTime + count;
                break;
            case OS_MESG_TYPE_COUNTER:
                __osTimerInterrupt();
                break;
            default:
                break;
        }
    }
}
