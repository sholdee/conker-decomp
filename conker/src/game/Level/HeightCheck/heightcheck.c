#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 level_isCameraAboveLimit(s32 arg0) {
    if ((gLevelHeader->unk14 == 1)) {
        s32 tmp = camera_global_Cameras->eye.y;
        if (gLevelHeader->unk16 < tmp) {
            return 1;
        }
    }
    return 0;
}
