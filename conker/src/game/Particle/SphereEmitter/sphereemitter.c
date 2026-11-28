#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/texture.h"

void func_151A0E40(f32 x, f32 y, f32 z, u8 arg3, s32 arg4) {
    func_151A11CC(x, y, z, 163.0f, 2.019000053f, 160, 75, 1055, 1270,
                  1, 2.019000053f, 135, 68, 1055, 1270, 2,
                  1, 2.019000053f, 70, 66, 1055, 1270, 7, 65,
                  4, 1, arg3, arg4);
}

void func_151A0F28(f32 x, f32 y, f32 z, u8 arg3, s32 arg4) {
    func_151A11CC(x, y, z, 163.0f, 2.019000053f, 160, 75, 1055, 1270,
                  1, 2.019000053f, 135, 68, 1055, 1270, 2,
                  1, 2.019000053f, 70, 66, 1055, 1270, 7, 65,
                  4, 1, arg3, arg4);
}

/* Calls func_151A11CC with parameters scaled by scaleA/scaleB and a duration-derived count. */
void func_151A1010(f32 x, f32 y, f32 z, s16 duration, f32 scaleA, f32 scaleB, u8 arg6, s32 arg7) {
    s16 count;
    s32 offset;
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s32 a32;
    s32 b32;
    s32 c32;
    s32 d32;

    count = duration - 32;
    offset = (s16)(duration - 32);
    if (offset <= 0) {
        count = 1;
    }
    // matching: the dead `| (gFrameDelta == 0 & 0)` global read (no code, no relocation) keeps uopt from
    // merging each s32 into its s16 copy; a parameter in its place changes the code
    func_151A11CC(x, y, z, 250.0f * scaleA, 1.0f,
                  a = a32 = (s32)(70.0f * scaleB) | (gFrameDelta == 0 & 0),
                  b = b32 = (s32)(30.0f * scaleB) | (gFrameDelta == 0 & 0),
                  c = c32 = (s32)(1500.0f * scaleA) | (gFrameDelta == 0 & 0),
                  d = d32 = (s32)(100.0f * scaleA) | (gFrameDelta == 0 & 0), 18, 1.0f,
                  90.0f * scaleB, 20.0f * scaleB, c, d,
                  count, 16, 1.0f,
                  a, b, c, d,
                  15.0f * scaleA, 10.0f * scaleA, 18, offset + 16, arg6, arg7);
}

/* Empty in this build; func_151A0E40, func_151A0F28 and func_151A1010 call it with full parameter lists. */
void func_151A11CC(f32 x, f32 y, f32 z, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27) {
}

/*
 * Extra data of the particles spawned by particle_updateGatherEmitter, read by its callback particle_updateGatherParticle.
 * The *Start/*End fields are values of BillboardParticle.life, which counts down.
 */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 moveEnd;
    /* 0x0E */ s16 velX;    /* 8.8 fixed point: integer parts here, fractions in velSub* */
    /* 0x10 */ s16 velY;
    /* 0x12 */ s16 velZ;
    /* 0x14 */ s16 velSubX;
    /* 0x16 */ s16 velSubY;
    /* 0x18 */ s16 velSubZ;
    /* 0x1A */ s16 fadeOutStart;
    /* 0x1C */ s16 fadeOutStep;
    /* 0x1E */ s16 shrinkStart;
    /* 0x20 */ s16 shrinkStep;
} ConvergeParticleData;


/* Spawns particles at random points in a sphere around the emitter; each drifts back to the centre. */
void particle_updateGatherEmitter(SphereEmitter *emitter) {
    EmitterParams *params;
    BillboardTemplate packet;
    ConvergeParticleData data;
    u8 angle1;
    u8 angle2;
    f32 cos1;
    f32 sin1;
    f32 cos2;
    f32 sin2;
    f32 dist;
    f32 horiz;
    f32 tz;
    f32 ty;
    f32 tx;
    s16 size;
    s16 life;
    f32 inv;
    f32 vx;
    BillboardParticle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * gFrameDelta;
    params = &emitter->params;
    if (emitter->params.spawnAccum > 1.0f) {
        packet.primR = 255;
        packet.primG = 255;
        packet.primB = 255;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x10;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.anim = &gEffectTextures[39];
        packet.velX = 0;
        packet.velY = 0;
        packet.velZ = 0;
        packet.accelY = 0;
        packet.fadeStep = 0;
        packet.primA = 0;
        packet.height = 2000;
        packet.width = 2000;
        do {
            angle1 = math_random() & 255;
            angle2 = math_random() & 255;
            cos1 = math_cosBinAngleMasked(angle1 - 64);
            sin1 = math_cosBinAngleMasked(angle1);
            cos2 = math_cosBinAngleMasked(angle2 - 64);
            sin2 = math_cosBinAngleMasked(angle2);
            dist = math_randomFloat() * params->radius;
            horiz = dist * sin2;
            tx = params->x + horiz * cos1;
            ty = params->y - dist * cos2;
            tz = params->z + horiz * sin1;
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
            }
            packet.frameStep = math_random() % (params->frameStepRange + 1) + params->frameStep;
            size = math_random() % (params->sizeRange + 1) + params->size;
            packet.life = gEffectTextures[39].frameCount * (256 / packet.frameStep);
            life = packet.life;
            data.growStart = data.fadeInStart = life;
            data.fadeInEnd = life - (life >> 2);
            data.growEnd = life >> 1;
            data.fadeInStep = 255 / (data.fadeInStart - data.fadeInEnd);
            data.growStep = size / (data.growStart - data.growEnd);
            data.fadeOutStart = life >> 2;
            data.fadeOutStep = 255 / data.fadeOutStart;
            data.shrinkStart = life >> 1;
            data.shrinkStep = size / data.shrinkStart;
            packet.x = params->x;
            packet.y = params->y;
            packet.z = params->z;
            packet.subX = (s32)(params->x * 256.0f) & 0xFF;
            packet.subY = (s32)(params->y * 256.0f) & 0xFF;
            packet.subZ = (s32)(params->z * 256.0f) & 0xFF;
            data.moveEnd = life >> 1;
            inv = 1.0f / data.moveEnd;
            vx = (tx - params->x) * inv;
            // matching: vy and vz reuse horiz and dist (separate locals change the frame)
            // matching: an empty block boundary here makes IDO keep vx in its own register (mov.s)
            do { } while (0);
            horiz = (ty - params->y) * inv;
            dist = (tz - params->z) * inv;
            data.velX = vx;
            data.velY = horiz;
            data.velZ = dist;
            data.velSubX = (u8)(s32)(vx * 256.0f);
            data.velSubY = (u8)(s32)(horiz * 256.0f);
            data.velSubZ = (u8)(s32)(dist * 256.0f);
            particle = effect_spawnBillboard(&packet, 0, sizeof(data), -1, emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
            // matching: a scoped local after the decrement re-materializes 1.0f at the loop test
            { s32 q; }
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x10 (particle_updateGatherEmitter's particles): fade in and grow, drift, then fade out and shrink. */
void particle_updateGatherParticle(BillboardParticle *particle) {
    ConvergeParticleData *data = (ConvergeParticleData *)(particle + 1);

    if (particle->tmpl.life > data->fadeInEnd) {
        particle->tmpl.primA = (data->fadeInStart - particle->tmpl.life) * data->fadeInStep;
    }
    if (particle->tmpl.life > data->growEnd) {
        particle->tmpl.width = particle->tmpl.height = (data->growStart - particle->tmpl.life) * data->growStep;
    }
    if (particle->tmpl.life > data->moveEnd) {
        s32 px, py, pz, vx, vy, vz;

        px = particle->tmpl.x;
        px = (px << 8) | particle->tmpl.subX;
        vx = data->velX;
        vx = (vx << 8) | data->velSubX;
        px += vx * gFrameTicks;
        particle->tmpl.x = px >> 8;
        particle->tmpl.subX = px & 0xFF;
        py = particle->tmpl.y;
        py = (py << 8) | particle->tmpl.subY;
        vy = data->velY;
        vy = (vy << 8) | data->velSubY;
        py += vy * gFrameTicks;
        particle->tmpl.y = py >> 8;
        particle->tmpl.subY = py & 0xFF;
        pz = particle->tmpl.z;
        pz = (pz << 8) | particle->tmpl.subZ;
        vz = data->velZ;
        vz = (vz << 8) | data->velSubZ;
        pz += vz * gFrameTicks;
        particle->tmpl.z = pz >> 8;
        particle->tmpl.subZ = pz & 0xFF;
    }
    if (particle->tmpl.life < data->fadeOutStart) {
        particle->tmpl.primA = particle->tmpl.life * data->fadeOutStep;
    }
    if (particle->tmpl.life < data->shrinkStart) {
        particle->tmpl.width = particle->tmpl.height = particle->tmpl.life * data->shrinkStep;
    }
}

/* Data copied to 0x28 of the effect created by particle_startHoverEmitter: EmitterParams without growRate and
 * growRateRange, with pos as the spawner's Words3 (a word copy). */
typedef struct {
    /* 0x00 */ Words3 pos;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 spawnAccum;
    /* 0x14 */ f32 spawnRate;
    /* 0x18 */ s16 frameStep;
    /* 0x1A */ s16 frameStepRange;
    /* 0x1C */ s16 size;
    /* 0x1E */ s16 sizeRange;
} HoverEmitterParams;

/* Creates a type 3 effect_newTimedEffectNoDraw effect from the spawner's settings. */
void particle_startHoverEmitter(EffectSpawner *spawner) {
    s32 pad; // matching: unused stack slot
    HoverEmitterParams data;
    TimedEffect *effect;

    data.pos = spawner->pos;
    data.radius = spawner->radius;
    data.spawnAccum = 0.0f;
    data.spawnRate = spawner->spawnRate;
    data.frameStep = spawner->frameStep;
    data.frameStepRange = spawner->frameStepRange;
    data.size = spawner->size;
    data.sizeRange = spawner->sizeRange;

    effect = effect_newTimedEffectNoDraw(spawner->lifetime, -1, 3, 1, 0, 0x20, spawner->base.header.owner, spawner->base.header.kind);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/* Extra data of the particles spawned by particle_updateHoverEmitter, read by its callback particle_updateHoverParticle. */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 fadeOutStart;
    /* 0x0E */ s16 fadeOutStep;
    /* 0x10 */ s16 shrinkStart;
    /* 0x12 */ s16 shrinkStep;
} PuffParticleData;

/* Spawns particles at random points in a sphere around the emitter; they stay where they are. */
void particle_updateHoverEmitter(SphereEmitter *emitter) {
    EmitterParams *params;
    BillboardTemplate packet;
    PuffParticleData data;
    f32 cos1;
    f32 sin1;
    f32 cos2;
    f32 sin2;
    f32 dist;
    f32 x;
    f32 y;
    f32 z;
    u8 angle1;
    u8 angle2;
    s16 size;
    s16 life;
    BillboardParticle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * gFrameDelta;
    if (emitter->params.spawnAccum > 1.0f) {
        params = &emitter->params;
        packet.primR = 255;
        packet.primG = 255;
        packet.primB = 255;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x11;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.anim = &gEffectTextures[39];
        packet.velX = 0;
        packet.velY = 0;
        packet.velZ = 0;
        packet.accelY = 0;
        packet.fadeStep = 0;
        packet.primA = 0;
        packet.width = packet.height = 2000;
        do {
            angle1 = math_random() & 255;
            angle2 = math_random() & 255;
            cos1 = math_cosBinAngleMasked(angle1 - 64);
            sin1 = math_cosBinAngleMasked(angle1);
            cos2 = math_cosBinAngleMasked(angle2 - 64);
            sin2 = math_cosBinAngleMasked(angle2);
            dist = math_randomFloat() * params->radius;
            x = params->x + dist * sin2 * cos1;
            y = params->y - dist * cos2;
            z = params->z + dist * sin2 * sin1;
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
            }
            packet.frameStep = math_random() % (params->frameStepRange + 1) + params->frameStep;
            size = math_random() % (params->sizeRange + 1) + params->size;
            life = packet.life = gEffectTextures[39].frameCount * (256 / packet.frameStep);
            data.fadeInStart = life;
            data.fadeInEnd = life - (life >> 2);
            data.fadeInStep = 255 / (data.fadeInStart - data.fadeInEnd);
            data.growStart = life;
            data.growEnd = life >> 1;
            data.growStep = size / (data.growStart - data.growEnd);
            data.fadeOutStart = life >> 2;
            data.fadeOutStep = 255 / data.fadeOutStart;
            data.shrinkStart = life >> 1;
            data.shrinkStep = size / data.shrinkStart;
            packet.x = x;
            packet.y = y;
            packet.z = z;
            packet.subX = (s32)(x * 256.0f) & 0xFF;
            packet.subY = (s32)(y * 256.0f) & 0xFF;
            packet.subZ = (s32)(z * 256.0f) & 0xFF;
            particle = effect_spawnBillboard(&packet, 0, sizeof(data), -1, emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x11 (particle_updateHoverEmitter's particles): fade in and grow, then fade out and shrink. */
void particle_updateHoverParticle(BillboardParticle *particle) {
    s16 life;
    PuffParticleData *data;

    data = (PuffParticleData *)(particle + 1);
    life = particle->tmpl.life;
    if (data->fadeInEnd < life) {
        particle->tmpl.primA = (data->fadeInStart - life) * data->fadeInStep;
    }
    if (data->growEnd < life) {
        particle->tmpl.width = particle->tmpl.height = (data->growStart - life) * data->growStep;
    }
    if (life < data->fadeOutStart) {
        particle->tmpl.primA = life * data->fadeOutStep;
    }
    if (life < data->shrinkStart) {
        particle->tmpl.width = particle->tmpl.height = life * data->shrinkStep;
    }
}

/* Data copied to 0x28 of the effect created by particle_startSwellEmitter: EmitterParams with pos as the spawner's
 * Words3 (a word copy). */
typedef struct {
    /* 0x00 */ Words3 pos;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 spawnAccum;
    /* 0x14 */ f32 spawnRate;
    /* 0x18 */ s16 frameStep;
    /* 0x1A */ s16 frameStepRange;
    /* 0x1C */ s16 size;
    /* 0x1E */ s16 sizeRange;
    /* 0x20 */ s16 growRate;
    /* 0x22 */ s16 growRateRange;
} SwellEmitterParams;

/* Creates a type 4 effect_newTimedEffectNoDraw effect from the spawner's settings. */
void particle_startSwellEmitter(EffectSpawner *spawner) {
    s32 pad; // matching: unused stack slot
    SwellEmitterParams data;
    TimedEffect *effect;

    data.pos = spawner->pos;
    data.radius = spawner->radius;
    data.spawnAccum = 0.0f;
    data.spawnRate = spawner->spawnRate;
    data.frameStep = spawner->frameStep;
    data.frameStepRange = spawner->frameStepRange;
    data.size = spawner->size;
    data.sizeRange = spawner->sizeRange;
    data.growRate = spawner->growRate;
    data.growRateRange = spawner->growRateRange;

    effect = effect_newTimedEffectNoDraw(spawner->lifetime, -1, 4, 1, 0, 0x24, spawner->base.header.owner, spawner->base.header.kind);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/*
 * Extra data of the particles spawned by particle_updateSwellEmitter, read by its callbacks particle_updateSwellParticle and
 * particle_updateSwellTint. The *Start/*End fields are values of BillboardParticle.life, which counts down.
 */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 tintStart;    /* from here the env colour follows life (callback 0x13) */
    /* 0x0E */ s16 tintStep;
    /* 0x10 */ s16 fadeOutStart;
    /* 0x12 */ s16 fadeOutStep;
    /* 0x14 */ s16 swellStart;   /* from here the size grows by swellRate per frame */
    /* 0x16 */ s16 swellRate;
    /* 0x18 */ s16 accelStart;   /* once life passes it, accelY = accel */
    /* 0x1A */ s16 accel;
} SmokeParticleData;

/* Spawns particles at random points in a sphere around the emitter; they grow, tint and accelerate. */
void particle_updateSwellEmitter(SphereEmitter *emitter) {
    EmitterParams *params;
    BillboardTemplate packet;
    SmokeParticleData data;
    u8 angle1;
    u8 angle2;
    f32 sin1;
    f32 cos1;
    f32 sin2;
    f32 cos2;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    s16 size;
    BillboardParticle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * gFrameDelta;
    params = &emitter->params;
    if (emitter->params.spawnAccum > 1.0f) {
        packet.primR = 5;
        packet.primG = 0;
        packet.primB = 0;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x12;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.anim = &gEffectTextures[39];
        packet.velX = 0;
        packet.velY = 0;
        packet.velZ = 0;
        packet.accelY = 0;
        packet.fadeStep = 0;
        packet.primA = 0;
        packet.height = 2000;
        packet.width = 2000;
        do {
            angle1 = math_random() & 255;
            angle2 = math_random() & 255;
            cos1 = math_cosBinAngleMasked(angle1 - 64);
            sin1 = math_cosBinAngleMasked(angle1);
            cos2 = math_cosBinAngleMasked(angle2 - 64);
            sin2 = math_cosBinAngleMasked(angle2);
            dist = math_randomFloat() * params->radius;
            x = params->x + dist * sin2 * cos1;
            y = params->y - dist * cos2;
            z = params->z + dist * sin2 * sin1;
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (math_random() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
            }
            packet.frameStep = math_random() % (params->frameStepRange + 1) + params->frameStep;
            size = math_random() % (params->sizeRange + 1) + params->size;
            packet.life = gEffectTextures[39].frameCount * (256 / packet.frameStep);
            data.fadeInStart = packet.life;
            data.growStart = packet.life;
            data.fadeInEnd = packet.life - (packet.life >> 2);
            data.fadeInStep = 255 / (packet.life - data.fadeInEnd);
            data.growEnd = packet.life >> 1;
            // matching: the target recomputes life >> 1 here instead of reading growEnd
            data.growStep = size / (packet.life - (s16)(packet.life >> 1));
            data.tintStart = packet.life >> 1;
            data.tintStep = 255 / (packet.life >> 1);
            data.fadeOutStart = packet.life >> 2;
            data.fadeOutStep = 255 / (packet.life >> 2);
            data.swellStart = packet.life >> 1;
            data.swellRate = math_random() % (params->growRateRange + 1) + params->growRate;
            data.accelStart = packet.life >> 1;
            data.accel = math_random() % 81U + 80;
            packet.x = x;
            packet.y = y;
            packet.z = z;
            packet.subX = (s32)(x * 256.0f) & 0xFF;
            packet.subY = (s32)(y * 256.0f) & 0xFF;
            packet.subZ = (s32)(z * 256.0f) & 0xFF;
            particle = effect_spawnBillboard(&packet, 0, sizeof(data), -1, emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x12 (particle_updateSwellEmitter's particles): fade in, grow, fade out, swell; switches to 0x13 to tint. */
void particle_updateSwellParticle(BillboardParticle *particle) {
    SmokeParticleData *data = (SmokeParticleData *)(particle + 1);

    if (particle->tmpl.life > data->fadeInEnd) {
        particle->tmpl.primA = (data->fadeInStart - particle->tmpl.life) * data->fadeInStep;
    }
    if (particle->tmpl.life > data->growEnd) {
        particle->tmpl.width = particle->tmpl.height = (data->growStart - particle->tmpl.life) * data->growStep;
    }
    if (particle->tmpl.life < data->fadeOutStart) {
        particle->tmpl.primA = particle->tmpl.life * data->fadeOutStep;
    }
    if (particle->tmpl.life < data->swellStart) {
        s16 growth = data->swellRate * gFrameTicks;

        particle->tmpl.width += growth;
        particle->tmpl.height += growth;
    }
    if (particle->tmpl.life < data->tintStart) {
        particle->tmpl.callback = 0x13;
        particle->tmpl.flags |= BILLBOARD_ENV_COLOR | BILLBOARD_CUSTOM_MODE;
        particle->tmpl.envR = particle->tmpl.envG = particle->tmpl.envB = particle->tmpl.life * data->tintStep;
        particle->tmpl.unk4 = 0x520003;
    }
    if (particle->tmpl.life < data->accelStart) {
        particle->tmpl.accelY = data->accel;
        data->accelStart = -9999;
    }
}

/* Callback 0x13: particle_updateSwellParticle after the switch, with the env colour following life. */
void particle_updateSwellTint(BillboardParticle *particle) {
    SmokeParticleData *data = (SmokeParticleData *)(particle + 1);

    if (particle->tmpl.life > data->fadeInEnd) {
        particle->tmpl.primA = (data->fadeInStart - particle->tmpl.life) * data->fadeInStep;
    }
    if (particle->tmpl.life > data->growEnd) {
        particle->tmpl.width = particle->tmpl.height = (data->growStart - particle->tmpl.life) * data->growStep;
    }
    if (particle->tmpl.life < data->fadeOutStart) {
        particle->tmpl.primA = particle->tmpl.life * data->fadeOutStep;
    }
    if (particle->tmpl.life < data->swellStart) {
        s16 growth = data->swellRate * gFrameTicks;

        particle->tmpl.width += growth;
        particle->tmpl.height += growth;
    }
    if (particle->tmpl.life < data->accelStart) {
        particle->tmpl.accelY = data->accel;
        data->accelStart = -9999;
    }
    particle->tmpl.envR = particle->tmpl.envG = particle->tmpl.envB = particle->tmpl.life * data->tintStep;
}

/* particle_spawnDragFadeSprite's per-particle data, before it is copied into the particle. */
typedef struct {
    /* 0x0 */ f32 drag;
    /* 0x4 */ FadeEvent fade;
} ParticleData151A26EC;

/* Fade event handlers (FadeEvent.eventFunc), called with the particle and its event time; declared
   without a prototype as the handlers take only the particle. */
static void (*gDragFadeEventFuncs[])() = { particle_spawnTrailBurstAtSprite, particle_spawnHeavyTrailBurstAtSprite };

/* Spawns one particle_newOwnedSpriteParticle particle (texture 39, update func 0x11) at pos/offset with velocity vel. */
s32 particle_spawnDragFadeSprite(Vec3f *pos, Vec3f *offset, Vec3f *vel, f32 drag, f32 gravity, f32 size,
                  s16 timer, u8 alpha, s16 fadeStart, s16 fadeOutStart, s16 eventTime, s8 eventFunc,
                  u8 r, u8 g, u8 b, u8 owner, s32 list) {
    ParticleData151A26EC data;
    SpriteParticle *particle;
    ParticleDesc desc;

    if (fadeStart <= 0) {
        fadeStart = 1;
    }
    if (fadeOutStart <= 0) {
        fadeOutStart = 1;
    }
    data.drag = drag;
    data.fade.fadeOutStart = fadeOutStart;
    data.fade.fadeOutStep = 255 / fadeOutStart;
    desc.fadeStart = fadeStart;
    desc.fadeRate = 255 / fadeStart;
    data.fade.eventTime = eventTime;
    data.fade.eventFunc = eventFunc;
    desc.texture = 39;
    desc.renderMode = 0x5203;
    desc.setGeometryMode = 0x200005;
    desc.clearGeometryMode = 0;
    desc.timer = timer;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = r;
    desc.g = g;
    desc.b = b;
    desc.a = 255;
    desc.r2 = 255;
    desc.g2 = 255;
    desc.b2 = 255;
    desc.alpha = alpha;
    desc.brightness = 255;
    desc.height = size;
    desc.width = size;
    desc.pos = *pos;
    desc.offset = *offset;
    desc.vel = *vel;
    desc.scaleStart = 1;
    desc.gravity = gravity;
    desc.scaleRate = 1.0f;
    // matching: this grouping of the constant flags sets IDO's or-chain order
    desc.flags = (math_random() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (math_random() & 1 ? 0x80 : 0)) | 0x200 |
                 0x1000 | 0x4000 | 0x8000 | 0x800000;
    desc.primColorMode = 7;
    desc.envColorMode = 2;
    desc.updateFunc = 0x11;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 2;
    particle = particle_newOwnedSpriteParticle(&desc, 1, 0x10, owner, list);
    if (particle != NULL) {
        // matching: the target copies 8 bytes from data.drag, overlapping the second copy
        memcpy(&particle->data.dragFade.drag, &data.drag, 8);
        memcpy(&particle->data.dragFade.fade, &data.fade, sizeof(data.fade));
    }
    return 0;
}

/* Update func 0x11 (particle_spawnDragFadeSprite's particles): dim out, run the one-shot event, then apply drag. */
s32 particle_updateDragFade(SpriteParticle *particle, Vec3f *prevPos) {
    FadeEvent *fade;

    fade = &particle->data.dragFade.fade;
    if (particle->head.desc.timer < fade->fadeOutStart) {
        particle->head.desc.brightness = particle->head.desc.timer * fade->fadeOutStep;
    }
    if (gIsMultiplayer == 0 && fade->eventTime != -1 && fade->eventFunc != -1 &&
        fade->eventTime >= particle->head.desc.timer) {
        gDragFadeEventFuncs[fade->eventFunc](particle, fade->eventTime);
        fade->eventTime = -1;
    }
    return particle_updateDrag(particle, prevPos);
}

/* Bursts `count` particles from pos moving at yaw/pitch with a speed that falls off over the burst. */
void particle_burstAtYawPitch(Vec3f *pos, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count,
                   f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange,
                   s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 owner,
                   s32 list) {
    particle_spawnDragFadeBurst(pos, NULL, yaw, pitch, speed, sizeBase, sizeSlope, count, spread, drag, gravityBase,
                  gravityRange, delayBase, delayRange, alphaBase, alphaRange, fadeStart, fadeOutStart, eventFunc,
                  owner, list, 1);
}

/* Bursts `count` particles from pos moving along dir, scaled down over the burst. */
void particle_burstAlongDir(Vec3f *pos, f32 *dir, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread, f32 drag,
                   f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase,
                   s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 owner, s32 list) {
    particle_spawnDragFadeBurst(pos, dir, 0, 0, 0.0f, sizeBase, sizeSlope, count, spread, drag, gravityBase, gravityRange,
                  delayBase, delayRange, alphaBase, alphaRange, fadeStart, fadeOutStart, eventFunc, owner, list,
                  0);
}

/* Velocity of a burst particle at fraction t of the burst. */
typedef void (*BurstVelocityFunc)(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out);

/* BurstVelocityFunc: dir scaled by 1 - t. */
void particle_getBurstVelAlongDir(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out) {
    f32 scale = 1.0f - t;

    out->x = dir[0] * scale;
    out->y = dir[1] * scale;
    out->z = dir[2] * scale;
}

/* BurstVelocityFunc: the yaw/pitch direction at speed * (1 - t). */
void particle_getBurstVelAtYawPitch(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out) {
    math_dirFromBinAngles(yaw, pitch, (1.0f - t) * speed, out);
}

/* BillboardParticle colours picked at random by particle_spawnDragFadeBurst. */
typedef struct {
    u8 rgb[4][3];
} BurstColors;

static BurstColors gDragFadeBurstColors = { { { 0, 0, 0 }, { 89, 0, 0 }, { 255, 255, 255 }, { 128, 128, 128 } } };

/* Spawns `count` particle_spawnDragFadeSprite particles around pos; size and speed change linearly over the burst. */
void particle_spawnDragFadeBurst(Vec3f *pos, f32 *dir, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope,
                   s32 count, f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase,
                   s16 delayRange, s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart,
                   s8 eventFunc, u8 owner, s32 list, u8 useAngles) {
    s16 minFade;
    s16 delay;
    f32 t;
    f32 step;
    BurstVelocityFunc velocityFunc;
    Vec3f offset;
    Vec3f vel;
    s32 color;
    BurstColors colors;

    if (!useAngles) {
        velocityFunc = particle_getBurstVelAlongDir;
    } else {
        velocityFunc = particle_getBurstVelAtYawPitch;
    }
    if (fadeStart < fadeOutStart) {
        minFade = fadeStart;
    } else {
        minFade = fadeOutStart;
    }
    t = 0.0f;
    step = 1.0f / count;
    while (count != 0) {
        f32 rand;
        s32 a;  // matching: a and b are reused for the alpha roll and the event time
        s32 b;

        delay = math_random() % (delayRange + 1) + delayBase;
        a = math_random();
        b = math_random();
        math_dirFromBinAngles(a & 0xFF, b & 0xFF, math_randomFloat() * spread, &offset);
        velocityFunc(dir, yaw, pitch, speed, t, &vel);
        colors = gDragFadeBurstColors;
        color = math_random() & 3;
        rand = math_randomFloat();
        a = math_random();
        if (math_randomFloat() < 0.0802000016f) {
            b = 25;
        } else {
            b = -1;
        }
        particle_spawnDragFadeSprite(pos, &offset, &vel, drag, rand * gravityRange + gravityBase,
                      sizeSlope * t + sizeBase, delay + minFade, a % (u32)(alphaRange + 1) + alphaBase,
                      fadeStart, fadeOutStart, b, eventFunc, colors.rgb[color][0], colors.rgb[color][1],
                      colors.rgb[color][2], owner, list);
        t += step;
        count--;
    }
}

/* gDragFadeEventFuncs[0]: a particle_spawnTrailBurst burst at the particle's position, sized by the particle. */
void particle_spawnTrailBurstAtSprite(SpriteParticle *particle) {
    TrailBurst config;

    config.countBase = 8;
    config.countRange = 6;
    config.pos = particle->head.desc.pos;
    if (particle->head.desc.flags & 0x1000) {
        config.pos.x += particle->head.desc.offset.x;
        config.pos.y += particle->head.desc.offset.y;
        config.pos.z += particle->head.desc.offset.z;
    }
    config.sizeBase = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.01999999955f;
    config.sizeRange = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.03529999778f;
    config.gravityBase = -0.2680000067f;
    config.gravityRange = 0.0f;
    config.speedBase = 8.0f;
    config.speedRange = 10.0f;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.angles.pitchBase = -64;
    config.angles.pitchRange = 86;
    config.capacityBase = 3;
    config.capacityRange = 1;
    config.lifeBase = 17;
    config.lifeRange = 18;
    config.headerFlags = 1;
    config.flags = 4;
    config.primMode = 2;
    config.envMode = 3;
    config.colorBase.red = 255;
    config.colorBase.green = 200;
    config.colorBase.blue = 200;
    config.colorBase.alpha = 255;
    config.colorRange.red = 0;
    config.colorRange.green = 55;
    config.colorRange.blue = 55;
    config.colorRange.alpha = 0;
    config.color2Base.red = 255;
    config.color2Base.green = 255;
    config.color2Base.blue = 255;
    config.color2Base.alpha = 255;
    config.color2Range.red = 0;
    config.color2Range.green = 0;
    config.color2Range.blue = 0;
    config.color2Range.alpha = 0;
    config.colorLevelBase = 255;
    config.colorLevelRange = 0;
    config.renderMode = 1;
    config.combiner = 0x24;
    config.setGeometryMode = 0x200005;
    config.clearGeometryMode = 0x60600;
    config.fadeStart = 7;
    config.fadeRate = 0x24;
    config.shrinkStart = 1;
    config.shrinkPaused = 0;
    config.shrinkFactor = 1.0f;
    config.moveFn = -1;
    config.shapeFn = 0;
    config.unk72 = -1;
    config.unk73 = -1;

    particle_spawnTrailBurst(&config, particle->head.header.owner, particle->head.header.kind);
}

/* gDragFadeEventFuncs[1]: a particle_spawnTrailBurst burst at the particle's position, sized by the particle. */
void particle_spawnHeavyTrailBurstAtSprite(SpriteParticle *particle) {
    TrailBurst config;

    config.countBase = 10;
    config.countRange = 15;
    config.pos = particle->head.desc.pos;
    if (particle->head.desc.flags & 0x1000) {
        config.pos.x += particle->head.desc.offset.x;
        config.pos.y += particle->head.desc.offset.y;
        config.pos.z += particle->head.desc.offset.z;
    }
    config.sizeBase = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.02119999938f;
    config.sizeRange = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.05319999903f;
    config.gravityBase = -1.031000018f;
    config.gravityRange = 0.0f;
    config.speedBase = 8.0f;
    config.speedRange = 8.0f;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.angles.pitchBase = -64;
    config.angles.pitchRange = 86;
    config.capacityBase = 3;
    config.capacityRange = 2;
    config.lifeBase = 15;
    config.lifeRange = 30;
    config.headerFlags = 1;
    config.flags = 4;
    config.primMode = 2;
    config.envMode = 3;
    config.colorBase.red = 255;
    config.colorBase.green = 200;
    config.colorBase.blue = 200;
    config.colorBase.alpha = 255;
    config.colorRange.red = 0;
    config.colorRange.green = 55;
    config.colorRange.blue = 55;
    config.colorRange.alpha = 0;
    config.color2Base.red = 255;
    config.color2Base.green = 255;
    config.color2Base.blue = 255;
    config.color2Base.alpha = 255;
    config.color2Range.red = 0;
    config.color2Range.green = 0;
    config.color2Range.blue = 0;
    config.color2Range.alpha = 0;
    config.colorLevelBase = 255;
    config.colorLevelRange = 0;
    config.renderMode = 1;
    config.combiner = 0x24;
    config.setGeometryMode = 0x200005;
    config.clearGeometryMode = 0x60600;
    config.fadeStart = 15;
    config.fadeRate = 17;
    config.shrinkStart = 1;
    config.shrinkPaused = 0;
    config.shrinkFactor = 1.0f;
    config.moveFn = -1;
    config.shapeFn = 0;
    config.unk72 = -1;
    config.unk73 = -1;

    particle_spawnTrailBurst(&config, particle->head.header.owner, particle->head.header.kind);
}
