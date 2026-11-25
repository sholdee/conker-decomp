#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150FDD10(s32 arg0) {
    Actor *obj;

    func_15103828();
    if (D_800C35EA[0] == 1) {
        obj = &movement_obj[0];
        do {
            if (obj->character_type == 0x28) {
                func_150ED638(obj, 0x14, 0x14);
            }
            obj++;
        } while (obj != &D_800D121C);
    }
}
