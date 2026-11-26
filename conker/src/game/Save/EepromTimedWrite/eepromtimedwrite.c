#include <ultra64.h>

/* The pending request: the caller's message queue, the next EEPROM block, the source and the bytes left. */
static OSMesgQueue *gEepromWriteQueue;
static u8 gEepromWriteBlock;
static u8 *gEepromWriteSource;
static s32 gEepromWriteLeft;

#include "functions.h"
#include "variables.h"
#include "controller.h"

s32 save_writeEeprom(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size) {
    while (save_pollEepromWrite(1) != 0) {
    }
    gEepromWriteQueue = mq;
    gEepromWriteBlock = block;
    gEepromWriteSource = buffer;
    gEepromWriteLeft = size;
    return osSetTimer(&__osEepromTimer, 12000ULL * osClockRate / 1000000ULL, 0, &__osEepromTimerQ, &__osEepromTimerMsg);
}

s32 save_pollEepromWrite(s32 arg0) {
    OSMesgQueue *queue;
    u8 block;
    u8 *data;

    if (gEepromWriteQueue == NULL) {
        return 0;
    }
    if (osRecvMesg(&__osEepromTimerQ, NULL, arg0) != -1) {
        if (gEepromWriteLeft <= 0) {
            gEepromWriteQueue = NULL;
            gEepromWriteLeft = 0;
            return 0;
        }

        queue = gEepromWriteQueue;
        block = gEepromWriteBlock;
        data = gEepromWriteSource;
        if (osEepromWrite(queue, block, data) != 0) {
            gEepromWriteQueue = NULL;
            gEepromWriteLeft = 0;
            return 0;
        }
        gEepromWriteLeft -= 8;
        gEepromWriteBlock++;
        gEepromWriteSource += 8;
        osSetTimer(&__osEepromTimer, 12000ULL * osClockRate / 1000000ULL, 0, &__osEepromTimerQ, &__osEepromTimerMsg);
        return 1;
    }
    return 0;
}
