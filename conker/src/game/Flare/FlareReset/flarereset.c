#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Clears the lens flares and, when the level has a sun position, adds the sun flare. */
void flare_resetFlares(s32 arg0) {
    LensFlare *flare;

    gLensFlares = NULL;
    D_800DDD61 = 0;
    gScreenFlash = 0;
    if ((gDaylight != 0) && (gSunPositions[D_800B0DF0->unk10] != NULL)) {
        flare = flare_add(0, 0xFF);
        flare->flags = 3; // FLARE_SUN | FLARE_FLASH (game/Flare/LensFlare/lensflare.c)
    }
}
