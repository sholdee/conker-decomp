#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800DC004; // camera transition height (see func_15128030)

void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 accel, f32 maxSpeed, f32 dt);

/* The player object the camera follows (a struct127; struct127 only has pads at 0x137 and 0x17C). */
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

/* Per-player camera data that sits next to the player object. */
typedef struct CamPlayerExt {
    /* 0x000 */ u8 pad0[0x95];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x6];
    /* 0x09C */ s32 unk9C;
} CamPlayerExt;

/* File-local view of struct108 (the 0x9A0-byte player camera); names follow game_14FF90.c. */
typedef struct Camera {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 mode; // one-hot mode bit
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x1B4];
    /* 0x23C */ u8 snap; // nonzero: jump to targets instead of easing
    /* 0x23D */ u8 pad23D[0x1F];
    /* 0x25C */ f32 heightVel;
    /* 0x260 */ u8 pad260[0x9C];
    /* 0x2FC */ f32 eyeY; // camera position y
    /* 0x300 */ u8 pad300[0x44];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ u8 pad34C[0x8];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ u8 pad358[0x8];
    /* 0x360 */ f32 unk360;
    /* 0x364 */ u8 pad364[0x8];
    /* 0x36C */ OSContPad *contPad;
    /* 0x370 */ u8 pad370[0x44];
    /* 0x3B4 */ f32 distanceAccel;
    /* 0x3B8 */ f32 distanceMaxSpeed;
    /* 0x3BC */ u8 pad3BC[0x14];
    /* 0x3D0 */ CamPlayer *player;
    /* 0x3D4 */ CamPlayerExt *playerExt;
    /* 0x3D8 */ u8 pad3D8[0x218];
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0xFC];
    /* 0x6F0 */ f32 minHeight;
    /* 0x6F4 */ f32 maxHeight;
    /* 0x6F8 */ u8 pad6F8[0x44];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 dt; // frame time scale
    /* 0x7B8 */ u8 pad7B8[0x3C];
    /* 0x7F4 */ u16 transitionState;
    /* 0x7F6 */ u8 pad7F6[0x12A];
    /* 0x920 */ u16 unk920;
    /* 0x922 */ u8 pad922[0xA];
    /* 0x92C */ u8 unk92C;
} Camera;

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
    if (!(camera->mode & 0x100) && camera->unk92C != 0 && !(camera->unk920 & 8)) {
        return;
    }
    height = camera->eyeY - camera->unk354;
    if ((camera->status & 0x10) && (camera->contPad->button & R_TRIG)) {
        targetHeight = 95.0f;
    } else {
        targetHeight = camera->height;
    }
    if (camera->status & 0x400) {
        targetHeight *= 1.5f;
    }
    if (!(camera->unk84 & 0x10)) {
        if (camera->player->unk180 < camera->player->unk17C) {
            ceiling = camera->player->unk17C - 40.0f;
            // matching: the second test recomputes unk17C - 40.0f instead of reusing ceiling
            if (ceiling < camera->unk354 + targetHeight && camera->unk354 < camera->player->unk17C - 40.0f) {
                targetHeight = ceiling - camera->unk354;
                cappedByCeiling = TRUE;
            } else {
                cappedByCeiling = FALSE;
            }
        }
    }
    if (camera->minHeight != 0.0f && camera->maxHeight != 0.0f) {
        targetHeight = (targetHeight < camera->minHeight) ? camera->minHeight
                     : (camera->maxHeight < targetHeight) ? camera->maxHeight
                     : targetHeight;
    }
    if (camera->transitionState != 0) {
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
                   camera->playerExt->unk9C == 0 && camera->playerExt->unk95 == 0 &&
                   camera->player->unk137 == 0 && camera->player->unk84 != 0x194 && !(camera->status & 0x80)) {
            return;
        }
        if ((camera->mode & 0x100) || cappedByCeiling) {
            func_150495B0(&height, targetHeight, &camera->heightVel, 5.0f, 9.0f, camera->dt);
        } else {
            func_150495B0(&height, targetHeight, &camera->heightVel, camera->distanceAccel,
                          camera->distanceMaxSpeed, camera->dt);
        }
    }
    camera->eyeY = camera->unk354 + height;
    camera->unk344 = camera->eyeY - camera->unk354;
    if (camera->player->in_water == 1 && camera->unk73C != 0 && camera->unk360 != -10000.0f) {
        if (camera->eyeY < camera->unk360 + 5.0f) {
            camera->eyeY = camera->unk360 + 5.0f;
            camera->unk344 = 5.0f;
        }
    }
}
