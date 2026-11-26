#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Loads the level's cylinder-set table (D_800D18B0) and relocates each set's cylinder offset to a pointer. */
void collision_loadCylinderSets(s32 arg0) {
    s32 count;
    s32 i;

    if ((s32)rom_loadPathTo(&D_800D18B0, 3, 12, arg0, 9) > 0) {
        count = 1;
        i = 0;
        while (D_800D18B0[i].cylinders != NULL) {
            count++;
            i++;
        }
        for (i = 0; i < count - 1; i++) {
            D_800D18B0[i].cylinders = (BoundCylinder *)((s32)D_800D18B0[i].cylinders + (s32)D_800D18B0);
        }
    } else {
        D_800D18B0 = NULL;
    }
}
