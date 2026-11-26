#ifndef CONKER_ANIM_H
#define CONKER_ANIM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "anim": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_36280.c
 *   game_6C960.c
 *   game_A9260.c
 *   game_AC030.c
 *   game_1A8060.c
 */

/* game/done/game_36280.c */
void func_15008DD0(void);

/* game_6C960.c */
void func_1503F4B0(AnimInstance *anim);
void func_1503F5B8(AnimInstance *model, s32 restart, s32 anim, f32 speed, f32 blendFrames, s32 flag8000);
s32 func_1503F62C(s32 model, s32 bank, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg5, AnimInstance **out);
void func_1503F7B8(AnimInstance *model);

/* game_A9260.c */
void func_1507BDB0(AnimInstance *anim, f32 rate, Actor *obj, s32 mode);
void func_1507C22C(s32 onlyType4);
void func_1507C324(Actor *dst, Actor *src);

/* game_AC030.c */
void func_1507EB80(u8 *list, s32 *count, u8 seq);
void func_1507EBB8(u8 *list, s32 *count, s32 category);
s32 func_1507EC38(u8 *candidates, s32 numCandidates, u8 *out, s32 *outLen, u8 *recent);
void func_1507EE58(s32 seq, u8 *recent); /* K&R definition: seq is promoted */
void func_1507EEB8(u8 seq, u8 *recent);
void func_1507EEF4(void);
void func_1507EFA0(s32 seq, u8 *recent);
void func_1507EFD0(void);
s32 func_1507F454(void);
s32 func_1507F4C0(s32 phase);
void func_1507F54C(Actor *actor);
void func_1507F640(void);
void func_1507FC2C(Actor *actor);
void func_1507FEA0(Actor *actor);
void func_1507FF94(Actor *actor);
s32 func_1507FFD8(Actor *actor);

/* game_1A8060.c */
void func_1517ABB0(void);
s32 func_1517AD00(s32 frame, s32 duration, s32 actorIndex);

#endif
