#include <ultra64.h>

/* This file's .bss, before the headers: after them, the .bss order would change with the number of declarations in them. */
/* osGetCount() samples that command 12 takes (start and lap). */
static s32 gTaskTimerLap;
static s32 gTaskTimerStart;

#include "conker/actor.h"
#include "conker/boot.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/game.h"
#include "conker/player.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/text.h"
#include "conker/view.h"

// matching: offset + base operand order (indexing puts the base first and swaps registers)
#define ID_FLAG_BYTE(id) (*(((id) >> 3) + gTaskDoneBits))
#define ID_FLAG_BIT(id) (1 << ((id) & 7))

/*
 * Runs command `opcode` for task `id` with arguments `cmd`. Returns 1 for some record commands, else 0.
 * Note: the "clear" forms below keep only `bit & 7` of the byte, exactly as the game does.
 */
s32 script_taskCommand(s32 id, s32 opcode, ScriptCmd *cmd) {
    IdRecord *node2;
    IdRecord *node3;
    IdRecord *node5;
    IdRecord *node4;
    IdRecord *node11;
    s32 count;
    s32 i;

    switch (opcode) {
    case 2:
        node2 = script_findTask(id);
        ID_FLAG_BYTE(id) &= 7 & ID_FLAG_BIT(id);
        gLevelTasksDirty = 1;
        gAutoSavePending = 1;
        if (node2 == NULL) {
            return 0;
        }
        node2->TaskID |= TASK_FINISHED;
        return 1;
    case 1:
        if (D_80087410.unk1) {
            D_80087410.unk1 = 0;
            // matching: the block keeps IDO from hoisting 'move v0,zero' above the branch
            {
                s32 pad;
                return 0;
            }
        }
        ID_FLAG_BYTE(id) |= ID_FLAG_BIT(id);
        gLevelTasksDirty = 1;
        gAutoSavePending = 1;
        // fallthrough
    case 3:
        node3 = script_findTask(id);
        if (node3 == NULL) {
            return 0;
        }
        node3->TaskID |= TASK_FINISHED;
        if (id == gCurrentTaskId) {
            script_exitTask();
            return 0;
        }
        return 1;
    case 5:
        node5 = script_findTask(id);
        if (node5 == NULL) {
            return 0;
        }
        bzero((u8 *)node5 + node5->BssOffset, node5->BSSSize);
        return 0;
    case 4:
        node4 = script_findTask(id);
        if (node4 != NULL) {
            node4->TaskID |= TASK_INIT_PENDING;
            return 1;
        }
        break;
    case 6:
        script_getTask(id, 0);
        return 0;
    case 7:
        if (gScriptCmdWordCount == 4) {
            game_requestLevelChange(2, cmd->a[0].w, cmd->a[1].w, 0, 0);
            return 0;
        }
        break;
    case 8:
        if (gScriptCmdWordCount == 4) {
            player_startWarp(movement_obj, cmd->a[0].w & 0xFFF, cmd->a[1].w & 0xFFF);
            return 0;
        }
        break;
    case 9:
        gSeenTextChanged = 1;
        if (cmd->a[0].w) {
            gSeenTextBits[cmd->a[1].w >> 3] |= 1 << (cmd->a[1].w & 7);
        } else {
            gSeenTextBits[cmd->a[1].w >> 3] &= 7 & (1 << (cmd->a[1].w & 7));
        }
        gAutoSavePending = 1;
        break;
    case 10:
        save_endMatch(cmd->a[0].w);
        return 0;
    case 11:
        node11 = script_findTask(id);
        ID_FLAG_BYTE(id) |= ID_FLAG_BIT(id);
        gLevelTasksDirty = 1;
        gAutoSavePending = 1;
        if (node11 == NULL) {
            return 0;
        }
        node11->TaskID |= TASK_FINISHED;
        save_endMatch(0);
        script_exitTask();
        return 0;
    case 12:
        if (cmd->a[0].w < 0) {
            count = osGetCount();
            gTaskTimerLap = count;
            gTaskTimerStart = count;
            break;
        }
        osGetCount();
        gTaskTimerLap = osGetCount();
        return 0;
    case 13:
        D_800DDE38 = 1;
        break;
    case 14:
        D_800E9D00 |= cmd->a[0].w;
        break;
    case 15:
        if (cmd->a[1].w == 0) {
            gTeam0SpawnMask &= ~(1 << cmd->a[0].w);
        } else {
            gTeam1SpawnMask &= ~(1 << cmd->a[0].w);
        }
        break;
    case 16:
        if (cmd->a[1].w == 0) {
            gTeam0SpawnMask |= 1 << cmd->a[0].w;
        } else {
            gTeam1SpawnMask |= 1 << cmd->a[0].w;
        }
        break;
    case 17:
        screen_startViewFade(0, 0, 0, 0, 1, 0);
        return 0;
    case 18:
        for (i = 0; i <= gScreenLayout; i++) {
            screen_startViewFade(cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w, cmd->a[4].w, i);
        }
        break;
    case 19:
        gCheckpointLevel = cmd->a[0].w;
        // matching: the redundant '& 0xFF' uses up a temp register that the case 19/23 allocation needs
        gCheckpointSpawn = cmd->a[1].w & 0xFF;
        gNextLevel = gCheckpointLevel;
        break;
    case 20:
        save_startCredits();
        return 0;
    case 21:
        func_151E4DC4();
        return 0;
    case 22:
        text_print("%d", cmd->a[0].w);
        return 0;
    case 23:
        if (cmd->a[0].w) {
            gAutoSaveDisabled = 1;
        } else {
            gAutoSaveDisabled = 0;
        }
        break;
    }
    return 0;
}

/* Answers query `opcode` about task `id` with arguments `cmd`; unknown queries return 0. */
s32 script_taskQuery(s32 id, s32 opcode, ScriptCmd *cmd) {
    // matching: three unused locals give the 0x30 frame
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 i;
    s32 sum;
    s32 j;
    s32 threshold;
    s32 mask;
    IdRecord *node;

    switch (opcode) {
    case 26:
        return ID_FLAG_BYTE(id) & ID_FLAG_BIT(id);
    case 27:
        return script_findTask(id)->LoadSection;
    case 28:
        return level;
    case 29:
        mask = 0;
        for (j = 0; j < gPlayerCount; j++) {
            if (gDefaultCharacters[j] != 10) {
                mask |= 1 << j;
            }
        }
        mask &= ~gEliminatedPlayers;
        return mask;
    case 30:
        return save_getTeam(cmd->a[0].w);
    case 31:
        return gPrevLevel;
    case 32:
        return gCheckpointLevel;
    case 33:
        return gCheckpointSpawn;
    case 34:
        /* Weighted pick: index of the first nonzero weight whose running total reaches math_randomFloat() * total. */
        sum = 0;
        for (i = 0; i < cmd->a[0].w; i++) {
            sum += cmd->a[1 + i].w;
        }
        threshold = math_randomFloat() * sum;
        sum = 0;
        for (i = 0; i < cmd->a[0].w; i++) {
            sum += cmd->a[1 + i].w;
            if (sum >= threshold && cmd->a[1 + i].w != 0) {
                return i;
            }
        }
        return cmd->a[0].w - 1;
    case 35:
        return gHumanCount;
    case 36:
        return D_800E0BB1;
    case 37:
        return gMatchFlags & 0x10;
    case 38:
        return gPlayerCount;
    case 39:
        return gMenuButtonsPressed[cmd->a[0].w];
    case 40:
        return gMenuPads[cmd->a[0].w].button;
    case 41:
        return gFrontEndState == 2;
    case 42:
        return cmd->a[0].w & D_800E9D00;
    case 43:
        node = script_findTask(id);
        if (node != NULL) {
            return (node->TaskID & SUSPEND_BETWEEN_SECTIONS) == 0;
        }
        return 0;
    case 44:
        return screen_isViewFadedOut(gScriptCmdWordCount == 3 ? cmd->a[0].w : 0);
    case 45:
        return gStartedFromListSelection;
    }
    return 0;
}
