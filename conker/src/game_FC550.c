#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150CF0A0(WorldObject *arg0) {
    WorldObject *temp;

    if ((arg0->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (arg0->object_status & 0x4) {
            if (movement_obj[0].mv->unk57 != 0) {
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
