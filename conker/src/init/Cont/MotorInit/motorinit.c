#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
static u8 _motorstopbuf[32]; // the motor data block that stops the motor (all 0)
static u8 _motorstartbuf[32]; // the motor data block that starts the motor (all 1)

#include "functions.h"
#include "variables.h"
#include "controller.h"

void _MakeMotorData(s32 channel, u16 address, u8 *data, u8 *command);

s32 osMotorInit(OSMesgQueue *queue, OSPfs *device, int channel) {
    s32 i;
    s32 result;
    u8 data[32];

    device->queue = queue;
    device->channel = channel;
    device->status = 0;
    device->activebank = 0x80;
    for (i = 0; i < 32; i++) {
        data[i] = 0x80;
    }
    result = __osContRamWrite(queue, channel, 0x400, data, 0);
    if (result == 2) {
        result = __osContRamWrite(queue, channel, 0x400, data, 0);
    }
    if (result != 0) {
        return result;
    }
    result = __osContRamRead(queue, channel, 0x400, data);
    if (result != 0) {
        return result;
    }
    if (data[31] != 0x80) {
        return 0xB;
    }
    for (i = 0; i < 32; i++) {
        _motorstartbuf[i] = 1;
        _motorstopbuf[i] = 0;
    }
    _MakeMotorData(channel, 0x600, _motorstartbuf, _MotorStartData[channel]);
    _MakeMotorData(channel, 0x600, _motorstopbuf, _MotorStopData[channel]);
    return 0;
}

void _MakeMotorData(s32 channel, u16 address, u8 *data, u8 *command) {
    u8 *ptr = command;
    PakReply reply;
    s32 i;

    for (i = 0; i < 15; i++) {
        ((u32 *)command)[i] = 0;
    }
    ((u32 *)command)[15] = 1;
    reply.dummy = 0xFF;
    reply.txSize = 0x23;
    reply.rxSize = 1;
    reply.command = 3;
    reply.address = (address << 5) | __osContAddressCrc(address);
    reply.dataCrc = 0xFF;
    for (i = 0; i < 32; i++) {
        reply.data[i] = *data++;
    }
    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = 0;
        }
    }
    *(PakReply *)ptr = reply;
    ptr += sizeof(PakReply);
    *ptr = 0xFE;
}
