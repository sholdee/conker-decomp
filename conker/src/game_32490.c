#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern void func_10004074(void *ptr); // frees an allocate_memory() buffer

/*
 * Loads asset `assetId` (a table of AimPointDef spheres) and converts it into the
 * s16 aim point table D_800C6650, which also caches radius squared and the x extent. Returns the count.
 */
s32 func_15004FE0(s32 assetId) {
    AimPointDef *src;
    s32 remaining;
    s32 i;

    src = NULL;
    i = 0;
    remaining = func_1502B7F0((void **)&src, 3, 12, assetId, 3) / sizeof(AimPointDef);
    D_800C6650 = allocate_memory(remaining * sizeof(AimPoint), 1, 1, 0);
    D_800C6654 = remaining;
    while (remaining > 0) {
        D_800C6650[i].x = src[i].x;
        D_800C6650[i].y = src[i].y;
        D_800C6650[i].z = src[i].z;
        D_800C6650[i].flags = 0;
        D_800C6650[i].radiusSq = src[i].radius * src[i].radius;
        D_800C6650[i].unkC = src[i].unk10;
        D_800C6650[i].maxX = src[i].x + src[i].radius;
        D_800C6650[i].minX = src[i].x - src[i].radius;
        D_800C6650[i].unk12 = src[i].unk14;
        remaining--;
        i++;
    }
    if (src != NULL) {
        func_10004074(src);
    }
    return i;
}
