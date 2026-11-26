#include <ultra64.h>
#include "functions.h"
#include "variables.h"


struct DObj800CC5EC {
    u8 pad0[0x57];
    u8 unk57;
};

extern struct DObj800CC5EC *D_800CC5EC;

void func_150CF0A0(WorldObject *arg0) {
    WorldObject *temp;

    if ((arg0->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (arg0->object_status & 0x4) {
            if (D_800CC5EC->unk57 != 0) {
                temp = func_151149AC(0xFE);
                temp->task_flag &= ~TASK_FLAG_STATE;
                temp->task_flag |= TASK_FLAG_GOING_ON;
                temp = func_151149AC(0xFD);
                temp->task_flag &= ~TASK_FLAG_STATE;
                temp->task_flag |= TASK_FLAG_GOING_ON;
            }
        }
    } else {
        func_15117798(arg0);
    }
}
