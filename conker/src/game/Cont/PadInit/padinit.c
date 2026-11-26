#include <ultra64.h>

#include "functions.h"
#include "variables.h"

static OSContStatus gContStatus[MAXCONTROLLERS];
static OSMesg gSiMesgBuf[8]; // message buffer of the SI event queue gSiMesgQueue


void cont_initSiQueue(void) {
    osCreateMesgQueue(&gSiMesgQueue, gSiMesgBuf, 8);
    osSetEventMesg(OS_EVENT_SI, &gSiMesgQueue, D_800BE9E0);
    gPadPtrs[0] = &gPads[0];
    gPadPtrs[1] = &gPads[1];
    gPadPtrs[2] = &gPads[2];
    gPadPtrs[3] = &gPads[3];
    func_15007644();
}

void cont_init(void) {
    s32 i;
    s32 ret;
    u8 pattern;

    while (osContInit(&gSiMesgQueue, &pattern, gContStatus)) {
    }

    gNumControllers = 0;

    for (i = 0; i < 4; i++) {
        gRumblePakPresent[i] = 0;
        gRumbleOn[i] = 0;
        gRumbleWasOn[i] = 0;
        gMotorRunning[i] = 0;
        gRumbleTime[i] = 0.0f;
        gRumbleOnTime[i] = 1.0f;
        gRumbleOffTime[i] = 1.0f;
        gRumblePeriod[i] = gRumbleOnTime[i] + gRumbleOffTime[i];
        gPendingButtons[i] = 0;

        if (pattern & (1 << i)) {
            if (!(gContStatus[i].errno & CONT_NO_RESPONSE_ERROR)) {
                gConnectedPads |= (1 << i);
                gNumControllers++;
                if (gContStatus[i].type & CONT_JOYPORT) {
                    if (gContStatus[i].status & CONT_CARD_ON) {
                        ret = osPfsInit(&gSiMesgQueue, &gRumblePaks[i], i);
                        if ((ret == PFS_ERR_ID_FATAL) || (ret == PFS_ERR_DEVICE)) {
                            if (_MakeMotorData(&gSiMesgQueue, &gRumblePaks[i], i) == 0) {
                                gRumblePakPresent[i] = 1;
                                osMotorStop(&gRumblePaks[i]);
                            }
                        }
                    }
                }
            }
        }
    }

    D_8002AAE4 = 1;
}
