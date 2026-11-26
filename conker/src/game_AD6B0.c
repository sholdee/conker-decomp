#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Scripted expression control for the current object.
 */

extern s32 D_8009BD10[];
extern s32 D_800D1928;
extern s32 D_800D192C;
extern s32 D_800D1930;

/* Resets the expression mode, cycle index and D_800D1930. */
void func_15080200(void) {
    D_800D1930 = D_800D1928 = D_800D192C = 0;
}

/* Drains queued 0x100-byte messages; type 0x4C ones set the current object's expression. */
void func_15080228(void) {
    u8 message[0x100];
    s32 morphDuration;

    while (func_100126E8(message, sizeof(message)) != 0) {
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
            func_15080430(&movement_obj[D_800D18D0], message[3] - 1, morphDuration);
        }
    }
}

/*
 * Per-frame update while mode D_800D1928 is set: mode 1 sets unk1FF from D_800427F0. Mode 2, once
 * D_800427F0 exceeds 1000 and morphDuration is below the frame step, moves unk134 to the next of
 * the 5 D_8009BD10 values with a random morphDuration of 4-10. Both clear D_800427F0.
 */
void func_15080348(Actor *actor) {
    s32 index;
    s32 countdown;

    if (D_800D1928 != 0 && actor != NULL) {
        if (D_800D1928 == 1) {
            if (D_800427F0 < 1000) {
                actor->levelMode = 0;
            } else {
                actor->levelMode = 1;
            }
        } else {
            index = D_800D192C;
            if (D_800427F0 > 1000) {
                countdown = actor->morphDuration;
                index++;
                countdown -= D_800BE9E4;
                if (countdown < 0) {
                    if (index >= 5) {
                        index = 0;
                    }
                    actor->unk134 = D_8009BD10[index];
                    actor->morphDuration = (func_151EF610() % 7) + 4;
                    D_800D192C = index;
                }
            }
        }
        D_800427F0 = 0;
    }
}

/*
 * Applies a scripted expression: 0 resets to expression 0, 1 starts the func_15080348 cycle (mode 2),
 * others are set directly with morphDuration clamped to 1-20. Objects without an expression table
 * only get mode 1 (expression != 0) or 0.
 */
void func_15080430(Actor *actor, s32 expression, s32 morphDuration) {
    s32 enabled;

    if (actor != NULL) {
        if (func_1507E968(actor) > 0) {
            if (expression == 0) {
                D_800D1928 = 0;
                actor->expressionTimer = 20;
                func_1507E500(actor, 0, 10);
            } else {
                actor->expressionTimer = 0xFFFE; // frozen
                if (expression == 1) {
                    D_800D1928 = 2;
                } else {
                    if (morphDuration != 0) {
                        if (morphDuration > 20) {
                            morphDuration = 20;
                        }
                        if (morphDuration <= 0) {
                            morphDuration = 1;
                        }
                    }
                    func_1507E500(actor, expression, morphDuration);
                }
            }
        } else {
            // matching: assigning and testing D_800D1928 directly mismatches (grows the function)
            enabled = (expression != 0);
            D_800D1928 = enabled;
            if (!enabled) {
                actor->levelMode = 0;
            }
        }
    }
}
