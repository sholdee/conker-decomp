#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void effect_initQueue(void) {
    effect_allocQueue();
}

void effect_allocQueue(void) {
    D_800D98E0 = effect_newQueue(12, 1000, 1);
}

void func_15008BE0(void) {
}
