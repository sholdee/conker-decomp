#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-level hooks, indexed by the level's hook number (D_800B0DF0->unkF; 0 = none). The handlers take
 * different arguments, so the table has no prototype. */
static void (*gLevelHooks[])() = {
    NULL, camera_levelHook01, func_150AE5B0, camera_levelHook03,
    camera_levelHook04, camera_levelHook05, camera_levelHook06, camera_levelHook07,
    effect_emptyLevelHook8, camera_levelHook09, camera_levelHook10, camera_resetDistances,
    func_150E0D90, camera_levelHook13, effect_emptyLevelHook14, camera_levelHook15,
    func_150CF080, func_150ECC70, camera_levelHook18, camera_levelHook19,
    camera_scaleBySpeed, func_150B2340, camera_levelHook22, camera_levelHook23,
    func_150F43F0, camera_levelHook25, func_150F26A0, func_150BEEB0,
    camera_levelHook28, func_150B1E20, func_150D08C0, func_150D1BD0,
    camera_levelHook32, camera_levelHook33, camera_levelHook34, camera_levelHook35,
};

void level_runHook(void *arg0, s32 arg1) {
    if (D_800B0DF0->unkF) {
        gLevelHooks[D_800B0DF0->unkF](arg0);
    }
}
