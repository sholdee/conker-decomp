#include <ultra64.h>

/*
 * This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss
 * in the order it first sees each name (see init/Sched/SchedInit/schedinit.c).
 */
u8 gZoneCursorEnabled; // nonzero: zone_advanceObjectCursor cycles the zone-object cursor
u8 gZoneObjectCursor; // zone-object cursor: index into movement_obj (0..24)

#include "functions.h"
#include "variables.h"

/* Walks the zone tree down through the squares containing (x, z). Returns 0 only when the point lies
   in a leaf zone and every zone on the way has `bit` set in views; otherwise 1. */
s32 func_1503F800(void *unused, s16 x, s16 z, s32 bit, s32 unused2) {
    ZoneNode *node;
    s32 offset;

    collision_selectMap(0);
    node = gCurMapQuadTree;
    while (node != NULL) {
        if (node->x + node->halfSize >= x && x >= node->x - node->halfSize
            && node->z + node->halfSize >= z && z >= node->z - node->halfSize) {
            if (!(node->views & (1 << bit))) {
                return 1;
            }
            if (node->child != 0) {
                offset = node->child;
            } else {
                return 0;
            }
        } else {
            offset = node->next;
        }
        if (offset != 0) {
            node = (ZoneNode *)((u8 *)node + offset);
        } else {
            node = NULL;
        }
    }
    return 1;
}

/* func_1503F800 at the object's x/z position. */
s32 func_1503F904(Actor *obj, s32 bit, s32 unused) {
    return func_1503F800(&obj->unk320, obj->xpos, obj->zpos, bit, 1);
}

/* Advances gZoneObjectCursor to the next of the first 25 objects (wrapping) with flag 0x800000 in unkF8. */
void zone_advanceObjectCursor(void) {
    s32 start;
    s32 i;

    if (gZoneCursorEnabled != 0) {
        start = gZoneObjectCursor;
        i = gZoneObjectCursor + 1;
        if (i >= 25) {
            i = 0;
        }
        while (i != start) {
            if (movement_obj[i].noshove & 0x800000) {
                gZoneObjectCursor = i;
                return;
            }
            i++;
            if (i >= 25) {
                i = 0;
            }
        }
    }
}
