#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Zone handler D_80082ECC[...]: spawns timed effect 89 for the zone at its x/z. */
s32 func_150106D0(TriggerZone *zone) {
    ShapeEmitter tmp;
    TimedEffect *effect;

    tmp.shape = zone;
    tmp.rate = func_15144598(zone);
    tmp.accumulator = 0.0f;

    collision_selectMap(0);

    tmp.zone = collision_findZoneLeaf(zone->x, zone->z);
    if (tmp.zone == NULL) {
        return 1;
    } else {
        effect = effect_newTimedEffect(300, -1, 89, -1, 0, 0, 16, 255, 0);
        if (effect != NULL) {
            memcpy(effect + 1, &tmp, sizeof(tmp));
        }
    }
    return 1;
}
