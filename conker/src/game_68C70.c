#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 pad0[0x44];
    f32 unk44;
    u8 pad48[0x4];
    s16 unk4C;
} func_1503B7C0_sub;

typedef struct {
    u8 pad0[0x11C];
    func_1503B7C0_sub *unk11C;
} func_1503B7C0_inner;

void func_1503B7C0(struct127 *arg0) {
    ((func_1503B7C0_inner *)arg0->unk31C)->unk11C = allocate_memory(0x50, 1, 0, 0);
    bzero(((func_1503B7C0_inner *)arg0->unk31C)->unk11C, 0x50);
    ((func_1503B7C0_inner *)arg0->unk31C)->unk11C->unk44 = 30.0f;
    ((func_1503B7C0_inner *)arg0->unk31C)->unk11C->unk4C = func_150ADA20() % 30U;
}

extern u16 D_8009877C[90];
extern u16 D_80098830[3];

void func_1503B840(struct127 *arg0) {
    s32 i;

    if (arg0->interaction_state == 1) {
        for (i = 0; i < 3; i++) {
            if (arg0->unk84.uh == D_80098830[i]) {
                arg0->unk2FB |= 2;
                return;
            }
        }
        for (i = 0; i < 90; i++) {
            if (arg0->unk84.uh == D_8009877C[i]) {
                arg0->unk2FB |= 1;
                return;
            }
        }
    }
}

extern u8 D_800CC5CB[];

s32 func_1503B95C(s32 arg0, u8 *arg1) {
    s32 v = D_800CC5CB[arg0 * 812];
    if (v & 2) {
        arg1[0x4E] = 0;
        return 0;
    }
    if (v & 1) {
        return 0;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_68C70/func_1503B9BC.s")

void func_150440A0(f32 arg0[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);

void func_1503CB98(s32 arg0, s32 arg1, f32 arg2, f32 (*arg3)[3]) {
    struct127 *obj;
    f32 up[3];
    f32 eyeX, eyeY, eyeZ;
    f32 atX, atY, atZ;
    f32 mag;
    f32 inv;
    s32 i;
    s32 j;
    f32 mtx[4][4];
    f32 pts[5][3];
    s32 k;
    s32 off;
    f32 (*bones)[4][4];
    f32 (*m)[4];
    f32 (*dst)[4];
    f32 (*p)[3];
    f32 (*n)[4];

    obj = &D_800CC2D0[arg0];
    func_150A9B0C(mtx, obj->unkB8, obj->unk40, obj->unkC4, 1.0f, 1.0f, 1.0f);
    off = 0;
    k = 0x600;

    i = 0;
    do {
        p = &arg3[off];
        dst = (f32 (*)[4])((u8 *)obj->unk1D4 + k);
        bones = (f32 (*)[4][4])obj->unk1D4;
        if (i == 0) {
            m = mtx;
        } else {
            m = (f32 (*)[4])((u8 *)bones + k - 0x40);
        }
        up[0] = m[1][0];
        up[1] = m[1][1];
        up[2] = m[1][2];
        mag = sqrtf(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
        if (mag != 0.0f) {
            inv = 1.0f / mag;
            up[0] *= inv;
            up[1] *= inv;
            up[2] *= inv;
        }
        eyeX = p[0][0];
        eyeY = p[0][1];
        eyeZ = p[0][2];
        atX = p[1][0];
        atY = p[1][1];
        atZ = p[1][2];
        if (arg1 == 2) {

            if (i == 0) {
                n = (f32 (*)[4])bones;
                for (j = 0; j < 4; j++) {
                    n += 0x60;
                    pts[j][0] = n[3][0];
                    pts[j][1] = n[3][1];
                    pts[j][2] = n[3][2];
                    n = (f32 (*)[4])((u8 *)bones + ((j + 1) << 6));
                }
                pts[4][0] = (pts[3][0] - pts[2][0]) + pts[3][0];
                pts[4][1] = (pts[3][1] - pts[2][1]) + pts[3][1];
                pts[4][2] = (pts[3][2] - pts[2][2]) + pts[3][2];
            }
            eyeX += ((pts + off)[0][0] - eyeX) * arg2;
            eyeY += ((pts + off)[0][1] - eyeY) * arg2;
            eyeZ += ((pts + off)[0][2] - eyeZ) * arg2;
            atX += ((pts + off)[1][0] - atX) * arg2;
            atY += ((pts + off)[1][1] - atY) * arg2;
            atZ += ((pts + off)[1][2] - atZ) * arg2;
        }
        if (eyeX == atX && eyeZ == atZ) {
            eyeX += 1.0f;
        }
        func_150440A0(dst, eyeX, eyeY, eyeZ, atX, atY, atZ, up[0], up[1], up[2]);
        func_15043EC8(dst, obj->xz_scale, obj->y_scale, obj->xz_scale, eyeX, eyeY, eyeZ);
        k += 0x40;
        i++;
        off++;

    } while (&arg3[off] < &arg3[4]);
    if (arg1 != 2) {
        ((u8 *)arg3)[0x4E] = 0x1E;
    }
}
