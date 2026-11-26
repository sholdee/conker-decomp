#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Collision handlers of the pooled physics particles (game_15F680.c's D_80089934 / D_80089974 tables).
 * The particles are ModelParticles (effects.h). The types below are local copies of layouts worked out
 * elsewhere (fields this file does not use are padding):
 *   CollisionProbe - game_15F680.c
 *   AxisParticle   - the particle system of game_169510.c
 */

/* A particle spawned by func_1513D594: the 0x110-byte particle, its 0x18-byte quad axes, then the
 * caller's motion block. */
typedef struct {
    /* 0x000 */ u8 pad0[0x128];
    /* 0x128 */ f32 motion[2];
} AxisParticle;

extern s32 D_80088734[];
extern s32 D_8008873C;
void *memcpy(void *, const void *, u32);

/* Floor hit: above speed 30 the particle shatters into dust and is destroyed; above 15 it bounces;
 * otherwise it comes to rest, pointing its matrix 200 units along its current facing. */
s32 func_150B89E0(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface) {
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
            surface = (void *)func_1513D594(&def, &D_800A4AA0, 0x18, 0, 0,
                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, 0, (s32)surface,
                                            particle->unk14C, particle->unk168, 0, 8, particle->header.owner,
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
                burst.unk58 = particle->unk14C;
                burst.unk5C = particle->unk168;
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
                func_15151A38(&burst, particle->header.owner, particle->header.kind);
            }
            func_10010F88(D_80088734[func_150ADA20() & 1], 32000, 0, 0, 0, particle->pos.x, floorHeight + 25.0f,
                          particle->pos.z, 1000, 4000);
            return 0;
        }
        particle->vel.x *= particle->elasticity;
        particle->vel.y = -particle->vel.y * particle->elasticity;
        particle->vel.z *= particle->elasticity;
        particle->rotVel.x *= particle->elasticity;
        particle->rotVel.y *= particle->elasticity;
        particle->rotVel.z *= particle->elasticity;
        func_10010F88(D_8008873C, 32000, 0, 0, 0, particle->pos.x, particle->pos.y, particle->pos.z, 1000, 4000);
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
s32 func_150B8F44(ModelParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z) {
    f32 impactSpeed = particle->speed - fabsf(particle->vel.y); /* speed less the vertical part */

    if (impactSpeed > 6.0f) {
        if (func_150A3058(1, probe->pos.x, probe->pos.y, probe->pos.z) == 0) {
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
                switch (func_15145C90(hitObject)) {
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
                    spawned = (AxisParticle *)func_1513D594(&def, &D_800A4AA0, 0x18, 0, 0,
                                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, 0,
                                                            (s32)&triangle, particle->unk14C, particle->unk168, 0,
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
                    burst.unk74 = particle->unk14C;
                    burst.unk78 = particle->unk168;
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
                    func_15151670(&burst, particle->header.owner, particle->header.kind);
                }
            }
        }
        func_10010F88(D_80088734[func_150ADA20() & 1], 32000, 0, 0, 0, particle->pos.x, particle->pos.y,
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
    func_10010F88(D_8008873C, 64000, 0, 0, 0, probe->pos.x, probe->pos.y, probe->pos.z, 1000, 4000);
    return 1;
}
