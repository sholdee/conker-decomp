#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
CamMsgQueue *gCameraMsgQueues; // one per camera (allocated by game_32740.c)

#include "functions.h"
#include "variables.h"



// matching: gCameraMsgQueues is a CamMsgQueue array; the original code indexes it as raw 0xB0-byte rows
// (typed CamMsgQueue access swaps the base/offset addu operands and changes the reloads).
typedef u8 Entry15[0xB0];

/* Queues message (type, arg) for the camera; arg is a pointer or a small integer, depending on the type. */
void camera_queueMessage(Camera *camera, s32 type, void *arg) {
    u8 *entry;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8] = type;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8 + 4] = (s32)arg;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    *(s32 *)&entry[0xAC] = *(s32 *)&entry[0xAC] + 1;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    if (*(s32 *)&entry[0xAC] == 0x14) {
        *(s32 *)&entry[0xAC] = 0;
    }
}

// matching: temp is a scratch variable in the parameter list (callers pass only the camera); a local
// changes the register allocation
CamMsg *camera_popMessage(Camera *camera, s32 temp) {
    u8 *entry;
    CamMsg *ret;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    temp = *(s32 *)&entry[0xA8];
    ret = (CamMsg *)((s32)entry + (temp * 8));
    *(s32 *)&entry[0xA8] = temp + 1;
    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    if (*(s32 *)&entry[0xA8] == 0x14) {
        *(s32 *)&entry[0xA8] = 0;
    }
    return ret;
}


void camera_clearMessages(Camera *camera) {
    gCameraMsgQueues[camera->CNumber].head = 0;
    gCameraMsgQueues[camera->CNumber].tail = 0;
}

s32 camera_isQueueEmpty(Camera *camera) {
    u8 *entry;

    entry = (*(Entry15 **)&gCameraMsgQueues)[camera->CNumber];
    return *(s32 *)&entry[0xA8] == *(s32 *)&entry[0xAC];
}
