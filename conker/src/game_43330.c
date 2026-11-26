#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_15015E80(void) {
    if (D_800B0DF0->gfxCount == 0) {
        D_800B0DF0->gfxCount = 8000;
    }
    D_800B0DF0->gfxCount += 400;
    D_800BE9C8[0] = allocate_memory(D_800B0DF0->gfxCount * sizeof(Gfx), 255, 2, 0);
    D_800BE9C8[1] = allocate_memory(D_800B0DF0->gfxCount * sizeof(Gfx), 255, 2, 0);
    D_800BEBA4 = (D_800BE9C8[1] - D_800BE9C8[0]) - 400;
}
