#include <ultra64.h>

#include "conker/effect.h"

void effect_clearHomingEffects(void) {
    bzero(&gHomingEffects, 32); // bzero
}
