#include <ultra64.h>
#include "conker/collision.h"
#include "conker/face.h"
#include "conker/rom.h"

/* Loads the level's cylinder-set table (gCylinderSets) and relocates each set's cylinder offset to a pointer. */
void collision_loadCylinderSets(s32 arg0) {
    s32 count;
    s32 i;

    if ((s32)rom_loadPathTo(&gCylinderSets, 3, 12, arg0, 9) > 0) {
        count = 1;
        i = 0;
        while (gCylinderSets[i].cylinders != NULL) {
            count++;
            i++;
        }
        for (i = 0; i < count - 1; i++) {
            gCylinderSets[i].cylinders = (BoundCylinder *)((s32)gCylinderSets[i].cylinders + (s32)gCylinderSets);
        }
    } else {
        gCylinderSets = NULL;
    }
}
