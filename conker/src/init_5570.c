#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#ifdef osMotorStop
#undef osMotorStop
#endif
#ifdef osMotorStart
#undef osMotorStart
#endif

typedef struct PakReply {
    u8 dummy;
    u8 txSize;
    u8 rxSize;
    u8 command;
    u16 address;
    u8 data[32];
    u8 dataCrc;
} PakReply;

void __osSiGetAccess(void);
void __osSiRelAccess(void);
extern u8 D_800429D0[64];
extern u8 __osContLastCmd;

s32 osMotorStop(OSPfs *device) {
    u8 *ptr;
    s32 result;
    s32 i;
    PakReply reply;

    __osSiGetAccess();
    __osContLastCmd = 3;
    __osSiRawStartDma(1, D_8003BC30[device->channel]);
    osRecvMesg(device->queue, 0, 1);
    __osSiRawStartDma(0, D_800429D0);
    osRecvMesg(device->queue, 0, 1);
    ptr = D_800429D0;
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
    __osSiRawStartDma(1, D_8003BD30[device->channel]);
    osRecvMesg(device->queue, 0, 1);
    __osSiRawStartDma(0, D_800429D0);
    osRecvMesg(device->queue, 0, 1);
    ptr = D_800429D0;
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
