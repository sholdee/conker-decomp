#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/* One entry of the gBoneGroundBurstDefs table: where on the owner's skeleton particle_spawnBoneGroundBurst spawns, and its random ranges. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 bone;
    /* 0x04 */ Vec3f offset; /* bone-local spawn point */
    /* 0x10 */ u8 alphaMin;
    /* 0x11 */ u8 alphaRange;
    /* 0x12 */ s16 growDelayMin; /* subtracted from the lifetime */
    /* 0x14 */ s16 growDelayRange;
    /* 0x18 */ f32 sizeMin;
    /* 0x1C */ f32 sizeRange;
    /* 0x20 */ f32 growRateMin; /* per unit of size */
    /* 0x24 */ f32 growRateRange;
    /* 0x28 */ s16 yawOffset;
} GroundBurstDef;

/* Per-corner colour passed to particle_spawnAxisQuadParticleNoModel. */
typedef struct {
    /* 0x0 */ s16 r;
    /* 0x2 */ s16 g;
    /* 0x4 */ s16 b;
    /* 0x6 */ s16 a;
    /* 0x8 */ s16 unk8;
} CornerColor;

/* This file's .rodata front-end data. */
static const GroundBurstDef gBoneGroundBurstDefs[] = {
    { 0x4E, 27, { -68.0f, 0.0f, -3.0f }, 190, 65, 12, 6, 0.18f, 0.040000003f, 0.1f, 0.060000002f, -114 },
    { 0x4E, 27, { -68.0f, 0.0f, -3.0f }, 190, 65, 17, 6, 0.18f, 0.040000003f, 0.09f, 0.05f, -114 },
    { 0x4D, 27, { 0.0f, 0.0f, 40.0f }, 150, 50, 15, 8, 0.2f, 0.1f, 0.080000006f, 0.040000003f, 0 },
};

/* Spawns a burst at a bone-relative point of owner, dropped onto the ground below: a growing
   ground-coloured particle, two particle_emitOnRandomCircle effects, a effect_newRingEffect effect and a particle_startPlayerRumbles effect. */
void particle_spawnBoneGroundBurst(Actor *owner, u8 index, s32 fxList, u8 fxOwner) {
    Vec3f pos;
    const GroundBurstDef *def;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    CollisionHit ground;

    if (owner == NULL || owner->ob_matrix_ptr == NULL) {
        return;
    }
    collision_initHitFromObject(&ground, owner);
    def = &gBoneGroundBurstDefs[index];
    math_transformOffset(&def->offset, &pos, &owner->ob_matrix_ptr[def->bone]);
    {
        Vec3f ray;

        ray.x = pos.x;
        ray.y = pos.y + 200.0f;
        ray.z = pos.z;
        if (collision_probeFloorSurfABSet0Cached(&ray, 0, pos.y - 2000.0f, &ground) == 0) {
            return;
        }
    }
    pos.y = ground.height;
lbl: // matching: unused label ends the basic block so the pos.y store stays out of the jal delay slot
    particle_getDepthTint(pos.z, &fr, &fg, &fb);
    r = fr;
    g = fg;
    b = fb;
    {
        GrowParticleExtra extra;
        ParticleDef spawn;
        s16 rnd;
        CornerColor colors[4];
        GrowParticle *particle;

        colors[0].r = r;
        colors[0].g = g;
        colors[0].b = b;
        colors[0].a = 0xFF;
        colors[0].unk8 = 0;
        colors[1].r = r;
        colors[1].g = g;
        colors[1].b = b;
        colors[1].a = 0xFF;
        colors[1].unk8 = 0;
        colors[2].r = r;
        colors[2].g = g;
        colors[2].b = b;
        colors[2].a = 0xFF;
        colors[2].unk8 = 0;
        colors[3].r = r;
        colors[3].g = g;
        colors[3].b = b;
        colors[3].a = 0xFF;
        colors[3].unk8 = 0;
        spawn.texture = def->type;
        spawn.renderModes = 0x2601;
        spawn.lifetime = (math_random() % 51U) + 132;
        spawn.frame = 0;
        spawn.frameSpeed = 0;
        spawn.rgba[0] = 0xFF;
        spawn.rgba[1] = 0xFF;
        spawn.rgba[2] = 0xFF;
        spawn.rgba[3] = 0xFF;
        spawn.width = spawn.height = (math_randomFloat() * def->sizeRange) + def->sizeMin;
        spawn.pos.x = pos.x;
        spawn.pos.y = pos.y + 10.0f;
        spawn.pos.z = pos.z;
        spawn.rot.x = 0.0f;
        spawn.rot.y = 0.0f;
        spawn.rot.z = 0.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x1C0001;
        spawn.alpha = (math_random() % (def->alphaRange + 1)) + def->alphaMin;
        spawn.colorLevel = 0xFF;
        spawn.primMode = 0;
        spawn.envMode = 6;
        extra.growDelay = (spawn.lifetime - (math_random() % (def->growDelayRange + 1))) - def->growDelayMin;
        extra.growRate = ((math_randomFloat() * def->growRateRange) + def->growRateMin) * spawn.width;
        particle = particle_spawnAxisQuadParticleNoModel(&spawn, colors, 0, 0x18, 0, 0, (owner->langle >> 8) + def->yawOffset,
                                 500.0f, 500.0f, NULL, &ground.tri, 1, 8, fxOwner, fxList);
        if (particle != NULL) {
            memcpy(&particle->extra, &extra, sizeof(extra));
        }
        {
            RadialEffectDef fx;
            f32 d;

            rnd = (math_random() % 1U) + 41; // matching: a degenerate random range; the call and divu are in the ROM
            particle_emitOnRandomCircle(pos.x, pos.y, pos.z, (math_randomFloat() * 50.0f) + 80.0f,
                          0, 0xFF, rnd, 0x11, 0, 0.0f, NULL, fxOwner);
            d = 13.90000057f;
lbl2: // matching: unused label ends the basic block so &fx isn't hoisted above the 13.9f load
            {
                s16 n1; // matching: a variable, not a literal, so IDO emits the runtime int-to-float convert

                fx.pos = pos;
                fx.speed = 70.59999847f;
                fx.flags = 1;
                fx.checkCallback = -1;
                fx.count = 12;
                fx.remaining = 40;
                fx.growUntil = 30;
                n1 = 10;
                fx.growRate = d / n1;
                fx.shrinkFrom = 25;
                fx.shrinkRate = d / fx.shrinkFrom;
                fx.emitCallback = -1;
                fx.updateCallback = -1;
                fx.emitRate = 0.0f;
                effect_newRingEffect(&fx, 0, 0xFF, 1);
            }
        }
        {
            Rgb8 rgb;

            rnd = (math_random() % 14U) + 15;
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            particle_emitOnRandomCircle(pos.x, pos.y, pos.z, (math_randomFloat() * 70.0f) + 70.0f,
                          0, 0xFF, rnd, 0x12, 0, 0.0f, &rgb, fxOwner);
        }
        particle_startPlayerRumbles(&pos, 500.0f, 5000.0f, 0.0002f, 12, 15, 0xFF, 0);
    }
}

/* Particle update: fades in over its first 32 ticks, then grows once past its delay. */
s32 particle_fadeInGrow(GrowParticle *p) {
    GrowParticleExtra *extra;

    if (p->flags & 1) {
        extra = &p->extra; // matching: the pointer is computed once and shared by both branches
        if (p->age < 32) {
            s32 fadeAlpha = p->age * 8;

            if (fadeAlpha < p->alpha) {
                p->alpha = fadeAlpha;
            }
        }
        if (extra->growDelay < p->age) {
            f32 growth = extra->growRate * gFrameDelta;

            p->size0 += growth;
            p->size1 += growth;
        }
    }
    return 1;
}

/* Spawns a particle at (x, y, z) flung along yaw with a random pitch; it shrinks by 0.901 per tick (particle_dampYawSpriteXZ). */
s32 particle_spawnYawDampedSprite(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius, s32 arg11,
                  s32 arg12, void *data, u8 owner) {
    TimedEffect *particle;
    ParticleDesc spawn;
    f32 size;
    f32 decay;
    f32 speed;
    u32 random;

    spawn.texture = 0x29;
    spawn.renderMode = 0xE03;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0;
    spawn.timer = (math_random() & 0xF) + 20;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r2 = 0xB0;
    spawn.g2 = 0xA0;
    spawn.b2 = 0x2A;
    spawn.r = 0x40;
    spawn.g = 0xB;
    spawn.b = 0x6A;
    spawn.a = 0xFF;

    spawn.alpha = (math_random() % 156U) + 100;
    spawn.brightness = 0xFF;
    spawn.primColorMode = 3;
    spawn.envColorMode = 3;

    size = (math_randomFloat() * 401.0f) + 300.0f;
    spawn.width = size;
    spawn.height = size;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;

    random = math_random();
    speed = (math_randomFloat() * 200.0f + 350.0f) * 0.1000000015f;
    math_dirFromBinAngles(yaw, (random % 21U) - 25, speed, &spawn.vel);

    spawn.gravity = 0.0f;
    spawn.flags = 0xE05;

    if (math_random() & 1) {
        spawn.flags |= 0x40;
    }

    if (math_random() & 1) {
        spawn.flags |= 0x80;
    }

    spawn.updateFunc = 8;
    spawn.scaleRate = 1.039999962f;
    decay = 0.9010000229f;
    spawn.deathFunc = -1;
    spawn.fadeStart = 10;
    spawn.fadeRate = 25;
    spawn.scaleStart = 27;

    particle = particle_newOwnedSpriteParticle(&spawn, 1, 4, owner, 1);
    if (particle != NULL) {
        memcpy((u8 *)particle + 0xA8, &decay, sizeof(decay));
    }

    return 1;
}

/* Particle update: scales unk58 and unk60 by the decay factor once per elapsed tick. */
s32 particle_dampYawSpriteXZ(DecayParticle *p, Vec3f *prevPos) {
    f32 *decay;
    s32 i;

    decay = &p->decay; // matching: IDO keeps the factor's address in a register only via this pointer
    for (i = gFrameTicks; i != 0; i--) {
        p->head.desc.vel.x *= *decay;
        p->head.desc.vel.z *= *decay;
    }
    return 1;
}

/* Spawns a tumbling particle at (x, y, z), flung along yaw with a random upward pitch and spin. */
s32 particle_spawnYawTumbler(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius, s32 arg11,
                  s32 arg12, void *data, u8 owner) {
    s32 pad; // matching: unused stack slot
    ParticleSpawn spawn;
    f32 speed;
    u32 random;

    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.flags = 0x29E8;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.radius = 1.0f;
    spawn.model = 0x20;
    spawn.pos.z = z;
    spawn.elasticity = 0.200000003f;

    random = math_random();
    speed = (math_randomFloat() * 200.0f + 300.0f) * 0.1000000015f;
    math_dirFromBinAngles(yaw, (random % 44U) - 63, speed, &spawn.vel);

    spawn.rotVel.x = (math_randomFloat() * 260.0f + -130.0f) * 0.1000000015f;
    spawn.rotVel.z = (math_randomFloat() * 260.0f + -130.0f) * 0.1000000015f;
    spawn.lifetime = (math_random() % 51U) + 50;
    spawn.gravity = (math_randomFloat() * 1500.0f + -3500.0f) * 0.001000000047f;

    spawn.sizeXZ = spawn.sizeY = (math_randomFloat() * 500.0f + 100.0f) * 0.001000000047f;
    spawn.variant = 0;
    spawn.zone = 0;

    spawn.alpha = (math_random() % 101U) + 155;
    spawn.updateFn = 3;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.lightMode = 2;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 0x20;
    spawn.fadeRate = 7;

    particle_newModelParticle(&spawn, 3, 0xFF, 0, owner, 0);
    return 1;
}

/* Integrates a tumbling particle's position (with gravity) and rotation over one frame step. */
s32 particle_moveTumbler(TumbleParticle *p) {
    p->position[0] += p->velocity[0] * gFrameDelta;
    p->position[1] += p->velocity[1] * gFrameDelta + p->gravity * gFrameDelta * gFrameDelta * 0.5f;
    p->position[2] += p->velocity[2] * gFrameDelta;
    p->velocity[1] += p->gravity * gFrameDelta;
    p->rotation[0] += p->rotationVel[0] * gFrameDelta;
    p->rotation[1] += p->rotationVel[1] * gFrameDelta;
    p->rotation[2] += p->rotationVel[2] * gFrameDelta;
    return 1;
}
