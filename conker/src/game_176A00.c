#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawns a particle described by `emitter`, with randomly flickering size and channel C. */
ChannelParticle *particle_spawnChannelParticle(ParticleEmitterDesc *emitter, u8 arg1, u8 arg2, u8 kind, u8 arg4, s32 arg5) {
    ChannelParticle *particle;
    ParticleDef spawn;
    ParticleChannels motion;
    s32 flip;

    spawn.flags = emitter->flags | 0x40000000;
    spawn.pos = emitter->pos;
    if (arg2) {
        spawn.flags |= 0x01000000;
        collision_selectMap(0);
        spawn.unk48 = collision_findZoneLeaf(spawn.pos.x, spawn.pos.z);
    } else {
        spawn.flags &= ~0x01000000;
        spawn.unk48 = 0;
    }

    switch (kind) {
    case 1:
        spawn.texture = 0x13;
        break;
    case 2:
        spawn.texture = 0x71;
        break;
    case 0:
    default:
        if (func_150ADA20() & 1) {
            spawn.texture = 0x13;
        } else {
            spawn.texture = 0x14;
        }
        break;
    }

    spawn.renderModes = 0x301;
    spawn.lifetime = emitter->life;
    spawn.rgba[0] = emitter->color[0];
    spawn.rgba[1] = emitter->color[1];
    spawn.rgba[2] = emitter->color[2];
    spawn.rgba[3] = emitter->color[3];
    spawn.unk44 = emitter->unk2C;
    spawn.unk45 = emitter->unk2D;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = emitter->scale;
    spawn.scale.y = 0.0f;
    spawn.scale.z = 1.0f;

    motion.maxA = emitter->maxA;
    motion.minA = emitter->minA;
    motion.timerA = 0;
    motion.periodA = 6;
    motion.targetA = (motion.maxA + motion.minA) * 0.5f;
    motion.rateA = 0.186f;
    motion.maxB = emitter->maxB;
    motion.minB = emitter->minB;
    motion.altB = emitter->altB;
    motion.targetB = (motion.maxB + motion.minB) * 0.5f;
    motion.timerB = 0;
    motion.periodB = 17;
    motion.rateB = 0.359f;
    motion.maxC = emitter->maxC;
    motion.minC = emitter->minC;
    motion.targetC = 0;
    motion.timerC = 0;
    motion.periodC = 15;
    motion.rateC = 0.231f;
    motion.shrinkRate = emitter->shrinkRate;
    motion.growRate = emitter->growRate;
    spawn.frame = 0;
    spawn.frameSpeed = (motion.maxC + motion.minC) >> 1;
    spawn.width = 0.0f;
    spawn.height = 0.0f;
    motion.effect = emitter->effect;
    motion.maxHeight = emitter->maxHeight;

    if (func_150ADA20() & 1) {
        flip = 1;
    } else {
        flip = 0;
    }
    particle = particle_newQuadParticleWhite(&spawn, emitter->unk2E, 0xE, emitter->unk2F, arg1, flip | 2,
                             emitter->extraSize + sizeof(ParticleChannels), arg4, arg5);
    if (particle != NULL) {
        memcpy(&particle->motion, &motion, sizeof(motion));
    }
    return particle;
}

/* Per-frame update: re-rolls each motion channel's target when its timer expires and eases
 * width, height and unk24 towards them. Returns 1 (keep alive). */
s32 particle_flickerChannelParticle(ChannelParticle *particle) {
    ParticleChannels *motion = &particle->motion;

    motion->timerA -= D_800BE9E4;
    if (motion->timerA < 0) {
        motion->timerA = func_150ADA20() % motion->periodA;
        motion->targetA = func_150ADA68() * (motion->maxA - motion->minA) + motion->minA;
    }
    particle->head.def.width += (motion->targetA - particle->head.def.width) * motion->rateA;

    motion->timerB -= D_800BE9E4;
    if (motion->timerB < 0) {
        motion->timerB = func_150ADA20() % motion->periodB;
        if (func_150ADA20() & 3) {
            motion->targetB = func_150ADA68() * (motion->maxB - motion->minB) + motion->minB;
        } else {
            motion->targetB = func_150ADA68() * (motion->altB - motion->maxB) + motion->maxB;
        }
    }
    particle->head.def.height += (motion->targetB - particle->head.def.height) * motion->rateB;

    motion->timerC -= D_800BE9E4;
    if (motion->timerC < 0) {
        motion->timerC = func_150ADA20() % motion->periodC;
        motion->targetC = func_150ADA20() % (motion->maxC - motion->minC + 1) + motion->minC;
    }
    particle->head.def.frameSpeed += (s32)(((f32)motion->targetC - (f32)particle->head.def.frameSpeed) * motion->rateC);

    if (particle->head.def.lifetime < 5) {
        particle_setQuadParticleFuncs(&particle->head, particle->head.updateFn1, motion->effect, particle->head.expireFn, particle->head.vertexFn);
        particle->head.def.lifetime = 300;
        particle->head.def.flags &= ~1;
    }
    return 1;
}

/* Grows width and height towards their targets; once both are within 95%, switches the
 * particle to effect 0xD. Returns 1 (keep alive). */
s32 particle_growChannelParticle(ChannelParticle *particle) {
    ParticleChannels *motion = &particle->motion;
    s32 flags = particle->head.unk74;
    s32 grown = TRUE;

    if (!(flags & 2)) {
        particle->head.def.width += (motion->targetA - particle->head.def.width) * motion->growRate;
        if (motion->targetA * 0.95f < particle->head.def.width) {
            flags = particle->head.unk74 |= 2;
        } else {
            grown = FALSE;
            flags = particle->head.unk74;
        }
    }
    if (!(flags & 8)) {
        particle->head.def.height += (motion->targetB - particle->head.def.height) * motion->growRate;
        if (motion->targetB * 0.95f < particle->head.def.height) {
            particle->head.unk74 |= 8;
        } else {
            grown = FALSE;
        }
    }
    if (grown) {
        particle_setQuadParticleFuncs(&particle->head, particle->head.updateFn1, 0xD, particle->head.expireFn, particle->head.vertexFn);
    }
    if (particle->head.def.lifetime < 5) {
        particle_setQuadParticleFuncs(&particle->head, particle->head.updateFn1, motion->effect, particle->head.expireFn, particle->head.vertexFn);
        particle->head.def.lifetime = 100;
        particle->head.def.flags &= ~1;
    }
    return 1;
}

/* Shrinks the particle; returns 0 (kill) once either side is below 2 units. */
s32 particle_shrinkChannelParticle(ChannelParticle *particle) {
    particle->head.def.width -= particle->head.def.width * particle->motion.shrinkRate;
    particle->head.def.height -= particle->head.def.height * particle->motion.shrinkRate;
    if (particle->head.def.width < 2.0f || particle->head.def.height < 2.0f) {
        return 0;
    }
    return 1;
}

/* Shrinks the particle while it accelerates upwards; returns 0 (kill) once it rises past
 * maxHeight or either side drops below 4 units. */
s32 particle_shrinkRisingChannelParticle(ChannelParticle *particle) {
    particle->head.def.width -= particle->head.def.width * particle->motion.shrinkRate;
    particle->head.def.height -= particle->head.def.height * particle->motion.shrinkRate;
    particle->head.def.scale.y += particle->head.def.scale.x * gFrameDelta;
    particle->head.def.pos.y += particle->head.def.scale.y * gFrameDelta;
    if (particle->motion.maxHeight < particle->head.def.pos.y) {
        return 0;
    }
    if (particle->head.def.width < 4.0f || particle->head.def.height < 4.0f) {
        return 0;
    }
    return 1;
}

/* Fetches this frame's vertex buffer for billboard quad `index` (initialising both halves from
 * quadTemplate when newly allocated) and positions its four corners around the particle,
 * rotated by the angle tables D_800DD1D8/D_800DD1E8. Returns NULL if no buffer is available. */
Vtx *particle_buildChannelQuad(ChannelParticle *particle, s16 index) {
    Vtx *vtx;
    Vtx *result; // matching: IDO keeps a spilled copy of vtx for the return
    f32 dz;
    f32 dx;
    u8 fresh;

    particle_getViewBuffer(particle->head.vtxBuffers, index, sizeof(particle->head.vtxTemplate), &vtx, &fresh);
    result = vtx;
    if (vtx != NULL) {
        if (fresh) {
            memcpy(particle->head.vtxBuffers[index], particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
            memcpy(particle->head.vtxBuffers[index] + 4, particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
        }
    } else {
        return NULL;
    }

    dz = D_800DD1D8[index] * particle->head.def.width;
    dx = D_800DD1E8[index] * particle->head.def.width;

    vtx[0].v.ob[0] = vtx[3].v.ob[0] = particle->head.def.pos.x + dx;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = particle->head.def.pos.y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = particle->head.def.pos.z - dz;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = particle->head.def.pos.x - dx;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = particle->head.def.pos.y + particle->head.def.height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = particle->head.def.pos.z + dz;

    return result;
}

/* Spawns a randomised particle_spawnGrowingBillboard effect at the particle's position. Returns 0. */
s32 particle_spawnBillboardOnExpire(ChannelParticle *particle) {
    BillboardSpawn effect;

    effect.pos = particle->head.def.pos;
    effect.width = 0;
    effect.height = 0;
    effect.widthStep = effect.heightStep = (func_150ADA20() % 6U) + 8;
    effect.velX = 0;
    effect.velZ = 0;
    effect.velY = (func_150ADA20() % 201U) + 100;
    effect.accelY = (func_150ADA20() % 5U) + 3;
    effect.life = 600;
    func_151429E0(3, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
    func_151429E0(4, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
    effect.intensity = 0xFF;
    effect.alpha = (func_150ADA20() % 65U) + 92;
    effect.fadeStep = (func_150ADA20() % 3U) + 1;
    effect.onExpire = 0;
    particle_spawnGrowingBillboard(&effect, 1);
    return 0;
}
