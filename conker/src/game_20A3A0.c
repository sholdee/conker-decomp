#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern OSTimer __osEepromTimer;
extern OSMesgQueue __osEepromTimerQ;
extern OSMesgQueue *D_800E0A20;
extern u8 D_800E0A24;
extern u8 *D_800E0A28;
extern s32 D_800E0A2C;

s32 func_151DCFD8(s32 arg0);

void func_151DCEF0(s32 arg0, u8 arg1, s32 arg2, s32 arg3) {
    while (func_151DCFD8(1) != 0) {
    }
    D_800E0A20 = arg0;
    D_800E0A24 = arg1;
    D_800E0A28 = arg2;
    D_800E0A2C = arg3;
    osSetTimer(&__osEepromTimer, 12000ULL * osClockRate / 1000000ULL, 0, &__osEepromTimerQ, &__osEepromTimerMsg);
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
