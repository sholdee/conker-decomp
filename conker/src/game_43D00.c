#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init_50A0.c).
u8 D_800BE590;      // number of distinct spawn model types in D_800BE598
u16 D_800BE598[8];  // the distinct spawn model types (< 0xBB) of the level
s32 *D_800BE5A8[8]; // per spawn model type: data loaded by func_1502B7F0

#include "functions.h"
#include "variables.h"

/* Collects the distinct spawn-table model types (< 0xBB) and the data func_1502B7F0 loads for each. */
void func_15016850(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 *data;
    s32 id;

    D_800BE590 = 0;
    data = NULL;
    for (i = 0; i < D_800D2100; i++) {
        id = D_800D20FC[i].type;
        found = 0;
        for (j = 0; j < D_800BE590; j++) {
            if (id == D_800BE598[j]) {
                found = 1;
                break;
            }
        }
        if (found) {
            continue;
        }
        if (id < 0xBB) {
            func_1502B7F0(&data, 2, 0x12, id);
        }
        if (data != NULL) {
            D_800BE5A8[D_800BE590] = data;
            D_800BE598[D_800BE590] = id;
            D_800BE590++;
        }
    }
}

extern u8 D_1A33F0[];
extern u8 D_1A34E0[];
extern u8 compressed_ROM_START[];

/* Loads the spawn table for `levelId` (base entries from ROM plus the level's own), then fills in one
 * entry per player slot (the first D_8008FD8C) from the type player_getStat reports and the slot's object. */
void func_150169A0(s32 levelId) {
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
    D_800D20FC = allocate_memory(func_1502B9B4(2, 0xE, levelId) + size, 1, 2, 0);
    func_10004514((s32)romStart, D_800D20FC, size, 1);
    D_800D2100 = size / sizeof(SpawnRecord);
    D_800D2101 = D_800D2100;
    D_800D2100 += func_1502B8E0(&D_800D20FC[D_800D2100], 0, 2, 0xE, levelId) / sizeof(SpawnRecord);
    func_15017114(levelId);
    count = D_800D2100;
    D_800D210C = allocate_memory(count, 1, 0, 0);
    size = count * sizeof(s16);
    D_800D2110 = allocate_memory(size, 1, 0, 0);
    bzero(D_800D210C, count);
    bzero(D_800D2110, size);
    D_800D1548 = 0;
players: // matching: unused label splits the basic block; IDO schedules the loop setup differently without it
    for (i = 0; i < D_8008FD8C; i++) {
        if ((1 << i) & gMatchState.playerMask) {
            s32 type = player_getStat(i, 1);

            D_800D20FC[i].unk2 = 0;
            switch (type) {
            case 0:
                if (level == 0x36 || level == 0x3A || level == 0x40) {
                    D_800D20FC[i].type = 0x96;
                } else {
                    D_800D20FC[i].type = 0;
                }
                break;
            case 16:
                D_800D20FC[i].type = 0x96;
                break;
            case 1:
                D_800D20FC[i].type = 0x3B;
                break;
            case 2:
                D_800D20FC[i].type = 0x98;
                break;
            case 3:
                D_800D20FC[i].type = 0x75;
                break;
            case 4:
                D_800D20FC[i].type = 0x80;
                break;
            case 37:
                D_800D20FC[i].type = 0xB0;
                D_800D20FC[i].xz_scale = D_800D20FC[i].y_scale = 0.6000000238f;
                break;
            case 38:
                D_800D20FC[i].type = 0xB1;
                D_800D20FC[i].xz_scale = D_800D20FC[i].y_scale = 0.6000000238f;
                break;
            case 39:
                D_800D20FC[i].type = 0xB2;
                break;
            case 40:
                D_800D20FC[i].type = 0xB4;
                break;
            case 33:
                D_800D20FC[i].type = 0x9F;
                break;
            case 34:
                D_800D20FC[i].type = 0xA0;
                break;
            case 11:
                D_800D20FC[i].type = 0x88;
                break;
            case 35:
                D_800D20FC[i].type = 0x9B;
                D_800D20FC[i].interaction_state = 0x15;
                break;
            case 36:
                D_800D20FC[i].type = 0x9A;
                D_800D20FC[i].interaction_state = 0x15;
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
                D_800D20FC[i].type = 0x9C;
                break;
            case 29:
            case 30:
            case 31:
            case 32:
                D_800D20FC[i].type = 0x9D;
                break;
            case 12:
            case 13:
            case 14:
            case 15:
                D_800D20FC[i].type = 0x90;
                if (i < D_8008FD90) {
                    D_800D20FC[i].interaction_state = 1;
                } else {
                    D_800D20FC[i].interaction_state = 0xC;
                }
                D_800D20FC[i].objFlags |= 4;
                D_800D20FC[i].unkD = 0xD;
                break;
            case 8:
                D_800D20FC[i].type = 0x77;
                D_800D20FC[i].interaction_state = 0x1E;
                D_800D20FC[i].unk10 = 0xAA;
                if (i >= D_8008FD90) {
                    D_800D20FC[i].unkD = 3;
                } else {
                    D_800D20FC[i].unkD = 1;
                }
                D_800D20FC[i].objFlags |= 0x180000;
                D_800D20FC[i].unkE |= 0x80;
                break;
            case 10:
                D_800D20FC[i].type = 0x77;
                D_800D20FC[i].interaction_state = 0x25;
                D_800D20FC[i].unk10 = 0xA8;
                D_800D20FC[i].unkD = 3;
                D_800D20FC[i].objFlags |= 0x100000;
                break;
            case 9:
                D_800D20FC[i].type = 0x53;
                D_800D20FC[i].interaction_state = 0x1F;
                D_800D20FC[i].unk10 = 0xA8;
                D_800D20FC[i].unkD = 2;
                D_800D20FC[i].xz_scale = 0.6999999881f;
                D_800D20FC[i].y_scale = 0.6999999881f;
                D_800D20FC[i].objFlags |= 1;
                break;
            }
            if (level == 0x33 || level == 0x32) {
                D_800D20FC[i].objFlags |= 0x4000000;
            }
            obj = movement_obj;
            obj += i | 0; // matching: `| 0` keeps IDO from strength-reducing &movement_obj[i] into a loop pointer
            D_800D20FC[i].x = obj->xpos;
            D_800D20FC[i].y = obj->ypos;
            D_800D20FC[i].z = obj->zpos;
            D_800D20FC[i].angle = obj->angle >> 8;
            continue;
        }
        D_800D20FC[i].unk2 = 1;
    }
    func_15016850();
    for (j = 0; j < D_800D2100; j++) {
        if (D_800D20FC[j].unique_id == 0) {
            D_800D20FC[j].unique_id = func_15083DA0();
        }
    }
    func_1508295C(levelId, 1, 1);
    player_cancelWarp(0);
    func_1508295C(levelId, 1, 0);
}

/* An entry of the level's path table: the path's byte offset from the table start and its point count. */
typedef struct {
    s32 offset;
    s32 count;
} PathTableEntry;

/* Allocates one path per spawn entry: an empty two-point path for each player slot, and the level's own
 * paths for the level-specific spawn entries. */
void func_15017114(s32 levelId) {
    s16 count;
    PathTableEntry *table;
    s32 i;
    u32 countsSize;

    count = D_800D2100;
    table = NULL;
    D_800D2104 = allocate_memory(count * 4, 1, 0, 0); // matching: `* sizeof(PathPoint *)` makes the size unsigned and moves bytes
    countsSize = count;
    D_800D2108 = allocate_memory(countsSize, 1, 0, 0);
    bzero(D_800D2104, count * 4);
    bzero(D_800D2108, countsSize);
    for (i = 0; i < D_8008FD8C; i++) {
        D_800D2104[i] = allocate_memory(2 * sizeof(PathPoint), 1, 2, 0); // centre + one waypoint
        bzero(D_800D2104[i], 2 * sizeof(PathPoint));
        D_800D2108[i] = 2;
    }
    if (func_1502B7F0(&table, 2, 0x10, levelId) != 0) {
        for (i = D_800D2101; i < D_800D2100; i++) {
            if (table[i - D_800D2101].offset != 0) {
                D_800D2108[i] = table[i - D_800D2101].count;
                D_800D2104[i] = (PathPoint *)((u8 *)table + table[i - D_800D2101].offset);
            }
        }
    }
}
