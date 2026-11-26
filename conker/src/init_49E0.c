#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals. IDO's .bss order depends on the
 * declarations before it (see init_50A0.c): after exactly these three headers it is the definition order. */
static s8 D_8003A580;
static u8 D_8003A581;
static u8 D_8003A582; // graphics task state
static u8 D_8003A583;
static u8 D_8003A584;
static OSTimer D_8003A588; // audio start timer
static u8 D_8003A5A8[0x20]; // not referenced
static OSScMsg D_8003A5C8; // pre-NMI message
u64 D_8003A5E8[OS_YIELD_DATA_SIZE / sizeof(u64)]; // the graphics task's yield buffer

#include "functions.h"
#include "variables.h"

static OSScTask *D_8002AC50 = NULL; // the current graphics task
static OSScTask *D_8002AC54 = NULL; // the audio task (from the D_8003B200 queue)
static OSScTask *D_8002AC58 = NULL; // the graphics task last started on the RSP
u8 D_8002AC5C = FALSE;              // a reset (pre-NMI) is pending
u8 D_8002AC60 = TRUE;
u16 D_8002AC64 = 800;
s32 D_8002AC68 = -1; // not referenced
static s8 D_8002AC6C = 0;

/* Messages received on the scheduler's interrupt queue D_8003B218. */
#define SCHED_MESG_RETRACE     0 /* VI retrace (osViSetEvent) */
#define SCHED_MESG_RDP_DONE    1 /* OS_EVENT_DP */
#define SCHED_MESG_SP_DONE     2 /* OS_EVENT_SP */
#define SCHED_MESG_AUDIO_TIMER 3 /* delay before starting the pending audio task */
#define SCHED_MESG_CONT_READ   6 /* timer set in game_48FD0.c: start a controller read */

/* Scheduler thread: dispatches VI retrace, RSP/RDP completion and timer messages. */
void func_100049E0(void *arg) {
    OSMesg msg;
    SchedulerClient *client;
    OSTime delay;

    D_8003A581 = 0;
    D_8003A582 = 0;
    D_8003A584 = 1;
    D_8003A583 = 0;
    D_8003B240.type = OS_SC_RETRACE_MSG;
    msg = NULL;
    arg = NULL; // matching: the thread argument is reused as a scratch variable (here and below)
    D_8003A5C8.type = OS_SC_PRE_NMI_MSG;
    while (TRUE) {
        osRecvMesg(&D_8003B218, &msg, OS_MESG_BLOCK);
        switch ((s32)msg) {
        case SCHED_MESG_RETRACE:
            for (client = D_8003B234; client != NULL; client = client->next) {
                if (!(client->flags & 1)) {
                    osSendMesg(client->msgQ, &D_8003B240, OS_MESG_NOBLOCK);
                }
            }
            if ((D_8003B238 != 0xFF) && (D_8003B238 < 0xFF)) {
                D_8003B238++;
            }
            if (D_8003B23A != 0) {
                D_8003B23A--;
            }
            arg = (void *)D_8003A581;
            if ((arg == NULL) && (D_8002AC6C == 0)) {
                if (osRecvMesg(&D_8003B200, (OSMesg *)&D_8002AC54, OS_MESG_NOBLOCK) == 0) {
                    delay = 200000;
                    if ((D_8003A582 != 0) || !(osAiGetStatus() & AI_STATUS_FIFO_FULL)) {
                        delay = 20000;
                    }
                    osSetTimer(&D_8003A588, delay, 0, &D_8003B218, (OSMesg)SCHED_MESG_AUDIO_TIMER);
                    D_8002AC6C = 1;
                }
            }
            if (D_8003A581 == 0) {
                func_10004DB0();
            }
            arg = NULL;
            break;
        case SCHED_MESG_SP_DONE:
            if (D_8003A582 == 3) {
                if (osSpTaskYielded(&D_8002AC50->list) == 1) {
                    osSpTaskLoad(&D_8002AC54->list);
                    osSpTaskStartGo(&D_8002AC54->list);
                    D_8003A581 = 1;
                    D_8003A582 = 4;
                } else {
                    D_8003A582 = 1;
                    osSpTaskLoad(&D_8002AC54->list);
                    osSpTaskStartGo(&D_8002AC54->list);
                    D_8003A581 = 1;
                    D_8003A583 = 0;
                }
            } else if (D_8003A581 != 0) {
                osSendMesg(D_8002AC54->msgQ, D_8002AC54->msg, OS_MESG_BLOCK);
                D_8003A581 = 0;
                if (D_8003A582 == 4) {
                    osSpTaskLoad(&D_8002AC50->list);
                    osSpTaskStartGo(&D_8002AC50->list);
                    D_8003A580 = 1;
                    D_8003A582 = 1;
                }
            } else {
                D_8003A583 = 0;
                if (D_8003A584 == 1) {
                    func_10004FE0();
                }
            }
            break;
        case SCHED_MESG_RDP_DONE:
            D_8003A584 = 1;
            if (D_8003A583 == 0) {
                func_10004FE0();
            }
            break;
        case SCHED_MESG_AUDIO_TIMER:
            D_8002AC6C = (s32)arg;
            if (D_8003A583 != 0) {
                osSpTaskYield();
                D_8003A582 = 3;
            } else {
                osSpTaskLoad(&D_8002AC54->list);
                osSpTaskStartGo(&D_8002AC54->list);
                D_8003A581 = 1;
            }
            break;
        case SCHED_MESG_CONT_READ:
            if (D_8002AC5C == 0) {
                osContStartReadData(&D_800BE900);
            }
            break;
        }
    }
}

/* Advances the graphics task state machine (D_8003A582) on a retrace while no audio task runs. */
void func_10004DB0(void) {
    if (D_8003A582 == 0) {
        if (osRecvMesg(&D_8003B1E8, (OSMesg *)&D_8002AC50, OS_MESG_NOBLOCK) == 0) {
            if ((osViGetCurrentFramebuffer() != D_8002AC50->framebuffer) &&
                (osViGetNextFramebuffer() != D_8002AC50->framebuffer) &&
                ((D_8003B23A == 0) || (D_8003B238 >= D_8003B239))) {
                if ((D_8003B238 != 0xFF) &&
                    ((D_8003B238 >= D_8003B239) || (D_8003B23A == 0))) {
                    D_8003B239 = D_8003B238;
                }
                func_10004F00();
                return;
            }
            D_8003A582 = 2;
        }
    } else if (D_8003A582 == 2) {
        if ((D_8003B23A == 0) || (D_8003B238 >= D_8003B239)) {
            func_10004F00();
        }
    } else if (D_8003A582 == 6) {
        func_10004FE0();
    }
}
/* Starts the current graphics task D_8002AC50 on the RSP unless a reset is pending. */
void func_10004F00(void) {
    if (D_8002AC5C == 0) {
        osSpTaskLoad(&D_8002AC50->list);
        osSpTaskStartGo(&D_8002AC50->list);
        D_8003A580 = 0;
        D_8002AC58 = D_8002AC50;
        D_8003A583 = 1;
        D_8003A584 = 0;
        if ((D_8003B238 == 255) ||
            ((D_8003B238 >= 11) && ((D_8003B238 >= 21) || (D_800C35EA[0] != 1)))) {
            D_8003B238 = 2;
        }
        D_800BE9E4 = D_8003B238;
        D_8003B238 = 0;
        D_8003A582 = 1;
        osSendMesg(D_8003B230, &D_8003B240, OS_MESG_NOBLOCK);
    }
}

/* Swaps in the finished frame, or waits (state 6) until at least one retrace has passed. */
void func_10004FE0(void) {
    if (D_8003B238 <= 0) {
        D_8003A582 = 6;
    } else {
        func_10005020();
    }
}

/* Finishes the current graphics task: swaps its framebuffer if requested and notifies its owner. */
void func_10005020(void) {
    void *framebuffer;

    D_8003A582 = 0;
    framebuffer = D_8002AC50->framebuffer;
    if ((D_8002AC50->flags & OS_SC_SWAPBUFFER) && (D_8002AC5C == 0)) {
        colourfb_drawScreenEffects((s32)framebuffer);
        osViSwapBuffer(framebuffer);
    }
    osSendMesg(D_8002AC50->msgQ, D_8002AC50->msg, OS_MESG_BLOCK);
}
