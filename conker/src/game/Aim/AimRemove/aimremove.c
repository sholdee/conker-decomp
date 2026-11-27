#include <ultra64.h>

// This file's .bss, defined before the headers that declare the globals (conker/aim.h): IDO lays
// out .bss in the order it first sees each name.
struct AimPoint *gAimPoints; // aim points (gAimPointCount of them)
s32 gAimPointCount;

#include "conker/aim.h"


/* Removes aim point `index`, then drops removed points from the end of the table. */
void aim_removePoint(s32 index) {
    if (index >= 0 && (u32)index < (u32)gAimPointCount) {
        gAimPoints[index].flags = 2;
        index = gAimPointCount - 1;
        while (gAimPointCount != 0 && (gAimPoints[index].flags & 2)) {
            gAimPointCount--;
            index--;
        }
    }
}
