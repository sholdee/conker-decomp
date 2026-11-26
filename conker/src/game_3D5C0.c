#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The six ropes of the level (higher end first). */
static const RopeEnds D_800963A0[6] = {
    { { 17.0f, 96.0f, -1183.0f }, { 17.0f, -983.0f, -1183.0f } },
    { { 321.0f, 530.0f, -1506.0f }, { 321.0f, -8.0f, -1506.0f } },
    { { 585.0f, 642.0f, -1579.0f }, { 585.0f, -369.0f, -1579.0f } },
    { { 832.0f, 149.0f, -1461.0f }, { 832.0f, -1077.0f, -1461.0f } },
    { { 432.0f, 630.0f, -777.0f }, { 432.0f, -589.0f, -777.0f } },
    { { 440.0f, -260.0f, -601.0f }, { 440.0f, -1246.0f, -601.0f } },
};

void func_15010110(void) {
    RopeSpawn tmp;
    s32 i;

    flamethrower_addTexScroll(D_800B0E00[0], D_80090298[0x14], 0, -1, 0, 0, 0, -8);
    flamethrower_addTexScroll(D_800B0E00[1], D_80090298[0x14], 0, -1, 0, 1, 0, -8);

    tmp.unk20 = 0;
    for (i = 0; i < 6; i++) {
        tmp.ends = D_800963A0[i];
        tmp.id = i + 1;
        tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
        func_151ACBD4(&tmp, 0);
    }
}
