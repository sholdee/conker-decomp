#include <ultra64.h>
#include "conker/actor.h"
#include "conker/prop.h"

typedef struct PropLink {
    /* 0x00 */ WorldObject *prop;
    /* 0x04 */ s32 group;
} PropLink;

static s32 gNumPropLinks; /* number of used gPropLinks entries */
static PropLink gPropLinks[4];

/* Clears the linked-prop table. */
void prop_clearLinks(void) {
    gNumPropLinks = 0;
}

/*
 * Registers a prop in the linked-prop table (up to 4) and updates its motion state. On first
 * registration the state is initialised from mode; afterwards a prop that starts moving or returning
 * passes that state on to the other props of its link group.
 */
void prop_registerLink(WorldObject *prop, s32 mode) {
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
    for (index = 0; index < gNumPropLinks; index++) {
        if (gPropLinks[index].prop == prop) {
            break;
        }
    }
    if (index == gNumPropLinks) {
        if (index >= 4) {
            return;
        }
        gPropLinks[index].prop = prop;
        gPropLinks[index].group = group;
        gNumPropLinks++;
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
            for (i = 0; i < gNumPropLinks; i++) {
                if (i == index || group != gPropLinks[i].group) {
                    continue;
                }
                if (!flag && (gPropLinks[i].prop->task_flag & TASK_FLAG_LOCKED)) {
                    continue;
                }
                if ((gPropLinks[i].prop->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON && motion == TASK_FLAG_GOING_ON) {
                    continue;
                }
                if ((gPropLinks[i].prop->task_flag & TASK_FLAG_STATE) == TASK_FLAG_OFF && motion == TASK_FLAG_GOING_OFF) {
                    continue;
                }
                gPropLinks[i].prop->task_flag &= ~TASK_FLAG_STATE;
                gPropLinks[i].prop->task_flag |= motion;
            }
        }
    }
    prop->task_flag &= ~TASK_FLAG_STATE;
    prop->task_flag |= motion;
}
