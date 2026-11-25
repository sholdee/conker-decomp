#ifndef CONKER_PIPEEXPLODE_H
#define CONKER_PIPEEXPLODE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "pipeexplode": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_DFBF0.c
 */

/* The chunk object func_15132A4C creates; func_150B36AC is its floor-hit callback. */
typedef struct PipeChunk {
    /* 0x000 */ EffectHeader header;
    /* 0x010 */ f32 unk10;
    /* 0x014 */ f32 restitution;
    /* 0x018 */ u8 pad18[0x3C - 0x18];
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ u8 pad40[0x44 - 0x40];
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f unk50;
    /* 0x05C */ u8 pad5C[0x60 - 0x5C];
    /* 0x060 */ s32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0x170 - 0x66];
    /* 0x170 */ f32 minBounceVelY; // a hit with vel.y above this stops the chunk
} PipeChunk;

/* Per-actor effect state reached through PipeExplodeActor::state. */
typedef struct PipeExplodeState {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[5];
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ u8 unk20;
} PipeExplodeState;

typedef struct PipeExplodePoint {
    f32 x;
    f32 y;
    f32 z;
    f32 unk0C;
    f32 unk10;
} PipeExplodePoint;

typedef struct PipeExplodeActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ PipeExplodePoint *points;
    /* 0x98 */ PipeExplodeState *state;
} PipeExplodeActor;

/* game_DFBF0.c */
void func_150B2740(u8 pipe, u8 arg1);
s32 func_150B2EB4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE);
s32 func_150B3188(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE);
s32 func_150B36AC(PipeChunk *chunk, s32 arg1, s32 arg2, s32 arg3, f32 arg4);
s32 func_150B378C(PipeExplodeActor *actor);
s32 func_150B37C8(PipeExplodeActor *actor, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5);

#endif
