#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef void (*ObjectHandler)(Actor *obj);

extern ObjectHandler *D_8008F860[];

/* Runs the handler for obj's id group, picked by the surface type in obj->unk184 (and the level). */
void func_15192800(Actor *obj, s32 arg1) {
    s32 group;
    s32 *groupPtr; // matching: IDO computes &group before the NULL test only via a local
    s32 slot;

    groupPtr = &group;
    if (obj != NULL && func_151928B0(obj, groupPtr)) {
        switch (func_1510F8CC(obj->floorMaterial)) {
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
        if (D_8008F860[group][slot] != NULL) {
            D_8008F860[group][slot](obj);
        }
    }
}

/* Maps obj->id to a handler group (ids 0-4 -> 0, id 0x53 -> 1); returns FALSE for other ids. */
s32 func_151928B0(Actor *obj, s32 *group) {
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
