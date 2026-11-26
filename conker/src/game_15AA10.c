#include <ultra64.h>
#include "functions.h"
#include "variables.h"



// matching: D_800DC2B0 is a CamMsgQueue array; the original code indexes it as raw 0xB0-byte rows
// (typed CamMsgQueue access swaps the base/offset addu operands and changes the reloads).
typedef u8 Entry15[0xB0];

/* Queues message (type, arg) for the camera; arg is a pointer or a small integer, depending on the type. */
void func_1512D560(Camera *camera, s32 type, void *arg) {
    u8 *entry;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8] = type;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8 + 4] = (s32)arg;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    *(s32 *)&entry[0xAC] = *(s32 *)&entry[0xAC] + 1;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    if (*(s32 *)&entry[0xAC] == 0x14) {
        *(s32 *)&entry[0xAC] = 0;
    }
}

// matching: temp is a scratch variable in the parameter list (callers pass only the camera); a local
// changes the register allocation
CamMsg *func_1512D604(Camera *camera, s32 temp) {
    u8 *entry;
    CamMsg *ret;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    temp = *(s32 *)&entry[0xA8];
    ret = (CamMsg *)((s32)entry + (temp * 8));
    *(s32 *)&entry[0xA8] = temp + 1;
    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    if (*(s32 *)&entry[0xA8] == 0x14) {
        *(s32 *)&entry[0xA8] = 0;
    }
    return ret;
}


void func_1512D66C(Camera *camera) {
    D_800DC2B0[camera->CNumber].head = 0;
    D_800DC2B0[camera->CNumber].tail = 0;
}

s32 func_1512D6B0(Camera *camera) {
    u8 *entry;

    entry = (*(Entry15 **)&D_800DC2B0)[camera->CNumber];
    return *(s32 *)&entry[0xA8] == *(s32 *)&entry[0xAC];
}
