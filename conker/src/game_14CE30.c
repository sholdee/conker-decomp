#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Motion states in the low 2 bits of WorldObject.behaviorState (as in game_144C70.c). */
#define PROP_REST 0
#define PROP_RETURNING 1
#define PROP_MOVING 2
#define PROP_ARRIVED 3

typedef struct PropLink {
    /* 0x00 */ WorldObject *prop;
    /* 0x04 */ s32 group;
} PropLink;

typedef struct Owner {
    /* 0x000 */ u8 pad0[0x11B];
    /* 0x11B */ u8 unk11B;
} Owner;

extern s32 D_800DBFC0;          /* number of used D_800DBFC8 entries */
extern PropLink D_800DBFC8[4];
extern Owner *D_800CC5EC;

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
    group = (s8)((prop->param.word >> 16) & 0xFF);
    added = FALSE;
    motion = prop->behaviorState & 3;
    flag = prop->behaviorState & 4;
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
            if (group == D_800CC5EC->unk11B) {
                if (mode == 0) {
                    if (motion != PROP_REST && motion != PROP_RETURNING) {
                        motion = PROP_RETURNING;
                    }
                    prop->rotY = (prop->param.word << 22) >> 22;
                } else if (mode == 1) {
                    motion = PROP_ARRIVED;
                } else if (mode == 2) {
                    motion = PROP_MOVING;
                } else if (mode == 3) {
                    if (motion != PROP_REST && motion != PROP_RETURNING) {
                        if (flag) {
                            motion = PROP_RETURNING;
                        }
                    } else if (!flag) {
                        motion = PROP_ARRIVED;
                    }
                    prop->rotY = (prop->param.word << 22) >> 22;
                }
            } else if (mode == 0 || mode == 2 || mode == 3) {
                if (mode != 3 || flag) {
                    motion = PROP_REST;
                } else {
                    motion = PROP_ARRIVED;
                }
            }
        } else if (motion == PROP_RETURNING || motion == PROP_MOVING) {
            /* pass the motion on to the rest of the group, skipping props already at the end it heads for */
            for (i = 0; i < D_800DBFC0; i++) {
                if (i == index || group != D_800DBFC8[i].group) {
                    continue;
                }
                if (!flag && (D_800DBFC8[i].prop->behaviorState & 4)) {
                    continue;
                }
                if ((D_800DBFC8[i].prop->behaviorState & 3) == PROP_ARRIVED && motion == PROP_MOVING) {
                    continue;
                }
                if ((D_800DBFC8[i].prop->behaviorState & 3) == PROP_REST && motion == PROP_RETURNING) {
                    continue;
                }
                D_800DBFC8[i].prop->behaviorState &= ~3;
                D_800DBFC8[i].prop->behaviorState |= motion;
            }
        }
    }
    prop->behaviorState &= ~3;
    prop->behaviorState |= motion;
}
