#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_1500ED80(void) {
    prop_createWaveDeformer(1, 0xD8, -0xDAC, 0, 0x4BB, 0.0f, 15000, 30.0f, 0x4D, 0xF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F);
    func_150124A0();
}

void func_1500EE18(Actor *arg0, s32 arg1, s32 arg2) {
    ActorRef tmp;
    TimedEffect *effect;

    tmp.actor = arg0;
    tmp.uniqueId = arg0->unique_id;

    effect = effect_newTimedEffect(0x12C, -1, 0x45, -1, 0, 0, 8, 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}

/* Zone handler D_80082F70[1]: spawns timed effect 0x46 for the zone. */
void func_1500EE94(TriggerZone *zone) {
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
