#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15004D50(void) { 
    s32 i; //s0
    Vtx **s1;

    D_800C3FC0[0] = D_80084300;
    for( i = 1;  i <= gScreenLayout; i++){
        D_800C3FC0[i] = allocate_memory(sizeof(D_80084300), 1, 2, 0);
        bcopy(D_800C3FC0[0], D_800C3FC0[i], sizeof(D_80084300));
    }
}
