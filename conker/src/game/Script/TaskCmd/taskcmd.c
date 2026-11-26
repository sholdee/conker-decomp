#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Argument block of a command; the arguments start at 0x8. */
struct CommandArgs {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
};


/* osGetCount() samples that command 12 takes (start and lap). */
static s32 gTaskTimerLap;
static s32 gTaskTimerStart;

// matching: offset + base operand order (indexing puts the base first and swaps registers)
#define ID_FLAG_BYTE(id) (*(((id) >> 3) + gTaskDoneBits))
#define ID_FLAG_BIT(id) (1 << ((id) & 7))

/*
 * Runs command `cmd` for record `id` with arguments `args`. Returns 1 for some record commands, else 0.
 * Note: the "clear" forms below keep only `bit & 7` of the byte, exactly as the game does.
 */
s32 script_taskCommand(s32 id, s32 cmd, CommandArgs *args) {
    IdRecord *node2;
    IdRecord *node3;
    IdRecord *node5;
    IdRecord *node4;
    IdRecord *node11;
    s32 count;
    s32 i;

    switch (cmd) {
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
            game_requestLevelChange(2, args->unk8, args->unkC, 0, 0);
            return 0;
        }
        break;
    case 8:
        if (gScriptCmdWordCount == 4) {
            player_startWarp(movement_obj, args->unk8 & 0xFFF, args->unkC & 0xFFF);
            return 0;
        }
        break;
    case 9:
        gSeenTextChanged = 1;
        if (args->unk8) {
            gSeenTextBits[args->unkC >> 3] |= 1 << (args->unkC & 7);
        } else {
            gSeenTextBits[args->unkC >> 3] &= 7 & (1 << (args->unkC & 7));
        }
        gAutoSavePending = 1;
        break;
    case 10:
        save_endMatch(args->unk8);
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
        if (args->unk8 < 0) {
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
        D_800E9D00 |= args->unk8;
        break;
    case 15:
        if (args->unkC == 0) {
            gTeam0SpawnMask &= ~(1 << args->unk8);
        } else {
            gTeam1SpawnMask &= ~(1 << args->unk8);
        }
        break;
    case 16:
        if (args->unkC == 0) {
            gTeam0SpawnMask |= 1 << args->unk8;
        } else {
            gTeam1SpawnMask |= 1 << args->unk8;
        }
        break;
    case 17:
        screen_startViewFade(0, 0, 0, 0, 1, 0);
        return 0;
    case 18:
        for (i = 0; i <= gScreenLayout; i++) {
            screen_startViewFade(args->unk8, args->unkC, args->unk10, args->unk14, args->unk18, i);
        }
        break;
    case 19:
        D_800BE3DF = args->unk8;
        // matching: the redundant '& 0xFF' uses up a temp register that the case 19/23 allocation needs
        D_800BE3E0 = args->unkC & 0xFF;
        gNextLevel = D_800BE3DF;
        break;
    case 20:
        func_151E8214();
        return 0;
    case 21:
        func_151E4DC4();
        return 0;
    case 22:
        text_print("%d", args->unk8);
        return 0;
    case 23:
        if (args->unk8) {
            gAutoSaveDisabled = 1;
        } else {
            gAutoSaveDisabled = 0;
        }
        break;
    }
    return 0;
}

/* Argument block of a query; the arguments start at 0x8. */
struct QueryArgs {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;     // query 34: number of weights
    /* 0x0C */ s32 unkC[1];  // query 34: the weights
};

/* Answers query `query` about record `id` with arguments `args`; unknown queries return 0. */
s32 script_taskQuery(s32 id, s32 query, QueryArgs *args) {
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

    switch (query) {
    case 26:
        return ID_FLAG_BYTE(id) & ID_FLAG_BIT(id);
    case 27:
        return script_findTask(id)->LoadSection;
    case 28:
        return level;
    case 29:
        mask = 0;
        for (j = 0; j < D_8008FD8C; j++) {
            if (D_80087270[j] != 10) {
                mask |= 1 << j;
            }
        }
        mask &= ~gEliminatedPlayers;
        return mask;
    case 30:
        return save_getTeam(args->unk8);
    case 31:
        return gPrevLevel;
    case 32:
        return D_800BE3DF;
    case 33:
        return D_800BE3E0;
    case 34:
        /* Weighted pick: index of the first nonzero weight whose running total reaches math_randomFloat() * total. */
        sum = 0;
        for (i = 0; i < args->unk8; i++) {
            sum += args->unkC[i];
        }
        threshold = math_randomFloat() * sum;
        sum = 0;
        for (i = 0; i < args->unk8; i++) {
            sum += args->unkC[i];
            if (sum >= threshold && args->unkC[i] != 0) {
                return i;
            }
        }
        return args->unk8 - 1;
    case 35:
        return D_8008FD90;
    case 36:
        return D_800E0BB1;
    case 37:
        return D_8008FDBC & 0x10;
    case 38:
        return D_8008FD8C;
    case 39:
        return gMenuButtonsPressed[args->unk8];
    case 40:
        return gMenuPads[args->unk8].button;
    case 41:
        return gFrontEndState == 2;
    case 42:
        return args->unk8 & D_800E9D00;
    case 43:
        node = script_findTask(id);
        if (node != NULL) {
            return (node->TaskID & SUSPEND_BETWEEN_SECTIONS) == 0;
        }
        return 0;
    case 44:
        return screen_isViewFadedOut(gScriptCmdWordCount == 3 ? args->unk8 : 0);
    case 45:
        return D_800D2E40;
    }
    return 0;
}
