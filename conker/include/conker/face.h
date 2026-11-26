#ifndef CONKER_FACE_H
#define CONKER_FACE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "face": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Face/Expression/expression.c
 *   game/Face/ScriptedExpression/scriptedexpression.c
 */

/* One entry of a model's expression table (game/Face/Expression/expression.c). */
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

/* game/Face/Expression/expression.c */
void face_updateBlink(Actor *actor);
void face_showEyeParts(Actor *actor);
void face_setExpression(Actor *actor, s32 index, s32 morphDuration);
void face_applyExpression(Actor *actor, s32 morphDuration);
s32 face_canBlink(Actor *actor);
void face_update(Actor *actor);
void face_requestExpression(Actor *actor, u8 index, u8 priority, u16 timer, s32 morphDuration);
ExpressionRecord *face_getExpressionRecord(Actor *actor, s32 index);
s32 face_getExpressionCount(Actor *actor);
void func_1507E9E8(Actor *actor, s32 index);
const u8 *face_getActionTable(Actor *actor, s32 *count);
void face_startAction(Actor *actor, s32 action, s32 param);
void face_revertExpression(Actor *actor);
void face_resetDefaultExpression(Actor *actor);
void face_setDefaultExpression(Actor *actor, s32 expression);
extern BoundCylinderSet *gCylinderSets; // per-level actor-confining cylinder sets (collision_loadCylinderSets loads, actor_clampToZoneSet uses)

/* game/Face/ScriptedExpression/scriptedexpression.c */
void face_resetScripted(void);
void face_processMessages(void);
void face_updateScripted(Actor *actor);
void face_applyScriptedExpression(Actor *actor, s32 expression, s32 morphDuration);
extern u8 gExpressionTarget; // the object whose expression the messages set (index into movement_obj)

#endif
