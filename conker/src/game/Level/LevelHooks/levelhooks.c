#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"

/* Per-level hooks, indexed by the level's hook number (gLevelHeader->hookCallback; 0 = none). The handlers take
 * different arguments, so the table has no prototype. */
static void (*gLevelHooks[])() = {
    NULL, camera_levelHook01, effect_levelHook02, camera_levelHook03,
    camera_levelHook04, camera_levelHook05, camera_levelHook06, camera_levelHook07,
    effect_emptyLevelHook8, camera_levelHook09, camera_levelHook10, camera_resetDistances,
    effect_levelHook12, camera_levelHook13, effect_emptyLevelHook14, camera_levelHook15,
    effect_levelHook16, particle_levelHook17, camera_levelHook18, camera_levelHook19,
    camera_scaleBySpeed, particle_levelHook21, camera_levelHook22, camera_levelHook23,
    player_levelHook24, camera_levelHook25, effect_levelHook26, effect_levelHook27,
    camera_levelHook28, particle_levelHook29, level_hook30, light_levelHook31,
    camera_levelHook32, camera_levelHook33, camera_levelHook34, camera_levelHook35,
};

void level_runHook(void *arg0, s32 arg1) {
    if (gLevelHeader->hookCallback) {
        gLevelHooks[gLevelHeader->hookCallback](arg0);
    }
}
