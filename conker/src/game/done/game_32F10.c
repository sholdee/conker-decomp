#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Per-level camera setup hooks, indexed by LevelHeader.unkE (0 = none). */
static void (*gLevelCameraSetups[])(Camera *) = {
    NULL,
    func_15005AB0,
    func_15005AF0,
    func_15005B00,
    func_15005B10,
    func_15005B50,
    func_15005B60,
    func_15005B70,
    func_15005BD0,
    func_15005C30,
    func_15005C80,
    func_15005CF0,
    func_15005D00,
    func_15005D60,
    func_15005DA0,
    func_15005DB0,
    func_15005E30,
    func_15005E70,
    func_15005EA0,
    func_15005EE0,
    func_15005F20,
    func_15005F60,
    func_15005FA0,
    func_15005FB0,
    func_15006010,
    func_15006140,
    func_15006170,
    NULL,
};


void camera_runLevelSetup(Camera *arg0, s32 arg1) {
    if (D_800B0DF0->unkE != 0) {
        gLevelCameraSetups[D_800B0DF0->unkE](arg0);
    }
}
