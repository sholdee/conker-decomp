#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Simulation state of the 5-joint bone chain driven from bones 24-28 (0x50 bytes). */
typedef struct {
    /* 0x00 */ f32 pos[5][3];   /* joint world positions, root first */
    /* 0x3C */ f32 bobPhase;    /* degrees, advanced by the walking speed */
    /* 0x40 */ f32 swayAngle;   /* degrees */
    /* 0x44 */ f32 swaySpeed;   /* degrees per frame */
    /* 0x48 */ f32 swayAmp;
    /* 0x4C */ s16 swayTimer;   /* frames until the next idle sway */
    /* 0x4E */ u8 blendTimer;   /* frames left blending back from the animated pose */
} BoneChain;

/* The part of the object extension (struct127.unk31C) used here. */
typedef struct {
    /* 0x000 */ u8 pad0[0x78];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[0x11C - 0x79];
    /* 0x11C */ BoneChain *chain;
} ChainObjExt;

#define CHAIN_EXT(obj) ((ChainObjExt *)(obj)->unk31C)

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);

/* Allocates and initialises the bone chain state of an object. */
void func_1503B7C0(struct127 *obj) {
    CHAIN_EXT(obj)->chain = allocate_memory(sizeof(BoneChain), 1, 0, 0);
    bzero(CHAIN_EXT(obj)->chain, sizeof(BoneChain));
    CHAIN_EXT(obj)->chain->swaySpeed = 30.0f;
    CHAIN_EXT(obj)->chain->swayTimer = func_150ADA20() % 30U;
}

extern u16 D_8009877C[90];
extern u16 D_80098830[3];

/*
 * Flags in unk2FB whether the object's current animation (unk84) poses the bone chain itself:
 * bit 1 for the animations in D_80098830, bit 0 for those in D_8009877C.
 */
void func_1503B840(struct127 *obj) {
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

typedef struct {
    f32 pos[5][3];
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    u8 unk4E;
} func_1503B9BC_data;

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
    u8 pad79[0x11C - 0x79];
    func_1503B9BC_data *unk11C;
} func_1503B9BC_inner;

typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;
    u8 pad30[0x2F8 - 0x30];
    f32 unk2F8;
    f32 unk2FC;
    f32 unk300;
    u8 pad304[0x388 - 0x304];
    f32 unk388;
    u8 pad38C[0x74C - 0x38C];
    f32 unk74C;
    f32 unk750;
    f32 unk754;
} func_1503B9BC_cam;

extern f32 D_800986F0[4];
extern f32 D_80098700[4];
extern f32 D_80098710[4];
extern f32 D_80098720[4];
extern f32 D_80098730[4];
extern f32 D_80098740[5];
extern f32 D_80098754[5];
extern f32 D_80098768[5];
extern u8 D_800BEA0C;
extern u8 D_800C3C9D;
extern f32 D_800C4008[3];
extern f32 D_800C4010;
typedef f32 MtxF[4][4]; /* one bone matrix; struct127.unk1D4 points at an array of them */

void func_1503CB98(s32 objIndex, s32 mode, f32 blend, BoneChain *chain);
extern f32 sqrtf(f32);
extern f32 sinf(f32);
extern f32 cosf(f32);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

void func_1503B9BC(s32 arg0) {
    struct127 *obj;
    s32 j;
    s32 i;
    f32 vec[3];
    f32 offs[5][3];
    u8 *bits;
    f32 target[3];
    f32 c;
    f32 s;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 x;
    f32 y;
    f32 z;
    f32 inv;
    s32 k;
    f32 t;
    f32 ang;
    f32 *wtab;
    f32 yaw;
    s32 flag;
    f32 speed;
    f32 amp;
    f32 ratio;
    s32 mode;
    s32 reset;
    f32 bob;
    func_1503B9BC_cam *cam;
    f32 *table;
    func_1503B9BC_data *data;
    s32 anim;
    f32 a;
    f32 b;
    f32 (*m)[4];

    dx = dy = dz = 0.0f;
    obj = &D_800CC2D0[arg0];
    flag = 0;
    if (obj->unk31C != NULL) {
        if (((func_1503B9BC_inner *)obj->unk31C)->unk11C == NULL) {
            func_1503B7C0(obj);
        }
        data = ((func_1503B9BC_inner *)obj->unk31C)->unk11C;
        mode = func_1503B95C(arg0, (u8 *)data);
        bits = &D_800C4000;
        if (obj->unk1D4 == NULL) {
            *bits |= 1 << arg0;
            return;
        }
        if (D_800C4000 & (1 << arg0)) {
            *bits = D_800C4000 & ~(1 << arg0);
            reset = 1;
        } else {
            reset = 0;
        }
        if (*(s32 *)D_800DBFF4 != 0 || obj->pad2FC[1] != 0) {
            reset = 1;
        }
        if (reset) {
            data->unk44 = 30.0f;
            data->unk3C = 0.0f;
            data->unk40 = 0.0f;
            data->unk48 = 100.0f;
            data->unk4C = func_150ADA20() % 30U;
        }
        if (mode == 0) {
            if (D_800BE9E4 < data->unk4E) {
                if (D_800BEAC0 == 0) {
                    data->unk4E -= D_800BE9E4;
                }
                mode = 2;
            } else {
                data->unk4E = 0;
            }
        }
        j = 24;
        m = ((f32 (*)[4][4])obj->unk1D4)[0];
        D_800C4008[0] = m[3][0];
        D_800C4008[1] = m[3][1];
        D_800C4008[2] = m[3][2];
        if (mode == 0) {
            for (k = j; k < 28; k++) {
                m = (f32 (*)[4])((u32)obj->unk1D4 + k * 64);
                data->pos[k - j][0] = m[3][0];
                data->pos[k - j][1] = m[3][1];
                data->pos[k - j][2] = m[3][2];
            }
            return;
        }
        m = ((f32 (*)[4][4])obj->unk1D4)[24];
        data->pos[0][0] = m[3][0];
        data->pos[0][1] = m[3][1];
        data->pos[0][2] = m[3][2];
        if (obj->id == 0x80 || obj->id == 0xB0) {
            table = D_80098700;
        } else {
            table = D_800986F0;
        }
        for (i = 0; i < 5; i++) {
            offs[i][0] = 0.0f;
            offs[i][1] = 0.0f;
            offs[i][2] = 0.0f;
        }
        speed = obj->xz_velocity;
        speed = CLAMP(speed, 0.0f, 60.0f);
        if (obj->unk28 > 3.0f) {
            speed *= 0.5f;
        }
        bob = sinf(data->unk3C * 0.01745329238f) * speed;
        if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
            data->unk3C += speed;
            if (data->unk3C >= 360.0f) {
                data->unk3C -= 360.0f;
            }
        }
        b = sinf((data->unk3C + 180.0f) * 0.01745329238f) * speed;
        offs[0][1] += bob;
        offs[4][1] += b;
        cam = (func_1503B9BC_cam *)obj->camera;
        if (cam != NULL && cam->unk2C == 0x100 && cam->unk388 > -17.0f) {
            s32 flag2 = 0;
            f32 diff;
            f32 cx;
            f32 cz;

            cx = (cam->unk2F8 + cam->unk74C) - obj->x_position;
            cz = (cam->unk300 + cam->unk754) - obj->z_position;
            ang = func_150484A0(cx, cz) * 57.29577637f + 90.0f;
            if (ang >= 360.0f) {
                ang -= 360.0f;
            }
            diff = func_15048A70(obj->unk76 * 0.0054931640625f, ang);
            t = fabsf(diff);
            if (t < 90.0f) {
                if (t < 0.5f) {
                    diff = 0.0f;
                }
                diff = CLAMP(diff, -15.0f, 15.0f);
                if (diff > 0.0f) {
                    if (diff < 23.0f) {
                        diff = 23.0f;
                        flag2 = 1;
                    }
                } else if (diff > -23.0f) {
                    diff = -23.0f;
                    flag2 = 1;
                }
                if (flag2) {
                    t = diff - data->unk40;
                    diff = data->unk40 + t * 0.6000000238f;
                }
                data->unk40 = diff;
                data->unk4C = 60;
                data->unk48 = 200.0f;
                flag = 1;
            }
        }
        yaw = obj->unk40 * 0.01745329238f;
        if (obj->unk84.uh == 0x122 || obj->unk84.uh == 0x128 || obj->unk84.uh == 0x129 || obj->unk84.uh == 0x12A) {
            yaw -= 0.7853981853f;
        }
        s = sinf(yaw);
        c = cosf(yaw);
        anim = D_800CC2D0[arg0].unk84.uh;
        if (anim == 0x24 || anim == 0x22 || anim == 0xD1) {
            D_800C4008[1] = data->pos[0][1];
            if (obj->unkB8 > 0.0f) {
                D_800C4008[0] = data->pos[0][0] + s;
                D_800C4010 = data->pos[0][2] + c;
            }
            offs[0][1] -= bob;
            offs[1][1] += bob;
        } else if (obj->unk28 < 0.5f) {
            speed = obj->xz_velocity;
            speed = CLAMP(speed, 0.0f, 30.0f);
            bob = 60.0f - (speed + speed);
            if (D_800C35EA == 1 && D_800C3C9D < 100) {
                bob *= D_800C3C9D;
                bob *= 0.009999999776f;
            }
            if (obj->id == 0x80 || obj->id == 0xB0) {
                wtab = D_80098768;
            } else if (anim == 0x1B || anim == 0x18B) {
                wtab = D_80098754;
                for (i = 0; i < 5; i++) {
                    offs[i][1] = 0.0f;
                }
                bob = 60.0f;
            } else {
                wtab = D_80098740;
            }
            if (((func_1503B9BC_inner *)obj->unk31C)->unk78 == 0x3B && cam != NULL && cam->unk2C == 0x100 && cam->unk388 > -17.0f) {
                bob = 0;
            }
            offs[0][1] += bob * wtab[0];
            offs[1][1] += bob * wtab[1];
            offs[2][1] += bob * wtab[2];
            offs[3][1] += bob * wtab[3];
            offs[4][1] += bob * wtab[4];
            if (speed == 0.0f) {
                if (data->unk4C != 0 && flag == 0) {
                    data->unk4C -= D_800BE9E4;
                    if (data->unk4C <= 0) {
                        data->unk40 = 0.0f;
                        data->unk48 = func_150ADA20() % 100U;
                        bob = func_150ADA20() % 10U + 10;
                        if (func_150ADA20() & 1) {
                            data->unk44 = bob;
                        } else {
                            data->unk44 = -bob;
                        }
                        data->unk4C = 0;
                    }
                } else {
                    bob = sinf(data->unk40 * 0.01745329238f) * data->unk48;
                    b = sinf((data->unk40 + 30.0f) * 0.01745329238f) * data->unk48 * 5.0f;
                    offs[4][0] += c * b;
                    offs[0][0] += c * bob;
                    offs[4][2] += -s * b;
                    offs[0][2] += -s * bob;
                    if (D_800BEAC0 == 0 && D_800BEA0C == 0 && flag == 0) {
                        data->unk40 += data->unk44;
                        if (data->unk40 > 360.0f || data->unk40 < -360.0f) {
                            if (fabsf(data->unk44) > 15.0f) {
                                data->unk44 *= 0.5f;
                                data->unk48 *= 0.5f;
                                if (data->unk40 > 360.0f) {
                                    data->unk40 -= 360.0f;
                                } else {
                                    data->unk40 += 360.0f;
                                }
                            } else {
                                data->unk4C = func_150ADA20() % 200U;
                            }
                        }
                    }
                }
            } else if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
                data->unk4C = 200;
            }
        }
        if (mode == 2) {
            ratio = (f32)(30 - data->unk4E) / 30.0f;
        }
        for (i = 1; i < 5; i++) {
            j = i - 1;
            dx = data->pos[i - 1][0] + offs[i - 1][0] * obj->xz_scale;
            dy = data->pos[i - 1][1] + offs[i - 1][1] * obj->y_scale;
            dz = data->pos[i - 1][2] + offs[i - 1][2] * obj->xz_scale;
            if (j == 0) {
                dx -= D_800C4008[0];
                dy -= D_800C4008[1];
                dz -= D_800C4008[2];
            } else {
                dx -= data->pos[j - 1][0];
                dy -= data->pos[j - 1][1];
                dz -= data->pos[j - 1][2];
            }
            inv = 1.0f / sqrtf(dx * dx + dy * dy + dz * dz);
            dx *= inv;
            dy *= inv;
            dz *= inv;
            target[0] = data->pos[j][0] + table[j] * dx * obj->xz_scale;
            target[1] = data->pos[j][1] + table[j] * dy * obj->y_scale;
            target[2] = data->pos[j][2] + table[j] * dz * obj->xz_scale;
            if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
                t = target[0] - data->pos[i][0];
                if (!reset) {
                    t *= D_80098710[j];
                }
                data->pos[i][0] += t;
                t = target[1] - data->pos[i][1];
                if (!reset) {
                    t *= D_80098720[j];
                }
                data->pos[i][1] += t;
                t = target[2] - data->pos[i][2];
                if (!reset) {
                    t *= D_80098710[j];
                }
                data->pos[i][2] += t;
            }
            vec[0] = data->pos[i][0] - data->pos[j][0];
            vec[1] = data->pos[i][1] - data->pos[j][1];
            vec[2] = data->pos[i][2] - data->pos[j][2];
            inv = 1.0f / sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
            vec[0] *= inv;
            vec[1] *= inv;
            vec[2] *= inv;
            ang = func_15048360(dx * vec[0] + dy * vec[1] + dz * vec[2]) * 57.29577637f;
            ang = fabsf(ang);
            if (D_800BEAC0 == 0 && D_800BEA0C == 0) {
                if (D_80098730[j] < ang) {
                    inv = D_80098730[j] / ang;
                    inv = 1.0f - inv;
                    if (reset) {
                        inv = 1.0f;
                    }
                    x = data->pos[i][0];
                    y = data->pos[i][1];
                    z = data->pos[i][2];
                    t = target[0] - x;
                    data->pos[i][0] += t * inv;
                    t = target[1] - y;
                    data->pos[i][1] += t * inv;
                    t = target[2] - z;
                    data->pos[i][2] += t * inv;
                }
            }
            vec[0] = data->pos[i][0] - data->pos[j][0];
            vec[1] = data->pos[i][1] - data->pos[j][1];
            vec[2] = data->pos[i][2] - data->pos[j][2];
            inv = 1.0f / sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
            vec[0] *= inv;
            vec[1] *= inv;
            vec[2] *= inv;
            data->pos[i][0] = data->pos[j][0] + vec[0] * table[j] * obj->xz_scale;
            data->pos[i][1] = data->pos[j][1] + vec[1] * table[j] * obj->y_scale;
            data->pos[i][2] = data->pos[j][2] + vec[2] * table[j] * obj->xz_scale;
        }
        func_1503CB98(arg0, mode, ratio, data);
    }
}

void func_150440A0(f32 arg0[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);

/*
 * Rebuilds bone matrices 24-27 so that each one looks from chain joint i to joint i+1, keeping the
 * up axis of the previous bone (of the object's own matrix for the first). In mode 2 the joints are
 * first blended towards the animated pose by `blend`; otherwise the blend timer is rearmed.
 */
void func_1503CB98(s32 objIndex, s32 mode, f32 blend, BoneChain *chain) {
    struct127 *obj;
    f32 up[3];
    f32 eyeX, eyeY, eyeZ;
    f32 atX, atY, atZ;
    f32 mag;
    f32 inv;
    s32 i;
    s32 j;
    f32 rootMtx[4][4];
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
        bones = (MtxF *)obj->unk1D4;
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









































