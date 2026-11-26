#include <ultra64.h>

#include "functions.h"
#include "variables.h"

static OSContStatus D_800BE2C0[MAXCONTROLLERS];
static OSMesg D_800BE2D0[8]; // message buffer of the SI event queue D_800BE900


void func_150061B0(void) {
    osCreateMesgQueue(&D_800BE900, D_800BE2D0, 8);
    osSetEventMesg(OS_EVENT_SI, &D_800BE900, D_800BE9E0);
    D_800BE730[0] = &D_800BE748[0];
    D_800BE730[1] = &D_800BE748[1];
    D_800BE730[2] = &D_800BE748[2];
    D_800BE730[3] = &D_800BE748[3];
    func_15007644();
}

void func_15006234(void) {
    s32 i;
    s32 ret;
    u8 pattern;

    while (osContInit(&D_800BE900, &pattern, D_800BE2C0)) {
    }

    D_800BE72C = 0;

    for (i = 0; i < 4; i++) {
        D_800BE944[i] = 0;
        D_800BE93C[i] = 0;
        D_800BE940[i] = 0;
        D_800BE948[i] = 0;
        D_800BE950[i] = 0.0f;
        D_800BE960[i] = 1.0f;
        D_800BE970[i] = 1.0f;
        D_800BE980[i] = D_800BE960[i] + D_800BE970[i];
        D_800BE720[i] = 0;

        if (pattern & (1 << i)) {
            if (!(D_800BE2C0[i].errno & CONT_NO_RESPONSE_ERROR)) {
                D_800BE740 |= (1 << i);
                D_800BE72C++;
                if (D_800BE2C0[i].type & CONT_JOYPORT) {
                    if (D_800BE2C0[i].status & CONT_CARD_ON) {
                        ret = osPfsInit(&D_800BE900, &D_800BE760[i], i);
                        if ((ret == PFS_ERR_ID_FATAL) || (ret == PFS_ERR_DEVICE)) {
                            if (_MakeMotorData(&D_800BE900, &D_800BE760[i], i) == 0) {
                                D_800BE944[i] = 1;
                                osMotorStop(&D_800BE760[i]);
                            }
                        }
                    }
                }
            }
        }
    }

    D_8002AAE4 = 1;
}
