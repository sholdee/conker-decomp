#include <ultra64.h>
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/weapon.h"

/* This file's .rodata: the ends of a bone beam that func_15071888 (game/Event/TimedEvent/timedevent.c) draws, then the effect
 * points of this file, in the local space of bone D_80088B94. */
const Vec3f D_800A1FB0 = { 0.0f, -28.0f, 24.0f };
const Vec3f D_800A1FBC = { 0.0f, 0.0f, 24.0f };
static const Vec3f D_800A1FC8 = { 0.0f, -155.0f, 27.0f };
static const Vec3f D_800A1FD4 = { 0.0f, 10.0f, 27.0f };
static const Vec3f D_800A1FE0 = { 0.0f, -210.0f, 27.0f };

/* This file's .data: the bones of the two point sets above. */
u8 D_80088B90 = 9;
static u8 D_80088B94 = 9;

/* Runs the effect_spawnBonePointShotFx effects for `obj`, then spawns a weapon_spawnProjectile effect from the result. */
WeaponProjectile *effect_fireBonePointProjectile(Actor *obj, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    Vec3f axis0;
    Vec3f position;
    Vec3f origin;
    Vec3f direction;
    u8 randomInt;
    f32 random1;
    f32 random0;

    if (obj == NULL) {
        return NULL;
    }

    math_getActorAim(obj, &direction, &axis0, NULL, 0.0f);
    effect_spawnBonePointShotFx(obj, arg1, arg2, &position, &origin);

    random0 = math_randomFloat();
    random1 = math_randomFloat();
    randomInt = math_randomU8();

    return weapon_spawnProjectile(&origin, &direction, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                         random0 * 10.0f + 25.0f,
                         random1 * 200.0f + 600.0f, 50.0f,
                         (randomInt % 56U) + 200, obj, 1, 1, 0, 0xFF, 1, 1,
                         0, 0x23, 0.0f, 0xFF, -1, 0, arg1, arg2);
}

/*
 * Computes the effect position and origin of `obj` (its bone points D_800A1FC8 / D_800A1FD4, or 70 units
 * above it when it has no bone matrices), runs the effects there and copies both to the non-NULL outputs.
 */
void effect_spawnBonePointShotFx(Actor *obj, u8 arg1, s32 arg2, Vec3f *outPosition, Vec3f *outOrigin) {
    Vec3f position;
    Vec3f origin;
    const Vec3f *src[2];
    Vec3f *dst[2];

    if (obj != NULL) {
        if (obj->ob_matrix_ptr != NULL) {
            src[0] = &D_800A1FC8;
            src[1] = &D_800A1FD4;
            dst[0] = &position;
            dst[1] = &origin;
            math_transformOffsets(src, dst, obj->ob_matrix_ptr + D_80088B94, 2);
        } else {
            position.x = obj->xpos;
            position.y = obj->ypos + 70.0f;
            position.z = obj->zpos;
            origin = position;
        }
        particle_runAttackAtMidpoint(obj, &position, &origin, 0x23);
        particle_spawnShortLightFlash(&position, arg1, arg2);
        effect_spawnBonePointBeam(obj, &position, arg1, arg2);
        effect_spawnActorRumble(obj);
        if (outPosition != NULL) {
            *outPosition = position;
        }
        if (outOrigin != NULL) {
            *outOrigin = origin;
        }
    }
}

/*
 * Spawns a effect_spawnBoneBeamParticle effect for `obj` between its bone points D_800A1FC8 and D_800A1FE0, from `position`;
 * when the object has bone matrices and cullMask's low nibble isn't 0xF.
 */
void effect_spawnBonePointBeam(Actor *obj, Vec3f *position, u8 arg2, s32 arg3) {
    ParticleSize motion;
    u8 mask;
    s32 pad; // matching: unused stack slot
    s32 random1;
    s32 random0;

    if (obj->ob_matrix_ptr != NULL) {
        if ((obj->cullMask & 0xF) != 0xF) {
            motion.height = math_randomFloat() * (1.0f + 1.0f) + 3.7099998f; // matching: a plain 2.0f literal mismatches
            motion.width = math_randomFloat() * 15.0f + 35.0f;

            if ((obj->mv != NULL) && (obj->mv->SniperState != 0) && (obj->CameraPtr != NULL)) {
                mask = ~(1 << obj->CameraPtr->CNumber) & 0xFF;
            } else {
                mask = 0xFF;
            }

            random0 = math_randomU8();
            random1 = math_randomU8();
            effect_spawnBoneBeamParticle(obj, D_80088B94, &D_800A1FC8, &D_800A1FE0, &motion,
                          (random0 % 3U) + 4, (random1 % 56U) + 200,
                          math_randomFloat() * 301.0f + 500.0f, position, mask,
                          0, -1, arg2, arg3);
        }
    }
}

/* Handler: writes the world position of `obj`'s bone point D_800A1FC8 to `out`. */
void effect_getBonePointPos(Actor *obj, s16 arg1, Vec3f *out) {
    particle_getBonePoint(obj, out, &D_800A1FC8, D_80088B94, 0.0f);
}


/* Sends a cont_spawnRumble event (kind 1, 20-32 long) to `obj`'s camera, if it has one. */
void effect_spawnActorRumble(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (math_randomU8() % 13U) + 20;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        math_randomU8(); // draws a random number that is never used
        event.strength = 8;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 1);
    }
}
