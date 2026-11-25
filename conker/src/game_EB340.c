#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* The 8-byte payload func_150BDE90 copies to +0x28 of the object it creates. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ s16 timer; // countdown to the next emitter spawn
    /* 0x06 */ s16 unk6;
} EB340Spawn;

/* The object func_150BDE90 creates; func_150BDF0C is its per-frame update. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EB340Spawn spawn;
} EB340Obj;

/* Creates the spawner object for `owner`, with its spawn timer at 0. */
void func_150BDE90(Camera *owner, u8 arg1, s32 arg2) {
    EB340Spawn spawn;
    EB340Obj *obj;

    spawn.owner = owner;
    spawn.timer = 0;

    obj = (EB340Obj *)func_15149130(300, -1, 0x4F, -1, 0, 60, 8, arg1, arg2);
    if (obj != NULL) {
        memcpy(&obj->spawn, &spawn, sizeof(spawn));
    }
}

/* Copied to +0x70 of the emitter object func_1515548C returns. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ u8   pad4[0x8];
    /* 0x0C */ u8   unkC;
    /* 0x0D */ u8   unkD;
    /* 0x0E */ u8   padE[0x2];
    /* 0x10 */ f32  unk10;
    /* 0x14 */ u8   pad14[0x44];
} EB340Payload;

extern void *func_1515548C(ScreenSpriteInfo *, s32, s32, s32, s32, u8, s32);

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void func_150BDF0C(EB340Obj *obj) {
    EB340Spawn *spawn;
    ScreenSpriteInfo emitter;
    void *effect;
    EB340Payload payload;

    obj->spawn.timer -= D_800BE9E4;
    spawn = &obj->spawn;
    if (obj->spawn.timer < 0) {
        emitter.x = (func_150ADA68() * 270.0f) + -135.0f;
        emitter.y = -120.0f;
        emitter.halfWidth = (func_150ADA68() * 10.0f) + 3.0f;
        emitter.halfHeight = (func_150ADA68() * 17.0f) + 9.0f;
        emitter.texture = 0xAB;
        emitter.timer = 1000;
        emitter.flags = 0x31;
        emitter.fadeStart = 1;
        emitter.fadeRate = 0xFF;
        emitter.kind = 7;
        emitter.color1.red = 0xFF;
        emitter.color1.green = 0xFF;
        emitter.color1.blue = 0xFF;
        emitter.color1.alpha = (func_150ADA20() % 156U) + 100;
        emitter.color2.red = 0xFF;
        emitter.color2.green = 0xFF;
        emitter.color2.blue = 0xFF;
        emitter.color2.alpha = 0xFF;
        emitter.unk23 = 0xFF;
        emitter.otherModeH = 0;
        emitter.unk28 = 0x200004;
        emitter.unk2C = 0x1F0601;
        emitter.combineMode = 3;
        emitter.unk34 = 0x22;
        emitter.unk38 = 0x80;
        emitter.unk3C = 0x20;
        emitter.unk40 = 0;
        emitter.unk41 = 7;
        emitter.viewport = spawn->owner->CNumber;
        emitter.scaleX = 1.0f;
        emitter.scaleY = 1.0f;
        emitter.offsetS = 0.0f;
        emitter.offsetT = 0.0f;
        payload.owner = spawn->owner;
        payload.unkD = 0;
        payload.unkC = 0;
        payload.unk10 = (func_150ADA68() * 4.06400013f) + 2.513000011f;
        effect = func_1515548C(&emitter, 10, 0, 0, sizeof(emitter), obj->base.header.owner, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x70, &payload, sizeof(payload));
        }
        spawn->timer = (func_150ADA20() % 151U) + 25;
    }
}

typedef struct {
    /* 0x00 */ u8  pad0[0x14];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8  pad18[0x68];
    /* 0x80 */ f32 unk80;
} EB340Rising;

/* Behaviour scratch state (WorldObject.touch) of func_150BE210. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ f32 angle; // turns at spinY per frame until past 360
} TurnerWork;

#define TURNER_WORK(obj) ((TurnerWork *)&(obj)->touch)

void func_150BE210(WorldObject *obj);

/* Behaviour scratch state (WorldObject.touch) of func_150BE2E8: the end points of its path. */
typedef struct {
    /* 0x7C */ s16 from[3];
    /* 0x82 */ s16 to[3];
} MoverWork;

#define MOVER_WORK(obj) ((MoverWork *)&(obj)->touch)

// matching: 0.128f read through its rodata symbol; the plain literal allocates FP registers differently
void func_150BE2E8(WorldObject *mover);

/* Two-halfword command, as in game_61490.c's GameCommand. */
typedef struct {
    /* 0x00 */ s16 opcode;
    /* 0x02 */ s16 value;
} EB340Command;


