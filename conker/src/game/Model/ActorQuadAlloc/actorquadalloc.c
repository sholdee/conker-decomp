#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void model_allocActorQuadVtx(void) { 
    s32 i; //s0
    Vtx **s1;

    gActorQuadVtx[0] = gActorQuadVtxTemplate;
    for( i = 1;  i <= gScreenLayout; i++){
        gActorQuadVtx[i] = allocate_memory(sizeof(gActorQuadVtxTemplate), 1, 2, 0);
        bcopy(gActorQuadVtx[0], gActorQuadVtx[i], sizeof(gActorQuadVtxTemplate));
    }
}
