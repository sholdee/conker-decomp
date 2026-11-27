#include <ultra64.h>
#include "conker/effect.h"
#include "conker/light.h"
#include "conker/sight.h"
#include "conker/zone.h"

void effect_turnBeamsOff(void) {
    u8 i;
    const s32 ids[17] = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };

    for (i = 0; i < 17; i++) {
        sight_sendBeamCommand(1, &zones[ids[i]]);
    }
    light_setGroupDisabled(9, 1);
}
