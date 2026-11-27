#include <ultra64.h>
#include "conker/camera.h"
#include "conker/level.h"


s32 level_isCameraAboveLimit(s32 levelId) {
    if ((gLevelHeader->hasCameraLimit == 1)) {
        s32 tmp = camera_global_Cameras->eye.y;
        if (gLevelHeader->cameraLimitY < tmp) {
            return 1;
        }
    }
    return 0;
}
