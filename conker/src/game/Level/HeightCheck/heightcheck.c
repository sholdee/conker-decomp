#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 level_isCameraAboveLimit(s32 arg0) {
    if ((D_800B0DF0->unk14 == 1)) {
        s32 tmp = camera_global_Cameras->eye.y;
        if (D_800B0DF0->unk16 < tmp) {
            return 1;
        }
    }
    return 0;
}
