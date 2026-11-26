#ifndef CONKER_SCHED_H
#define CONKER_SCHED_H

#include <ultra64.h>
#include <PR/sched.h>

/*
 * Subsystem "sched": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Sched/Scheduler/scheduler.c
 *   init/Sched/SchedInit/schedinit.c
 *
 * init/Sched/SchedInit/schedinit.c includes this header. A new type here can change the order of its .bss: see init/Sched/SchedInit/schedinit.c.
 */

/* A thread that receives the scheduler's retrace message; sched_addClient links it into gSchedClients. */
typedef struct SchedulerClient {
    /* 0x0 */ struct SchedulerClient *next;
    /* 0x4 */ OSMesgQueue *msgQ;
    /* 0x8 */ u32 flags; /* bit 0: don't send retrace messages */
} SchedulerClient;

/* init/Sched/Scheduler/scheduler.c */
void sched_threadMain(void *arg);
void sched_updateGfxTask(void);
void sched_startGfxTask(void);
void sched_swapOrWait(void);
void sched_finishGfxTask(void);
extern u8 gResetPending; // a reset (pre-NMI) is pending
extern u8 D_8002AC60;
extern u16 D_8002AC64;
extern u64 gGfxYieldBuffer[OS_YIELD_DATA_SIZE / sizeof(u64)]; // the graphics task's yield buffer

/* init/Sched/SchedInit/schedinit.c */
void sched_init(OSMesgQueue *queue);
void sched_addClient(SchedulerClient *client, OSMesgQueue *msgQ);
void sched_sendRetraceMsg(void);
void sched_startPreNmiThread(void);
void func_10005298(void);
void sched_preNmiThread(void *arg);
extern OSMesgQueue gSchedTaskQueue;
extern OSMesgQueue gSchedAudioTaskQueue;
extern OSMesgQueue gSchedInterruptQueue;
extern OSMesgQueue *gSchedRetraceQueue;
extern SchedulerClient *gSchedClients; /* scheduler client list */
extern u8 gSchedRetraceCount;
extern u8 D_8003B239;
extern u8 D_8003B23A;
extern OSScMsg gSchedRetraceMsg; /* retrace message */
extern u64 gSchedThreadStack[0x80]; /* scheduler thread stack; [0] holds a fill pattern */
extern OSMesg gSchedTaskMsgs[4];
extern OSMesg gSchedAudioTaskMsgs[4];
extern OSMesg gSchedInterruptMsgs[8];
extern OSThread gSchedThread; /* scheduler thread */
extern u64 gPreNmiThreadStack[0x30]; /* pre-NMI thread stack */
extern OSMesgQueue gPreNmiQueue;
extern OSMesg gPreNmiMsgs[2];
extern OSThread gPreNmiThread; /* pre-NMI thread */
extern u64 D_8003BBA0[0x10];

#endif
