#include <ultra64.h>

#include "conker/anim.h"


void anim_clearMorphCache(void) {
    s32 i;

    for (i = 0; i < 2; i++)
    {
        gVtxMorphCache[i] = 0;
    }
}
