#include <ultra64.h>

#include "conker/collision.h"
#include "conker/flamethrower.h"
#include "conker/gfx.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/particle.h"
#include "conker/texture.h"

struct TexScroll **gW3HauntedHouseTexScrolls; /* four texture scrolls */


void level_setupW3HauntedHouse(void) {
    gW3HauntedHouseTexScrolls = allocate_memory(16, 1, 0, 0);
    gW3HauntedHouseTexScrolls[0] = flamethrower_addTexScroll(gLevelDisplayLists[1], D_80090310, 1, -1, 0, 1, 0, 0);
    gW3HauntedHouseTexScrolls[1] = flamethrower_addTexScroll(gLevelDisplayLists[0], D_80090314, 0, -1, 0, 0, 0, 0);
    gW3HauntedHouseTexScrolls[2] = flamethrower_addObjectTexScroll(gfx_findObjectById(250), D_80090310, 1, -1, 0, 0, 0);
    gW3HauntedHouseTexScrolls[3] = flamethrower_addObjectTexScroll(gfx_findObjectById(249), D_80090310, 1, -1, 0, 0, 0);
    collision_selectMap(0);
    D_800D9A40 = collision_findZoneLeaf(0x1500, 0x700);
}
