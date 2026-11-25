#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_1503E5F8(MtxF mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ,
                          f32 *scaleX, f32 *scaleY, f32 *scaleZ);
extern Actor *func_15073118(Actor *parent, s32 bone, s32 anim, f32 offX, f32 offY, f32 offZ,
                                f32 yawOffset, f32 speed, f32 yVelocity, f32 gravity, s32 unk83);

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ Mtx *mtx; // one per frame buffer, indexed by D_800BE9C0
} Hook150C8730;

/*
 * While the actor is in state 0x14 past animation frame 10, or in state 0x23, spawns a projectile
 * (animation 0x4E) placed and rotated by the hook's matrix from the other frame buffer.
 * Returns 1 when it spawned one.
 */
s32 func_150C8730(Hook150C8730 *hook, Actor *actor) {
    struct197 *anim;
    s32 state;
    Actor *obj;
    u8 frame;
    Mtx *mtx;
    s32 pad[2]; // matching: unused stack space
    MtxF mf;
    f32 x;
    f32 y;
    f32 z;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    s16 yaw;

    anim = actor->unk2D0;
    if (anim == NULL) {
        return 0;
    }

    state = actor->unk84.uh;
    if ((state == 0x14 && 10.0f < anim->unk8) || state == 0x23) {
        frame = D_800BE9C0;
        mtx = hook->mtx;
        mtx += frame == 0; // matching: &hook->mtx[frame == 0] allocates t-registers instead of v0/a1
        guMtxL2F(mf, mtx);
        mf[0][3] = 0.0f;
        mf[1][3] = 0.0f;
        mf[2][3] = 0.0f;
        mf[3][3] = 1.0f;
        func_1503E5F8(mf, &x, &y, &z, &rotX, &rotY, &rotZ, &scaleX, &scaleY, &scaleZ);

        obj = func_15073118(actor, -1, 0x4E, 0.0f, 0.0f, 0.0f, 0.0f, 42.79999924f, 49.90000153f, 2.900000095f, 200);
        obj->x_position = x;
        obj->y_position = y;
        obj->z_position = z;
        obj->unkB8 = rotX;
        obj->unk40 = rotY;
        obj->unkC4 = rotZ;
        yaw = (obj->unk40 - 90.0f) * 182.0444489f;
        obj->unk76 = yaw;
        obj->unk7A = yaw;
        return 1;
    }

    return 0;
}
