#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150FDD10(s32 arg0) {
    s32 i;

    func_15103828();
    if (D_800C35EA[0] == 1) {
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].character_type == 0x28) {
                func_150ED638(&movement_obj[i], 0x14, 0x14);
            }
        }
    }
}
