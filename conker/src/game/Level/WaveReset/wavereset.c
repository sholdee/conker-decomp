#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void level_resetWaveDeformers(void) {
    s32 i;

    gNumWaveDeformers = 0;
    for (i = 0; i < 2; i++)
    {
        gWaveDeformers[i] = NULL;
    }
}
