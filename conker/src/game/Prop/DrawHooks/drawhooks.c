#include <ultra64.h>

#include "conker/prop.h"


void prop_emptyFrameHook(s32 bufferIndex) {
}

/* Draw hooks left empty in this build: they return the display list unchanged. */
Gfx *prop_emptyDrawEffectAngled(Gfx *gfx, f32 x, f32 y, f32 z, f32 rot, s32 view) {
    return gfx;
}

Gfx *prop_emptyDrawEffect(Gfx *gfx, f32 x, f32 y, f32 z, s32 view) {
    return gfx;
}
