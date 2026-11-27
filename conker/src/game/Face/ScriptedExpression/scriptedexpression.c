#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
u8 gExpressionTarget;       // the object whose expression the messages set (index into movement_obj)
u8 D_800D18D8[0x50]; /* not referenced */
static s32 gScriptedExpressionMode;
static s32 gScriptedExpressionCycleIndex;
static s32 D_800D1930;

#include "functions.h"
#include "variables.h"

/*
 * Scripted expression control for the current object.
 */

/* The expressions that mode 2 cycles through. */
static const s32 gScriptedExpressionCycle[5] = { 0, 1, 2, 3, 4 };

/* Resets the expression mode, cycle index and D_800D1930. */
void face_resetScripted(void) {
    D_800D1930 = gScriptedExpressionMode = gScriptedExpressionCycleIndex = 0;
}

/* Drains queued 0x100-byte messages; type 0x4C ones set the current object's expression. */
void face_processMessages(void) {
    u8 message[0x100];
    s32 morphDuration;

    while (mp3_popMessage(message, sizeof(message)) != 0) {
        if (message[0] == 0x4C) {
            // 28-bit value packed 7 bits per byte, most significant first
            morphDuration = message[4] & 0x7F;
            morphDuration = (message[5] & 0x7F) | (morphDuration << 7);
            morphDuration = (morphDuration << 7) | (message[6] & 0x7F);
            morphDuration = (morphDuration << 7) | (message[7] & 0x7F);
            morphDuration /= 367;
            if (morphDuration == 0) {
                morphDuration = 20;
            }
            face_applyScriptedExpression(&movement_obj[gExpressionTarget], message[3] - 1, morphDuration);
        }
    }
}

/*
 * Per-frame update while mode gScriptedExpressionMode is set: mode 1 sets unk1FF from D_800427F0. Mode 2, once
 * D_800427F0 exceeds 1000 and morphDuration is below the frame step, moves unk134 to the next of
 * the 5 gScriptedExpressionCycle values with a random morphDuration of 4-10. Both clear D_800427F0.
 */
void face_updateScripted(Actor *actor) {
    s32 index;
    s32 countdown;

    if (gScriptedExpressionMode != 0 && actor != NULL) {
        if (gScriptedExpressionMode == 1) {
            if (D_800427F0 < 1000) {
                actor->levelMode = 0;
            } else {
                actor->levelMode = 1;
            }
        } else {
            index = gScriptedExpressionCycleIndex;
            if (D_800427F0 > 1000) {
                countdown = actor->morphDuration;
                index++;
                countdown -= gFrameTicks;
                if (countdown < 0) {
                    if (index >= 5) {
                        index = 0;
                    }
                    actor->unk134 = gScriptedExpressionCycle[index];
                    actor->morphDuration = (math_rand() % 7) + 4;
                    gScriptedExpressionCycleIndex = index;
                }
            }
        }
        D_800427F0 = 0;
    }
}

/*
 * Applies a scripted expression: 0 resets to expression 0, 1 starts the face_updateScripted cycle (mode 2),
 * others are set directly with morphDuration clamped to 1-20. Objects without an expression table
 * only get mode 1 (expression != 0) or 0.
 */
void face_applyScriptedExpression(Actor *actor, s32 expression, s32 morphDuration) {
    s32 enabled;

    if (actor != NULL) {
        if (face_getExpressionCount(actor) > 0) {
            if (expression == 0) {
                gScriptedExpressionMode = 0;
                actor->expressionTimer = 20;
                face_setExpression(actor, 0, 10);
            } else {
                actor->expressionTimer = 0xFFFE; // frozen
                if (expression == 1) {
                    gScriptedExpressionMode = 2;
                } else {
                    if (morphDuration != 0) {
                        if (morphDuration > 20) {
                            morphDuration = 20;
                        }
                        if (morphDuration <= 0) {
                            morphDuration = 1;
                        }
                    }
                    face_setExpression(actor, expression, morphDuration);
                }
            }
        } else {
            // matching: assigning and testing gScriptedExpressionMode directly mismatches (grows the function)
            enabled = (expression != 0);
            gScriptedExpressionMode = enabled;
            if (!enabled) {
                actor->levelMode = 0;
            }
        }
    }
}
