#include <ultra64.h>

#include "conker/level.h"
#include "conker/prop.h"
#include "conker/weapon.h"


void level_setup10(void) {
    gBulletHitTest = prop_shotHitsCylinders;
}
