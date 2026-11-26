#include <ultra64.h>

#include "functions.h"
#include "variables.h"



/* Clears the per-id record flag bits (gTaskDoneBits) and state tables. */
void zone_resetTaskProgress(void) {
    script_reallocTaskDoneBits();
    bzero(gTaskDoneBits, 0x1B);
    bzero(&D_800D2E50, 0x10);
    bzero(&gSeenTextBits, 9);
    bzero(gTaskStates, 0xCC);
    D_800D2E44 = 0;
}

/* Resets the per-id record list (gTaskWorkingSet) and related state. */
void zone_resetTaskWorkingSet(void) {
    script_purgeTasks(1);
    bzero(&gTaskWorkingSet, 0xC);
    script_resetTimers();
    D_800D2DB0 = 999.0f;
    D_800D2E41 = 10;
    D_800D2E42 = 6;
    gScriptSavedSp = 0;
    D_800D2E44 = 0;
}


/* Moves every listed PURGE_BETWEEN_SECTIONS task to the current level (LoadSection), then reloads the tasks. */
void zone_reloadSectionTasks(void) {
    IdRecord *node;
    s32 i;

    for (i = 0; i < gLevelTaskCount; i++) {
        node = script_findTask(gLevelTaskIds[i]);
        if (node != NULL && (node->TaskID & PURGE_BETWEEN_SECTIONS)) {
            node->LoadSection = level;
        }
    }
    script_purgeTasks(0);
    script_loadLevelTasks((s16)level, 1);
}

/* Loads the level's object table and builds the list gCharFlagZones of the ZONE_TYPE_CHAR_FLAGS TriggerZones with subTypeData 0, 2 or 6. */
void zone_loadObjects(s32 arg0) {
    u32 i;
    u32 count;
    s32 type;

    gZoneCount = rom_loadPathTo(&zones, 3, 0xC, arg0, 5);
    count = gZoneCount / sizeof(TriggerZone);
    gZoneCount = count;
    zone_computeAllBounds(zones, count);
    gCharFlagZoneCount = 0;
    gCharFlagZones = NULL;
    for (i = 0; i < gZoneCount; i++) {
        if (ZONE_TYPE(&zones[i]) == ZONE_TYPE_CHAR_FLAGS &&
            (zones[i].subTypeData == 2 || zones[i].subTypeData == 0 || zones[i].subTypeData == 6)) {
            gCharFlagZoneCount++;
        }
        type = ZONE_TYPE(&zones[i]);
        if (type == ZONE_TYPE_SOUND || type == ZONE_TYPE_TUNE_CHANNEL) {
            zones[i].data3 = 0;
        }
    }
    if (gCharFlagZoneCount != 0) {
        gCharFlagZones = allocate_memory(gCharFlagZoneCount * sizeof(TriggerZone *), 1, 0, 0);
        if (gCharFlagZones != NULL) {
            TriggerZone **out = gCharFlagZones;

            for (i = 0; i < gZoneCount; i++) {
                if (ZONE_TYPE(&zones[i]) == ZONE_TYPE_CHAR_FLAGS &&
                    (zones[i].subTypeData == 2 || zones[i].subTypeData == 0 || zones[i].subTypeData == 6)) {
                    *out++ = &zones[i];
                }
            }
        }
    }
    bzero(gActorZoneBits, sizeof(gActorZoneBits));
    bzero(gActorLastZoneBox, sizeof(gActorLastZoneBox));
}

/* Whether the ranges [a0, a1) and [b0, b1) overlap. */
#define OVERLAP(a0, a1, b0, b1) (((a0 <= b0) && (b0 < a1)) || ((b0 < a0) && (a0 < b1)) || ((a0 < b1) && (b1 < a1)))

/*
 * Computes the bounds of every level object and the object bitsets gZonesAlwaysTested (shape 3 or flag 8,
 * without flag 4) and gZonesCandidates (all others without flag 4). For each object in gZonesCandidates it
 * records which other gZonesCandidates objects its bounds overlap.
 */
void zone_computeAllBounds(TriggerZone *objects, s32 count) {
    s32 pad[6]; // matching: stack frame size
    u64 bitI;
    u64 bitJ;
    u64 *wordI;
    u64 *wordJ;
    u64 *overlapWord;
    s32 i;
    s32 j;

    gZoneBounds = allocate_memory(count * sizeof(ZoneBounds), 1, 1, 0);
    for (i = 0; i < count; i++) {
        zone_cacheAngles(&objects[i]);
        zone_computeBounds(&objects[i], &gZoneBounds[i]);
    }
    bzero(gZonesAlwaysTested, sizeof(gZonesAlwaysTested));
    for (i = 0; i < count; i++) {
        if ((ZONE_AREA_TYPE(&objects[i]) == ZONE_AREA_GATE) || (objects[i].flags & 8)) {
            if (!(objects[i].flags & 4)) {
                gZonesAlwaysTested[i >> 6] |= (u64)1 << (i & 63);
            }
        }
    }
    bzero(gZonesCandidates, sizeof(gZonesCandidates));
    for (i = 0; i < count; i++) {
        if (ZONE_AREA_TYPE(&objects[i]) != ZONE_AREA_GATE) {
            if (!(objects[i].flags & 8) && !(objects[i].flags & 4)) {
                gZonesCandidates[i >> 6] |= (u64)1 << (i & 63);
            }
        }
    }
    wordI = gZonesCandidates;
    bitI = 1;
    for (i = 0; i < count; i++) {
        wordJ = gZonesCandidates;
        gZoneBounds[i].numOverlaps = 0;
        if (*wordI & bitI) {
            bitJ = 1;
            overlapWord = (u64 *)&gZoneBounds[i]; // matching: same as .overlaps, but that form schedules differently
            bzero(gZoneBounds[i].overlaps, sizeof(gZoneBounds[i].overlaps));
            for (j = 0; j < count; j++) {
                if (*wordJ & bitJ) {
                    if (OVERLAP(gZoneBounds[i].minX, gZoneBounds[i].maxX, gZoneBounds[j].minX, gZoneBounds[j].maxX) &&
                        OVERLAP(gZoneBounds[i].minY, gZoneBounds[i].maxY, gZoneBounds[j].minY, gZoneBounds[j].maxY) &&
                        OVERLAP(gZoneBounds[i].minZ, gZoneBounds[i].maxZ, gZoneBounds[j].minZ, gZoneBounds[j].maxZ)) {
                        *overlapWord |= bitJ;
                        gZoneBounds[i].numOverlaps++;
                    }
                }
                bitJ <<= 1;
                if (bitJ == 0) {
                    wordJ++;
                    overlapWord++;
                    bitJ = 1;
                }
            }
        }
        bitI <<= 1;
        if (bitI == 0) {
            wordI++;
            bitI = 1;
        }
    }
}

/*
 * Computes obj's axis-aligned bounds: the extremes (and the origin) of the unit cube's corners transformed by the
 * object's scale (from its size and shape) and rotation, offset by its position. Unknown shapes
 * get zero-size bounds at the position.
 */
void zone_computeBounds(TriggerZone *obj, ZoneBounds *bounds) {
    s32 i;
    s32 shape;
    s32 invalid;
    s32 minX, maxX, minY, maxY, minZ, maxZ;
    MtxF transform;
    MtxF rotation;
    f32 signX, signY, signZ;
    f32 x, y, z;

    invalid = 0;
    shape = ZONE_AREA_TYPE(obj);
    switch (shape) {
    default:
        invalid = 1;
        break;
    case ZONE_AREA_CYLINDER:
        math_mtxTranslateF(rotation, 0.0f, 1.0f, 0.0f);
        math_mtxScaleF(transform, obj->xSize, obj->ySize * 0.5f, obj->xSize);
        math_mtxMulF(rotation, transform, transform);
        math_mtxRotateRPYF(rotation, obj->pitch, obj->yaw, 0.0f);
        math_mtxMulF(transform, rotation, transform);
        break;
    case ZONE_AREA_SPHERE:
        math_mtxScaleF(transform, obj->xSize, obj->xSize, obj->xSize);
        break;
    case ZONE_AREA_BOX:
        math_mtxTranslateF(rotation, 0.0f, 1.0f, 0.0f);
        math_mtxScaleF(transform, obj->xSize, obj->ySize * 0.5f, obj->zSize);
        math_mtxMulF(rotation, transform, transform);
        math_mtxRotateRPYF(rotation, obj->pitch, obj->yaw, 0.0f);
        math_mtxMulF(transform, rotation, transform);
        break;
    }
    minZ = 0;
    minY = 0;
    minX = 0;
    maxZ = 0;
    maxY = 0;
    maxX = 0;
    if (!invalid) {
        for (i = 0; i < 8; i++) {
            if (i & 1) {
                signX = 1.0f;
            } else {
                signX = -1.0f;
            }
            if (i & 2) {
                signY = 1.0f;
            } else {
                signY = -1.0f;
            }
            if (i & 4) {
                signZ = 1.0f;
            } else {
                signZ = -1.0f;
            }
            math_transformPoint(transform, signX, signY, signZ, &x, &y, &z);
            if (x < minX) {
                minX = x;
            } else if (maxX < x) {
                maxX = x;
            }
            if (y < minY) {
                minY = y;
            } else if (maxY < y) {
                maxY = y;
            }
            if (z < minZ) {
                minZ = z;
            } else if (maxZ < z) {
                maxZ = z;
            }
        }
    }
    bounds->minX = obj->x + minX;
    bounds->maxX = obj->x + maxX;
    bounds->minY = obj->y + minY;
    bounds->maxY = obj->y + maxY;
    bounds->minZ = obj->z + minZ;
    bounds->maxZ = obj->z + maxZ;
}
