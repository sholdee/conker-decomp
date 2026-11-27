#include <ultra64.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/* This file's .rodata front-end data: the point of each side (2, 1) in its bone's space. */
static const Vec3f gDustSide2Offset = { 7.0f, 0.0f, 0.0f };
static const Vec3f gDustSide1Offset = { -7.0f, 0.0f, 0.0f };

/* This file's .bss. */
struct Queue *gEffectQueue; /* the effect FIFO queue (allocated by effect_allocQueue) */


/*
 * Transforms the fixed offset for `side` (1 or 2) by that side's bone matrix of the actor
 * (bone 40 or 47) into `pos`.
 * If `ground` is non-NULL, also probes the ground below that point into it.
 */
s32 particle_getDustSidePos(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side) {
    Vec3f probe;
    const Vec3f *offset;
    MtxF *bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        offset = &gDustSide1Offset;
    } else {
        offset = &gDustSide2Offset;
    }

    if (side == 1) {
        bone = &actor->ob_matrix_ptr[40];
    } else {
        bone = &actor->ob_matrix_ptr[47];
    }

    math_transformOffset(offset, pos, bone);
    if (ground == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 100.0f;
    probe.z = pos->z;
    collision_initHitFromObject(ground, actor);
    return collision_probeFloorSurfABSet0Cached(&probe, 0, pos->y - 500.0f, ground);
}

s32 particle_selectSideDust(s32 arg0, s32 arg1) {
    return 0xB;
}

/* One corner colour passed to particle_spawnAxisQuadParticleNoModel. */
typedef struct {
    s16 r;
    s16 g;
    s16 b;
    s16 a;
    s16 unk8;
} DustColor;

/* Spawns a dust puff (tinted by particle_getDepthTint) on the ground below the actor's bone on `side`. */
void particle_spawnSideDust(Actor *actor, s32 side, s32 arg2) {
    Vec3f pos;
    CollisionHit ground;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    s32 angleOffset;
    DustGrowth growth;
    ParticleDef desc;
    DustPuff *particle;

    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    if (particle_getDustSidePos(&pos, actor, &ground, side) == 0) {
        return;
    }
    camera_startEnvelope(&camera_global_Cameras[gLastPlayerIndex], 7, 1);
    particle_startPlayerRumbles(&pos, 1502.0f, 3000.0f, 1.0f / 3000.0f, 12, 15, 0xFF, 0);
    particle_getDepthTint(pos.z, &fr, &fg, &fb);
    r = fr;
    g = fg;
    b = fb;
    {
        DustColor colors[4];
        s16 roll;
        s16 roll2;
        Rgb8 rgb;

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
        if (side == 1) {
            desc.texture = 0x4C;
        } else {
            desc.texture = 0x4B;
        }
        desc.renderModes = 0x2603;
        desc.lifetime = 2;
        desc.frame = 0;
        desc.frameSpeed = 0;
        desc.rgba[0] = 0xFF;
        desc.rgba[1] = 0xFF;
        desc.rgba[2] = 0xFF;
        desc.rgba[3] = 0xFF;
        desc.width = desc.height = ((math_randomFloat() * 50.0f) + 325.0f) * 0.001f;
        desc.pos.x = pos.x;
        desc.pos.y = ground.height + 10.0f;
        desc.pos.z = pos.z;
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.flags = 0x401C0000;
        desc.alpha = (math_random() % 51U) + 160;
        desc.colorLevel = 0xFF;
        growth.growTime = (math_random() & 3) + 7;
        growth.growRate = (((math_randomFloat() * 30.0f) + 30.0f) * desc.width) * 0.001f;
        desc.primMode = 0;
        desc.envMode = 6;
        angleOffset = (side == 1) ? -8 : 8;
        particle = particle_spawnAxisQuadParticleNoModel(&desc, colors, 0, 0x17, 0, 0, (actor->langle >> 8) - angleOffset,
                                 500.0f, 500.0f, NULL, &ground.tri, 1, 8, 0xFF, 0);
        if (particle != NULL) {
            memcpy(&particle->growth, &growth, sizeof(growth));
            if (effect_pushQueue(gEffectQueue, particle, 2) == 0) {
                effect_destroyObject(&particle->header);
            }
        }
        roll = (math_random() & 0xF) + 10;
        particle_emitOnRandomCircle(pos.x, pos.y, pos.z, (math_randomFloat() * 59.0f) + 80.0f,
                      0, 0xFF, roll, 14, 0, 0.0f, NULL, 0xFF);
        roll2 = (math_random() & 0xF) + 5;
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;
        particle_emitOnRandomCircle(pos.x, pos.y, pos.z, (math_randomFloat() * 59.0f) + 70.0f,
                      0, 0xFF, roll2, 15, 0, 0.0f, &rgb, 0xFF);
    }
}

/* QuadSprite update: fades the sprite (def.alpha) out over the last 32 frames of its lifetime. */
s32 particle_fadeSideDustSprite(QuadSprite *sprite) {
    s32 fade;

    if (sprite->def.lifetime < 32) {
        fade = sprite->def.lifetime * 8;
        if (fade < sprite->def.alpha) {
            sprite->def.alpha = fade;
        }
    }
    return 1;
}

/* Spawns one particle_newModelParticle particle at (x, y, z), its velocity built by math_dirFromBinAngles from `angle`. */
s32 particle_spawnYawModelParticle(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    s32 pad; // matching: unused stack slot
    ParticleSpawn desc;
    f32 fraction;
    u32 random;

    desc.pos.x = x;
    desc.pos.y = y;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = 0x29E8;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    desc.rotVel.y = 0.0f;
    desc.radius = 1.0f;
    desc.model = 32;
    desc.pos.z = z;
    desc.elasticity = 0.2f;

    random = math_random();
    fraction = math_randomFloat();
    math_dirFromBinAngles(angle, (random % 36U) - 55, (fraction * 200.0f + 200.0f) * 0.1f, &desc.vel);
    desc.rotVel.x = (math_randomFloat() * 300.0f + -149.0f) * 0.1f;
    desc.rotVel.z = (math_randomFloat() * 300.0f + -149.0f) * 0.1f;
    desc.lifetime = (math_random() % 33U) + 32;
    desc.gravity = (math_randomFloat() * 603.0f + -2402.0f) * 0.001f;
    desc.sizeY = (math_randomFloat() * 300.0f + 101.0f) * 0.001f;
    desc.variant = 0;
    desc.zone = 0;
    desc.sizeXZ = desc.sizeY;
    desc.alpha = (math_random() % 101U) + 155;
    desc.updateFn = 2;
    desc.update2Fn = 0;
    desc.floorHitFn = 0;
    desc.floorType3Fn = 0;
    desc.wallHitFn = 0;
    desc.destroyFn = 0;
    desc.squashFn = 0;
    desc.matrixFn = 0;
    desc.lightMode = 2;
    desc.owner = 0;
    desc.ownerId = 0;
    desc.fadeStart = 32;
    desc.fadeRate = 7;
    particle_newModelParticle(&desc, 3, 0xFF, 0, arg14, 0);
    return 1;
}

/*
 * Update for the particle_spawnSideDust dust: grows it while growTime lasts, and once flag 1 is set
 * fades it out over the last 32 frames of its timer.
 */
s32 particle_updateDustPuff(DustPuff *dust) {
    dust->growth.growTime -= gFrameTicks;
    if (dust->growth.growTime > 0) {
        f32 grow = dust->growth.growRate * gFrameDelta;

        dust->size0 = dust->size0 + grow;
        dust->size1 = dust->size1 + grow;
    }
    if (dust->flags & 1) {
        if (dust->timer < 32) {
            if (dust->timer * 8 < dust->alpha) {
                dust->alpha = dust->timer * 8;
            }
        }
    }
    return 1;
}

/*
 * Spawns one particle_newOwnedSpriteParticle particle at (x, y, z), its velocity built by math_dirFromBinAngles from
 * `angle`. It stores the damping factor that particle_dampSparkXZ applies each frame step.
 */
s32 particle_spawnYawSpark(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    SparkParticle *particle;
    ParticleDesc desc;
    f32 size;
    f32 damping;
    f32 fraction;
    u32 random;

    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.setGeometryMode = 0x200005;
    desc.clearGeometryMode = 0;
    desc.timer = (math_random() % 41U) + 40;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0x0B;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = (math_random() % 157U) + 100;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    size = (math_randomFloat() * 401.0f) + 300.0f;
    desc.width = size;
    desc.height = size;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;

    random = math_random();
    fraction = math_randomFloat();
    math_dirFromBinAngles(angle, (random % 26U) - 25, ((fraction * 500.0f) + 1000.0f) * 0.01f, &desc.vel);
    desc.flags = 0xE05;
    desc.gravity = 0.0f;

    if (math_random() & 1) {
        desc.flags |= 0x40;
    }

    if (math_random() & 1) {
        desc.flags |= 0x80;
    }

    desc.updateFunc = 7;
    desc.deathFunc = -1;
    desc.fadeStart = 25;
    desc.fadeRate = 10;
    desc.scaleStart = 60;
    desc.scaleRate = 1.02f;
    damping = 0.962000072f;

    particle = particle_newOwnedSpriteParticle(&desc, 1, sizeof(damping), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Multiplies desc.vel.x and desc.vel.z by the particle's damping once per frame step. */
s32 particle_dampSparkXZ(SparkParticle *particle, s32 arg1) {
    s32 i;
    f32 *damping;

    damping = &particle->damping; // matching: read through a pointer, IDO reloads it after every store
    for (i = gFrameTicks; i != 0; i--) {
        particle->head.desc.vel.x *= *damping;
        particle->head.desc.vel.z *= *damping;
    }
    return 1;
}

/* Advances the particle one frame step under constant vertical acceleration. */
s32 particle_moveBallisticConstAccel(BallisticParticle *particle) {
    particle->pos[0] += particle->vel[0] * gFrameDelta;
    particle->pos[1] += (particle->vel[1] * gFrameDelta) + (((particle->gravity * gFrameDelta) * gFrameDelta) * 0.5f);
    particle->pos[2] += particle->vel[2] * gFrameDelta;
    particle->vel[1] += particle->gravity * gFrameDelta;
    particle->unk20[0] += particle->unk50[0] * gFrameDelta;
    particle->unk20[1] += particle->unk50[1] * gFrameDelta;
    particle->unk20[2] += particle->unk50[2] * gFrameDelta;
    return 1;
}

/* Sets flag 1 and a 32-frame timer (the fade-out particle_updateDustPuff runs on its dust). */
void particle_startDustFadeOut(FadeoutParticle *particle) {
    particle->flags |= 1;
    particle->timer = 32;
}
