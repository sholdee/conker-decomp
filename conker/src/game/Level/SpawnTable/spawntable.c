#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init/Sched/SchedInit/schedinit.c).
u8 gSpawnModelTypeCount;      // number of distinct spawn model types in gSpawnModelTypes
u16 gSpawnModelTypes[8];  // the distinct spawn model types (< 0xBB) of the level
s32 *gSpawnModelData[8]; // per spawn model type: data loaded by rom_loadPathTo

#include "functions.h"
#include "variables.h"

/* Collects the distinct spawn-table model types (< 0xBB) and the data rom_loadPathTo loads for each. */
void level_collectSpawnModelTypes(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 *data;
    s32 id;

    gSpawnModelTypeCount = 0;
    data = NULL;
    for (i = 0; i < gSpawnRecordCount; i++) {
        id = gSpawnRecords[i].type;
        found = 0;
        for (j = 0; j < gSpawnModelTypeCount; j++) {
            if (id == gSpawnModelTypes[j]) {
                found = 1;
                break;
            }
        }
        if (found) {
            continue;
        }
        if (id < 0xBB) {
            rom_loadPathTo(&data, 2, 0x12, id);
        }
        if (data != NULL) {
            gSpawnModelData[gSpawnModelTypeCount] = data;
            gSpawnModelTypes[gSpawnModelTypeCount] = id;
            gSpawnModelTypeCount++;
        }
    }
}

extern u8 D_1A33F0[];
extern u8 D_1A34E0[];
extern u8 compressed_ROM_START[];

/* Loads the spawn table for `levelId` (base entries from ROM plus the level's own), then fills in one
 * entry per player slot (the first D_8008FD8C) from the type player_getStat reports and the slot's object. */
void level_loadSpawnTable(s32 levelId) {
    s32 j;
    u8 *romEnd;
    u8 *romStart;
    u32 size;
    Actor *obj;
    s32 count;
    s32 i;

    actor_clearModelTables();
    switch (levelId) {
    case 26:
    case 36:
    case 43:
    case 45:
    case 48:
    case 51:
    case 52:
    case 63:
        romStart = D_1A34E0;
        romEnd = compressed_ROM_START;
        break;
    default:
        romStart = D_1A33F0;
        romEnd = D_1A34E0;
        break;
    }
    size = romEnd - romStart;
    gSpawnRecords = allocate_memory(rom_getPathSize(2, 0xE, levelId) + size, 1, 2, 0);
    rom_read((s32)romStart, gSpawnRecords, size, 1);
    gSpawnRecordCount = size / sizeof(SpawnRecord);
    gSpawnFirstCount = gSpawnRecordCount;
    gSpawnRecordCount += rom_loadPathInto(&gSpawnRecords[gSpawnRecordCount], 0, 2, 0xE, levelId) / sizeof(SpawnRecord);
    level_allocSpawnPaths(levelId);
    count = gSpawnRecordCount;
    gSpawnInUse = allocate_memory(count, 1, 0, 0);
    size = count * sizeof(s16);
    gSpawnDelays = allocate_memory(size, 1, 0, 0);
    bzero(gSpawnInUse, count);
    bzero(gSpawnDelays, size);
    D_800D1548 = 0;
players: // matching: unused label splits the basic block; IDO schedules the loop setup differently without it
    for (i = 0; i < D_8008FD8C; i++) {
        if ((1 << i) & gMatchState.playerMask) {
            s32 type = player_getStat(i, 1);

            gSpawnRecords[i].unk2 = 0;
            switch (type) {
            case 0:
                if (level == 0x36 || level == 0x3A || level == 0x40) {
                    gSpawnRecords[i].type = 0x96;
                } else {
                    gSpawnRecords[i].type = 0;
                }
                break;
            case 16:
                gSpawnRecords[i].type = 0x96;
                break;
            case 1:
                gSpawnRecords[i].type = 0x3B;
                break;
            case 2:
                gSpawnRecords[i].type = 0x98;
                break;
            case 3:
                gSpawnRecords[i].type = 0x75;
                break;
            case 4:
                gSpawnRecords[i].type = 0x80;
                break;
            case 37:
                gSpawnRecords[i].type = 0xB0;
                gSpawnRecords[i].xz_scale = gSpawnRecords[i].y_scale = 0.6000000238f;
                break;
            case 38:
                gSpawnRecords[i].type = 0xB1;
                gSpawnRecords[i].xz_scale = gSpawnRecords[i].y_scale = 0.6000000238f;
                break;
            case 39:
                gSpawnRecords[i].type = 0xB2;
                break;
            case 40:
                gSpawnRecords[i].type = 0xB4;
                break;
            case 33:
                gSpawnRecords[i].type = 0x9F;
                break;
            case 34:
                gSpawnRecords[i].type = 0xA0;
                break;
            case 11:
                gSpawnRecords[i].type = 0x88;
                break;
            case 35:
                gSpawnRecords[i].type = 0x9B;
                gSpawnRecords[i].interaction_state = 0x15;
                break;
            case 36:
                gSpawnRecords[i].type = 0x9A;
                gSpawnRecords[i].interaction_state = 0x15;
                break;
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
                gSpawnRecords[i].type = 0x9C;
                break;
            case 29:
            case 30:
            case 31:
            case 32:
                gSpawnRecords[i].type = 0x9D;
                break;
            case 12:
            case 13:
            case 14:
            case 15:
                gSpawnRecords[i].type = 0x90;
                if (i < D_8008FD90) {
                    gSpawnRecords[i].interaction_state = 1;
                } else {
                    gSpawnRecords[i].interaction_state = 0xC;
                }
                gSpawnRecords[i].objFlags |= 4;
                gSpawnRecords[i].unkD = 0xD;
                break;
            case 8:
                gSpawnRecords[i].type = 0x77;
                gSpawnRecords[i].interaction_state = 0x1E;
                gSpawnRecords[i].unk10 = 0xAA;
                if (i >= D_8008FD90) {
                    gSpawnRecords[i].unkD = 3;
                } else {
                    gSpawnRecords[i].unkD = 1;
                }
                gSpawnRecords[i].objFlags |= 0x180000;
                gSpawnRecords[i].unkE |= 0x80;
                break;
            case 10:
                gSpawnRecords[i].type = 0x77;
                gSpawnRecords[i].interaction_state = 0x25;
                gSpawnRecords[i].unk10 = 0xA8;
                gSpawnRecords[i].unkD = 3;
                gSpawnRecords[i].objFlags |= 0x100000;
                break;
            case 9:
                gSpawnRecords[i].type = 0x53;
                gSpawnRecords[i].interaction_state = 0x1F;
                gSpawnRecords[i].unk10 = 0xA8;
                gSpawnRecords[i].unkD = 2;
                gSpawnRecords[i].xz_scale = 0.6999999881f;
                gSpawnRecords[i].y_scale = 0.6999999881f;
                gSpawnRecords[i].objFlags |= 1;
                break;
            }
            if (level == 0x33 || level == 0x32) {
                gSpawnRecords[i].objFlags |= 0x4000000;
            }
            obj = movement_obj;
            obj += i | 0; // matching: `| 0` keeps IDO from strength-reducing &movement_obj[i] into a loop pointer
            gSpawnRecords[i].x = obj->xpos;
            gSpawnRecords[i].y = obj->ypos;
            gSpawnRecords[i].z = obj->zpos;
            gSpawnRecords[i].angle = obj->angle >> 8;
            continue;
        }
        gSpawnRecords[i].unk2 = 1;
    }
    level_collectSpawnModelTypes();
    for (j = 0; j < gSpawnRecordCount; j++) {
        if (gSpawnRecords[j].unique_id == 0) {
            gSpawnRecords[j].unique_id = collision_getFreeUniqueId();
        }
    }
    collision_trySpawnRecords(levelId, 1, 1);
    player_cancelWarp(0);
    collision_trySpawnRecords(levelId, 1, 0);
}

/* An entry of the level's path table: the path's byte offset from the table start and its point count. */
typedef struct {
    s32 offset;
    s32 count;
} PathTableEntry;

/* Allocates one path per spawn entry: an empty two-point path for each player slot, and the level's own
 * paths for the level-specific spawn entries. */
void level_allocSpawnPaths(s32 levelId) {
    s16 count;
    PathTableEntry *table;
    s32 i;
    u32 countsSize;

    count = gSpawnRecordCount;
    table = NULL;
    gSpawnPaths = allocate_memory(count * 4, 1, 0, 0); // matching: `* sizeof(PathPoint *)` makes the size unsigned and moves bytes
    countsSize = count;
    gSpawnPathCounts = allocate_memory(countsSize, 1, 0, 0);
    bzero(gSpawnPaths, count * 4);
    bzero(gSpawnPathCounts, countsSize);
    for (i = 0; i < D_8008FD8C; i++) {
        gSpawnPaths[i] = allocate_memory(2 * sizeof(PathPoint), 1, 2, 0); // centre + one waypoint
        bzero(gSpawnPaths[i], 2 * sizeof(PathPoint));
        gSpawnPathCounts[i] = 2;
    }
    if (rom_loadPathTo(&table, 2, 0x10, levelId) != 0) {
        for (i = gSpawnFirstCount; i < gSpawnRecordCount; i++) {
            if (table[i - gSpawnFirstCount].offset != 0) {
                gSpawnPathCounts[i] = table[i - gSpawnFirstCount].count;
                gSpawnPaths[i] = (PathPoint *)((u8 *)table + table[i - gSpawnFirstCount].offset);
            }
        }
    }
}
