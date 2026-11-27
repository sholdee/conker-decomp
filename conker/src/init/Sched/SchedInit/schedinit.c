#include <ultra64.h>
#include <PR/sched.h>

/*
 * IDO lays out this file's .bss in an order that depends on the declarations before it, and the static
 * gPreNmiTime of sched_preNmiThread must come last. So this file does not include the other subsystems' headers,
 * which change often. It declares what it needs from other subsystems here, defines its .bss, and
 * includes its own header after the .bss. A new type in conker/sched.h can still change the order.
 */

/* From other subsystems: conker/boot.h, cont.h and sched.h declare them too, and conker/os.h declares libultra's
 * __osShutdown and piThread (D_80035910). */
/* matching: these lines also set the .bss order; with fewer declarations here the .bss moves */
extern u32 __osShutdown;
extern u8 gPageFaultsDisabled;
extern u8 gPadInitDone;
extern u8 gResetPending;
extern OSThread gMainThread;
extern OSThread D_80035910; /* piThread */
extern u8 gContReadDone;
extern OSPfs gRumblePaks[MAXCONTROLLERS]; /* rumble pak per controller channel */
extern OSMesgQueue gSiMesgQueue;
extern u8 gRumblePakPresent[];
extern u8 gMotorRunning[];
extern OSMesg gSiMesg;
void boot_initScreen(void);

/*
 * Scheduler state, linked at 0x8003B1E8..0x8003BC28 in this order (with gPreNmiTime of sched_preNmiThread last).
 * They are defined before conker/sched.h declares them: an earlier extern changes the order.
 */
OSMesgQueue gSchedTaskQueue;
OSMesgQueue gSchedAudioTaskQueue;
OSMesgQueue gSchedInterruptQueue;
OSMesgQueue *gSchedRetraceQueue;
struct SchedulerClient *gSchedClients; /* scheduler client list */
u8 gSchedRetraceCount;
u8 D_8003B239;
u8 D_8003B23A;
OSScMsg gSchedRetraceMsg; /* retrace message */
u64 gSchedThreadStack[0x80]; /* scheduler thread stack; [0] holds a fill pattern */
OSMesg gSchedTaskMsgs[4];
OSMesg gSchedAudioTaskMsgs[4];
OSMesg gSchedInterruptMsgs[8];
OSThread gSchedThread; /* scheduler thread */
u64 gPreNmiThreadStack[0x30]; /* pre-NMI thread stack */
OSMesgQueue gPreNmiQueue;
OSMesg gPreNmiMsgs[2];
OSThread gPreNmiThread; /* pre-NMI thread */
u64 D_8003BBA0[0x10]; /* not referenced */

#include "conker/sched.h"

void sched_init(OSMesgQueue *queue) {
    gSchedThreadStack[0] = 0x00000000FEFEFEFE;
    gSchedClients = 0;
    gSchedRetraceQueue = queue;
    gSchedRetraceCount = 0xFF;
    D_8003B239 = 0;
    D_8003B23A = 0;
    osCreateMesgQueue(&gSchedTaskQueue, gSchedTaskMsgs, 3);
    osCreateMesgQueue(&gSchedAudioTaskQueue, gSchedAudioTaskMsgs, 1);
    osCreateMesgQueue(&gSchedInterruptQueue, gSchedInterruptMsgs, 8);
    boot_initScreen();
    osViSetEvent(&gSchedInterruptQueue, 0, 1);
    osSetEventMesg(OS_EVENT_DP, &gSchedInterruptQueue, (OSMesg)1);
    osSetEventMesg(OS_EVENT_SP, &gSchedInterruptQueue, (OSMesg)2);
    osCreateThread(&gSchedThread, 0x14, sched_threadMain, NULL, &gSchedThreadStack[0x80], 13);
    osStartThread(&gSchedThread);
}

void sched_addClient(SchedulerClient *client, OSMesgQueue *msgQ) {
    client->msgQ = msgQ;
    client->next = gSchedClients;
    gSchedClients = client;
}

void sched_sendRetraceMsg(void) {
    osSendMesg(gSchedRetraceQueue, &gSchedRetraceMsg, 0);
}

void sched_startPreNmiThread(void) {
    osCreateMesgQueue(&gPreNmiQueue, gPreNmiMsgs, 2);
    osSetEventMesg(OS_EVENT_PRENMI, &gPreNmiQueue, (OSMesg)5);
    osCreateThread(&gPreNmiThread, 0x15, sched_preNmiThread, NULL, &gPreNmiThreadStack[0x30], OS_PRIORITY_RMON);
    osStartThread(&gPreNmiThread);
}

void func_10005298(void) {
}

#include "conker/cont.h"
#include "conker/audio.h"

void sched_preNmiThread(void *arg) {
    static u64 gPreNmiTime; // boottime?
    OSMesg mesg;
    s32 i;

    mesg = NULL;
    if (__osShutdown == 0) {
        osRecvMesg(&gPreNmiQueue, &mesg, 1);
    }
    gResetPending = 1;
    osStopThread(&D_80035910);
    osStopThread(&gMainThread);
    audio_stopThread(); // stop thread gAudioThread if gAudioThreadStarted not null
    gPreNmiTime = osGetTime();
    __osViInit();
    gPageFaultsDisabled = 1;
    osSetThreadPri(NULL, 11);

    if (gPadInitDone != 0) {
        if (gContReadDone == 0) {
            osRecvMesg(&gSiMesgQueue, &gSiMesg, 1);
        }
        for (i = 0; i < 4; i++) { // 4 controllers?
            if (gRumblePakPresent[i] != 0) {
                osMotorInit(&gSiMesgQueue, &gRumblePaks[i], i);
                osMotorStop(&gRumblePaks[i]);
                gMotorRunning[i] = 0;
            }
        }
    }

    while (osGetTime() < gPreNmiTime + 2272727) {
    }

    while (osGetTime() < gPreNmiTime + 7500000) {
    }

    osWritebackDCacheAll();

    while (1) {
    }
}
