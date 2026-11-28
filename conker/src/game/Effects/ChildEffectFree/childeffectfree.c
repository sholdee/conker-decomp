#include <ultra64.h>
#include "conker/effect.h"
#include "conker/particle.h"

/* gTimedEffectFreeFuncs callback of the two-sided emitters (set 37): effect_destroyChildEffect (which destroys the camera
 * wobble), then the shared effect_defaultFreeTimedEffect. */
void effect_freeWithChild(TwoSidedEmitter *obj) {
    effect_destroyChildEffect(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}
