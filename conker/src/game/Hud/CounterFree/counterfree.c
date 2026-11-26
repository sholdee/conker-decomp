#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Destructor of a counter effect: frees its panel and digits, then the effect itself. */
void hud_freeCounter(CounterEffect *effect) {
    effect_freeStatCounterParts(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/* Message handler: frees the object on message 0x57. */
void hud_counterMessage(EffectHeader *obj, s32 arg1, u8 msg) {
    if (msg == 0x57) {
        effect_destroyObject(obj);
    }
}
