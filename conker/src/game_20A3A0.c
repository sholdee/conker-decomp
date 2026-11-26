#include <ultra64.h>

/* The pending request: the caller's message queue, the next EEPROM block, the source and the bytes left. */
static OSMesgQueue *D_800E0A20;
static u8 D_800E0A24;
static u8 *D_800E0A28;
static s32 D_800E0A2C;

#include "functions.h"
#include "variables.h"
#include "controller.h"

s32 func_151DCEF0(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size) {
    while (func_151DCFD8(1) != 0) {
    }
    D_800E0A20 = mq;
    D_800E0A24 = block;
    D_800E0A28 = buffer;
    D_800E0A2C = size;
    return osSetTimer(&__osEepromTimer, 12000ULL * osClockRate / 1000000ULL, 0, &__osEepromTimerQ, &__osEepromTimerMsg);
}

s32 func_151DCFD8(s32 arg0) {
    OSMesgQueue *queue;
    u8 block;
    u8 *data;

    if (D_800E0A20 == NULL) {
        return 0;
    }
    if (osRecvMesg(&__osEepromTimerQ, NULL, arg0) != -1) {
        if (D_800E0A2C <= 0) {
            D_800E0A20 = NULL;
            D_800E0A2C = 0;
            return 0;
        }

        queue = D_800E0A20;
        block = D_800E0A24;
        data = D_800E0A28;
        if (func_151DD4E0(queue, block, data) != 0) {
            D_800E0A20 = NULL;
            D_800E0A2C = 0;
            return 0;
        }
        D_800E0A2C -= 8;
        D_800E0A24++;
        D_800E0A28 += 8;
        osSetTimer(&__osEepromTimer, 12000ULL * osClockRate / 1000000ULL, 0, &__osEepromTimerQ, &__osEepromTimerMsg);
        return 1;
    }
    return 0;
}
