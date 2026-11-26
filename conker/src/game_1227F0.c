#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150F5340(void) {
    u8 i;
    const s32 ids[17] = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };

    for (i = 0; i < 17; i++) {
        sight_sendBeamCommand(1, &zones[ids[i]]);
    }
    light_setGroupDisabled(9, 1);
}
