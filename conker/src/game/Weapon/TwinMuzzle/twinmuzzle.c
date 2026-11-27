#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/light.h"
#include "conker/weapon.h"

/* Points on bone 2 that give the aim: its start and a point further along it. */
static const Vec3f gTwinAimStart = { -40.0f, 0.0f, 57.0f };
static const Vec3f gTwinAimPoint = { -40.0f, 0.0f, 160.0f };
/* Per muzzle: the beam start and end on its bone, and the bone. */
static const Vec3f gTwinMuzzleStarts[2] = { { 17.0f, 0.0f, 570.0f }, { -17.0f, 0.0f, 570.0f } };
static const Vec3f gTwinMuzzleBeamEnds[2] = { { 17.0f, 0.0f, 700.0f }, { -17.0f, 0.0f, 700.0f } };
static const u8 gTwinMuzzleBones[2] = { 2, 4 };

/* Fires a shot from muzzle `slot` (0 or 1) of `actor`: casts a ray along the aim given by bone 2, spawns a
 * weapon_spawnProjectile effect from the muzzle towards the jittered hit point (or along the aim on a miss), a
 * light_createWithEffect flash at the muzzle and a effect_spawnBoneBeamParticle beam on the muzzle's bone. */
Game1EF500Trail *weapon_fireTwinMuzzleShot(Actor *actor, u8 slot, u8 arg2, s32 arg3) {
    Game1EF500Trail *tracer;
    Vec3f aimStart;
    Vec3f aimDir;
    Vec3f muzzle;
    Vec3f *tracerDir;
    Vec3f hitDir;
    const Vec3f *srcs[2];
    Vec3f *dsts[2];
    const Vec3f *muzzleSrc[1];
    Vec3f *muzzleDst[1];
    Vec3f hit;
    f32 jitter[2];
    LightEffectHeader flashHeader;
    Vec3i flashPos;
    ParticleSize beamSize;
    f32 randB;
    s32 randInt;
    f32 randA;
    s32 randInt2;

    if (actor == NULL) {
        return NULL;
    }
    if (slot >= 2) {
        return NULL;
    }
    if (actor->ob_matrix_ptr == NULL) {
        return NULL;
    }
    srcs[0] = &gTwinAimStart;
    srcs[1] = &gTwinAimPoint;
    dsts[0] = &aimStart;
    dsts[1] = &aimDir;
    math_transformOffsets(srcs, dsts, actor->ob_matrix_ptr + 2, 2);
    aimDir.x -= aimStart.x;
    aimDir.y -= aimStart.y;
    aimDir.z -= aimStart.z;
    {
        /* matching: the block (needing a declaration, so a prototype) fences as1's scheduling */
        void math_transformOffsets(const Vec3f **, Vec3f **, void *, s32);

        muzzleSrc[0] = &gTwinMuzzleStarts[slot];
        muzzleDst[0] = &muzzle;
        math_transformOffsets(muzzleSrc, muzzleDst, actor->ob_matrix_ptr + gTwinMuzzleBones[slot], 1);
    }
    if (collision_castRayWorld(aimStart.x, aimStart.y, aimStart.z, aimDir.x, aimDir.y, aimDir.z, NULL,
                      NULL, &hit.x, &hit.y, &hit.z, NULL, NULL, NULL, 0.0f)) {
        randInt = math_random();
        randA = math_randomFloat() * 80.0f;
        math_dirFromBinAngleXZ(randInt & 0xFF, randA, &jitter[0], &jitter[1]);
        {
            /* matching: the block (needing a declaration, so a prototype) fences as1's scheduling */
            void math_dirFromBinAngleXZ(s16, f32, f32 *, f32 *);

            hit.x += jitter[0];
            hit.z += jitter[1];
            tracerDir = &hitDir;
            hitDir.x = hit.x - muzzle.x;
            hitDir.y = hit.y - muzzle.y;
            hitDir.z = hit.z - muzzle.z;
        }
    } else {
        tracerDir = &aimDir;
    }
    {
        /* matching: the block (needing a declaration, so a prototype) orders the stores to tracer */
        f32 math_randomFloat(void);

        randA = math_randomFloat();
        randB = math_randomFloat();
        math_random();
        tracer = weapon_spawnProjectile(&muzzle, tracerDir, 0, 0, 0, 0, 130.0f, 1.0f / 130.0f, randA * 18.0f + 29.0f,
                               randB * 400.0f + 262.0f, 100.0f, 255, actor, 1, 1, 0, 0, 0, 1, 3, 26, 0.0f, 255, -1,
                               0, arg2, arg3);
    }
    flashHeader.flags = 3;
    flashHeader.update = -1;
    flashHeader.lifetime = (math_random() % 3U) + 3;
    flashHeader.lightType = 0;
    flashPos.x = muzzle.x;
    flashPos.y = muzzle.y;
    flashPos.z = muzzle.z;
    randInt = (math_random() % 13U) + 90;
    light_createWithEffect(&flashHeader, &flashPos, randInt, 255, 255, 255, 255, 0, 0, arg2, arg3);
    beamSize.height = (math_randomFloat() * 2586.0f + 1758.0f) * 0.001000000047f;
    beamSize.width = (math_randomFloat() * 1357.0f + 968.0f) * 0.1000000015f;
    randInt2 = math_random();
    randInt = math_random();
    randA = math_randomFloat() * 1077.0f + 4000.0f;
    effect_spawnBoneBeamParticle(actor, gTwinMuzzleBones[slot], &gTwinMuzzleStarts[slot], &gTwinMuzzleBeamEnds[slot], &beamSize, (randInt2 % 6U) + 6,
                  (randInt % 52U) + 193, randA, &muzzle, 255, 0, -1, arg2, arg3);
    return tracer;
}

/* Returns whether `actor` is not `self`, is active (nonzero type) and has one of the ids 0-4, 40
 * or 119. */
s32 func_150EC3D4(Actor *actor, Actor *self) {
    s32 id;

    if (actor == self) {
        return FALSE;
    }
    if (actor->type == 0) {
        return FALSE;
    }
    id = actor->character_type;
    if (id == 0xFF) {
        return FALSE;
    }
    if (id == 0 || id == 1 || id == 2 || id == 3 || id == 4 || id == 40 || id == 119) {
        return TRUE;
    }
    return FALSE;
}

/* Particle callback (table D_8008FBD0): weapon_spawnHitEffects with 1.0f, 1.0f, 1.0f, 0.0f and 0xFF masks. */
void func_150EC45C(void *particle) {
    weapon_spawnHitEffects(particle, 1.0f, 1.0f, 1.0f, 0.0f, 0xFF, 0xFF, 0xFF);
}
