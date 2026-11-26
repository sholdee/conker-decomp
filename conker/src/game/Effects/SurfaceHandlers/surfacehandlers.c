#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef void (*ObjectHandler)(Actor *obj);

/*
 * This file's .data: the handlers by id group (effect_getSurfaceHandlerGroup), then surface slot. Slot 2 (surface type 5 in level
 * 20) runs particle_attachSideEmitter (game/Particle/SideEmitter/sideemitter.c) for group 0 and particle_startOwnerSideTask (game/Particle/OwnerSideEmitter/ownersideemitter.c) for group 1.
 */
static ObjectHandler gSurfaceHandlersGroup0[] = { NULL, NULL, particle_attachSideEmitter, NULL };
static ObjectHandler gSurfaceHandlersGroup1[] = { NULL, NULL, particle_startOwnerSideTask, NULL };
static ObjectHandler *gSurfaceHandlerGroups[] = { gSurfaceHandlersGroup0, gSurfaceHandlersGroup1 };

/* Runs the handler for obj's id group, picked by the surface type in obj->unk184 (and the level). */
void effect_runSurfaceHandler(Actor *obj, s32 arg1) {
    s32 group;
    s32 *groupPtr; // matching: IDO computes &group before the NULL test only via a local
    s32 slot;

    groupPtr = &group;
    if (obj != NULL && effect_getSurfaceHandlerGroup(obj, groupPtr)) {
        switch (collision_getSurfaceType(obj->floorMaterial)) {
        case 5:
            if (level == 20) {
                slot = 2;
            } else {
                slot = 1;
            }
            break;
        case 6:
            slot = 3;
            break;
        default:
            slot = 1;
            break;
        }
        if (gSurfaceHandlerGroups[group][slot] != NULL) {
            gSurfaceHandlerGroups[group][slot](obj);
        }
    }
}

/* Maps obj->id to a handler group (ids 0-4 -> 0, id 0x53 -> 1); returns FALSE for other ids. */
s32 effect_getSurfaceHandlerGroup(Actor *obj, s32 *group) {
    s32 found;

    switch (obj->character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        found = TRUE;
        *group = 0;
        break;
    case 0x53:
        found = TRUE;
        *group = 1;
        break;
    default:
        found = FALSE;
        break;
    }
    return found;
}
