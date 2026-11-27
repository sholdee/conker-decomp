#include <ultra64.h>

#include "effects.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/level.h"
#include "conker/particle.h"


/* Starts a global colour cycle (timed effect 30, light_updateColourCycle) around grey 50. */
void level_startAmbientColourCycle(s32 arg0) {
    TimedEffect *effect;
    LightColourPulse tmp;

    tmp.base.x = 50.0f;
    tmp.base.y = 50.0f;
    tmp.base.z = 50.0f;
    tmp.amplitude.x = 50.0f;
    tmp.amplitude.y = 50.0f;
    tmp.amplitude.z = 50.0f;
    tmp.phase = 0.0f;
    tmp.speed = 0.07609999925f;

    effect = effect_newTimedEffect(300, -1, 30, -1, 0, 65, 32, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}

/* Attaches a drip emitter (particle_newDripEmitter) to the actor, except in level 24. */
void level_attachDripEmitter(Actor *arg0) {
    ActorLink tmp;

    if (level != LEVEL_W5_THRONE_ROOM) {
        tmp.actor = arg0;
        tmp.actorId = arg0->unique_id;
        tmp.timer = 300;
        tmp.flags = 0;
        tmp.unk9 = 0;
        tmp.kind = 2;
        particle_newDripEmitter(&tmp, 0, 255, 1);
    }
}
