#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/script.h"

/* Mirrors game flag 0x4054 into camera->AvailableModes bit 0x10000; when 0x405B is set but 0x405C is not (and
 * camera->Mode != 0x40), sets the camera target's unk17C to its unk180 + 10. */
void particle_levelHookWindmillExploded(Camera *camera) {
    Actor *target;

    if (script_query(1, 0x4054, 6, 0x9000)) {
        camera->AvailableModes |= 0x10000;
    } else {
        camera->AvailableModes &= ~0x10000;
    }
    if (!script_query(1, 0x405C, 6, 0x9000)) {
        if (script_query(1, 0x405B, 6, 0x2000) && camera->Mode != 0x40) {
            target = camera->CharPtr;
            target->ceilingY = target->floorY + 10.0f;
        }
    }
}

/* Spawns an emitter effect (particle_updatePointEmitter) at `pos` with a random duration of 69-201. */
void particle_spawnPointEmitter(EffectHeader *spawner, Vec3f *pos, f32 top, u8 noGround) {
    PointEmitterData data;
    TimedEffect *effect;

    data.pos = *pos;
    data.accum = 0.0f;

    effect = effect_newTimedEffect(math_random() % 133U + 69, -1, 0x49, -1, 1, 0, sizeof(data),
                           spawner->owner, spawner->kind);
    if (effect != NULL) {
        memcpy(&((PointEmitter *)effect)->data, &data, sizeof(data));
    }
}

/* Emitter effect update: accumulates a random rate per frame and spawns one particle at the
 * emitter's position per whole unit owed. */
void particle_updatePointEmitter(PointEmitter *effect) {
    PointEmitterData *data;

    data = &effect->data;
    data->accum += (0.07859999686f + math_randomFloat() * 0.03460000083f) * gFrameDelta;
    if (data->accum > 1.0f) {
        ParticleDesc spawn;
        WobbleData extra;
        void *particle;

        extra.drag = 0.9794009924f;
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.setGeometryMode = 0x200005;
        spawn.fadeStart = 80;
        spawn.fadeRate = 3;
        spawn.flags = 0x90DE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        spawn.updateFunc = 0x16;
        extra.phase[0] = 0;
        extra.phase[1] = 0;
        spawn.clearGeometryMode = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 80;
        spawn.r = 0x67;
        spawn.g = 0x6C;
        spawn.b = 0x6C;
        spawn.a = 0xFF;
        spawn.r2 = 0x16;
        spawn.g2 = 0xB;
        spawn.b2 = 0;
        spawn.brightness = 0xFF;
        spawn.scaleRate = 1.016479015f;
        spawn.pos = data->pos;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            extra.phaseStep[0] = math_random() % 5U + 4;
            extra.phaseStep[1] = math_random() % 5U + 4;
            extra.amplitude[0] = math_randomFloat() * 27.0f;
            extra.amplitude[1] = math_randomFloat() * 27.0f;
            spawn.gravity = math_randomFloat() * 0.2090000063f + 0.1350000054f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0);
            spawn.alpha = math_random() % 101U + 100;
            spawn.timer = math_random() % 78U + 126;
            spawn.width = spawn.height = math_randomFloat() * 503.0f + 405.0f;
            particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0x10, effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            data->accum -= 1.0f;
        } while (data->accum > 1.0f);
    }
}

/* Sets camera distances (YOffsetTget, height/idealHeight/IdealTripodMagnitude) and flag bits in AvailableModes/SpecialFlags from game flags. */
void particle_levelHookBarnArena(Camera *camera) {
    s32 flagBC;
    s32 flagBB;

    flagBC = script_query(0, script_query(0, 0x2007, 0xB7) | 0x2000, 0xBC);
    flagBB = script_query(0, 0x2000, 0xBB);

    if (script_query(0, 0x5043, 0x1A)) {
        if (script_query(0, 0x5045, 0x1A)) {
            camera->AvailableModes |= 0x4000;
        }
        camera->AvailableModes |= 0x400000;
        camera->YOffsetTget = 0.0f;
    } else if (script_query(1, 0x4030, 6, 0x2000)) {
        camera->YOffsetTget = 235.0f;
    } else {
        camera->height = 114.0f;
        camera->idealHeight = 114.0f;
        camera->IdealTripodMagnitude = 384.0f;
        camera->YOffsetTget = 60.0f;
    }

    if (flagBC != 0 && flagBB != -1) {
        camera->SpecialFlags |= 0x100;
    } else {
        camera->SpecialFlags &= ~0x100;
    }
}
