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

/* game/done/game_39690.c */
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
s32 func_15178750(s32 arg0, SwarmMember *member, s16 view);
void func_151787A4(void);
void func_151787AC(Swarm *swarm, s32 x, s32 y, s32 z);
Swarm *func_15178B98(u8 tag);
void func_15178BE4(u8 tag, f32 *targetPos, s16 yOffset);
void func_15178C34(u8 tag, s32 x, s32 y, s32 z, s16 yOffset);
s32 func_15178C9C(u8 tag, s32 index);
void func_15178DA4(Swarm *swarm);
void func_15178E14(u8 tag);

/* game_1B0740.c */
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
