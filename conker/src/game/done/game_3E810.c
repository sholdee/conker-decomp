#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011360(void) {
    prop_clearWaveChains();
    prop_startWaveChain(2);
    prop_startWaveChain(3);
    if ((gTaskDoneBits[0x14] & 1) != 0) {
        D_800B0DF0->unk5 = 0x30;
        D_800B0DF0->unk6 = 0x38;
        D_800B0DF0->unk7 = 0x5F;
    }
}
