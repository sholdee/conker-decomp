#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Clears the lens flares and, when the level has a sun position, adds the sun flare. */
void func_15009150(s32 arg0) {
    LensFlare *flare;

    D_800DDD64 = NULL;
    D_800DDD61 = 0;
    D_800DD2D0 = 0;
    if ((D_800DCDD0 != 0) && (D_8008CFFC[D_800B0DF0->unk10] != NULL)) {
        flare = func_1517E080(0, 0xFF);
        flare->flags = 3; // FLARE_SUN | FLARE_FLASH (game_1AB530.c)
    }
}
