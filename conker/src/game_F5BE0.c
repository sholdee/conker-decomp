#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Model attachment hook 10 (gAttachHooks[10]). While the actor is in state 0x14 past animation frame 10,
 * or in state 0x23, spawns a projectile (animation 0x4E) placed and rotated by the attachment's matrix
 * from the other frame buffer.
 * Returns 1 when it spawned one.
 */
s32 attach_hookSpawnProjectile(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim;
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

    state = actor->animseq.uh;
    if ((state == 0x14 && 10.0f < anim->frame[0]) || state == 0x23) {
        frame = D_800BE9C0;
        mtx = att->mtx;
        mtx += frame == 0; // matching: &att->mtx[frame == 0] allocates t-registers instead of v0/a1
        guMtxL2F(mf, mtx);
        mf[0][3] = 0.0f;
        mf[1][3] = 0.0f;
        mf[2][3] = 0.0f;
        mf[3][3] = 1.0f;
        model_decomposeMatrix(mf, &x, &y, &z, &rotX, &rotY, &rotZ, &scaleX, &scaleY, &scaleZ);

        obj = event_spawnAnimProjectile(actor, -1, 0x4E, 0.0f, 0.0f, 0.0f, 0.0f, 42.79999924f, 49.90000153f, 2.900000095f, 200);
        obj->xpos = x;
        obj->ypos = y;
        obj->zpos = z;
        obj->vtilt = rotX;
        obj->theta = rotY;
        obj->lean_angle = rotZ;
        yaw = (obj->theta - 90.0f) * 182.0444489f;
        obj->angle = yaw;
        obj->langle = yaw;
        return 1;
    }

    return 0;
}
