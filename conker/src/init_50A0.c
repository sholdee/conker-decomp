#include <ultra64.h>
#include <PR/sched.h>

/*
 * IDO lays out this file's .bss in an order that depends on the declarations before it, and the static
 * D_8003BC20 of func_100052A0 must come last. So this file does not include functions.h or variables.h,
 * which change often. It declares what it needs from other subsystems here, defines its .bss, and
 * includes its own header after the .bss. A new type in conker/sched.h can still change the order.
 */

/* From other subsystems. */
extern u32 __osShutdown;
extern u8 D_8002AAE0;
extern u8 D_8002AAE4;
extern u8 D_8002AC5C;
extern u8 D_8002AE40;
extern OSThread D_80031AE0;
extern OSThread D_80035910; /* piThread */
extern OSThread D_8003E3A0;
extern u8 D_80084064;
extern OSPfs D_800BE760[MAXCONTROLLERS]; /* rumble pak per controller channel */
extern OSMesgQueue D_800BE900;
extern u8 D_800BE944[];
extern u8 D_800BE948[];
extern OSMesg D_800BE990;
void func_100039C0(void);
void func_100093CC(void);

/*
 * Scheduler state, linked at 0x8003B1E8..0x8003BC28 in this order (with D_8003BC20 of func_100052A0 last).
 * They are defined before conker/sched.h declares them: an earlier extern changes the order.
 */
OSMesgQueue D_8003B1E8;
OSMesgQueue D_8003B200;
OSMesgQueue D_8003B218;
OSMesgQueue *D_8003B230;
struct SchedulerClient *D_8003B234; /* scheduler client list */
u8 D_8003B238;
u8 D_8003B239;
u8 D_8003B23A;
OSScMsg D_8003B240; /* retrace message */
u64 D_8003B260[0x80]; /* scheduler thread stack; [0] holds a fill pattern */
OSMesg D_8003B660[4];
OSMesg D_8003B670[4];
OSMesg D_8003B680[8];
OSThread D_8003B6A0; /* scheduler thread */
u64 D_8003B850[0x30]; /* pre-NMI thread stack */
OSMesgQueue D_8003B9D0;
OSMesg D_8003B9E8[2];
OSThread D_8003B9F0; /* pre-NMI thread */
u64 D_8003BBA0[0x10]; /* not referenced */

#include "conker/sched.h"

void func_100050A0(OSMesgQueue *queue) {
    D_8003B260[0] = 0x00000000FEFEFEFE;
    D_8003B234 = 0;
    D_8003B230 = queue;
    D_8003B238 = (u8)0xFF;
    D_8003B239 = (u8)0;
    D_8003B23A = (u8)0;
    osCreateMesgQueue(&D_8003B1E8, D_8003B660, 3);
    osCreateMesgQueue(&D_8003B200, D_8003B670, 1);
    osCreateMesgQueue(&D_8003B218, D_8003B680, 8);
    func_100039C0();
    osViSetEvent(&D_8003B218, 0, 1);
    osSetEventMesg(OS_EVENT_DP, &D_8003B218, (OSMesg *) 1);
    osSetEventMesg(OS_EVENT_SP, &D_8003B218, (OSMesg *) 2);
    osCreateThread(&D_8003B6A0, 0x14, (void *) func_100049E0, 0, &D_8003B660, 13);
    osStartThread(&D_8003B6A0);
}

void func_100051C8(SchedulerClient *client, OSMesgQueue *msgQ) {
    client->msgQ = msgQ;
    client->next = D_8003B234;
    D_8003B234 = client;
}

void func_100051E8(void) {
    osSendMesg(D_8003B230, &D_8003B240, 0);
}

void func_10005218(void) {
    osCreateMesgQueue(&D_8003B9D0, D_8003B9E8, 2);
    osSetEventMesg(OS_EVENT_PRENMI, &D_8003B9D0, (OSMesg *)5);
    osCreateThread(&D_8003B9F0, 0x15, (void *) func_100052A0, 0, &D_8003B9D0, OS_PRIORITY_RMON);
    osStartThread(&D_8003B9F0);
}

void func_10005298(void) {
}

#undef osMotorStop
extern s32 osMotorStop(OSPfs *pfs);
extern s32 _MakeMotorData(OSMesgQueue *mq, OSPfs *pfs, s32 channel);

void func_100052A0(s32 arg0) {
    static u64 D_8003BC20; // boottime?
    OSMesg mesg;
    s32 i;

    mesg = NULL;
    if (__osShutdown == 0) {
        osRecvMesg(&D_8003B9D0, &mesg, 1);
    }
    D_8002AC5C = 1;
    osStopThread(&D_80035910);
    osStopThread(&D_80031AE0);
    func_100093CC(); // stop thread D_8003E3A0 if D_8002AE40 not null
    D_8003BC20 = osGetTime();
    __osViInit();
    D_8002AAE0 = 1;
    osSetThreadPri(NULL, 11);

    if (D_8002AAE4 != 0) {
        if (D_80084064 == 0) {
            osRecvMesg(&D_800BE900, &D_800BE990, 1);
        }
        for (i = 0; i < 4; i++) { // 4 controllers?
            if (D_800BE944[i] != 0) {
                _MakeMotorData(&D_800BE900, &D_800BE760[i], i);
                osMotorStop(&D_800BE760[i]); // macro for __osMotorAccess
                D_800BE948[i] = 0;
            }
        }
    }

    while (osGetTime() < D_8003BC20 + 2272727) {
    }

    while (osGetTime() < D_8003BC20 + 7500000) {
    }

    osWritebackDCacheAll();

    while (1) {
    }
}
