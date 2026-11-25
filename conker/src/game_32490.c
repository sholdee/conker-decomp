#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern void func_10004074(void *ptr); // frees an allocate_memory() buffer

/*
 * Loads asset `assetId` (a table of struct161 spheres: x, y, z, radius, ...) and converts it into the
 * s16 aim point table D_800C6650, which also caches radius squared and the x extent. Returns the count.
 */
s32 func_15004FE0(s32 assetId) {
    struct161 *src;
    s32 remaining;
    s32 i;

    src = NULL;
    i = 0;
    remaining = func_1502B7F0((void **)&src, 3, 12, assetId, 3) / sizeof(struct161);
    D_800C6650 = allocate_memory(remaining * sizeof(struct160), 1, 1, 0);
    D_800C6654 = remaining;
    while (remaining > 0) {
        D_800C6650[i].unk0 = src[i].unk0;
        D_800C6650[i].unk2 = src[i].unk4;
        D_800C6650[i].unk4 = src[i].unk8;
        D_800C6650[i].unk6 = 0;
        D_800C6650[i].unk8 = src[i].unkC * src[i].unkC;
        D_800C6650[i].unkC = src[i].unk10;
        D_800C6650[i].unkE = src[i].unk0 + src[i].unkC;
        D_800C6650[i].unk10 = src[i].unk0 - src[i].unkC;
        D_800C6650[i].unk12 = src[i].unk14;
        remaining--;
        i++;
    }
    if (src != NULL) {
        func_10004074(src);
    }
    return i;
}
