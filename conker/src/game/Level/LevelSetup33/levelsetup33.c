#include <ultra64.h>

#include "functions.h"
#include "variables.h"

struct TexScroll **gSetup33TexScrolls; /* four texture scrolls */


void level_setup33(void) {
    gSetup33TexScrolls = allocate_memory(16, 1, 0, 0);
    gSetup33TexScrolls[0] = flamethrower_addTexScroll(gLevelDisplayLists[1], D_80090310, 1, -1, 0, 1, 0, 0);
    gSetup33TexScrolls[1] = flamethrower_addTexScroll(gLevelDisplayLists[0], D_80090314, 0, -1, 0, 0, 0, 0);
    gSetup33TexScrolls[2] = flamethrower_addObjectTexScroll(gfx_findObjectById(250), D_80090310, 1, -1, 0, 0, 0);
    gSetup33TexScrolls[3] = flamethrower_addObjectTexScroll(gfx_findObjectById(249), D_80090310, 1, -1, 0, 0, 0);
    collision_selectMap(0);
    D_800D9A40 = collision_findZoneLeaf(0x1500, 0x700);
}
