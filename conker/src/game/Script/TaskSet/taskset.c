#include <ultra64.h>
#include "libc/stdarg.h"
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 D_800D2E40;
u8 D_800D2E41;
u8 D_800D2E42;
u8 D_800D2E43;
u8 D_800D2E44;
u8 D_800D2E45;
s32 gCurrentTaskId;
u8 *gTaskDoneBits; // 0x1B-byte bit array of per-id flags (saved with the game)
s32 D_800D2E50[4];
u8 gSeenTextBits[9]; // bit array of seen text ids (saved with the game); [8] also holds flags
u8 D_800D2E69;
u8 gTaskStates[0xCC]; // one state byte per resource id
u8 gLevelTaskCount;
u16 *gLevelTaskIds;
IdRecordList gTaskWorkingSet;
s32 D_800D2F54; /* not referenced */
s32 D_800D2F58;
// matching: a definition (not extern) lets as1 share one `lui` for the [0]/[1] stores of the command
// dispatchers (script_query)
s32 gScriptCmdBuffer[16];

#include "functions.h"
#include "variables.h"

Struct_80087410 D_80087410 = { 0, 0 };
char D_80087414[] = "<NO TASK NAME SET!>"; /* not referenced */

/* Bit `id` of the 0x1B-byte bit array at gTaskDoneBits: the task is done. */
// matching: offset + base operand order (indexing puts the base first and swaps registers)
#define ID_FLAG(id) (*(((id) >> 3) + gTaskDoneBits) & (1 << ((id) & 7)))

/* Gets (and, if needed, creates) the task of every id listed in gLevelTaskIds that script_areTaskDepsDone accepts. */
void script_loadLevelTasks(s32 unused, s32 reload) {
    s32 i;

    D_8003C8E0 = 0x5000000;
    script_reallocTaskDoneBits();
    script_clearPoseCommands();

    for (i = 0; i < gLevelTaskCount; i++) {
        if (!script_areTaskDepsDone(gLevelTaskIds[i])) {
            continue;
        }
        script_getTask(gLevelTaskIds[i], reload);
    }

    script_autoSave();
    D_8003C8E0 = 0;
}

/* Returns the BSS of the task `id`, or NULL if it is not loaded. */
void *script_getTaskBss(s32 id) {
    IdRecord *node;

    node = script_findTask(id);
    if (node != NULL) {
        return (u8 *)node + node->BssOffset;
    }
    return NULL;
}

/* Returns the task `id`, creating it if needed; NULL if `reload` is 0 and the task is done. */
IdRecord *script_getTask(s32 id, s32 reload) {
    IdRecord *node;

    node = script_findTask(id);
    if (!reload && ID_FLAG(id)) {
        return NULL;
    }

    if (node == NULL) {
        node = script_loadTask(id);
        script_insertTask(node);
        if (!ID_FLAG(TASK_ID(node))) {
            D_8003C8E0 = 0x6000000;
            func_150ADAF0(node, TASK_ENTRY_START);
            D_8003C8E0 = 0;
        }
        if (!reload || (node->TaskID & RUN_INIT_ONCE_ONLY)) {
            node->TaskID |= TASK_INIT_PENDING;
        }
    } else if (reload) {
        if ((node->TaskID & PURGE_BETWEEN_SECTIONS) && !(node->TaskID & DO_NOT_CLEAR_BSS)) {
            bzero((u8 *)node + node->BssOffset, node->BSSSize);
        }
    }

    return node;
}

/* Returns the loaded task `id`, or NULL. */
IdRecord *script_findTask(s16 id) {
    IdRecord *node;
    s32 i;

    node = gTaskWorkingSet.head;
    for (i = 0; i < gTaskWorkingSet.count; i++) {
        if (id == TASK_ID(node)) {
            return node;
        }
        node = node->NextTaskPtr;
    }
    return NULL;
}


/* Unlinks `node` from the task working set and frees it. */
void script_freeTask(IdRecord *node) {
    if (gTaskWorkingSet.count == 1) {
        gTaskWorkingSet.head = NULL;
        gTaskWorkingSet.tail = NULL;
    } else {
        if (node == gTaskWorkingSet.head) {
            gTaskWorkingSet.head = node->NextTaskPtr;
            node->NextTaskPtr->PrevTaskPtr = NULL;
        } else {
            node->PrevTaskPtr->NextTaskPtr = node->NextTaskPtr;
        }
        if (node == gTaskWorkingSet.tail) {
            gTaskWorkingSet.tail = node->PrevTaskPtr;
            node->PrevTaskPtr->NextTaskPtr = NULL;
        } else {
            node->NextTaskPtr->PrevTaskPtr = node->PrevTaskPtr;
        }
    }
    heap_free(node);
    gTaskWorkingSet.count--;
}

/* Inserts `node` into the task working set, keeping it sorted by task id. */
void script_insertTask(IdRecord *node) {
    IdRecord *cur;
    IdRecord *head;
    s32 id;
    s32 i;

    cur = gTaskWorkingSet.tail;
    id = TASK_ID(node);
    if (gTaskWorkingSet.count == 0) {
        gTaskWorkingSet.head = node;
        gTaskWorkingSet.tail = node;
        node->NextTaskPtr = NULL;
        node->PrevTaskPtr = NULL;
        gTaskWorkingSet.count += 1;
        return;
    }

    for (i = 0; i < gTaskWorkingSet.count; i++) {
        if (TASK_ID(cur) < id) {
            if (cur == gTaskWorkingSet.tail) {
                node->PrevTaskPtr = cur;
                node->NextTaskPtr = NULL;
                cur->NextTaskPtr = node;
                gTaskWorkingSet.tail = node;
                gTaskWorkingSet.count += 1;
                return;
            }
            node->PrevTaskPtr = cur;
            node->NextTaskPtr = cur->NextTaskPtr;
            cur->NextTaskPtr->PrevTaskPtr = node;
            cur->NextTaskPtr = node;
            gTaskWorkingSet.count += 1;
            return;
        }
        cur = cur->PrevTaskPtr;
    }

    head = gTaskWorkingSet.head;
    gTaskWorkingSet.head = node;
    node->PrevTaskPtr = NULL;
    node->NextTaskPtr = head;
    head->PrevTaskPtr = node;
    gTaskWorkingSet.count += 1;
}


/* Loads task `id` (via rom_loadPath) and sets its LoadSection to the current level level. */
IdRecord *script_loadTask(s16 id) {
    IdRecord *node;
    u32 size;

    node = rom_loadPath(&size, 2, 20, id);
    node->TaskID |= id;
    node->LoadSection = level;
    return script_allocTaskBss(node);
}

/* Reallocates `node` with its BSS appended, 8-byte aligned and zeroed, and frees the original. */
IdRecord *script_allocTaskBss(IdRecord *node) {
    IdRecord *copy;
    s32 padding;

    padding = 8 - (((s32)node + node->TaskSize) & 7);
    node->BssOffset = node->TaskSize + padding;
    node->TaskSize = node->BssOffset + node->BSSSize;
    node->TaskSize = (node->TaskSize - (((s32)node + node->TaskSize) & 7)) + 8;

    copy = allocate_memory(node->TaskSize, 0xFF, 2, 0);
    if (copy == NULL) {
        while (1) {}
    }

    bcopy(node, copy, node->TaskSize);
    bzero((u8 *)copy + copy->BssOffset, copy->BSSSize);
    heap_free(node);
    return copy;
}


/*
 * Frees tasks. With `unloadAll` every task runs its purge entry and is freed. Otherwise done tasks are
 * freed silently (state 3), and TASK_FINISHED tasks, or PURGE_BETWEEN_SECTIONS tasks whose LoadSection
 * is not the current level, run the purge entry and are freed.
 */
void script_purgeTasks(s32 unloadAll) {
    IdRecord *node;
    IdRecord *next;
    s32 count;
    s32 i;

    count = gTaskWorkingSet.count;
    node = gTaskWorkingSet.head;
    if (D_800D2E69 != 0) {
        func_1509C228();
        D_800D2E69 = 0;
    }
    for (i = 0; i < count; i++, node = next) {
        s32 id;
        s32 key;

        key = node->TaskID;
        id = key & ~TASK_FLAGS;
        next = node->NextTaskPtr;
        if (ID_FLAG(id) && !unloadAll) {
            script_freeTask(node);
            gTaskStates[id] = 3;
        } else if (unloadAll || (key & TASK_FINISHED) || ((key & PURGE_BETWEEN_SECTIONS) && level != node->LoadSection)) {
            D_8003C8E0 = 0x6000001;
            func_150ADAF0(node, TASK_ENTRY_PURGE);
            D_8003C8E0 = 0;
            script_freeTask(node);
            if (!ID_FLAG(id)) {
                gTaskStates[id] = 0;
            } else {
                gTaskStates[id] = 3;
            }
        }
    }
}


/* Runs entry `mode` (TASK_ENTRY_INIT, _RUN or _DONE) of every task and updates the per-id state bytes. */
void script_runTasks(s32 mode) {
    IdRecord *node;
    s32 i;
    u16 id;
    s32 key;

    node = gTaskWorkingSet.head;
    script_updatePoseCommands();
    if (!gIsMultiplayer && level != 50 && !script_query(0, 0x2000, 0x93) && mode == TASK_ENTRY_RUN) {
        return;
    }
    func_1509C228();
    for (i = 0; i < gTaskWorkingSet.count; i++) {
        id = node->TaskID & TASK_ID_MASK;
        gCurrentTaskId = id;
        key = node->TaskID;
        if (key & TASK_FINISHED) {
            node = node->NextTaskPtr;
            continue;
        }
        if (mode == TASK_ENTRY_DONE) {
            if (ID_FLAG(id)) {
                D_8003C8E0 = 0x6000002;
                func_150ADAF0(node, TASK_ENTRY_DONE);
                D_8003C8E0 = 0;
                gTaskStates[id] = 3;
                node->TaskID |= TASK_FINISHED;
                node = node->NextTaskPtr;
                continue;
            }
        } else if (!ID_FLAG(id)) {
            if (key & TASK_INIT_PENDING) {
                D_8003C8E0 = 0x6000003;
                func_150ADAF0(node, TASK_ENTRY_INIT);
                D_8003C8E0 = 0;
                key = node->TaskID &= ~TASK_INIT_PENDING;
            }
            if (key & TASK_FINISHED) {
                node = node->NextTaskPtr;
                continue;
            }
            if (!(key & SUSPEND_BETWEEN_SECTIONS) || level == node->LoadSection) {
                if (mode == TASK_ENTRY_RUN || (mode == TASK_ENTRY_INIT && !(key & RUN_INIT_ONCE_ONLY))) {
                    D_8003C8E0 = 0x6000004;
                    func_150ADAF0(node, mode);
                    D_8003C8E0 = 0;
                }
                if (mode == TASK_ENTRY_RUN) {
                    gTaskStates[id] = 1;
                }
            } else {
                gTaskStates[id] = 2;
            }
        } else {
            gTaskStates[id] = 3;
        }
        node = node->NextTaskPtr;
    }
    if (D_800D2F58 != 0) {
        D_800D2F58 = 0;
        script_loadLevelTasks((s16)level, 0);
    }
    D_800D3858 = 0;
}

/*
 * Command dispatchers: bits 12-15 of `cmd` select a handler in the table, bits 0-11 are passed
 * to it with `arg` and `count` further variadic words (collected in gScriptCmdBuffer with cmd and arg).
 * matching: cmdWord and arg are read back from their home slots via va_arg (va_start on `count`)
 * into a separate local `cmd` and the reused `arg`.
 */
s32 script_query(s32 count, s32 cmdWord, s32 arg, ...) {
    va_list ap;
    s32 i;
    s32 cmd;
    s32 type;
    s32 saved;
    s32 result;

    va_start(ap, count);
    cmd = va_arg(ap, s32);
    saved = D_800D3840;
    type = ((cmd & 0xF000) >> 12) - 1;
    cmd &= 0xFFF;
    arg = va_arg(ap, s32);
    gScriptCmdBuffer[0] = cmd;
    gScriptCmdBuffer[1] = arg;
    for (i = 0; i < count; i++) {
        gScriptCmdBuffer[i + 2] = va_arg(ap, s32);
    }
    va_end(ap);
    D_800D3840 = count + 2;
    result = gScriptQueryHandlers[type](cmd, arg, gScriptCmdBuffer);
    D_800D3840 = saved;
    return result;
}

s32 script_command(s32 count, s32 cmdWord, s32 arg, ...) {
    va_list ap;
    s32 i;
    s32 cmd;
    s32 type;
    s32 saved;
    s32 result;

    va_start(ap, count);
    cmd = va_arg(ap, s32);
    saved = D_800D3840;
    type = ((cmd & 0xF000) >> 12) - 1;
    cmd &= 0xFFF;
    arg = va_arg(ap, s32);
    gScriptCmdBuffer[0] = cmd;
    gScriptCmdBuffer[1] = arg;
    for (i = 0; i < count; i++) {
        gScriptCmdBuffer[i + 2] = va_arg(ap, s32);
    }
    va_end(ap);
    D_800D3840 = count + 2;
    result = gScriptCommandHandlers[type](cmd, arg, gScriptCmdBuffer);
    D_800D3840 = saved;
    return result;
}

/*
 * (Re)allocates the 0x1B-byte id bit array: allocates 1-4 buffers (count from func_150ADA20),
 * keeps the last one and frees the others, then copies the old array over and frees it.
 */
void script_reallocTaskDoneBits(void) {
    s32 i;
    s32 last;
    u8 *bufs[4];
    u8 *old;

    last = func_150ADA20() & 3;
    old = gTaskDoneBits;
    if (old != NULL) {
        if (!(func_150ADA20() & 4)) {
            return;
        }
    }
    for (i = 0; i <= last; i++) {
        bufs[i] = allocate_memory(0x1B, 0xFF, 2, 0);
    }
    for (i = 0; i <= last; i++) {
        if (i != last) {
            heap_free(bufs[i]);
        }
    }
    gTaskDoneBits = bufs[last];
    if (old != NULL) {
        bcopy(old, gTaskDoneBits, 0x1B);
        heap_free(old);
    }
}

void func_1509C228(void) {
    if ((gSeenTextBits[8] & 8) || ID_FLAG(120)) {
        script_command(2, 0x2000, 0x36, 0, 1);
        return;
    }
    script_command(2, 0x2000, 0x36, 1, 1);
}

/* Returns 0 when level is one of the listed values or D_800D2E44 is set, else 1. */
s32 script_canAutoSave(void) {
    s32 value = level;
    if ((value == 3) ||
        (value == 5) ||
        (value == 9) ||
        (value == 13) ||
        (value == 15) ||
        (value == 17) ||
        (value == 21) ||
        (value == 22) ||
        (value == 24) ||
        (value == 26) ||
        (value == 29) ||
        (value == 31) ||
        (value == 32) ||
        (value == 33) ||
        (value == 34) ||
        (value == 36) ||
        (value == 37) ||
        (value == 42) ||
        (value == 43) ||
        (value == 45) ||
        (value == 48) ||
        (value == 51) ||
        (value == 52) ||
        (value == 56) ||
        (value == 62) ||
        (value == 63) ||
        (D_800D2E44 != 0)
      ) {
        return 0;
    }
    return 1;
}

void script_autoSave(void) {
    if (D_800D2E43 && !D_800D2E44 && script_canAutoSave()) {
        D_800BE3DF = gNextLevel;
        if (D_800C35C4 == 0) {
            save_requestWriteSlot(gCurrentSaveSlot);
        }
        D_800D2E43 = 0;
    }
}

/* Returns index + 0x1400, plus 0x400 when id flag 24 is set. */
s32 func_1509C414(s32 index) {
    return (ID_FLAG(24) << 10) + index + 0x1400;
}
