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

/* TriggerZone handler: sends message 0x60034 to `target` with its facing angle and the index of the actor
 * of the zone's ZoneEffect (-1 if none). */
void func_151A6B68(Actor *target, TriggerZone *zone) {
    ZoneEffect *obj;
    Actor *actor;
    s32 actorIndex;

    actor = NULL;
    obj = func_151A6BD8(zone);
    if (obj != NULL) {
        actor = obj->data.actor;
    }
    if (actor != NULL) {
        actorIndex = actor - movement_obj;
    } else {
        actorIndex = -1;
    }
    func_1505D024(target, 0x60034, target->langle, actorIndex);
}

/* Returns the ZoneEffect (callbackSet 0x2C) of `zone` in either object list, among the object types listed
 * in D_800A5770, or NULL. */
ZoneEffect *func_151A6BD8(TriggerZone *zone) {
    u8 i;
    u8 list;
    ZoneEffect *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = (ZoneEffect *)D_800DCE50[list][D_800A5770[i]]; obj != NULL;
                 obj = (ZoneEffect *)obj->base.header.next) {
                if (obj->base.callbackSet == 0x2C && zone == obj->data.zone) {
                    return obj;
                }
            }
        // matching: the label starts a new basic block, which keeps IDO from hoisting list++
        next_list:;
        }
    }
    return NULL;
}
