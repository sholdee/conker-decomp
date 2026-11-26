#include <ultra64.h>

#include "functions.h"
#include "variables.h"


typedef struct {
    s32 start;
    s32 end;
    s32 unk8;
} RelocRange;

/* The vertex color groups D_800B0E30 (VtxColorGroup) as loaded: their pointers are still offsets to rebase. */
#define sRangeLists (*(RelocRange ***)&D_800B0E30)
#define sRangeCounts D_800B0E34

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



extern s32 D_800B0E20[];

/* Loads level `levelId`'s models, rebases them and sets up the first four as the level's display lists. */
void func_150031EC(s32 levelId) {
    LevelModelEntry *models;
    ModelAsset *model;
    s32 count;
    s32 loaded;
    u32 i;
    s32 j;
    s32 k;
    s32 size;
    s32 id;

    if (levelId >= 69) {
        levelId = 0;
    }
    models = func_1502B6BC(&loaded, 0, &count, 2, 4, levelId);
    D_800B0E50 = models;
    for (k = 1; k < 4; k++) {
        D_800B0E00[k] = 0;
        D_800B0E10[k] = NULL;
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
                if (model->displayList != NULL) {
                    model->displayList = (Gfx *)((u8 *)model->displayList + (s32)model);
                }
                if (model->unk8 != 0) {
                    model->unk8 += (s32)model;
                }
                if (model->materials != NULL) {
                    model->materials = (s32 *)((u8 *)model->materials + (s32)model);
                }
                func_15003120(i, (s32)model, model->rangeList);
                if (i < 4) {
                    D_800B0E00[i] = model->displayList;
                    D_800B0E10[i] = model->vertices;
                    D_800B0E40[i] = (u32)((s32)D_800B0E00[i] - (s32)D_800B0E10[i]) / sizeof(Vtx); // the display list follows the vertices
                    D_800B0E20[i] = model->unk8;
                    if (i == 0) {
                        func_150039BC((s32)model->materials);
                    }
                    func_150049A4((DlCommand *)D_800B0E00[i], (s32)D_800B0E10[i] + 0xFF000000, (s32)model);
                    func_1510CE60(D_800B0E00[i], 0, 1, 63, NULL);
                } else {
                    func_150049A4((DlCommand *)model->displayList, 0, (s32)model);
                }
            }
        }
    }
    if (D_800B0DF0->assetList != 0) {
        j = 0;
        while ((id = D_80082B20[D_800B0DF0->assetList][j++]) != 0) {
            func_1510D0EC(id, &size, 62, 0);
        }
    }
}

#define NUM_TEXTURES 7762


/* Marks every texture unloaded and resets the requested texture id range. */
s32 func_150034B4(void) {
    s32 i;

    for (i = 0; i < NUM_TEXTURES; i++) {
        D_800B0E58[i] = -1;
        D_800BC448[i] = 0;
    }

    D_800D9F58 = 0xFFFF;
    D_800D9F5C = -1;

    if (level != 1 && level != 50) {
        D_800D9F60 = 0;
    } else {
        D_800D9F60 = 1;
    }
}

extern u8 compressed_ROM_START[];

/* Reads each texture's decompressed size from the 4-byte header of its compressed data in ROM. */
void func_15003570(void) {
    u32 romAddr;
    s32 odd;
    u8 *buf;
    s32 i;
    u8 *header;
    s32 size;

    buf = allocate_memory(16, 1, 2, 0);
    romAddr = (u32)compressed_ROM_START;
    for (i = 0; i < NUM_TEXTURES; i++) {
        if (romAddr & 1) {
            romAddr--;
            odd = 1;
        } else {
            odd = D_80091D20[i] * 0; // matching: a use here ranks the size-table IV above the store IV
        }
        func_10004514(romAddr, buf, 16, 1);
        header = odd + buf; // matching: operand order fixes the addu operand order
        size = (header[0] << 24) + (header[1] << 16) + (header[2] << 8) + header[3];
        D_800B87A0[i] = size;
        romAddr += D_80091D20[i] + odd;
    }

    func_10004074(buf);
}


/*
 * Builds the collision triangle tables of the three maps: map 0 from level display list 0, map 1
 * from display list 3 and map 2 from the db_Object_array objects, recording each object's triangle range.
 */
void func_15003668(s32 unused) {
    s32 firstTri;
    s32 size;
    s32 i;

    D_800DBE62 = 0;
    func_15001970();
    D_800D2C68 = 0;

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
    for (i = 0; i < gWorldObjectCount; i++) {
        if ((db_Object_array[i].object_status & 0x60) != 0x20) {
            func_15002724(db_Object_array[i].displayList);
        }
    }
    func_15002754();

    D_800D2C68 = 1;
    D_800D2C28[1] = 0;
    for (i = 0; i < gWorldObjectCount; i++) {
        if ((db_Object_array[i].object_status & 0x60) != 0x20) {
            // matching: storing D_800DBE38 rather than firstTri keeps firstTri at two references (regalloc)
            firstTri = D_800DBE38;
            db_Object_array[i].firstTri = D_800DBE38;
            func_15001460(db_Object_array[i].displayList);
            db_Object_array[i].numTris = D_800DBE38 - firstTri;
        } else {
            db_Object_array[i].firstTri = 0;
            db_Object_array[i].numTris = 0;
        }
    }
    func_150026E8(2);

    D_800D2C68 = 0;
    D_800D3300[0].height = 0;
    D_800D3300[0].tri = (s32 *)((s32)D_800DBE3C << 8);
    func_1510F800(0);

    size = (func_15002878() + 7) & ~7;
    D_800D3668 = (u8 *)D_8003809C;
    D_800D366C = D_800D3668 + size;
    func_1510F800(2);
}

/* Builds the quadtrees of the three collision maps (map 2 gets a single leaf). */
void func_150038A0(void) {
    func_15001A08();
    func_15001BC8(0, &D_800DBE2A, &D_800DBE2C);
    func_15001BC8(1, NULL, NULL);
    func_15001CEC(2);
    func_1510F800(0);
    func_150025FC();
    D_800DBE62 = 1;
}

/* Returns the id of the texture loaded at `addr`, or -1. */
s32 func_1500390C(s32 addr) {
    s32 i;

    for (i = 0; i < NUM_TEXTURES; i++) {
        if (addr == D_800B0E58[i]) {
            return i;
        }
    }
    return -1;
}

void func_150039B0(s32 arg0) {
}

/* Sets D_800DBE5C to `addr` + 8 (past the material block's header), or to NULL when `addr` is 0. */
void func_150039BC(s32 addr) {
    D_800DBE5C = (u32 *)addr;
    if (D_800DBE5C != NULL) {
        D_800DBE5C += 2;
    }
}
