/* SDK osCreatePiManager, rebuilt by Rare with the game's flags. piThread is not static here: init/Os/DevMgr/devmgr,
 * init/Rom/RomDma/romdma and init/Sched/SchedInit/schedinit stop and start the PI manager's thread. */
#include <ultra64.h>

/* This file's .bss, defined before piint.h: IDO lays out .bss in an order that depends on the declarations
 * before it (see init/Sched/SchedInit/schedinit.c). */
OSThread D_80035910;                  /* piThread */
u8 D_80035AC0[0x80];                  /* not referenced: the rest of Rare's 0x230-byte thread object (see vimgr.c) */
char piThreadStack[OS_PIM_STACKSIZE];
OSMesgQueue piEventQueue;
OSMesg D_80036B58[1];                 /* piEventBuf */

#include "piint.h"

OSDevMgr __osPiDevMgr = { 0 };
OSPiHandle *__osPiTable = NULL;

void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt)
{
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (!__osPiDevMgr.active)
    {
        osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
        osCreateMesgQueue(&piEventQueue, D_80036B58, 1);
        if (!__osPiAccessQueueEnabled)
            __osPiCreateAccessQueue();
        osSetEventMesg(OS_EVENT_PI, &piEventQueue, (OSMesg) 0x22222222);
        oldPri = -1;
        myPri = osGetThreadPri(NULL);
        if (myPri < pri)
        {
            oldPri = myPri;
            osSetThreadPri(NULL, pri);
        }
        savedMask = __osDisableInt();
        __osPiDevMgr.active = 1;
        __osPiDevMgr.thread = &D_80035910;
        __osPiDevMgr.cmdQueue = cmdQ;
        __osPiDevMgr.evtQueue = &piEventQueue;
        __osPiDevMgr.acsQueue = &__osPiAccessQueue;
        __osPiDevMgr.dma = osPiRawStartDma;
        __osPiDevMgr.edma = osEPiRawStartDma;
        osCreateThread(&D_80035910, 0, __osDevMgrMain, &__osPiDevMgr, &piThreadStack[OS_PIM_STACKSIZE], pri);
        osStartThread(&D_80035910);
        __osRestoreInt(savedMask);
        if (oldPri != -1)
        {
            osSetThreadPri(NULL, oldPri);
        }
    }
}
