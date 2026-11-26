#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Emission point offsets in the bone space of side 2 (bone 25) and side 1 (bone 32). */
static const Vec3f D_800AA0F0 = { -19.0f, 0.0f, 25.0f };
static const Vec3f D_800AA0FC = { 19.0f, 0.0f, 25.0f };


/*
 * Transforms the fixed offset for `side` (1 or 2) by that side's bone matrix of the actor
 * (bone 32 or 25) into `pos`.
 * If `ground` is non-NULL, also probes the ground below that point into it.
 */
s32 particle_getSpraySidePos(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side) {
    Vec3f probe;
    const Vec3f *offset;
    MtxF *bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        offset = &D_800AA0FC;
    } else {
        offset = &D_800AA0F0;
    }

    if (side == 1) {
        bone = &actor->ob_matrix_ptr[32];
    } else {
        bone = &actor->ob_matrix_ptr[25];
    }

    func_15143134(offset, pos, bone);
    if (ground == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 100.0f;
    probe.z = pos->z;
    collision_initHitFromObject(ground, actor);
    return collision_probeFloorSurfABSet0Cached(&probe, 0, pos->y - 500.0f, ground);
}

s32 func_151AF6C0(s32 arg0, s32 arg1) {
    return 12;
}

/* One corner colour passed to particle_spawnAxisQuadParticleNoModel. */
typedef struct {
    s16 r;
    s16 g;
    s16 b;
    s16 a;
    s16 unk8;
} SprayColor;

/*
 * Spawns a spray particle on the ground below the emission point on the actor's `side`, plus a
 * particle_emitOnRandomCircle puff there. The colour is tinted by particle_getDepthTint when level is 20.
 */
void particle_spawnSideSpray(Actor *actor, s32 side, s32 arg2) {
    Vec3f pos;
    CollisionHit ground;
    f32 fr;
    f32 fg;
    f32 fb;
    SprayParticle *particle;
    SprayGrowth growth;
    ParticleDef desc;
    s32 lifeRoll; // matching: IDO schedules the first roll differently when it is inlined
    SprayColor colors[4];
    s16 roll;
    u8 r;
    u8 g;
    u8 b;

    if (actor == NULL) {
        return;
    }
    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    if (particle_getSpraySidePos(&pos, actor, &ground, side) == 0) {
        return;
    }
    if (level == 20) {
        particle_getDepthTint(pos.z, &fr, &fg, &fb);
        r = fr;
        g = fg;
        b = fb;
    } else {
        r = g = b = 0xFF;
    }
    colors[0].r = r;
    colors[0].g = g;
    colors[0].b = b;
    colors[0].a = 180;
    colors[0].unk8 = 0;
    colors[1].r = r;
    colors[1].g = g;
    colors[1].b = b;
    colors[1].a = 180;
    colors[1].unk8 = 0;
    colors[2].r = r;
    colors[2].g = g;
    colors[2].b = b;
    colors[2].a = 180;
    colors[2].unk8 = 0;
    colors[3].r = r;
    colors[3].g = g;
    colors[3].b = b;
    colors[3].a = 180;
    colors[3].unk8 = 0;
    desc.texture = 0x50;
    desc.renderModes = 0x2502;
    lifeRoll = func_150ADA20();
    desc.lifetime = (lifeRoll % 61U) + 88;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.rgba[0] = 0xFF;
    desc.rgba[1] = 0xFF;
    desc.rgba[2] = 0xFF;
    desc.rgba[3] = 0xFF;
    desc.width = desc.height = ((func_150ADA68() * 21.0f) + 91.0f) * 0.001f;
    desc.pos.x = pos.x;
    desc.pos.y = ground.height;
    desc.pos.z = pos.z;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = 0x401C0001;
    desc.unk44 = (func_150ADA20() % 41U) + 80;
    desc.unk45 = 0xFF;
    growth.growEndTimer = (desc.lifetime - (func_150ADA20() % 6U)) - 8;
    growth.growRate = (((func_150ADA68() * 40.0f) + 51.0f) * desc.width) * 0.001f;
    desc.unk46 = 0;
    desc.unk47 = 6;
    particle = particle_spawnAxisQuadParticleNoModel(&desc, colors, 0, 0x16, 0, 0, (actor->langle >> 8) - ((side == 1) ? -10 : 10),
                             500.0f, 500.0f, NULL, &ground.tri, 1, 8, 0xFF, 0);
    if (particle != NULL) {
        memcpy(&particle->growth, &growth, sizeof(growth));
    }
    roll = (func_150ADA20() % 11U) + 5;
    particle_emitOnRandomCircle(pos.x, pos.y, pos.z, (func_150ADA68() * 17.0f) + 29.0f,
                  0, 0xFF, roll, 16, 0, 0.0f, NULL, 0xFF);
}

/* QuadSprite update: fades the sprite (def.unk10) out over the last 32 frames of its lifetime. */
s32 func_151AFBD4(QuadSprite *sprite) {
    s32 fade;

    if (sprite->def.lifetime < 32) {
        fade = sprite->def.lifetime * 8;
        if (fade < sprite->def.unk10) {
            sprite->def.unk10 = fade;
        }
    }
    return 1;
}

/*
 * Update for the particle_spawnSideSpray spray: once flag 1 is set fades it out over the last 32 frames
 * of its timer, and grows it until the timer reaches growth.growEndTimer.
 */
/*
 * Update for the particle_spawnSideSpray spray: once flag 1 is set, fades it out over the last 32 frames
 * of its life and grows it until life drops to growth.growEndTimer.
 */
s32 particle_updateSpray(SprayParticle *spray) {
    ParticleDef *desc;
    SprayGrowth *growth;

    if (spray->head.def.flags & 1) {
        desc = &spray->head.def;
        growth = &spray->growth;
        if (spray->head.def.lifetime < 32) {
            if (spray->head.def.lifetime * 8 < spray->head.def.unk44) {
                // matching: a store through a second pointer makes IDO reload life afterwards
                desc->unk44 = spray->head.def.lifetime * 8;
            }
        }
        if (growth->growEndTimer < spray->head.def.lifetime) {
            f32 grow = growth->growRate * gFrameDelta;

            spray->head.def.width = spray->head.def.width + grow;
            spray->head.def.height = spray->head.def.height + grow;
        }
    }
    return 1;
}

/* Spawns one spark at (x, y, z), thrown in a random direction around `yaw`. */
s32 particle_spawnYawSpraySpark(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    SparkParticle *spark;
    ParticleDesc desc;
    s32 pad1; // matching: unused stack slot
    f32 damping;
    s32 pad2; // matching: unused stack slot
    s32 roll;

    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = (func_150ADA20() & 0xF) + 25;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0xB;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = (func_150ADA20() % 156U) + 100;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    desc.width = desc.height = (func_150ADA68() * 202.0f) + 101.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;

    roll = func_150ADA20();
    func_15143794(yaw, (roll % 21U) - 20, (func_150ADA68() * 301.0f + 500.0f) * 0.01f, &desc.vel);

    desc.flags = 0xE05;
    desc.gravity = 0.0f;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }

    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.updateFunc = 6;
    desc.scaleRate = 1.04f;
    damping = 0.9490000606f;
    desc.deathFunc = -1;
    desc.fadeStart = 15;
    desc.fadeRate = 17;
    desc.scaleStart = 25;

    spark = particle_newOwnedSpriteParticle(&desc, 1, 4, arg14, 1);
    if (spark != NULL) {
        memcpy(&spark->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Damps the spark's desc.vel.x and desc.vel.z by its damping factor once per elapsed frame. */
s32 particle_dampSprayXZ(SparkParticle *spark, s32 arg1) {
    f32 *damping;
    s32 frames;

    damping = &spark->damping;
    for (frames = D_800BE9E4; frames != 0; frames--) {
        spark->head.desc.vel.x *= *damping;
        spark->head.desc.vel.z *= *damping;
    }
    return 1;
}
