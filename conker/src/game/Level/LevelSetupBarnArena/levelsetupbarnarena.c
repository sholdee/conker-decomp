#include <ultra64.h>

#include "effects.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/prop.h"
#include "conker/view.h"


void level_setupBarnArena(void) {
    prop_createWaveDeformer(1, 0xD8, -0xDAC, 0, 0x4BB, 0.0f, 15000, 30.0f, 0x4D, 0xF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F);
    level_initPipeBurstPoints();
}

void level_spawnObjBurst(Actor *obj, s32 owner, s32 list) {
    ActorRef tmp;
    TimedEffect *effect;

    tmp.actor = obj;
    tmp.uniqueId = obj->unique_id;

    effect = effect_newTimedEffect(0x12C, -1, 0x45, -1, 0, 0, 8, 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}

/* Zone handler gBoltZoneSpawns[1]: spawns timed effect 0x46 for the zone. */
void level_spawnBoltRingZone(TriggerZone *zone) {
    BoltRingData tmp;
    TimedEffect *effect;

    tmp.emitter = zone;
    tmp.spawnAccum = 0.0f;
    tmp.emitterState = zone->inactive;
    effect = effect_newTimedEffect(0x12C, -1, 0x46, -1, 0, 0x39, 0xC, 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}
