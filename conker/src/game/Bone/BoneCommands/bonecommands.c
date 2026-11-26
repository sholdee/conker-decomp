#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Builds the command list for actor `index`, dispatching on its id, and terminates it with 0x1000. */
void bone_buildCommands(GameCommand *commands, s32 index) {
    Actor *actor;
    s32 first;
    GameCommand *cursor;

    actor = &movement_obj[index];
    cursor = commands;
    if (actor->ob_matrix_ptr == NULL) {
        commands->opcode = 0x1000;
        return;
    }
    if (actor->type == 1) {
        cursor = func_150341BC(commands, index);
    } else {
        switch (actor->character_type) {
        case 0x2B:
            cursor = func_150D8590(commands, index);
            break;
        case 0x28:
        case 0x77:
            cursor = func_150344A0(commands, index);
            break;
        case 0x4B:
            cursor = effect_emitFacingCommand(commands, index);
            break;
        case 0x7B:
            cursor = func_150F1B48(commands, index);
            break;
        case 0x8A:
            cursor = func_15034420(commands, index);
            break;
        case 0x21:
            cursor = func_150BE438(commands, index);
            break;
        case 0x98:
            cursor = func_15034340(commands, index);
            break;
        case 0x5A:
        case 0x74:
        case 0x7A:
        case 0x8D:
            cursor = func_150343B0(commands, index);
            break;
        }
    }
    first = func_1503DA3C(actor->character_type, 0);
    bone_addLevelCommands(cursor, actor, first, func_1503DA3C(actor->character_type, 1))->opcode = 0x1000;
}

/* Appends the commands for an actor in interaction state 1. Angles are in degrees, scaled to 65536/360 units. */
GameCommand *func_150341BC(GameCommand *cmd, s32 index) {
    Actor *actor;
    ActorCtl *sub;
    s16 angle;
    s32 kind;
    s32 offset;

    actor = &movement_obj[index];
    sub = actor->mv;
    if ((sub != NULL) && !(actor->unk2F8 & 0x80)) {
        // pad54 is declared u32 in structs.h but holds an f32 here
        angle = actor->unk54 * (65536.0f / 360.0f);
        kind = actor->animseq.uh;
        if ((kind != 0x42) && (kind != 0x14) && (kind != 0x76)) {
            offset = (sub->rt_context_b == 10) ? 0 : 4;
            cmd->opcode = offset + 6;
            cmd->value = angle;
            cmd++;
        } else {
            cmd->opcode = 10;
            cmd->value = angle;
            cmd++;
            cmd->opcode = 4;
            cmd->value = -angle;
        // matching: an unused label stops IDO from merging the two branches' increments
        lbl:
            cmd++;
        }
    }
    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    if ((movement_obj[dc_glbl_CharNum].mv != NULL) && (movement_obj[dc_glbl_CharNum].mv->pssd_waist_tilt != 0)) {
        cmd->opcode = 8;
        cmd++;
        cmd[-1].value = -movement_obj[dc_glbl_CharNum].mv->pssd_waist_tilt;
    }
    return cmd;
}

/* Appends an opcode-6 command scaled from unk1D1, if it is set. */
GameCommand *func_15034340(GameCommand *cmd, s32 index) {
    Actor *actor = &movement_obj[index];

    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    return cmd;
}

/* Identical to func_15034340. */
GameCommand *func_150343B0(GameCommand *cmd, s32 index) {
    Actor *actor = &movement_obj[index];

    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    return cmd;
}

/* Converts taskint[0] (tenths of a degree) to angle units and splits it across an opcode-0 and an opcode-0x18 command. */
GameCommand *func_15034420(GameCommand *cmd, s32 index) {
    s16 part;
    s16 value;

    value = movement_obj[index].taskint[0] * (65536.0f / 3600.0f);
    part = value >> 4;
    value -= part;
    cmd[0].value = part;
    cmd[1].value = value;
    cmd[0].opcode = 0;
    cmd[1].opcode = 0x18;
    return cmd + 2;
}

/*
 * Appends two commands built from the object's taskint[0] angle pair (two s16s):
 * opcode 0x12 with the second angle, opcode 0xE with the negated first one,
 * both scaled by 65536 / 18000.
 */
GameCommand *func_150344A0(GameCommand *cmd, s32 index) {
    s32 angle;

    cmd[0].opcode = 0x12;
    angle = ((s16 *)&movement_obj[index].taskint[0])[1];
    cmd[0].value = angle * 3.640889f;
    cmd[1].opcode = 0xE;
    angle = -((s16 *)&movement_obj[index].taskint[0])[0];
    cmd[1].value = angle * 3.640889f;
    return cmd + 2;
}

/* Calls bone_updateChain for objects with id 0-4, 0x80 or 0xB0. */
void bone_updateChainByType(s32 index) {
    switch (movement_obj[index].character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x80:
    case 0xB0:
        bone_updateChain(index);
        break;
    }
}

/* Per-object update hook for objects that have bone matrices. */
void bone_updateObject(s32 index) {
    if (movement_obj[index].ob_matrix_ptr != NULL) {
        if (movement_obj[index].type == 1 || movement_obj[index].drawtype == 7) {
            bone_updateChainByType(index);
        }

        if (movement_obj[index].type == 0x22) {
            bone_updateChainByType(index);
        }

        switch (movement_obj[index].character_type) {
        case 4:
            if (gIsMultiplayer == 0) {
                bone_lookAtCamera(0, index, 9, 0);
                bone_lookAtCamera(0, index, 4, 0);
            }
            break;
        case 0x70:
            bone_lowerBones(&movement_obj[index], 10, 11);
            break;
        case 0x7B:
            func_150F1A00(&movement_obj[index]);
            break;
        case 0x60:
            func_150DFDA4(&movement_obj[index]);
            break;
        }
    }
}

/* Zeroes the 3x3 rotation part of every bone matrix whose bit is set in unk9C (bones 0-31). */
void bone_collapseMaskedBones(Actor *obj) {
    s32 bone;
    s32 row;
    s32 col;
    s32 lastBone;
    MtxF *bones;

    if (obj->ob_matrix_ptr != NULL && obj->unk9C != 0) {
        lastBone = actor_getBoneMatrixCount(obj->character_type) - 1;
        bones = obj->ob_matrix_ptr;
        bone = 31;
        if (lastBone < 31) {
            bone = lastBone;
        }
        for (; bone >= 0; bone--) {
            if (obj->unk9C & (1 << bone)) {
                for (row = 0; row < 3; row++) {
                    for (col = 0; col < 3; col++) {
                        // matching: bones[bone][row][col] lets IDO strength-reduce the row loop; the
                        // explicit (row << 4) byte offset keeps the per-row address recompute
                        ((f32 *)((u8 *)(bone + bones) + (row << 4)))[col] = 0.0f;
                    }
                }
            }
        }
    }
}

/* Runs bone_collapseMaskedBones on every active object with a non-zero unk9C mask. */
void bone_collapseAllMaskedBones(void) {
    s32 i;

    if (gPaused != 0) {
        return;
    }

    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0 && movement_obj[i].unk9C != 0) {
            bone_collapseMaskedBones(&movement_obj[i]);
        }
    }
}
