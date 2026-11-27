#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata: the muzzle point, the end of the muzzle flash beam and the launch point, in bone space. */
static const Vec3f gLauncherMuzzle = { 7.0f, -136.0f, 45.0f };
static const Vec3f gLauncherFlashEnd = { 7.0f, -169.0f, 51.0f };
static const Vec3f gLauncherLaunchPoint = { 7.0f, 40.0f, 13.0f };

/* This file's .data. */
static u8 gLauncherBone = 9; // bone of the muzzle points above

/* Fires a projectile (weapon_spawnProjectile) from obj's launch point towards its target, after the muzzle
 * effects of weapon_runLauncherMuzzle. Returns weapon_spawnProjectile's result, or NULL without an obj. */
// matching: arg1 is a u8 read from its stack slot at each use; a K&R u8 parameter is
// narrowed once into a spilled copy instead
Game1EF500Trail *weapon_fireLauncherProjectile(Actor *obj, s32 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    Vec3f dir;
    Vec3f muzzle;
    Vec3f target;
    Vec3f launch;
    u32 random;
    s32 kind;
    f32 rand1;
    f32 rand0;

    if (obj == NULL) {
        return NULL;
    }

    math_getActorAim(obj, &target, &dir, NULL, 0.0f);
    weapon_runLauncherMuzzle(obj, ((u8 *)&arg1)[3], arg2, &muzzle, &launch);

    rand0 = math_randomFloat();
    rand1 = math_randomFloat();
    random = math_random();
    kind = (level == 0x2B) ? 0x27 : 0x1A;

    return weapon_spawnProjectile(&launch, NULL, &launch, &target, 0, 0, 300.0f, 0.003333333414f, rand0 * 10.0f + 25.0f,
                         rand1 * 201.0f + 601.0f, 50.0f, (random % 56U) + 200, obj, 1, 1, 0, 0xFF, 1, 1, 0, kind,
                         0.0f, 0xFF, -1, 0, ((u8 *)&arg1)[3], arg2);
}

/* Computes obj's muzzle and launch points (bone gLauncherBone, or 56 above its origin without bone
 * matrices), runs the muzzle effects there and optionally returns both points. */
void weapon_runLauncherMuzzle(obj, arg1, arg2, muzzleOut, launchOut)
    Actor *obj;
    u8 arg1;
    s32 arg2;
    Vec3f *muzzleOut;
    Vec3f *launchOut;
{
    Vec3f muzzle;
    Vec3f launch;
    const Vec3f *src[2];
    Vec3f *dst[2];

    if (obj != NULL) {
        if (obj->ob_matrix_ptr != NULL) {
            src[0] = &gLauncherMuzzle;
            src[1] = &gLauncherLaunchPoint;
            dst[0] = &muzzle;
            dst[1] = &launch;
            math_transformOffsets(src, dst, obj->ob_matrix_ptr + gLauncherBone, 2);
        } else {
            muzzle.x = obj->xpos;
            muzzle.y = obj->ypos + 56.0f;
            muzzle.z = obj->zpos;
            launch = muzzle;
        }
        particle_runAttackAtMidpoint(obj, &muzzle, &launch, (level == 0x2B) ? 0x28 : 0x1E);
        particle_spawnShortLightFlash(&muzzle, arg1, arg2);
        weapon_spawnLauncherFlash(obj, &muzzle, arg1, arg2);
        weapon_rumbleLauncher(obj);
        if (muzzleOut != NULL) {
            *muzzleOut = muzzle;
        }
        if (launchOut != NULL) {
            *launchOut = launch;
        }
    }
}

/* Muzzle effect (effect_spawnBoneBeamParticle) at `muzzle` for objects with bone matrices, hidden from the
 * camera of obj's mv target while that has SniperState set. */
void weapon_spawnLauncherFlash(obj, muzzle, arg2, arg3)
    Actor *obj;
    Vec3f *muzzle;
    u8 arg2;
    s32 arg3;
{
    ParticleSize params;
    u8 cameraMask;
    s32 pad; // matching: unused stack slot
    s32 random1;
    s32 random0;

    if (obj->ob_matrix_ptr != NULL && (obj->cullMask & 0xF) != 0xF) {
        params.height = math_randomFloat() * 1.5f + 2.0f;
        params.width = math_randomFloat() * 10.10000038f + 2.059999943f;

        if (obj->mv != NULL && obj->mv->SniperState != 0 && obj->CameraPtr != NULL) {
            cameraMask = ~(1 << obj->CameraPtr->CNumber) & 0xFF;
        } else {
            cameraMask = 0xFF;
        }

        random0 = math_random();
        random1 = math_random();
        effect_spawnBoneBeamParticle(obj, gLauncherBone, &gLauncherMuzzle, &gLauncherFlashEnd, &params, (random0 % 5U) + 6, (random1 % 101U) + 155,
                      math_randomFloat() * 199.0f + 501.0f, muzzle, cameraMask, 0, -1, arg2, arg3);
    }
}

/* Handler: writes the world position of obj's muzzle point to `out`. */
void weapon_getLauncherMuzzlePos(Actor *obj, s16 arg1, Vec3f *out) {
    particle_getBonePoint(obj, out, &gLauncherMuzzle, gLauncherBone, 0.0f);
}

/* Sends a cont_spawnRumble event (type 1, random duration 15-23) to the camera of obj. */
void weapon_rumbleLauncher(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (math_random() % 9U) + 15;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        event.strength = (math_random() & 3) + 3;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 1);
    }
}
