#ifndef CONKER_PIPEEXPLODE_H
#define CONKER_PIPEEXPLODE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "pipeexplode": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Effects/PipeExplode/pipeexplode.c
 */

/* The chunk object particle_newModelParticle creates; pipeexplode_onChunkFloorHit is its floor-hit callback. */
typedef struct PipeChunk {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ f32 minBounceVelY; // a hit with vel.y above this stops the chunk
} PipeChunk;

/* Per-actor effect state reached through PipeExplodeActor::state. */
typedef struct PipeExplodeState {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 velocityX; /* TrailMotion.velocity.x */
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ f32 velocityZ; /* TrailMotion.velocity.z */
    /* 0x10 */ f32 gravity;   /* TrailMotion.gravity */
    /* 0x14 */ u8 pad14[5];
    /* 0x19 */ u8 texture;    /* TrailMotion.texture */
    /* 0x1A */ u8 shade;      /* TrailMotion.shade */
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

/* game/Effects/PipeExplode/pipeexplode.c */
extern const PipeSpot gPipeSpots[3]; // the three exploding pipes
void pipeexplode_explode(u8 pipe, u8 owner);
s32 pipeexplode_spawnChunk(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s32 arg8, s32 baseAngle, f32 radius, s32 argB,
                  f32 argC, void *data, u8 owner);
s32 pipeexplode_spawnTunableChunk(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s32 arg8, s32 baseAngle, f32 radius, s32 argB,
                  f32 argC, void *data, u8 owner);
s32 pipeexplode_onChunkFloorHit(PipeChunk *chunk, f32 x, f32 y, f32 z, f32 floorHeight);
s32 pipeexplode_fadeByLife(PipeExplodeActor *actor);
s32 pipeexplode_spawnLandParticle(PipeExplodeActor *actor, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);

#endif
