#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void effect_clearHomingEffects(void) {
    bzero(&gHomingEffects, 32); // bzero
}
