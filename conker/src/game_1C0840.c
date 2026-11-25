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
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 cooldown;
    /* 0x10 */ u8 pad10[0x18];
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
    func_15168800(&spawn, ((ObjectEmitter *)pos)->unkC, ((ObjectEmitter *)pos)->unk1);
    emitter->cooldown = (func_150ADA20() & 0x7F) + 100;
}

void func_151934B4(struct42 *arg0) {
    f32 sp38[4];

    if (arg0->unkA4 < arg0->unk96) {
        arg0->unkA4 = ((*(s16 *)((u8 *)arg0 + 0xAA) >> 2) * 24) / 3;
        arg0->unkA2 = ((*(s16 *)((u8 *)arg0 + 0xAA) >> 2) * 6) / 3;
        *(s16 *)((u8 *)arg0 + 0xAA) = *(s16 *)((u8 *)arg0 + 0xAA) + D_800BE9E4;
        return;
    }

    arg0->unk94++;
    if (arg0->unk94 >= 0x15) {
        arg0->unk94 = 0x14;
    }

    arg0->unk9E = arg0->unk9E - (((s32)((u32) arg0->unk94 * D_800BE9E4)) >> 1);
    if (arg0->unk9E < arg0->unkA6) {
        if (*(s8 *)((u8 *)arg0 + 0xB3) == 1) {
            sp38[0] = (f32)arg0->unk9C;
            sp38[1] = (f32)arg0->unkA6;
            sp38[2] = (f32)*(s16 *)((u8 *)arg0 + 0xA0);
            func_151DBCBC(5, 30.0f, 0xFF, 0, sp38, *(u8 *)((u8 *)arg0 + 0xC), *(u8 *)((u8 *)arg0 + 1));
        }
        *(s16 *)((u8 *)arg0 + 0x98) = -1;
    }
}
