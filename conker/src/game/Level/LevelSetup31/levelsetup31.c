#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void level_setup31(void) {
    prop_clearWaveChains();
    prop_startWaveChain(2);
    prop_startWaveChain(3);
    if ((gTaskDoneBits[0x14] & 1) != 0) {
        gLevelHeader->backgroundRed = 0x30;
        gLevelHeader->backgroundGreen = 0x38;
        gLevelHeader->backgroundBlue = 0x5F;
    }
}
