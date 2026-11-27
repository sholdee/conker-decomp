#include <ultra64.h>
#include "conker/camera.h"
#include "conker/cutscene.h"
#include "conker/math.h"




/*
 * Eases the camera's height above unk354 (eyeY - unk354) towards its target height: the preset
 * height, kept 40 units below the player's unk17C and inside [minHeight, maxHeight].
 */
void camera_easeHeight(Camera *camera) {
    f32 targetHeight;
    f32 height;
    s32 cappedByCeiling; // left unset when unk84 bit 4 is set or unk180 >= unk17C (as in the original)
    f32 ceiling;

    if (gSpawnCutsceneActive != 0) {
        return;
    }
    if (!(camera->Mode & 0x100) && camera->DoingRail != 0 && !(camera->RailFlags & 8)) {
        return;
    }
    height = camera->eye.y - camera->unk354;
    if ((camera->SpecialFlags & 0x10) && (camera->PadPtr->button & R_TRIG)) {
        targetHeight = 95.0f;
    } else {
        targetHeight = camera->height;
    }
    if (camera->SpecialFlags & 0x400) {
        targetHeight *= 1.5f;
    }
    if (!(camera->AvailableModes & 0x10)) {
        if (camera->CharPtr->floorY < camera->CharPtr->ceilingY) {
            ceiling = camera->CharPtr->ceilingY - 40.0f;
            // matching: the second test recomputes unk17C - 40.0f instead of reusing ceiling
            if (ceiling < camera->unk354 + targetHeight && camera->unk354 < camera->CharPtr->ceilingY - 40.0f) {
                targetHeight = ceiling - camera->unk354;
                cappedByCeiling = TRUE;
            } else {
                cappedByCeiling = FALSE;
            }
        }
    }
    if (camera->cmdState.minHeight != 0.0f && camera->cmdState.maxHeight != 0.0f) {
        targetHeight = (targetHeight < camera->cmdState.minHeight) ? camera->cmdState.minHeight
                     : (camera->cmdState.maxHeight < targetHeight) ? camera->cmdState.maxHeight
                     : targetHeight;
    }
    if (camera->transition.state != 0) {
        targetHeight = gCameraTransitionHeight;
    }
    if (camera->Snap) {
        camera->YVel = 0; // matching: int 0, not 0.0f: IDO then rematerializes zero instead of reusing $f12
        height = targetHeight;
    } else {
        if (((camera->SpecialFlags & 0x40) && camera->Mode != 0x100) || (camera->Mode & 0x01000000)) {
            targetHeight = 0; // matching: int 0 again (see above)
        } else if (camera->CharPtr->ypos - camera->CharPtr->floorY > 600.0f && !(camera->SpecialFlags & 0x400) &&
                   camera->CharPtr->inwater == 0 && camera->CharPtr->unk102 == 0 &&
                   camera->mv->ropePtr == NULL && camera->mv->ladder == 0 &&
                   camera->CharPtr->unk137 == 0 && camera->CharPtr->animseq.uh != 0x194 && !(camera->SpecialFlags & 0x80)) {
            return;
        }
        if ((camera->Mode & 0x100) || cappedByCeiling) {
            math_springTo(&height, targetHeight, &camera->YVel, 5.0f, 9.0f, camera->SpringTime);
        } else {
            math_springTo(&height, targetHeight, &camera->YVel, camera->AccelA,
                          camera->AccelB, camera->SpringTime);
        }
    }
    camera->eye.y = camera->unk354 + height;
    camera->unk344 = camera->eye.y - camera->unk354;
    if (camera->CharPtr->inwater == 1 && camera->DoingAim != 0 && camera->waterY != -10000.0f) {
        if (camera->eye.y < camera->waterY + 5.0f) {
            camera->eye.y = camera->waterY + 5.0f;
            camera->unk344 = 5.0f;
        }
    }
}
