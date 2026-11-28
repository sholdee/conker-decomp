#include <ultra64.h>

#include "conker/effect.h"
#include "conker/level.h"
#include "conker/weapon.h"


void level_setupOutsideMolehole(void) {
    gProjectileHitCallback = func_150EA904;
}
