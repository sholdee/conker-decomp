#include <ultra64.h>

#include "functions.h"
#include "variables.h"

struct TexScroll **D_800BE4F0; /* four texture scrolls */


void func_15010FB0(void) {
    D_800BE4F0 = allocate_memory(16, 1, 0, 0);
    D_800BE4F0[0] = flamethrower_addTexScroll(D_800B0E00[1], D_80090310, 1, -1, 0, 1, 0, 0);
    D_800BE4F0[1] = flamethrower_addTexScroll(D_800B0E00[0], D_80090314, 0, -1, 0, 0, 0, 0);
    D_800BE4F0[2] = flamethrower_addObjectTexScroll(gfx_findObjectById(250), D_80090310, 1, -1, 0, 0, 0);
    D_800BE4F0[3] = flamethrower_addObjectTexScroll(gfx_findObjectById(249), D_80090310, 1, -1, 0, 0, 0);
    collision_selectMap(0);
    D_800D9A40 = collision_findZoneLeaf(0x1500, 0x700);
}
