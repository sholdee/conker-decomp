#include <ultra64.h>

#include "conker/actor.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/model.h"


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
    gBonePoolSize = gLevelHeader->bonePoolSize;
    gBonePools[0] = allocate_memory(gBonePoolSize << 6, 1, 3, 0);
    gBonePools[1] = allocate_memory(gBonePoolSize << 6, 1, 3, 0);
}
