#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/prop.h"
#include "conker/zone.h"

/* The zones TriggerZones used here are collision cylinders (ZONE_AREA_CYLINDER, tested by
   zone_segmentHitsCylinder in game/Zone/ZoneState/zonestate.c). */

/* The gate cylinders that a hit disables, and the ones that are only tested. */
static s32 gBreakableCylinders[4] = { 1, 2, 4, 3 };
static s32 gSolidCylinders[8] = { 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34 };

/* Level hook gBulletHitTest: tests the segment from start to start + delta against the gate cylinders. */
s32 prop_shotHitsCylinders(Vec3f *start, Vec3f *delta, Vec3f *hit) {
    Vec3f end;

    end.x = start->x + delta->x;
    end.y = start->y + delta->y;
    end.z = start->z + delta->z;
    return prop_segmentHitsCylinders(start, &end, hit);
}

/*
 * Tests the segment from start to end against the zones cylinders. While object 9 exists,
 * a hit on one of the four gBreakableCylinders cylinders disables it (zone_setDisabled(id, 1)), stores
 * which one (1-4) in the object's taskint[1] and plays prop_spawnCylinderBurst's effect at it; the eight
 * gSolidCylinders cylinders are only tested. Otherwise only cylinder 76 is tested, and a hit sets
 * its data1. Returns TRUE when object 9 exists and the segment hit one of the twelve.
 */
s32 prop_segmentHitsCylinders(Vec3f *start, Vec3f *end, Vec3f *hit) {
    s32 i;
    s32 id;
    Actor *obj;

    obj = collision_findObjectById(9);
    if (obj != NULL) {
        for (i = 0; i < 4; i++) {
            id = gBreakableCylinders[i];
            if (zone_segmentHitsCylinder(&zones[id], start, end, hit)) {
                zone_setDisabled(id, 1);
                obj->taskint[1] = i + 1;
                prop_spawnCylinderBurst(&zones[id], 0xFF, 1);
                return TRUE;
            }
        }
        for (i = 0; i < 8; i++) {
            if (zone_segmentHitsCylinder(&zones[gSolidCylinders[i]], start, end, hit)) {
                return TRUE;
            }
        }
        return FALSE;
    }
    if (zone_segmentHitsCylinder(&zones[76], start, end, hit)) {
        zones[76].data1 = 1;
    }
    return FALSE;
}

#define WORK(obj) ((MoverWork *)&(obj)->touch)

/*
 * Mover handler. In states 0-1 it swings rotX on a damped spring toward an angle set by the
 * player's distance (with flags & 4: -40 degrees at the player rising to -32 at 740 units;
 * otherwise -40). Once the player's mv->unk57 is set it switches to state 2, which bounces
 * rotX back to 0 with prop_bounceX until it stops; then state 3 keeps it at 0.
 */
void prop_springTiltByPlayerDistance(WorldObject *obj) {
    f32 velocity;
    f32 dz;
    f32 target;
    f32 dx;
    f32 damping;
    f32 acceleration;
    f32 dy;
    f32 angle;
    f32 before;
    f32 distance;
    f32 delta;
    f32 magnitude;

    if ((obj->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        return;
    }
    if ((obj->task_flag & TASK_FLAG_STATE) != TASK_FLAG_GOING_ON) {
        if (obj->object_status & 4) {
            if (movement_obj->mv->unk57 != 0) {
                obj->task_flag &= ~TASK_FLAG_STATE;
                obj->task_flag |= TASK_FLAG_GOING_ON;
                obj->moveData.word = 0;
                WORK(obj)->bounce.rebound = 0.5f;
                WORK(obj)->bounce.acceleration = 0.3000000119f;
                WORK(obj)->bounce.velocity = 0.0f;
            }
            dz = obj->z - movement_obj->zpos;
            dx = obj->x - movement_obj->xpos;
            dy = obj->y - movement_obj->ypos;
            distance = sqrtf(dz * dz + (dx * dx + dy * dy));
            if (distance > 740.0f) {
                distance = 740.0f;
            }
            target = distance * 0.01081081107f + -40.0f;
        } else {
            target = -40.0f;
        }
        angle = obj->t;
        damping = WORK(obj)->swing.damping;
        acceleration = WORK(obj)->swing.acceleration;
        velocity = WORK(obj)->swing.velocity;
        if (angle != target || velocity != 0.0f) {
            angle += velocity * gFrameTicks;
            delta = math_angleDiff(angle, target);
            magnitude = fabsf(delta);
            if (delta > 0.0f) {
                velocity += magnitude * acceleration;
            } else {
                velocity -= magnitude * acceleration;
            }
            velocity *= damping;
            if (fabsf(delta) < 0.009999999776f && fabsf(velocity) < 0.009999999776f) {
                velocity = 0.0f;
                angle = target;
            } else if (angle < 0.0f) {
                angle += 360.0f;
            } else if (angle >= 360.0f) {
                angle -= 360.0f;
            }
            WORK(obj)->swing.velocity = velocity;
        }
        obj->t = angle;
    } else {
        before = obj->t;
        prop_bounceX(obj);
        if (before == obj->t) {
            obj->task_flag &= ~TASK_FLAG_STATE;
            obj->task_flag |= TASK_FLAG_ON;
            obj->t = 0.0f;
        }
    }
}

static const Vec3f D_800A1490 = { -304.75f, 304.5f, 6214.25f };
static const Vec3f D_800A149C = { -304.75f, 250.0f, 6214.25f };
static const Vec3f D_800A14A8[4] = {
    { -309.0f, 335.0f, 6247.0f },
    { -312.0f, 266.0f, 6251.0f },
    { -299.0f, 269.0f, 6178.0f },
    { -299.0f, 348.0f, 6181.0f },
};
static const u8 D_800A14D8[4] = { 0xA8, 0xA9, 0xAA, 0xAB };

/*
 * Plays a fixed burst of effects: camera_startWobblePreset, a particle_startRandomEmitter effect at D_800A149C lasting
 * 120-181 frames, the particle_spawnBlastSpray/particle_spawnLightFlash/particle_spawnGrowFade trio at D_800A1490, and four
 * particle_newModelParticleWithFloor debris pieces thrown from the D_800A14A8 points at random yaw/pitch/speed.
 */
void func_150EB8C4(void) {
    ParticleSpawn spawn;

    camera_startWobblePreset(2, gLastPlayerIndex, NULL, 0xFF, 1);
    particle_startRandomEmitter(&D_800A149C, math_random() % 62U + 120, 0xFF, 1);
    particle_spawnBlastSpray(&D_800A1490, 0xFF, 1);
    particle_spawnLightFlash(&D_800A1490, 0xFF, 1);
    particle_spawnGrowFade(&D_800A1490, math_randomFloat() * 4.0f + 12.0f, 1.640999913f,
                  math_random() % 56U + 200, 0.0f, 0xFF, 1);
    {
        u8 i;
        s32 yaw;
        s32 pitch;

        spawn.sizeY = 1.0f;
        spawn.sizeXZ = 1.0f;
        spawn.radius = 9.0f;
        spawn.elasticity = 0.4350000322f;
        spawn.rot = gZeroVec3f;
        spawn.rotVel.y = 0.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x29E9;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.updateFn = 13;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 12;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 25;
        spawn.fadeRate = 10;
        spawn.sound = 0;
        // matching: an early use of pitch makes IDO give it $s1 and yaw $s2
        if (pitch) {
        }
        for (i = 0; i < 4; i++) {
            spawn.pos = D_800A14A8[i];
            yaw = math_random();
            pitch = math_random();
            math_dirFromBinAngles(yaw % 26U + 45, (pitch & 0xF) - 25, math_randomFloat() * 30.0f + 15.0f, &spawn.vel);
            spawn.rotVel.x = math_randomFloat() * 15.10800076f + -7.554000378f;
            spawn.rotVel.z = math_randomFloat() * 15.10800076f + -7.554000378f;
            spawn.gravity = math_randomFloat() * 0.6170000434f + -1.621000051f;
            spawn.lifetime = math_random() % 51U + 50;
            spawn.model = D_800A14D8[i];
            particle_newModelParticleWithFloor(&spawn, 3, 0xFF, 0, 0, 0xFF, 1);
        }
    }
}


/*
 * Effect burst at a gate cylinder: a particle_startRandomEmitter effect lasting 300-400 frames, a rumble
 * (type 1, 15-35 long), camera_startWobblePreset, and the particle_spawnLightFlash/particle_spawnBlastSpray/particle_spawnGrowFade trio at
 * the cylinder's base and again half its height up.
 */
void prop_spawnCylinderBurst(TriggerZone *gate, u8 owner, s32 kind) {
    Vec3f position;
    f32 random;
    f32 step;
    RumbleParams event;
    f32 growth;
    u8 i;

    position.x = gate->x;
    position.y = gate->y;
    position.z = gate->z;
    step = gate->ySize * 0.5f;
    particle_startRandomEmitter(&position, math_random() % 101U + 300, owner, kind);
    event.flags = 1;
    event.duration = math_random() % 21U + 15;
    event.strength = 0;
    event.updateKind = -1;
    event.playerMask = 1;
    cont_spawnRumble(&event, 0, owner, kind);
    camera_startWobblePreset(0, 0, NULL, owner, kind);
    growth = 1.640999913f; // matching: a local (not a literal) is loaded once into a saved FP register
    for (i = 0; i < 2; i++) {
        particle_spawnLightFlash(&position, owner, kind);
        particle_spawnBlastSpray(&position, owner, kind);
        random = math_randomFloat();
        particle_spawnGrowFade(&position, random * 4.0f + 12.0f, growth,
                      math_random() % 56U + 200, 0.0f, owner, kind);
        position.y += step;
    }
}
