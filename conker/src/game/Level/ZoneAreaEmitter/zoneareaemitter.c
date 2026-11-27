#include <ultra64.h>

#include "effects.h"
#include "conker/beam.h"
#include "conker/level.h"


void level_spawnZoneAreaEmitter(s32 zone, s32 kind) {
    EmitterDef tmp;

    tmp.flags = 7;
    tmp.anchor = (TriggerZone *)zone;
    tmp.rate = 6.999999869e-08f;
    tmp.rateRange = 1.800000007e-07f;
    tmp.pos.x = 0.0f;
    tmp.pos.y = 0.0f;
    tmp.pos.z = 0.0f;
    tmp.bottom = 0.0f;
    tmp.radius = 0.0f;
    tmp.type = 0;
    tmp.update = -1;
    tmp.spawn = 3;

    beam_newAreaEmitter(&tmp, -1, 0, 0xFF, 0);
}
