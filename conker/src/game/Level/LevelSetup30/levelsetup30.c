#include <ultra64.h>

#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/prop.h"
#include "conker/texture.h"


void level_setupPooBoss(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902E8, 0, -1, 0, 0, 0, -6);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902E8, 0, -1, 0, 1, 0, -6);
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902EC, 0, -1, 0, 0, 0, -3);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902EC, 0, -1, 0, 1, 0, -3);
    effect_newTimedEffect(300, -1, 59, -1, 0, 0, 0, 255, 1);
}

void level_setupUBend(void) {
    prop_createWaveDeformer(0, 255, -3477, -963, 684, 0.0f, 15000, 15.0f, -106, 30, 127, 127, 127, 127, 127, 127);
    prop_createWaveDeformer(4, 254,  2404, -992, 779, 0.0f,  2000, 15.0f, -106, 30, 127, 127, 127, 127, 127, 127);
}
