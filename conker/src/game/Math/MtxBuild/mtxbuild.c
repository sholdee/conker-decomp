#include <ultra64.h>

#include <math.h>
#include "conker/collision.h"
#include "conker/math.h"

/* The calls to sqrtf in this file are real calls (jal sqrtf), not the inline sqrt.s of gu.h's #pragma intrinsic. */
#pragma function sqrtf

/* Builds an Mtx from a rotation (math_mtxRotateRPYF), per-axis scale and translation. */
void math_mtxRotScaleTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z) {
    MtxF mtx;

    math_mtxRotateRPYF(mtx, rotX, rotY, rotZ);
    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;
    mtx[0][0] *= scaleX;
    mtx[0][1] *= scaleX;
    mtx[0][2] *= scaleX;
    mtx[1][0] *= scaleY;
    mtx[1][1] *= scaleY;
    mtx[1][2] *= scaleY;
    mtx[2][0] *= scaleZ;
    mtx[2][1] *= scaleZ;
    mtx[2][2] *= scaleZ;
    guMtxF2L(mtx, m);
}

/* Builds an Mtx from a rotation (math_mtxRotateRPYF) and translation. */
void math_mtxRotTrans(Mtx *m, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
    MtxF mtx;

    math_mtxRotateRPYF(mtx, rotX, rotY, rotZ);
    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;
    guMtxF2L(mtx, m);
}

/* Sets mtx's translation and scales its three basis rows by x, y and z. */
void math_mtxSetScaleTrans(MtxF mtx, f32 x, f32 y, f32 z, f32 transX, f32 transY, f32 transZ) {
    mtx[3][0] = transX;
    mtx[3][1] = transY;
    mtx[3][2] = transZ;
    mtx[0][0] *= x;
    mtx[0][1] *= x;
    mtx[0][2] *= x;
    mtx[1][0] *= y;
    mtx[1][1] *= y;
    mtx[1][2] *= y;
    mtx[2][0] *= z;
    mtx[2][1] *= z;
    mtx[2][2] *= z;
}

/* Builds a rotate-scale matrix (math_mtxRotScaleF) with translation x, y, z and a clean last column. */
void math_mtxFRotScaleTrans(MtxF mtx, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ, f32 x, f32 y, f32 z) {
    math_mtxRotScaleF(mtx, rotX, rotY, rotZ, scaleX, scaleY, scaleZ);
    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
}

/* Reads the translation of a fixed-point matrix: the integer halves of row 3 (s16 12..14), plus their
 * fractions (s16 28..30). */
void math_mtxGetTranslation(Vec3f *out, Mtx *mtx) {
    out->x = ((s16 *)mtx)[12];
    out->y = ((s16 *)mtx)[13];
    out->z = ((s16 *)mtx)[14];
    out->x += ((s16 *)mtx)[28] * (1.0f / 65536.0f);
    out->y += ((s16 *)mtx)[29] * (1.0f / 65536.0f);
    out->z += ((s16 *)mtx)[30] * (1.0f / 65536.0f);
}

/*
 * Builds a look-at orientation from eye, target and up: columns hold the right, up and look axes
 * (look is the negated eye->target direction, as in guLookAtF), and the translation row is the eye itself.
 */
void math_mtxLookAt(MtxF mtx, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {
    f32 lookX;
    f32 lookY;
    f32 lookZ;
    f32 rightX;
    f32 rightY;
    f32 rightZ;
    f32 invLen;

    lookX = atX - eyeX;
    lookY = atY - eyeY;
    lookZ = atZ - eyeZ;
    invLen = -1.0f / sqrtf((lookX * lookX) + (lookY * lookY) + (lookZ * lookZ));
    lookX *= invLen;
    lookY *= invLen;
    lookZ *= invLen;

    rightX = (upY * lookZ) - (upZ * lookY);
    rightY = (upZ * lookX) - (upX * lookZ);
    rightZ = (upX * lookY) - (upY * lookX);
    invLen = 1.0f / sqrtf((rightX * rightX) + (rightY * rightY) + (rightZ * rightZ));
    rightX *= invLen;
    rightY *= invLen;
    rightZ *= invLen;

    upX = (lookY * rightZ) - (lookZ * rightY);
    upY = (lookZ * rightX) - (lookX * rightZ);
    upZ = (lookX * rightY) - (lookY * rightX);
    invLen = 1.0f / sqrtf((upX * upX) + (upY * upY) + (upZ * upZ));
    upX *= invLen;
    upY *= invLen;
    upZ *= invLen;

    mtx[0][0] = rightX;
    mtx[1][0] = upX;
    mtx[2][0] = lookX;
    mtx[3][0] = eyeX;
    mtx[0][1] = rightY;
    mtx[1][1] = upY;
    mtx[2][1] = lookY;
    mtx[3][1] = eyeY;
    mtx[0][2] = rightZ;
    mtx[1][2] = upZ;
    mtx[2][2] = lookZ;
    mtx[3][2] = eyeZ;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
}
