#include <ultra64.h>
#include "effects.h"
#include "conker/actor.h"
#include "conker/effect.h"
#include "conker/particle.h"

void effect_destroyZoneEmitter(ZoneEmitter *obj) {
    particle_shutZoneEmitter(obj);
    particle_destroyQuadParticleDefault(&obj->head);
}

void effect_freeZoneEmitter(ZoneEmitter *obj) {
    particle_shutZoneEmitter(obj);
    particle_freeQuadParticleDefault(&obj->head);
}

/* TriggerZone handler: sends message 0x60034 to `target` with its facing angle and the index of the actor
 * of the zone's ZoneEffect (-1 if none). */
void func_151A6B68(Actor *target, TriggerZone *zone) {
    ZoneEffect *obj;
    Actor *actor;
    s32 actorIndex;

    actor = NULL;
    obj = effect_findZoneEffect(zone);
    if (obj != NULL) {
        actor = obj->data.actor;
    }
    if (actor != NULL) {
        actorIndex = actor - movement_obj;
    } else {
        actorIndex = -1;
    }
    actor_hitScripted(target, 0x60034, target->langle, actorIndex);
}

/* Returns the ZoneEffect (callbackSet 0x2C) of `zone` in either object list, among the object types listed
 * in gTimedEffectTypes, or NULL. */
ZoneEffect *effect_findZoneEffect(TriggerZone *zone) {
    u8 i;
    u8 list;
    ZoneEffect *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = (ZoneEffect *)gObjectLists[list][gTimedEffectTypes[i]]; obj != NULL;
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
