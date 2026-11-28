#include <ultra64.h>
#include <math.h>
#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/rope.h"
#include "conker/texture.h"

/* The six ropes of the level (higher end first). */
static const RopeEnds gPooCabinRopeEnds[6] = {
    { { 17.0f, 96.0f, -1183.0f }, { 17.0f, -983.0f, -1183.0f } },
    { { 321.0f, 530.0f, -1506.0f }, { 321.0f, -8.0f, -1506.0f } },
    { { 585.0f, 642.0f, -1579.0f }, { 585.0f, -369.0f, -1579.0f } },
    { { 832.0f, 149.0f, -1461.0f }, { 832.0f, -1077.0f, -1461.0f } },
    { { 432.0f, 630.0f, -777.0f }, { 432.0f, -589.0f, -777.0f } },
    { { 440.0f, -260.0f, -601.0f }, { 440.0f, -1246.0f, -601.0f } },
};

void level_setupPooCabin(void) {
    RopeSpawn tmp;
    s32 i;

    flamethrower_addTexScroll(gLevelDisplayLists[0], D_80090298[0x14], 0, -1, 0, 0, 0, -8);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_80090298[0x14], 0, -1, 0, 1, 0, -8);

    tmp.unk20 = 0;
    for (i = 0; i < 6; i++) {
        tmp.ends = gPooCabinRopeEnds[i];
        tmp.id = i + 1;
        tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
        rope_create(&tmp, 0);
    }
}
