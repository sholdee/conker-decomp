#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0_43D00
#include "functions.h"
#undef func_1502B7F0
#define D_800D20FC D_800D20FC_hdr
#define D_800D2104 D_800D2104_hdr
#define D_800D2110 D_800D2110_hdr
#include "variables.h"
#undef D_800D20FC
#undef D_800D2104
#undef D_800D2110

// One 0x30-byte entry of the level's spawn table D_800D20FC (same layout as game_AEB40.c's SpawnRecord).
typedef struct SpawnRecord {
    /* 0x00 */ u16 flags;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 interaction_state;
    /* 0x04 */ u8 type;         // model id
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8 angle;        // high byte of the facing angle
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x2];
    /* 0x18 */ s32 objFlags;    // copied to the object's unkF8
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[0x2];
    /* 0x20 */ f32 xz_scale;
    /* 0x24 */ f32 y_scale;
    /* 0x28 */ u8 unique_id;
    /* 0x29 */ u8 partSet;
    /* 0x2A */ u8 pad2A[0x2];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
} SpawnRecord; // size 0x30

extern s32 func_1502B7F0(s32 *, s32, s32, s32);
extern u8 D_800BE590;
extern u16 D_800BE598[];
extern s32 D_800BE5A8[];
extern SpawnRecord *D_800D20FC;
extern u8 D_800D2100;

/* Collects the distinct spawn-table model types (< 0xBB) and the asset size func_1502B7F0 reports for each. */
void func_15016850(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 size;
    s32 id;

    D_800BE590 = 0;
    size = 0;
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
            func_1502B7F0(&size, 2, 0x12, id);
        }
        if (size != 0) {
            D_800BE5A8[D_800BE590] = size;
            D_800BE598[D_800BE590] = id;
            D_800BE590++;
        }
    }
}

extern u8 D_1A33F0[];
extern u8 D_1A34E0[];
extern u8 D_1A37E0[];
extern u8 D_800D2101;
extern u8 *D_800D210C;
extern s16 *D_800D2110; // header declares it as s16[]
extern s32 func_1502B9B4(s32, s32, ...);
extern u32 func_1502B8E0(void *, s32, s32, s32, ...);
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15017114(s32 level);
extern u8 func_15083DA0(void);
extern void func_1508295C(s32 arg0, s32 arg1, s32 arg2);
extern void func_15172CA8(s32 arg0);

/* Loads the spawn table for `level` (base entries from ROM plus the level's own), then fills in one
 * entry per player slot (the first D_8008FD8C) from the type func_150859AC reports and the slot's object. */
void func_150169A0(s32 level) {
    s32 j;
    u8 *romEnd;
    u8 *romStart;
    u32 size;
    struct127 *obj;
    s32 count;
    s32 i;

    func_15004F30();
    switch (level) {
    case 26:
    case 36:
    case 43:
    case 45:
    case 48:
    case 51:
    case 52:
    case 63:
        romStart = D_1A34E0;
        romEnd = D_1A37E0;
        break;
    default:
        romStart = D_1A33F0;
        romEnd = D_1A34E0;
        break;
    }
    size = romEnd - romStart;
    D_800D20FC = allocate_memory(func_1502B9B4(2, 0xE, level) + size, 1, 2, 0);
    func_10004514((s32)romStart, D_800D20FC, size, 1);
    D_800D2100 = size / sizeof(SpawnRecord);
    D_800D2101 = D_800D2100;
    D_800D2100 += func_1502B8E0(&D_800D20FC[D_800D2100], 0, 2, 0xE, level) / sizeof(SpawnRecord);
    func_15017114(level);
    count = D_800D2100;
    D_800D210C = allocate_memory(count, 1, 0, 0);
    size = count * sizeof(s16);
    D_800D2110 = allocate_memory(size, 1, 0, 0);
    bzero(D_800D210C, count);
    bzero(D_800D2110, size);
    D_800D1548 = 0;
players: // matching: unused label splits the basic block; IDO schedules the loop setup differently without it
    for (i = 0; i < D_8008FD8C; i++) {
        if ((1 << i) & (u16)D_800D2340) {
            s32 type = func_150859AC(i, 1);

            D_800D20FC[i].unk2 = 0;
            switch (type) {
            case 0:
                if (D_800BE9F0 == 0x36 || D_800BE9F0 == 0x3A || D_800BE9F0 == 0x40) {
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
            if (D_800BE9F0 == 0x33 || D_800BE9F0 == 0x32) {
                D_800D20FC[i].objFlags |= 0x4000000;
            }
            obj = D_800CC2D0;
            obj += i | 0; // matching: `| 0` keeps IDO from strength-reducing &D_800CC2D0[i] into a loop pointer
            D_800D20FC[i].x = obj->x_position;
            D_800D20FC[i].y = obj->y_position;
            D_800D20FC[i].z = obj->z_position;
            D_800D20FC[i].angle = obj->unk76 >> 8;
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
    func_1508295C(level, 1, 1);
    func_15172CA8(0);
    func_1508295C(level, 1, 0);
}

/* One point of a path: s16 world coordinates (as in game_A28B0.c). */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 unk6;
} PathPoint;

/* A path, D_800D2104[n]: a centre point followed by D_800D2108[n] - 1 waypoints. */
typedef struct {
    PathPoint center;
    PathPoint points[1];
} Path;

/* An entry of the level's path table: the path's byte offset from the table start and its point count. */
typedef struct {
    s32 offset;
    s32 count;
} PathTableEntry;

extern Path **D_800D2104; // header declares it as s32 *

/* Allocates one path per spawn entry: an empty two-point path for each player slot, and the level's own
 * paths for the level-specific spawn entries. */
void func_15017114(s32 level) {
    s16 count;
    PathTableEntry *table;
    s32 i;
    u32 countsSize;

    count = D_800D2100;
    table = NULL;
    D_800D2104 = allocate_memory(count * 4, 1, 0, 0); // matching: `* sizeof(Path *)` makes the size unsigned and moves bytes
    countsSize = count;
    D_800D2108 = allocate_memory(countsSize, 1, 0, 0);
    bzero(D_800D2104, count * 4);
    bzero(D_800D2108, countsSize);
    for (i = 0; i < D_8008FD8C; i++) {
        D_800D2104[i] = allocate_memory(sizeof(Path), 1, 2, 0);
        bzero(D_800D2104[i], sizeof(Path));
        D_800D2108[i] = 2;
    }
    if (func_1502B7F0((s32 *)&table, 2, 0x10, level) != 0) {
        for (i = D_800D2101; i < D_800D2100; i++) {
            if (table[i - D_800D2101].offset != 0) {
                D_800D2108[i] = table[i - D_800D2101].count;
                D_800D2104[i] = (Path *)((u8 *)table + table[i - D_800D2101].offset);
            }
        }
    }
}
