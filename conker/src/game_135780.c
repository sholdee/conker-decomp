#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define EMITTER_ENABLED 1

/* Emitter update: accumulates 0.03..0.04 particles per unit of D_800BE9A4 and, while enabled,
 * spawns one at a random point of the emitter's area for each whole one. */
void func_151082D0(AreaEmitter *emitter) {
    AreaEmitterData *data;

    data = &emitter->data;
    data->spawnAccum += (0.030000001f + func_150ADA68() * 0.010000001f) * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        if (data->flags & EMITTER_ENABLED) {
            ParticleDesc desc;
            FlashParticleData extra;
            AreaFlashParticle *particle;

            desc.texture = 0x74;
            desc.renderMode = 0x5310;
            desc.unk0 = 0x200005;
            desc.unk4 = 0x9F0600;
            desc.r = 0xFF;
            desc.g = 0xFF;
            desc.b = 0xFF;
            desc.a = 0xFF;
            desc.frame = 0;
            desc.frameRate = 0;
            desc.brightness = 0xFF;
            desc.fadeStart = 1;
            desc.offset.x = 0.0f;
            desc.offset.y = 0.0f;
            desc.offset.z = 0.0f;
            desc.vel.x = 0.0f;
            desc.vel.y = 0.0f;
            desc.vel.z = 0.0f;
            desc.gravity = 0.0f;
            desc.fadeRate = 0xFF;
            desc.scaleStart = 1;
            desc.scaleRate = 1.0f;
            desc.primColorMode = 6;
            desc.envColorMode = 6;
            desc.updateFunc = 0x18;
            desc.deathFunc = -1;
            desc.drawFunc = -1;
            desc.handlerSet = 0;
            desc.viewMask = NULL;
            desc.timer = 300;
            desc.r2 = 0xFF;
            desc.g2 = 0xFF;
            desc.b2 = 0xFF;
            extra.flashTimer = 0.0f;
            do {
                extra.life = extra.lifetime = func_150ADA68() * 20.0f + 30.0f;
                extra.frame1Time = extra.life * 0.15f;
                extra.frame0Time = extra.frame1Time * 0.5f;
                desc.width = func_150ADA68() * 500.0f + 600.0f;
                desc.height = desc.width * 0.5f;
                func_1514373C(func_150ADA68() * 3.1415927f * 2.0f, data->area->radius, &desc.pos.x, &desc.pos.z);
                desc.pos.x += data->area->x;
                desc.pos.z += data->area->z;
                desc.pos.y = data->area->y + data->area->height * func_150ADA68();
                desc.flags = ((func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0) | PARTICLE_DRAW_FUNC | PARTICLE_HANDLERS;
                desc.alpha = (func_150ADA20() & 0x7F) + 0x80;
                particle = func_15130280(&desc, 2, 0, sizeof(extra), emitter->base.header.owner, emitter->base.header.kind);
                if (particle != NULL) {
                    memcpy(&particle->data, &extra, sizeof(extra));
                }
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        } else {
            do {
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

/* Update of func_151082D0's particles: picks the animation frame from the time since birth and
 * until death, randomly starts an 11-unit flash (drawn with draw function 3), and kills the
 * particle when its life runs out. Returns 0 to kill it. */
s32 func_15108658(AreaFlashParticle *particle, s32 arg1) {
    FlashParticleData *data = &particle->data;
    f32 elapsed = data->lifetime - data->life;

    if (data->life < data->frame0Time || elapsed < data->frame0Time) {
        particle->head.desc.frame = 0;
    } else if (data->life < data->frame1Time || elapsed < data->frame1Time) {
        particle->head.desc.frame = 0x10000;
    } else {
        particle->head.desc.frame = 0x20000;
    }
    if (data->flashTimer) {
        f32 flashElapsed = 11.0f - data->flashTimer;

        if (data->flashTimer < 1.5730001f || flashElapsed < 1.5730001f) {
            particle->head.desc.frame = 0;
            particle->head.desc.drawFunc = -1;
        } else {
            particle->head.desc.drawFunc = 3;
        }
        data->flashTimer -= D_800BE9A4;
        if (data->flashTimer < 0.0f) {
            data->flashTimer = 0.0f;
            particle->head.desc.drawFunc = -1;
        }
    } else {
        data->life -= D_800BE9A4;
        if (data->life < 0.0f) {
            return 0;
        }
        if (particle->head.desc.frame == 0x20000 && func_150ADA68() < 0.051000003f) {
            data->flashTimer = 11.0f;
        }
    }
    return 1;
}

/* Event handler of the func_151082D0 emitter: event 0x2B enables spawning, 0x2C disables it. */
void func_151087FC(AreaEmitter *emitter, s32 arg1, u8 event) {
    AreaEmitterData *data = &emitter->data;

    if (event == 0x2B) {
        data->flags |= EMITTER_ENABLED;
    } else if (event == 0x2C) {
        data->flags &= ~EMITTER_ENABLED;
    }
}
