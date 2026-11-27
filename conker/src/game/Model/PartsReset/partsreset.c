#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void model_resetParts(void) {
    u32 i;

    for (i = 0; i < 25; i++)
    {
        gObjPartsStates[i].parts = NULL;
        gObjPartsStates[i].partMaskLo = 0;
        gObjPartsStates[i].partMaskHi = 0;
        gObjPartsStates[i].state = 0;
    }
}

void model_allocBonePools(void) {
    gBonePoolSize = D_800B0DF0->bonePoolSize;
    gBonePools[0] = allocate_memory(gBonePoolSize << 6, 1, 3, 0);
    gBonePools[1] = allocate_memory(gBonePoolSize << 6, 1, 3, 0);
}
