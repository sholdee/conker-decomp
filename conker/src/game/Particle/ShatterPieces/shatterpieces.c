#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Collision handlers of the pooled physics particles (game/Particle/ModelParticle/modelparticle.c's gModelParticleFloorFuncs / gModelParticleWallFuncs tables).
 * The particles are ModelParticles (effects.h). CollisionProbe (game/Particle/ModelParticle/modelparticle.c's query) and AxisParticle
 * (the particle system of game/Particle/QuadParticle/quadparticle.c) are in conker/particle.h.
 */

Actor *gKickableLookAtActor = NULL;                  /* object 1, followed by particle_updateKickable */
Vec3f gKickableLookAtPos = { 0.0f, 0.0f, 0.0f };   /* its position, 50 above */
Actor *gKickablePusher = NULL;                  /* object 1, pushing particle_fallAndGrow's particles */
static s32 gShatterSounds[2] = { 0x9C, 0x9D }; /* shatter sounds, one at random */
static s32 gShatterWallSound = 0xA1;              /* wall-hit sound */
static s32 D_80088740 = 0x69;              /* landing sound of the falling/growing particles */
void *memcpy(void *, const void *, u32);

/* Floor hit: above speed 30 the particle shatters into dust and is destroyed; above 15 it bounces;
 * otherwise it comes to rest, pointing its matrix 200 units along its current facing. */
s32 particle_floorShatter(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface) {
    particle->pos.y = particle->radius + floorHeight;
    if (particle->speed > 15.0f) {
        if (particle->speed > 30.0f) {
            ParticleDef def;
            f32 extra[2];

            def.fadeStart = 25;
            def.fadeRate = 10;
            def.texture = 4;
            def.type = 0;
            def.renderModes = 0x3B02;
            def.lifetime = (func_150ADA20() % 31U) + 40;
            def.frame = 0;
            def.frameSpeed = 0;
            def.rgba[0] = 0xFF;
            def.rgba[1] = 0xFF;
            def.rgba[2] = 0xFF;
            def.rgba[3] = 0xFF;
            def.width = def.height = ((func_150ADA68() * 25.0f) + 25.0f) * 0.001f;
            def.flags = 0x0C5C0001;
            def.unk44 = 0xFF;
            def.unk45 = 0xFF;
            def.unk46 = 0;
            def.unk47 = 7;
            def.pos.x = x;
            def.rot.x = 0.0f;
            def.rot.y = 0.0f;
            def.rot.z = 0.0f;
            def.scale.x = 0.0f;
            def.scale.y = 0.0f;
            def.scale.z = 0.0f;
            def.pos.y = floorHeight;
            def.pos.z = z;
            extra[0] = ((func_150ADA68() * 80.0f) + 50.0f) * 1e-05f;
            def.unk50 = particle->floor.material;
            // matching: the spawned particle reuses the surface parameter; a new local takes another stack slot
            surface = particle_spawnAxisQuadParticle(&def, &gWhiteVtxColors, 0x18, 0, 0,
                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, NULL, surface,
                                            particle->link.model, particle->link.unk1C, 0, 8, particle->header.owner,
                                            particle->header.kind);
            if (surface != NULL) {
                memcpy(((AxisParticle *)surface)->motion, extra, 8);
            }
            {
                PuffSpray burst;

                burst.pos.x = x;
                burst.countBase = 6;
                burst.countRange = 6;
                burst.angles.yawRange = 0xFF;
                burst.angles.pitchBase = -63;
                burst.angles.pitchRange = 29;
                burst.sizeBase = 3.0f;
                burst.sizeRange = 3.0f;
                burst.speedBase = 10.0f;
                burst.speedRange = 10.0f;
                burst.angles.yawBase = 0;
                burst.itemCountBase = 3;
                burst.itemCountRange = 3;
                burst.lifeBase = 50;
                burst.lifeRange = 20;
                burst.green = 1;
                burst.headerFlags = 0x21;
                burst.red = 0x48;
                burst.handlers[0] = 1;
                burst.handlers[1] = 0;
                burst.handlers[2] = 0;
                burst.handlers[3] = 0;
                burst.handlers[4] = 0;
                burst.handlers[5] = 0;
                burst.blue = 0;
                burst.alphaBase = 155;
                burst.alphaRange = 100;
                burst.pos.y = floorHeight;
                burst.pos.z = z;
                burst.growthBase = 0.661f;
                burst.growthRange = 0; // matching: 0.0f would keep the float zero above live in $f20 across the calls
                burst.unk58 = particle->link.model;
                burst.unk5C = particle->link.unk1C;
                burst.render.unk0 = 0;
                burst.render.setGeometryMode = 0x220005;
                burst.render.clearGeometryMode = 0x1D0600;
                burst.render.renderMode = 1;
                burst.render.unk10 = 0x3B;
                burst.render.unk14 = 0x80;
                burst.render.unk18 = 0x20;
                burst.render.primMode = 0;
                burst.render.envMode = 7;
                burst.unk80 = 10;
                burst.unk82 = 25;
                burst.unk84 = particle->floor.material;
                particle_spawnPuffSpray(&burst, particle->header.owner, particle->header.kind);
            }
            func_10010F88(gShatterSounds[func_150ADA20() & 1], 32000, 0, 0, 0, particle->pos.x, floorHeight + 25.0f,
                          particle->pos.z, 1000, 4000);
            return 0;
        }
        particle->vel.x *= particle->elasticity;
        particle->vel.y = -particle->vel.y * particle->elasticity;
        particle->vel.z *= particle->elasticity;
        particle->rotVel.x *= particle->elasticity;
        particle->rotVel.y *= particle->elasticity;
        particle->rotVel.z *= particle->elasticity;
        func_10010F88(gShatterWallSound, 32000, 0, 0, 0, particle->pos.x, particle->pos.y, particle->pos.z, 1000, 4000);
    } else {
        MtxF mtx;

        particle->update2Fn = 1;
        particle->matrixFn = 1;
        particle->flags &= ~0xFF;
        func_150A8050(mtx, particle->rot.x, particle->rot.y, particle->rot.z);
        func_150A7960(mtx, 0.0f, 0.0f, 200.0f, &particle->rot.x, &particle->rot.y, &particle->rot.z);
        particle->rot.x += particle->pos.x;
        particle->rot.y += particle->pos.y;
        particle->rot.z += particle->pos.z;
    }
    return 1;
}

/* Wall hit (x, y, z: the position before the move): a hard hit casts a ray along the move, puffs dust
 * off the surface it finds and destroys the particle; a soft one stops it against the wall. */
s32 particle_wallShatter(ModelParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z) {
    f32 impactSpeed = particle->speed - fabsf(particle->vel.y); /* speed less the vertical part */

    if (impactSpeed > 6.0f) {
        if (zone_isInNeutralZone(1, probe->pos.x, probe->pos.y, probe->pos.z) == 0) {
            Vec3f move;
            Vec3f hit;
            CollisionTri triangle;
            s32 hitObject;
            ParticleDef def;
            AxisParticle *spawned;
            f32 extra;

            move.x = particle->pos.x - x;
            move.y = particle->pos.y - y;
            move.z = particle->pos.z - z;
            if (func_150AC9C0(x, y, z, move.x, move.y, move.z, NULL, &triangle, &hit.x, &hit.y, &hit.z,
                              NULL, &hitObject, NULL, 0.0f) != 0) {
                // matching: a switch, not an if; the if form swaps the $s0/$s1 homes of particle and probe
                switch (math_isObjectHidden(hitObject)) {
                case 0:
                    break;
                default:
                    def.fadeStart = 25;
                    def.fadeRate = 10;
                    def.texture = 4;
                    def.type = 0;
                    def.renderModes = 0x3B02;
                    def.lifetime = (func_150ADA20() % 31U) + 40;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0xFF;
                    def.rgba[1] = 0xFF;
                    def.rgba[2] = 0xFF;
                    def.rgba[3] = 0xFF;
                    def.width = def.height = (func_150ADA68() * 25.0f + 25.0f) * 0.001f;
                    def.pos = hit;
                    def.flags = 0x0C5C0001;
                    def.unk44 = 0xFF;
                    def.unk45 = 0xFF;
                    def.unk46 = 0;
                    def.unk47 = 7;
                    def.rot.x = 0.0f;
                    def.rot.y = 0.0f;
                    def.rot.z = 0.0f;
                    def.scale.x = 0.0f;
                    def.scale.y = 0.0f;
                    def.scale.z = 0.0f;
                    extra = (func_150ADA68() * 80.0f + 50.0f) * 1e-05f;
                    def.unk50 = particle->floor.material;
                    spawned = particle_spawnAxisQuadParticle(&def, &gWhiteVtxColors, 0x18, 0, 0,
                                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, NULL,
                                                            &triangle, particle->link.model, particle->link.unk1C, 0,
                                                            8, particle->header.owner, particle->header.kind);
                    if (spawned != NULL) {
                        /* the 8-byte motion block; 4 bytes past extra are copied too */
                        memcpy(spawned->motion, &extra, 8);
                    }
                    break;
                }
                {
                    SurfacePuffSpray burst;

                    burst.source.normal.x = -move.x;
                    burst.source.normal.y = -move.y;
                    burst.source.normal.z = -move.z;
                    burst.source.triangle = triangle;
                    burst.source.spread = 502.0f;
                    burst.pos = hit;
                    burst.sizeBase = 3.0f;
                    burst.sizeRange = 3.0f;
                    burst.growthBase = 0.2650000155f; // not 0.265f: one ulp above it
                    burst.growthRange = 0.1610000134f; // not 0.161f: one ulp above it
                    burst.speedBase = 0.005399999674f; // not 0.0054f: one ulp below it
                    burst.speedRange = 0.01349999942f; // not 0.0135f: one ulp below it
                    burst.countBase = 6;
                    burst.countRange = 6;
                    burst.itemCountBase = 3;
                    burst.itemCountRange = 3;
                    burst.lifeBase = 50;
                    burst.lifeRange = 20;
                    burst.green = 1;
                    burst.headerFlags = 0x21;
                    burst.red = 0x48;
                    burst.handlers[0] = 1;
                    burst.handlers[1] = 0;
                    burst.handlers[2] = 0;
                    burst.handlers[3] = 0;
                    burst.handlers[4] = 0;
                    burst.handlers[5] = 0;
                    burst.blue = 0;
                    burst.alphaBase = 155;
                    burst.alphaRange = 100;
                    burst.unk74 = particle->link.model;
                    burst.unk78 = particle->link.unk1C;
                    burst.render.unk0 = 0;
                    burst.render.setGeometryMode = 0x220005;
                    burst.render.clearGeometryMode = 0x1D0600;
                    burst.render.renderMode = 1;
                    burst.render.unk10 = 0x3B;
                    burst.render.unk14 = 0x80;
                    burst.render.unk18 = 0x20;
                    burst.render.primMode = 0;
                    burst.render.envMode = 7;
                    burst.unk9C = 10;
                    burst.unk9E = 25;
                    burst.unkA0 = particle->floor.material;
                    particle_spawnSurfacePuffSpray(&burst, particle->header.owner, particle->header.kind);
                }
            }
        }
        func_10010F88(gShatterSounds[func_150ADA20() & 1], 32000, 0, 0, 0, particle->pos.x, particle->pos.y,
                      particle->pos.z, 1000, 4000);
        return 0;
    }
    particle->vel.x = 0.0f;
    particle->vel.z = 0.0f;
    if (particle->vel.y > 0.0f) {
        particle->vel.y *= particle->elasticity;
    }
    particle->pos.x = probe->pos.x;
    particle->pos.y = probe->pos.y;
    particle->pos.z = probe->pos.z;
    particle->rotVel.x *= particle->elasticity;
    particle->rotVel.y *= particle->elasticity;
    particle->rotVel.z *= particle->elasticity;
    func_10010F88(gShatterWallSound, 64000, 0, 0, 0, probe->pos.x, probe->pos.y, probe->pos.z, 1000, 4000);
    return 1;
}

/* Falling and growing physics particles of game/Particle/QuadParticle/quadparticle.c's particle/sprite system (FallGrowParticle, QuadSprite). */

/* Falls (speed growing by 0.3 per frame-time) until lifetime 80; from 115 on grows by 1.35x a frame,
 * below 65 the height grows instead. */
s32 particle_fallAndGrow(FallGrowParticle *particle) {
    f32 *speed = &particle->fallSpeed;
    f32 scale;

    if (particle->head.def.lifetime < 80) {
        *speed += 0.3f * gFrameDelta;
        particle->head.def.pos.y -= *speed;
    }

    if (particle->head.def.lifetime >= 115) {
        scale = 1.35f;
        particle->head.def.width *= scale;
        particle->head.def.height *= scale;
    } else if (particle->head.def.lifetime < 65) {
        particle->head.def.height += 1.2f;
    }

    return 1;
}

/* Sets def.unk44 to 8x the lifetime once it drops below 32. */
s32 particle_fadeFallGrowParticle(FallGrowParticle *particle) {
    s16 lifetime = particle->head.def.lifetime;

    if (lifetime < 32) {
        particle->head.def.unk44 = lifetime * 8;
    }
    return 1;
}

/* Sets def.unk10 to 4x the lifetime once it drops below 64. */
s32 particle_fadeQuadSpriteByLife(QuadSprite *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 64) {
        sprite->def.unk10 = lifetime * 4;
    }
    return 1;
}

/*
 * Update of a physics particle. Within reach of object 1 (gKickablePusher): when that object moves faster
 * than 15 it may kick the particle away along its heading; otherwise the particle spawns a sprite on
 * the floor below and a burst, plays a sound and ends (returns 0). Else its rot (look-at) point
 * eases toward 50 above object 1 (gKickableLookAtActor / gKickableLookAtPos).
 */
s32 particle_updateKickable(ModelParticle *particle) {
    f32 distSq;
    Vec3f playerPos;
    f32 dirX;
    f32 dirZ;
    Vec3f pos;

    if ((gKickablePusher == NULL) || (gKickablePusher->type == 0) || (gKickablePusher->unique_id != 1)) {
        gKickablePusher = collision_findObjectById(1);
    }
    if (gKickablePusher != NULL) {
        playerPos.x = gKickablePusher->xpos;
        playerPos.y = gKickablePusher->ypos;
        playerPos.z = gKickablePusher->zpos;
        distSq = (particle->pos.x - playerPos.x) * (particle->pos.x - playerPos.x) +
                 (particle->pos.y - playerPos.y) * (particle->pos.y - playerPos.y) +
                 (particle->pos.z - playerPos.z) * (particle->pos.z - playerPos.z);
        if (distSq < ((particle->radius * particle->radius) + 2500.0f)) {
            if ((gKickablePusher->speed > 15.0f) && (func_150ADA20() & 1)) {
                dirX = math_cosBinAngleMasked(gKickablePusher->angle >> 8);
                dirZ = math_cosBinAngleMasked((gKickablePusher->angle >> 8) + 0x40);
                particle->updateFn = 1;
                particle->flags |= 0x7D;
                particle->floorHitFn = 1;
                particle->wallHitFn = 1;
                particle->matrixFn = 0;
                particle->vel.x = gKickablePusher->speed * dirX * 1.3f;
                particle->vel.y = (func_150ADA68() * 15.0f) + 10.0f;
                particle->vel.z = gKickablePusher->speed * dirZ * 1.3f;
                particle->lifetime = 300;
                particle->flags |= 0x80;
                particle->rot.x = 0.0f;
                particle->rot.y = 0.0f;
                particle->rot.z = 0.0f;
                return 1;
            }
            pos.x = particle->pos.x;
            pos.y = particle->pos.y + 100.0f;
            pos.z = particle->pos.z;
            if (collision_probeFloorSurfABSet0Cached(&pos, 0, particle->pos.y - 2000.0f, &particle->floor) != 0) {
            // matching: an unused label here makes as1 emit the following branch as beqzl
            lbl_spawn:
                pos.y = particle->floor.height;
                {
                    ParticleDef def;
                    AxisParticle *spawned;
                    f32 extra;
                    def.fadeStart = 25;
                    def.fadeRate = 10;
                    def.texture = 4;
                    def.type = 0;
                    def.renderModes = 0x3B02;
                    def.lifetime = (func_150ADA20() % 31U) + 40;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0xFF;
                    def.rgba[1] = 0xFF;
                    def.rgba[2] = 0xFF;
                    def.rgba[3] = 0xFF;
                    def.width = def.height = ((func_150ADA68() * 25.0f) + 25.0f) * 0.001f;
                    def.pos = pos;
                    def.flags = 0x0C5C0001;
                    def.unk44 = 0xFF;
                    def.unk45 = 0xFF;
                    def.unk46 = 0;
                    def.unk47 = 7;
                    def.rot.x = 0.0f;
                    def.rot.y = 0.0f;
                    def.rot.z = 0.0f;
                    def.scale.x = 0.0f;
                    def.scale.y = 0.0f;
                    def.scale.z = 0.0f;
                    extra = ((func_150ADA68() * 80.0f) + 50.0f) * 1e-05f;
                    def.unk50 = particle->floor.material;
                    spawned = particle_spawnAxisQuadParticle(&def, &gWhiteVtxColors, 0x18, 0, 0,
                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), func_150ADA20() & 0xFF,
                                            500.0f, 500.0f, NULL, &particle->floor.tri, particle->link.model,
                                            particle->link.unk1C, 0, 8, particle->header.owner, particle->header.kind);
                    if (spawned != NULL) {
                        /* the 8-byte motion block; 4 bytes past extra are copied too */
                        memcpy(spawned->motion, &extra, 8);
                    }
                }
            }
            {
                PuffSpray burst;
                burst.pos = particle->pos;
                burst.sizeBase = 3.0f;
                burst.sizeRange = 3.100000143f; // not 3.1f: the original constant is one ulp above it
                burst.countBase = 6;
                burst.countRange = 6;
                burst.angles.yawRange = 0xFF;
                burst.angles.pitchBase = -0x40;
                burst.angles.pitchRange = 30;
                burst.angles.yawBase = 0;
                burst.itemCountBase = 3;
                burst.itemCountRange = 3;
                burst.lifeBase = 30;
                burst.lifeRange = 20;
                burst.green = 1;
                burst.headerFlags = 0x21;
                burst.red = 0x48;
                burst.handlers[0] = 1;
                burst.handlers[1] = 0;
                burst.handlers[2] = 0;
                burst.handlers[3] = 0;
                burst.handlers[4] = 0;
                burst.handlers[5] = 0;
                burst.blue = 0;
                burst.alphaBase = 0x9A;
                burst.alphaRange = 0x65;
                burst.growthBase = 0.7050000429f; // not 0.705f: one ulp above it
                burst.growthRange = 0.31f;
                burst.speedBase = 5.0f;
                burst.speedRange = 7.0f;
                burst.unk58 = particle->link.model;
                burst.unk5C = particle->link.unk1C;
                burst.render.unk0 = 0;
                burst.render.setGeometryMode = 0x220005;
                burst.render.clearGeometryMode = 0x1D0600;
                burst.render.renderMode = 1;
                burst.render.unk10 = 0x3B;
                burst.render.unk14 = 0x80;
                burst.render.unk18 = 0x20;
                burst.render.primMode = 0;
                burst.render.envMode = 7;
                burst.unk80 = 10;
                burst.unk82 = 25;
                burst.unk84 = particle->floor.material;
                particle_spawnPuffSpray(&burst, particle->header.owner, particle->header.kind);
            }
            func_10010F88(D_80088740, 25000, 0, 0, 0, particle->pos.x, particle->pos.y, particle->pos.z, 1000, 4000);
            return 0;
        }
    }
    if ((gKickableLookAtActor == NULL) || (gKickableLookAtActor->type == 0) || (gKickableLookAtActor->unique_id != 1)) {
        gKickableLookAtActor = collision_findObjectById(1);
    }
    if (gKickableLookAtActor != NULL) {
        gKickableLookAtPos.x = gKickableLookAtActor->xpos;
        gKickableLookAtPos.y = gKickableLookAtActor->ypos + 50.0f;
        gKickableLookAtPos.z = gKickableLookAtActor->zpos;
    }
    particle->rot.x += 0.2f * (gKickableLookAtPos.x - particle->rot.x);
    particle->rot.y += 0.2f * (gKickableLookAtPos.y - particle->rot.y);
    particle->rot.z += 0.2f * (gKickableLookAtPos.z - particle->rot.z);
    return 1;
}

/* Matrix callback: scale by size and scale, place at pos, and face the particle's rot point. */
s32 particle_buildLookAtMatrix(Mtx *mtx, ModelParticle *particle) {
    math_mtxLookAtScale(mtx, particle->sizeXZ, particle->sizeY, particle->scale.x,
                  particle->scale.y, particle->scale.z, particle->pos.x, particle->pos.y,
                  particle->pos.z, particle->rot.x, particle->rot.y, particle->rot.z);
    return 1;
}

/* When unk98 is -1, spawns a flat sprite (updated by particle_fadeQuadSpriteByLife) at (unk9C, unkA6 + 2, unkA0),
 * unkA2 wide and long, with a random angle and flip. */
void particle_spawnFlatMark(Obj150B9D8C *obj) {
    SpriteDef2C def;
    s32 randFlipHi;
    s32 randFlipLo;
    s32 randAngle;

    if (obj->unk98 == -1) {
        def.texture = 4;
        def.frame = 0;
        def.frameSpeed = 0;
        def.flags = 0x11;
        def.lifetime = 100;
        def.unk10 = 0xFF;
        def.unk11 = obj->unk90;
        def.rgba[0] = 0;
        def.rgba[1] = 0;
        def.rgba[2] = 0;
        def.rgba[3] = 0xFF;
        def.renderModes = 0x30001;

        randAngle = func_150ADA20();
        randFlipLo = func_150ADA20();
        randFlipHi = func_150ADA20();

        particle_spawnFlatQuadSprite(&def, 0, 2, 0, obj->unk9C, obj->unkA6 + 2, obj->unkA0, obj->unkA2, obj->unkA2,
                      randAngle & 0xFF, ((randFlipHi & 1) << 1) + (randFlipLo & 1), 3, 0xFF, 0, obj->owner,
                      obj->list);
    }
}
