#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void level_setup24(void) {
    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902E4, 0, -1, 0, 0, 0, 4);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902E4, 0, -1, 0, 1, 0, 4);
}
