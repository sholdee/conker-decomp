#include <ultra64.h>

#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/view.h"
#include "libc/string.h"

void particle_emitDiscParticle(Struct150C29F0 *emitter) {
    DiscEmitter *pos;
    ParticleEmitterDesc particle;
    f32 scaleA;
    f32 scaleB;
    u8 angle;
    f32 cosine;
    f32 sine;
    f32 radius;

    angle = math_random() & 0xFF;
    cosine = math_cosBinAngleMasked(angle - 0x40);
    sine = math_cosBinAngleMasked(angle);
    pos = &emitter->unk28;
    radius = math_randomFloat() * pos->radius;
    particle.maxC = 0x6231; particle.minC = 0x1A4D; // matching: one source line (-g3 line numbers steer as1 scheduling)
    particle.color[0] = 0;
    particle.color[1] = 0;
    particle.color[2] = 0;
    particle.color[3] = 0xFF;
    particle.unk2C = 0xFF;
    particle.unk2D = 0xFF;
    particle.extraSize = 0;
    particle.flags = 1;
    particle.unk2E = 0;
    particle.unk2F = 1;
    particle.life = (math_random() % 201U) + 100;
    particle.effect = 0xF;
    particle.scale = 0.0f;
    particle.shrinkRate = 0.045f;
    particle.growRate = 0.7f;
    particle.pos.x = pos->x + radius * cosine;
    particle.pos.y = pos->y;
    particle.pos.z = pos->z + radius * sine;
    particle.maxHeight = 10000.0f;
    scaleA = (math_randomFloat() * 0.5f) + 1.0f;
    scaleB = (math_randomFloat() * 0.5f) + 1.0f;
    particle.maxA = 22.6f * scaleA;
    particle.minA = 13.786f * scaleA;
    particle.maxB = 57.969f * scaleB;
    particle.minB = 42.714f * scaleB;
    particle.altB = 118.763f * scaleB;
    particle_spawnChannelParticle(&particle, 0xA, 1, 0, emitter->base.header.owner, emitter->base.header.kind);
    emitter->base.header.state = (math_random() % 51U) + 25;
}

void func_150C2C00(Struct150C29F0 *emitter) {
    f32 *timer;
    f32 *pos;
    f32 distSq;
    ParticleEmitterDesc particle;
    f32 scaleA;
    f32 angle;
    f32 scaleB;
    ChannelParticle *created;

    timer = &emitter->unk28.x;
    *timer += math_randomFloat() * 20.0f * 1e-9f * gFrameDelta * 36447764.0f;
    if (*timer > 1.0f) {
        if (1) { // matching: the extra block makes IDO give emitter $s3 and pos $s2
            // matching: the camera position read as f32[3]; through Vec3f fields IDO emits other code
            pos = (f32 *)math_getCameraPos(gCurrentView);
            distSq = (pos[2] * pos[2]) + (pos[0] * pos[0]);
            if (2500000.0f < distSq) {
                particle.maxC = 0x6231;
                particle.minC = 0x1A4D;
                particle.pos.y = -390.0f;
                particle.color[0] = 0;
                particle.color[1] = 0;
                particle.color[2] = 0;
                particle.color[3] = 0xFF;
                particle.unk2D = 0xFF;
                particle.unk2E = 20;
                particle.unk2F = 1;
                particle.extraSize = 4;
                particle.flags = 1;
                particle.effect = 25;
                particle.maxHeight = 0.0f;
                particle.shrinkRate = 0.045f;
                particle.growRate = 0.7f;
                do {
                    angle = (math_randomFloat() * 5.8659425f) + 0.957204f;
                    if (math_angleDistance(math_atan2(pos[0], pos[2]), angle) < 0.4f) {
                        math_dirFromAngleXZ(angle, (math_randomFloat() * 1278.0f) + 3900.0f, &particle.pos.x, &particle.pos.z);
                        scaleA = ((math_randomFloat() * 1991.0f) + 1985.0f) * 0.001f;
                        scaleB = ((math_randomFloat() * 1493.0f) + 3000.0f) * 0.001f;
                        particle.maxA = 22.6f * scaleA;
                        particle.minA = 13.786f * scaleA;
                        particle.maxB = 57.969f * scaleB;
                        particle.minB = 42.714f * scaleB;
                        particle.altB = 118.763f * scaleB;
                        particle.life = (math_random() % 151U) + 50;
                        particle.scale = (math_randomFloat() * 500.0f) * 0.001f;
                        particle.unk2C = (math_random() % 156U) + 100;
                        created = particle_spawnChannelParticle(&particle, 0xA, 1, 0, emitter->base.header.owner, emitter->base.header.kind);
                        if (created != NULL) {
                            memcpy(&created->extra, &angle, sizeof(angle));
                        }
                    }
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            } else {
                do {
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            }
        }
    }
}
s32 particle_shrinkBallisticChannelParticle(ChannelParticle *particle) {
    particle->head.def.width -= particle->head.def.width * particle->motion.shrinkRate;
    particle->head.def.height -= particle->head.def.height * particle->motion.shrinkRate;
    particle->head.def.pos.y += (particle->head.def.scale.y * gFrameDelta) + (0.5f * particle->head.def.scale.x * gFrameDelta * gFrameDelta);
    particle->head.def.scale.y += particle->head.def.scale.x * gFrameDelta;
    if ((particle->head.def.width < 10.0f) || (particle->head.def.height < 10.0f)) {
        return 0;
    }
    return 1;
}
s32 func_150C308C(ChannelParticle *particle) {
    u8 triggered;
    f32 value; // matching: one local for the squared distance and then the angle (IDO puts both in $f12)
    Vec3f *pos;

    triggered = FALSE;
    if (particle->head.def.lifetime >= 6) {
        pos = math_getCameraPos(gCurrentView);
        value = pos->x * pos->x + pos->z * pos->z;
        if (value < 2500000.0f) {
            triggered = TRUE;
        } else {
            value = math_atan2(pos->x, pos->z);
            if (0.4f < math_angleDistance(value, particle->extra)) {
                triggered = TRUE;
            }
        }
    }
    if (triggered) {
        particle->head.def.lifetime = 5;
    }
    return 1;
}
