#ifndef CONKER_SWARM_H
#define CONKER_SWARM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "swarm": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_39690.c
 *   game_1A20A0.c
 *   game_1A5440.c
 *   game_1B0740.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct SwarmActor SwarmActor;
typedef struct Swarm Swarm;
typedef struct SwarmMember SwarmMember;

/* A flying swarm member (bat/bird; object types 10, 0x13 and those func_15175390 is given): spawned by
 * func_15174BF0 / func_15174FA4 / func_15175390, updated by func_15175958 and friends, drawn by func_151770C8 / func_15176DF0. The flamethrower jet
 * (func_1519897C) sets the ones of types 0x15-0x17 that it touches on fire. */
struct SwarmActor {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Mtx mtx[2];           /* double-buffered by D_800BE9C0 */
    /* 0x90 */ u8 group;             /* index into D_800DDF5C / D_800DDF60 */
    /* 0x91 */ u8 state;
    /* 0x92 */ s16 flags;
    /* 0x94 */ Vtx *vertices;        /* bound to segment 1 when drawn */
    /* 0x98 */ f32 x, y, z;
    /* 0xA4 */ s16 speed;
    /* 0xA6 */ s16 speedStep;
    /* 0xA8 */ f32 pitch, yaw, roll;
    /* 0xB4 */ f32 pitchStep, yawStep, rollStep;
    /* 0xC0 */ f32 scale;
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ f32 groundHeight;
    /* 0xCC */ f32 perchPitchStep;   /* head-bob pitch rate while perched (func_1517685C) */
    /* 0xD0 */ s16 originX, originY, originZ;
    /* 0xD6 */ s16 maxRadius, minRadius, heightRange;
    /* 0xDC */ s16 turnTimer, hoverTimer;
    /* 0xE0 */ f32 targetYaw;
    /* 0xE4 */ f32 targetY;
    /* 0xE8 */ u32 surfaceFlags;
    /* 0xEC */ u8 brightness;
    /* 0xED */ u8 modelIndex;        /* index into D_800DDE80 / D_8008D0B0 */
    /* 0xEE */ u16 sound;
    /* 0xF0 */ u8 alpha;
    /* 0xF1 */ u8 fadingOut;
};

/* game/done/game_39690.c */
extern const ParticleTexInfo D_80095CE0[69];
void func_1500C1E0(void);

/* game_1A20A0.c */
void func_15174BF0(s32 count, s32 group, s32 x, s32 y, s32 z, s32 player, f32 minScale, f32 scaleRange, s32 flags);
void func_15174FA4(s32 count, s32 group, s32 x, s32 y, s32 z, s32 spread, s32 arg6, s32 airborne, f32 minScale,
                   f32 scaleRange);
void func_15175390(s32 count, s32 group, s32 x, s32 y, s32 z, s32 maxRadius, s32 player, s32 heightRange, s32 minRadius,
                   s32 flags, s32 baseType, u32 typeCount, s32 baseModel, s32 modelMask, s32 state, f32 minScale,
                   f32 scaleRange, s32 spawnFlags);
void func_15175958(SwarmActor *actor);
void func_1517685C(SwarmActor *actor);
void func_15176B84(SwarmActor *actor);
Gfx *func_15176DF0(Gfx *gdl, SwarmActor *actor, s16 camera);
Gfx *func_151770C8(Gfx *gdl, SwarmActor *actor, s16 camera);
void func_1517725C(u8 index);

/* game_1A5440.c */
void func_15177F90(u8 memberCount, f32 *targetPos, u8 tag, s32 x, s32 y, s32 z, s32 arg6, s32 arg7, u8 arg8, s32 playSfx,
                   s32 argA);
void func_15178268(SwarmMember *member);
Gfx *func_15178750(Gfx *gfx, SwarmMember *member, s16 view);
void func_151787A4(void);
void func_151787AC(Swarm *swarm, s32 x, s32 y, s32 z);
Swarm *func_15178B98(u8 tag);
void func_15178BE4(u8 tag, f32 *targetPos, s16 yOffset);
void func_15178C34(u8 tag, s32 x, s32 y, s32 z, s16 yOffset);
s32 func_15178C9C(u8 tag, s32 index);
void func_15178DA4(Swarm *swarm);
void func_15178E14(u8 tag);

/* game_1B0740.c */
extern EffectModelSlot D_800DDE80[11]; /* one loaded model per effect model slot */
extern u8 D_800DDF5C[4];               /* per swarm group: in use */
extern u8 D_800DDF60[4];               /* per swarm group: countdown */
extern s8 D_800DDF68[11];              /* per slot: use state (1 = used this frame, -2 = unused) */
extern Gfx *D_800DDF78[11];            /* per slot: the model's second display list */
s32 func_15183290(s32 x, s32 y, s32 z, s32 count, s32 type, s32 radius, s32 player, s32 maxRadius, s32 heightRange,
                  s32 minRadius, s32 spawnFlags);
void func_151838B0(s32 slot);
void func_15183974(s32 slot);
void func_151839F0(s32 slot, s32 extraSlot);
void func_15183ACC(s32 slot);
Gfx *func_15183BA4(Gfx *gdl, s32 modelId, s32 arg2, s32 arg3);
void func_15183C28(s32 slot);
void func_15183D28(void);
s32 func_15184118(Actor *obj);

#endif
