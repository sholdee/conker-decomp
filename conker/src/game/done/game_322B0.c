#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15004E00(void) {
    u32 i;

    for (i = 0; i < 25; i++)
    {
        D_800C6660[i].parts = NULL;
        D_800C6660[i].partMaskLo = 0;
        D_800C6660[i].partMaskHi = 0;
        D_800C6660[i].state = 0;
    }
}

void func_15004E80(void) {
    D_800C3E7C = D_800B0DF0->unk1A;
    D_800C3E80[0] = allocate_memory(D_800C3E7C << 6, 1, 3, 0);
    D_800C3E80[1] = allocate_memory(D_800C3E7C << 6, 1, 3, 0);
}
