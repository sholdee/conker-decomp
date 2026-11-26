#include <ultra64.h>
#include "structs.h"

/*
 * This file's .bss, defined before the headers declare it (see init_50A0.c). Its code does not use it:
 * game/Collision/CylinderSets/cylindersets.c loads the table and game/Actor/ActorMotion/actormotion.c reads it. It lies between the .bss of game/Actor/KillRespawn/killrespawn and
 * of game_AC030 on its own 16-byte line, so this unit is the one that defines it.
 */
BoundCylinderSet *gCylinderSets; // per-level actor-confining cylinder sets (collision_loadCylinderSets loads, actor_clampToZoneSet uses)

#include "functions.h"
#include "variables.h"

/*
 * Character expressions and eye blinking.
 *
 * Each character model has a table of 10-byte expression records. The table is described by an
 * 8-byte header stored just before the model's gModelDefs entry.
 */

/* Header stored 8 bytes before a model's gModelDefs entry. */
typedef struct {
    /* 0x0 */ ExpressionRecord *records;
    /* 0x4 */ u32 size; // in bytes
} ExpressionTableHeader;



/* Advances eye blinking: random open periods of 10-149 frames, closed for one frame. */
void face_updateBlink(Actor *actor) {
    if ((actor->character_type != 0x2B) && (gCutsceneStates[0] != 1)) {
        if ((actor->playernumber != 0xFF) && (actor->mv->death_State != 0)) {
            actor->eyeClosed[0] = 1;
            actor->eyeClosed[1] = 1;
            return;
        }
        if (actor->eyeClosed[0] >= 3) {
            actor->eyeClosed[0] = 0;
        }
        if (actor->eyeClosed[1] >= 3) {
            actor->eyeClosed[1] = 0;
        }
        // matching: the dead `== 1U` makes IDO load the constant 1 (later stored) in this block
        if ((actor->blinkTimer >= D_800BE9E4) | (0 * (actor->blinkTimer == 1U))) {
            actor->blinkTimer -= D_800BE9E4;
            return;
        }
        actor->blinkTimer = 0;
        if (actor->eyeCode[0] != 1) {
            actor->eyeClosed[0] ^= 1;
            actor->eyeClosed[1] ^= 1;
            if (actor->eyeClosed[0] == 0) {
                actor->blinkTimer = (func_150ADA20() % 140U) + 10;
                return;
            }
            actor->blinkTimer = 1;
        }
    }
}

/* Models 0xF, 0x46 and 0x4C (Lady Cog per the other decomp): shows one eye part per eye. */
void face_showEyeParts(Actor *actor) {
    s32 i;
    s32 eyePart[2];
    u8 model;

    model = actor->character_type;
    if ((model == 0xF) || (model == 0x46) || (model == 0x4C)) {
        for (i = 0; i < 2; i++) {
            eyePart[i] = actor->eyeCode[i];
            if (eyePart[i] >= 10) {
                eyePart[i] -= 10;
                if (eyePart[i] == 5) {
                    eyePart[i] = 0;
                } else if (eyePart[i] == 1) {
                    eyePart[i] = 1;
                } else {
                    eyePart[i] = 2;
                }
            } else if (eyePart[i] < 2) {
                eyePart[i]++;
            }
        }
        actor->unk94 |= 0x7E;
        if (eyePart[0] == 0) {
            actor->unk94 &= ~8;
        } else if (eyePart[0] == 1) {
            actor->unk94 &= ~0x10;
        } else {
            actor->unk94 &= ~4;
        }
        if (eyePart[1] == 0) {
            actor->unk94 &= ~0x20;
        } else if (eyePart[1] == 1) {
            actor->unk94 &= ~0x40;
        } else {
            actor->unk94 &= ~2;
        }
    }
}

/* Switches to expression `index`, stopping the previous expression's action. */
void face_setExpression(Actor *actor, s32 index, s32 morphDuration) {
    ExpressionRecord *previous;
    const u8 *actions;

    if (index < face_getExpressionCount(actor)) {
        previous = face_getExpressionRecord(actor, actor->expression);
        if (previous->action != 0) {
            actions = face_getActionTable(actor, NULL);
            if (actions != NULL) {
                attach_removeSet(actor, actions[previous->action - 1]);
            }
        }
        actor->expression = index;
        face_applyExpression(actor, morphDuration);
        if (morphDuration != 0) {
            actor->morphDuration = morphDuration;
        } else {
            actor->morphDuration = face_getExpressionRecord(actor, index)->morphDuration;
        }
    }
}

/* Applies the current expression record: its action, eye codes and the 0x68/0x69 selectors. */
void face_applyExpression(Actor *actor, s32 morphDuration) {
    ExpressionRecord *record;

    record = face_getExpressionRecord(actor, actor->expression);
    if (record != NULL) {
        face_startAction(actor, record->action, record->actionParam);
        if (actor->unk134 != record->unk2) {
            actor->unk134 = record->unk2;
            if (morphDuration == 0) {
                actor->morphDuration = record->morphDuration;
            } else {
                actor->morphDuration = morphDuration;
            }
        }
        actor->eyeCode[0] = record->eyeCode[0] + 10;
        actor->eyeCode[1] = record->eyeCode[1] + 10;
        if (record->unk8 != 0) {
            actor->unk68[0] = record->unk8;
        } else {
            actor->unk68[0] = gModelDefs[actor->character_type]->eyeDefault[0];
        }
        if (record->unk9 != 0) {
            actor->unk68[1] = record->unk9;
        } else {
            actor->unk68[1] = gModelDefs[actor->character_type]->eyeDefault[1];
        }
    }
}

/* Whether the actor blinks: it must be alive and showing its default expression (or a blinking one). */
s32 face_canBlink(Actor *actor) {
    s32 model;
    s32 expression;

    if (actor->energy == 0) {
        return FALSE;
    }
    if (actor->expression == actor->defaultExpression) {
        return TRUE;
    }
    model = collision_getModelId(actor);
    expression = actor->expression;
    if (model == 0) {
        if (expression == 0x15) {
            return TRUE;
        }
    } else if (model == 0x52) {
        return TRUE;
    }
    return FALSE;
}

/* Per-frame expression update: counts down the expression timer, blinks, and reverts on timeout. */
void face_update(Actor *actor) {
    s32 timer;

    if (actor->drawtype != 2) {
        if (actor->expressionTimer != 0) {
            timer = actor->expressionTimer;
            if (timer == 0xFFFE) {
                return;
            }
            if (timer != 0xFFFF) {
                if (D_800BE9E4 < timer) {
                    actor->expressionTimer = timer - D_800BE9E4;
                } else {
                    actor->expressionTimer = 0;
                }
            }
        }
        if (face_canBlink(actor)) {
            face_updateBlink(actor);
        }
        if ((actor->expressionTimer == 0) && (actor->expression != actor->defaultExpression)) {
            face_revertExpression(actor);
        }
    }
}

/*
 * Requests expression `index` with a priority and a timeout (frames, 0xFFFF = none). It replaces
 * the current one if priority is 3, the actor shows its default or the same expression, the current
 * one has timed out, or the new priority is higher.
 */
void face_requestExpression(Actor *actor, u8 index, u8 priority, u16 timer, s32 morphDuration) {
    ExpressionRecord *previous;
    const u8 *actions;

    if (((index != actor->expression) || (priority != actor->expressionPriority) ||
         (timer != actor->expressionTimer)) &&
        ((priority == 3) || (actor->expression == actor->defaultExpression) ||
         (actor->expression == index) || (actor->expressionTimer == 0) ||
         (actor->expressionPriority < priority)) &&
        (index < face_getExpressionCount(actor))) {
        previous = face_getExpressionRecord(actor, actor->expression);
        if (previous->action != 0) {
            actions = face_getActionTable(actor, NULL);
            if (actions != NULL) {
                attach_removeSet(actor, actions[previous->action - 1]);
            }
        }
        actor->expression = index;
        actor->expressionTimer = timer;
        actor->expressionPriority = priority;
        face_applyExpression(actor, morphDuration);
    }
}

/* Returns the actor's expression record `index`, or NULL if its model has no table. */
ExpressionRecord *face_getExpressionRecord(Actor *actor, s32 index) {
    ExpressionTableHeader *header;
    ExpressionRecord *records;

    header = (ExpressionTableHeader *)gModelDefs[collision_getModelId(actor)];
    if (header != NULL) {
        header--;
        records = header->records;
        if (records != NULL) {
            return &records[index];
        }
    }
    return NULL;
}

/* Returns the number of expressions of the actor's model (model 0x96 uses its own table). */
s32 face_getExpressionCount(Actor *actor) {
    ExpressionTableHeader *header;
    s32 model;
    s32 id;

    id = actor->character_type;
    model = (id != 0x96) ? collision_getModelId(actor) : id;
    if (model == 0xFF) {
        return 0;
    }
    header = (ExpressionTableHeader *)gModelDefs[model];
    if (header != NULL) {
        header--;
        return header->size / sizeof(ExpressionRecord);
    }
    return 0;
}

/* Empty stub. */
void func_1507E9E8(Actor *actor, s32 index) {
}

/* Returns the table of expression action ids (only model 0 has one) and its length. */
const u8 *face_getActionTable(Actor *actor, s32 *count) {
    if (collision_getModelId(actor) == 0) {
        if (count != NULL) {
            *count = 5;
        }
        return gModel0ExpressionActions;
    }
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}

/* Starts expression action `action` (1-based) with `param` thousandths. */
void face_startAction(Actor *actor, s32 action, s32 param) {
    const u8 *actions;

    if (action != 0) {
        actions = face_getActionTable(actor, NULL);
        if (actions != NULL) {
            collision_createPartSet(actor, actions[action - 1], param * 0.001f, 0);
        }
    }
}

/* Reverts to the default expression and clears expression-set eye codes. */
void face_revertExpression(Actor *actor) {
    face_requestExpression(actor, actor->defaultExpression, 3, 0xFFFF, 10);
    actor->expressionPriority = 0;
    actor->expressionTimer = 0;
    if (actor->eyeCode[0] >= 10) {
        actor->eyeCode[0] = 0;
        actor->eyeClosed[0] = 0;
    }
    if (actor->eyeCode[1] >= 10) {
        actor->eyeCode[1] = 0;
        actor->eyeClosed[1] = 0;
    }
}

/* Resets the default expression to 0. */
void face_resetDefaultExpression(Actor *actor) {
    face_setDefaultExpression(actor, 0);
}

/* Sets the default expression and switches to it. */
void face_setDefaultExpression(Actor *actor, s32 expression) {
    if (expression != actor->defaultExpression) {
        actor->defaultExpression = expression;
        face_revertExpression(actor);
    }
}
