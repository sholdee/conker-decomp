#ifndef CONKER_BEAM_H
#define CONKER_BEAM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "beam": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_F2820.c
 *   game_108320.c
 *   game_10EF60.c
 *   game_11C2B0.c
 *   game_11EEB0.c
 *   game_1228D0.c
 *   game_124260.c
 *   game_12E540.c
 *   game_131F30.c
 *   game_133190.c
 *   game_1BBFC0.c
 *   game_1D4E00.c
 *   game_1D5FD0.c
 *   game_1E26F0.c
 */

/* Lightning bolts (game_1D4E00.c, game_1D5FD0.c). */
typedef struct {
    u8 r, g, b, a;
} BoltColor;

/* Spawn parameters of a lightning bolt; func_151A7950 copies them into Bolt.def. */
typedef struct {
    /* 0x00 */ s16 lifetime;         /* frames, counted down while BOLT_TIMED */
    /* 0x02 */ s16 numPoints;
    /* 0x04 */ f32 jitterDelay;      /* re-jitter after jitterDelay + rand * jitterDelayRange */
    /* 0x08 */ f32 jitterDelayRange;
    /* 0x0C */ u8 flags;             /* BOLT_TIMED | BOLT_ACTIVE */
    /* 0x10 */ f32 jitter;           /* largest sideways displacement of a midpoint */
    /* 0x14 */ BoltColor color;
    /* 0x18 */ f32 width;
    /* 0x1C */ s8 update1;           /* index into D_8008F940, -1 for none */
    /* 0x1D */ s8 update2;           /* index into D_8008F948, -1 for none */
    /* 0x20 */ Vec3f start;
    /* 0x2C */ Vec3f end;
    /* 0x38 */ Vec3f up;
    /* 0x44 */ f32 length;
    /* 0x48 */ f32 invLength;
    /* 0x4C */ u8 type;              /* index into the D_8008F94C/D_8008F958/D_8008F964 handlers */
} BoltDef; /* size 0x50 */

#define BOLT_TIMED  1
#define BOLT_ACTIVE 2

typedef struct {
    Vec3f local; /* x: sideways, y: along up, z: fraction of the way from start to end */
    Vec3f world;
} BoltPoint;

/* User data of a bolt that runs from an actor bone to a point near the actor (func_151A8624). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;           /* owner->unique_id at spawn; the bolt stops when it changes */
    /* 0x08 */ Vec3f boneOffset;
    /* 0x14 */ u8 bone;
    /* 0x18 */ Vec3f targetOffset;   /* end point, relative to the owner's position */
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 linkFlags;         /* 1: the end point has been ray-cast */
} BoltLink; /* size 0x28 */

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ BoltDef def;
    /* 0x60 */ BoltLink *link;       /* user data, stored after the points */
    /* 0x64 */ BoltPoint *points;
    /* 0x68 */ f32 jitterTimer;
    /* 0x6C */ u8 vtxBuf[0x10];
    /* 0x7C */ s32 vtxBufSize;
    /* 0x80 */ BoltPoint pointData[1];
} Bolt;

#endif
