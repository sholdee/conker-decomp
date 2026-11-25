#include <ultra64.h>

#include "functions.h"
#include "variables.h"


typedef struct {
    s32 start;
    s32 end;
    s32 unk8;
} RelocRange;

/* variables.h declares these pointer variables as arrays; view them as what they hold. */
#define sRangeLists (*(RelocRange ***)D_800B0E30)
#define sRangeCounts (*(u8 **)D_800B0E34)

/* Rebases entry `index`'s zero-terminated range list (at offset `list` in `base`) and stores its length. */
void func_15003120(s32 index, s32 base, s32 list) {
    s32 i;

    sRangeLists[index] = (RelocRange *)list;
    sRangeCounts[index] = 0;
    if (list == 0) {
        return;
    }
    sRangeLists[index] = (RelocRange *)((u8 *)sRangeLists[index] + base);
    for (i = 0; sRangeLists[index][i].start != 0; i++) {
        sRangeLists[index][i].start += base;
        sRangeLists[index][i].end += base;
    }
    sRangeCounts[index] = i;
}


/* A level model asset. Its pointer fields hold offsets from the asset's start until rebased. */
typedef struct {
    /* 0x00 */ s32 displayList; // also marks the end of the vertices
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 rangeList;   // offset of a RelocRange list, rebased by func_15003120
    /* 0x24 */ s32 unk24;
    /* 0x28 */ Vtx vertices[1];
} LevelModel;

typedef struct {
    LevelModel *model;
    s32 loaded;
} LevelModelEntry;

/* The byte past the end of struct104 (variables.h's type for D_800B0DF0). */
typedef struct {
    u8 pad0[0x4B];
    u8 assetList; // index into D_80082B20, 0 = none
} LevelInfo;

/* Zero-terminated lists of asset ids loaded with each level (variables.h declares D_80082B20 as s32). */
#define sLevelAssetLists ((s32 **)&D_80082B20)

extern s32 D_800B0E10[];
extern s32 D_800B0E20[];
extern u8  D_800B0E38;
extern s32 D_800B0E40[];
extern LevelModelEntry *D_800B0E50;

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern LevelModelEntry *func_1502B6BC(s32 *loaded, s32 arg1, s32 *count, s32 arg3, s32 arg4, s32 arg5);
extern s32 func_150049A4(s32 dl, s32 arg1, s32 arg2);
extern s32 func_1510CE60(s32 dl, s32 strict, s32 countUse, s32 keepLevel, s32 *usedOut);
extern s32 func_1510D0EC(s32 id, s32 *size, s32 arg2, s32 arg3);

/* Loads level `level`'s models, rebases them and sets up the first four as the level's display lists. */
void func_150031EC(s32 level) {
    LevelModelEntry *models;
    LevelModel *model;
    s32 count;
    s32 loaded;
    u32 i;
    s32 j;
    s32 k;
    s32 size;
    s32 id;

    if (level >= 69) {
        level = 0;
    }
    models = func_1502B6BC(&loaded, 0, &count, 2, 4, level);
    D_800B0E50 = models;
    for (k = 1; k < 4; k++) {
        D_800B0E00[k] = 0;
        D_800B0E10[k] = 0;
        D_800B0E40[k] = 0;
    }
    func_150034B4();
    if (loaded) {
        sRangeLists = allocate_memory(count * 4, 1, 0, 0);
        sRangeCounts = allocate_memory(count, 1, 0, 0);
        bzero(sRangeCounts, count);
        bzero(sRangeLists, count);
        D_800B0E38 = count;
        for (i = 0; i < count; i++) {
            model = models[i].model;
            if (model != NULL && models[i].loaded) {
                if (model->displayList != 0) {
                    model->displayList += (s32)model;
                }
                if (model->unk8 != 0) {
                    model->unk8 += (s32)model;
                }
                if (model->unk10 != 0) {
                    model->unk10 += (s32)model;
                }
                func_15003120(i, (s32)model, model->rangeList);
                if (i < 4) {
                    D_800B0E00[i] = model->displayList;
                    D_800B0E10[i] = (s32)model->vertices;
                    D_800B0E40[i] = (u32)(D_800B0E00[i] - D_800B0E10[i]) / sizeof(Vtx);
                    D_800B0E20[i] = model->unk8;
                    if (i == 0) {
                        func_150039BC(model->unk10);
                    }
                    func_150049A4(D_800B0E00[i], D_800B0E10[i] + 0xFF000000, (s32)model);
                    func_1510CE60(D_800B0E00[i], 0, 1, 63, NULL);
                } else {
                    func_150049A4(model->displayList, 0, (s32)model);
                }
            }
        }
    }
    if (((LevelInfo *)D_800B0DF0)->assetList != 0) {
        j = 0;
        while ((id = sLevelAssetLists[((LevelInfo *)D_800B0DF0)->assetList][j++]) != 0) {
            func_1510D0EC(id, &size, 62, 0);
        }
    }
}

#define NUM_TEXTURES 7762

/* Per-texture u16 tables (variables.h declares them as s32 arrays). */
#define sTexCompressedSizes ((u16 *)D_80091D20)
#define sTexSizes ((u16 *)D_800B87A0)

/* Marks every texture unloaded and resets the requested texture id range. */
s32 func_150034B4(void) {
    s32 i;

    for (i = 0; i < NUM_TEXTURES; i++) {
        D_800B0E58[i] = -1;
        D_800BC448[i] = 0;
    }

    D_800D9F58 = 0xFFFF;
    D_800D9F5C = -1;

    if (D_800BE9F0 != 1 && D_800BE9F0 != 50) {
        D_800D9F60 = 0;
    } else {
        D_800D9F60 = 1;
    }
}

extern void func_10004074(void *);
extern u8 D_1A37E0[];

/* Reads each texture's decompressed size from the 4-byte header of its compressed data in ROM. */
void func_15003570(void) {
    u32 romAddr;
    s32 odd;
    u8 *buf;
    s32 i;
    u8 *header;
    s32 size;

    buf = allocate_memory(16, 1, 2, 0);
    romAddr = (u32)D_1A37E0;
    for (i = 0; i < NUM_TEXTURES; i++) {
        if (romAddr & 1) {
            romAddr--;
            odd = 1;
        } else {
            odd = sTexCompressedSizes[i] * 0; // matching: a use here ranks the size-table IV above the store IV
        }
        func_10004514(romAddr, buf, 16, 1);
        header = odd + buf; // matching: operand order fixes the addu operand order
        size = (header[0] << 24) + (header[1] << 16) + (header[2] << 8) + header[3];
        sTexSizes[i] = size;
        romAddr += sTexCompressedSizes[i] + odd;
    }

    func_10004074(buf);
}

typedef struct {
    s32 unk0;
    s32 unk4;
} SegmentEntry;

typedef struct {
    u8  pad0[0x1C];
    s32 unk1C;
    u8  pad20[0x2F];
    u8  unk4F;
    u8  pad50[0x6];
    s16 unk56;
    s16 unk58;
    u8  pad5A[0x46];
} SceneObject;

extern void func_15001460(s32 arg0);
extern void func_15002754(void);
extern s32 func_15002878(void);
extern void func_1510F800(s32 arg0);

void func_15003668(s32 arg0) {
    s32 start;
    s32 size;
    s32 i;

    D_800DBE62 = (u8)0;
    func_15001970();
    D_800D2C68 = (u8)0;

    func_150026C4(0);
    func_15002724(D_800B0E00[0]);
    func_15002754();
    func_15001460(D_800B0E00[0]);
    func_150026E8(0);

    func_150026C4(1);
    func_15002724(D_800B0E00[3]);
    func_15002754();
    func_15001460(D_800B0E00[3]);
    func_150026E8(1);

    func_150026C4(2);
    for (i = 0; i < D_800DBEF0; i++) {
        if ((((SceneObject *)D_800DBEF4)[i].unk4F & 0x60) != 0x20) {
            func_15002724(((SceneObject *)D_800DBEF4)[i].unk1C);
        }
    }
    func_15002754();

    D_800D2C68 = (u8)1;
    D_800D2C2C = 0;
    for (i = 0; i < D_800DBEF0; i++) {
        if ((((SceneObject *)D_800DBEF4)[i].unk4F & 0x60) != 0x20) {
            start = D_800DBE38;
            ((SceneObject *)D_800DBEF4)[i].unk58 = D_800DBE38;
            func_15001460(((SceneObject *)D_800DBEF4)[i].unk1C);
            ((SceneObject *)D_800DBEF4)[i].unk56 = D_800DBE38 - start;
        } else {
            ((SceneObject *)D_800DBEF4)[i].unk58 = 0;
            ((SceneObject *)D_800DBEF4)[i].unk56 = 0;
        }
    }
    func_150026E8(2);

    D_800D2C68 = (u8)0;
    ((SegmentEntry *)&D_800D3300)->unk0 = 0;
    ((SegmentEntry *)&D_800D3300)->unk4 = D_800DBE3C << 8;
    func_1510F800(0);

    size = (func_15002878() + 7) & ~7;
    D_800D3668 = D_8003809C;
    D_800D366C = D_800D3668 + size;
    func_1510F800(2);
}

// MATCHED 2026-08-21.  The residual described below was a 3-way rotation of the
// callee-saved registers holding {i, &D_800DBEF4, start}.  IDO ranks webs by
// reference count (defs+uses) and hands out $s1,$s2,... in that order; golden's
// ranking is off(10) > i(8) > base(7) > start(3).  Writing
//     start = D_800DBE38;  obj->unk58 = start;
// gives `start` THREE references, which lifts it above i and base and takes $s1.
// Spelling the store as `obj->unk58 = D_800DBE38;` leaves `start` with two
// references (one def, one use in the subtraction); IDO CSEs the two loads of
// D_800DBE38 into the single `lw $s3, 0($s4)` golden emits, the instruction
// stream is unchanged, and `start` drops to $s3.  Everything else in the note
// below (declaration order, block scoping, merging locals, `register`, the
// unused parameter, the line-join lever) was re-measured this wave and is inert
// -- all of it plateaus at 20 mismatching words.

// SUPERSEDED (kept for the record) -- score 125, instruction-for-instruction
// identical; the only
// residual is a 3-way rotation of the callee-saved registers holding the loop
// counter (target s1 / IDO s2), the hoisted &D_800DBEF4 base (target s2 / IDO
// s3) and `start` (target s3 / IDO s1). Declaration order, block scoping,
// merging `start` with `size`, chained assignment, `register` and using the
// (unused) parameter as the scratch all leave the rotation unchanged --
// permuter candidate.
//
// typedef struct {
//     s32 unk0;
//     s32 unk4;
// } SegmentEntry;
//
// typedef struct {
//     u8  pad0[0x1C];
//     s32 unk1C;
//     u8  pad20[0x2F];
//     u8  unk4F;
//     u8  pad50[0x6];
//     s16 unk56;
//     s16 unk58;
//     u8  pad5A[0x46];
// } SceneObject; /* size 0xA0 */
//
// extern void func_15001460(s32 arg0);
// extern void func_15002754(void);
// extern s32 func_15002878(void);
// extern void func_1510F800(s32 arg0);
//
// void func_15003668(s32 arg0) {
//     s32 start;
//     s32 size;
//     s32 i;
//
//     arg0 = arg0;
//     D_800DBE62 = (u8)0;
//     func_15001970();
//     D_800D2C68 = (u8)0;
//
//     func_150026C4(0);
//     func_15002724(D_800B0E00[0]);
//     func_15002754();
//     func_15001460(D_800B0E00[0]);
//     func_150026E8(0);
//
//     func_150026C4(1);
//     func_15002724(D_800B0E00[3]);
//     func_15002754();
//     func_15001460(D_800B0E00[3]);
//     func_150026E8(1);
//
//     func_150026C4(2);
//     for (i = 0; i < D_800DBEF0; i++) {
//         if ((((SceneObject *)D_800DBEF4)[i].unk4F & 0x60) != 0x20) {
//             func_15002724(((SceneObject *)D_800DBEF4)[i].unk1C);
//         }
//     }
//     func_15002754();
//
//     D_800D2C68 = (u8)1;
//     D_800D2C2C = 0;
//     for (i = 0; i < D_800DBEF0; i++) {
//         if ((((SceneObject *)D_800DBEF4)[i].unk4F & 0x60) != 0x20) {
//             start = D_800DBE38;
//             ((SceneObject *)D_800DBEF4)[i].unk58 = start;
//             func_15001460(((SceneObject *)D_800DBEF4)[i].unk1C);
//             ((SceneObject *)D_800DBEF4)[i].unk56 = D_800DBE38 - start;
//         } else {
//             ((SceneObject *)D_800DBEF4)[i].unk58 = 0;
//             ((SceneObject *)D_800DBEF4)[i].unk56 = 0;
//         }
//     }
//     func_150026E8(2);
//
//     D_800D2C68 = (u8)0;
//     ((SegmentEntry *)&D_800D3300)->unk0 = 0;
//     ((SegmentEntry *)&D_800D3300)->unk4 = D_800DBE3C << 8;
//     func_1510F800(0);
//
//     size = (func_15002878() + 7) & ~7;
//     D_800D3668 = D_8003809C;
//     D_800D366C = D_800D3668 + size;
//     func_1510F800(2);
// }

void func_150038A0(void) {
    func_15001A08();
    func_15001BC8(0, &D_800DBE2A, &D_800DBE2C);
    func_15001BC8(1, 0, 0);
    func_15001CEC(2);
    func_1510F800(0);
    func_150025FC();
    D_800DBE62 = (u8)1;
}

s32 func_1500390C(s32 arg0) {
    s32 i;

    for (i = 0; i < 7762; i++) {
        if (arg0 == D_800B0E58[i]) {
            return i;
        }
    }
    return -1;
}

void func_150039B0(s32 arg0) {
}

void func_150039BC(s32 arg0) {
    D_800DBE5C = arg0;
    if (D_800DBE5C) {
        D_800DBE5C += 8;
    }
}
