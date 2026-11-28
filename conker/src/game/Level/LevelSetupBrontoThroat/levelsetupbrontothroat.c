#include <ultra64.h>

#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/texture.h"


void level_setupBrontoThroat(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902B8, 1, -1, 0, 0, 0, -8);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902B8, 1, -1, 0, 1, 0, -8);
}
