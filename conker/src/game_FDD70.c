#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151DB5D0 (game_2062D0.c) is declared in each file that calls it: game/Effects/Blood/blood.c was compiled with a
// different view of it.
SteamParticle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
                             u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD);

/* This file's .data: the current values, targets and easing velocities of three channels. */
static f32 D_800888C0 = 255.0f;
static f32 D_800888C4 = 255.0f;
static f32 D_800888C8 = 255.0f;
static f32 D_800888CC = 255.0f;
static f32 D_800888D0 = 255.0f;
static f32 D_800888D4 = 255.0f;
static f32 D_800888D8 = 0.0f;
static f32 D_800888DC = 0.0f;
static f32 D_800888E0 = 0.0f;
static s32 D_800888E4[7] = { 0 }; /* not referenced */

/* Picks targets for three channels (255, or one of them 64 per flags 0x4043-0x4045 while 0x403D is
 * set), eases the channels toward them, and writes them to ids 0x30F3-0x30F5 and 0x30F8-0x30FA. */
void func_150D08C0(Camera *camera) {
    if (func_1509BE40(1, 0x403D, 6, 0x9000)) {
        camera->AvailableModes |= 0x80021000;
        if (func_1509BE40(1, 0x4043, 6, 0x9000)) {
            D_800888CC = 64.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 255.0f;
        } else if (func_1509BE40(1, 0x4044, 6, 0x9000)) {
            D_800888CC = 255.0f;
            D_800888D0 = 64.0f;
            D_800888D4 = 255.0f;
        } else if (func_1509BE40(1, 0x4045, 6, 0x9000)) {
            D_800888CC = 255.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 64.0f;
        } else {
            D_800888CC = 255.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 255.0f;
        }
    } else {
        D_800888CC = 255.0f;
        D_800888D0 = 255.0f;
        D_800888D4 = 255.0f;
        camera->AvailableModes &= ~0x80021000;
        camera->AvailableModes |= 8;
    }
    func_150495B0(&D_800888C0, D_800888CC, &D_800888D8, 4.0f, 6.0f, camera->SpringTime);
    func_150495B0(&D_800888C4, D_800888D0, &D_800888DC, 4.0f, 6.0f, camera->SpringTime);
    func_150495B0(&D_800888C8, D_800888D4, &D_800888E0, 4.0f, 6.0f, camera->SpringTime);
    func_1509BFB0(1, 0x30F5, 0x12, (u8)D_800888C0);
    func_1509BFB0(1, 0x30F4, 0x12, (u8)D_800888C4);
    func_1509BFB0(1, 0x30F3, 0x12, (u8)D_800888C8);
    func_1509BFB0(1, 0x30FA, 0x12, (u8)D_800888C0);
    func_1509BFB0(1, 0x30F9, 0x12, (u8)D_800888C4);
    func_1509BFB0(1, 0x30F8, 0x12, (u8)D_800888C8);
}

/* Steps four scroll counters by D_800BE9E4; then, unless movement_obj[0]'s mv->death_State is set, kills
 * movement_obj[0] once per prop 0xFA-0xF8 it stands on, and if it stands on prop 0xFB plays sound 0x627 and
 * runs it through func_15136C3C .. actor_kill(6). */
void func_150D0E90(s32 arg0) {
    WorldObject *prop;
    s32 propIndex;
    s32 i;

    D_80088890 += D_800BE9E4 * 40;
    D_80088894 += D_800BE9E4 * -66;
    D_80088898 += D_800BE9E4 * -8;
    D_8008889C += D_800BE9E4 * 92;

    if (movement_obj->mv->death_State == 0) {
        for (i = 0; i < 3; i++) {
            prop = func_151149AC(0xFA - i);
            propIndex = prop - db_Object_array;
            if (gWorldObjectContacts[propIndex] & 1) {
                func_151951E0(movement_obj);
                sound_playActorSound2(0x627, movement_obj, 0x7FFF, 200, 700);
            }
        }

        prop = func_151149AC(0xFB);
        propIndex = prop - db_Object_array;
        if (gWorldObjectContacts[propIndex] & 1) {
            sound_playActorSound2(0x627, movement_obj, 0x7FFF, 200, 700);
            func_15136C3C(movement_obj, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(movement_obj);
            actor_kill(movement_obj, 6);
        }
    }
}

/* Spawns a blood strip on obj that uses sample callback 6 and emit callback 7 (below);
 * mirrored selects which way it is offset. */
void func_150D10E4(Actor *obj, u8 mirrored) {
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
    func_15134DAC(&desc, 0);
}

/* BloodStrip sample callback 6: random power in [0.35, 0.5). */
s32 func_150D11B4(BloodStrip *strip) {
    strip->power = (func_150ADA68() * 150.0f + 350.0f) * 0.001f;
    return 1;
}

/* BloodStrip emit callback 7: moves both strip points `time` along their velocities and emits one
 * func_151DB5D0 particle at A, heading from A to B scaled by the strip's power. */
void func_150D1204(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
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

    random0 = func_150ADA68();
    random1 = func_150ADA68();
    randomInt = func_150ADA20();

    func_151DB5D0(0, &start.x, &velocity.x, random0 * 61.0f + 60.0f, 1.03571403f, 0.9704080224f,
                  random1 * 0.1570000052f + -0.08000000566f, (randomInt & 0xF) + 35,
                  func_150ADA20() % 156U + 100, 30, 8, 0, strip->header.owner, strip->header.kind);
}
