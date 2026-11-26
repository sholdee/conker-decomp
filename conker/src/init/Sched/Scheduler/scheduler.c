#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals. IDO's .bss order depends on the
 * declarations before it (see init/Sched/SchedInit/schedinit.c): after exactly these three headers it is the definition order. */
static s8 D_8003A580;
static u8 gSchedAudioRunning;
static u8 gSchedGfxState; // graphics task state
static u8 gSchedGfxRspBusy;
static u8 gSchedGfxRdpDone;
static OSTimer gSchedAudioTimer; // audio start timer
static u8 D_8003A5A8[0x20]; // not referenced
static OSScMsg gSchedPreNmiMsg; // pre-NMI message
u64 gGfxYieldBuffer[OS_YIELD_DATA_SIZE / sizeof(u64)]; // the graphics task's yield buffer

#include "functions.h"
#include "variables.h"

static OSScTask *gSchedGfxTask = NULL; // the current graphics task
static OSScTask *gSchedAudioTask = NULL; // the audio task (from the gSchedAudioTaskQueue queue)
static OSScTask *gSchedLastGfxTask = NULL; // the graphics task last started on the RSP
u8 gResetPending = FALSE;              // a reset (pre-NMI) is pending
u8 D_8002AC60 = TRUE;
u16 D_8002AC64 = 800;
s32 D_8002AC68 = -1; // not referenced
static s8 gSchedAudioTimerSet = 0;

/* Messages received on the scheduler's interrupt queue gSchedInterruptQueue. */
#define SCHED_MESG_RETRACE     0 /* VI retrace (osViSetEvent) */
#define SCHED_MESG_RDP_DONE    1 /* OS_EVENT_DP */
#define SCHED_MESG_SP_DONE     2 /* OS_EVENT_SP */
#define SCHED_MESG_AUDIO_TIMER 3 /* delay before starting the pending audio task */
#define SCHED_MESG_CONT_READ   6 /* timer set in game/Cont/PadRead/padread.c: start a controller read */

/* Scheduler thread: dispatches VI retrace, RSP/RDP completion and timer messages. */
void sched_threadMain(void *arg) {
    OSMesg msg;
    SchedulerClient *client;
    OSTime delay;

    gSchedAudioRunning = 0;
    gSchedGfxState = 0;
    gSchedGfxRdpDone = 1;
    gSchedGfxRspBusy = 0;
    gSchedRetraceMsg.type = OS_SC_RETRACE_MSG;
    msg = NULL;
    arg = NULL; // matching: the thread argument is reused as a scratch variable (here and below)
    gSchedPreNmiMsg.type = OS_SC_PRE_NMI_MSG;
    while (TRUE) {
        osRecvMesg(&gSchedInterruptQueue, &msg, OS_MESG_BLOCK);
        switch ((s32)msg) {
        case SCHED_MESG_RETRACE:
            for (client = gSchedClients; client != NULL; client = client->next) {
                if (!(client->flags & 1)) {
                    osSendMesg(client->msgQ, &gSchedRetraceMsg, OS_MESG_NOBLOCK);
                }
            }
            if ((gSchedRetraceCount != 0xFF) && (gSchedRetraceCount < 0xFF)) {
                gSchedRetraceCount++;
            }
            if (D_8003B23A != 0) {
                D_8003B23A--;
            }
            arg = (void *)gSchedAudioRunning;
            if ((arg == NULL) && (gSchedAudioTimerSet == 0)) {
                if (osRecvMesg(&gSchedAudioTaskQueue, (OSMesg *)&gSchedAudioTask, OS_MESG_NOBLOCK) == 0) {
                    delay = 200000;
                    if ((gSchedGfxState != 0) || !(osAiGetStatus() & AI_STATUS_FIFO_FULL)) {
                        delay = 20000;
                    }
                    osSetTimer(&gSchedAudioTimer, delay, 0, &gSchedInterruptQueue, (OSMesg)SCHED_MESG_AUDIO_TIMER);
                    gSchedAudioTimerSet = 1;
                }
            }
            if (gSchedAudioRunning == 0) {
                sched_updateGfxTask();
            }
            arg = NULL;
            break;
        case SCHED_MESG_SP_DONE:
            if (gSchedGfxState == 3) {
                if (osSpTaskYielded(&gSchedGfxTask->list) == 1) {
                    osSpTaskLoad(&gSchedAudioTask->list);
                    osSpTaskStartGo(&gSchedAudioTask->list);
                    gSchedAudioRunning = 1;
                    gSchedGfxState = 4;
                } else {
                    gSchedGfxState = 1;
                    osSpTaskLoad(&gSchedAudioTask->list);
                    osSpTaskStartGo(&gSchedAudioTask->list);
                    gSchedAudioRunning = 1;
                    gSchedGfxRspBusy = 0;
                }
            } else if (gSchedAudioRunning != 0) {
                osSendMesg(gSchedAudioTask->msgQ, gSchedAudioTask->msg, OS_MESG_BLOCK);
                gSchedAudioRunning = 0;
                if (gSchedGfxState == 4) {
                    osSpTaskLoad(&gSchedGfxTask->list);
                    osSpTaskStartGo(&gSchedGfxTask->list);
                    D_8003A580 = 1;
                    gSchedGfxState = 1;
                }
            } else {
                gSchedGfxRspBusy = 0;
                if (gSchedGfxRdpDone == 1) {
                    sched_swapOrWait();
                }
            }
            break;
        case SCHED_MESG_RDP_DONE:
            gSchedGfxRdpDone = 1;
            if (gSchedGfxRspBusy == 0) {
                sched_swapOrWait();
            }
            break;
        case SCHED_MESG_AUDIO_TIMER:
            gSchedAudioTimerSet = (s32)arg;
            if (gSchedGfxRspBusy != 0) {
                osSpTaskYield();
                gSchedGfxState = 3;
            } else {
                osSpTaskLoad(&gSchedAudioTask->list);
                osSpTaskStartGo(&gSchedAudioTask->list);
                gSchedAudioRunning = 1;
            }
            break;
        case SCHED_MESG_CONT_READ:
            if (gResetPending == 0) {
                osContStartReadData(&gSiMesgQueue);
            }
            break;
        }
    }
}

/* Advances the graphics task state machine (gSchedGfxState) on a retrace while no audio task runs. */
void sched_updateGfxTask(void) {
    if (gSchedGfxState == 0) {
        if (osRecvMesg(&gSchedTaskQueue, (OSMesg *)&gSchedGfxTask, OS_MESG_NOBLOCK) == 0) {
            if ((osViGetCurrentFramebuffer() != gSchedGfxTask->framebuffer) &&
                (osViGetNextFramebuffer() != gSchedGfxTask->framebuffer) &&
                ((D_8003B23A == 0) || (gSchedRetraceCount >= D_8003B239))) {
                if ((gSchedRetraceCount != 0xFF) &&
                    ((gSchedRetraceCount >= D_8003B239) || (D_8003B23A == 0))) {
                    D_8003B239 = gSchedRetraceCount;
                }
                sched_startGfxTask();
                return;
            }
            gSchedGfxState = 2;
        }
    } else if (gSchedGfxState == 2) {
        if ((D_8003B23A == 0) || (gSchedRetraceCount >= D_8003B239)) {
            sched_startGfxTask();
        }
    } else if (gSchedGfxState == 6) {
        sched_swapOrWait();
    }
}
/* Starts the current graphics task gSchedGfxTask on the RSP unless a reset is pending. */
void sched_startGfxTask(void) {
    if (gResetPending == 0) {
        osSpTaskLoad(&gSchedGfxTask->list);
        osSpTaskStartGo(&gSchedGfxTask->list);
        D_8003A580 = 0;
        gSchedLastGfxTask = gSchedGfxTask;
        gSchedGfxRspBusy = 1;
        gSchedGfxRdpDone = 0;
        if ((gSchedRetraceCount == 255) ||
            ((gSchedRetraceCount >= 11) && ((gSchedRetraceCount >= 21) || (gCutsceneStates[0] != 1)))) {
            gSchedRetraceCount = 2;
        }
        D_800BE9E4 = gSchedRetraceCount;
        gSchedRetraceCount = 0;
        gSchedGfxState = 1;
        osSendMesg(gSchedRetraceQueue, &gSchedRetraceMsg, OS_MESG_NOBLOCK);
    }
}

/* Swaps in the finished frame, or waits (state 6) until at least one retrace has passed. */
void sched_swapOrWait(void) {
    if (gSchedRetraceCount <= 0) {
        gSchedGfxState = 6;
    } else {
        sched_finishGfxTask();
    }
}

/* Finishes the current graphics task: swaps its framebuffer if requested and notifies its owner. */
void sched_finishGfxTask(void) {
    void *framebuffer;

    gSchedGfxState = 0;
    framebuffer = gSchedGfxTask->framebuffer;
    if ((gSchedGfxTask->flags & OS_SC_SWAPBUFFER) && (gResetPending == 0)) {
        colourfb_drawScreenEffects((s32)framebuffer);
        osViSwapBuffer(framebuffer);
    }
    osSendMesg(gSchedGfxTask->msgQ, gSchedGfxTask->msg, OS_MESG_BLOCK);
}
