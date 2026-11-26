#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
u8 _MotorStopData[MAXCONTROLLERS][64]; // per channel: the PIF command block that stops the motor
u8 _MotorStartData[MAXCONTROLLERS][64]; // per channel: the PIF command block that starts the motor

#include "functions.h"
#include "variables.h"
#include "controller.h"
#include "siint.h"

s32 osMotorStop(OSPfs *device) {
    u8 *ptr;
    s32 result;
    s32 i;
    PakReply reply;

    __osSiGetAccess();
    __osContLastCmd = 3;
    __osSiRawStartDma(1, _MotorStopData[device->channel]);
    osRecvMesg(device->queue, 0, 1);
    __osSiRawStartDma(0, &__osPfsPifRam);
    osRecvMesg(device->queue, 0, 1);
    ptr = (u8 *)&__osPfsPifRam;
    if (device->channel != 0) {
        for (i = 0; i < device->channel; i++) {
            ptr++;
        }
    }
    reply = *(PakReply *)ptr;
    result = (reply.rxSize & 0xC0) >> 4;
    if (result == 0 && reply.dataCrc != 0) {
        result = 4;
    }
    __osSiRelAccess();
    return result;
}

s32 osMotorStart(OSPfs *device) {
    u8 *ptr;
    s32 result;
    s32 i;
    PakReply reply;

    __osSiGetAccess();
    __osContLastCmd = 3;
    __osSiRawStartDma(1, _MotorStartData[device->channel]);
    osRecvMesg(device->queue, 0, 1);
    __osSiRawStartDma(0, &__osPfsPifRam);
    osRecvMesg(device->queue, 0, 1);
    ptr = (u8 *)&__osPfsPifRam;
    if (device->channel != 0) {
        for (i = 0; i < device->channel; i++) {
            ptr++;
        }
    }
    reply = *(PakReply *)ptr;
    result = (reply.rxSize & 0xC0) >> 4;
    if (result == 0 && reply.dataCrc != 0xEB) {
        result = 4;
    }
    __osSiRelAccess();
    return result;
}
