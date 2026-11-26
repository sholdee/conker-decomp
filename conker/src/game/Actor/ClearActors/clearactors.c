#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void actor_clearAll(void) {

    bzero(&movement_obj, 20300);
    D_80086000 = 0;

    if (gMatchState.mode == 2) {
        D_800CC26E = 2;
    } else if (gMatchState.mode == 3) {
        D_800CC26E = 3;
    } else if (gMatchState.mode == 4) {
        D_800CC26E = 4;
    } else {
        D_800CC26E = 0;
    }
    D_800CC2B0 = 0;
    bzero(&D_800CC298, 24);
    D_800CC298.unkA = 1;
    D_800D18D0 = 0;
}
