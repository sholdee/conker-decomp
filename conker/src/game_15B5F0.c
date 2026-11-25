#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 accel, f32 maxSpeed, f32 dt);

/*
 * Eases the camera's height above unk354 (eyeY - unk354) towards its target height: the preset
 * height, kept 40 units below the player's unk17C and inside [minHeight, maxHeight].
 */
void func_1512E140(Camera *camera) {
    f32 targetHeight;
    f32 height;
    s32 cappedByCeiling; // left unset when unk84 bit 4 is set or unk180 >= unk17C (as in the original)
    f32 ceiling;

    if (D_800C3671 != 0) {
        return;
    }
    if (!(camera->mode & 0x100) && camera->unk92C != 0 && !(camera->pathFlags & 8)) {
        return;
    }
    height = camera->eye.y - camera->unk354;
    if ((camera->status & 0x10) && (camera->contPad->button & R_TRIG)) {
        targetHeight = 95.0f;
    } else {
        targetHeight = camera->height;
    }
    if (camera->status & 0x400) {
        targetHeight *= 1.5f;
    }
    if (!(camera->flags & 0x10)) {
        if (camera->player->floorY < camera->player->ceilingY) {
            ceiling = camera->player->ceilingY - 40.0f;
            // matching: the second test recomputes unk17C - 40.0f instead of reusing ceiling
            if (ceiling < camera->unk354 + targetHeight && camera->unk354 < camera->player->ceilingY - 40.0f) {
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
        targetHeight = D_800DC004;
    }
    if (camera->snap) {
        camera->heightVel = 0; // matching: int 0, not 0.0f: IDO then rematerializes zero instead of reusing $f12
        height = targetHeight;
    } else {
        if (((camera->status & 0x40) && camera->mode != 0x100) || (camera->mode & 0x01000000)) {
            targetHeight = 0; // matching: int 0 again (see above)
        } else if (camera->player->ypos - camera->player->floorY > 600.0f && !(camera->status & 0x400) &&
                   camera->player->inwater == 0 && camera->player->unk102 == 0 &&
                   camera->playerExt->ropePtr == NULL && camera->playerExt->ladder == 0 &&
                   camera->player->unk137 == 0 && camera->player->animseq.uh != 0x194 && !(camera->status & 0x80)) {
            return;
        }
        if ((camera->mode & 0x100) || cappedByCeiling) {
            func_150495B0(&height, targetHeight, &camera->heightVel, 5.0f, 9.0f, camera->dt);
        } else {
            func_150495B0(&height, targetHeight, &camera->heightVel, camera->distanceAccel,
                          camera->distanceMaxSpeed, camera->dt);
        }
    }
    camera->eye.y = camera->unk354 + height;
    camera->unk344 = camera->eye.y - camera->unk354;
    if (camera->player->inwater == 1 && camera->aimState != 0 && camera->unk360 != -10000.0f) {
        if (camera->eye.y < camera->unk360 + 5.0f) {
            camera->eye.y = camera->unk360 + 5.0f;
            camera->unk344 = 5.0f;
        }
    }
}
