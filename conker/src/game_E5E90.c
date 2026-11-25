#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * Collision handlers of the pooled physics particles (game_15F680.c's D_80089934 / D_80089974 tables).
 * The types below are local copies of layouts worked out elsewhere (fields this file does not use are
 * padding):
 *   PhysicsParticle, ParticleFloor, CollisionProbe - game_15F680.c
 *   ParticleDef, AxisParticle                      - the particle system of game_169510.c
 *   BurstConfig                                    - func_15151A38's argument (game_17CAF0.c)
 *   SurfaceBurstConfig                             - func_15151670's argument (game_17CAF0.c)
 */

/* A particle spawned by func_1513D594: the 0x110-byte particle, its 0x18-byte quad axes, then the
 * caller's motion block. */
typedef struct {
    /* 0x000 */ u8 pad0[0x128];
    /* 0x128 */ f32 motion[2];
} AxisParticle;

/* Ground contact written by func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[8];
} ParticleFloor; /* size 0x24 */

/* A pooled physics particle of game_15F680.c. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ Vec3f rot;        /* the look-at point of the particle's matrix */
    /* 0x02C */ Vec3f scale;
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0xB];
    /* 0x071 */ u8 updateFn;
    /* 0x072 */ u8 update2Fn;
    /* 0x073 */ u8 floorHitFn;
    /* 0x074 */ u8 floorType3Fn;
    /* 0x075 */ u8 wallHitFn;
    /* 0x076 */ u8 destroyFn;
    /* 0x077 */ u8 squashFn;
    /* 0x078 */ u8 matrixFn;
    /* 0x079 */ u8 pad79[0x97];
    /* 0x110 */ ParticleFloor floor;
    /* 0x134 */ Vec3f squashOrigin;
    /* 0x140 */ f32 speed;
    /* 0x144 */ u8 pad144[8];
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8 pad150[0x18];
    /* 0x168 */ s32 unk168;
} PhysicsParticle;

/* func_15151A38's argument: spawns a random count of particles at position. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ s16 yawBase;
    /* 0x2A */ s16 yawRange;
    /* 0x2C */ s16 pitchBase;
    /* 0x2E */ s16 pitchRange;
    /* 0x30 */ s16 fadeBase;
    /* 0x32 */ s16 fadeRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 enabled;
    /* 0x3A */ u8 red;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 params[7];
    /* 0x7C */ u8 params1C;
    /* 0x7D */ u8 params1D;
    /* 0x7E */ u8 pad7E[2];
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 unk84;
} BurstConfig; /* size 0x88 */

extern s32 D_80088734[];
extern s32 D_8008873C;
void *memcpy(void *, const void *, u32);
void func_150A7960(MtxF m, f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_15151A38(BurstConfig *config, u8 arg1, s32 arg2);

/* Floor hit: above speed 30 the particle shatters into dust and is destroyed; above 15 it bounces;
 * otherwise it comes to rest, pointing its matrix 200 units along its current facing. */
s32 func_150B89E0(PhysicsParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface) {
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
            def.unk50 = particle->floor.unk18;
            // matching: the spawned particle reuses the surface parameter; a new local takes another stack slot
            surface = (void *)func_1513D594(&def, &D_800A4AA0, 0x18, 0, 0,
                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, 0, (s32)surface,
                                            particle->unk14C, particle->unk168, 0, 8, particle->unkC,
                                            particle->unk1);
            if (surface != NULL) {
                memcpy(((AxisParticle *)surface)->motion, extra, 8);
            }
            {
                BurstConfig burst;

                burst.position.x = x;
                burst.countBase = 6;
                burst.countRange = 6;
                burst.yawRange = 0xFF;
                burst.pitchBase = -63;
                burst.pitchRange = 29;
                burst.sizeBase = 3.0f;
                burst.sizeRange = 3.0f;
                burst.speedBase = 10.0f;
                burst.speedRange = 10.0f;
                burst.yawBase = 0;
                burst.fadeBase = 3;
                burst.fadeRange = 3;
                burst.lifeBase = 50;
                burst.lifeRange = 20;
                burst.green = 1;
                burst.enabled = 0x21;
                burst.red = 0x48;
                burst.unk3C = 1;
                burst.unk40 = 0;
                burst.unk44 = 0;
                burst.unk48 = 0;
                burst.unk4C = 0;
                burst.unk50 = 0;
                burst.blue = 0;
                burst.alphaBase = 155;
                burst.alphaRange = 100;
                burst.position.y = floorHeight;
                burst.position.z = z;
                burst.growthBase = 0.661f;
                burst.growthRange = 0; // matching: 0.0f would keep the float zero above live in $f20 across the calls
                burst.unk58 = particle->unk14C;
                burst.unk5C = particle->unk168;
                burst.params[0] = 0;
                burst.params[1] = 0x220005;
                burst.params[2] = 0x1D0600;
                burst.params[3] = 1;
                burst.params[4] = 0x3B;
                burst.params[5] = 0x80;
                burst.params[6] = 0x20;
                burst.params1C = 0;
                burst.params1D = 7;
                burst.unk80 = 10;
                burst.unk82 = 25;
                burst.unk84 = particle->floor.unk18;
                func_15151A38(&burst, particle->unkC, particle->unk1);
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

/* The surface triangle a ray hit, as func_150AC9C0 returns it. */
typedef struct {
    s16 coords[9];
} HitTriangle;

/* func_15151670's argument: spawns a random count of particles off a surface triangle, moving inside
 * the cone around its normal. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ Vec3f normal;
    /* 0x18 */ HitTriangle triangle;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 spread;
    /* 0x30 */ f32 sizeBase, sizeRange, growthBase, growthRange, speedBase, speedRange;
    /* 0x48 */ s16 countBase, countRange, fadeBase, fadeRange, lifeBase, lifeRange;
    /* 0x54 */ u8 green, enabled, red, pad57;
    /* 0x58 */ s32 unk58[6];
    /* 0x70 */ u8 blue, alphaBase, alphaRange, pad73;
    /* 0x74 */ s32 unk74, unk78;
    /* 0x7C */ s32 params[7];
    /* 0x98 */ u8 params1C, params1D, pad9A[2];
    /* 0x9C */ s16 unk9C, unk9E;
    /* 0xA0 */ s32 unkA0;
} SurfaceBurstConfig; /* size 0xA4 */

/* Wall-collision query of game_15F680.c (only its position is read here). */
typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ Vec3f pos;
} CollisionProbe;

s32 func_150A3058(s32, s16, s16, s16);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *,
                  s32 *, void *, f32);
s32 func_15145C90(s32);
void func_15151670(SurfaceBurstConfig *config, u8 arg1, s32 arg2);

/* Wall hit (x, y, z: the position before the move): a hard hit casts a ray along the move, puffs dust
 * off the surface it finds and destroys the particle; a soft one stops it against the wall. */
s32 func_150B8F44(PhysicsParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z) {
    f32 impactSpeed = particle->speed - fabsf(particle->vel.y); /* speed less the vertical part */

    if (impactSpeed > 6.0f) {
        if (func_150A3058(1, probe->pos.x, probe->pos.y, probe->pos.z) == 0) {
            Vec3f move;
            Vec3f hit;
            HitTriangle triangle;
            s32 hitObject;
            ParticleDef def;
            AxisParticle *spawned;
            f32 extra;

            move.x = particle->pos.x - x;
            move.y = particle->pos.y - y;
            move.z = particle->pos.z - z;
            if (func_150AC9C0(x, y, z, move.x, move.y, move.z, NULL, triangle.coords, &hit.x, &hit.y, &hit.z,
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
                    def.unk50 = particle->floor.unk18;
                    spawned = (AxisParticle *)func_1513D594(&def, &D_800A4AA0, 0x18, 0, 0,
                                                            (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                                            func_150ADA20() & 0xFF, 500.0f, 500.0f, 0,
                                                            (s32)&triangle, particle->unk14C, particle->unk168, 0,
                                                            8, particle->unkC, particle->unk1);
                    if (spawned != NULL) {
                        /* the 8-byte motion block; 4 bytes past extra are copied too */
                        memcpy(spawned->motion, &extra, 8);
                    }
                    break;
                }
                {
                    SurfaceBurstConfig burst;

                    burst.normal.x = -move.x;
                    burst.normal.y = -move.y;
                    burst.normal.z = -move.z;
                    burst.triangle = triangle;
                    burst.spread = 502.0f;
                    burst.position = hit;
                    burst.sizeBase = 3.0f;
                    burst.sizeRange = 3.0f;
                    burst.growthBase = 0.2650000155f; // not 0.265f: one ulp above it
                    burst.growthRange = 0.1610000134f; // not 0.161f: one ulp above it
                    burst.speedBase = 0.005399999674f; // not 0.0054f: one ulp below it
                    burst.speedRange = 0.01349999942f; // not 0.0135f: one ulp below it
                    burst.countBase = 6;
                    burst.countRange = 6;
                    burst.fadeBase = 3;
                    burst.fadeRange = 3;
                    burst.lifeBase = 50;
                    burst.lifeRange = 20;
                    burst.green = 1;
                    burst.enabled = 0x21;
                    burst.red = 0x48;
                    burst.unk58[0] = 1;
                    burst.unk58[1] = 0;
                    burst.unk58[2] = 0;
                    burst.unk58[3] = 0;
                    burst.unk58[4] = 0;
                    burst.unk58[5] = 0;
                    burst.blue = 0;
                    burst.alphaBase = 155;
                    burst.alphaRange = 100;
                    burst.unk74 = particle->unk14C;
                    burst.unk78 = particle->unk168;
                    burst.params[0] = 0;
                    burst.params[1] = 0x220005;
                    burst.params[2] = 0x1D0600;
                    burst.params[3] = 1;
                    burst.params[4] = 0x3B;
                    burst.params[5] = 0x80;
                    burst.params[6] = 0x20;
                    burst.params1C = 0;
                    burst.params1D = 7;
                    burst.unk9C = 10;
                    burst.unk9E = 25;
                    burst.unkA0 = particle->floor.unk18;
                    func_15151670(&burst, particle->unkC, particle->unk1);
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
