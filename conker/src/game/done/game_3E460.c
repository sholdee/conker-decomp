#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15010FB0(void) {
    D_800BE4F0 = allocate_memory(16, 1, 0, 0);
    D_800BE4F0[0] = func_15195AA8(D_800B0E00[1], D_80090310, 1, -1, 0, 1, 0, 0);
    D_800BE4F0[1] = func_15195AA8(D_800B0E00[0], D_80090314, 0, -1, 0, 0, 0, 0);
    D_800BE4F0[2] = (TexScroll *)func_15195FB0((ResourceEntry *)func_151149AC(250), D_80090310, 1, -1, 0, 0, 0);
    D_800BE4F0[3] = (TexScroll *)func_15195FB0((ResourceEntry *)func_151149AC(249), D_80090310, 1, -1, 0, 0, 0);
    func_1510F800(0);
    D_800D9A40 = (s16 *)func_1510FD20(0x1500, 0x700);
}
