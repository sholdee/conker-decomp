#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// ???
void func_150104F0(void) {
    WorldObject *temp_v0;

    gMaterialGlowLevels[0] = (gMaterialGlowLevels[1] = (gMaterialGlowLevels[2] = 0));
    temp_v0 = gfx_findObjectById(0xF6);
    temp_v0->touch.f.a = 2.0f;
    gWarpModelSetupDone = 0;
}

void func_15010538(Actor *arg0) {
    BoltSource tmp;
    TimedEffect *effect;

    light_createAtActor(arg0, 2, 2, 300, 30, 100, 200, 255, 255, 1);
// FAKEMATCH but works...
dummy_label_927029:
    tmp.obj = arg0;
    tmp.id = arg0->unique_id;
    tmp.flashTimer = 0;
    tmp.boltTimer = 0;
    tmp.timer2 = 0;

    effect = effect_newTimedEffect(300, -1, 80, -1, 0, 61, 12, 255, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
}

/* Clears the flag that script command 2 sets and the two 14-byte tables after it (as level_resetSetupState does). */
void func_15010600(void) {
    s32 i;

    D_800D9921 = 0;
    D_800D9920 = 0;
    for (i = 0; i < 14; i++) {
        D_800D9928[i] = 0;
        D_800D9938[i] = 0;
    }
}
