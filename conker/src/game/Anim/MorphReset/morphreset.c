#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void anim_clearMorphCache(void) {
    s32 i;

    for (i = 0; i < 2; i++)
    {
        gVtxMorphCache[i] = 0;
    }
}
