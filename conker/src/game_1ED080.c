#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Destructor of a counter effect: frees its panel and digits, then the effect itself. */
void func_151BFBD0(CounterEffect *effect) {
    func_151BFB2C(effect);
    func_15149368(&effect->base);
}

/* Message handler: frees the object on message 0x57. */
void func_151BFBFC(EffectHeader *obj, s32 arg1, u8 msg) {
    if (msg == 0x57) {
        func_1516972C(obj);
    }
}
