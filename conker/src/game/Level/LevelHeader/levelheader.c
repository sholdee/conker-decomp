#include <ultra64.h>
#include "conker/level.h"

/*
 * A unit that holds one variable, between game/Collision/QuadTree/quadtree and game/Level/LevelModels/levelmodels in
 * the link. quadtree's .bss ends at 0x800B0DE8 and is padded to 0x800B0DF0; this pointer has its own 16-byte
 * block, and levelmodels' .bss starts at 0x800B0E00. So neither neighbour defines it.
 */
LevelHeader *gLevelHeader; // the current level's settings resource (func_15000F04 loads it)
