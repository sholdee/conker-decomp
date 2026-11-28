#include <ultra64.h>
#include "effects.h"
#include "conker/effect.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/light.h"


// ???
void level_setupHarbour(void) {
    WorldObject *prop;

    gMaterialGlowLevels[0] = (gMaterialGlowLevels[1] = (gMaterialGlowLevels[2] = 0));
    prop = gfx_findObjectById(0xF6);
    prop->touch.f.a = 2.0f;
    gWarpModelSetupDone = 0;
}

void level_spawnBoltEmitter(Actor *obj) {
    BoltSource tmp;
    TimedEffect *effect;

    light_createAtActor(obj, 2, 2, 300, 30, 100, 200, 255, 255, 1);
// FAKEMATCH but works...
dummy_label_927029:
    tmp.obj = obj;
    tmp.id = obj->unique_id;
    tmp.flashTimer = 0;
    tmp.boltTimer = 0;
    tmp.timer2 = 0;

    effect = effect_newTimedEffect(300, -1, 80, -1, 0, 61, 12, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}
