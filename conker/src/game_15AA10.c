#include <ultra64.h>
#include "functions.h"
#include "variables.h"



// matching: D_800DC2B0 is a CamMsgQueue array; the original code indexes it as raw 0xB0-byte rows
// (typed CamMsgQueue access swaps the base/offset addu operands and changes the reloads).
typedef u8 Entry15[0xB0];

void func_1512D560(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *entry;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8] = arg1;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    *(s32 *)&entry[*(s32 *)&entry[0xAC] * 8 + 4] = arg2;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    *(s32 *)&entry[0xAC] = *(s32 *)&entry[0xAC] + 1;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    if (*(s32 *)&entry[0xAC] == 0x14) {
        *(s32 *)&entry[0xAC] = 0;
    }
}

u8 *func_1512D604(u8 *arg0, s32 temp) {
    u8 *entry;
    u8 *ret;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    temp = *(s32 *)&entry[0xA8];
    ret = (u8 *)((s32)entry + (temp * 8));
    *(s32 *)&entry[0xA8] = temp + 1;
    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    if (*(s32 *)&entry[0xA8] == 0x14) {
        *(s32 *)&entry[0xA8] = 0;
    }
    return ret;
}


void func_1512D66C(u8 *arg0) {
    D_800DC2B0[arg0[0x23D]].head = 0;
    D_800DC2B0[arg0[0x23D]].tail = 0;
}

s32 func_1512D6B0(u8 *arg0) {
    u8 *entry;

    entry = (*(Entry15 **)&D_800DC2B0)[arg0[0x23D]];
    return *(s32 *)&entry[0xA8] == *(s32 *)&entry[0xAC];
}
