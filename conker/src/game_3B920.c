#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Resets game globals, then runs the subsystem init routines for the given level. */
void func_1500E470(s32 levelId) {
    D_800DD190 = -1;
    bzero(D_800DD198, 24);
    D_800DD404 = 0xFF;
    D_800DD405 = 0;
    D_800DD406 = 0;
    D_800E0930 = 0;
    D_800E0934 = 0;
    D_800E0940 = 0;
    D_8008CD00 = 0;
    D_80088B60 = 0;
    D_800BE4E0 = NULL;
    {
        // matching: IDO schedules the store differently without the block-scoped temp
        u8 result = func_15012720();

        D_800E0A10 = result;
    }
    func_15012370();

    func_151EF640(64);
    func_151732E0(levelId);
    aim_loadPoints(levelId);
    func_150127B0();

    func_1519582C();
    func_15008BE0();
    func_15008B90();
    func_1500E5C0();

    if (D_800B0DF0->unkA) {
        D_80082D90[D_800B0DF0->unkA]();
    }
    func_1500ABA0(levelId);
    func_1500BEC0();
    func_1500AC14();
    func_151872B0(levelId);
    func_15178EB0();
    func_15012FE0();

    func_151645C4(!(D_800D2E4C[0x4] & 0x80));
}
