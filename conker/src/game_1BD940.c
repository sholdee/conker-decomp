#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Object handled by func_15190454 (game_1BD8B0.c). */
typedef struct Game1BD8B0Object Game1BD8B0Object;

void func_15190490(Game1BD8B0Object *obj) {
    func_15190454(obj);
    func_151617E4(obj);
}

/* AnchorEmitter destroy: frees its child effect, unregisters it and releases its anchor set. */
void func_151904BC(AnchorEmitter *emitter) {
    AnchorEmitterParams *params = &emitter->params;

    if (params->child != NULL) {
        func_1516972C((EffectHeader *)params->child);
    }
    func_1000FD38(func_1518E298, emitter, (s32)emitter->owner);
    func_151D2B4C(params->tmpl.anchorSet);
}

/* Sends event 0x2A about `actor` to every object of type 0x3E. */
void func_15190518(Actor *actor) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    func_1516944C(0x3E, (s32)&event, 0x2A);
}
