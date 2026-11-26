#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void particle_freeAnchorEmitterSprite(LightEffect *obj) {
    effect_unlinkAnchorLight(obj);
    func_151617E4(obj);
}

/* AnchorEmitter destroy: frees its child effect, unregisters it and releases its anchor set. */
void particle_destroyAnchorEmitter(AnchorEmitter *emitter) {
    AnchorEmitterParams *params = &emitter->params;

    if (params->child != NULL) {
        effect_destroyObject(params->child);
    }
    sound_stopByCallback(particle_followEmitterSound, emitter, (s32)emitter->group.actor);
    effect_releaseAnchorSet(params->tmpl.anchorSet);
}

/* Sends event 0x2A about `actor` to every object of type 0x3E. */
void particle_sendActorGoneToEmitters(Actor *actor) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    effect_sendEventToType(0x3E, (s32)&event, 0x2A);
}
