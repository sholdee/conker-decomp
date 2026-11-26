#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Bursts three debris pieces from the object's position. */
void actor_hitBurstDebris(Actor *obj, s32 arg1) {
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
    func_151A6F00(&emitter, 1, 0xFF, 1);
}

/*
 * Per-object movement-hit handlers, dispatched through D_80086110 by actor_runHitHandler. Before a call,
 * D_800D1560 holds the object's old position and D_800D1570 its position before the collision move.
 */
static Vec3f D_800D1560;
static Vec3f D_800D1570;

void func_1506A6FC(Actor *obj, s32 arg1) {
    func_150E0348(obj, 0xFF, 1);
}

void func_1506A724(Actor *obj, s32 arg1) {
    func_151BB61C(obj, &D_800D1560, &D_800D1570, 0xFF, 1);
}

void func_1506A760(Actor *obj, s32 arg1) {
    func_151BBA9C(obj, &D_800D1560.x, &D_800D1570.x, 0xFF, 1);
}

void func_1506A79C(Actor *obj, s32 arg1) {
    Vec3f pos;
    Vec3f delta;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    delta.x = D_800D1570.x - D_800D1560.x;
    delta.y = D_800D1570.y - D_800D1560.y;
    delta.z = D_800D1570.z - D_800D1560.z;
    func_151D3480(&pos, &delta, 0, 0, 0xFF, 1);
}

void func_1506A83C(Actor *obj, s32 arg1) {
    func_151BC104(obj, 0xFF, 1);
}

/* Spawns a flash 50 units above the object. */
void actor_hitSpawnFlash(Actor *obj, s32 arg1) {
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

void func_1506A968(Actor *obj, s32 arg1) {
    Vec3f pos;
    Vec3f delta;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    delta.x = D_800D1570.x - D_800D1560.x;
    delta.y = D_800D1570.y - D_800D1560.y;
    delta.z = D_800D1570.z - D_800D1560.z;
    func_151D3480(&pos, &delta, 0, 1, 0xFF, 1);
}

void func_1506AA08(Actor *obj, s32 arg1) {
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

    effect = func_15149130(10, 7, 35, -1, 1, 33, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &data, sizeof(data)); // the data follows the TimedEffect
    }
}

/* Calls func_15137610 for every object in the hit mask D_800CC260. */
void func_1506AAE0(Actor *obj, s32 arg1) {
    s32 mask;
    Actor *hit;

    for (mask = D_800CC260, hit = movement_obj; mask != 0; mask >>= 1, hit++) {
        if (mask & 1) {
            func_15137610(hit, &D_800D1560, &D_800D1570, 0, 0xFF, 1);
        }
    }
}

void func_1506AB7C(Actor *obj, Arg1506AB7C *arg) {
    func_15103E40(obj, arg->unk0, &arg->unk4, 1, arg->unk10, 0xFF, 1);
}

void func_1506ABC4(Actor *obj, Arg1506AB7C *arg) {
    func_15103E40(obj, arg->unk0, &arg->unk4, 2, arg->unk10, 0xFF, 1);
}

void func_1506AC0C(Actor *obj, s32 arg1) {
    ActorRef ref;

    ref.actor = obj;
    ref.uniqueId = obj->unique_id;
    trail_spawnRibbon(&ref, 0, 8, 0xFF, 1);
}

void func_1506AC58(Actor *obj, s32 arg1) {
    trail_spawnWaveTrail(obj, 0, 1, 0xFF, 1);
}

/* The movement-hit handlers by type. Declared without a prototype: two of them take a pointer as their
   second argument. */
static void (*D_80086110[])() = {
    actor_hitBurstDebris, func_1506A6FC, func_1506A724, func_1506A760, func_1506A79C,
    func_1506A83C, actor_hitSpawnFlash, func_1506A968, func_1506AA08, func_1506AA48,
    func_1506AAE0, func_1506AB7C, func_1506AC0C, func_1506AC58, func_1506ABC4,
};

/* Runs movement-hit handler `type` (0..14) for obj, passing arg on to it. */
void actor_runHitHandler(Actor *obj, s32 type, void *arg) {
    if (type >= 0 && type < 15) {
        D_800D1560.x = obj->old_xpos;
        D_800D1560.y = obj->old_ypos;
        D_800D1560.z = obj->old_zpos;
        D_800D1570.x = D_800CC254;
        D_800D1570.y = D_800CC258;
        D_800D1570.z = D_800CC25C;
        if (D_80086110[type] != NULL) {
            D_80086110[type](obj, arg);
        }
    }
}
