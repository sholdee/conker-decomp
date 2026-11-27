#ifndef _OSINT_H
#define _OSINT_H
#include <os_internal.h>
typedef struct __OSEventState
{
    OSMesgQueue *messageQueue;
    OSMesg message;
} __OSEventState;
extern struct __osThreadTail
{
    OSThread *next;
    OSPri priority;
} __osThreadTail;

//maybe should be in exceptasm.h?
/* Conker: all but __osDequeueThread (os/thread.c) are in init/Boot/Exception/exceptasm.s. */
extern void __osEnqueueAndYield(OSThread **);
extern void __osDequeueThread(OSThread **, OSThread *);
extern void __osEnqueueThread(OSThread **, OSThread *);
extern OSThread *__osPopThread(OSThread **);
extern void __osDispatchThread(void);
extern void __osCleanupThread(void);

extern void __osSetTimerIntr(OSTime);
extern OSTime __osInsertTimer(OSTimer *);
extern void __osTimerInterrupt(void);
extern u32 __osProbeTLB(void *);
extern int     __osSpDeviceBusy(void);

extern OSThread *__osRunningThread;
extern OSThread *__osActiveQueue;
extern OSThread *__osFaultedThread;
extern OSThread *__osRunQueue;

extern OSTimer *__osTimerList;
extern OSTimer __osBaseTimer;
extern OSTime __osCurrentTime;
extern u32 __osBaseCounter;
extern u32 __osViIntrCount;
extern u32 __osTimerCounter;

extern __OSEventState __osEventStateTab[OS_NUM_EVENTS];


//not sure if this should be here
extern s32 osViClock;
extern void __osTimerServicesInit(void);
extern s32 __osAiDeviceBusy(void);
extern int __osDpDeviceBusy(void);
extern s32 __osLeoInterrupt(void);

/* The exception preamble (init/Boot/Exception/exceptasm.s): code that __osInitialize_common copies to the
 * exception vectors, one __osExceptionVector at a time. Declared as the SDK's os_internal_compat.h declares it. */
typedef struct {
    u32 inst1;
    u32 inst2;
    u32 inst3;
    u32 inst4;
} __osExceptionVector;
extern __osExceptionVector __osExceptionPreamble[];

/*
 * osResetType and osAppNMIBuffer under the names of their fixed addresses. The game code's
 * declarations of the same two symbols are in include/conker/os.h.
 */
extern s32 D_8000030C;
extern s32 D_8000031C[16];
#endif
