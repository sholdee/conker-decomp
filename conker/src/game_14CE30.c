#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct PropLink {
    /* 0x00 */ WorldObject *prop;
    /* 0x04 */ s32 group;
} PropLink;

static s32 D_800DBFC0; /* number of used D_800DBFC8 entries */
static PropLink D_800DBFC8[4];

/* Clears the linked-prop table. */
void func_1511F980(void) {
    D_800DBFC0 = 0;
}

/*
 * Registers a prop in the linked-prop table (up to 4) and updates its motion state. On first
 * registration the state is initialised from mode; afterwards a prop that starts moving or returning
 * passes that state on to the other props of its link group.
 */
void func_1511F990(WorldObject *prop, s32 mode) {
    s32 index;
    s32 i;
    s32 added;
    s32 motion;
    s32 flag;
    s32 group;

    // matching: the redundant & 0xFF costs one integer web, setting IDO's t6..t9 temp rotation
    group = (s8)((prop->moveData.word >> 16) & 0xFF);
    added = FALSE;
    motion = prop->task_flag & TASK_FLAG_STATE;
    flag = prop->task_flag & TASK_FLAG_LOCKED;
    for (index = 0; index < D_800DBFC0; index++) {
        if (D_800DBFC8[index].prop == prop) {
            break;
        }
    }
    if (index == D_800DBFC0) {
        if (index >= 4) {
            return;
        }
        D_800DBFC8[index].prop = prop;
        D_800DBFC8[index].group = group;
        D_800DBFC0++;
        added = TRUE;
    }
    if (group >= 0) {
        if (added) {
            if (group == movement_obj[0].mv->spawnIndex) {
                if (mode == 0) {
                    if (motion != TASK_FLAG_OFF && motion != TASK_FLAG_GOING_OFF) {
                        motion = TASK_FLAG_GOING_OFF;
                    }
                    prop->u = (prop->moveData.word << 22) >> 22;
                } else if (mode == 1) {
                    motion = TASK_FLAG_ON;
                } else if (mode == 2) {
                    motion = TASK_FLAG_GOING_ON;
                } else if (mode == 3) {
                    if (motion != TASK_FLAG_OFF && motion != TASK_FLAG_GOING_OFF) {
                        if (flag) {
                            motion = TASK_FLAG_GOING_OFF;
                        }
                    } else if (!flag) {
                        motion = TASK_FLAG_ON;
                    }
                    prop->u = (prop->moveData.word << 22) >> 22;
                }
            } else if (mode == 0 || mode == 2 || mode == 3) {
                if (mode != 3 || flag) {
                    motion = TASK_FLAG_OFF;
                } else {
                    motion = TASK_FLAG_ON;
                }
            }
        } else if (motion == TASK_FLAG_GOING_OFF || motion == TASK_FLAG_GOING_ON) {
            /* pass the motion on to the rest of the group, skipping props already at the end it heads for */
            for (i = 0; i < D_800DBFC0; i++) {
                if (i == index || group != D_800DBFC8[i].group) {
                    continue;
                }
                if (!flag && (D_800DBFC8[i].prop->task_flag & TASK_FLAG_LOCKED)) {
                    continue;
                }
                if ((D_800DBFC8[i].prop->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON && motion == TASK_FLAG_GOING_ON) {
                    continue;
                }
                if ((D_800DBFC8[i].prop->task_flag & TASK_FLAG_STATE) == TASK_FLAG_OFF && motion == TASK_FLAG_GOING_OFF) {
                    continue;
                }
                D_800DBFC8[i].prop->task_flag &= ~TASK_FLAG_STATE;
                D_800DBFC8[i].prop->task_flag |= motion;
            }
        }
    }
    prop->task_flag &= ~TASK_FLAG_STATE;
    prop->task_flag |= motion;
}
