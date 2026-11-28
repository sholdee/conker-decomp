#include <ultra64.h>
#include "conker/level.h"


/* Clears the flag that script command 2 sets and the two 14-byte tables after it (as level_resetSetupState does). */
void level_setup02(void) {
    s32 i;

    D_800D9921 = 0;
    D_800D9920 = 0;
    for (i = 0; i < 14; i++) {
        D_800D9928[i] = 0;
        D_800D9938[i] = 0;
    }
}
