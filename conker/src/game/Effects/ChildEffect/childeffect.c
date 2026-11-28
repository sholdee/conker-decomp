#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/particle.h"

/* gObjectFilters object filter: rejects objects with id 0x12, 0x23, 0x73 or 0x8A. */
s32 effect_filterExcludedTargets(Actor *obj, s32 arg1, s32 arg2) {
    u8 id = obj->character_type;

    if (id == 0x12 || id == 0x23 || id == 0x73 || id == 0x8A) {
        return FALSE;
    }
    return TRUE;
}

/* Destroys the two-sided emitter's camera wobble (state.wobble), if any (effect_destroyObject). */
void effect_destroyChildEffect(TwoSidedEmitter *effect) {
    // matching: IDO copies the arg to $a1 and reloads the wobble for the call only through a pointer to the field
    CamWobble **slot = &effect->state.wobble;

    if (*slot != NULL) {
        effect_destroyObject(*slot);
    }
}

/* gTimedEffectDestroyFuncs callback of the two-sided emitters (set 37): effect_destroyChildEffect, then
 * effect_defaultDestroyTimedEffect on the effect. */
void effect_destroyWithChild(TwoSidedEmitter *effect) {
    effect_destroyChildEffect(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}
