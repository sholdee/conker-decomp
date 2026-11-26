#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* gObjectFilters object filter: rejects objects with id 0x12, 0x23, 0x73 or 0x8A. */
s32 effect_filterExcludedTargets(Actor *obj, s32 arg1, s32 arg2) {
    u8 id = obj->character_type;

    if (id == 0x12 || id == 0x23 || id == 0x73 || id == 0x8A) {
        return FALSE;
    }
    return TRUE;
}

/* Destroys the effect's child object, if any (effect_destroyObject). */
void effect_destroyChildEffect(Effect_129DE0 *effect) {
    // matching: IDO copies the arg to $a1 and reloads child for the call only through a pointer to the field
    EffectHeader **slot = &effect->child;

    if (*slot != NULL) {
        effect_destroyObject(*slot);
    }
}

/* gTimedEffectDestroyFuncs callback: effect_destroyChildEffect, then effect_defaultDestroyTimedEffect on the effect. */
void effect_destroyWithChild(Effect_129DE0 *effect) {
    effect_destroyChildEffect(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}
