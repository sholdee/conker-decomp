#include <ultra64.h>

#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/texture.h"


void level_setupW1VolcanoRace(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902E4, 0, -1, 0, 0, 0, 4);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902E4, 0, -1, 0, 1, 0, 4);
}
