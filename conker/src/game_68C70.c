#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Simulation state of the 5-joint bone chain driven from bones 24-28 (0x50 bytes). */
typedef struct BoneChain {
    /* 0x00 */ f32 pos[5][3];   /* joint world positions, root first */
    /* 0x3C */ f32 bobPhase;    /* degrees, advanced by the walking speed */
    /* 0x40 */ f32 swayAngle;   /* degrees */
    /* 0x44 */ f32 swaySpeed;   /* degrees per frame */
    /* 0x48 */ f32 swayAmp;
    /* 0x4C */ s16 swayTimer;   /* frames until the next idle sway */
    /* 0x4E */ u8 blendTimer;   /* frames left blending back from the animated pose */
} BoneChain;

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);

/* Allocates and initialises the bone chain state of an object. */
void func_1503B7C0(Actor *obj) {
    obj->unk31C->boneChain = allocate_memory(sizeof(BoneChain), 1, 0, 0);
    bzero(obj->unk31C->boneChain, sizeof(BoneChain));
    obj->unk31C->boneChain->swaySpeed = 30.0f;
    obj->unk31C->boneChain->swayTimer = func_150ADA20() % 30U;
}

extern u16 D_8009877C[90];
extern u16 D_80098830[3];

/*
 * Flags in unk2FB whether the object's current animation (unk84) poses the bone chain itself:
 * bit 1 for the animations in D_80098830, bit 0 for those in D_8009877C.
 */
void func_1503B840(Actor *obj) {
    s32 i;

    if (obj->interaction_state == 1) {
        for (i = 0; i < 3; i++) {
            if (obj->unk84.uh == D_80098830[i]) {
                obj->unk2FB |= 2;
                return;
            }
        }
        for (i = 0; i < 90; i++) {
            if (obj->unk84.uh == D_8009877C[i]) {
                obj->unk2FB |= 1;
                return;
            }
        }
    }
}

/* Returns whether object objIndex simulates its bone chain (0 while the animation poses it). */
s32 func_1503B95C(s32 objIndex, BoneChain *chain) {
    s32 flags = D_800CC2D0[objIndex].unk2FB;

    if (flags & 2) {
        chain->blendTimer = 0;
        return 0;
    }
    if (flags & 1) {
        return 0;
    }
    return 1;
}

extern f32 D_800986F0[4]; /* link lengths */
extern f32 D_80098700[4]; /* link lengths for objects 0x80 and 0xB0 */
extern f32 D_80098710[4]; /* per-link x/z follow factors */
extern f32 D_80098720[4]; /* per-link y follow factors */
extern f32 D_80098730[4]; /* per-link bend limits, degrees */
extern f32 D_80098740[5]; /* per-joint lift weights */
extern f32 D_80098754[5]; /* per-joint lift weights for animations 0x1B and 0x18B */
extern f32 D_80098768[5]; /* per-joint lift weights for objects 0x80 and 0xB0 */
extern u8 D_800C3C9D;
extern f32 D_800C4008[3]; /* chain anchor position */

void func_1503CB98(s32 objIndex, s32 mode, f32 blend, BoneChain *chain);
extern f32 sqrtf(f32);
extern f32 sinf(f32);
extern f32 cosf(f32);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

/*
 * Per-frame update of an object's bone chain (bones 24-28). While the animation poses the chain
 * (mode 0) the joints just copy the bones; otherwise the chain trails the root as a rope of fixed
 * link lengths, bobbing with the walking speed, swaying when idle and turning towards the R-trigger
 * camera. Mode 2 blends from the animated pose back to the simulated one. Ends by rebuilding the
 * bone matrices with func_1503CB98.
 */
void func_1503B9BC(s32 objIndex) {
    Actor *obj;
    s32 prev;
    s32 i;
    f32 link[3];
    f32 offsets[5][3];
    u8 *resetMask;
    f32 goal[3];
    f32 cosYaw;
    f32 sinYaw;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 oldX;
    f32 oldY;
    f32 oldZ;
    f32 inv;
    s32 k;
    f32 delta;
    f32 angle;
    f32 *liftWeights;
    f32 yaw;
    s32 facingCamera;
    f32 speed;
    f32 unused1; // matching: unused, keeps the stack frame layout
    f32 blend;
    s32 mode;
    s32 reset;
    f32 bob;
    Camera *cam;
    f32 *linkLen;
    BoneChain *chain;
    s32 anim;
    f32 unused2; // matching: unused, keeps the stack frame layout
    f32 tipBob;
    f32 (*bone)[4];

    dirX = dirY = dirZ = 0.0f; // matching: dead store, steers IDO's FP register order
    obj = &D_800CC2D0[objIndex];
    facingCamera = 0;
    if (obj->unk31C == NULL) {
        return;
    }
    if (obj->unk31C->boneChain == NULL) {
        func_1503B7C0(obj);
    }
    chain = obj->unk31C->boneChain;
    mode = func_1503B95C(objIndex, chain);
    resetMask = &D_800C4000; /* one bit per object: reset the chain once it has bones again */
    if (obj->unk1D4 == NULL) {
        *resetMask |= 1 << objIndex;
        return;
    }
    if (D_800C4000 & (1 << objIndex)) {
        *resetMask = D_800C4000 & ~(1 << objIndex);
        reset = 1;
    } else {
        reset = 0;
    }
    /* any of the four camera hold timers, or byte 0x2FD of the object */
    if (*(s32 *)D_800DBFF4 != 0 || obj->unk2FD != 0) {
        reset = 1;
    }
    if (reset) {
        chain->swaySpeed = 30.0f;
        chain->bobPhase = 0.0f;
        chain->swayAngle = 0.0f;
        chain->swayAmp = 100.0f;
        chain->swayTimer = func_150ADA20() % 30U;
    }
    if (mode == 0) {
        if (D_800BE9E4 < chain->blendTimer) {
            if (D_800BEAC0 == 0) {
                chain->blendTimer -= D_800BE9E4;
            }
            mode = 2;
        } else {
            chain->blendTimer = 0;
        }
    }
    // matching: the loop start below goes through a variable, so IDO unrolls it for any start
    prev = 24;
    /* the chain is anchored at bone 0's position */
    bone = obj->unk1D4[0];
    D_800C4008[0] = bone[3][0];
    D_800C4008[1] = bone[3][1];
    D_800C4008[2] = bone[3][2];
    if (mode == 0) {
        for (k = prev; k < 28; k++) {
            bone = (f32 (*)[4])((u32)obj->unk1D4 + k * 64);
            chain->pos[k - prev][0] = bone[3][0];
            chain->pos[k - prev][1] = bone[3][1];
            chain->pos[k - prev][2] = bone[3][2];
        }
        return;
    }
    bone = obj->unk1D4[24];
    chain->pos[0][0] = bone[3][0];
    chain->pos[0][1] = bone[3][1];
    chain->pos[0][2] = bone[3][2];
    if (obj->id == 0x80 || obj->id == 0xB0) {
        linkLen = D_80098700;
    } else {
        linkLen = D_800986F0;
    }
    for (i = 0; i < 5; i++) {
        offsets[i][0] = 0.0f;
        offsets[i][1] = 0.0f;
        offsets[i][2] = 0.0f;
    }
    speed = obj->xz_velocity;
    speed = CLAMP(speed, 0.0f, 60.0f);
    if (obj->unk28 > 3.0f) {
        speed *= 0.5f;
    }
    bob = sinf(chain->bobPhase * 0.01745329238f) * speed;
    if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
        chain->bobPhase += speed;
        if (chain->bobPhase >= 360.0f) {
            chain->bobPhase -= 360.0f;
        }
    }
    tipBob = sinf((chain->bobPhase + 180.0f) * 0.01745329238f) * speed;
    offsets[0][1] += bob;
    offsets[4][1] += tipBob;
    cam = obj->camera;
    if (cam != NULL && cam->mode == 0x100 && cam->pitch > -17.0f) {
        s32 clamped = 0;
        f32 turn;
        f32 toCamX;
        f32 toCamZ;

        toCamX = (cam->eye.x + cam->aim.offset.x) - obj->x_position;
        toCamZ = (cam->eye.z + cam->aim.offset.z) - obj->z_position;
        angle = func_150484A0(toCamX, toCamZ) * 57.29577637f + 90.0f;
        if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        turn = func_15048A70(obj->unk76 * 0.0054931640625f, angle);
        delta = fabsf(turn);
        if (delta < 90.0f) {
            if (delta < 0.5f) {
                turn = 0.0f;
            }
            turn = CLAMP(turn, -15.0f, 15.0f);
            if (turn > 0.0f) {
                if (turn < 23.0f) {
                    turn = 23.0f;
                    clamped = 1;
                }
            } else if (turn > -23.0f) {
                turn = -23.0f;
                clamped = 1;
            }
            if (clamped) {
                delta = turn - chain->swayAngle;
                turn = chain->swayAngle + delta * 0.6f;
            }
            chain->swayAngle = turn;
            chain->swayTimer = 60;
            chain->swayAmp = 200.0f;
            facingCamera = 1;
        }
    }
    yaw = obj->unk40 * 0.01745329238f;
    if (obj->unk84.uh == 0x122 || obj->unk84.uh == 0x128 || obj->unk84.uh == 0x129 || obj->unk84.uh == 0x12A) {
        yaw -= 0.7853981853f;
    }
    sinYaw = sinf(yaw);
    cosYaw = cosf(yaw);
    anim = D_800CC2D0[objIndex].unk84.uh; // matching: reloaded via the array, not obj
    if (anim == 0x24 || anim == 0x22 || anim == 0xD1) {
        D_800C4008[1] = chain->pos[0][1];
        if (obj->unkB8 > 0.0f) {
            D_800C4008[0] = chain->pos[0][0] + sinYaw;
            D_800C4008[2] = chain->pos[0][2] + cosYaw;
        }
        offsets[0][1] -= bob;
        offsets[1][1] += bob;
    } else if (obj->unk28 < 0.5f) {
        speed = obj->xz_velocity;
        speed = CLAMP(speed, 0.0f, 30.0f);
        bob = 60.0f - (speed + speed);
        if (D_800C35EA[0] == 1 && D_800C3C9D < 100) {
            bob *= D_800C3C9D;
            bob *= 0.01f;
        }
        if (obj->id == 0x80 || obj->id == 0xB0) {
            liftWeights = D_80098768;
        } else if (anim == 0x1B || anim == 0x18B) {
            liftWeights = D_80098754;
            for (i = 0; i < 5; i++) {
                offsets[i][1] = 0.0f;
            }
            bob = 60.0f;
        } else {
            liftWeights = D_80098740;
        }
        if (obj->unk31C->unk78 == 0x3B && cam != NULL && cam->mode == 0x100 && cam->pitch > -17.0f) {
            bob = 0; // matching: an int 0 (0.0f allocates differently)
        }
        offsets[0][1] += bob * liftWeights[0];
        offsets[1][1] += bob * liftWeights[1];
        offsets[2][1] += bob * liftWeights[2];
        offsets[3][1] += bob * liftWeights[3];
        offsets[4][1] += bob * liftWeights[4];
        if (speed == 0.0f) {
            if (chain->swayTimer != 0 && facingCamera == 0) {
                chain->swayTimer -= D_800BE9E4;
                if (chain->swayTimer <= 0) {
                    chain->swayAngle = 0.0f;
                    chain->swayAmp = func_150ADA20() % 100U;
                    bob = func_150ADA20() % 10U + 10;
                    if (func_150ADA20() & 1) {
                        chain->swaySpeed = bob;
                    } else {
                        chain->swaySpeed = -bob;
                    }
                    chain->swayTimer = 0;
                }
            } else {
                bob = sinf(chain->swayAngle * 0.01745329238f) * chain->swayAmp;
                tipBob = sinf((chain->swayAngle + 30.0f) * 0.01745329238f) * chain->swayAmp * 5.0f;
                offsets[4][0] += cosYaw * tipBob;
                offsets[0][0] += cosYaw * bob;
                offsets[4][2] += -sinYaw * tipBob;
                offsets[0][2] += -sinYaw * bob;
                if (D_800BEAC0 == 0 && D_800BEA0C == 0 && facingCamera == 0) {
                    chain->swayAngle += chain->swaySpeed;
                    if (chain->swayAngle > 360.0f || chain->swayAngle < -360.0f) {
                        if (fabsf(chain->swaySpeed) > 15.0f) {
                            chain->swaySpeed *= 0.5f;
                            chain->swayAmp *= 0.5f;
                            if (chain->swayAngle > 360.0f) {
                                chain->swayAngle -= 360.0f;
                            } else {
                                chain->swayAngle += 360.0f;
                            }
                        } else {
                            chain->swayTimer = func_150ADA20() % 200U;
                        }
                    }
                }
            }
        } else if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
            chain->swayTimer = 200;
        }
    }
    if (mode == 2) {
        blend = (f32)(30 - chain->blendTimer) / 30.0f;
    }
    for (i = 1; i < 5; i++) {
        prev = i - 1;
        dirX = chain->pos[prev][0] + offsets[prev][0] * obj->xz_scale;
        dirY = chain->pos[prev][1] + offsets[prev][1] * obj->y_scale;
        dirZ = chain->pos[prev][2] + offsets[prev][2] * obj->xz_scale;
        if (prev == 0) {
            dirX -= D_800C4008[0];
            dirY -= D_800C4008[1];
            dirZ -= D_800C4008[2];
        } else {
            dirX -= chain->pos[prev - 1][0];
            dirY -= chain->pos[prev - 1][1];
            dirZ -= chain->pos[prev - 1][2];
        }
        inv = 1.0f / sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
        dirX *= inv;
        dirY *= inv;
        dirZ *= inv;
        goal[0] = chain->pos[prev][0] + linkLen[prev] * dirX * obj->xz_scale;
        goal[1] = chain->pos[prev][1] + linkLen[prev] * dirY * obj->y_scale;
        goal[2] = chain->pos[prev][2] + linkLen[prev] * dirZ * obj->xz_scale;
        if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
            delta = goal[0] - chain->pos[i][0];
            if (!reset) {
                delta *= D_80098710[prev];
            }
            chain->pos[i][0] += delta;
            delta = goal[1] - chain->pos[i][1];
            if (!reset) {
                delta *= D_80098720[prev];
            }
            chain->pos[i][1] += delta;
            delta = goal[2] - chain->pos[i][2];
            if (!reset) {
                delta *= D_80098710[prev];
            }
            chain->pos[i][2] += delta;
        }
        link[0] = chain->pos[i][0] - chain->pos[prev][0];
        link[1] = chain->pos[i][1] - chain->pos[prev][1];
        link[2] = chain->pos[i][2] - chain->pos[prev][2];
        inv = 1.0f / sqrtf(link[0] * link[0] + link[1] * link[1] + link[2] * link[2]);
        link[0] *= inv;
        link[1] *= inv;
        link[2] *= inv;
        angle = func_15048360(dirX * link[0] + dirY * link[1] + dirZ * link[2]) * 57.29577637f;
        angle = fabsf(angle);
        if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
            if (D_80098730[prev] < angle) {
                inv = D_80098730[prev] / angle;
                inv = 1.0f - inv;
                if (reset) {
                    inv = 1.0f;
                }
                oldX = chain->pos[i][0];
                oldY = chain->pos[i][1];
                oldZ = chain->pos[i][2];
                delta = goal[0] - oldX;
                chain->pos[i][0] += delta * inv;
                delta = goal[1] - oldY;
                chain->pos[i][1] += delta * inv;
                delta = goal[2] - oldZ;
                chain->pos[i][2] += delta * inv;
            }
        }
        link[0] = chain->pos[i][0] - chain->pos[prev][0];
        link[1] = chain->pos[i][1] - chain->pos[prev][1];
        link[2] = chain->pos[i][2] - chain->pos[prev][2];
        inv = 1.0f / sqrtf(link[0] * link[0] + link[1] * link[1] + link[2] * link[2]);
        link[0] *= inv;
        link[1] *= inv;
        link[2] *= inv;
        chain->pos[i][0] = chain->pos[prev][0] + link[0] * linkLen[prev] * obj->xz_scale;
        chain->pos[i][1] = chain->pos[prev][1] + link[1] * linkLen[prev] * obj->y_scale;
        chain->pos[i][2] = chain->pos[prev][2] + link[2] * linkLen[prev] * obj->xz_scale;
    }
    func_1503CB98(objIndex, mode, blend, chain);
}

void func_150440A0(MtxF arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);

/*
 * Rebuilds bone matrices 24-27 so that each one looks from chain joint i to joint i+1, keeping the
 * up axis of the previous bone (of the object's own matrix for the first). In mode 2 the joints are
 * first blended towards the animated pose by `blend`; otherwise the blend timer is rearmed.
 */
void func_1503CB98(s32 objIndex, s32 mode, f32 blend, BoneChain *chain) {
    Actor *obj;
    f32 up[3];
    f32 eyeX, eyeY, eyeZ;
    f32 atX, atY, atZ;
    f32 mag;
    f32 inv;
    s32 i;
    s32 j;
    MtxF rootMtx;
    f32 animPos[5][3];
    s32 boneOffset;
    s32 joint;
    MtxF *bones;
    f32 (*parent)[4];
    f32 (*dst)[4];
    f32 (*pos)[3];
    f32 (*bone)[4];

    obj = &D_800CC2D0[objIndex];
    func_150A9B0C(rootMtx, obj->unkB8, obj->unk40, obj->unkC4, 1.0f, 1.0f, 1.0f);
    joint = 0;
    boneOffset = 24 * sizeof(MtxF);

    // matching: separate i/joint counters; merging them reallocates the loop registers
    i = 0;
    do {
        pos = &chain->pos[joint];
        // matching: a byte offset; indexing bones[24 + i] swaps the registers of i and the offset
        dst = (f32 (*)[4])((u8 *)obj->unk1D4 + boneOffset);
        bones = obj->unk1D4;
        if (i == 0) {
            parent = rootMtx;
        } else {
            parent = (f32 (*)[4])((u8 *)bones + boneOffset - sizeof(MtxF));
        }
        up[0] = parent[1][0];
        up[1] = parent[1][1];
        up[2] = parent[1][2];
        mag = sqrtf(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
        if (mag != 0.0f) {
            inv = 1.0f / mag;
            up[0] *= inv;
            up[1] *= inv;
            up[2] *= inv;
        }
        eyeX = pos[0][0];
        eyeY = pos[0][1];
        eyeZ = pos[0][2];
        atX = pos[1][0];
        atY = pos[1][1];
        atZ = pos[1][2];
        if (mode == 2) {
            if (i == 0) {
                // matching: bones[24 + j][3] folds the 0x600 into the loads; this keeps it in the base
                bone = (f32 (*)[4])&bones[0];
                for (j = 0; j < 4; j++) {
                    bone += 24 * 4;
                    animPos[j][0] = bone[3][0];
                    animPos[j][1] = bone[3][1];
                    animPos[j][2] = bone[3][2];
                    bone = (f32 (*)[4])&bones[j + 1];
                }
                animPos[4][0] = (animPos[3][0] - animPos[2][0]) + animPos[3][0];
                animPos[4][1] = (animPos[3][1] - animPos[2][1]) + animPos[3][1];
                animPos[4][2] = (animPos[3][2] - animPos[2][2]) + animPos[3][2];
            }
            eyeX += (animPos[joint][0] - eyeX) * blend;
            eyeY += (animPos[joint][1] - eyeY) * blend;
            eyeZ += (animPos[joint][2] - eyeZ) * blend;
            atX += (animPos[joint + 1][0] - atX) * blend;
            atY += (animPos[joint + 1][1] - atY) * blend;
            atZ += (animPos[joint + 1][2] - atZ) * blend;
        }
        if (eyeX == atX && eyeZ == atZ) {
            eyeX += 1.0f;
        }
        func_150440A0(dst, eyeX, eyeY, eyeZ, atX, atY, atZ, up[0], up[1], up[2]);
        func_15043EC8(dst, obj->xz_scale, obj->y_scale, obj->xz_scale, eyeX, eyeY, eyeZ);
        boneOffset += sizeof(MtxF);
        i++;
        joint++;
        // matching: the pointer compare (not joint < 4 or a for loop) gives IDO's loop test
    } while (&chain->pos[joint] < &chain->pos[4]);

    if (mode != 2) {
        chain->blendTimer = 30;
    }
}

