/* This build's symbol at 0x10005948 is libultra's static _MakeMotorData body;
 * hide os_motor.h's osMotorInit prototype so the real signature can be used. */
#define osMotorInit osMotorInit_os_motor_h
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef osMotorInit


#pragma GLOBAL_ASM("asm/nonmatchings/init_57E0/_MakeMotorData.s")

typedef struct PakReply {
    u8 dummy;
    u8 txSize;
    u8 rxSize;
    u8 command;
    u16 address;
    u8 data[32];
    u8 dataCrc;
} PakReply;

u8 __osContAddressCrc(u16);

void osMotorInit(s32 channel, u16 address, u8 *data, u8 *command) {
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
