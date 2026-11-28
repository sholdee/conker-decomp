#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/* Object spawned by particle_newOwnedSpriteParticle from a ParticleDesc packet. */
struct ParticleObject {
    u8 pad0[0x10];
    ParticleDesc p;   /* 0x10 */
    u8 pad80[0x28];
    f32 damping;  /* 0xA8: velocity factor per tick, written by effect_spawnDampedParticle */
};

/* The first 0x1C bytes of a SpriteDef, for particle_spawnFlatQuadSpriteNoModel (which copies sizeof(SpriteDef)).
 * matching: a whole SpriteDef local in its place moves effect_spawnImpactRing's stack slots. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;
    /* 0x07 */ u8 handlerSet;
    /* 0x08 */ s32 frame;
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 colorLevel;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 primMode;
    /* 0x17 */ u8 envMode;
    /* 0x18 */ s32 renderModes;
} SpriteDef1C;

/*
 * Impact effect at the actor: finds the floor below it, spawns an expanding ring there and,
 * when the floor was found, four particle_spawnFlatQuadSpriteNoModel quads around the impact point plus particles.
 */
void effect_spawnImpactRing(Actor *actor) {
    Vec3f pos;
    f32 r, g, b;
    u8 red, green, blue;
    CollisionHit floorInfo;
    u8 onGround;
    u32 end;
    u32 addr;
    s32 sum;
    u8 padA[4]; // matching: frame padding

    onGround = 0;
    if (actor != NULL) {
        collision_initHitFromObject(&floorInfo, actor);
        camera_startEnvelope(&camera_global_Cameras[gLastPlayerIndex], 7, 1);
        pos.x = actor->xpos;
        pos.y = actor->ypos + 100.0f;
        pos.z = actor->zpos;
        if (collision_probeFloorSurfABSet0Cached(&pos, 0, actor->ypos - 300.0f, &floorInfo) != 0) {
            onGround = 1;
            pos.y = floorInfo.height;
        } else {
            pos.y = actor->floorY;
        }
        /* Anti-tamper check: checksum the code from effect_spawnHitSplash to effect_hitReactSplash. */
        sum = 0;
        end = (u32)effect_hitReactSplash;
        for (addr = (u32)effect_spawnHitSplash; addr < end; addr += 4) {
            sum ^= ~*(s32 *)addr;
        }
        if (sum != 0x79844C6B) {
            gBonePoolSize = 0;
        }
        particle_getDepthTint(pos.z, &r, &g, &b);
        red = r;
        green = g;
        blue = b;
        particle_startPlayerRumbles(&pos, 500.0f, 5000.0f, 0.0002f, 0xC, 0xF, 0xFF, 0);
        {
            RadialEffectDef def;
            u8 padB[0x14]; // matching: frame padding

            def.pos = pos;
            def.speed = 43.20000076f;
            def.flags = 1;
            def.checkCallback = -1;
            def.count = 13;
            def.remaining = 101;
            def.growUntil = 88;
            def.growRate = 21.80000114f / (def.remaining - def.growUntil);
            def.shrinkFrom = 19;
            def.shrinkRate = 21.80000114f / def.shrinkFrom;
            def.emitCallback = 1;
            def.updateCallback = 0;
            def.emitRate = 7.999999798e-05f;
            effect_newRingEffect(&def, 0, 0xFF, 1);
        }
        if (onGround) {
            SpriteDef1C fx;
            f32 pad; // matching: unused stack slot
            f32 size;
            f32 extent;
            u8 heading;
            f32 sinH, cosH;
            u8 padC[8]; // matching: frame padding
            s16 colors[4][5];
            s16 randomArg;
            u8 spark[3];

            heading = -0x40 - (actor->langle >> 8);
            cosH = math_cosBinAngleMasked(heading);
            sinH = math_cosBinAngleMasked(heading - 0x40);
            size = extent = (math_randomFloat() * 96.0f + 352.0f) * 0.5f;
            fx.flags = 0x701;
            math_random();
            fx.lifetime = 0x408;
            fx.frame = 0;
            fx.frameSpeed = 0;
            fx.alpha = math_random() % 41U + 0xB4;
            fx.colorLevel = 0xFF;
            fx.rgba[0] = 0xFF;
            fx.rgba[1] = 0xFF;
            fx.primMode = 0;
            fx.rgba[2] = 0xFF;
            fx.rgba[3] = 0xFF;
            fx.renderModes = 0x260001;
            fx.envMode = 6;
            colors[0][3] = 0xFF;
            colors[0][4] = 0;
            colors[1][3] = 0xFF;
            colors[1][4] = 0;
            colors[2][3] = 0xFF;
            colors[2][4] = 0;
            colors[3][3] = 0xFF;
            colors[3][4] = 0;
            fx.texture = 0x51;
            colors[0][0] = red;
            colors[1][0] = red;
            colors[2][0] = red;
            colors[3][0] = red;
            colors[0][1] = green;
            colors[1][1] = green;
            colors[2][1] = green;
            colors[3][1] = green;
            colors[0][2] = blue;
            colors[1][2] = blue;
            colors[2][2] = blue;
            colors[3][2] = blue;
            // matching: the corner offsets are written as (+/-1.0f * extent)
            particle_spawnFlatQuadSpriteNoModel(&fx, colors, 0xC, 0, extent * cosH + pos.x - extent * sinH, pos.y + 10.0f, extent * cosH + pos.z + extent * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.texture = 0x52;
            particle_spawnFlatQuadSpriteNoModel(&fx, colors, 0xC, 0, extent * cosH + pos.x - (-1.0f * extent) * sinH, pos.y + 10.0f, (-1.0f * extent) * cosH + pos.z + extent * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.texture = 0x53;
            particle_spawnFlatQuadSpriteNoModel(&fx, colors, 0xC, 0, (-1.0f * extent) * cosH + pos.x - extent * sinH, pos.y + 10.0f, extent * cosH + pos.z + (-1.0f * extent) * sinH, size, size, heading, 0, 0, 0xFF, 0);
            fx.texture = 0x54;
            particle_spawnFlatQuadSpriteNoModel(&fx, colors, 0xC, 0, (-1.0f * extent) * cosH + pos.x - (-1.0f * extent) * sinH, pos.y + 10.0f, (-1.0f * extent) * cosH + pos.z + (-1.0f * extent) * sinH, size, size, heading, 0, 0, 0xFF, 0);
            effect_emitGroundCircle(&pos);
            randomArg = math_random() % 21U + 10;
            spark[0] = red;
            spark[1] = green;
            spark[2] = blue;
            particle_emitOnRandomCircle(pos.x, pos.y, pos.z, math_randomFloat() * 81.0f + 150.0f, 0, 0xFF, randomArg, 0x14, 0, 0.0f, spark, 0xFF);
        }
    }
}


void effect_emitGroundCircle(Vec3f *pos) {
    s16 randomArg;

    randomArg = (math_random() % 21U) + 10;
    particle_emitOnRandomCircle(pos->x, pos->y, pos->z, (math_randomFloat() * 59.0f) + 170.0f,
                  0, 0xFF, randomArg, 0x13, 0, 0.0f, NULL, 0xFF);
}

/* Fades the object out over its last 32 ticks by capping alpha at timer * 8. */
s32 effect_capQuadSpriteAlphaByLife(QuadSprite *sprite) {
    s32 maxAlpha;

    if (sprite->def.lifetime < 32) {
        maxAlpha = sprite->def.lifetime * 8;
        if (maxAlpha < sprite->def.alpha) {
            sprite->def.alpha = maxAlpha;
        }
    }
    return 1;
}

/* Spawns a particle at (x, y, z) moving along heading; effect_dampParticleVelocityXZ damps its velocity. */
s32 effect_spawnDampedParticle(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s16 heading, s32 baseAngle, f32 radius, s32 arg11,
                  s32 arg12, void *data, u8 owner) {
    ParticleObject *obj;
    ParticleDesc packet;
    s32 pad1; // matching: unused stack slot
    f32 damping;
    s32 pad2; // matching: unused stack slot
    u32 random;

    packet.texture = 0x29;
    packet.renderMode = 0xE03;
    packet.setGeometryMode = 0x200005;
    packet.clearGeometryMode = 0;
    packet.timer = (math_random() % 26U) + 25;
    packet.frame = 0;
    packet.frameRate = 0;
    packet.r2 = 0xB0;
    packet.g2 = 0xA0;
    packet.b2 = 0x2A;
    packet.r = 0x40;
    packet.g = 0x0B;
    packet.b = 0x6A;
    packet.a = 0xFF;
    packet.alpha = (math_random() % 156U) + 100;
    packet.brightness = 0xFF;
    packet.primColorMode = 3;
    packet.envColorMode = 3;

    packet.width = packet.height = (math_randomFloat() * 401.0f) + 800.0f;
    packet.pos.x = x;
    packet.pos.y = y;
    packet.pos.z = z;

    random = math_random();
    math_dirFromBinAngles(heading, (random % 12U) - 21,
                  (math_randomFloat() * 300.0f + 498.0f) * 0.1000000015f,
                  &packet.vel);

    packet.gravity = 0.0f;
    packet.flags = 0xE05;
    if (math_random() & 1) {
        packet.flags |= 0x40;
    }
    if (math_random() & 1) {
        packet.flags |= 0x80;
    }

    packet.updateFunc = 9;
    packet.scaleRate = 1.039999962f;
    damping = 0.9510000348f;
    packet.deathFunc = -1;
    packet.fadeStart = 25;
    packet.fadeRate = 10;
    packet.scaleStart = 0x20;

    obj = particle_newOwnedSpriteParticle(&packet, 1, 4, owner, 1);
    if (obj != NULL) {
        memcpy(&obj->damping, &damping, sizeof(f32));
    }

    return 1;
}

/* Damps a particle's horizontal velocity once per elapsed tick. */
s32 effect_dampParticleVelocityXZ(ParticleObject *obj, Vec3f *prevPos) {
    f32 *damping; // matching: reading obj->damping directly changes the loop code
    s32 ticks;

    damping = &obj->damping;
    for (ticks = gFrameTicks; ticks != 0; ticks--) {
        obj->p.vel.x *= *damping;
        obj->p.vel.z *= *damping;
    }
    return 1;
}

/* Spawns a tumbling burst object at (x, y, z) moving along heading. */
s32 effect_spawnTumblingBurst(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s16 heading, s32 baseAngle, f32 radius, s32 arg11,
                  s32 arg12, f32 *data, u8 owner) {
    s32 pad; // matching: unused stack slot
    ParticleSpawn packet;

    packet.pos.x = x;
    packet.pos.y = y;
    packet.scale.x = 1.0f;
    packet.scale.y = 1.0f;
    packet.scale.z = 1.0f;
    packet.flags = 0x29E8;
    packet.rot.x = 0.0f;
    packet.rot.y = 0.0f;
    packet.rot.z = 0.0f;
    packet.rotVel.y = 0.0f;
    packet.radius = 1.0f;
    packet.model = 0x20;
    packet.pos.z = z;
    packet.elasticity = 0.200000003f;

    // matching: the original math_random returned a full word; its spill is the
    // call-argument slot at sp+0x20, 8 below packet
    ((s32 *)&packet)[-2] = math_random();
    math_dirFromBinAngles(heading, (((s32 *)&packet)[-2] % 21U) - 40,
                  (math_randomFloat() * 204.0f + 500.0f) * 0.1000000015f,
                  &packet.vel);

    packet.rotVel.x = (math_randomFloat() * 260.0f + -130.0f) * 0.1000000015f;
    packet.rotVel.z = (math_randomFloat() * 260.0f + -130.0f) * 0.1000000015f;
    packet.lifetime = (math_random() % 51U) + 50;
    packet.gravity = (math_randomFloat() * 1008.0f + -3085.0f) * 0.001000000047f;

    packet.sizeXZ = packet.sizeY = (math_randomFloat() * 601.0f + 200.0f) * 0.001000000047f;
    packet.variant = 0;
    packet.zone = 0;

    packet.alpha = (math_random() % 76U) + 180;
    packet.updateFn = 4;
    packet.update2Fn = 0;
    packet.floorHitFn = 0;
    packet.floorType3Fn = 0;
    packet.wallHitFn = 0;
    packet.destroyFn = 0;
    packet.squashFn = 0;
    packet.matrixFn = 0;
    packet.lightMode = 2;
    packet.owner = 0;
    packet.ownerId = 0;
    packet.fadeStart = 0x20;
    packet.fadeRate = 7;

    particle_newModelParticle(&packet, 3, 0xFF, 0, owner, 0);
    return 1;
}

/* Same per-frame integration as effect_moveBurstParticle. */
s32 effect_moveTumblingBurst(BurstParticle *particle) {
    particle->p.pos.x += particle->p.vel.x * gFrameDelta;
    particle->p.pos.y += (particle->p.vel.y * gFrameDelta) + (particle->p.gravity * gFrameDelta * gFrameDelta * 0.5f);
    particle->p.pos.z += particle->p.vel.z * gFrameDelta;
    particle->p.vel.y += particle->p.gravity * gFrameDelta;
    particle->p.rot.x += particle->p.rotVel.x * gFrameDelta;
    particle->p.rot.y += particle->p.rotVel.y * gFrameDelta;
    particle->p.rot.z += particle->p.rotVel.z * gFrameDelta;
    return 1;
}
