#include <ultra64.h>
#include "conker/attach.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/weapon.h"

/* matching: no header declares particle_spawnSteamParticle (steam.c): modelbeam.c and blood.c were compiled with other views */
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 alpha, f32 size, s32 hitsFloor,
                             f32 burstScale, f32 altBurstScale, u8 lit, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 owner, s32 list);

/* Model-space points: weapon_fireSegmentShot transforms gSegmentShotStart and gSegmentShotEnd through model part 0x46 into
 * world space; weapon_spawnSegmentBeam passes gSegmentShotStart and gSegmentShotBeamEnd to effect_spawnBoneBeamParticle. */
const Vec3f gSegmentShotStart = { -113.8288f, -14.082394f, 14.5422f };
const Vec3f gSegmentShotBeamEnd = { -124.648605f, -14.082394f, 14.5422f };
const Vec3f gSegmentShotEnd = { 7.3199987f, -14.082394f, 4.5422006f };

/* Spawns a weapon_spawnProjectile effect along a segment at `obj`: offset along math_getActorAim's `dir` from its
 * position, or, when it has ob_matrix_ptr, between two points of model part 0x46, plus weapon_spawnLightAt and
 * (half the time) weapon_spawnSegmentBeam effects at the segment start. */
void weapon_fireSegmentShot(Actor *obj, u8 fxOwner, s32 fxList) {
    Vec3f start;
    Vec3f end;
    Vec3f dir;
    Vec3f axis0;
    ModelAttachment *part;
    const Vec3f *modelPoints[2];
    Vec3f *worldPoints[2];
    s32 soundChance;
    s32 filter;
    s32 randInt;
    f32 randB;
    f32 randA;

    if (obj != NULL) {
        math_getActorAim(obj, &dir, &axis0, NULL, 0.0f);
        dir.x *= 0.001f;
        dir.y *= 0.001f;
        dir.z *= 0.001f;
        if (obj->ob_matrix_ptr != NULL) {
            modelPoints[0] = &gSegmentShotStart;
            modelPoints[1] = &gSegmentShotEnd;
            worldPoints[0] = &start;
            worldPoints[1] = &end;
            part = attach_find(obj, 0x46, 0);
            if (part == NULL) {
                return;
            }
            if (math_transformAttachmentOffsets(obj, part, 0, modelPoints, worldPoints, 2) == 0) {
                return;
            }
            weapon_spawnLightAt(&start, fxOwner, fxList);
            if (math_random() & 1) {
                weapon_spawnSegmentBeam(obj, &start, 0x46, fxOwner, fxList);
            }
        } else {
            start.x = obj->xpos;
            start.y = obj->ypos + 106.0f;
            start.z = obj->zpos;
            end.x = start.x + (dir.x * -82.0f);
            end.y = start.y + (dir.y * -82.0f);
            end.z = start.z + (dir.z * -82.0f);
            start.x -= dir.x * -120.0f;
            start.y -= dir.y * -120.0f;
            start.z -= dir.z * -120.0f;
        }
        randA = math_randomFloat();
        randB = math_randomFloat();
        randInt = math_random();
        if (obj->ob_matrix_ptr != NULL) {
            soundChance = 255;
        } else {
            soundChance = 128;
        }
        if (gIsMultiplayer != 0) {
            filter = -1;
        } else {
            filter = 2;
        }
        weapon_spawnProjectile(&start, 0, &end, &dir, 1, 0, 300.0f, 1.0f / 300.0f, (randA * 10.0f) + 25.0f,
                      (randB * 150.0f) + 400.0f, 80.0f, (randInt % 56U) + 200, obj, 1, 1, 0, soundChance, 0, 1, 0,
                      0x1A, 0.0f, 0xFF, filter, 0, fxOwner, fxList);
    }
}

/* Spawns a effect_spawnBoneBeamParticle effect at `pos` on `obj` with randomized scale and parameters. */
void weapon_spawnSegmentBeam(Actor *obj, Vec3f *pos, s16 node, u8 fxOwner, s32 fxList) {
    ParticleSize scale;
    f32 angle;
    s32 rand[2]; /* rand[1] is drawn but never used */

    scale.height = (math_randomFloat() * 1001.0f + 604.0f) * 0.01f;
    scale.width = (math_randomFloat() * 59.0f + 141.0f) * 0.1f;
    rand[0] = math_random();
    rand[1] = math_random();
    angle = math_randomFloat();
    effect_spawnBoneBeamParticle(obj, 0, &gSegmentShotStart, &gSegmentShotBeamEnd, &scale, (rand[0] & 3) + 6, 0xFF,
                  angle * 270.0f + 359.0f, pos, 0xFF, 0, node, fxOwner, fxList);
}

/* Spawns a light_createWithEffect effect at `pos`, converted to integer coordinates. */
void weapon_spawnLightAt(Vec3f *pos, u8 fxOwner, s32 fxList) {
    LightEffectHeader header;
    Vec3i intPos;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (math_random() % 3U) + 3;
    header.lightType = 0;

    intPos.x = pos->x;
    intPos.y = pos->y;
    intPos.z = pos->z;

    light_createWithEffect(&header, &intPos, (math_random() & 1) + 6, 0xFF, 0xE8, 0xAB, 0xFF, 0, 0, fxOwner, fxList);
}

/* Spawns a burst of effects from the frame blood_buildImpactFrame derives from `from`, `to` and `direction`: always a
 * particle_spawnKindPlainSprite effect, then (each when its flag is set) a ring of particle_spawnSplash particles,
 * particle_spawnSteamParticle particles along `dir`, and particle_newSpriteParticleWithFloor particles thrown at random angles. */
void weapon_spawnHitBurst(Vec3f *from, Vec3f *to, Vec3f *direction, u8 doForward, u8 doRing, u8 doScatter, u8 fxOwner,
                   s32 fxList) {
    Vec3f start;
    Vec3f origin;
    // matching: dir and the basis are arrays; Vec3f locals change the stack layout
    f32 dir[3];
    f32 basisA[3];
    f32 basisB[3];
    s16 ringCount;
    f32 sinv;
    f32 cosv;
    Vec3f vel;
    s32 randB;
    s32 bit80;
    Vec3f vel2;
    s32 count;
    ParticleDesc spawn;
    f32 drag;
    f32 f;
    f32 f2;
    s32 randA;
    s32 randC;
    SpriteParticle *particle;

    if (blood_buildImpactFrame(&start, &origin, (Vec3f *)dir, (Vec3f *)basisA, (Vec3f *)basisB, from, to, direction) != 0) {
        f2 = math_randomFloat();
        randA = math_random();
        particle_spawnKindPlainSprite(&start, f2 * 24.0f + 168.0f, randA % 101U + 155, (math_random() & 7) + 15, 1, fxOwner, fxList);
        if (doRing) {
            ringCount = math_random() % 3U + 5;
            while (ringCount != 0) {
                f = (math_randomFloat() * 33.0f + 60.0f) * 0.1f;
                math_dirFromBinAngleXZ(math_random() & 0xFF, f, &cosv, &sinv);
                vel.x = basisB[0] * sinv + basisA[0] * cosv;
                vel.y = basisB[1] * sinv + basisA[1] * cosv;
                vel.z = basisB[2] * sinv + basisA[2] * cosv;
                f = math_randomFloat();
                randC = math_random();
                randA = math_random();
                f2 = math_randomFloat();
                particle_spawnSplash(&start, &vel, f * 0.162f + 0.193f, randC % 9U + 18, randA % 156U + 100,
                              f2 * 4.0f + 4.0f, 3, math_random() % 1U, 1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, fxOwner,
                              fxList);
                ringCount--;
            }
            // matching: this dead store gives the ring loop's exit its own block; uopt reloads the loop
            // constants there as well as at the join, and as1 deletes those dead reloads but keeps the
            // extra 0.1f literal that the ROM has
            ringCount = 0;
        }
        if (doForward) {
            for (count = math_random() % 6U + 5; count != 0; count--) {
                f = (math_randomFloat() * 150.0f + 200.0f) * 0.1f;
                vel2.x = dir[0] * f;
                vel2.y = dir[1] * f;
                vel2.z = dir[2] * f;
                f = math_randomFloat();
                randA = math_random();
                randB = math_random();
                particle_spawnSteamParticle(&origin, &vel2, 1, f * 0.682000041f + -0.4120000303f, randA % 11U + 5,
                              randB % 156U + 100, math_randomFloat() * 60.0f + 80.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0, fxOwner,
                              fxList);
            }
        }
        if (doScatter) {
            count = math_random() % 7U + 3;
            drag = 0.9508649707f;
            spawn.texture = 0x6C;
            spawn.renderMode = 0x5103;
            spawn.setGeometryMode = 0x200005;
            spawn.fadeStart = 20;
            spawn.fadeRate = 12;
            spawn.flags = 0x80D207;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = 0x10;
            spawn.clearGeometryMode = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.r = 0xE2;
            spawn.g = 0xB2;
            spawn.b = 0x60;
            spawn.a = 0xFF;
            spawn.r2 = 0x39;
            spawn.g2 = 0xF;
            spawn.b2 = 0;
            spawn.brightness = 0xFF;
            spawn.pos = origin;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            for (; count > 0; count--) {
                randA = math_random();
                randC = math_random();
                math_dirFromBinAngles(randA & 0xFF, -(randC & 0x3F), math_randomFloat() * 7.0f + 2.0f, &spawn.vel);
                f = math_randomFloat() * 117.0f;
                spawn.flags &= ~0xC0;
                spawn.gravity = (f + -197.0f) * 0.001f;
                bit80 = (math_random() & 1) ? 0x80 : 0;
                // matching: reusing randB for the 0x40 bit; a separate local changes the frame
                randB = (math_random() & 1) ? 0x40 : 0;
                spawn.flags |= randB | bit80;
                spawn.alpha = math_random() % 66U + 190;
                spawn.timer = math_random() % 45U + 44;
                f2 = math_randomFloat() * 122.0f + 77.0f;
                spawn.width = f2;
                spawn.height = f2;
                particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, sizeof(f32), fxOwner, fxList);
                if (particle != NULL) {
                    memcpy(&particle->data.drag, &drag, sizeof(drag));
                }
            }
        }
    }
}

/* Blood strip emit callback (gBloodStripEmitCallbacks): spawns one particle_spawnSteamParticle particle whose position, velocity and
 * parameters blood_getStripParticleParams computes. */
void weapon_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
    Vec3f pos;
    Vec3f unk60;
    Vec3f vel;
    /* particle_spawnSteamParticle arguments 3..6 */
    f32 gravity;
    s16 lifetime;
    u8 alpha;
    f32 size;

    blood_getStripParticleParams(pointA, pointB, velocityA, velocityB, time, strip, &pos, &unk60, &vel, &gravity, &lifetime, &alpha,
                  &size);
    particle_spawnSteamParticle(&pos, &vel, 1, gravity, lifetime, alpha, size, 0, 1.0f, 1.0f, 1, 0, 1, 0, strip->header.owner,
                  strip->header.kind);
}

/* Spawns a particle_newSpriteParticleWithFloor particle at `pos` moving at `vel`. A `timer` of -1 means 300 and leaves flag
 * bit 0 clear. Returns the particle, or NULL. */
SpriteParticle *weapon_spawnDragSprite(Vec3f *pos, Vec3f *vel, f32 gravity, f32 drag, u8 alpha, s16 timer, f32 size,
                        u8 altUpdate, CollisionHit *ground, u8 fxOwner, s32 fxList) {
    ParticleDesc spawn;
    f32 dragValue;
    SpriteParticle *particle;
    ParticleBounce bounce;
    s32 bit80;
    s32 hasTimer;
    s32 bit40;

    dragValue = drag;
    spawn.texture = 0x6C;
    spawn.renderMode = 0x5103;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.fadeStart = 20;
    spawn.fadeRate = 12;
    bit40 = (math_random() & 1) ? 0x40 : 0;
    hasTimer = (timer == -1) ? 0 : 1;
    bit80 = (math_random() & 1) ? 0x80 : 0;
    spawn.flags = bit80 | hasTimer | 2 | 4 | bit40 | 0x4000 | 0x8000 | 0x200 | 0x800000;
    spawn.primColorMode = 8;
    spawn.envColorMode = 6;
    if (altUpdate) {
        spawn.updateFunc = 0x1D;
    } else {
        spawn.updateFunc = 0xA;
    }
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.scaleStart = 1;
    spawn.handlerSet = 0;
    bounce.flags = 1;
    bounce.hitFunc = 0;
    spawn.r = 0xE2;
    spawn.g = 0xB2;
    spawn.b = 0x60;
    spawn.a = 0xFF;
    spawn.r2 = 0x39;
    spawn.g2 = 0xF;
    spawn.b2 = 0;
    spawn.brightness = 0xFF;
    spawn.scaleRate = 1.0f;
    bounce.restitution = 0.703f;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.vel = *vel;
    spawn.gravity = gravity;
    spawn.alpha = alpha;
    if (timer == -1) {
        spawn.timer = 300;
    } else {
        spawn.timer = timer;
    }
    spawn.height = size;
    spawn.width = size;
    particle = particle_newSpriteParticleWithFloor(&spawn, 1, ground, sizeof(particle->data.dragBounce), fxOwner, fxList);
    if (particle != NULL) {
        memcpy(&particle->data.dragBounce.drag, &dragValue, sizeof(dragValue));
        memcpy(&particle->data.dragBounce.bounce, &bounce, sizeof(bounce));
    }
    return particle;
}

/* Spawns a fixed particle_spawnParticleBurst effect at `pos`, with `ground` as its particles' floor. */
void weapon_spawnTedizSplash(Vec3f *pos, CollisionHit *ground, u8 fxOwner, s32 fxList) {
    ParticleBurst desc;

    desc.countBase = 10;
    desc.countRange = 4;
    desc.pos = pos;
    desc.yawBase = 0;
    desc.yawRange = 255;
    desc.pitchBase = -24;
    desc.pitchRange = 25;
    desc.speedBase = 5.0f;
    desc.speedRange = 7.0f;
    desc.gravityBase = -0.2080000043f;
    desc.gravityRange = 0.08500000089f;
    desc.drag = 0.9706320167f;
    desc.alphaBase = 190;
    desc.alphaRange = 65;
    desc.timerBase = 200;
    desc.timerRange = 150;
    desc.sizeBase = 96.0f;
    desc.sizeRange = 109.0f;
    desc.altChance = 1.0f;
    desc.ground = ground;
    particle_spawnParticleBurst(&desc, fxOwner, fxList);
}

/* gObjectFilters object filter: rejects objects with id 0x5A, 0x74 or 0x7A. */
s32 func_151C6290(Actor *obj, Actor *self, s32 filterArg) {
    if (obj->character_type == 0x5A || obj->character_type == 0x74 || obj->character_type == 0x7A) {
        return FALSE;
    }
    return TRUE;
}
