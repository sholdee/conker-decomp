#include <ultra64.h>

#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/texture.h"


void level_setupHauntedOuthouse(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_80090318, 0, -1, 0, 0, 0, 8);
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_8009031C, 0, -1, 0, 0, 8, 0);
}
