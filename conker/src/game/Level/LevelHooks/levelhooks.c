#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-level hooks, indexed by the level's hook number (D_800B0DF0->unkF; 0 = none). The handlers take
 * different arguments, so the table has no prototype. */
static void (*gLevelHooks[])() = {
    NULL, func_150C7770, func_150AE5B0, func_150C4BF0,
    func_150EB1C0, func_150BB700, func_150C7E00, func_150B67E0,
    effect_emptyLevelHook8, func_150DF820, func_150C7350, camera_resetDistances,
    func_150E0D90, func_150CEF10, effect_emptyLevelHook14, func_15103430,
    func_150CF080, func_150ECC70, func_150BE850, func_150FCA00,
    camera_scaleBySpeed, func_150B2340, func_150D0650, func_150F6B00,
    func_150F43F0, func_15107FE0, func_150F26A0, func_150BEEB0,
    func_150F9A20, func_150B1E20, func_150D08C0, func_150D1BD0,
    func_150EC4B0, func_150E1800, func_150F52B0, func_150E1860,
};

void level_runHook(void *arg0, s32 arg1) {
    if (D_800B0DF0->unkF) {
        gLevelHooks[D_800B0DF0->unkF](arg0);
    }
}
