#include <ultra64.h>
#include <math.h>
#include "conker/bone.h"
#include "conker/collision.h"
#include "conker/game.h"

static s16 gBoneLastChannelLevel; // the last channel level that bone_addLevelCommands wrote; bone_lowerBones lowers bones by it

/*
 * Writes up to three commands for the actor's two channels (index 0xFF = channel unused) and
 * advances the per-channel levels for the next call. Returns the cursor past the last command.
 */
GameCommand *bone_addLevelCommands(GameCommand *cmd, Actor *actor, s32 channel, s32 channel2) {
    s32 skipChannel;
    s32 padA; // matching: unused stack slot keeps the frame layout
    s32 reroll;
    f32 scale;
    s32 minStep;
    s32 maxStep;
    s32 levelOp;
    s32 padB; // matching: unused stack slot keeps the frame layout
    s32 jitter;
    s32 sign;
    s32 value;
    s32 reverse;
    s16 pos;
    s8 step;
    s16 sum;

    if (actor->character_type == 0x70) {
        skipChannel = TRUE;
    } else {
        skipChannel = FALSE;
    }
    if (actor->character_type == 0x7C) {
        levelOp = 4;
        maxStep = 0;
    } else {
        levelOp = 0;
        maxStep = 2;
    }
    switch (actor->character_type) {
    case 0x69:
    case 0x7C:
    case 0x87:
        jitter = 0;
        break;
    default:
        jitter = 10;
        break;
    }
    if (channel != 0xFF) {
        if (actor->levelRange == 0) {
            scale = 8191.875f;
        } else {
            scale = actor->levelRange * 182.04444885f;
        }
        value = (s16)(actor->level * scale * (1.0f / 255));
        if (!skipChannel) {
            cmd->opcode = channel * 6 + levelOp;
            cmd->value = value;
            cmd++;
        }
        gBoneLastChannelLevel = value;
        value = (s16)((actor->levelJitter * actor->level) / 255 * 65535.0f * (1.0f / 255));
        if (!skipChannel) {
            // matching: maxStep doubles as the second opcode offset; a separate local changes regalloc
            cmd->opcode = channel * 6 + maxStep;
            cmd->value = value;
            cmd++;
        }
        if (actor->character_type == 0x42) {
            minStep = 80;
            maxStep = 120;
        } else {
            maxStep = 60;
            minStep = 40;
        }
        if (actor->levelMode == 0) {
            actor->levelStep = -maxStep;
        } else if (actor->levelMode == 2) {
            actor->levelStep = maxStep;
            actor->levelJitter = 0;
        } else if (actor->levelStep == 0 || actor->level == 0 || actor->level == 0xFF) {
            actor->levelStep = math_random() % (u32)(maxStep - minStep) + minStep;
            if (actor->level == 0xFF) {
                actor->levelStep = -actor->levelStep;
            } else if (actor->level == 0 && jitter != 0) {
                actor->levelJitter = math_random() % (u32)(jitter * 2) - jitter;
            }
        }
        sum = actor->level;
        sum += actor->levelStep;
        sum = (sum < 0) ? 0 : ((sum >= 0x100) ? 0xFF : sum);
        actor->level = sum;
        if (actor->levelMode == 3) {
            if (actor->levelTimer >= gFrameTicks) {
                actor->levelTimer -= gFrameTicks;
            } else {
                actor->levelTimer = 0;
                actor->levelMode = 0;
            }
        }
    }
    if (channel2 != 0xFF) {
        if (actor->level2Range == 0) {
            scale = 8191.875f;
        } else {
            scale = actor->level2Range * 182.04444885f;
        }
        sign = (scale < 0.0f) ? -1 : 1;
        scale = fabsf(scale);
        sum = actor->level2 * scale * (1.0f / 255);
        cmd->opcode = channel2 * 6 + levelOp;
        reroll = FALSE;
        reverse = FALSE;
        cmd->value = -sum;
        cmd++;
        sum = actor->level2Mode; // matching: reusing sum (not a fresh local or a direct read) keeps IDO's allocation
        if (sum == 0) {
            if (sign > 0) {
                actor->level2Step = -60;
            } else {
                actor->level2Step = 60;
            }
        } else if (sum == 2) {
            if (sign > 0) {
                actor->level2Step = 60;
            } else {
                actor->level2Step = -60;
            }
        } else if ((step = actor->level2Step) == 0) {
            reroll = TRUE;
        } else {
            pos = actor->level2;
            if (sign > 0) {
                if (pos < 0 && step < 0) {
                    reverse = TRUE;
                } else if (pos == 0 && step < 0) {
                    reroll = TRUE;
                } else if (pos * 1 == 0xFF) { // matching: non-leaf operand puts the constant's register first in bne
                    reverse = TRUE;
                    reroll = TRUE;
                }
            } else {
                if (pos > 0 && step > 0) {
                    reverse = TRUE;
                } else if (pos == 0 && step > 0) {
                    reroll = TRUE;
                } else if (pos == -0xFF) {
                    reverse = TRUE;
                    reroll = TRUE;
                }
            }
        }
        if (reroll) {
            actor->level2Step = math_random() % 20U + 40;
            actor->level2Step = actor->level2Step * sign;
        }
        if (reverse) {
            actor->level2Step = -actor->level2Step;
        }
        actor->level2 += actor->level2Step;
        if (sign > 0) {
            if (actor->level2 < 0 && actor->level2Step < 0) {
                actor->level2 = 0;
            } else if (actor->level2 >= 0x100) {
                actor->level2 = 0xFF;
            }
        } else {
            if (actor->level2 > 0 && actor->level2Step > 0) {
                actor->level2 = 0;
            } else if (actor->level2 < -0xFF) {
                actor->level2 = -0xFF;
            }
        }
        if (actor->level2Mode == 3) {
            if (actor->level2Timer >= gFrameTicks) {
                actor->level2Timer -= gFrameTicks;
            } else {
                actor->level2Timer = 0;
                actor->level2Mode = 0;
            }
        }
    }
    return cmd;
}

/* Lowers bone `first` (and bone `second`, unless -1) by the last value stored in gBoneLastChannelLevel, scaled by the
   actor's scale_xz: subtracts it from the y translation of the bone matrix. */
void bone_lowerBones(Actor *actor, s32 first, s32 second) {
    f32 amount;
    MtxF *bone;
    MtxF *bone2;
    MtxF *bones;

    if (gBoneLastChannelLevel != 0) {
        bones = actor->ob_matrix_ptr;
        amount = (gBoneLastChannelLevel * 0.0007629627362f) * actor->scale_xz;
        bone = &bones[first];
        (*bone)[3][1] -= amount;
        if (second != -1) {
            bone2 = &bones[second];
            (*bone2)[3][1] -= amount;
        }
    }
}
