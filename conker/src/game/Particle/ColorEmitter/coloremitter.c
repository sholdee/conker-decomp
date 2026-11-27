#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/screen.h"

/* Spawns a flash particle, a burst of 12-18 drops, the particle_spawnShortLightFlash effect and a white screen
 * flash at `pos`. */
void particle_spawnFlashBurst(Vec3f *pos, u8 owner, s32 kind) {
    effect_spawnFlashParticle(math_random() % 5U + 12, math_random() % 56U + 200, math_randomFloat() * 600.0f + 1700.0f,
                  pos, 0xFF, owner, kind);
    {
        DropSpraySpec drops;

        drops.spray.pos = *pos;
        drops.spray.countBase = 12;
        drops.spray.countRange = 7;
        drops.angles.yawBase = 0;
        drops.angles.yawRange = 0xFF;
        drops.angles.pitchBase = -64;
        drops.angles.pitchRange = 60;
        drops.spray.lifeBase = 36;
        drops.spray.lifeRange = 60;
        drops.spray.alphaBase = 200;
        drops.spray.alphaRange = 55;
        drops.spray.unk30 = 0;
        drops.spray.kind = 12;
        drops.spray.wobbles = 1;
        drops.spray.speedBase = 8.0f;
        drops.spray.speedRange = 10.0f;
        drops.spray.gravityBase = -1.097f;
        drops.spray.gravityRange = 0.37f;
        drops.spray.sizeBase = 280.0f;
        drops.spray.sizeRange = 390.0f;
        drops.spray.unk34 = 0.6f;
        drops.spray.bounceChance = 1.0f;
        particle_spawnDropSprayAtAngles(&drops.angles, &drops.spray, 0, owner, kind);
    }
    particle_spawnShortLightFlash(pos, owner, kind);
    screen_spawnFlash(0xFF, 0xFF, 0xFF, math_random() % 51U + 150, math_random() % 5U + 8, 0, owner, kind);
}

/* Spawns an emitter effect at `pos` that lasts `duration` frames and is updated by particle_updateColorEmitter
 * (gTimedEffectTickFuncs[0x5B]), preceded by a particle_spawnFlashBurst burst when `burst` is set. */
void particle_startColorEmitter(pos, duration, burst, randomColors, owner, kind)
    Vec3f *pos;
    s32 duration;
    u8 burst;
    u8 randomColors;
    u8 owner;
    s32 kind;
{
    ColorEmitterData data;
    ColorEmitter *emitter;

    if (burst) {
        particle_spawnFlashBurst(pos, owner, kind);
    }

    data.pos = *pos;
    data.accum = 0.0f;
    data.randomColors = randomColors;

    emitter = effect_newTimedEffect(duration, -1, 0x5B, -1, 1, 0, sizeof(data), owner, kind);
    if (emitter != NULL) {
        memcpy(&emitter->data, &data, sizeof(data));
    }
}

/* Update function of the particle_startColorEmitter emitter: emits particles at its position at a random rate
 * of (0.136 to 0.257) * gFrameDelta per frame, in a fixed colour or one of three random colour pairs. */
void particle_updateColorEmitter(ColorEmitter *emitter) {
    ColorEmitterData *data;

    data = &emitter->data;
    data->accum += (0.136f + math_randomFloat() * 0.12100001f) * gFrameDelta;
    if (data->accum > 1.0f) {
        ParticleDesc desc;
        WobbleData extra;
        SpriteParticle *particle;
        u8 colorIndex;

        extra.drag = 0.97047f;
        desc.texture = 0x6C;
        desc.renderMode = 0x5103;
        desc.setGeometryMode = 0x200005;
        extra.phase[0] = 0;
        extra.phase[1] = 0;
        desc.clearGeometryMode = 0;
        desc.frame = 0;
        desc.frameRate = 0;
        desc.fadeStart = 40;
        desc.fadeRate = 6;
        desc.flags = 0x80DE07;
        desc.primColorMode = 8;
        desc.envColorMode = 6;
        desc.updateFunc = 0x16;
        desc.deathFunc = -1;
        desc.drawFunc = -1;
        desc.handlerSet = 0;
        desc.scaleStart = 120;
        desc.brightness = 0xFF;
        desc.scaleRate = 1.006579f;
        desc.pos = data->pos;
        desc.offset.x = 0.0f;
        desc.offset.y = 0.0f;
        desc.offset.z = 0.0f;
        desc.vel.x = 0.0f;
        desc.vel.y = 0.0f;
        desc.vel.z = 0.0f;
        if (data->randomColors == 0) {
            desc.r = 0xEF;
            desc.g = 0xE0;
            desc.b = 0xCD;
            desc.a = 0xFF;
            desc.r2 = 0x30;
            desc.g2 = 0x30;
            desc.b2 = 0x30;
        }
        do {
            extra.phaseStep[0] = math_random() % 5U + 4;
            extra.phaseStep[1] = math_random() % 5U + 4;
            extra.amplitude[0] = math_randomFloat() * 50.0f;
            extra.amplitude[1] = math_randomFloat() * 50.0f;
            desc.gravity = math_randomFloat() * 0.15100001f + 0.19100001f;
            desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
            desc.flags |= ((math_random() & 1) ? PARTICLE_FLIP_S : 0) | ((math_random() & 1) ? PARTICLE_FLIP_T : 0);
            desc.alpha = math_random() % 56U + 200;
            desc.timer = math_random() % 36U + 110;
            desc.width = desc.height = math_randomFloat() * 370.0f + 403.0f;
            if (data->randomColors != 0) {
                const u8 colors[3][3] = { { 0xEF, 0xE0, 0xCD }, { 0x9E, 0x9C, 0xA0 }, { 0x9E, 0x9C, 0xA0 } };
                const u8 colors2[3][3] = { { 0x30, 0x30, 0x30 }, { 0x9A, 0x69, 0xD2 }, { 0xD5, 0x83, 0x91 } };
                colorIndex = math_random() % 3U;
                desc.r = colors[colorIndex][0];
                desc.g = colors[colorIndex][1];
                desc.b = colors[colorIndex][2];
                desc.a = 0xFF;
                desc.r2 = colors2[colorIndex][0];
                desc.g2 = colors2[colorIndex][1];
                desc.b2 = colors2[colorIndex][2];
            }
            particle = particle_newSpriteParticleWithFloor(&desc, 0, 0, sizeof(extra), emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->data.wobble, &extra, sizeof(extra));
            }
            data->accum -= 1.0f;
        } while (data->accum > 1.0f);
    }
}
