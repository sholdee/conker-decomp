#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The five ropes of the level (higher end first). */
static const RopeEnds gSetup11Rope1Ends = { { -2544.0f, 2426.0f, 2213.0f }, { -2544.0f, 1144.0f, 2213.0f } };
static const RopeEnds gSetup11Rope2Ends = { { -1987.0f, 1380.0f, 1904.0f }, { -1987.0f, 249.0f, 1904.0f } };
static const RopeEnds gSetup11Rope3Ends = { { -2162.0f, 534.0f, 1344.0f }, { -2162.0f, -645.0f, 1344.0f } };
static const RopeEnds gSetup11Rope4Ends = { { -2128.0f, -701.0f, 2129.0f }, { -2128.0f, -1545.0f, 2129.0f } };
static const RopeEnds gSetup11Rope5Ends = { { -2893.0f, 2648.0f, 2174.0f }, { -2893.0f, 2111.0f, 2174.0f } };

/* Starts an object emitter at (x, y, z) over the floor at floorY. */
void level_queueObjectDrop11(s32 x, s32 y, s32 z, s32 floorY, s32 arg4) {
    ObjectEmitter *emitter;

    emitter = effect_newTimedEffectNoDraw((math_random() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.unk8 = arg4;
    }
}

void level_setup11(void) {
    RopeSpawn tmp;

    tmp.unk20 = 0;

    tmp.ends = gSetup11Rope1Ends;
    tmp.id = 1;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gSetup11Rope2Ends;
    tmp.id = 2;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gSetup11Rope3Ends;
    tmp.id = 3;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gSetup11Rope4Ends;
    tmp.id = 4;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    tmp.ends = gSetup11Rope5Ends;
    tmp.id = 5;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    level_queueObjectDrop11(-0x6E7, -0x46A, 0x421, -0x648, 1);
    level_queueObjectDrop11(-0xA71, 0x71B, 0x82B, -0x602, 2);
    level_queueObjectDrop11(-0x870, 0x36E, 0x4E1, -0x602, 2);
    level_queueObjectDrop11(-0xBD5, 0x3F1, 0x659, -0x602, 2);
    level_queueObjectDrop11(-0xE6B, -0x263, -0x427, -0x647, 1);

    D_800DCDA4 = gAmbientColour[0];
    D_800DCDA8 = gAmbientColour[1];
    D_800DCDAC = gAmbientColour[2];
    D_800DCDB0 = gDirLightAmbientTint[0];
    D_800DCDB4 = gDirLightAmbientTint[1];
    D_800DCDB8 = gDirLightAmbientTint[2];
    D_800DCDBC = gDirLightAmount;
    D_800DCDC0 = gDirLightAmbientAmount;
}
