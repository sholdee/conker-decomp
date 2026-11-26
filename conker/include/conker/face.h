#ifndef CONKER_FACE_H
#define CONKER_FACE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "face": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_AB760.c
 *   game_AD6B0.c
 */

/* One entry of a model's expression table (game_AB760.c). */
typedef struct {
    /* 0x0 */ u8 eyeCode[2];    // stored +10 into Actor.eyeCode
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 morphDuration; // default for Actor.morphDuration
    /* 0x4 */ u8 action;        // 1-based index into the action table, 0 = none
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 actionParam;  // thousandths, passed to collision_createPartSet as a float
    /* 0x8 */ u8 unk8;          // 0 = use the model default
    /* 0x9 */ u8 unk9;          // 0 = use the model default
} ExpressionRecord; // size 0xA

/* game_AB760.c */
void func_1507E2B0(Actor *actor);
void func_1507E3C0(Actor *actor);
void func_1507E500(Actor *actor, s32 index, s32 morphDuration);
void func_1507E5C8(Actor *actor, s32 morphDuration);
s32 func_1507E6B8(Actor *actor);
void func_1507E73C(Actor *actor);
void func_1507E7E4(Actor *actor, u8 index, u8 priority, u16 timer, s32 morphDuration);
ExpressionRecord *func_1507E908(Actor *actor, s32 index);
s32 func_1507E968(Actor *actor);
void func_1507E9E8(Actor *actor, s32 index);
const u8 *func_1507E9F8(Actor *actor, s32 *count);
void func_1507EA44(Actor *actor, s32 action, s32 param);
void func_1507EABC(Actor *actor);
void func_1507EB2C(Actor *actor);
void func_1507EB4C(Actor *actor, s32 expression);
extern BoundCylinderSet *D_800D18B0; // per-level actor-confining cylinder sets (collision_loadCylinderSets loads, actor_clampToZoneSet uses)

/* game_AD6B0.c */
void func_15080200(void);
void func_15080228(void);
void func_15080348(Actor *actor);
void func_15080430(Actor *actor, s32 expression, s32 morphDuration);
extern u8 D_800D18D0; // the object whose expression the messages set (index into movement_obj)

#endif
