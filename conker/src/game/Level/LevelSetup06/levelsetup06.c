#include <ultra64.h>

#include "conker/effect.h"
#include "conker/level.h"
#include "conker/weapon.h"


void level_setup06(void) {
    gProjectileHitCallback = func_150EA904;
}
