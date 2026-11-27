#include <ultra64.h>
#include "conker/actor.h"
#include "conker/gfx.h"
#include "conker/prop.h"

void func_150CF0A0(WorldObject *arg0) {
    WorldObject *temp;

    if ((arg0->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (arg0->object_status & 0x4) {
            if (movement_obj[0].mv->unk57 != 0) {
                temp = gfx_findObjectById(0xFE);
                temp->task_flag &= ~TASK_FLAG_STATE;
                temp->task_flag |= TASK_FLAG_GOING_ON;
                temp = gfx_findObjectById(0xFD);
                temp->task_flag &= ~TASK_FLAG_STATE;
                temp->task_flag |= TASK_FLAG_GOING_ON;
            }
        }
    } else {
        prop_bounceZ(arg0);
    }
}
