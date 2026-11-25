#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_15010110(void) {
    RopeSpawn tmp;
    s32 i;

    func_15195AA8((Gfx *)D_800B0E00[0], D_80090298[0x14], 0, -1, 0, 0, 0, -8);
    func_15195AA8((Gfx *)D_800B0E00[1], D_80090298[0x14], 0, -1, 0, 1, 0, -8);

    tmp.unk20 = 0;
    for (i = 0; i < 6; i++) {
        tmp.ends = D_800963A0[i];
        tmp.id = i + 1;
        tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y);
        func_151ACBD4(&tmp, 0);
    }
}
