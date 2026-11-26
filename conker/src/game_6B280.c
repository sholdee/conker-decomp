#include <ultra64.h>
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
