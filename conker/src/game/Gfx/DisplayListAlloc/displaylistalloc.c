#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void gfx_allocDisplayLists(void) {
    if (gLevelHeader->gfxCount == 0) {
        gLevelHeader->gfxCount = 8000;
    }
    gLevelHeader->gfxCount += 400;
    gDisplayListBuffers[0] = allocate_memory(gLevelHeader->gfxCount * sizeof(Gfx), 255, 2, 0);
    gDisplayListBuffers[1] = allocate_memory(gLevelHeader->gfxCount * sizeof(Gfx), 255, 2, 0);
    gDisplayListLimit = (gDisplayListBuffers[1] - gDisplayListBuffers[0]) - 400;
}
