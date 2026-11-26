#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void actor_clearModelTables(void) {
    bzero(&D_800D19A0, 748); // bzero
    bzero(&D_800D1C90, 748);
    bzero(&D_800D1F80, 187);
    bzero(&D_800D2040, 187);
    bzero(&gModelAnimTables, 748);
    bzero(&gModelAnimCounts, 374);
    bzero(&gModelVtxRuns, 748);
    bzero(&gModelTris, 748);
    bzero(&gModelSavedTexCoords, 748);
}
