#include <ultra64.h>
// matching: functions.h declares func_150ADA20 as returning u8, but this file was built against an
// int-returning prototype, so hide the shared one.
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

/*
 * Collision handlers of the pooled physics particles (game_15F680.c's D_80089934 / D_80089974 tables).
 * The types below are local copies of layouts worked out elsewhere (fields this file does not use are
 * padding):
 *   PhysicsParticle, ParticleFloor, CollisionProbe - game_15F680.c
 *   ParticleDef, AxisParticle                      - the particle system of game_169510.c
 *   BurstConfig                                    - func_15151A38's argument (game_17CAF0.c)
 *   SurfaceBurstConfig                             - func_15151670's argument (game_17CAF0.c)
 */

/* Spawn template of a particle (func_1513D594's def). */
typedef struct {
    /* 0x00 */ u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;
    /* 0x02 */ u16 renderModes;    /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x08 */ s32 frame;          /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ vertex pos;
    /* 0x28 */ vertex rot;
    /* 0x34 */ vertex scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
    /* 0x4C */ u8 viewportMask;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 fadeStart;      /* lifetime below which the particle fades */
    /* 0x56 */ s16 fadeRate;
} ParticleDef; /* size 0x58 */

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
    /* 0x020 */ vertex rot;        /* the look-at point of the particle's matrix */
    /* 0x02C */ vertex scale;
    /* 0x038 */ vertex pos;
    /* 0x044 */ vertex vel;
    /* 0x050 */ vertex rotVel;
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
    /* 0x134 */ vertex squashOrigin;
    /* 0x140 */ f32 speed;
    /* 0x144 */ u8 pad144[8];
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8 pad150[0x18];
    /* 0x168 */ s32 unk168;
} PhysicsParticle;

/* func_15151A38's argument: spawns a random count of particles at position. */
typedef struct {
    /* 0x00 */ vertex position;
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
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
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
            surface = (void *)func_1513D594((s32)&def, (s32)&D_800A4AA0, 0x18, 0, 0,
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
                burst.alphaBase = 0x9B;
                burst.alphaRange = 0x64;
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
        f32 mtx[4][4];

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

typedef struct {
    f32 x, y, z;
} Vec3_150B8F44;

typedef struct {
    s16 data[9];
} Surface150B8F44;

typedef struct {
    u8 kind;
    u8 field01;
    s16 mode;
    s16 lifetime;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 color[4];
    f32 scale14;
    f32 scale18;
    Vec3_150B8F44 position;
    Vec3_150B8F44 velocity;
    Vec3_150B8F44 acceleration;
    s32 flags;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    u8 pad48[8];
    s32 field50;
    s16 field54;
    s16 field56;
} Desc150B8F44;

typedef struct {
    u8 pad000[0x128];
    f32 field128[2];
} Particle150B8F44;

typedef struct {
    Vec3_150B8F44 position;
    Vec3_150B8F44 normal;
    Surface150B8F44 surface;
    u8 pad2A[2];
    f32 spread;
    f32 sizeBase, sizeRange, growthBase, growthRange, speedBase, speedRange;
    s16 countBase, countRange, fadeBase, fadeRange, lifeBase, lifeRange;
    u8 green, enabled, red, pad57;
    s32 callbacks[6];
    u8 blue, alphaBase, alphaRange, pad73;
    s32 field74, field78;
    s32 flags[7];
    u8 field98, field99, pad9A[2];
    s16 field9C, field9E;
    s32 fieldA0;
} SBurst150B8F44;

typedef struct {
    u8 pad00[0x14];
    Vec3_150B8F44 position;
} Target150B8F44;

s32 func_150A3058(s32, s16, s16, s16);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *,
                  s32 *, void *, f32);
s32 func_15145C90(s32);
void func_15151670(SBurst150B8F44 *, u8, s32);

s32 func_150B8F44(PhysicsParticle *arg0, Target150B8F44 *arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 height;

    height = arg0->speed - fabsf(arg0->vel.y);
    if (height > 6.0f) {
        if (func_150A3058(1, arg1->position.x, arg1->position.y, arg1->position.z) == 0) {
            Vec3_150B8F44 delta;
            Vec3_150B8F44 hit;
            Surface150B8F44 surface;
            s32 id;
            Desc150B8F44 descriptor;
            Particle150B8F44 *object;
            f32 rate;

            delta.x = arg0->pos.x - arg2;
            delta.y = arg0->pos.y - arg3;
            delta.z = arg0->pos.z - arg4;
            if (func_150AC9C0(arg2, arg3, arg4, delta.x, delta.y, delta.z, NULL, surface.data,
                              &hit.x, &hit.y, &hit.z, NULL, &id, NULL, 0.0f) != 0) {
                switch (func_15145C90(id)) {
                case 0:
                    break;
                default:
                    descriptor.field54 = 0x19;
                    descriptor.field56 = 0xA;
                    descriptor.kind = 4;
                    descriptor.field01 = 0;
                    descriptor.mode = 0x3B02;
                    descriptor.lifetime = (func_150ADA20() % 31U) + 0x28;
                    descriptor.field08 = 0;
                    descriptor.field0C = 0;
                    descriptor.color[0] = 0xFF;
                    descriptor.color[1] = 0xFF;
                    descriptor.color[2] = 0xFF;
                    descriptor.color[3] = 0xFF;
                    descriptor.scale14 = descriptor.scale18 =
                        (func_150ADA68() * 25.0f + 25.0f) * 0.001000000047f;
                    descriptor.position = hit;
                    descriptor.flags = 0x0C5C0001;
                    descriptor.field44 = 0xFF;
                    descriptor.field45 = 0xFF;
                    descriptor.field46 = 0;
                    descriptor.field47 = 7;
                    descriptor.velocity.x = 0.0f;
                    descriptor.velocity.y = 0.0f;
                    descriptor.velocity.z = 0.0f;
                    descriptor.acceleration.x = 0.0f;
                    descriptor.acceleration.y = 0.0f;
                    descriptor.acceleration.z = 0.0f;
                    rate = (func_150ADA68() * 80.0f + 50.0f) * 9.999999747e-06f;
                    descriptor.field50 = arg0->floor.unk18;
                    object = (Particle150B8F44 *)func_1513D594((s32)&descriptor, (s32)&D_800A4AA0, 0x18, 0, 0,
                                           (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1),
                                           func_150ADA20() & 0xFF, 500.0f, 500.0f, 0,
                                           (s32)&surface, arg0->unk14C, arg0->unk168, 0, 8,
                                           arg0->unkC, arg0->unk1);
                    if (object != NULL) {
                        memcpy(object->field128, &rate, 8);
                    }
                    break;
                }
                {
                    SBurst150B8F44 burst;

                    burst.normal.x = -delta.x;
                    burst.normal.y = -delta.y;
                    burst.normal.z = -delta.z;
                    burst.surface = surface;
                    burst.spread = 502.0f;
                    burst.position = hit;
                    burst.sizeBase = 3.0f;
                    burst.sizeRange = 3.0f;
                    burst.growthBase = 0.2650000155f;
                    burst.growthRange = 0.1610000134f;
                    burst.speedBase = 0.005399999674f;
                    burst.speedRange = 0.01349999942f;
                    burst.countBase = 6;
                    burst.countRange = 6;
                    burst.fadeBase = 3;
                    burst.fadeRange = 3;
                    burst.lifeBase = 0x32;
                    burst.lifeRange = 0x14;
                    burst.green = 1;
                    burst.enabled = 0x21;
                    burst.red = 0x48;
                    burst.callbacks[0] = 1;
                    burst.callbacks[1] = 0;
                    burst.callbacks[2] = 0;
                    burst.callbacks[3] = 0;
                    burst.callbacks[4] = 0;
                    burst.callbacks[5] = 0;
                    burst.blue = 0;
                    burst.alphaBase = 0x9B;
                    burst.alphaRange = 0x64;
                    burst.field74 = arg0->unk14C;
                    burst.field78 = arg0->unk168;
                    burst.flags[0] = 0;
                    burst.flags[1] = 0x220005;
                    burst.flags[2] = 0x1D0600;
                    burst.flags[3] = 1;
                    burst.flags[4] = 0x3B;
                    burst.flags[5] = 0x80;
                    burst.flags[6] = 0x20;
                    burst.field98 = 0;
                    burst.field99 = 7;
                    burst.field9C = 0xA;
                    burst.field9E = 0x19;
                    burst.fieldA0 = arg0->floor.unk18;
                    func_15151670(&burst, arg0->unkC, arg0->unk1);
                }
            }
        }
        func_10010F88(D_80088734[func_150ADA20() & 1], 0x7D00, 0, 0, 0,
                      arg0->pos.x, arg0->pos.y, arg0->pos.z, 1000, 4000);
        return 0;
    }
    arg0->vel.x = 0.0f;
    arg0->vel.z = 0.0f;
    if (arg0->vel.y > 0.0f) {
        arg0->vel.y *= arg0->elasticity;
    }
    arg0->pos.x = arg1->position.x;
    arg0->pos.y = arg1->position.y;
    arg0->pos.z = arg1->position.z;
    arg0->rotVel.x *= arg0->elasticity;
    arg0->rotVel.y *= arg0->elasticity;
    arg0->rotVel.z *= arg0->elasticity;
    func_10010F88(D_8008873C, 0xFA00, 0, 0, 0, arg1->position.x, arg1->position.y,
                  arg1->position.z, 1000, 4000);
    return 1;
}
