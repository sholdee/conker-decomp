#include <ultra64.h>
#include "conker/actor.h"
#include "conker/blood.h"
#include "conker/effect.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/trail.h"

/* Bursts three debris pieces from the object's position. */
void actor_hitBurstDebris(Actor *obj, s32 arg) {
    DebrisEmitter emitter;

    emitter.count = 3;
    emitter.pos.x = obj->xpos;
    emitter.pos.y = obj->ypos;
    emitter.pos.z = obj->zpos;
    emitter.radius = 151.5f;
    emitter.speedBase = 15.10000038f;
    emitter.speedRange = 25.20000076f;
    emitter.lifetime = 90;
    emitter.yawBase = 0;
    emitter.yawRange = 255;
    emitter.pitchBase = -64;
    emitter.pitchRange = 40;
    emitter.dir.x = 0.0f;
    emitter.dir.y = 0.0f;
    emitter.dir.z = 0.0f;
    emitter.landPuffs = 0;
    emitter.landPuffFlag = 0;
    emitter.landPuffLifeBase = 50;
    emitter.landPuffLifeRange = 25;
    emitter.sizeBase = 0.3880000114f;
    emitter.sizeRange = 0.2430000156f;
    emitter.gravityBase = -1.0200001f;
    emitter.gravityRange = 0; // matching: an int 0 gets its own mtc1 zero; 0.0f would share the 0.0f register
    emitter.spinBase = -4.0f;
    emitter.spinRange = 8.0f;
    blood_spawnDebris(&emitter, 1, 0xFF, 1);
}

/*
 * Per-object movement-hit handlers, dispatched through gActorHitHandlers by actor_runHitHandler. Before a call,
 * gHitOldPos holds the object's old position and gHitNewPos its position before the collision move.
 */
static Vec3f gHitOldPos;
static Vec3f gHitNewPos;

void actor_hitAttachBloodDrip(Actor *obj, s32 arg) {
    effect_attachBloodDrip(obj, 0xFF, 1);
}

void actor_hitShotImpact(Actor *obj, s32 arg) {
    effect_spawnShotImpact(obj, &gHitOldPos, &gHitNewPos, 0xFF, 1);
}

void actor_hitMidpointImpact(Actor *obj, s32 arg) {
    effect_spawnMidpointImpact(obj, &gHitOldPos.x, &gHitNewPos.x, 0xFF, 1);
}

void func_1506A79C(Actor *obj, s32 arg) {
    Vec3f pos;
    Vec3f delta;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    delta.x = gHitNewPos.x - gHitOldPos.x;
    delta.y = gHitNewPos.y - gHitOldPos.y;
    delta.z = gHitNewPos.z - gHitOldPos.z;
    particle_spawnHitImpact(&pos, &delta, 0, 0, 0xFF, 1);
}

void func_1506A83C(Actor *obj, s32 arg) {
    func_151BC104(obj, 0xFF, 1);
}

/* Spawns a flash 50 units above the object. */
void actor_hitSpawnFlash(Actor *obj, s32 arg) {
    LightEffect *flash;
    LightEffectHeader header;
    f32 params[4];
    Vec3i pos;

    pos.x = obj->xpos;
    pos.y = obj->ypos + 50.0f;
    pos.z = obj->zpos;
    params[0] = 0.0f;
    params[2] = 0.0f;
    header.flags = 2;
    header.update = 15;
    header.lifetime = 300;
    header.lightType = 5;
    params[1] = 80.0f;
    params[3] = 0.1020999998f;

    flash = light_createWithEffect(&header, &pos, 0, 0xFF, 0xFF, 90, 0xFF, 0, 0x10, 0xFF, 1);
    if (flash != NULL) {
        memcpy(&flash->data, params, sizeof(params));
    }
}

void func_1506A968(Actor *obj, s32 arg) {
    Vec3f pos;
    Vec3f delta;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    delta.x = gHitNewPos.x - gHitOldPos.x;
    delta.y = gHitNewPos.y - gHitOldPos.y;
    delta.z = gHitNewPos.z - gHitOldPos.z;
    particle_spawnHitImpact(&pos, &delta, 0, 1, 0xFF, 1);
}

void actor_hitSpawnTrails(Actor *obj, s32 arg) {
    trail_spawnParticleAtTarget(obj, 0xFF, 1);
    trail_spawnFollowTrail(obj, 0xFF, 1);
}

/* Spawns an effect that carries the object and the two words at arg as its data. */
void func_1506AA48(Actor *obj, Pair1506AA48 *arg) {
    TimedEffect *effect;
    struct {
        ActorRef ref;
        Pair1506AA48 value;
    } data;

    data.ref.actor = obj;
    data.ref.uniqueId = obj->unique_id;
    data.value = *arg;

    effect = effect_newTimedEffect(10, 7, 35, -1, 1, 33, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &data, sizeof(data)); // the data follows the TimedEffect
    }
}

/* Calls blood_spawnActorImpact for every object in the hit mask gAttackHitMask. */
void actor_hitBloodImpacts(Actor *obj, s32 arg) {
    s32 mask;
    Actor *hit;

    for (mask = gAttackHitMask, hit = movement_obj; mask != 0; mask >>= 1, hit++) {
        if (mask & 1) {
            blood_spawnActorImpact(hit, &gHitOldPos, &gHitNewPos, 0, 0xFF, 1);
        }
    }
}

void func_1506AB7C(Actor *obj, ImpactBlastArgs *arg) {
    effect_spawnImpactBlast(obj, arg->source, &arg->pos, 1, arg->unk10, 0xFF, 1);
}

void func_1506ABC4(Actor *obj, ImpactBlastArgs *arg) {
    effect_spawnImpactBlast(obj, arg->source, &arg->pos, 2, arg->unk10, 0xFF, 1);
}

void actor_hitSpawnRibbon(Actor *obj, s32 arg) {
    ActorRef ref;

    ref.actor = obj;
    ref.uniqueId = obj->unique_id;
    trail_spawnRibbon(&ref, 0, 8, 0xFF, 1);
}

void actor_hitSpawnWaveTrail(Actor *obj, s32 arg) {
    trail_spawnWaveTrail(obj, 0, 1, 0xFF, 1);
}

/* The movement-hit handlers by type. Declared without a prototype: two of them take a pointer as their
   second argument. */
static void (*gActorHitHandlers[])() = {
    actor_hitBurstDebris, actor_hitAttachBloodDrip, actor_hitShotImpact, actor_hitMidpointImpact, func_1506A79C,
    func_1506A83C, actor_hitSpawnFlash, func_1506A968, actor_hitSpawnTrails, func_1506AA48,
    actor_hitBloodImpacts, func_1506AB7C, actor_hitSpawnRibbon, actor_hitSpawnWaveTrail, func_1506ABC4,
};

/* Runs movement-hit handler `type` (0..14) for obj, passing arg on to it. */
void actor_runHitHandler(Actor *obj, s32 type, void *arg) {
    if (type >= 0 && type < 15) {
        gHitOldPos.x = obj->old_xpos;
        gHitOldPos.y = obj->old_ypos;
        gHitOldPos.z = obj->old_zpos;
        gHitNewPos.x = gPreCollisionX;
        gHitNewPos.y = gPreCollisionY;
        gHitNewPos.z = gPreCollisionZ;
        if (gActorHitHandlers[type] != NULL) {
            gActorHitHandlers[type](obj, arg);
        }
    }
}
