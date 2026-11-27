#include <ultra64.h>
#include "effects.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"

/* This file's .rodata: the one debris variant func_15108120 throws (model id and scale). */
static const s32 D_800A2430[] = { 0xE2 };
static const f32 D_800A2434[] = { 1.0f };

/* Plays an effect at obj lasting 500-700 frames, throws debris from 30 units above it, then calls
 * blood_spawnHitEffects on obj. */
void func_15108120(Actor *obj, u8 owner, s32 list) {
    Vec3f pos;
    Vec3f debrisOrigin;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    debrisOrigin.x = obj->xpos;
    debrisOrigin.y = obj->ypos + 30.0f;
    debrisOrigin.z = obj->zpos;
    particle_startRandomEmitter(&pos, (math_random() % 201) + 500, owner, list);
    {
        DebrisBurst debris;

        debris.countBase = 15;
        debris.countRange = 8;
        debris.pos = debrisOrigin;
        debris.angles.yawBase = 0;
        debris.angles.yawRange = 0xFF;
        debris.angles.pitchBase = -64;
        debris.angles.pitchRange = 41;
        debris.lifeBase = 39;
        debris.lifeRange = 20;
        debris.scaleBase = 0.3050000072f;
        debris.scaleRange = 0.3050000072f;
        debris.speedBase = 8.0f;
        debris.speedRange = 10.0f;
        debris.gravityBase = -0.8000000119f;
        debris.gravityRange = 0.3440000117f;
        debris.spin = 26.41100121f;
        particle_spawnDebrisBurst(&debris, D_800A2430, D_800A2434, 1, 40.0f, 0, owner, list);
    }
    blood_spawnHitEffects(obj, 1, 1, 1, 0, 0, owner, list);
}
