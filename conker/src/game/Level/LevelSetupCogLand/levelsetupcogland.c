#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/rope.h"

/* The five ropes of the level (higher end first). */
static const RopeEnds gCogLandRope1Ends = { { -2544.0f, 2426.0f, 2213.0f }, { -2544.0f, 1144.0f, 2213.0f } };
static const RopeEnds gCogLandRope2Ends = { { -1987.0f, 1380.0f, 1904.0f }, { -1987.0f, 249.0f, 1904.0f } };
static const RopeEnds gCogLandRope3Ends = { { -2162.0f, 534.0f, 1344.0f }, { -2162.0f, -645.0f, 1344.0f } };
static const RopeEnds gCogLandRope4Ends = { { -2128.0f, -701.0f, 2129.0f }, { -2128.0f, -1545.0f, 2129.0f } };
static const RopeEnds gCogLandRope5Ends = { { -2893.0f, 2648.0f, 2174.0f }, { -2893.0f, 2111.0f, 2174.0f } };

/* Starts an object emitter at (x, y, z) over the floor at floorY. */
void level_queueObjectDropCogLand(s32 x, s32 y, s32 z, s32 floorY, s32 markOnLanding) {
    ObjectEmitter *emitter;

    emitter = effect_newTimedEffectNoDraw((math_random() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.markOnLanding = markOnLanding;
    }
}

void level_setupCogLand(void) {
    RopeSpawn tmp;

    tmp.unk20 = 0;

    tmp.ends = gCogLandRope1Ends;
    tmp.id = 1;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gCogLandRope2Ends;
    tmp.id = 2;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gCogLandRope3Ends;
    tmp.id = 3;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    tmp.ends = gCogLandRope4Ends;
    tmp.id = 4;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    tmp.ends = gCogLandRope5Ends;
    tmp.id = 5;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
    rope_create(&tmp, 0);

    level_queueObjectDropCogLand(-0x6E7, -0x46A, 0x421, -0x648, 1);
    level_queueObjectDropCogLand(-0xA71, 0x71B, 0x82B, -0x602, 2);
    level_queueObjectDropCogLand(-0x870, 0x36E, 0x4E1, -0x602, 2);
    level_queueObjectDropCogLand(-0xBD5, 0x3F1, 0x659, -0x602, 2);
    level_queueObjectDropCogLand(-0xE6B, -0x263, -0x427, -0x647, 1);

    gSavedAmbientRed = gAmbientColour[0];
    gSavedAmbientGreen = gAmbientColour[1];
    gSavedAmbientBlue = gAmbientColour[2];
    gSavedDirLightAmbientTintRed = gDirLightAmbientTint[0];
    gSavedDirLightAmbientTintGreen = gDirLightAmbientTint[1];
    gSavedDirLightAmbientTintBlue = gDirLightAmbientTint[2];
    gSavedDirLightAmount = gDirLightAmount;
    gSavedDirLightAmbientAmount = gDirLightAmbientAmount;
}
