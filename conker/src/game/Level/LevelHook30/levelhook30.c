#include <ultra64.h>
#include "effects.h"
#include "conker/actor.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/script.h"
#include "conker/sound.h"

/* matching: no header declares particle_spawnKindSprite (steam.c): blood.c was compiled with another view */
SteamParticle *particle_spawnKindSprite(s32 kind, f32 *pos, f32 *vel, f32 size, f32 scaleRate, f32 drag, f32 gravity, s16 lifetime,
                             u8 alpha, s16 fadeFrames, s16 fadeStep, u8 spin, u8 owner, s32 list);

/* This file's .data: the current values, targets and easing velocities of three channels. */
static f32 gHook30Value0 = 255.0f;
static f32 gHook30Value1 = 255.0f;
static f32 gHook30Value2 = 255.0f;
static f32 gHook30Target0 = 255.0f;
static f32 gHook30Target1 = 255.0f;
static f32 gHook30Target2 = 255.0f;
static f32 gHook30Velocity0 = 0.0f;
static f32 gHook30Velocity1 = 0.0f;
static f32 gHook30Velocity2 = 0.0f;
static s32 D_800888E4[7] = { 0 }; /* not referenced */

/* Picks targets for three channels (255, or one of them 64 per flags 0x4043-0x4045 while 0x403D is
 * set), eases the channels toward them, and writes them to ids 0x30F3-0x30F5 and 0x30F8-0x30FA. */
void level_hook30(Camera *camera) {
    if (script_query(1, 0x403D, 6, 0x9000)) {
        camera->AvailableModes |= 0x80021000;
        if (script_query(1, 0x4043, 6, 0x9000)) {
            gHook30Target0 = 64.0f;
            gHook30Target1 = 255.0f;
            gHook30Target2 = 255.0f;
        } else if (script_query(1, 0x4044, 6, 0x9000)) {
            gHook30Target0 = 255.0f;
            gHook30Target1 = 64.0f;
            gHook30Target2 = 255.0f;
        } else if (script_query(1, 0x4045, 6, 0x9000)) {
            gHook30Target0 = 255.0f;
            gHook30Target1 = 255.0f;
            gHook30Target2 = 64.0f;
        } else {
            gHook30Target0 = 255.0f;
            gHook30Target1 = 255.0f;
            gHook30Target2 = 255.0f;
        }
    } else {
        gHook30Target0 = 255.0f;
        gHook30Target1 = 255.0f;
        gHook30Target2 = 255.0f;
        camera->AvailableModes &= ~0x80021000;
        camera->AvailableModes |= 8;
    }
    math_springTo(&gHook30Value0, gHook30Target0, &gHook30Velocity0, 4.0f, 6.0f, camera->SpringTime);
    math_springTo(&gHook30Value1, gHook30Target1, &gHook30Velocity1, 4.0f, 6.0f, camera->SpringTime);
    math_springTo(&gHook30Value2, gHook30Target2, &gHook30Velocity2, 4.0f, 6.0f, camera->SpringTime);
    script_command(1, 0x30F5, 0x12, (u8)gHook30Value0);
    script_command(1, 0x30F4, 0x12, (u8)gHook30Value1);
    script_command(1, 0x30F3, 0x12, (u8)gHook30Value2);
    script_command(1, 0x30FA, 0x12, (u8)gHook30Value0);
    script_command(1, 0x30F9, 0x12, (u8)gHook30Value1);
    script_command(1, 0x30F8, 0x12, (u8)gHook30Value2);
}

/* Steps four scroll counters by gFrameTicks; then, unless movement_obj[0]'s mv->death_State is set, kills
 * movement_obj[0] once per prop 0xFA-0xF8 it stands on, and if it stands on prop 0xFB plays sound 0x627 and
 * runs it through blood_spawnHitEffects .. actor_kill(6). */
void level_updateHazardProps(s32 arg0) {
    WorldObject *prop;
    s32 propIndex;
    s32 i;

    gTile0ScrollS += gFrameTicks * 40;
    gTile0ScrollT += gFrameTicks * -66;
    gTile1ScrollS += gFrameTicks * -8;
    gTile1ScrollT += gFrameTicks * 92;

    if (movement_obj->mv->death_State == 0) {
        for (i = 0; i < 3; i++) {
            prop = gfx_findObjectById(0xFA - i);
            propIndex = prop - db_Object_array;
            if (gWorldObjectContacts[propIndex] & 1) {
                effect_killAndSplitActor(movement_obj);
                sound_playActorSound2(0x627, movement_obj, 0x7FFF, 200, 700);
            }
        }

        prop = gfx_findObjectById(0xFB);
        propIndex = prop - db_Object_array;
        if (gWorldObjectContacts[propIndex] & 1) {
            sound_playActorSound2(0x627, movement_obj, 0x7FFF, 200, 700);
            blood_spawnHitEffects(movement_obj, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(movement_obj);
            actor_kill(movement_obj, 6);
        }
    }
}

/* Spawns a blood strip on obj that uses sample callback 6 and emit callback 7 (below);
 * mirrored selects which way it is offset. */
void level_spawnBloodStrip(Actor *obj, u8 mirrored) {
    BloodStripDesc desc;

    desc.ownerId = obj->unique_id;
    desc.owner = obj;
    desc.bone = 0;
    desc.offsetA.x = 0.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 0.0f;
    desc.offsetB.x = 0.0f;
    if (mirrored) {
        desc.offsetB.y = -30.0f;
    } else {
        desc.offsetB.y = 30.0f;
    }
    desc.offsetB.z = 0.0f;
    desc.flags = 2;
    desc.duration = 50;
    desc.pause = 22;
    desc.lifetime = 2000;
    desc.sound = 0;
    desc.sampleCallback = 6;
    desc.emitCallback = 7;
    desc.rateCallback = -1;
    desc.rate = 0.3508499861f;
    desc.mode = 0;
    desc.updateCallback = -1;
    blood_spawnStrip(&desc, 0);
}

/* BloodStrip sample callback 6: random power in [0.35, 0.5). */
s32 level_sampleBloodStripPower(BloodStrip *strip) {
    strip->power = (math_randomFloat() * 150.0f + 350.0f) * 0.001f;
    return 1;
}

/* BloodStrip emit callback 7: moves both strip points `time` along their velocities and emits one
 * particle_spawnKindSprite particle at A, heading from A to B scaled by the strip's power. */
void level_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
    Vec3f start;
    f32 endX;
    f32 endY;
    f32 endZ;
    Vec3f velocity;
    s32 pad; // matching: unused stack slot
    s32 randomInt;
    f32 random1;
    f32 random0;

    start.x = pointA->x + velocityA->x * time;
    start.y = pointA->y + velocityA->y * time;
    start.z = pointA->z + velocityA->z * time;

    endX = pointB->x + velocityB->x * time;
    endY = pointB->y + velocityB->y * time;
    endZ = pointB->z + velocityB->z * time;

    velocity.x = (endX - start.x) * strip->power;
    velocity.y = (endY - start.y) * strip->power;
    velocity.z = (endZ - start.z) * strip->power;

    random0 = math_randomFloat();
    random1 = math_randomFloat();
    randomInt = math_random();

    particle_spawnKindSprite(0, &start.x, &velocity.x, random0 * 61.0f + 60.0f, 1.03571403f, 0.9704080224f,
                  random1 * 0.1570000052f + -0.08000000566f, (randomInt & 0xF) + 35,
                  math_random() % 156U + 100, 30, 8, 0, strip->header.owner, strip->header.kind);
}
