#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_800A1AB0[17];

void func_150F5340(void) {
    typedef struct {
        s32 unk0[17];
    } temp150F5340;
    u8 i;
    temp150F5340 sp28;

    sp28 = *(temp150F5340 *)D_800A1AB0;
    for (i = 0; i < 0x11; i++) {
        func_151C970C(1, &zones[sp28.unk0[i]]);
    }
    func_1515F170(9, 1);
}
