#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* User data of this effect kind: an object the effect owns. */
typedef struct EffectUserData1E5820 {
    /* 0x00 */ EffectHeader *child;
} EffectUserData1E5820;

/* Effect object (see EffectObj in game_174BF0.c); only the user data pointer is used here. */
struct Effect1E5820 {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ EffectUserData1E5820 *userData;
};

/* Destroys the object owned by this effect, if any. */
void effect_destroyRibbonParticle(Effect1E5820 *effect) {
    EffectUserData1E5820 *userData = effect->userData;
    EffectHeader *child = userData->child;

    if (child != NULL) {
        effect_destroyObject(child);
    }
}

/* Effect kind handler (gEffectObjDestroyFuncs): destroys the owned object, then runs the generic handler. */
void effect_destroyRibbon(Effect1E5820 *effect) {
    effect_destroyRibbonParticle(effect);
    effect_defaultDestroyEffectObj(effect);
}

/* Effect kind handler (gEffectObjFreeFuncs): destroys the owned object, then runs the generic handler. */
void effect_freeRibbon(Effect1E5820 *effect) {
    effect_destroyRibbonParticle(effect);
    effect_defaultFreeEffectObj(effect);
}
