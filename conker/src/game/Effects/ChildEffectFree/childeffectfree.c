#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Runs effect_destroyChildEffect on the object (which destroys its child object), then the shared effect_defaultFreeTimedEffect. */
void effect_freeWithChild(Effect_129DE0 *obj) {
    effect_destroyChildEffect(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}
