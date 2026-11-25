#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800DC004; // camera transition height (see func_15128030)

void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 accel, f32 maxSpeed, f32 dt);

/* The player object the camera follows (Camera.player, a Actor): the fields Actor does not
   declare yet (0x137, 0x17C) or declares as a union (0x84). */
typedef struct CamPlayer {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 y_position;
    /* 0x01C */ u8 pad1C[0x68];
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[0x27];
    /* 0x0AD */ u8 in_water;
    /* 0x0AE */ u8 padAE[0x54];
    /* 0x102 */ u8 unk102;
    /* 0x103 */ u8 pad103[0x34];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x44];
    /* 0x17C */ f32 unk17C;
    /* 0x180 */ f32 unk180;
} CamPlayer;

/* Camera.playerExt (ActorCtl): 0x9C, which ActorCtl does not declare yet, and 0x95, read unsigned
   here (ActorCtl declares it s8). */
typedef struct CamPlayerExt {
    /* 0x000 */ u8 pad0[0x95];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x6];
    /* 0x09C */ s32 unk9C;
} CamPlayerExt;

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
        if (camera->player->unk180 < ((CamPlayer *)camera->player)->unk17C) {
            ceiling = ((CamPlayer *)camera->player)->unk17C - 40.0f;
            // matching: the second test recomputes unk17C - 40.0f instead of reusing ceiling
            if (ceiling < camera->unk354 + targetHeight && camera->unk354 < ((CamPlayer *)camera->player)->unk17C - 40.0f) {
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
        } else if (camera->player->y_position - camera->player->unk180 > 600.0f && !(camera->status & 0x400) &&
                   camera->player->in_water == 0 && camera->player->unk102 == 0 &&
                   ((CamPlayerExt *)camera->playerExt)->unk9C == 0 && ((CamPlayerExt *)camera->playerExt)->unk95 == 0 &&
                   ((CamPlayer *)camera->player)->unk137 == 0 && ((CamPlayer *)camera->player)->unk84 != 0x194 && !(camera->status & 0x80)) {
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
    if (camera->player->in_water == 1 && camera->aimState != 0 && camera->unk360 != -10000.0f) {
        if (camera->eye.y < camera->unk360 + 5.0f) {
            camera->eye.y = camera->unk360 + 5.0f;
            camera->unk344 = 5.0f;
        }
    }
}
