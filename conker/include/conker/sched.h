#ifndef CONKER_SCHED_H
#define CONKER_SCHED_H

#include <ultra64.h>
#include <PR/sched.h>

/*
 * Subsystem "sched": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_49E0.c
 *   init_50A0.c
 *
 * init_50A0.c includes this header. A new type here can change the order of its .bss: see init_50A0.c.
 */

/* A thread that receives the scheduler's retrace message; func_100051C8 links it into D_8003B234. */
typedef struct SchedulerClient {
    /* 0x0 */ struct SchedulerClient *next;
    /* 0x4 */ OSMesgQueue *msgQ;
    /* 0x8 */ u32 flags; /* bit 0: don't send retrace messages */
} SchedulerClient;

/* init_49E0.c */
void func_100049E0(void *arg);
void func_10004DB0(void);
void func_10004F00(void);
void func_10004FE0(void);
void func_10005020(void);
extern u8 D_8002AC5C; // a reset (pre-NMI) is pending
extern u8 D_8002AC60;
extern u16 D_8002AC64;

/* init_50A0.c */
void func_100050A0(OSMesgQueue *queue);
void func_100051C8(SchedulerClient *client, OSMesgQueue *msgQ);
void func_100051E8(void);
void func_10005218(void);
void func_10005298(void);
void func_100052A0(s32 arg0);
extern OSMesgQueue D_8003B1E8;
extern OSMesgQueue D_8003B200;
extern OSMesgQueue D_8003B218;
extern OSMesgQueue *D_8003B230;
extern SchedulerClient *D_8003B234; /* scheduler client list */
extern u8 D_8003B238;
extern u8 D_8003B239;
extern u8 D_8003B23A;
extern OSScMsg D_8003B240; /* retrace message */
extern u64 D_8003B260[0x80]; /* scheduler thread stack; [0] holds a fill pattern */
extern OSMesg D_8003B660[4];
extern OSMesg D_8003B670[4];
extern OSMesg D_8003B680[8];
extern OSThread D_8003B6A0; /* scheduler thread */
extern u64 D_8003B850[0x30]; /* pre-NMI thread stack */
extern OSMesgQueue D_8003B9D0;
extern OSMesg D_8003B9E8[2];
extern OSThread D_8003B9F0; /* pre-NMI thread */
extern u64 D_8003BBA0[0x10];

#endif
