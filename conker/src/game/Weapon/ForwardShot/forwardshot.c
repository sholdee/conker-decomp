#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/weapon.h"

/* This file's .rodata: the projectile's start point, the end of its flash beam, its origin and its
 * direction point, in the local space of bone gForwardShotBone. */
static const Vec3f gForwardShotStart = { 10.0f, -102.0f, 75.0f };
static const Vec3f gForwardShotBeamEnd = { 10.0f, -151.0f, 105.0f };
static const Vec3f gForwardShotOrigin = { -1.0f, -38.0f, 38.0f };
static const Vec3f gForwardShotDirPoint = { -26.0f, -23.0f, 51.0f };

/* This file's .data. */
static u8 gForwardShotBone = 9; // bone of the points above

/* Spawns a projectile (weapon_spawnProjectile) along `actor`'s direction. With bone matrices it starts at
 * bone gForwardShotBone and is followed by the particle_spawnShortLightFlash / particle_spawnBoneDirParticle / weapon_spawnBoneBeam effects;
 * without them it starts 34 units to the side of and 49 units above the actor. */
void weapon_spawnForwardProjectile(Actor *actor, u8 fxOwner, s32 fxList) {
    Vec3f position;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    Vec3f origin;
    Vec3f delta;
    f32 sideZ;
    f32 sideX;
    void *mtx;

    particle_countShot(actor);
    math_getActorAim(actor, &direction, &axis0, &axis1, 0.8110000491f);
    if (actor->ob_matrix_ptr != NULL) {
        const Vec3f *sources[3];
        Vec3f *destinations[3];

        mtx = actor->ob_matrix_ptr[gForwardShotBone];
        sources[0] = &gForwardShotStart;
        sources[1] = &gForwardShotOrigin;
        sources[2] = &gForwardShotDirPoint;
        destinations[0] = &position;
        destinations[1] = &origin;
        destinations[2] = &delta;
        math_transformOffsets(sources, destinations, mtx, 3);
        delta.x -= origin.x;
        delta.y -= origin.y;
        delta.z -= origin.z;
    } else {
        f32 x = direction.x;
        f32 z = direction.z;

        mtx = NULL;
        if (0.0001f < fabsf(x) || 0.0001f < fabsf(z)) {
            f32 length = sqrtf(x * x + z * z);
            f32 inverse = 1.0f / length;

            sideZ = z * inverse;
            sideX = -x * inverse;
        } else {
            sideZ = 1.0f;
            sideX = 0.0f;
        }
        position.x = actor->xpos + 34.0f * sideX;
        position.y = actor->ypos + 49.0f;
        position.z = actor->zpos + 34.0f * sideZ;
    }
    weapon_spawnProjectile(&position, &direction, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                  math_randomFloat() * 10.0f + 25.0f, math_randomFloat() * 200.0f + 400.0f, 50.0f,
                  (math_random() % 156U) + 100, actor, 1, 1, 0, 0, 1, 1, 0, 0x35, 0.0f, 0xFF, -1, 0,
                  fxOwner, fxList);
    if (actor->ob_matrix_ptr != NULL && (actor->cullMask & 0xF) != 0xF) {
        particle_spawnShortLightFlash(&position, fxOwner, fxList);
        particle_spawnBoneDirParticle(&origin, &delta, mtx, actor, 1.0f, fxOwner, fxList);
        if (math_random() & 1) {
            weapon_spawnBoneBeam(actor, gForwardShotBone, &gForwardShotStart, &gForwardShotBeamEnd, &position, fxOwner, fxList);
        }
    }
}

/* Transforms the point gForwardShotStart by bone gForwardShotBone of `actor` into `*out`. */
void weapon_getForwardProjectileStart(Actor *actor, s16 unused, Vec3f *out) {
    void *mtx;
    const Vec3f *src;
    Vec3f *dst;

    mtx = actor->ob_matrix_ptr[gForwardShotBone];
    src = &gForwardShotStart;
    dst = out;
    math_transformOffsets(&src, &dst, mtx, 1);
}

/* Spawns a randomized effect (effect_spawnBoneBeamParticle) on bone `bone` of `actor` at `position`. */
void weapon_spawnBoneBeam(Actor *actor, u8 bone, const Vec3f *start, const Vec3f *end, Vec3f *position, u8 fxOwner, s32 fxList) {
    ParticleSize motion;
    s32 pad; // matching: unused stack slot between motion and random
    s32 random[2];

    motion.height = math_randomFloat() * 1.001f + 0.6040000319f;
    motion.width = math_randomFloat() * 5.9f + 14.1f;
    random[0] = math_random();
    random[1] = math_random();
    effect_spawnBoneBeamParticle(actor, bone, start, end, &motion, (random[0] & 3) + 6, 0xFF, math_randomFloat() * 270.0f + 359.0f,
                  position, 0xFF, 0, -1, fxOwner, fxList);
}
