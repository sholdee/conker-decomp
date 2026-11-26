#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void actor_clearModelTables(void) {
    bzero(&gModelData, 748); // bzero
    bzero(&gModelDefs, 748);
    bzero(&D_800D1F80, 187);
    bzero(&gModelUnloadTimers, 187);
    bzero(&gModelAnimTables, 748);
    bzero(&gModelAnimCounts, 374);
    bzero(&gModelVtxRuns, 748);
    bzero(&gModelTris, 748);
    bzero(&gModelSavedTexCoords, 748);
}
