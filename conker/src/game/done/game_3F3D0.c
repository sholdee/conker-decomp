#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011F20(s32 arg0) {
    Vec3f tmp;
    struct37 *temp_v0;

    tmp.x = 50.0f;
    tmp.y = 50.0f;
    tmp.z = 0.0f;
    temp_v0 = func_15149130(300, -1, 71, -1, 0, 0, 12, 255, 1);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk28, &tmp, 12);
    }
}
