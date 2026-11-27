#include <ultra64.h>
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/view.h"
#include "libc/string.h"



/* Effect object header from effect_newTimedEffect; effect-specific data starts at 0x28. */
struct PointEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Vec3f pos;
};

/* The explosion's 15 spawned effects: offsets from pos, rotation weights and speeds, and the kind (0-2,
 * an index into gExplosionRiseSpeeds / gExplosionRiseSpeedRanges) of each. */
static const Vec3f gExplosionOffsets[15] = {
    { -29.022f, -20.655f, -14.009f },
    { -51.612f, -23.07f, -212.214f },
    { 93.48f, -24.76f, -141.967f },
    { 136.363f, -20.358f, 54.784f },
    { -1.247f, -25.218f, 139.536f },
    { -183.712f, -27.429f, 80.539f },
    { -200.619f, -25.171f, -83.532f },
    { 71.607f, -19.136f, -323.491f },
    { 220.55f, -22.568f, -166.693f },
    { 240.525f, -22.392f, 124.099f },
    { 90.442f, -32.245f, 300.165f },
    { -177.672f, -23.695f, 237.533f },
    { -339.578f, -20.367f, 32.438f },
    { -254.773f, -20.701f, -179.78f },
    { -135.739f, -23.215f, -316.984f },
};
static const f32 gExplosionSpinWeightsZ[15] = {
    -0.900571f, -0.236319f, 0.549948f, 0.927915f, -0.008936f,
    -0.915855f, -0.923174f, 0.216125f, 0.797771f, 0.888685f,
    0.288496f, -0.598969f, -0.995468f, -0.817058f, -0.393647f,
};
static const f32 gExplosionSpinWeightsX[15] = {
    -0.434708f, -0.971676f, -0.835199f, 0.372791f, 0.99996f,
    0.401509f, -0.384383f, -0.976366f, -0.60296f, 0.458518f,
    0.957481f, 0.800772f, 0.095092f, -0.576555f, -0.919262f,
};
static const f32 gExplosionSpinSpeeds[15] = {
    32.226208f, 218.40005f, 169.97981f, 146.95631f, 139.54156f,
    200.59071f, 217.31447f, 331.32156f, 276.4577f, 270.65262f,
    313.49448f, 296.62985f, 341.1238f, 311.81744f, 344.8245f,
};
static const f32 gExplosionPieceKinds[15] = {
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 2.0f, 2.0f, 2.0f,
    2.0f, 2.0f, 2.0f, 2.0f, 2.0f,
};

/* Base and random range of the upward speed of each kind. */
static f32 gExplosionRiseSpeeds[3] = { 28.0f, 23.0f, 15.0f };
static f32 gExplosionRiseSpeedRanges[3] = { 4.0f, 5.0f, 4.0f };

/*
 * Explosion at pos: 15 spawned effects (offsets from gExplosionOffsets), followed by effect_spawnQuadEffect,
 * particle_startPlayerRumbles/5334/5514/3FF4 and particle_spawnGrowFade effects, a burst of 3-6 debris aimed
 * around the camera yaw, and a short-lived PointEffect at pos.
 */
void effect_spawnExplosion(Vec3f *pos, u8 owner, s32 kind) {
    s32 yaw;

    // camera yaw in degrees -> 0..255 binary angle
    yaw = camera_global_Cameras[gCurrentView].TrueCameraAngle * 0.7111111283f;
    {
        s32 i;
        ParticleSpawn spawn;
        u8 idx;

        spawn.radius = 15.0f;
        spawn.sizeY = 1.0f;
        spawn.sizeXZ = 1.0f;
        spawn.elasticity = 0.200000003f;
        spawn.rot = gZeroVec3f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.rotVel.y = 0.0f;
        spawn.flags = 0x29E9;
        spawn.variant = 0;
        spawn.zone = 0;
        spawn.alpha = 255;
        spawn.updateFn = 13;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 10;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.lightMode = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 35;
        spawn.fadeRate = 7;
        spawn.sound = 0;
        spawn.vel.x = 0.0f;
        spawn.vel.z = 0.0f;
        for (i = 0; i < 15; i++) {
            idx = gExplosionPieceKinds[i];
            spawn.pos.x = gExplosionOffsets[i].x + pos->x;
            spawn.pos.y = gExplosionOffsets[i].y + pos->y;
            spawn.pos.z = gExplosionOffsets[i].z + pos->z;
            spawn.vel.y = gExplosionRiseSpeeds[idx] + math_randomFloat() * gExplosionRiseSpeedRanges[idx];
            spawn.rotVel.x = gExplosionSpinSpeeds[i] * gExplosionSpinWeightsX[i] * 0.01900000125f;
            spawn.rotVel.z = gExplosionSpinSpeeds[i] * gExplosionSpinWeightsZ[i] * 0.01900000125f;
            spawn.gravity = math_randomFloat() * 0.2080000043f + -1.032000065f;
            spawn.lifetime = math_random() % 111U + 130;
            if (math_random() & 1) {
                spawn.model = 0xC4;
            } else {
                spawn.model = 0xC5;
            }
            // matching: *(u8 *)&owner re-reads owner from its stack slot at every use (a K&R u8 gets CSE'd)
            particle_newModelParticleWithFloor(&spawn, 3, 0xFF, 0, 0, *(u8 *)&owner, kind);
        }
    }
    effect_spawnQuadEffect(-1, pos->x, pos->y, pos->z, 30, 50, 0, *(u8 *)&owner, kind);
    particle_startPlayerRumbles(pos, 506.0f, 1013.0f, 0.0009871668881f, 15, 20, *(u8 *)&owner, kind);
    particle_startPlayerCameraWobbles(pos, 506.0f, 1013.0f, 0.0009871668881f, 5, *(u8 *)&owner, kind);
    particle_spawnBlastSpray(pos, *(u8 *)&owner, kind);
    particle_spawnLightFlash(pos, *(u8 *)&owner, kind);
    particle_spawnGrowFade(pos, math_randomFloat() * 4.0f + 12.0f, 1.6409999f,
                  math_random() % 56U + 200, 0.0f, *(u8 *)&owner, kind);
    {
        DebrisEmitter debris;

        debris.count = (math_random() & 3) + 3;
        debris.lifetime = 190;
        debris.yawBase = yaw - 0x40;
        debris.yawRange = 0x80;
        debris.pitchBase = -0x3F;
        debris.pitchRange = 0x5A;
        debris.radius = 250.0f;
        debris.pos = *pos;
        debris.speedBase = 5.0f;
        debris.velYBase = 15.0f;
        debris.velYRange = 15.0f;
        debris.dir.x = 0.0f;
        debris.dir.y = 0.0f;
        debris.landPuffs = 1;
        debris.landPuffFlag = 1;
        debris.landPuffLifeBase = 100;
        debris.landPuffLifeRange = 50;
        debris.speedRange = 10.0f;
        debris.spinRange = 10.0f;
        debris.dir.z = 0.0f;
        debris.sizeBase = 0.25f;
        debris.sizeRange = 0.5f;
        debris.gravityBase = -0.6999999881f;
        debris.gravityRange = -0.6200000048f;
        debris.spinBase = -5.0f;
        blood_spawnDebris(&debris, 0, *(u8 *)&owner, kind);
    }
    {
        TimedEffect *effect;
        Vec3f effectPos;

        effectPos = *pos;
        effect = effect_newTimedEffect(15, 8, -1, -1, 1, 0, sizeof(effectPos), *(u8 *)&owner, kind);
        if (effect != NULL) {
            memcpy(&((PointEffect *)effect)->pos, &effectPos, sizeof(effectPos));
        }
    }
}

/*
 * Effect update: drops the effect's position onto the ground below it (if any), spawns a
 * point effect (particle_startRandomEmitter) there and scatters 3-5 beam_spawnAtGroundPoints spawns around it.
 */
void effect_dropPointEffect(PointEffect *effect) {
    Vec3f *pos;
    CollisionHit hit;
    u8 onGround;
    Vec3f groundPos;
    Vec3f probe;
    Vec3f center;

    pos = &effect->pos;
    onGround = 0;
    probe.x = effect->pos.x;
    probe.y = effect->pos.y + 500.0f;
    probe.z = effect->pos.z;
    if (collision_probeFloorSurfABSet0(&probe, 0, probe.y - 1000.0f, &hit) != 0) {
        groundPos.x = pos->x;
        groundPos.y = hit.height;
        groundPos.z = pos->z;
        onGround = 1;
    }
    particle_startRandomEmitter(onGround ? &groundPos : pos, math_random() % 62U + 120, effect->base.header.owner, effect->base.header.kind);
    pos = onGround ? &groundPos : &effect->pos;
    center = *pos;
    center.y += 500.0f;
    beam_spawnAtGroundPoints(&center, center.y - 1000.0f, 250.0f, &hit, math_random() % 3U + 3, 1, 0, effect->base.header.owner,
                  effect->base.header.kind);
}

/* Spawns a particle at pos, then 1-3 more scattered within 30 units of it. */
void effect_spawnScatteredParticles(Vec3f *pos, s32 arg1, s32 arg2, u8 owner, s32 kind) {
    ParticleEmitterDesc emitter;
    f32 scaleA;
    f32 scaleB;
    s32 count;

    emitter.pos = *pos;
    scaleA = math_randomFloat() * 0.5f + 1.0f;
    scaleB = math_randomFloat() * 0.5f + 1.0f;
    emitter.maxA = 22.6f * scaleA;
    emitter.minA = 13.786f * scaleA;
    emitter.maxC = 0x6231; emitter.minC = 0x1A4D; // matching: same line, -g3 line numbers order the two sh
    emitter.maxB = 57.969f * scaleB;
    emitter.minB = 42.714f * scaleB;
    emitter.altB = 118.763f * scaleB;
    emitter.life = math_random() % 151U + 150;
    emitter.color[0] = 0;
    emitter.color[1] = 0;
    emitter.color[2] = 0;
    emitter.color[3] = 255;
    emitter.flags = 1;
    emitter.alpha = math_random() % 156U + 100;
    emitter.colorLevel = 255;
    emitter.updateFn1 = 0;
    emitter.expireFn = 1;
    emitter.extraSize = 0;
    emitter.effect = 15;
    emitter.scale = 0.0f;
    emitter.maxHeight = 10000.0f;
    emitter.shrinkRate = 0.0450000018f;
    emitter.growRate = 0.699999988f;
    particle_spawnChannelParticle(&emitter, 10, 0, 0, owner, kind);
    for (count = math_random() % 3U + 1; count > 0; count--) {
        math_dirFromBinAngleXZ(math_random() & 0xFF, 30.0f, &emitter.pos.x, &emitter.pos.z);
        emitter.pos.x += pos->x;
        emitter.pos.z += pos->z;
        scaleA = math_randomFloat() * 0.5f + 0.75f;
        scaleB = math_randomFloat() * 0.5f + 0.75f;
        emitter.maxA = 22.6f * scaleA;
        emitter.minA = 13.786f * scaleA;
        emitter.maxB = 57.969f * scaleB;
        emitter.minB = 42.714f * scaleB;
        emitter.altB = 118.763f * scaleB;
        emitter.life = math_random() % 151U + 150;
        emitter.alpha = math_random() % 156U + 100;
        emitter.expireFn = 0;
        particle_spawnChannelParticle(&emitter, 10, 0, 0, owner, kind);
    }
}
