#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef void (*ObjectHandler)(Actor *obj);

/*
 * This file's .data: the handlers by id group (func_151928B0), then surface slot. Slot 2 (surface type 5 in level
 * 20) runs func_15192920 (game_1BFDD0.c) for group 0 and func_151B1AB0 (game_1DEF60.c) for group 1.
 */
static ObjectHandler D_8008F840[] = { NULL, NULL, func_15192920, NULL };
static ObjectHandler D_8008F850[] = { NULL, NULL, func_151B1AB0, NULL };
static ObjectHandler *D_8008F860[] = { D_8008F840, D_8008F850 };

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
