#include <ultra64.h>
#include "effects.h"
#include "conker/effect.h"
#include "conker/trail.h"

/* Destroys the particle a ribbon effect (trail_spawnRibbon) drags along, if any. */
void effect_destroyRibbonParticle(EffectObj *effect) {
    RibbonState *state = effect->userData;
    LinkedParticle *linked = state->linked;

    if (linked != NULL) {
        effect_destroyObject(linked);
    }
}

/* Effect kind handler (gEffectObjDestroyFuncs[16], the ribbons): destroys the linked particle, then runs the generic handler. */
void effect_destroyRibbon(EffectObj *effect) {
    effect_destroyRibbonParticle(effect);
    effect_defaultDestroyEffectObj(effect);
}

/* Effect kind handler (gEffectObjFreeFuncs[16], the ribbons): destroys the linked particle, then runs the generic handler. */
void effect_freeRibbon(EffectObj *effect) {
    effect_destroyRibbonParticle(effect);
    effect_defaultFreeEffectObj(effect);
}
