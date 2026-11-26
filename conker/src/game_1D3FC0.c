#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_151A6B10(ZoneEmitter *obj) {
    func_151A6AB8(obj);
    func_1513CA6C(&obj->head);
}

void func_151A6B3C(ZoneEmitter *obj) {
    func_151A6AB8(obj);
    func_1513CAA0(&obj->head);
}

/* Sends message 0x60034 to `target` with its facing angle and the index of the actor owned by `owner`
 * (-1 if none). */
void func_151A6B68(Actor *target, s32 owner) {
    ListObj *obj;
    Actor *actor;
    s32 actorIndex;

    actor = NULL;
    obj = func_151A6BD8(owner);
    if (obj != NULL) {
        actor = obj->actor;
    }
    if (actor != NULL) {
        actorIndex = actor - movement_obj;
    } else {
        actorIndex = -1;
    }
    func_1505D024(target, 0x60034, target->langle, actorIndex);
}

/* Returns the first type-0x2C object in either object list, among the object types listed in D_800A5770,
 * whose owner is `owner`, or NULL. */
ListObj *func_151A6BD8(s32 owner) {
    u8 i;
    u8 list;
    ListObj *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = (ListObj *)D_800DCE50[list][D_800A5770[i]]; obj != NULL; obj = obj->next) {
                if (obj->callbackSet == 0x2C && owner == obj->owner) {
                    return obj;
                }
            }
        // matching: the label starts a new basic block, which keeps IDO from hoisting list++
        next_list:;
        }
    }
    return NULL;
}
