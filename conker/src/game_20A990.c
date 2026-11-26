#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "controller.h"

extern OSPifRam D_800E0A30;

/* func_151DD65C = __osPackEepWriteData.  Byte-perfect at the tree default.
 *
 * func_151DD4E0 = osEepromWrite: the osEepromRead shape (game_20A5F0) with `ptr += 4`
 * and no `i` local (the frame has no slot for one).
 *
 * The others are parked near-misses, all with golden's instruction sequence already:
 *   func_151DD710 (__osEepStatus)  mism=89,  n=109/108 -- one instruction too MANY.
 *   func_151DD8C0 (demo playback)  mism=35,  n=44/44   -- every instruction is golden's;
 *       the residue is register allocation (golden keeps &D_800BE748 in $a1 and the frame
 *       pointer in $a0 and starts short-lived temps at $t8; we use $a0/$v0/$t7).  Six
 *       spellings of the frame-pointer expression all land within 45-50.
 */

s32 func_151DD4E0(OSMesgQueue *mq, u8 address, u8 *buffer) {
    s32 ret = 0;
    u16 type;
    u8 *ptr;
    __OSContEepromFormat eepromformat;
    OSContStatus sdata;

    ptr = (u8 *) &D_800E0A30;
    __osSiGetAccess();
    ret = func_151DD710(mq, &sdata);
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
        func_151DD710(mq, &sdata);
    }
    func_151DD65C(address, buffer);
    __osSiRawStartDma(OS_WRITE, &D_800E0A30);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    __osSiRawStartDma(OS_READ, &D_800E0A30);
    __osContLastCmd = 5;
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    ptr += 4;
    eepromformat = *(__OSContEepromFormat *) ptr;
    ret = (eepromformat.rxsize & 0xC0) >> 4;
    __osSiRelAccess();
    return ret;
}

void func_151DD65C(u8 address, u8 *buffer) {
    u8 *ptr;
    __OSContEepromFormat eepromformat;
    s32 i;

    ptr = (u8 *) &D_800E0A30;
    D_800E0A30.pifstatus = 1;
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

s32 func_151DD710(OSMesgQueue *mq, OSContStatus *data) {
    s32 ret = 0;
    s32 i;
    u8 *ptr;
    __OSContRequesFormat requestformat;

    for (i = 0; i < 16; i++) {
        D_800E0A30.ramarray[i] = 0;
    }
    D_800E0A30.pifstatus = 1;
    ptr = (u8 *) &D_800E0A30;
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

    ret = __osSiRawStartDma(1, &D_800E0A30);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    __osContLastCmd = 0xFE;
    if (ret != 0) {
        return ret;
    }
    ret = __osSiRawStartDma(0, &D_800E0A30);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    if (ret != 0) {
        return ret;
    }
    ptr = (u8 *) &D_800E0A30;
    for (i = 0; i < 4; i++) {
        *ptr++ = 0;
    }
    requestformat = *(__OSContRequesFormat *) ptr;
    data->errno = (requestformat.rxsize & 0xC0) >> 4;
    data->type = (requestformat.typel << 8) | (requestformat.typeh);
    data->status = requestformat.status;
    return data->errno;
}

extern u8 *D_800E0A70;

void func_151DD8C0(void) {
    typedef struct { s8 x; s8 y; u16 time; u16 buttons; } Entry;
    OSContPad *input = D_800BE748;
    Entry *e;
    s32 idx;
    s32 next;
    s32 time;
    u16 held;
    held = input->button & START_BUTTON;
    idx = D_800E0A74;
    e = (Entry *)D_800E0A70 + idx;
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
    D_800E0A74 = idx + 1;
    if (D_800E0A74 >= 0x1F3) {
        D_800E0A74 = 0x1F3;
    }
}
