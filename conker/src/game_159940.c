#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void guMtxIdentF(float mf[4][4]);
extern void guMtxF2L(float mf[4][4], Mtx *m);
extern void guMtxCatL(Mtx *m, Mtx *n, Mtx *res);
f32 func_150AD780(f32);
f32 func_150AD78C(f32);
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 stiffness, f32 damping, f32 dt);
void func_15047B80(Mtx *m, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);
void func_150A7DF0(Mtx *m, f32 rx, f32 ry, f32 rz); // rotation, angles in degrees
void func_150A7D00(Mtx *m, f32 x, f32 y, f32 z);    // translation
s32 func_1502B5C8(s32 *size, s32 arg1, s32 arg2, s32 arg3);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} CamVec3f;

/* A translation plus a rotation (degrees) applied on top of a camera's view matrix. */
typedef struct {
    /* 0x00 */ CamVec3f pos;
    /* 0x0C */ CamVec3f rot;
} CamOffset;

/* Player extension block (struct127.unk31C), as in game_14FF90.c. */
typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0xF];
    /* 0x130 */ CamVec3f aimDir;
    /* 0x13C */ CamVec3f aimEye;
    /* 0x148 */ CamVec3f aimTarget;
    /* 0x154 */ u8 pad154[0x43];
    /* 0x197 */ u8 aimState;
} CamPlayerExt;

/* Partial view of the per-player camera (struct108); field names follow game_14FF90.c's Camera. */
typedef struct Camera {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 mode; /* one-hot mode bit */
    /* 0x030 */ u8 pad30[0x15C];
    /* 0x18C */ f32 lift;       /* vertical offset of eye and target */
    /* 0x190 */ f32 liftTarget;
    /* 0x194 */ f32 side;       /* sideways offset of eye and target */
    /* 0x198 */ f32 sideTarget;
    /* 0x19C */ u8 pad19C[0xA0];
    /* 0x23C */ u8 snap; /* nonzero: jump to targets instead of easing */
    /* 0x23D */ u8 playerIndex;
    /* 0x23E */ u8 zone;
    /* 0x23F */ u8 pad23F[0x25];
    /* 0x264 */ f32 swayVel;
    /* 0x268 */ u8 pad268[0x30];
    /* 0x298 */ s16 playerMoved;
    /* 0x29A */ u8 pad29A[0xA];
    /* 0x2A4 */ CamVec3f focus;
    /* 0x2B0 */ u8 pad2B0[0xC];
    /* 0x2BC */ CamVec3f target; /* look-at point */
    /* 0x2C8 */ u8 pad2C8[0x18];
    /* 0x2E0 */ CamVec3f savedTarget;
    /* 0x2EC */ CamVec3f savedEye;
    /* 0x2F8 */ CamVec3f eye;
    /* 0x304 */ u8 pad304[0x84];
    /* 0x388 */ f32 pitch; /* degrees */
    /* 0x38C */ f32 unk38C;
    /* 0x390 */ u8 pad390[0x8];
    /* 0x398 */ f32 pitchRad;
    /* 0x39C */ u8 pad39C[0x4];
    /* 0x3A0 */ f32 unk3A0; /* yaw in radians */
    /* 0x3A4 */ u8 pad3A4[0x4];
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x24];
    /* 0x3D0 */ struct127 *player;
    /* 0x3D4 */ CamPlayerExt *playerExt;
    /* 0x3D8 */ u8 pad3D8[0x1F8];
    /* 0x5D0 */ f32 unk5D0;
    /* 0x5D4 */ u8 pad5D4[0x14];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 swayAmount; /* roll, degrees */
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0x158];
    /* 0x74C */ f32 unk74C;
    /* 0x750 */ u8 pad750[0x4];
    /* 0x754 */ f32 unk754;
    /* 0x758 */ u8 pad758[0x58];
    /* 0x7B0 */ f32 wobblePhase;
    /* 0x7B4 */ f32 dt; /* frame time scale */
    /* 0x7B8 */ u8 pad7B8[0x18];
    /* 0x7D0 */ f32 sideVel;
    /* 0x7D4 */ f32 liftVel;
    /* 0x7D8 */ f32 wobbleVel;
    /* 0x7DC */ f32 wobbleAmount;
    /* 0x7E0 */ u8 pad7E0[0x6D];
    /* 0x84D */ u8 shakeState; /* 0 idle, 1 start requested, 2 playing */
    /* 0x84E */ u8 shakeFrame;
    /* 0x84F */ u8 pad84F;
    /* 0x850 */ s32 shakeIndex; /* which D_800DC280 table */
    /* 0x854 */ CamVec3f shakeRot;
    /* 0x860 */ CamVec3f shakePos;
} Camera;

/* Per-player view record (0x180 bytes) in the array D_800BE628 points to. */
typedef struct {
    /* 0x00 */ u8 pad0[0x84];
    /* 0x84 */ f32 unk84;
    /* 0x88 */ u8 pad88[0xF8];
} CamView;

typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ CamVec3f unk54;
    /* 0x60 */ u8 pad60[0x8];
} Entry800DC2C0;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0x64];
} Entry800DC320;

extern u8 D_800BE9C0; // which of the two matrix buffers is being built this frame
extern s32 D_800BE9E4; // frames elapsed this update
extern CamOffset *D_800DC280[4]; // camera shake tables: one CamOffset delta per frame
extern s32 D_800DC290[4];        // their lengths in frames
extern CamOffset D_800DCDE0[];
extern Entry800DC2C0 D_800DC2C0[];
extern Entry800DC320 D_800DC320[];
extern LookAt D_800D9B90[];

/* variables.h declares D_800DC2A0 as struct10 *, but it is Mtx *[2]: double-buffered per-player view matrices. */
#define VIEW_MTX(player) (&((Mtx **)&D_800DC2A0)[D_800BE9C0][player])

void func_1512D070(Camera *camera);
void func_1512D2F8(Camera *camera);
void func_1512D368(Camera *camera);

void func_1512C490(Camera *camera) {
    Mtx rotMtx;
    Mtx transMtx;
    s32 pad114;
    f32 side;
    f32 lift;
    f32 shiftX;
    f32 shiftZ;
    s32 pad100;
    f32 sideZ;
    f32 sideX;
    CamVec3f eye;
    CamVec3f at;
    CamVec3f prevEye;
    CamVec3f prevTarget;
    f32 shakeScale;
    CamVec3f dir;
    f32 stiffness;
    f32 damping;
    f32 dirZ;
    f32 dirY;
    f32 dirX;
    f32 dirMtx[4][4];
    f32 yaw;

    shakeScale = ((CamView *)D_800BE628)[camera->playerIndex].unk84;
    shakeScale = (1.0f - shakeScale) * 0.5f + shakeScale;
    if (camera->playerExt->unk120 != 0) {
        camera->sideTarget = 0.0f;
        camera->liftTarget = 0.0f;
        camera->unk74C = 0.0f;
        camera->unk754 = 0.0f;
    }
    if (camera->snap != 0) {
        camera->sideVel = 0.0f;
        camera->liftVel = 0.0f;
        camera->side = camera->sideTarget;
        camera->lift = camera->liftTarget;
    } else if ((camera->mode == 0x100) || (camera->zone != 0)) {
        func_150495B0(&camera->side, camera->sideTarget, &camera->sideVel, 4.0f, 6.0f, camera->dt);
        func_150495B0(&camera->lift, camera->liftTarget, &camera->liftVel, 4.0f, 6.0f, camera->dt);
    } else {
        func_150495B0(&camera->side, camera->sideTarget, &camera->sideVel, 0.75f, 1.1f, camera->dt);
        func_150495B0(&camera->lift, camera->liftTarget, &camera->liftVel, 0.75f, 1.1f, camera->dt);
    }
    camera->savedTarget = camera->target;
    camera->savedEye = camera->eye;
    if ((camera->mode & 0x40000) || (camera->mode & 0x20)) {
        eye = camera->eye;
        at = camera->focus;
    } else if (camera->playerExt->aimState != 0) {
        eye = camera->eye;
        at = camera->target;
        func_1512523C((struct108 *)camera);
        func_15125594((struct108 *)camera);
    } else {
        side = camera->side;
        lift = camera->lift;
        shiftX = camera->unk74C;
        shiftZ = camera->unk754;
        sideX = sinf(camera->unk3A0 - 1.57079637f) * side;
        sideZ = cosf(camera->unk3A0 - 1.57079637f) * side;
        eye.x = camera->eye.x + shiftX + sideX;
        eye.y = camera->eye.y + lift;
        eye.z = camera->eye.z + shiftZ + sideZ;
        at.x = camera->target.x + shiftX + sideX;
        at.y = camera->target.y + lift;
        at.z = camera->target.z + shiftZ + sideZ;
        // matching: these copies are never read, but the original makes them
        prevEye = camera->eye;
        prevTarget = camera->target;
        camera->eye = eye;
        camera->target = at;
        func_1512523C((struct108 *)camera);
        func_15125594((struct108 *)camera);
    }
    func_150491EC((struct17 *)&eye, (struct17 *)&at, (struct17 *)&dir);
    camera->playerExt->aimTarget = at;
    camera->playerExt->aimEye = eye;
    camera->playerExt->aimDir = dir;
    {
        f32 swayTarget;

        func_15047B80(VIEW_MTX(camera->playerIndex), &D_800D9B90[D_800BE9C0], eye.x, eye.y, eye.z, at.x, at.y, at.z, 0.0f, 1.0f, 0.0f);
        if (camera->mode & 0x80000) {
            func_1512D070(camera);
            func_1512D368(camera);
            return;
        }
        if (camera->mode == 0x40000) {
            func_150A7DF0(&rotMtx, 0.0f, 0.0f, camera->swayAmount);
            guMtxCatL(VIEW_MTX(camera->playerIndex), &rotMtx, VIEW_MTX(camera->playerIndex));
            func_1512D070(camera);
            return;
        }
        if (camera->playerExt->aimState != 0) {
            if (camera->shakeState == 2) {
                CamOffset *tbl;

                tbl = D_800DC280[camera->shakeIndex];
                // matching: index-first addition orders the addu operands as the original (tbl[i] swaps them)
                camera->shakeRot.x += (camera->shakeFrame + tbl)->rot.x;
                camera->shakeRot.y += (camera->shakeFrame + tbl)->rot.y;
                camera->shakeRot.z += (camera->shakeFrame + tbl)->rot.z;
                camera->shakePos.x += (camera->shakeFrame + tbl)->pos.x;
                camera->shakePos.y += (camera->shakeFrame + tbl)->pos.y;
                camera->shakePos.z += (camera->shakeFrame + tbl)->pos.z;
            } else {
                camera->shakeRot.x -= camera->shakeRot.x * 0.08f;
                camera->shakeRot.y -= camera->shakeRot.y * 0.08f;
                camera->shakeRot.z -= camera->shakeRot.z * 0.08f;
                camera->shakePos.x -= camera->shakePos.x * 0.08f;
                camera->shakePos.y -= camera->shakePos.y * 0.08f;
                camera->shakePos.z -= camera->shakePos.z * 0.08f;
            }
            func_1512D2F8(camera);
        } else {
            camera->shakeRot.x -= camera->shakeRot.x * 0.08f;
            camera->shakeRot.y -= camera->shakeRot.y * 0.08f;
            camera->shakeRot.z -= camera->shakeRot.z * 0.08f;
            camera->shakePos.x -= camera->shakePos.x * 0.08f;
            camera->shakePos.y -= camera->shakePos.y * 0.08f;
            camera->shakePos.z -= camera->shakePos.z * 0.08f;
        }
        camera->swayAmount += camera->shakeRot.x * shakeScale;
        func_150A7DF0(&rotMtx, -camera->shakeRot.y * shakeScale, camera->shakeRot.z * shakeScale, camera->shakeRot.x * shakeScale);
        guMtxCatL(VIEW_MTX(camera->playerIndex), &rotMtx, VIEW_MTX(camera->playerIndex));
        if (camera->playerExt != NULL) {
            yaw = func_15048FC8((struct17 *)&camera->playerExt->aimDir) - 180.0f;
            func_150A8050(dirMtx,
                          -func_150484A0(camera->playerExt->aimDir.y, sqrtf(camera->playerExt->aimDir.x * camera->playerExt->aimDir.x + camera->playerExt->aimDir.z * camera->playerExt->aimDir.z)) * 57.2957764f - camera->shakeRot.y * shakeScale,
                          yaw - camera->shakeRot.z * shakeScale, 0.0f);
            guMtxXFMF(dirMtx, 0.0f, 0.0f, 1.0f, &dirX, &dirY, &dirZ);
            camera->playerExt->aimDir.x = dirX;
            camera->playerExt->aimDir.y = dirY;
            camera->playerExt->aimDir.z = dirZ;
        }
        if (camera->playerExt->aimState != 0) {
            func_150A7DF0(&rotMtx, D_800DCDE0[camera->playerIndex].rot.x, D_800DCDE0[camera->playerIndex].rot.y, D_800DCDE0[camera->playerIndex].rot.z);
            func_150A7D00(&transMtx, D_800DCDE0[camera->playerIndex].pos.x, D_800DCDE0[camera->playerIndex].pos.y, D_800DCDE0[camera->playerIndex].pos.z);
            guMtxCatL(&rotMtx, &transMtx, &rotMtx);
            guMtxCatL(VIEW_MTX(camera->playerIndex), &rotMtx, VIEW_MTX(camera->playerIndex));
            return;
        }
        if (((camera->mode & 0x80) || (camera->player->unk102 != 0)) && (camera->playerMoved != 0)) {
            swayTarget = camera->player->unkC4 * 0.25f;
        } else if ((camera->status & 0x80) && (camera->mode != 0x40)) {
            swayTarget = camera->player->unkC4 * 0.5f;
        } else {
            swayTarget = 0.0f;
        }
        if (!(camera->mode & 0x40000)) {
            if ((camera->status & 0x80) || (camera->player->unk102 != 0)) {
                stiffness = 2.0f;
                damping = 4.0f;
            } else {
                stiffness = 4.0f;
                damping = 8.0f;
            }
            func_150495B0(&camera->swayAmount, swayTarget, &camera->swayVel, stiffness, damping, camera->dt);
        }
        func_1512D368(camera);
        if (camera->mode != 0x8000) {
            if (camera->mode == 1) {
                camera->pitch -= camera->unk5D0;
                camera->pitchRad = camera->pitch * 0.0174532924f;
            }
            func_150A7DF0(&rotMtx, D_800DCDE0[camera->playerIndex].rot.x + (camera->unk5D0 + camera->unk3A8 + camera->unk38C),
                          D_800DCDE0[camera->playerIndex].rot.y + camera->unk5E8,
                          (camera->swayAmount + D_800DC320[camera->playerIndex].unk0 * 0.5f) + D_800DCDE0[camera->playerIndex].rot.z);
            if (camera->mode == 0x40000) {
                func_150A7DF0(&rotMtx, 0.0f, 0.0f, camera->swayAmount);
            }
        }
        guMtxCatL(VIEW_MTX(camera->playerIndex), &rotMtx, VIEW_MTX(camera->playerIndex));
        func_1512D070(camera);
        if (!(camera->mode & 0x40000)) {
            func_150A7D00(&transMtx, D_800DC2C0[camera->playerIndex].unk54.x + D_800DCDE0[camera->playerIndex].pos.x, D_800DC2C0[camera->playerIndex].unk54.y + D_800DCDE0[camera->playerIndex].pos.y, D_800DC2C0[camera->playerIndex].unk54.z + D_800DCDE0[camera->playerIndex].pos.z);
            guMtxCatL(VIEW_MTX(camera->playerIndex), &transMtx, VIEW_MTX(camera->playerIndex));
        }
    }
}

/* Applies the camera's breathing wobble: a scale on x and y that oscillates with wobblePhase. */
void func_1512D070(Camera *camera) {
    Mtx mtx;
    f32 mf[4][4];
    f32 amount;

    if (camera->mode & 0x80000) {
        amount = 0.003000000026f;
    } else if (camera->playerMoved != 0) {
        amount = 0.03999999911f;
    } else {
        amount = 0.02999999933f;
    }

    if ((camera->mode & 0x80000) || (camera->status & 1) || (camera->mode != 0x40000 && (camera->status & 8))) {
        camera->wobblePhase += 0.04363323003f * (f32)D_800BE9E4; // 2.5 degrees per frame
        if (camera->wobblePhase > 3.141592741f) {
            camera->wobblePhase -= 6.283185482f;
        }
    } else {
        amount = 0.0f;
    }

    if (camera->snap != 0 || (camera->status & 4)) {
        camera->wobbleAmount = amount;
    } else {
        func_150495B0(&camera->wobbleAmount, amount, &camera->wobbleVel, 0.2f, 0.400000006f, camera->dt);
    }

    guMtxIdentF(mf);
    mf[0][0] = func_150AD78C(camera->wobblePhase) * camera->wobbleAmount * 2.0f + (camera->wobbleAmount + camera->wobbleAmount) + 1.0f;
    mf[1][1] = func_150AD780(camera->wobblePhase) * camera->wobbleAmount + camera->wobbleAmount + 1.0f;
    guMtxF2L(mf, &mtx);
    guMtxCatL(VIEW_MTX(camera->playerIndex), &mtx, VIEW_MTX(camera->playerIndex));
}

/* Loads the four camera shake tables. */
void func_1512D238(void) {
    u32 size;
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800DC280[i] = (CamOffset *)func_1502B5C8((s32 *)&size, 2, 0x1B, i);
        D_800DC290[i] = size / sizeof(CamOffset);
    }
}

/* Starts camera shake number `index`. */
void func_1512D2E4(Camera *camera, s32 index) {
    camera->shakeIndex = index;
    camera->shakeState = 1;
}

/* Advances the camera shake by the frames elapsed, stopping at the end of its table. */
void func_1512D2F8(Camera *camera) {
    u8 frame;

    switch (camera->shakeState) {
    case 1:
        camera->shakeFrame = 0;
        camera->shakeState = 2;
        break;
    case 2:
        frame = camera->shakeFrame + D_800BE9E4;
        camera->shakeFrame = frame;
        // matching: the explicit mask makes IDO store the unmasked sum and re-mask a copy
        if ((frame & 0xFF) >= D_800DC290[camera->shakeIndex]) {
            camera->shakeState = 0;
        }
        break;
    }
}

void func_1512D368(Camera *camera) {
}
