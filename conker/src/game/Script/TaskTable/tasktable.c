#include <ultra64.h>
#include "structs.h"

/* A pointer fixup: `offset` is relative to its block until relocated; bit 31 of `flags` ends a list. */
typedef struct RelocPair {
    /* 0x0 */ s32 offset;
    /* 0x4 */ u32 flags;
} RelocPair;

#define RELOC_LAST 0x80000000

/* Header of a loaded resource block; it starts with its own fixup list (0x0/0x4, 0x8/0xC). */
typedef struct LoadedResource {
    /* 0x0 */ s16 *unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ s16 *dependencies;
    /* 0xC */ u32 dependencyCount;   // stored doubled in the file
} LoadedResource;

/* The loaded resource table: a relocated RomTableEntry table (rom_loadPathTable) whose offsets are now pointers;
 * slot 0's info holds the slot count, slots 1.. point at resource blocks. */
typedef struct ResourceSlot {
    /* 0x0 */ LoadedResource *data;
    /* 0x4 */ s32 info;
} ResourceSlot;

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
u8 D_800D2FA0[0x10]; /* not referenced */
static ResourceSlot *gResourceTable;

#include "conker/heap.h"
#include "conker/rom.h"
#include "conker/script.h"


#define RESOURCE_STATE_LOADED 3

/* Per-resource "loaded" bitset and per-resource state bytes. */
#define gResourceLoadedBits gTaskDoneBits
#define gResourceStates gTaskStates
#define RESOURCE_IS_LOADED(idx) (gResourceLoadedBits[(idx) >> 3] & (1 << ((idx) & 7)))

/* Returns the name of resource `index`, or NULL for an empty slot. */
const char *script_getTaskName(s32 index) {
    return gTaskManifest[index].name;
}

u16 script_getTaskLevel(s32 index) {
    return gTaskManifest[index].levelId;
}

s32 script_getTaskSpawnPoint(s32 index) {
    return gTaskManifest[index].packedSpawnPoint & 0xFFF;
}

s32 script_getTaskPackedId(s32 index) {
    return gTaskManifest[index].packedId;
}

/* Finds the resource whose id is `id`; returns RESOURCE_COUNT if none. */
s32 script_findTaskIndex(s32 id) {
    s32 i = 0; // matching: initialising here schedules the i = 0 move before the loop setup

    for (; i < RESOURCE_COUNT; i++) {
        if (gTaskManifest[i].name != NULL && id == (gTaskManifest[i].packedId & 0x1FFFFFFF) + 1) {
            return i;
        }
    }
    return RESOURCE_COUNT;
}

/* Counts the descriptor slots that are in use. */
s32 script_countTasks(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < RESOURCE_COUNT; i++) {
        if (gTaskManifest[i].name != NULL) {
            count++;
        }
    }
    return count;
}

/* Returns 1 if every dependency of `index` is loaded, recursively. */
s32 script_areTaskDepsDone(s32 index) {
    const u16 *deps;
    s32 depsLoaded;
    s32 i;

    deps = gTaskManifest[index].dependencies;
    for (i = 0; deps[i] != RESOURCE_LIST_END; i++) {
        depsLoaded = script_areTaskDepsDone(deps[i]);
        if (!RESOURCE_IS_LOADED(deps[i]) || depsLoaded == 0) {
            return 0;
        }
    }
    return 1;
}


void func_1509CC94(s32 index) {
    script_markTaskDepsDone(index);
}

/* Marks every dependency of `index` loaded, depth first. */
void script_markTaskDepsDone(s32 index) {
    script_markTasksDone(gTaskManifest[index].dependencies, script_markTaskDepsDone);
}

/*
 * Marks each related dependency of `index` loaded; the ones not yet in the loaded state
 * also get their own dependencies marked. Returns how many changed state.
 */
s32 script_markRelatedTasksDone(s32 index) {
    const u16 *p;
    s32 count;

    count = 0;
    p = gTaskManifest[index].relatedDependencies;
    while (*p != RESOURCE_LIST_END) {
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        if (gResourceStates[*p] != RESOURCE_STATE_LOADED) {
            gResourceStates[*p] = RESOURCE_STATE_LOADED;
            script_markTaskDepsDone(*p);
            count++;
        }
        p++;
    }
    return count;
}

/* Marks `index` and, until nothing changes, everything reachable from loaded resources. */
void script_propagateTasksDone(s32 index) {
    s32 i;
    s32 changed;

    script_markRelatedTasksDone(index);
    do {
        changed = 0;
        for (i = 0; i < RESOURCE_COUNT; i++) {
            if (gResourceStates[i] == RESOURCE_STATE_LOADED) {
                changed += script_markRelatedTasksDone(i);
            }
        }
    } while (changed != 0);
}

/* Calls `callback` on each index in `list`, then marks that index loaded. */
void script_markTasksDone(const u16 *list, void (*callback)(s32)) {
    const u16 *p;

    p = list;
    while (*p != RESOURCE_LIST_END) {
        callback(*p);
        gResourceStates[*p] = RESOURCE_STATE_LOADED;
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        p++;
    }
}


/* Loads the resource table, relocates every block in it and returns its resource count in *countOut. */
s32 script_loadResourceTable(s32 category, s32 *countOut) {
    ResourceSlot *table;
    u32 i;
    s32 done;
    s32 count;
    RelocPair *reloc;
    u32 flags;
    s32 pad[2]; // matching: frame size

    count = 0;
    gResourceTable = rom_loadPathTable(0, 0, &count, 3, 8, 0, category);
    if (gResourceTable == NULL) {
        *countOut = 0;
        return 0;
    }
    table = gResourceTable;
    table[0].info = count - 1;
    *countOut = count - 1;
    for (i = 1; i < count; i++) {
        done = 0;
        reloc = (RelocPair *)gResourceTable[i].data;
        do {
            reloc->offset += (s32)gResourceTable[i].data;
            flags = reloc->flags;
            if (flags & RELOC_LAST) {
                if (1) { // matching: -g3 block keeps IDO's register order
                    done = 1;
                }
            }
            reloc->flags = flags & 0x0FFFFFFF;
            reloc++;
        } while (!done);
        // matching: indexing from &table[1] keeps IDO's stepped table pointer
        (&table[1])[i - 1].data->dependencyCount = ((&table[1])[i - 1].data->dependencyCount & 0x0FFFFFFF) >> 1;
    }
    return 1;
}

/* Frees the loaded resource table. */
void script_freeResourceTable(void) {
    ResourceSlot *table;

    if (gResourceTable != NULL) {
        table = gResourceTable;
        heap_free(table);
        gResourceTable = NULL;
    }
}

/*
 * Returns 1 if every dependency of loaded resource `index` is loaded. Also reports its first
 * dependency (0xBF if missing) and the first halfword of its unk0 list (0 if missing).
 */
s32 script_areResourceDepsLoaded(s32 category, s32 index, s32 *firstDepOut, s32 *unk0Out) {
    ResourceSlot *table;
    ResourceSlot *slot;
    LoadedResource *resource;
    s16 *deps;
    u32 i;
    u32 count;

    table = gResourceTable;
    slot = ((ResourceSlot (*)[1])table)[index]; // matching: row view keeps IDO's base+offset operand order
    if (table == NULL) {
        if (firstDepOut != NULL) {
            *firstDepOut = 0xBF;
        }
        if (unk0Out != NULL) {
            *unk0Out = 0;
        }
        return 0;
    }
    resource = slot[1].data;
    if (resource == NULL) {
        if (firstDepOut != NULL) {
            *firstDepOut = 0xBF;
        }
        if (unk0Out != NULL) {
            *unk0Out = 0;
        }
        return 0;
    }
    if (firstDepOut != NULL) {
        *firstDepOut = resource->dependencies[0];
        resource = slot[1].data;
    }
    if (unk0Out != NULL) {
        *unk0Out = resource->unk0[0];
        resource = slot[1].data;
    }
    i = 0;
    count = resource->dependencyCount;
    if (count != 0) {
        deps = resource->dependencies;
        do {
            i++;
            if (!RESOURCE_IS_LOADED(*deps)) {
                return 0;
            }
            deps++;
        } while (i < count);
    }
    return 1;
}
