#include <ultra64.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/weapon.h"


/* Muzzle points (from, to) of this gun on bone gGunMuzzleBone (gGunMuzzlePoints points to the first). */
const Vec3f gGunMuzzleStart = { 10.0f, -121.0f, 81.0f };
static const Vec3f gGunMuzzleEnd = { 10.0f, -155.0f, 99.0f };
/* Muzzle points (from, to) used by particle_spawnBoneSegmentParticle in game/Event/TimedEvent/timedevent. */
const Vec3f gGunShotOrigin = { -1.0f, -38.0f, 38.0f };
const Vec3f gGunShotDirPoint = { -26.0f, -23.0f, 51.0f };
/* Particle textures that weapon_spawnFannedParticles picks from at random. */
static const TextureChoices gFannedParticleTextures = { { 0x60, 0x61, 0x62, 0x63 } };

/* This file's .data. */
u8 gGunMuzzleBone = 9; // bone of the muzzle points above (game/Event/TimedEvent/timedevent uses it too)

/*
 * Shot effects for `obj`: a muzzle effect and particles when it has bones, three actor_runAttackAt
 * spawns along `direction` if `spawnAttacks`, a collision_castRay call if `castRay`, then a camera event.
 */
void weapon_spawnShotEffects(Actor *obj, u8 spawnAttacks, u8 castRay, u8 fxOwner, s32 fxList) {
    Vec3f position;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    ParticleSize motion;
    Vec3f trail[3];
    RayHit scratch;
    RayHit *buffer;
    ActorCtl *state;
    s32 random1;
    s32 random0;

    weapon_getShotFrame(obj, &position, &direction, &axis0, &axis1);
    particle_countShot(obj);
    if ((obj->ob_matrix_ptr != NULL) && ((obj->cullMask & 0xF) != 0xF)) {
        particle_spawnShortLightFlash(&position, fxOwner, fxList);
        motion.height = ((math_randomFloat() * 3400.0f) + 4800.0f) * 0.001000000047f;
        motion.width = ((math_randomFloat() * 220.0f) + 320.0f) * 0.1000000015f;
        random0 = math_random();
        random1 = math_random();
        effect_spawnBoneBeamParticle(obj, gGunMuzzleBone, &gGunMuzzleStart, &gGunMuzzleEnd, &motion,
                      (random0 % 3U) + 4, (random1 & 0x7F) + 0x80,
                      (math_randomFloat() * 450.0f) + 780.0f, &position, 0xFF,
                      0, -1, fxOwner, fxList);
        weapon_spawnFannedParticles(&position, &direction, fxOwner, fxList);
    }
    if (spawnAttacks) {
        s32 pad; // matching: unused stack slot
        trail[0].x = (direction.x * 0.08800000697f) + position.x;
        trail[0].y = (direction.y * 0.08800000697f) + position.y;
        trail[0].z = (direction.z * 0.08800000697f) + position.z;
        trail[1].x = (direction.x * 0.3830000162f) + position.x;
        trail[1].y = (direction.y * 0.3830000162f) + position.y;
        trail[1].z = (direction.z * 0.3830000162f) + position.z;
        trail[2].x = (direction.x * 0.810000062f) + position.x;
        trail[2].y = (direction.y * 0.810000062f) + position.y;
        trail[2].z = (direction.z * 0.810000062f) + position.z;
        weapon_spawnAttackAt(&trail[0], &position, obj, 0x32);
        weapon_spawnAttackAt(&trail[1], &position, obj, 0x30);
        weapon_spawnAttackAt(&trail[2], &position, obj, 0x31);
    }
    if (castRay) {
        state = obj->mv;
        if (state != NULL) {
            buffer = &state->rayHit;
        } else {
            buffer = &scratch;
        }
        collision_castRay(obj, position.x, position.y, position.z,
                      direction.x * 0.001000000047f, direction.y * 0.001000000047f,
                      direction.z * 0.001000000047f, buffer, 2000.0f, 0, 0, 0, -1, 0, 0);
        collision_applyRayHit(obj, buffer, 0x10);
    }
    weapon_rumbleGun(obj);
}

/* Spawns actor_runAttackAt effect `type` at `position`, owned by `obj`. */
void weapon_spawnAttackAt(Vec3f *position, Vec3f *origin, Actor *obj, s32 type) {
    s32 flags;
    s32 index;
    u16 angle;

    flags = type | 0x60000;
    index = obj - movement_obj;
    angle = obj->langle;
    actor_runAttackAt(position->x, position->y, position->z, flags, index, angle, 0, (s32)origin);
}

/* Writes the world position of the muzzle point gGunMuzzleStart on bone gGunMuzzleBone of `obj` to `out`. */
void weapon_getGunMuzzlePos(Actor *obj, s16 unused, Vec3f *out) {
    const Vec3f *src;
    Vec3f *dst;

    src = &gGunMuzzleStart;
    dst = out;
    math_transformOffsets(&src, &dst, obj->ob_matrix_ptr[gGunMuzzleBone], 1);
}


/* Sends a cont_spawnRumble event (kind 1, 20-30 long) to `obj`'s camera, if it has one. */
void weapon_rumbleGun(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (math_random() % 11U) + 20;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        event.strength = (math_random() & 1) + 7;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 1);
    }
}

/* Gets the shot `position` and `direction` (and two axes) of `obj`. */
void weapon_getShotFrame(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1) {
    particle_getGunAxes(obj, position, direction, axis0, axis1, gGunMuzzlePoints[0], *gGunMuzzleBones[0]);
    particle_runAttackAtMidpoint(obj, position, position, 0x8003A);
}


/* Calls collision_applyShotPartHit(other, 4, 0) when `other` is a 0x9F/0xA0 object without flag 0x80 in hiddenParts. */
void func_150FFD2C(Actor *obj, Actor *other, s32 unused) {
    if (other->character_type == 0x9F || other->character_type == 0xA0) {
        if (!(other->hiddenParts & 0x80)) {
            collision_applyShotPartHit(other, 4, 0);
        }
    }
}

/* Spawns 10-18 particles at `position`, fanned out around the yaw of `direction`. */
void weapon_spawnFannedParticles(Vec3f *position, Vec3f *direction, u8 fxOwner, s32 fxList) {
    ParticleDesc spawn;
    s32 count;
    s16 baseYaw;
    s16 basePitch;
    s32 random0;
    f32 yaw;
    f32 pitch;

    count = math_random() % 9U + 10;
    spawn.renderMode = 0x2203;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.brightness = 0xFF;
    spawn.pos = *position;
    spawn.offset = gZeroVec3f;
    spawn.fadeStart = 7;
    spawn.fadeRate = 36;
    spawn.scaleStart = 1;
    spawn.flags = 0x4C207;
    spawn.primColorMode = 0;
    spawn.envColorMode = 7;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    spawn.scaleRate = 1.0f;
    math_getHeadingElevation(direction, &yaw, &pitch);
    pitch = 0.0f;
    /* degrees to a 16-bit binary angle, then to 1/256 turns */
    baseYaw = yaw * 182.0444489f;
    baseYaw >>= 8;
    baseYaw -= 138;
    basePitch = pitch * 182.0444489f;
    basePitch >>= 8;
    basePitch -= 138;
    do {
        TextureChoices ids;
        s32 random1;

        ids = gFannedParticleTextures;
        spawn.texture = ids.texture[math_random() & 3];
        spawn.timer = math_random() % 13U + 10;
        spawn.alpha = math_random() % 101U + 155;
        spawn.width = spawn.height = math_randomFloat() * 155.0f + 75.0f;
        random0 = math_random();
        random1 = math_random();
        math_dirFromBinAngles(random0 % 21U + baseYaw, random1 % 21U + basePitch, math_randomFloat() * 20.0f + 20.0f,
                      &spawn.vel);
        spawn.gravity = math_randomFloat() * 0.6210000515f + -0.6570000052f;
        spawn.flags &= ~0xC0;
        random0 = (math_random() & 1) ? 0x80 : 0;
        random1 = (math_random() & 1) ? 0x40 : 0;
        spawn.flags |= random1 | random0;
        particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0, fxOwner, fxList);
        count--;
    } while (count > 0);
}
