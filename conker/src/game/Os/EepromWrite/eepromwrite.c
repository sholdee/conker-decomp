#include <ultra64.h>
#include "controller.h"
#include "siint.h"

/* One recorded frame of controller input, replayed by os_playDemoInput. */
struct DemoInput {
    s8 x;
    s8 y;
    u16 time;
    u16 buttons;
};

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
OSPifRam __osEepPifRam; /* __osEepPifRam */
void *gDemoInput; /* the recorded input: 500 struct DemoInput frames */
s32 gDemoInputIndex;

#include "functions.h"
#include "variables.h"

/*
 * osEepromWrite = osEepromWrite: the osEepromRead shape (game/Os/EepromRead/eepromread) with `ptr += 4`
 * and no `i` local (the frame has no slot for one).
 * __osPackEepWriteData = __osPackEepWriteData, __osEepStatus = __osEepStatus.
 * os_playDemoInput is Rare's demo playback: it replaces the input of the first controller with the next recorded frame.
 */

s32 osEepromWrite(OSMesgQueue *mq, u8 address, u8 *buffer) {
    s32 ret = 0;
    u16 type;
    u8 *ptr;
    __OSContEepromFormat eepromformat;
    OSContStatus sdata;

    ptr = (u8 *) &__osEepPifRam;
    __osSiGetAccess();
    ret = __osEepStatus(mq, &sdata);
    if (ret == 0) {
        type = sdata.type & (CONT_EEPROM | CONT_EEP16K);
        switch (type) {
            case CONT_EEPROM:
                if (address >= 64) {
                    ret = -1;
                }
                break;
            case CONT_EEPROM | CONT_EEP16K:
                if (address >= 256) {
                    ret = -1;
                }
                break;
            default:
                ret = CONT_NO_RESPONSE_ERROR;
        }
    }
    if (ret != 0) {
        __osSiRelAccess();
        return ret;
    }
    while (sdata.status & CONT_EEPROM_BUSY) {
        __osEepStatus(mq, &sdata);
    }
    __osPackEepWriteData(address, buffer);
    __osSiRawStartDma(OS_WRITE, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    __osSiRawStartDma(OS_READ, &__osEepPifRam);
    __osContLastCmd = 5;
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    ptr += 4;
    eepromformat = *(__OSContEepromFormat *) ptr;
    ret = (eepromformat.rxsize & 0xC0) >> 4;
    __osSiRelAccess();
    return ret;
}

void __osPackEepWriteData(u8 address, u8 *buffer) {
    u8 *ptr;
    __OSContEepromFormat eepromformat;
    s32 i;

    ptr = (u8 *) &__osEepPifRam;
    __osEepPifRam.pifstatus = 1;
    eepromformat.txsize = 10;
    eepromformat.rxsize = 1;
    eepromformat.cmd = 5;
    eepromformat.address = address;
    for (i = 0; i < 8; i++) {
        eepromformat.data[i] = *buffer++;
    }
    for (i = 0; i < 4; i++) {
        *ptr++ = 0;
    }
    *(__OSContEepromFormat *) ptr = eepromformat;
    ptr += sizeof(__OSContEepromFormat);
    *ptr = 0xFE;
}

s32 __osEepStatus(OSMesgQueue *mq, OSContStatus *data) {
    s32 ret = 0;
    s32 i;
    u8 *ptr;
    __OSContRequesFormat requestformat;

    for (i = 0; i < 16; i++) {
        __osEepPifRam.ramarray[i] = 0;
    }
    __osEepPifRam.pifstatus = 1;
    ptr = (u8 *) &__osEepPifRam;
    for (i = 0; i < 4; i++) {
        *ptr++ = 0;
    }
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = 0;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    *(__OSContRequesFormat *) ptr = requestformat;
    ptr += sizeof(__OSContRequesFormat);
    *ptr = 0xFE;

    ret = __osSiRawStartDma(1, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    __osContLastCmd = 0xFE;
    if (ret != 0) {
        return ret;
    }
    ret = __osSiRawStartDma(0, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    if (ret != 0) {
        return ret;
    }
    ptr = (u8 *) &__osEepPifRam;
    for (i = 0; i < 4; i++) {
        *ptr++ = 0;
    }
    requestformat = *(__OSContRequesFormat *) ptr;
    data->errno = (requestformat.rxsize & 0xC0) >> 4;
    data->type = (requestformat.typel << 8) | (requestformat.typeh);
    data->status = requestformat.status;
    return data->errno;
}

void os_playDemoInput(void) {
    OSContPad *input = gPads;
    struct DemoInput *e;
    s32 idx;
    s32 next;
    s32 time;
    u16 held;
    held = input->button & START_BUTTON;
    idx = gDemoInputIndex;
    e = (struct DemoInput *)gDemoInput + idx;
    input->stick_x = e->x;
    input->stick_y = e->y;
    input->button = e->buttons | held;
    D_800E0BD2 = 0;
    if (idx < 0x1F3) {
        next = e[1].time;
        if (D_800BE9E4 < next) {
            D_800E0BD2 = next - D_800BE9E4;
        }
    }
    time = e->time;
    D_800BE9E4 = time;
    gDemoInputIndex = idx + 1;
    if (gDemoInputIndex >= 0x1F3) {
        gDemoInputIndex = 0x1F3;
    }
}
