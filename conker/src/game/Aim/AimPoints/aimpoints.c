#include <ultra64.h>

#include "conker/aim.h"
#include "conker/heap.h"
#include "conker/rom.h"


/*
 * Loads asset `assetId` (a table of AimPointDef spheres) and converts it into the
 * s16 aim point table gAimPoints, which also caches radius squared and the x extent. Returns the count.
 */
s32 aim_loadPoints(s32 assetId) {
    AimPointDef *src;
    s32 remaining;
    s32 i;

    src = NULL;
    i = 0;
    remaining = rom_loadPathTo(&src, 3, 12, assetId, 3) / sizeof(AimPointDef);
    gAimPoints = allocate_memory(remaining * sizeof(AimPoint), 1, 1, 0);
    gAimPointCount = remaining;
    while (remaining > 0) {
        gAimPoints[i].x = src[i].x;
        gAimPoints[i].y = src[i].y;
        gAimPoints[i].z = src[i].z;
        gAimPoints[i].flags = 0;
        gAimPoints[i].radiusSq = src[i].radius * src[i].radius;
        gAimPoints[i].unkC = src[i].unk10;
        gAimPoints[i].maxX = src[i].x + src[i].radius;
        gAimPoints[i].minX = src[i].x - src[i].radius;
        gAimPoints[i].unk12 = src[i].unk14;
        remaining--;
        i++;
    }
    if (src != NULL) {
        heap_free(src);
    }
    return i;
}
