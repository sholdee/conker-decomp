#include <ultra64.h>
#include "conker/camera.h"
#include "conker/level.h"


s32 level_isCameraAboveLimit(s32 arg0) {
    if ((gLevelHeader->unk14 == 1)) {
        s32 tmp = camera_global_Cameras->eye.y;
        if (gLevelHeader->unk16 < tmp) {
            return 1;
        }
    }
    return 0;
}
