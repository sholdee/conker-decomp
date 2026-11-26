#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011C70(void) {
    gSprayAreas[0] = 0;
    gSprayAreas[1] = 0;
    gSprayAreas[2] = 0;
    sound_clearCooldowns();
}

void func_15011CA0(void) {
    sound_clearCooldowns();
}

void func_15011CC0(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902E4, 0, -1, 0, 0, 0, 4);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902E4, 0, -1, 0, 1, 0, 4);
}
