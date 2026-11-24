#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_1503DDD0(s32 arg0) {
    if (arg0 >= 0 && (u32)arg0 < (u32)D_800C6654) {
        (*(struct160 **)&D_800C6650)[arg0].unk6 = 2;
        arg0 = D_800C6654 - 1;
        while (D_800C6654 != 0 && ((u16)(*(struct160 **)&D_800C6650)[arg0].unk6 & 2)) {
            D_800C6654--;
            arg0--;
        }
    }
}
