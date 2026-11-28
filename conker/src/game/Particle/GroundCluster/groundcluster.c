#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/* Builds the four camera-facing vertices of a beam quad (gQuadParticleVertexFuncs[4]): from def.pos (its start) toward
 * def.rot (its end), def.width wide, drawn def.height of the way to the end (its progress); within def.scale.x of the start
 * the drawn part is stretched. */
Vtx *particle_buildCameraFacingBeamQuad(DefParticleHead *beam, s16 cam) {
    Vtx *vertices;
    Vtx *result;
    f32 distanceSquared;
    Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    Vec3f *eye;
    f32 radiusSquared;
    u8 fresh;

    particle_getViewBuffer(beam->vtxBuffers, cam, sizeof(beam->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(beam->vtxBuffers[cam], beam->vtxTemplate, sizeof(beam->vtxTemplate));
            memcpy(beam->vtxBuffers[cam] + 4, beam->vtxTemplate, sizeof(beam->vtxTemplate));
        }
    } else {
        return NULL;
    }
    dx = beam->def.rot.x - beam->def.pos.x;
    dy = beam->def.rot.y - beam->def.pos.y;
    dz = beam->def.rot.z - beam->def.pos.z;
    tip.x = beam->def.pos.x + dx * beam->def.height;
    tip.y = beam->def.pos.y + dy * beam->def.height;
    tip.z = beam->def.pos.z + dz * beam->def.height;
    eye = &camera_global_Cameras[cam].eye;
    dx = tip.x - beam->def.pos.x;
    dy = tip.y - beam->def.pos.y;
    dz = tip.z - beam->def.pos.z;
    distanceSquared = dx * dx + dy * dy + dz * dz;
    radiusSquared = beam->def.scale.x * beam->def.scale.x;
    if (distanceSquared < radiusSquared) {
        f32 factor = 1.0f - sqrtf(distanceSquared) / beam->def.scale.x;

        dx += dx * factor;
        dy += dy * factor;
        dz += dz * factor;
    }
    vx = beam->def.pos.x - eye->x;
    vy = beam->def.pos.y - eye->y;
    vz = beam->def.pos.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        f32 scale = beam->def.width / sqrtf(lengthSquared);

        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->v.ob[0] = beam->def.pos.x + nx;
    vertices->v.ob[1] = beam->def.pos.y + ny;
    vertices->v.ob[2] = beam->def.pos.z + nz;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = beam->def.pos.x - nx;
    vertices->v.ob[1] = beam->def.pos.y - ny;
    vertices->v.ob[2] = beam->def.pos.z - nz;
    vertices->v.flag = 0;
    vertices++;
    dx = -dx;
    dy = -dy;
    dz = -dz;
    vx = tip.x - eye->x;
    vy = tip.y - eye->y;
    vz = tip.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        f32 scale = beam->def.width / sqrtf(lengthSquared);

        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->v.ob[0] = tip.x + nx;
    vertices->v.ob[1] = tip.y + ny;
    vertices->v.ob[2] = tip.z + nz;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = tip.x - nx;
    vertices->v.ob[1] = tip.y - ny;
    vertices->v.ob[2] = tip.z - nz;
    vertices->v.flag = 0;
    return result;
}

/* Spawns `count` particles at random points within `radius` of (x, y, z). */
void particle_scatterChannelParticles(f32 x, f32 y, f32 z, f32 radius, s16 count, f32 sizeBase, f32 sizeRange,
                   f32 heightBase, f32 heightRange, s16 lifeBase, s16 lifeRange, u8 expireFn) {
    ParticleEmitterDesc particle;
    u8 angle;
    f32 sin;
    f32 cos;
    f32 dist;
    f32 size;
    f32 height;

    if (count > 0) {
        // matching: both stores on one line (-g3 line numbers steer as1 scheduling)
        particle.maxC = 0x6231; particle.minC = 0x1A4D;
        particle.color[0] = 0;
        particle.color[1] = 0;
        particle.color[2] = 0;
        particle.color[3] = 0xFF;
        particle.flags = 1;
        particle.alpha = 0xFF;
        particle.colorLevel = 0xFF;
        particle.expireFn = expireFn;
        particle.scale = 0.0f;
        particle.shrinkRate = 0.045f;
        particle.growRate = 0.7f;
        particle.extraSize = 0;
        particle.updateFn1 = 0;
        particle.effect = 15;
        particle.pos.y = y;

        do {
            // matching: explicit mask; math_random really returns a full word
            angle = math_random() & 0xFF;
            sin = math_cosBinAngleMasked(angle - 0x40);
            cos = math_cosBinAngleMasked(angle);
            dist = math_randomFloat() * radius;
            size = math_randomFloat() * sizeRange + sizeBase;
            height = math_randomFloat() * heightRange + heightBase;
            particle.pos.x = dist * sin + x;
            particle.pos.z = dist * cos + z;
            particle.maxA = 22.6f * size;
            particle.minA = 13.786f * size;
            particle.maxB = 57.969f * height;
            particle.minB = 42.714f * height;
            particle.altB = 118.763f * height;
            particle.life = math_random() % (lifeRange + 1) + lifeBase;
            particle_spawnChannelParticle(&particle, 10, 0, 0, 0xFF, 1);
            count--;
        } while (count != 0);
    }
}

/* Spawns one particle at `pos` with randomized size, lifetime and fade. */
void particle_spawnRandomChannelParticle(Vec3f *pos, u8 owner, s32 list) {
    ParticleEmitterDesc particle;
    f32 size;
    f32 height;

    // matching: both stores on one line (-g3 line numbers steer as1 scheduling)
    particle.maxC = 0x6231; particle.minC = 0x1A4D; particle.pos = *pos;
    particle.color[0] = 0;
    particle.color[1] = 0;
    particle.color[2] = 0;
    particle.color[3] = 0xFF;
    particle.colorLevel = 0xFF;
    particle.updateFn1 = 0;
    particle.expireFn = 1;
    particle.extraSize = 0;
    particle.flags = 1;
    particle.effect = 28;
    particle.maxHeight = 0.0f;
    particle.shrinkRate = 0.080000006f;
    particle.growRate = 0.4f;
    size = (math_randomFloat() * 501.0f + 999.0f) * 0.001f;
    height = math_randomFloat();
    particle.maxA = 22.6f * size;
    particle.minA = 13.786f * size;
    height = (height * 1007.0f + 1505.0f) * 0.001f;
    particle.maxB = 57.969f * height;
    particle.minB = 42.714f * height;
    particle.altB = 118.763f * height;
    particle.life = math_random() % 17U + 16;
    particle.scale = (math_randomFloat() * 347.0f + 50.0f) * 0.001f;
    particle.alpha = math_random() % 156U + 100;
    particle_spawnChannelParticle(&particle, 10, 0, 0, owner, list);
}

/*
 * Shrinks unk2C/unk30 by the fraction shrinkFraction and integrates unk38 with velocity unk50 and
 * acceleration unk4C over one frame. Returns 0 once either size drops below 10.
 */
s32 particle_shrinkBallisticQuad(Particle177B50 *p) {
    p->head.def.width -= p->head.def.width * p->shrinkFraction;
    p->head.def.height -= p->head.def.height * p->shrinkFraction;
    p->head.def.pos.y += p->head.def.scale.y * gFrameDelta + 0.5f * p->head.def.scale.x * gFrameDelta * gFrameDelta;
    p->head.def.scale.y += p->head.def.scale.x * gFrameDelta;
    if (p->head.def.width < 10.0f || p->head.def.height < 10.0f) {
        return 0;
    }
    return 1;
}

/* Spawns a random number of particles scattered within 42 units of `pos` in the XZ plane. */
void particle_scatterChannelParticlesInDisc(Vec3f *pos, u8 owner, s32 list) {
    f32 count;
    ParticleEmitterDesc particle;
    s32 angle;
    f32 size;
    f32 height;

    count = (math_randomFloat() * 20.0f + 29.0f) * 0.0001f * 5541.7695f;
    if (count > 1.0f) {
        particle.maxC = 0x6231;
        particle.minC = 0x1A4D;
        particle.pos.y = pos->y;
        particle.color[0] = 0;
        particle.color[1] = 0;
        particle.color[2] = 0;
        particle.color[3] = 0xFF;
        particle.colorLevel = 0xFF;
        particle.updateFn1 = 0;
        particle.expireFn = 1;
        particle.extraSize = 0;
        particle.flags = 1;
        particle.effect = 28;
        particle.maxHeight = 0.0f;
        do {
            angle = math_random();
            math_dirFromBinAngleXZ(angle & 0xFF, math_randomFloat() * 42.0f, &particle.pos.x, &particle.pos.z);
            particle.pos.x += pos->x;
            particle.pos.z += pos->z;
            particle.shrinkRate = (math_randomFloat() * 16.0f + 29.0f) * 0.001f;
            particle.growRate = (math_randomFloat() * 600.0f + 400.0f) * 0.001f;
            size = (math_randomFloat() * 1205.0f + 304.0f) * 0.001f;
            height = (math_randomFloat() * 1006.0f + 504.0f) * 0.001f;
            particle.maxA = 22.6f * size;
            particle.minA = 13.786f * size;
            particle.maxB = 57.969f * height;
            particle.minB = 42.714f * height;
            particle.altB = 118.763f * height;
            particle.life = math_random() % 31U + 20;
            particle.scale = math_randomFloat() * 497.0f * 0.001f;
            particle.alpha = math_random() % 156U + 100;
            particle_spawnChannelParticle(&particle, 10, 0, 0, owner, list);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}


/* If collision_probeFloorSurfABSet0Cached finds ground within 200 units of `position`, spawns a cluster of effects there. */
void particle_spawnGroundCluster(Vec3f *position, CollisionHit *ground, u8 owner, s32 list) {
    Vec3f pos;

    pos = *position;
    {
        Vec3f probe;

        probe.x = position->x;
        probe.y = position->y + 200.0f;
        probe.z = position->z;
        if (collision_probeFloorSurfABSet0Cached(&probe, 0, position->y - 200.0f, ground) == 0) {
            return;
        }
    }
    {
        TrailBurst spawn;

        spawn.countBase = 10;
        spawn.countRange = 0;
        spawn.sizeBase = 9.900001f;
        spawn.sizeRange = 21.0f;
        spawn.gravityBase = -3.0000002f;
        spawn.gravityRange = 1.016f;
        spawn.speedBase = 30.0f;
        spawn.speedRange = 30.4f;
        spawn.angles.yawBase = 0;
        spawn.angles.yawRange = 0xFF;
        spawn.angles.pitchBase = -63;
        spawn.angles.pitchRange = 43;
        spawn.capacityBase = 4;
        spawn.capacityRange = 5;
        spawn.lifeBase = 30;
        spawn.lifeRange = 30;
        spawn.headerFlags = 1;
        spawn.flags = 12;
        spawn.primMode = 2;
        spawn.envMode = 3;
        spawn.colorBase.red = 180;
        spawn.colorBase.green = 0;
        spawn.colorBase.blue = 0;
        spawn.colorBase.alpha = 155;
        spawn.colorRange.red = 50;
        spawn.colorRange.green = 100;
        spawn.colorRange.blue = 0;
        spawn.colorRange.alpha = 100;
        spawn.color2Base.red = 0xFF;
        spawn.color2Base.green = 0xFF;
        spawn.color2Base.blue = 0xFF;
        spawn.color2Base.alpha = 0xFF;
        spawn.color2Range.red = 0;
        spawn.color2Range.green = 0;
        spawn.color2Range.blue = 0;
        spawn.color2Range.alpha = 0;
        spawn.colorLevelBase = 0xFF;
        spawn.colorLevelRange = 0;
        spawn.renderMode = 1;
        spawn.combiner = 36;
        spawn.setGeometryMode = 0x200005;
        spawn.clearGeometryMode = 0x60600;
        spawn.fadeStart = 20;
        spawn.fadeRate = 12;
        spawn.shrinkStart = 1;
        spawn.shrinkPaused = 0;
        spawn.shrinkFactor = 1.0; // matching: a double literal; 1.0f loads differently
        spawn.moveFn = -1;
        spawn.shapeFn = 0;
        spawn.unk72 = -1;
        spawn.unk73 = -1;
        spawn.pos = pos;
        particle_spawnTrailBurst(&spawn, owner, list);
    }
    {
        SimplePuffSpray emit;

        emit.pos = pos;
        emit.sizeBase = 3.99f;
        emit.sizeRange = 8.11f;
        emit.growthBase = 0.60200006f;
        emit.growthRange = 0.407f;
        emit.speedBase = 10.139999f;
        emit.speedRange = 10.2699995f;
        emit.countBase = 8;
        emit.countRange = 8;
        emit.angles.yawBase = 0;
        emit.angles.yawRange = 0xFF;
        emit.angles.pitchBase = -64;
        emit.angles.pitchRange = 44;
        emit.itemCountBase = 5;
        emit.itemCountRange = 4;
        emit.lifeBase = 66;
        emit.lifeRange = 30;
        emit.green = 10;
        emit.red = 33;
        emit.handlers[0] = 1;
        emit.handlers[1] = 12;
        emit.handlers[2] = 0;
        emit.handlers[3] = 0;
        emit.handlers[4] = 0;
        emit.handlers[5] = 0;
        emit.blue = 0xFF;
        emit.alpha = math_random() % 101U + 155;
        particle_spawnSimplePuffSpray(&emit, 0xFF);
    }
    particle_scatterChannelParticlesInDisc(&pos, owner, list);

    {
        ParticleDef desc;
        s32 randA;
        s32 randC;
        s32 randB;
        s32 sum;

        desc.texture = 56;
        desc.renderModes = 1;
        desc.lifetime = 50;
        desc.frame = 0;
        desc.frameSpeed = 0x6666;
        desc.rgba[3] = 0xFF;
        desc.width = 1.0f;
        desc.height = 1.0f;
        desc.pos.x = pos.x;
        desc.pos.y = pos.y + 10.0f;
        desc.pos.z = pos.z;
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 0.2f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.flags = 0x400C0001;
        desc.alpha = math_random() % 101U + 100;
        desc.colorLevel = 0xFF;
        desc.primMode = 0;
        desc.envMode = 6;
        desc.rgba[0] = math_random() % 51U + 180;
        desc.rgba[1] = math_random() % 101U;
        desc.rgba[2] = math_random() % 1U;
        randA = math_random();
        randB = math_random();
        randC = math_random();
        sum = (randB & 1) + (randA & 1);
        particle_spawnAxisQuadParticleNoModel(&desc, 0, 18, 29, 0, sum, (u8)randC,
                      100.0f, 100.0f, NULL, &ground->tri, 0, 0, owner, list);
    }
}

/* Trail update handler (gMotionTrailUpdateProcs): fades out over the last 16 frames of life, capping the alpha at life * 16. */
s32 particle_fadeTrailByLife(MotionTrail *trail) {
    TrailParams *params = trail->params;

    if (trail->head.tmpl.life < 16 && trail->head.tmpl.life * 16 < params->motion.alpha) {
        params->motion.alpha = trail->head.tmpl.life * 16;
    }
    return 1;
}

/* Quad particle update (gQuadParticleUpdate1Funcs[18]): grows def.width and def.height by the one rate def.scale.x, times
 * gFrameDelta (effect_growQuadAtRates has a rate for each). */
s32 particle_growQuadAtRate(Particle177B50 *p) {
    f32 growth = p->head.def.scale.x * gFrameDelta;

    p->head.def.width += growth;
    p->head.def.height += growth;
    return 1;
}

/* Fades out over the last 16 frames of life: caps alpha at life * 16. */
s32 particle_capQuadAlphaByLife(Particle177B50 *p) {
    if (p->head.def.lifetime < 16 && p->head.def.lifetime * 16 < p->head.def.alpha) {
        p->head.def.alpha = p->head.def.lifetime * 16;
    }
    return 1;
}

/*
 * Spawns a particle at `pos` through particle_newQuadParticle and stores its initial particle_updateJitter state.
 * A `life` of -1 means 300 frames without the timed-life flag.
 */
void *particle_spawnJitterParticle(Vec3f *pos, ParticleSize *scale, s16 life, u8 alpha, const Vec3f *velocity,
                    f32 gravity, f32 drag, f32 easeFactor, u8 updateFn1, u8 updateFn2, u8 expireFn, s32 texture, u8 flags,
                    s16 fadeStart, s16 fadeRate, s32 extraSize, u8 owner, s32 list) {
    void *particle;
    ParticleDef desc;
    ParticleJitter state;
    s32 flag;

    if (velocity != NULL) {
        state.velocity = *velocity;
    } else {
        state.velocity.x = 0.0f;
        state.velocity.y = 0.0f;
        state.velocity.z = 0.0f;
    }
    state.gravity = gravity;
    state.widthTimer = 0.0f;
    state.heightTimer = 0.0f;
    state.speedTimer = 0.0f;
    desc.frameSpeed = 15935;
    state.speed = 15935.0f;
    state.speedTarget = 15935.0f;
    state.drag = drag;
    state.easeFactor = easeFactor;
    state.speedBase = 6733.0f;
    state.speedRange = 18404.0f;
    state.widthBase = scale->width * 15.045272f;
    state.widthRange = scale->width * 5.8676558f;
    state.widthTarget = scale->width * 12.111444f;
    desc.width = 0.0f;
    state.heightBase = scale->height * 0.396174f;
    state.heightRangeA = scale->height * 0.1042563f;
    state.heightRangeB = scale->height * 0.5197371f;
    state.heightTarget = scale->height * 0.34404588f;
    desc.height = 0.0f;
    desc.texture = texture;
    desc.type = 0;
    desc.renderModes = 0x2203;
    if (life == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = life;
    }
    desc.frame = 0;
    desc.rgba[0] = 0xFF;
    desc.rgba[1] = 0xFF;
    desc.rgba[2] = 0xFF;
    desc.rgba[3] = 0xFF;
    desc.pos = *pos;
    desc.rot.x = pos->x;
    desc.rot.y = pos->y + 100.0f;
    desc.rot.z = pos->z;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = ((life == -1) ? 0 : 1) | 0x40000 | 0x80000 | 0x200000 | 0x400000 | ((flags & 1) ? 0x800000 : 0) | ((flags & 2) ? 0x2000000 : 0) | 0x40000000;
    desc.alpha = alpha;
    desc.colorLevel = 0xFF;
    desc.primMode = 0;
    desc.envMode = 7;
    desc.zone = 0;
    desc.fadeStart = fadeStart;
    desc.fadeRate = fadeRate;
    if (math_random() & 1) {
        flag = 1;
    } else {
        flag = 0;
    }
    particle = particle_newQuadParticle(&desc, &gWhiteVtxColors, updateFn1, updateFn2, expireFn, 28, flag | 2, 0, 0, extraSize + 80, owner, list);
    if (particle != NULL) {
        memcpy((u8 *)particle + 0x110, &state, sizeof(state));
    }
    return particle;
}

/*
 * Eases def.width, def.height and def.frameSpeed toward random targets, picking a new target for each when its
 * countdown timer (decremented by the frame delta) runs out.
 */
s32 particle_updateJitter(JitterParticle *p, ParticleJitter *w) {
    w->widthTimer -= gFrameDelta;
    if (w->widthTimer < 0.0f) {
        w->widthTimer = math_randomFloat() * 4.0f;
        w->widthTarget = math_randomFloat() * w->widthRange + w->widthBase;
    }
    p->head.def.width += (w->widthTarget - p->head.def.width) * 0.35f;
    w->heightTimer -= gFrameDelta;
    if (w->heightTimer < 0.0f) {
        w->heightTimer = math_randomFloat() * 9.0f;
        if (math_random() & 1) {
            w->heightTarget = math_randomFloat() * w->heightRangeA + w->heightBase;
        } else {
            w->heightTarget = math_randomFloat() * w->heightRangeB + w->heightBase;
        }
    }
    p->head.def.height += (w->heightTarget - p->head.def.height) * 0.4f;
    w->speedTimer -= gFrameDelta;
    if (w->speedTimer < 0.0f) {
        w->speedTimer = math_randomFloat() * 7.0f;
        w->speedTarget = math_randomFloat() * w->speedRange + w->speedBase;
    }
    w->speed += (w->speedTarget - w->speed) * 0.4f;
    p->head.def.frameSpeed = w->speed;
    return 1;
}

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void particle_updateJitterParticle(JitterParticle *p) {
    particle_updateJitter(p, &p->jitter);
}

/*
 * Eases unk40 toward the point 100 units above the particle's position by easeFactor,
 * then pins it back to exactly 100 units from the position along that direction.
 */
void particle_easeBendTarget(void *arg) {
    Vec3f above;
    Vec3f eased;
    JitterParticle *p;
    Vec3f dir;
    f32 length;
    f32 invLength;

    // matching: the void * parameter copied into a typed local hoists &dir's address to entry
    p = arg;
    above.x = p->head.def.pos.x;
    above.y = p->head.def.pos.y + 100.0f;
    above.z = p->head.def.pos.z;
    eased.x = p->head.def.rot.x + (above.x - p->head.def.rot.x) * p->jitter.easeFactor;
    eased.y = p->head.def.rot.y + (above.y - p->head.def.rot.y) * p->jitter.easeFactor;
    eased.z = p->head.def.rot.z + (above.z - p->head.def.rot.z) * p->jitter.easeFactor;
    {
        s32 pad; // matching: stack slot needed for the original frame layout

        dir.x = eased.x - p->head.def.pos.x;
        dir.y = eased.y - p->head.def.pos.y;
        dir.z = eased.z - p->head.def.pos.z;
        if (math_vec3Normalize(&dir, &dir, &length, &invLength)) {
            p->head.def.rot.x = p->head.def.pos.x + dir.x * 100.0f;
            p->head.def.rot.y = p->head.def.pos.y + dir.y * 100.0f;
            p->head.def.rot.z = p->head.def.pos.z + dir.z * 100.0f;
        } else {
            p->head.def.rot = p->head.def.pos;
        }
    }
}

/* Sets unk40 to the point 100 units above the particle's position. */
void particle_resetBendTarget(JitterParticle *p) {
    p->head.def.rot.x = p->head.def.pos.x;
    p->head.def.rot.y = p->head.def.pos.y + 100.0f;
    p->head.def.rot.z = p->head.def.pos.z;
}

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void particle_updateRelaunchedJitter(JitterParticle *p) {
    particle_updateJitter(p, &p->jitter);
}

/*
 * Advances the particle one frame: applies drag once per elapsed tick and gravity to the
 * velocity, then moves the position by the average of the old and new velocity.
 */
void particle_moveJitterBallistic(JitterParticle *p) {
    ParticleJitter *motion;
    Vec3f oldVel;
    s32 ticks;

    motion = &p->jitter;
    oldVel = motion->velocity;
    for (ticks = gFrameTicks; ticks != 0; ticks--) {
        motion->velocity.x *= motion->drag;
        motion->velocity.y *= motion->drag;
        motion->velocity.z *= motion->drag;
    }
    {
        f32 accelX;
        f32 accelY;
        f32 accelZ;

        motion->velocity.y += motion->gravity * gFrameDelta;
        accelX = (motion->velocity.x - oldVel.x) * gFrameDeltaInv;
        accelY = (motion->velocity.y - oldVel.y) * gFrameDeltaInv;
        accelZ = (motion->velocity.z - oldVel.z) * gFrameDeltaInv;
        p->head.def.pos.x += (oldVel.x + 0.5f * accelX * gFrameDelta) * gFrameDelta;
        p->head.def.pos.y += (oldVel.y + 0.5f * accelY * gFrameDelta) * gFrameDelta;
        p->head.def.pos.z += (oldVel.z + 0.5f * accelZ * gFrameDelta) * gFrameDelta;
    }
}

/* Update callback: ballistic motion, then particle_easeBendTarget. */
s32 particle_updateJitterBallisticEased(JitterParticle *p) {
    particle_moveJitterBallistic(p);
    particle_easeBendTarget(p);
    return 1;
}

/* Update callback: ballistic motion, then re-aims unk40 above the new position. */
s32 particle_updateJitterBallistic(JitterParticle *p) {
    particle_moveJitterBallistic(p);
    particle_resetBendTarget(p);
    return 1;
}

/* Resets the particle: life from relaunch.lifetime, gravity from relaunch.gravity, sets flag 0x08000000. */
s32 particle_relaunchJitterParticle(JitterParticle *p) {
    p->head.expireFn = 0;
    p->head.updateFn2 = 36;
    p->head.def.lifetime = p->relaunch.lifetime;
    p->head.def.flags |= 0x08000000;
    p->jitter.gravity = p->relaunch.gravity;
    return 1;
}
