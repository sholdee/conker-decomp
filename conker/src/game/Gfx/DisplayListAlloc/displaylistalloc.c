#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void gfx_allocDisplayLists(void) {
    if (D_800B0DF0->gfxCount == 0) {
        D_800B0DF0->gfxCount = 8000;
    }
    D_800B0DF0->gfxCount += 400;
    gDisplayListBuffers[0] = allocate_memory(D_800B0DF0->gfxCount * sizeof(Gfx), 255, 2, 0);
    gDisplayListBuffers[1] = allocate_memory(D_800B0DF0->gfxCount * sizeof(Gfx), 255, 2, 0);
    D_800BEBA4 = (gDisplayListBuffers[1] - gDisplayListBuffers[0]) - 400;
}
