#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Zone handler D_80082ECC[...]: spawns timed effect 89 for the zone at its x/z. */
s32 func_150106D0(TriggerZone *zone) {
    struct176 tmp;
    TimedEffect *effect;

    tmp.unk0 = zone;
    tmp.unk4 = func_15144598(zone);
    tmp.unkC = 0.0f;

    func_1510F800(0);

    tmp.unk8 = func_1510FD20(zone->x, zone->z);
    if (tmp.unk8 == 0) {
        return 1;
    } else {
        effect = func_15149130(300, -1, 89, -1, 0, 0, 16, 255, 0);
        if (effect != NULL) {
            memcpy(effect + 1, &tmp, 16);
        }
    }
    return 1;
}
