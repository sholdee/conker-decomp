#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals (conker/aim.h): IDO lays
// out .bss in the order it first sees each name.
struct AimPoint *gAimPoints; // aim points (gAimPointCount of them)
s32 gAimPointCount;

#include "functions.h"
#include "variables.h"


/* Removes aim point `arg0`, then drops removed points from the end of the table. */
void aim_removePoint(s32 arg0) {
    if (arg0 >= 0 && (u32)arg0 < (u32)gAimPointCount) {
        gAimPoints[arg0].flags = 2;
        arg0 = gAimPointCount - 1;
        while (gAimPointCount != 0 && (gAimPoints[arg0].flags & 2)) {
            gAimPointCount--;
            arg0--;
        }
    }
}
