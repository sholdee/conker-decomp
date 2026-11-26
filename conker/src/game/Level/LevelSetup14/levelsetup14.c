#include <ultra64.h>

#include "functions.h"
#include "variables.h"

u8 D_800BE500[5]; /* level flags (game/Effects/FlagLights/flaglights toggles them) */


void func_15011C40(void) {
    bzero(&D_800BE500, 5); // bzero
}
