#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spins the emitter and, while math_isZoneHiddenFromViews allows it, throws particles from random points of its region. */
s32 particle_updateZoneEmitter(ZoneEmitter *obj) {
    ZoneEmitterState *state;

    state = &obj->state;
    if (state->hasZone) {
        mp_addZone(obj->head.def.pos.x, obj->head.def.pos.y, obj->head.def.pos.z, 500.0f);
    }
    state->angle += state->angleSpeed * gFrameDelta;
    state->angle = math_wrapFloat(state->angle, 2048.0f, 0.0f);
    state->phase += state->phaseSpeed * D_800BE9E4;
    state->alpha = math_cosBinAngleMasked(state->phase - 0x40) * 63.0f + 192.0f;
    if (math_isZoneHiddenFromViews(obj->head.def.unk48) == 0) {
        state->pending += (0.3930000067f + math_randomFloat() * 0.4370000064f) * gFrameDelta;
        if (state->pending > 1.0f) {
            ParticleDesc spawn;

            spawn.texture = 40;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x1F0600;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.r2 = 0xFF;
            spawn.g2 = 0xFF;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.b2 = 0xFF;
            spawn.fadeStart = 12;
            spawn.fadeRate = 21;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0xC207;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            do {
                math_dirFromBinAngleXZ((u8)math_random(), state->region->xSize * math_randomFloat(), &spawn.pos.x,
                              &spawn.pos.z);
                spawn.pos.x += state->region->x;
                spawn.pos.y = state->region->y;
                spawn.pos.z += state->region->z;
                spawn.timer = (math_random() & 0xF) + 25;
                spawn.alpha = math_random() % 101U + 100;
                spawn.width = spawn.height = math_randomFloat() * 205.0f + 98.0f;
                math_dirFromBinAngles((u8)math_random(), math_random() % 35U - 64, math_randomFloat() * 10.0f + 10.0f,
                              &spawn.vel);
                // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
                spawn.gravity = math_randomFloat() * 0.2840000093f + -0.9100000262f;
                particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0, obj->head.header.owner, obj->head.header.kind);
                state->pending -= 1.0f;
            } while (state->pending > 1.0f);
        }
    }
    return 1;
}

/* Shuts the emitter down: marks its region off, calls sound_stopByCallback with sound_stopWhenSilentCallback on it, and frees unk10. */
void particle_shutZoneEmitter(ZoneEmitter *obj) {
    obj->state.region->inactive = 1;
    sound_stopByCallback(sound_stopWhenSilentCallback, obj->state.region, 0);
    if (obj->state.unk10 != NULL) {
        effect_destroyObject(obj->state.unk10);
    }
}
