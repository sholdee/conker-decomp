#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void level_setup31(void) {
    prop_clearWaveChains();
    prop_startWaveChain(2);
    prop_startWaveChain(3);
    if ((gTaskDoneBits[0x14] & 1) != 0) {
        D_800B0DF0->backgroundRed = 0x30;
        D_800B0DF0->backgroundGreen = 0x38;
        D_800B0DF0->backgroundBlue = 0x5F;
    }
}
