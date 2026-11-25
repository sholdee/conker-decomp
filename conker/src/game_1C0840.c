#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_1516865C(void *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_15168800(void *spawn, s32 arg1, s32 arg2);

/* Object-spawn request for func_1516865C/func_15168800 (Struct150E4550 in game_111670.c). */
typedef struct ObjectSpawn {
    /* 0x00 */ u8 pad0[0x86];
    /* 0x86 */ s16 unk86;
    /* 0x88 */ u8 pad88[4];
    /* 0x8C */ s16 x;
    /* 0x8E */ s16 y;
    /* 0x90 */ s16 z;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ s16 floorY;
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[6];
    /* 0xA0 */ u8 objectType;
    /* 0xA1 */ u8 padA1;
    /* 0xA2 */ u8 unkA2;
    /* 0xA3 */ u8 unkA3;
    /* 0xA4 */ u8 unkA4;
    /* 0xA5 */ u8 padA5[0xB]; // matching: the frame needs 0xB0 bytes, though only 0xA8 are cleared
} ObjectSpawn;

typedef struct EmitterPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 floorY; // -10000 = no floor
    /* 0x08 */ u8 unk8;
} EmitterPos;

typedef struct ObjectEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterPos pos;
} ObjectEmitter;

/*
 * Spawns object type 0x45 at the emitter's position, resting on its floor height (or 200 units
 * below it, clearing pos.unk8, when it has none), then rearms the emitter's cooldown to 100..227.
 */
// matching: volatile keeps `emitter` in its stack slot (reloaded after each call) instead of $s0
void func_15193390(ObjectEmitter *volatile emitter) {
    ObjectSpawn spawn;
    EmitterPos *pos;
    s16 floorY;

    bzero(&spawn, 0xA8);
    pos = &emitter->pos;
    floorY = pos->floorY;
    if (floorY != -10000.0f) {
        spawn.floorY = floorY;
    } else {
        spawn.floorY = (f32)pos->y - 200.0f;
        pos->unk8 = 0;
    }

    spawn.x = pos->x;
    spawn.y = pos->y;
    spawn.z = pos->z;
    spawn.unk92 = 0;
    spawn.unk94 = 0;
    spawn.unkA2 = 15;
    spawn.objectType = 0x45;
    spawn.unk86 = 10;
    spawn.unkA3 = pos->unk8;
    spawn.unk98 = 0x4880;
    spawn.unkA4 = 0x80;

    func_1516865C(&spawn, 0xFF, 0xFF, 0xFF, 0x80);
    // matching: one load of the volatile `emitter` serves both arguments
    pos = (EmitterPos *)emitter;
    func_15168800(&spawn, ((ObjectEmitter *)pos)->base.header.owner, ((ObjectEmitter *)pos)->base.header.kind);
    emitter->base.header.state = (func_150ADA20() & 0x7F) + 100;
}

typedef struct FallingObject {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x87];
    /* 0x94 */ s16 fallSpeed;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 x;
    /* 0x9E */ s16 y;
    /* 0xA0 */ s16 z;
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 floorY;
    /* 0xA8 */ u8 padA8[2];
    /* 0xAA */ s16 timer;
    /* 0xAC */ u8 padAC[7];
    /* 0xB3 */ s8 unkB3;
} FallingObject;

/*
 * While unkA4 is below unk96, grows unkA4/unkA2 with the timer. After that the object falls with
 * a speed that ramps up to 20; once it drops below floorY it sets unk98 to -1, first spawning a
 * func_151DBCBC effect at the landing point when unkB3 is 1.
 */
void func_151934B4(FallingObject *obj) {
    f32 pos[4]; // matching: a 4th (unused) slot keeps pos at its original stack offset

    if (obj->unkA4 < obj->unk96) {
        obj->unkA4 = ((obj->timer >> 2) * 24) / 3;
        obj->unkA2 = ((obj->timer >> 2) * 6) / 3;
        obj->timer += D_800BE9E4;
        return;
    }

    obj->fallSpeed++;
    if (obj->fallSpeed > 20) {
        obj->fallSpeed = 20;
    }

    obj->y -= (obj->fallSpeed * D_800BE9E4) >> 1;
    if (obj->y < obj->floorY) {
        if (obj->unkB3 == 1) {
            pos[0] = obj->x;
            pos[1] = obj->floorY;
            pos[2] = obj->z;
            func_151DBCBC(5, 30.0f, 0xFF, 0, pos, obj->unkC, obj->unk1);
        }
        obj->unk98 = -1;
    }
}
