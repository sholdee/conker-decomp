#include <ultra64.h>
#include "conker/actor.h"
#include "conker/gfx.h"
#include "conker/prop.h"

void func_150CF0A0(WorldObject *obj) {
    WorldObject *prop;

    if ((obj->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (obj->object_status & 0x4) {
            if (movement_obj[0].mv->unk57 != 0) {
                prop = gfx_findObjectById(0xFE);
                prop->task_flag &= ~TASK_FLAG_STATE;
                prop->task_flag |= TASK_FLAG_GOING_ON;
                prop = gfx_findObjectById(0xFD);
                prop->task_flag &= ~TASK_FLAG_STATE;
                prop->task_flag |= TASK_FLAG_GOING_ON;
            }
        }
    } else {
        prop_bounceZ(obj);
    }
}
